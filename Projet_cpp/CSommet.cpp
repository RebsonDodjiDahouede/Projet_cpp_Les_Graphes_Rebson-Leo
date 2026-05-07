#include "CSommet.h"

void CSommet::SOMSetNum(int iNum)
{
    iSOMNum = iNum;
}

void CSommet::SOMSupprimerArcEntrant(CArc* pArcEntrantASupprimer)
{
    auto it = find(vpSOMArcEntrants.begin(), vpSOMArcEntrants.end(), pArcEntrantASupprimer);
    if (it != vpSOMArcEntrants.end())
        vpSOMArcEntrants.erase(it);
}

void CSommet::SOMSupprimerArcSortant(CArc* pArcSortantASupprimer)
{
    auto it = find(vpSOMArcSortants.begin(), vpSOMArcSortants.end(), pArcSortantASupprimer);
    if (it != vpSOMArcSortants.end())
        vpSOMArcSortants.erase(it);
}