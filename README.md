# Projet tutoré : Librairie générique Graphes C++

Projet COO1 — Polytech Tours 3A DI  
Leo NOUHOUANG · Rebson Dodji DAHOUEDE

***

## Prérequis & compilation

- **Environnement** : Windows, compilé sous Visual Studio 2022 en mode Release x64.
- **Langage** : C++ (norme C++17), sans bibliothèque tierce.
- **Genericité** : Les classes `CGraphOrient` et `CGraph` sont templatées sur le type de sommet et le type d'arc, avec des `static_assert` garantissant que ces types dérivent bien de `CSommet` et `CArc`.
- **Architecture** : Hiérarchie d'héritage stricte — `CGraph` hérite de `CGraphOrient`, qui est seul responsable de la cohérence interne du graphe. Les méthodes structurellement sensibles de `CSommet` sont déclarées `protected` et accessibles uniquement via un mécanisme d'amitié avec `CGraphOrient`.
- **Gestion de la mémoire** : Tous les sommets et arcs sont alloués dynamiquement et libérés par les destructeurs des classes.
- **Gestion des erreurs** : Toutes les préconditions sont vérifiées et signalées via la classe `CException` avec des codes d'erreur identifiés par des constantes `#define` propres à chaque classe.


Ouvrir la solution `.sln` dans Visual Studio, sélectionner la configuration **Release x64**, puis compiler.

***

## Structure des fichiers

```
├── CArc.h               Classe de base des arcs
├── CSommet.h            Classe de base des sommets
├── CGraphOrient.h       Graphe orienté générique (template)
├── CGraph.h             Graphe non orienté (hérite de CGraphOrient)
├── CException.h         Classe d'exception maison
├── GraphFileReader.h    Lecteur de fichier texte
├── main.cpp             Tests unitaires + scénario principal
└── graph.txt            Exemple de fichier graphe
```

***

## Format du fichier d'entrée

Le fichier texte attendu par `GraphFileReader` doit respecter strictement ce format :

```
NBSommets=<n>
NBArcs=<m>
Sommets=[
Numero=<id>
...
]
Arcs=[
Debut=<id_depart>, Fin=<id_arrivee>
...
]
```

**Exemple :**
```
NBSommets=3
NBArcs=2
Sommets=[
Numero=1
Numero=2
Numero=3
]
Arcs=[
Debut=1, Fin=2
Debut=2, Fin=3
]
```

> Le nombre de `Numero=` doit correspondre exactement à `NBSommets`, et le nombre de lignes `Debut=..., Fin=...` doit correspondre exactement à `NBArcs`. Tout écart lève une `CException`.

***

## Utilisation rapide

### Créer et manipuler un graphe orienté

```cpp
#include "CGraphOrient.h"

CGraphOrient<CSommet, CArc> graphe;

graphe.GROAjouterSommet(1);
graphe.GROAjouterSommet(2);
graphe.GROAjouterSommet(3);

graphe.GROAjouterArc(1, 2);
graphe.GROAjouterArc(2, 3);

graphe.GROAfficher();

// Graphe inverse (l'appelant est responsable du delete)
CGraphOrient<CSommet, CArc>* pInverse = graphe.GROInverser();
pInverse->GROAfficher();
delete pInverse;
```

### Créer un graphe non orienté

```cpp
#include "CGraph.h"

CGraph<CSommet, CArc> grapheND;

grapheND.GROAjouterSommet(1);
grapheND.GROAjouterSommet(2);

grapheND.GRAAjouterArete(1, 2);  // crée (1→2) et (2→1) en interne
grapheND.GROAfficher();           // affiche "1 -- 2" une seule fois
```

### Lire un graphe depuis un fichier

```cpp
#include "GraphFileReader.h"

GraphFileReader reader("graph.txt");

// L'appelant est responsable du delete
CGraphOrient<CSommet, CArc>* pGraphe = reader.GFRCreateGraphFromTextFile();
pGraphe->GROAfficher();
delete pGraphe;
```

***

## Codes d'erreur — référence rapide

| Code | Constante | Classe | Déclencheur |
|------|-----------|--------|-------------|
| 2 | `GROEXC_SOMMET_DEJA_PRESENTE` | `CGraphOrient` | Ajout d'un sommet déjà présent |
| 3 | `GROEXC_SOMMET_ABSENT` | `CGraphOrient` | Opération sur un sommet inexistant |
| 4 | `GROEXC_SOMMET_ARC_ABSENT` | `CGraphOrient` | Arc avec un sommet inexistant |
| 5 | `GROEXC_ARC_DEJA_PRESENT` | `CGraphOrient` | Ajout d'un arc déjà présent |
| 6 | `GROEXC_ARC_ABSENT` | `CGraphOrient` | Suppression d'un arc inexistant |
| 10 | `GFREXC_FICHIER_INTROUVABLE` | `GraphFileReader` | Fichier inaccessible |
| 11 | `GFREXC_FORMAT_INVALIDE` | `GraphFileReader` | Ligne mal formée |
| 12 | `GFREXC_NB_SOMMETS_INCOHERENT` | `GraphFileReader` | Compteur sommets incorrect |
| 13 | `GFREXC_NB_ARCS_INCOHERENT` | `GraphFileReader` | Compteur arcs incorrect |

***

## Lancer les tests

Les tests sont intégrés dans `main.cpp`. Compiler et exécuter le projet affiche automatiquement les résultats :

```
===== TESTS CArc =====
[OK]  CArc() : sommet de depart initialise a 0
[OK]  CArc(3,7) : sommet de depart = 3
...
Bilan : 42/42 tests réussis
```

## Choix de conception

### Documentation et commentaires

Chaque classe est précédée d'un en-tête documentaire indiquant son rôle, sa version, ses auteurs et sa date de création. Chaque méthode est documentée selon un format homogène inspiré des conventions vues en cours :

```cpp
/* *************************************************************
* GROAjouterSommet
* **************************************************************
* Entrée  : Entier représentant le numéro du nouveau sommet
* Nécessite : Le numéro ne doit pas déjà exister dans le graphe
* Sortie  : Rien
* Entraine : Lance CException(GROEXC_SOMMET_DEJA_PRESENTE) si doublon
* **************************************************************/
```

Ce format systématique présente un double avantage : il force à réfléchir aux préconditions et aux effets de bord avant d'écrire le corps de la méthode, et il rend le code lisible par un tiers sans avoir à en lire l'implémentation. Les commentaires en ligne sont volontairement rares et réservés aux décisions non évidentes — comme la justification du `catch(...)` dans `GraphFileReader` ou l'explication du `this->` obligatoire dans les classes template dérivées.

### Convention de nommage

Nous avons appliqué la convention de nommage recommandée par le professeur de manière rigoureuse et homogène sur l'ensemble du projet. Chaque identifiant est préfixé d'un code indiquant son type et sa classe d'appartenance :

| Préfixe | Signification | Exemple |
|---|---|---|
| `i` | Entier (`int`) | `iSOMNum`, `iNBSommets` |
| `b` | Booléen | `bCondition` |
| `s` | Chaîne de caractères | `sGFRFileName` |
| `p` | Pointeur | `pSommetAAjouter`, `pArc` |
| `vp` | Vecteur de pointeurs | `vpARCGROListeArcs` |
| `mip` | Map `<int, pointeur>` | `mipSOMGROListeSommets` |
| Préfixe classe | Appartenance à une classe | `GRO` pour `CGraphOrient`, `SOM` pour `CSommet`, `ARC` pour `CArc` |

Cette convention présente un avantage concret lors du développement : en lisant `mipSOMGROListeSommets`, on sait immédiatement qu'il s'agit d'une `map<int, pointeur>` de sommets appartenant à `CGraphOrient`, sans avoir à consulter la déclaration. Elle réduit les erreurs de type et facilite la relecture croisée entre les deux membres du binôme.

### Classe `CArc`

`CArc` représente un arc orienté entre deux sommets. C'est la structure de données la plus atomique de la librairie — elle ne porte que deux informations : le numéro du sommet de départ et le numéro du sommet d'arrivée. Elle sert de classe de base pour tout type d'arc personnalisé grâce au mécanisme de template.

---

#### Attributs protégés

| Attribut | Type | Rôle |
|---|---|---|
| `iARCSommetDepart` | `int` | Numéro du sommet de départ de l'arc |
| `iARCSommetArrivee` | `int` | Numéro du sommet d'arrivée de l'arc |

---

#### Interface publique

**Constructeurs / Destructeur**

```cpp
CArc();
```
*Constructeur par défaut. Initialise les deux extrémités à 0.*

```cpp
CArc(int iDepart, int iArrivee);
```
*Constructeur paramétré. Initialise directement les numéros de départ et d'arrivée.*

```cpp
virtual ~CArc() = default;
```
*Destructeur virtuel. Permet la destruction correcte par polymorphisme.*

---

**Accesseurs**

```cpp
int ARCGetSommetDepart() const;
```
- **Entrée :** Rien
- **Sortie :** Numéro du sommet de départ
- **Entraîne :** Rien

```cpp
int ARCGetSommetArrivee() const;
```
- **Entrée :** Rien
- **Sortie :** Numéro du sommet d'arrivée
- **Entraîne :** Rien

---

**Mutateurs**

```cpp
void ARCSetSommetDepart(int iDepart);
```
- **Entrée :** Nouveau numéro de départ
- **Sortie :** Rien
- **Entraîne :** Modifie `iARCSommetDepart`

```cpp
void ARCSetSommetArrivee(int iArrivee);
```
- **Entrée :** Nouveau numéro d'arrivée
- **Sortie :** Rien
- **Entraîne :** Modifie `iARCSommetArrivee`

---

**Opérateur**

```cpp
bool operator==(const CArc& arcParam) const;
```
- **Entrée :** Un arc à comparer
- **Sortie :** `true` si les deux extrémités sont identiques
- **Entraîne :** Rien

---

### Classe `CSommet`

`CSommet` représente un sommet du graphe. En plus de son numéro d'identification, il maintient deux listes de pointeurs : une vers ses arcs entrants, une vers ses arcs sortants. Cette structure locale permet de naviguer dans le graphe depuis un sommet sans remonter systématiquement à la structure globale, ce qui sera utile pour des algorithmes de parcours futurs.

Les méthodes de modification structurelle (ajout/suppression d'arcs, changement de numéro) sont déclarées **`protected`** et accessibles uniquement par `CGraphOrient` via le mécanisme `friend`. Cette contrainte garantit que la cohérence du graphe ne peut être rompue depuis l'extérieur.

---

#### Attributs protégés

| Attribut | Type | Rôle |
|---|---|---|
| `iSOMNum` | `int` | Numéro identifiant du sommet |
| `vpSOMArcEntrants` | `vector<CArc*>` | Liste des arcs dont ce sommet est l'arrivée |
| `vpSOMArcSortants` | `vector<CArc*>` | Liste des arcs dont ce sommet est le départ |

---

#### Interface publique

**Constructeurs / Destructeur**

```cpp
CSommet();
```
*Constructeur par défaut. Initialise le numéro à 0 et les listes vides.*

```cpp
CSommet(int iNum);
```
*Constructeur paramétré. Initialise le sommet avec le numéro donné.*

```cpp
virtual ~CSommet() = default;
```
*Destructeur virtuel. Permet la destruction correcte par polymorphisme.*

---

**Accesseurs**

```cpp
int SOMGetNum() const;
```
- **Entrée :** Rien
- **Sortie :** Numéro du sommet
- **Entraîne :** Rien

```cpp
const vector<CArc*>& SOMGetArcsEntrants() const;
```
- **Entrée :** Rien
- **Sortie :** Référence constante sur la liste des arcs entrants
- **Entraîne :** Rien

```cpp
const vector<CArc*>& SOMGetArcsSortants() const;
```
- **Entrée :** Rien
- **Sortie :** Référence constante sur la liste des arcs sortants
- **Entraîne :** Rien

---

**Opérateur**

```cpp
bool operator==(const CSommet& somParam) const;
```
- **Entrée :** Un sommet à comparer
- **Sortie :** `true` si les deux numéros sont identiques
- **Entraîne :** Rien

---

#### Méthodes protégées (réservées à `CGraphOrient`)

```cpp
void SOMSetNum(int iNum);
```
- **Entrée :** Nouveau numéro du sommet
- **Nécessite :** Être appelée uniquement depuis `CGraphOrient` (friend)
- **Sortie :** Rien
- **Entraîne :** Modifie `iSOMNum`. La mise à jour des arcs associés est gérée par `CGraphOrient::GROModifierSommet`.

```cpp
void SOMAjouterArcEntrant(CArc* pArc);
```
- **Entrée :** Pointeur vers un arc dont ce sommet est l'arrivée
- **Sortie :** Rien
- **Entraîne :** Ajoute le pointeur à `vpSOMArcEntrants`

```cpp
void SOMAjouterArcSortant(CArc* pArc);
```
- **Entrée :** Pointeur vers un arc dont ce sommet est le départ
- **Sortie :** Rien
- **Entraîne :** Ajoute le pointeur à `vpSOMArcSortants`

```cpp
void SOMSupprimerArcEntrant(CArc* pArc);
```
- **Entrée :** Pointeur vers l'arc entrant à retirer
- **Nécessite :** Si l'arc est absent de la liste, l'opération est sans effet (garde contre le crash)
- **Sortie :** Rien
- **Entraîne :** Retire le pointeur de `vpSOMArcEntrants`

```cpp
void SOMSupprimerArcSortant(CArc* pArc);
```
- **Entrée :** Pointeur vers l'arc sortant à retirer
- **Nécessite :** Si l'arc est absent de la liste, l'opération est sans effet (garde contre le crash)
- **Sortie :** Rien
- **Entraîne :** Retire le pointeur de `vpSOMArcSortants`

### Classe `CGraphOrient<TSommet, TArc>`

`CGraphOrient` est la classe centrale de la librairie. Elle orchestre l'ensemble des opérations sur le graphe orienté et est la seule responsable de la cohérence entre ses trois composantes : la `map` des sommets, le vecteur des arcs, et les listes locales de chaque sommet. Deux `static_assert` garantissent à la compilation que `TSommet` dérive bien de `CSommet` et `TArc` de `CArc`, rendant toute instanciation illégale impossible sans message d'erreur explicite.

La copie d'un graphe est explicitement interdite (`= delete`) pour prévenir les doubles libérations mémoire dues au partage de pointeurs.

***

#### Attributs protégés

| Attribut | Type | Rôle |
|---|---|---|
| `mipSOMGROListeSommets` | `map<int, TSommet*>` | Dictionnaire associant chaque numéro de sommet à son objet |
| `vpARCGROListeArcs` | `vector<TArc*>` | Liste de tous les arcs du graphe |

***

#### Interface publique

**Constructeurs / Destructeur**

```cpp
CGraphOrient();
```
*Constructeur par défaut. Initialise un graphe vide.*

```cpp
virtual ~CGraphOrient();
```
*Destructeur virtuel. Libère tous les arcs puis tous les sommets alloués dynamiquement.*


***

**Accesseurs**

```cpp
TSommet* GROGetSommet(int iNum) const;
```
- **Entrée :** Numéro du sommet recherché
- **Sortie :** Pointeur vers le sommet, `nullptr` si absent
- **Entraîne :** Rien

```cpp
const map<int, TSommet*>& GROGetSommets() const;
```
- **Entrée :** Rien
- **Sortie :** Référence constante sur la map des sommets
- **Entraîne :** Rien

```cpp
const vector<TArc*>& GROGetArcs() const;
```
- **Entrée :** Rien
- **Sortie :** Référence constante sur le vecteur des arcs
- **Entraîne :** Rien

```cpp
TSommet& operator[](int iNumSommet);
```
- **Entrée :** Numéro du sommet
- **Nécessite :** Le sommet existe (lève `std::out_of_range` sinon)
- **Sortie :** Référence sur l'objet sommet
- **Entraîne :** Rien

***

**Gestion des sommets**

```cpp
void GROAjouterSommet(int iNum);
```
- **Entrée :** Numéro du nouveau sommet
- **Nécessite :** Le numéro ne doit pas déjà exister dans le graphe
- **Sortie :** Rien
- **Entraîne :** Alloue un `TSommet` et l'insère dans la map. Lance `CException(GROEXC_SOMMET_DEJA_PRESENTE)` si doublon.

```cpp
void GROModifierSommet(int iOldNum, int iNewNum);
```
- **Entrée :** Ancien numéro et nouveau numéro
- **Nécessite :** `iOldNum` présent, `iNewNum` absent du graphe
- **Sortie :** Rien
- **Entraîne :** Met à jour le numéro interne du sommet via `SOMSetNum`, met à jour toutes les extrémités des arcs concernés, puis recrée l'entrée dans la map. Lance `CException` si préconditions non respectées.

```cpp
void GROSupprimerSommet(int iNum);
```
- **Entrée :** Numéro du sommet à supprimer
- **Nécessite :** Le sommet existe
- **Sortie :** Rien
- **Entraîne :** Supprime tous les arcs adjacents (entrants et sortants) en mettant à jour les listes des sommets voisins concernés, puis libère le sommet et le retire de la map. Lance `CException(GROEXC_SOMMET_ABSENT)` si absent.

***

**Gestion des arcs**

```cpp
void GROAjouterArc(int iDepart, int iArrivee);
```
- **Entrée :** Numéros des sommets de départ et d'arrivée
- **Nécessite :** Les deux sommets existent et l'arc n'est pas déjà présent
- **Sortie :** Rien
- **Entraîne :** Alloue un `TArc`, l'ajoute au vecteur global et met à jour les listes sortante/entrante des sommets concernés. Lance `CException` si préconditions non respectées.

```cpp
void GROSupprimerArc(int iDepart, int iArrivee);
```
- **Entrée :** Numéros des sommets de départ et d'arrivée
- **Nécessite :** L'arc existe dans le graphe
- **Sortie :** Rien
- **Entraîne :** Retire l'arc des listes locales des sommets concernés, le libère et le retire du vecteur global. Lance `CException(GROEXC_ARC_ABSENT)` si absent.

***

**Affichage et algorithmes**

```cpp
virtual void GROAfficher(ostream& os = cout) const;
```
- **Entrée :** Flux de sortie (cout par défaut)
- **Sortie :** Rien
- **Entraîne :** Affiche la liste des sommets puis la liste des arcs sous la forme `depart -> arrivee`. Méthode virtuelle surchargée par `CGraph`.

```cpp
CGraphOrient<TSommet, TArc>* GROInverser() const;
```
- **Entrée :** Rien
- **Sortie :** Pointeur vers un nouveau graphe alloué dynamiquement dans lequel chaque arc `A → B` est remplacé par `B → A`
- **Entraîne :** L'appelant est responsable du `delete` du graphe retourné.

***

### Classe `CGraph<TSommet, TArc>`

`CGraph` hérite de `CGraphOrient` et spécialise son comportement pour modéliser un **graphe non orienté**. Le principe retenu est de représenter chaque arête par deux arcs symétriques internes, ce qui permet de réutiliser intégralement l'infrastructure de `CGraphOrient` sans dupliquer de logique. `CGraph` n'introduit aucun nouvel attribut — seules les opérations d'ajout et de suppression d'arêtes sont ajoutées, et l'affichage est surchargé pour ne pas afficher deux fois chaque arête.

***

#### Interface publique

**Constructeurs / Destructeur**

```cpp
CGraph();
```
*Constructeur par défaut. Délègue à `CGraphOrient`.*

```cpp
virtual ~CGraph() = default;
```
*Destructeur virtuel. La libération mémoire est entièrement gérée par le destructeur de `CGraphOrient`.*

***

**Gestion des arêtes**

```cpp
void CGRAjouterArete(int iNumA, int iNumB);
```
- **Entrée :** Numéros des deux sommets de l'arête
- **Nécessite :** Les deux sommets existent dans le graphe et l'arête n'est pas déjà présente
- **Sortie :** Rien
- **Entraîne :** Délègue deux appels à `GROAjouterArc` du parent : crée `(iNumA → iNumB)` puis `(iNumB → iNumA)`. Lance `CException` si préconditions non respectées.

```cpp
void CGRSupprimerArete(int iNumA, int iNumB);
```
- **Entrée :** Numéros des deux sommets de l'arête
- **Nécessite :** L'arête existe (les deux arcs symétriques sont présents)
- **Sortie :** Rien
- **Entraîne :** Délègue deux appels à `GROSupprimerArc` du parent : supprime `(iNumA → iNumB)` puis `(iNumB → iNumA)`. Lance `CException(GROEXC_ARC_ABSENT)` si l'un des deux arcs est absent.

***

**Affichage**

```cpp
virtual void GROAfficher(ostream& os = cout) const override;
```
- **Entrée :** Flux de sortie (cout par défaut)
- **Sortie :** Rien
- **Entraîne :** Affiche chaque arête une seule fois en n'affichant que les arcs dont le numéro de départ est strictement inférieur au numéro d'arrivée (convention `depart < arrivee`), évitant ainsi l'affichage redondant des deux arcs symétriques. Les arêtes sont affichées sous la forme `A -- B`.


### Classe `GraphFileReader`

#### Pourquoi un module dédié à la lecture de fichier

Nous aurions pu intégrer la lecture de fichier directement dans `CGraphOrient` sous la forme d'un constructeur prenant un chemin en paramètre. Nous avons choisi de ne pas le faire. Mélanger la logique de parsing de fichier texte avec la logique de gestion d'un graphe aurait violé le **principe de responsabilité unique** : `CGraphOrient` est responsable de la cohérence du graphe, pas de savoir comment un fichier est formaté. Si demain le format du fichier change, ou si l'on souhaite ajouter une lecture depuis un fichier XML ou une base de données, `CGraphOrient` ne devrait pas avoir à être modifié.

`GraphFileReader` est donc une classe de **service pur** : elle lit, valide, parse et construit. Elle ne conserve aucun état autre que le nom du fichier, et son unique responsabilité est de produire un graphe correctement initialisé depuis une source externe. Cette séparation rend chaque classe plus facilement testable et maintenable indépendamment de l'autre.

***

#### Attribut privé

| Attribut | Type | Rôle |
|---|---|---|
| `sGFRFileName` | `string` | Chemin vers le fichier texte du graphe |

***

#### Interface publique

**Constructeur**

```cpp
GraphFileReader(const string& sParam);
```
*Initialise le lecteur avec le chemin du fichier cible. Le constructeur par défaut est volontairement absent : un `GraphFileReader` sans fichier n'a aucune utilité fonctionnelle.*

***

**Méthode principale**

```cpp
CGraphOrient<CSommet, CArc>* GFRCreateGraphFromTextFile() const;
```
- **Entrée :** Rien (utilise `sGFRFileName` initialisé à la construction)
- **Nécessite :** Le fichier respecte strictement le format imposé :
  ```
  NBSommets=<n>
  NBArcs=<m>
  Sommets=[
  Numero=<id>
  ...
  ]
  Arcs=[
  Debut=<id_depart>, Fin=<id_arrivee>
  ...
  ]
  ```
- **Sortie :** Pointeur vers un `CGraphOrient<CSommet, CArc>` alloué dynamiquement et entièrement construit — **l'appelant est responsable du `delete`**
- **Entraîne :**
  - Ouvre le fichier et lit son contenu ligne par ligne
  - Parse `NBSommets` et `NBArcs` pour connaître les tailles attendues
  - Crée chaque sommet via `GROAjouterSommet` en lisant les lignes `Numero=`
  - Crée chaque arc via `GROAjouterArc` en lisant les lignes `Debut=, Fin=`
  - Vérifie en fin de parsing que le nombre de sommets et d'arcs effectivement lus correspond aux valeurs déclarées en en-tête
  - En cas d'erreur à n'importe quelle étape, libère le graphe partiellement construit avant de relancer l'exception — aucune fuite mémoire n'est possible
  - Lance `CException(GFREXC_FICHIER_INTROUVABLE)` si le fichier est inaccessible
  - Lance `CException(GFREXC_FORMAT_INVALIDE)` si une ligne ne peut pas être parsée
  - Lance `CException(GFREXC_NB_SOMMETS_INCOHERENT)` ou `CException(GFREXC_NB_ARCS_INCOHERENT)` si les compteurs ne correspondent pas aux valeurs déclarées

***

**Méthodes privées**

```cpp
int GFRExtraireEntier(const string& sLigne, const string& sCle) const;
```
- **Entrée :** Une ligne de texte et une clé (ex : `"NBSommets"`, `"Numero"`)
- **Nécessite :** La ligne contient la séquence `"cle=<entier>"`
- **Sortie :** L'entier extrait après le caractère `=`
- **Entraîne :** Lance `CException(GFREXC_FORMAT_INVALIDE)` si la clé est absente ou si la valeur n'est pas convertible en entier. Utilisé en interne par `GFRCreateGraphFromTextFile` et `GFRExtraireArc`.

```cpp
void GFRExtraireArc(const string& sLigne, int& iDebut, int& iFin) const;
```
- **Entrée :** Une ligne du type `"Debut=X, Fin=Y"`
- **Nécessite :** La ligne contient les clés `"Debut"` et `"Fin"`
- **Sortie :** `iDebut` et `iFin` remplis par référence
- **Entraîne :** Délègue deux appels à `GFRExtraireEntier`. Lance `CException(GFREXC_FORMAT_INVALIDE)` si l'un des deux champs est manquant ou malformé.
