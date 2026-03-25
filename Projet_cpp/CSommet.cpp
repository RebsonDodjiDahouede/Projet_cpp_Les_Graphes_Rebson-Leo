#include "CSommet.h"

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
