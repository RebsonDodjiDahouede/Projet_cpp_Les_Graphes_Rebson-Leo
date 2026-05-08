# Librairie Graphes — C++17

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

> ⚠️ Le nombre de `Numero=` doit correspondre exactement à `NBSommets`, et le nombre de lignes `Debut=..., Fin=...` doit correspondre exactement à `NBArcs`. Tout écart lève une `CException`.

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

### Gestion des exceptions

```cpp
#include "CException.h"

try {
    graphe.GROAjouterSommet(1);
    graphe.GROAjouterSommet(1); // doublon → exception
}
catch (const CException& e) {
    cout << "Erreur code : " << e.EXCLire() << endl;
    // code 2 = GROEXC_SOMMET_DEJA_PRESENTE
}
```

### Étendre les classes de base

```cpp
// Sommet avec un label texte
class CSommetLabel : public CSommet {
public:
    string sLabel;
    CSommetLabel(int iNum, const string& label)
        : CSommet(iNum), sLabel(label) {}
};

// Arc avec un poids
class CArcPondere : public CArc {
public:
    double dPoids;
    CArcPondere(int iDep, int iArr, double poids)
        : CArc(iDep, iArr), dPoids(poids) {}
};

// Instanciation avec les types personnalisés
CGraphOrient<CSommetLabel, CArcPondere> graphePondere;
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
