#ifndef CARC_H
#define CARC_H

#include <iostream>

using namespace std;

class CArc
{
private:
	int iARCSommetDepart;
	int iARCSommetArrivee;

public:
	CArc() : iARCSommetArrivee(0), iARCSommetDepart(0) {};
	CArc(int iSommetDepart, int iSommetArrivee) : iARCSommetDepart(iSommetDepart), iARCSommetArrivee(iSommetArrivee) {};
	~CArc() = default;

	int ARCGetSommetDepart() const { return iARCSommetDepart; };
	int ARCGetSommetArrivee() const { return iARCSommetArrivee; };
	void ARCSetSommetDepart(int iSommetDepart) {iARCSommetDepart = iSommetDepart; };
	void ARCSetSommetArrivee(int iSommetArrivee) { iARCSommetArrivee = iSommetArrivee; };
};
#endif //CARC_H
