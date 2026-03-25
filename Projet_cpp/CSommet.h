#ifndef CSOMMET_H
#define CSOMMET_H
#pragma once

#include <vector>
#include <iostream>
#include "CArc.h"

using namespace std;


class CSommet
{
private :
	int iSOMNum;
	vector<CArc*> vpSOMArcEntrants;
	vector<CArc*> vpSOMArcSortants;

public :
	CSommet() : iSOMNum(0) {};
	CSommet(int iNum) : iSOMNum(iNum) {};
	~CSommet() = default;

	int SOMGetNum() const { return iSOMNum; };
	void SOMSetNum(int iNum) { iSOMNum = iNum; };
	CArc* SOMGetArcEntrants(int iNum) const;
	CArc* SOMGetArcSortants(int iNum) const;

	void SOMAjouterArcEntrants(CArc* pArcDepart);
	void SOMAjouterArcSortants(CArc* pArcArrivee);
	void SOMSupprimerArcEntrants(int iNum);
	void SOMSupprimerArcSortants(int iNum);

};
#endif //CSOMMET_H

