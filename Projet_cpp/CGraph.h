#ifndef CGRAPH_H
#define CGRAPH_H
/***************************************************************
* CLASSE : CGraph
* **************************************************************
* ROLE : Graphe non orienté. SpéciNumAlise CGraphOrient en
*        représentant chaque arête par deux arcs symétriques.
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 04/05/2026
* **************************************************************/
#include "CGraphOrient.h"

using namespace std;

template<typename TSommet, typename TArc>
class CGraph : public CGraphOrient<TSommet, TArc>
{
public:

    // CONSTRUCTEURS
    CGraph() = default;
    virtual ~CGraph() = default;

    // MÉTHODES

    /* *************************************************************
    * CGRAjouterArete
    * **************************************************************
    * Entrée  : Numéro des deux sommets de l'arête
    * Nécessite : Les deux sommets existent dans le graphe
    * Sortie  : Rien
    * Entraine : Création de deux arcs symétriques (iNumA->iNumB) et (iNumB->iNumA)
    *            Lance CException si un sommet est absent ou si
    *            l'arête existe déjà
    * **************************************************************/
    void CGRAjouterArete(int iNumA, int iNumB);

    /* *************************************************************
    * CGRSupprimerArete
    * **************************************************************
    * Entrée  : Numéro des deux sommets de l'arête
    * Nécessite : L'arête existe (les deux arcs symétriques présents)
    * Sortie  : Rien
    * Entraine : Suppression des deux arcs (iNumA->iNumB) et (iNumB->iNumA)
    *            Lance CException si l'un des deux arcs est absent
    * **************************************************************/
    void CGRSupprimerArete(int iNumA, int iNumB);

    /* *************************************************************
    * GROAfficher (override/surcharge)
    * **************************************************************
    * Entrée  : Flots (cout par défaut)
    * Nécessite : Rien
    * Sortie  : Rien
    * Entraine : Affichage du graphe où chaque arête n'est affichée
    *            qu'une fois (On n'affiche que les arcs
    *            où depart < arrivee pour éviter les doublons visuels)
    * **************************************************************/
    virtual void GROAfficher(ostream& os = cout) const override;
};



// =====================================================================
// IMPLÉMENTATIONS DES MÉTHODES (Toujours dans le .h comme CGRAPHORIENT)
// =====================================================================

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::CGRAjouterArete(int iNumA, int iNumB)
{
    // Délègue entièrement à GROAjouterArc du parent
    // GROAjouterArc gère déjà la vérification des sommets,
    // détection des doublons, mise à jour des listes de CSommet.
    CGraphOrient<TSommet, TArc>::GROAjouterArc(iNumA, iNumB);
    CGraphOrient<TSommet, TArc>::GROAjouterArc(iNumB, iNumA);
}

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::CGRSupprimerArete(int iNumA, int iNumB)
{
    // Délègue entièrement à GROSupprimerArc du parent comme pour l'ajout
    // Si l'un des arcs est absent, GROSupprimerArc lève une
    // CException(GROEXC_ARC_ABSENT)
    CGraphOrient<TSommet, TArc>::GROSupprimerArc(iNumA, iNumB);
    CGraphOrient<TSommet, TArc>::GROSupprimerArc(iNumB, iNumA);
}

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::GROAfficher(ostream& os) const
{
    os << "Sommets :\n";
    for (const auto& paire : this->mipSOMGROListeSommets)
        os << "  Sommet " << paire.first << "\n";

    // On n'affiche que les arcs où depart < arrivee pour éviter
    // d'afficher deux fois chaque arête (A-B et B-A)
    os << "Aretes :\n";
    for (const TArc* pArc : this->vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() < pArc->ARCGetSommetArrivee())
            os << "  " << pArc->ARCGetSommetDepart()
            << " -- " << pArc->ARCGetSommetArrivee() << "\n";
    }
}

#endif // CGRAPH_H
