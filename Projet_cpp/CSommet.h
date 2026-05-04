#ifndef CSOMMET_H
#define CSOMMET_H
/***************************************************************
* CLASSE : CSommet
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Sommet de graphes
* **************************************************************
* VERSION : 1.1
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/
#include <vector>
#include <algorithm>
#include "CArc.h"
#include "CException.h"

/* std -> pour utiliser le stdout */
using namespace std;

/* TYPES :
*   Pas de type particulier de déclaré
*/

/* Variables :
*   Pas de Variable globale de déclaré
*/

class CSommet
{
    // ATTRIBUTS
protected:
    int iSOMNum;
    std::vector<CArc*> vpSOMArcEntrants;
    std::vector<CArc*> vpSOMArcSortants;

    /* *************************************************************
    * SOMAjouterArcEntrant
    * **************************************************************
    * Entrée : Un pointeur sur l'arc
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : L'ajout d'un arc entrant dans le vecteur des arcs entrants
    * **************************************************************/
    void SOMAjouterArcEntrant(CArc* pArcEntrant) { vpSOMArcEntrants.push_back(pArcEntrant); }

    /* *************************************************************
    * SOMAjouterArcSortant
    * **************************************************************
    * Entrée : Un pointeur sur l'arc
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : L'ajout d'un arc sortant dans le vecteur des arcs sortants
    * **************************************************************/
    void SOMAjouterArcSortant(CArc* pArcSortant) { vpSOMArcSortants.push_back(pArcSortant); }

    /* *************************************************************
    * SOMSupprimerArcEntrant
    * **************************************************************
    * Entrée : Un pointeur sur l'arc
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : La suppression de l'arc entrant dans le vecteur des arcs entrants
    *            Ne fait rien si le pointeur est absent (securise contre l'UB)
    * **************************************************************/
    void SOMSupprimerArcEntrant(CArc* pArcEntrantASupprimer);

    /* *************************************************************
    * SOMSupprimerArcSortant
    * **************************************************************
    * Entrée : Un pointeur sur l'arc
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : La suppression de l'arc sortant dans le vecteur des arcs sortants
    *            Ne fait rien si le pointeur est absent (securise contre l'UB)
    * **************************************************************/
    void SOMSupprimerArcSortant(CArc* pArcSortantASupprimer);

    /* *************************************************************
    * SOMSetNum
    * **************************************************************
    * Entrée : Un entier
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : La modification du numéro du sommet uniquement.
    *            La mise à jour des arcs adjacents est de la
    *            responsabilité de CGraphOrient::GROModifierSommet.
    * **************************************************************/
    void SOMSetNum(int iNum);

    // Seul CGraphOrient peut appeler les méthodes protected ci-dessus
    template<typename TSommet, typename TArc>
    friend class CGraphOrient;

    // CONSTRUCTEURS
public:
    CSommet() : iSOMNum(0) {}
    CSommet(int iNum) : iSOMNum(iNum) {}
    virtual ~CSommet() = default;

    // MÉTHODES

    /* *************************************************************
    * SOMGetNum
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : La valeur du numéro du sommet
    * Entraine : Rien
    * **************************************************************/
    int SOMGetNum() const { return iSOMNum; }

    /* *************************************************************
    * SOMGetArcsEntrants
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Une référence constante sur le vecteur des arcs entrants
    * Entraine : Rien
    * **************************************************************/
    const std::vector<CArc*>& SOMGetArcsEntrants() const { return vpSOMArcEntrants; }

    /* *************************************************************
    * SOMGetArcsSortants
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Une référence constante sur le vecteur des arcs sortants
    * Entraine : Rien
    * **************************************************************/
    const std::vector<CArc*>& SOMGetArcsSortants() const { return vpSOMArcSortants; }

    /* *************************************************************
    * operator==
    * **************************************************************
    * Entrée : Une référence constante sur un objet CSommet
    * Nécessite : Rien
    * Sortie : Un bool — vrai si les numéros sont identiques
    * Entraine : Rien
    * Note : On compare uniquement les numéros. Comparer les vecteurs
    *        de pointeurs comparerait des adresses mémoire, pas les arcs.
    * **************************************************************/
    bool operator==(const CSommet& SOMParam) const { return iSOMNum == SOMParam.iSOMNum; }
};

#endif // CSOMMET_H