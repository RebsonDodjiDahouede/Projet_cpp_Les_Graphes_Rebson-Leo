#ifndef CGRAPH_H
#define CGRAPH_H
/***************************************************************
* CLASSE : CGraph
* **************************************************************
* ROLE : Graphe non orienté. Spécialise CGraphOrient en
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
    * Entraine : Création de deux arcs symétriques (iA->iB) et (iB->iA)
    *            Lance CException si un sommet est absent ou si
    *            l'arête existe déjà
    * **************************************************************/
    void CGRAjouterArete(int iA, int iB);

    /* *************************************************************
    * CGRSupprimerArete
    * **************************************************************
    * Entrée  : Numéro des deux sommets de l'arête
    * Nécessite : L'arête existe (les deux arcs symétriques présents)
    * Sortie  : Rien
    * Entraine : Suppression des deux arcs (iA->iB) et (iB->iA)
    *            Lance CException si l'un des deux arcs est absent
    * **************************************************************/
    void CGRSupprimerArete(int iA, int iB);

    /* *************************************************************
    * GROAfficher (override/surcharge)
    * **************************************************************
    * Entrée  : Flux de sortie (cout par défaut)
    * Nécessite : Rien
    * Sortie  : Rien
    * Entraine : Affichage du graphe — chaque arête n'est affichée
    *            qu'une fois (convention : on n'affiche que les arcs
    *            où depart < arrivee pour éviter les doublons visuels)
    * **************************************************************/
    virtual void GROAfficher(ostream& os = cout) const override;
};

#endif // CGRAPH_H


// ================================================================
// IMPLÉMENTATIONS (obligatoirement dans le .h pour les templates)
// ================================================================

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::CGRAjouterArete(int iA, int iB)
{
    // Délègue entièrement à GROAjouterArc du parent.
    // GROAjouterArc gère déjà : vérification des sommets,
    // détection des doublons, mise à jour des listes de CSommet.
    CGraphOrient<TSommet, TArc>::GROAjouterArc(iA, iB);
    CGraphOrient<TSommet, TArc>::GROAjouterArc(iB, iA);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::CGRSupprimerArete(int iA, int iB)
{
    // Délègue entièrement à GROSupprimerArc du parent.
    // Si l'un des arcs est absent, GROSupprimerArc lève une
    // CException(GROEXC_ARC_ABSENT) — comportement cohérent.
    CGraphOrient<TSommet, TArc>::GROSupprimerArc(iA, iB);
    CGraphOrient<TSommet, TArc>::GROSupprimerArc(iB, iA);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
void CGraph<TSommet, TArc>::GROAfficher(ostream& os) const
{
    os << "Sommets :\n";
    for (const auto& paire : this->mipSOMGROListeSommets)
        os << "  Sommet " << paire.first << "\n";

    // On n'affiche que les arcs où depart < arrivee pour éviter
    // d'afficher deux fois chaque arête (A-B et B-A).
    os << "Aretes :\n";
    for (const TArc* pArc : this->vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() < pArc->ARCGetSommetArrivee())
            os << "  " << pArc->ARCGetSommetDepart()
            << " -- " << pArc->ARCGetSommetArrivee() << "\n";
    }
}