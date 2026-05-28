#ifndef CGRAPHPONDERE_H
#define CGRAPHPONDERE_H
/***************************************************************
* CLASSE : CGraphPondere
* **************************************************************
* ROLE : Graphe non orienté pondéré. Chaque arête est représentée
*        par deux arcs pondérés symétriques.
*        Cette classe fournit notamment l'algorithme de Prim.
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 19/05/2026
* **************************************************************/
#include <set>
#include <climits>
#include "CGraph.h"
#include "CArcPondere.h"
#include "CException.h"

using namespace std;

#define GRPEXC_GRAPHE_NON_CONNEXE 20

class CGraphPondere : public CGraph<CSommet, CArcPondere>
{
public:
    // CONSTRUCTEURS
    CGraphPondere() = default;
    virtual ~CGraphPondere() = default;

    /* *************************************************************
    * CGRAjouterAretePonderee
    * **************************************************************
    * Entrée : Numéros des deux sommets + poids
    * Nécessite : Les deux sommets existent
    * Sortie : Rien
    * Entraine : Ajoute deux arcs symétriques de même poids
    * **************************************************************/
    void CGRAjouterAretePonderee(int iNumA, int iNumB, int iPoids);

    /* *************************************************************
    * GRPPrim
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Graphe connexe
    * Sortie : Un nouveau graphe pondéré représentant l'arbre
    *          couvrant de poids minimal
    * Entraine : Lance une exception si le graphe n'est pas connexe
    * **************************************************************/
    CGraphPondere* GRPPrim() const;

    /* *************************************************************
    * GRPGetPoidsTotal
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Somme des poids des arêtes du graphe
    * Entraine : Dans un graphe non orienté représenté par deux arcs
    *            par arête, un seul sens est comptabilisé
    * **************************************************************/
    int GRPGetPoidsTotal() const;

    /* *************************************************************
    * GROAfficher
    * **************************************************************
    * Entrée : Flux de sortie
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : Affiche les arêtes une seule fois avec leur poids
    * **************************************************************/
    virtual void GROAfficher(ostream& os = cout) const override;
};

#endif // CGRAPHPONDERE_H