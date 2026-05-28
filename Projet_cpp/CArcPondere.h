#ifndef CARCPONDERE_H
#define CARCPONDERE_H
/***************************************************************
* CLASSE : CArcPondere
* **************************************************************
* ROLE : Représente un arc pondéré, c'est-à-dire un arc orienté
*        auquel on associe un poids entier.
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 19/05/2026
* **************************************************************/
#include "CArc.h"

using namespace std;

class CArcPondere : public CArc
{
protected:
    int iARCPoids;

public:
    // CONSTRUCTEURS
    CArcPondere() : CArc(), iARCPoids(0) {}

    CArcPondere(int iDepart, int iArrivee, int iPoids) : CArc(iDepart, iArrivee), iARCPoids(iPoids) {}

    virtual ~CArcPondere() = default;

    // ACCESSEURS
    int ARCGetPoids() const { return iARCPoids; }

    // MUTATEURS
    void ARCSetPoids(int iPoids) { iARCPoids = iPoids; }

    // OPERATEUR
    bool operator==(const CArcPondere& ARCParam) const
    {
        return (ARCGetSommetDepart() == ARCParam.ARCGetSommetDepart() && ARCGetSommetArrivee() == ARCParam.ARCGetSommetArrivee() && iARCPoids == ARCParam.iARCPoids);
    }
};

#endif // CARCPONDERE_H