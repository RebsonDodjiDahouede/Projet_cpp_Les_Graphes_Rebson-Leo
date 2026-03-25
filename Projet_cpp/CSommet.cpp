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
