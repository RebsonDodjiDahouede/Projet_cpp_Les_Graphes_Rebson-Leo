#include "CSommet.h"

void CSommet::SOMSetNum(int iNum)
{
    for (CArc* arc : vpSOMArcEntrants) {
        arc->ARCSetSommetArrivee(iNum);
    }

    for (CArc* arc : vpSOMArcSortants) {
        arc->ARCSetSommetDepart(iNum);
    }

    iSOMNum = iNum;
}

CArc* CSommet::SOMGetArcEntrants(int iNum) const
{
    
}

CArc* CSommet::SOMGetArcSortants(int iNum) const
{
    
}

void CSommet::SOMAjouterArcEntrants(CArc* pArcDepart)
{
    vpSOMArcEntrants.push_back(pArcDepart);
}

void CSommet::SOMAjouterArcSortants(CArc* pArcArrivee)
{
}

void CSommet::SOMSupprimerArcEntrants(int iNum)
{
}

void CSommet::SOMSupprimerArcSortants(int iNum)
{
}
