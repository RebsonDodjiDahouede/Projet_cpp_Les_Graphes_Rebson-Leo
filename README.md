## I. Introduction

### Contexte du projet

Le projet Graphes consiste à concevoir et développer une **librairie C++ générique** permettant de modéliser, manipuler et exploiter des structures de graphes orientés et non orientés. Contrairement à une implémentation rigide et mono-type, notre librairie repose sur la généricité via les **templates C++** afin de permettre à n'importe quel utilisateur d'étendre les classes de base pour y intégrer ses propres types de sommets et d'arcs sans modifier le cœur de la librairie.

L'objectif est de produire un ensemble de classes robustes et cohérentes, capables de lire un graphe depuis un fichier texte, d'en afficher la structure, et d'en calculer le graphe inverse, tout en mettant en pratique les principes fondamentaux de la **programmation orientée objet** : encapsulation, héritage, polymorphisme et gestion maîtrisée de la mémoire.

### Spécifications fonctionnelles

La librairie doit permettre la **création et la manipulation complète** d'un graphe orienté : ajout, modification et suppression de sommets et d'arcs, avec une gestion des erreurs par exceptions pour tout cas incohérent (doublon, élément absent). Chaque sommet connaît ses arcs entrants et sortants, ce qui permet de naviguer localement dans le graphe sans remonter systématiquement à la structure globale.

La **lecture depuis un fichier texte** au format imposé (`NBSommets`, `NBArcs`, sections `Sommets=[...]` et `Arcs=[...]`) constitue le point d'entrée principal de la librairie depuis l'extérieur. Une fois le graphe construit, il peut être **affiché** et **inversé** — c'est-à-dire qu'un nouveau graphe est produit dans lequel chaque arc `A → B` devient `B → A`.

La librairie expose également une classe `CGraph` pour les **graphes non orientés**, qui hérite de `CGraphOrient` et simule une arête par deux arcs symétriques, garantissant ainsi la cohérence bidirectionnelle sans dupliquer la logique d'ajout et de suppression.

### Contraintes techniques et non fonctionnelles

- **Environnement** : Windows, compilé sous Visual Studio 2022 en mode Release x64.
- **Langage** : C++ (norme C++17), sans bibliothèque tierce.
- **Genericité** : Les classes `CGraphOrient` et `CGraph` sont templatées sur le type de sommet et le type d'arc, avec des `static_assert` garantissant que ces types dérivent bien de `CSommet` et `CArc`.
- **Architecture** : Hiérarchie d'héritage stricte — `CGraph` hérite de `CGraphOrient`, qui est seul responsable de la cohérence interne du graphe. Les méthodes structurellement sensibles de `CSommet` sont déclarées `protected` et accessibles uniquement via un mécanisme d'amitié avec `CGraphOrient`.
- **Gestion de la mémoire** : Tous les sommets et arcs sont alloués dynamiquement et libérés par les destructeurs des classes.
- **Gestion des erreurs** : Toutes les préconditions sont vérifiées et signalées via la classe `CException` avec des codes d'erreur identifiés par des constantes `#define` propres à chaque classe.


## II. Choix de conception

### 2.1 Vision générale — `CGraphOrient` comme chef d'orchestre

Dès la lecture attentive du sujet, une question s'est posée naturellement : qui est responsable de la cohérence du graphe ? Un sommet peut-il s'ajouter lui-même à un graphe ? Un arc peut-il se supprimer sans que le graphe le sache ? La réponse était évidente — non. Un graphe est une structure globale dont la cohérence repose sur la coordination de toutes ses parties. Laisser un utilisateur modifier directement un sommet depuis l'extérieur, sans passer par le graphe, risquerait de désynchroniser la `map` des sommets, les listes d'arcs et les références internes de chaque sommet.

Nous avons donc posé une règle simple : **`CGraphOrient` est le seul chef d'orchestre**. Toute opération structurelle — ajout, suppression, modification d'un sommet ou d'un arc — doit obligatoirement transiter par lui. Pour implémenter cette contrainte, nous avons découvert et combiné deux mécanismes du C++ : les **attributs et méthodes `protected`** de `CSommet`, qui rendent les opérations sensibles inaccessibles depuis l'extérieur, et la **déclaration `friend`** de `CGraphOrient` dans `CSommet`, qui lui octroie un accès exclusif à ces membres protégés. Cette combinaison garantit qu'aucun code client ne peut modifier la structure interne d'un sommet sans passer par le graphe, tout en conservant une interface publique propre et lisible pour l'utilisateur de la librairie.

### 2.2 Choix des structures de données

Le choix des structures utilisées dans `CGraphOrient` n'est pas arbitraire et répond à des besoins précis dictés par les opérations attendues sur le graphe.

Les sommets sont stockés dans une **`std::map<int, TSommet*>`**, c'est-à-dire un dictionnaire ordonné associant le numéro d'un sommet à un pointeur vers cet objet. Ce choix se justifie par trois raisons. Premièrement, l'accès à un sommet par son numéro est une opération très fréquente — notamment lors de l'ajout ou de la suppression d'un arc — et la `map` garantit un accès en **O(log n)** sans avoir à parcourir toute la collection. Deuxièmement, la `map` assure naturellement **l'unicité des clés**, ce qui empêche structurellement d'avoir deux sommets avec le même numéro. Troisièmement, la modification du numéro d'un sommet (`GROModifierSommet`) se traduit simplement par un `erase` de l'ancienne clé et une insertion de la nouvelle, sans toucher à l'objet lui-même.

Les arcs sont quant à eux stockés dans un **`std::vector<TArc*>`**. Les arcs n'ont pas de clé naturelle unique — un arc est identifié par la paire (départ, arrivée) et non par un identifiant propre. Un vecteur est donc suffisant et plus adapté : il permet un parcours séquentiel efficace, notamment lors de la suppression d'un sommet où tous les arcs adjacents doivent être identifiés et supprimés en une seule passe avec un itérateur. La recherche d'un arc spécifique se fait via `std::find_if` avec un prédicat, ce qui reste acceptable compte tenu du fait que cette opération n'est pas un goulot d'étranglement pour les tailles de graphes envisagées.

Enfin, chaque `CSommet` maintient deux **`std::vector<CArc*>`** : un pour ses arcs entrants et un pour ses arcs sortants. Ce choix permet de naviguer localement dans le graphe depuis un sommet sans remonter à la structure globale, ce qui sera particulièrement utile pour des algorithmes de parcours comme le BFS ou le DFS que l'on pourrait envisager d'implémenter par la suite.

### 2.3 Classe `CArc`

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

### 2.4 Classe `CSommet`

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

### 2.5 Classe `CGraphOrient<TSommet, TArc>`

`CGraphOrient` est la classe centrale de la librairie. Elle orchestre l'ensemble des opérations sur le graphe orienté et est la seule responsable de la cohérence entre ses trois composantes : la `map` des sommets, le vecteur des arcs, et les listes locales de chaque sommet. Deux `static_assert` garantissent à la compilation que `TSommet` dérive bien de `CSommet` et `TArc` de `CArc`, rendant toute instanciation illégale impossible sans message d'erreur explicite.

La copie d'un graphe est explicitement interdite (`= delete`) pour prévenir les doubles libérations mémoire dues au partage de pointeurs.

***

#### Attributs protégés

| Attribut | Type | Rôle |
|---|---|---|
| `mipSOMGROListeSommets` | `map<int, TSommet*>` | Dictionnaire associant chaque numéro de sommet à son objet |
| `vpARCGROListeArcs` | `vector<TArc*>` | Liste de tous les arcs du graphe |

***

#### Codes d'exception

| Constante | Valeur | Situation déclenchante |
|---|---|---|
| `GROEXC_SOMMET_DEJA_PRESENTE` | 2 | Ajout d'un sommet dont le numéro existe déjà |
| `GROEXC_SOMMET_ABSENT` | 3 | Opération sur un sommet inexistant |
| `GROEXC_SOMMET_ARC_ABSENT` | 4 | Ajout d'un arc avec un sommet inexistant |
| `GROEXC_ARC_DEJA_PRESENT` | 5 | Ajout d'un arc déjà présent |
| `GROEXC_ARC_ABSENT` | 6 | Suppression d'un arc inexistant |

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

### 2.6 Classe `CGraph<TSommet, TArc>`

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


### 2.7 Classe `GraphFileReader`

#### Pourquoi un module dédié à la lecture de fichier

Nous aurions pu intégrer la lecture de fichier directement dans `CGraphOrient` sous la forme d'un constructeur prenant un chemin en paramètre. Nous avons choisi de ne pas le faire. Mélanger la logique de parsing de fichier texte avec la logique de gestion d'un graphe aurait violé le **principe de responsabilité unique** : `CGraphOrient` est responsable de la cohérence du graphe, pas de savoir comment un fichier est formaté. Si demain le format du fichier change, ou si l'on souhaite ajouter une lecture depuis un fichier XML ou une base de données, `CGraphOrient` ne devrait pas avoir à être modifié.

`GraphFileReader` est donc une classe de **service pur** : elle lit, valide, parse et construit. Elle ne conserve aucun état autre que le nom du fichier, et son unique responsabilité est de produire un graphe correctement initialisé depuis une source externe. Cette séparation rend chaque classe plus facilement testable et maintenable indépendamment de l'autre.

***

#### Attribut privé

| Attribut | Type | Rôle |
|---|---|---|
| `sGFRFileName` | `string` | Chemin vers le fichier texte du graphe |

***

#### Codes d'exception

| Constante | Valeur | Situation déclenchante |
|---|---|---|
| `GFREXC_FICHIER_INTROUVABLE` | 10 | Le fichier ne peut pas être ouvert |
| `GFREXC_FORMAT_INVALIDE` | 11 | Une ligne ne respecte pas le format attendu |
| `GFREXC_NB_SOMMETS_INCOHERENT` | 12 | Le nombre de sommets lus diffère de `NBSommets` déclaré |
| `GFREXC_NB_ARCS_INCOHERENT` | 13 | Le nombre d'arcs lus diffère de `NBArcs` déclaré |

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

## III. Génie logiciel et bonnes pratiques

Dès le départ, nous avons souhaité que ce projet ne soit pas simplement un exercice de programmation, mais une démarche structurée où chaque décision technique est consciente et justifiable. Cette section présente les pratiques mises en place tout au long du développement pour répondre à cet objectif.

### 3.1 Documentation et commentaires

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

### 3.2 Convention de nommage

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

### 3.3 Refactorisation

La version finale du code est le résultat de plusieurs cycles de refactorisation. Parmi les améliorations les plus significatives, on peut citer le déplacement de `SOMSetNum` en `protected` — initialement `public`, nous avons réalisé en cours de développement que laisser cette méthode accessible depuis l'extérieur rendait la cohérence du graphe impossible à garantir. C'est précisément ce type de décision, prise après réflexion sur les invariants du système, qui illustre l'intérêt d'une démarche itérative.

De même, les méthodes `SOMSupprimerArcEntrant` et `SOMSupprimerArcSortant` ont été sécurisées par une vérification de présence avant suppression, évitant un appel indéfini à `erase(end())`. Dans `GROSupprimerSommet`, les accès directs à la `map` via `operator[]` ont été remplacés par des appels à `GROGetSommet`, qui retourne `nullptr` sans insérer d'élément parasite si la clé est absente. Ces corrections, bien que discrètes, reflètent une attention portée au comportement du code dans les cas limites et pas uniquement dans les cas nominaux.

### 3.4 Tests unitaires

Chaque classe dispose de sa propre fonction de test dans le `main` : `TesterCArc`, `TesterCSommet`, `TesterCGraphOrient` et `TesterCGraph`. Ces fonctions ne se contentent pas de tester les cas nominaux — elles vérifient aussi les comportements aux limites : levée d'exception sur un sommet doublon, levée d'exception sur un arc absent, cohérence des listes d'arcs après suppression d'un sommet, mise à jour des extrémités d'arc après modification du numéro d'un sommet, ou encore vérification que le graphe inverse contient bien les arcs dans le bon sens.

Le résultat de chaque test est affiché avec un indicateur `[OK]` ou `[ECHEC]` et un bilan final donne le nombre de tests réussis sur le total. Cette approche, bien que manuelle et sans framework dédié, nous a permis de détecter plusieurs bugs au cours du développement — notamment une inversion des noms `SOMSupprimerArcArrivant` / `SOMSupprimerArcEntrant` et une erreur de syntaxe pointeur dans `GROModifierSommet` — avant qu'ils ne se propagent dans le reste du code.