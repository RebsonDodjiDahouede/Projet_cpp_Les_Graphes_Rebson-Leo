#include "CGraphPondere.h"


inline void CGraphPondere::CGRAjouterAretePonderee(int iNumA, int iNumB, int iPoids)
{
    CGraph<CSommet, CArcPondere>::CGRAjouterArete(iNumA, iNumB);

    // On va parcourir les arcs afin de lui affecter son poids
    for (CArcPondere* pArc : this->vpARCGROListeArcs)
    {
        if ((pArc->ARCGetSommetDepart() == iNumA && pArc->ARCGetSommetArrivee() == iNumB) ||
            (pArc->ARCGetSommetDepart() == iNumB && pArc->ARCGetSommetArrivee() == iNumA))
        {
            pArc->ARCSetPoids(iPoids);
        }
    }
}


inline CGraphPondere* CGraphPondere::GRPPrim() const
{
    CGraphPondere* pACM = new CGraphPondere();

    if (this->mipSOMGROListeSommets.empty())
        return pACM;

    for (const auto& paire : this->mipSOMGROListeSommets)
        pACM->GROAjouterSommet(paire.first);

    set<int> setSommetsDansArbre;

    int iSommetDepart = this->mipSOMGROListeSommets.begin()->first;
    setSommetsDansArbre.insert(iSommetDepart);

    while (setSommetsDansArbre.size() < this->mipSOMGROListeSommets.size())
    {
        CArcPondere* pMeilleurArc = nullptr;
        int iPoidsMin = INT_MAX;

        for (int iNumSommet : setSommetsDansArbre)
        {
            CSommet* pSommet = this->GROGetSommet(iNumSommet);
            const vector<CArc*>& vpArcsSortants = pSommet->SOMGetArcsSortants();

            for (CArc* pArcBase : vpArcsSortants)
            {
                CArcPondere* pArc = dynamic_cast<CArcPondere*>(pArcBase);
                if (!pArc)
                    continue;

                int iArrivee = pArc->ARCGetSommetArrivee();

                if (iArrivee == iNumSommet)
                    continue;

                if (setSommetsDansArbre.count(iArrivee) == 0 &&
                    pArc->ARCGetPoids() < iPoidsMin)
                {
                    iPoidsMin = pArc->ARCGetPoids();
                    pMeilleurArc = pArc;
                }
            }
        }

        if (!pMeilleurArc)
        {
            delete pACM;
            CException except;
            except.EXCModifier(GRPEXC_GRAPHE_NON_CONNEXE);
            throw except;
        }

        int iDepart = pMeilleurArc->ARCGetSommetDepart();
        int iArrivee = pMeilleurArc->ARCGetSommetArrivee();
        int iPoids = pMeilleurArc->ARCGetPoids();

        pACM->CGRAjouterAretePonderee(iDepart, iArrivee, iPoids);
        setSommetsDansArbre.insert(iArrivee);
    }

    return pACM;
}

inline int CGraphPondere::GRPGetPoidsTotal() const
{
    int iSomme = 0;

    for (const CArcPondere* pArc : this->vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() < pArc->ARCGetSommetArrivee())
            iSomme += pArc->ARCGetPoids();
    }

    return iSomme;
}

inline void CGraphPondere::GROAfficher(ostream& os) const
{
    os << "Sommets :\n";
    for (const auto& paire : this->mipSOMGROListeSommets)
        os << "  Sommet " << paire.first << "\n";

    os << "Aretes ponderees :\n";
    for (const CArcPondere* pArc : this->vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() < pArc->ARCGetSommetArrivee())
        {
            os << "  " << pArc->ARCGetSommetDepart()
                << " -- " << pArc->ARCGetSommetArrivee()
                << " (poids = " << pArc->ARCGetPoids() << ")\n";
        }
    }
}