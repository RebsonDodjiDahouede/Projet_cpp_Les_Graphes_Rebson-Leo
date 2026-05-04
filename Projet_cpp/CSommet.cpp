#include "CSommet.h"

/* *************************************************************
* SOMSetNum
* **************************************************************
* Modifie uniquement le numéro du sommet.
* La mise à jour des arcs adjacents (ARCSetSommetDepart /
* ARCSetSommetArrivee) est gérée par CGraphOrient::GROModifierSommet,
* qui est le seul point d'entrée légal pour renommer un sommet
* dans un graphe cohérent.
* **************************************************************/
void CSommet::SOMSetNum(int iNum)
{
    iSOMNum = iNum;
}

/* *************************************************************
* SOMSupprimerArcEntrant
* **************************************************************
* Recherche le pointeur dans le vecteur avant de l'effacer.
* Sans ce garde, erase(end()) provoquerait un undefined behavior.
* **************************************************************/
void CSommet::SOMSupprimerArcEntrant(CArc* pArcEntrantASupprimer)
{
    auto it = find(vpSOMArcEntrants.begin(), vpSOMArcEntrants.end(), pArcEntrantASupprimer);
    if (it != vpSOMArcEntrants.end())
        vpSOMArcEntrants.erase(it);
}

/* *************************************************************
* SOMSupprimerArcSortant
* **************************************************************
* Idem : garde contre l'undefined behavior si le pointeur
* est introuvable dans le vecteur.
* **************************************************************/
void CSommet::SOMSupprimerArcSortant(CArc* pArcSortantASupprimer)
{
    auto it = find(vpSOMArcSortants.begin(), vpSOMArcSortants.end(), pArcSortantASupprimer);
    if (it != vpSOMArcSortants.end())
        vpSOMArcSortants.erase(it);
}