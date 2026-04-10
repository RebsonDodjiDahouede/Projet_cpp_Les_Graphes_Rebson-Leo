#ifndef CGRAPHORIENT_H
#define CGRAPHORIENT_H
/***************************************************************
* CLASSE : CSommet
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Graphe orienté
* **************************************************************
* VERSION : 1.1
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "CSommet.h"
#include "CArc.h"
#include "CException.h"

/* std -> pour utiliser le stdout */
using namespace std;

/* TYPES :
*   Pas de type particulier de déclaré
*/

/* Variables :
*   Pas de Variable globale de déclaré
*/
#define GROEXC_SOMMET_DEJA_PRESENTE 2
#define GROEXC_SOMMET_ABSENT 3
#define GROEXC_SOMMET_ARC_ABSENT 4
#define GROEXC_ARC_DEJA_PRESENT 5
#define GROEXC_ARC_ABSENT 6

template<typename TSommet, typename TArc>
class CGraphOrient
{
	// ATTRIBUTS
	static_assert(is_base_of<CSommet, TSommet>::value,
		"TSommet doit hérité de CSommet");
	static_assert(is_base_of<CArc, TArc>::value,
		"TArc doit hérité de CArc");
protected:
	map<int, TSommet*> mipSOMGROListeSommets;
	vector<TArc*> vpARCGROListeArcs;

public:

	// CONSTRUCTEURS
	CGraphOrient() = default;
	virtual ~CGraphOrient() {
		for (TArc* arc : vpARCGROListeArcs) {
			delete arc;
		}
		for (TSommet sommet : mipSOMGROListeSommets) {
			delete sommet;
		}
	};

	// MÉTHODES
	/* *************************************************************
	* GROAjouterSommet
	* **************************************************************
	* Entrée : Entier
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : Lance une exception si la valeur est déjà présente
	* **************************************************************/
	TSommet* GROGetSommet(int iNum) const throw(CException);

	/* *************************************************************
	* GROAjouterSommet
	* **************************************************************
	* Entrée : Entier
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : Lance une exception si la valeur est déjà présente
	* **************************************************************/
	void GROAjouterSommet(int iNum) throw(CException);

	/* *************************************************************
	* GROModifierSommet
	* **************************************************************
	* Entrée : Entier
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : Lance une exception si la valeur est déjà présente
	* **************************************************************/
	void GROModifierSommet(int OldNum, int iNewNum) throw(CException);

	/* *************************************************************
	* GROSupprimerSommet
	* **************************************************************
	* Entrée : Entier
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : Lance une exception si la valeur est absente
	* **************************************************************/
	void GROSupprimerSommet(int iNum) throw(CException);

	/* *************************************************************
	* GROAfficher
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : Rien
	* **************************************************************/
	virtual void GROAfficher(ostream& os = cout) const;

	/* *************************************************************
	* GROGetArcs
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Le vecteur contenant les pointeurs vers les arcs du graphe
	* Entraine : Rien
	* **************************************************************/
	vector<TArc*>& GROGetArcs() const { return vpARCGROListeArcs; };

	/* *************************************************************
	* GROAjouterArcs
	* **************************************************************
	* Entrée : Num sommet de départ et d'arrivée
	* Nécessite : Rien
	* Sortie : Le vecteur contenant les pointeurs vers les arcs du graphe
	* Entraine : Rien
	* **************************************************************/
	void GROAjouterArc(int iDepart, int iArrivee);

	/* *************************************************************
	* GROAjouterArcs
	* **************************************************************
	* Entrée : Num sommet de départ et d'arrivée
	* Nécessite : Rien
	* Sortie : Le vecteur contenant les pointeurs vers les arcs du graphe
	* Entraine : Rien
	* **************************************************************/
	void GROSupprimerArc(int iDepart, int iArrivee); 

	/* *************************************************************
	* GROAjouterArcs
	* **************************************************************
	* Entrée : Num sommet de départ et d'arrivée
	* Nécessite : Rien
	* Sortie : Le vecteur contenant les pointeurs vers les arcs du graphe
	* Entraine : Rien
	* **************************************************************/
	CGraphOrient<TSommet, TArc>* GROInverser() const;

	/* *************************************************************
	* GROGetSommets
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Le dictionnaire contenant les sommets du graphe
	* Entraine : Rien
	* **************************************************************/
	map<int, TSommet*>& GROGetSommets() const { return mipSOMGROListeSommets; };

	/* *************************************************************
	* Operator[]
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Le dictionnaire contenant les sommets du graphe
	* Entraine : Rien
	* **************************************************************/
	TSommet& operator[](int iNumSommet) { return mipSOMGROListeSommets.at(iNumSommet); }

};
#endif //CGRAPHORIENT_H

template<typename TSommet, typename TArc>
inline TSommet* CGraphOrient<TSommet, TArc>::GROGetSommet(int iNum) const
{
	auto it = mipSOMGROListeSommets.find(iNum);
	return (it != mipSOMGROListeSommets.end()) ? it->second : nullptr;
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAjouterSommet(int iNum) throw (CException)
{
	if (mipSOMGROListeSommets.count(iNum)) {
		CException except;
		except.EXCModifier(GROEXC_SOMMET_DEJA_PRESENTE);
		throw(except);
	}
	TSommet* pSommetAAjouter = new TSommet(iNum);
	mipSOMGROListeSommets[iNum] = pSommetAAjouter;
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROModifierSommet(int iOldNum, int iNewNum) throw(CException)
{
	if (!mipSOMGROListeSommets.count(iOldNum)) {
		CException except;
		except.EXCModifier(GROEXC_SOMMET_ABSENT);
		throw(except);
	};
	if (mipSOMGROListeSommets.count(iNewNum)) {
		CException except;
		except.EXCModifier(GROEXC_SOMMET_DEJA_PRESENTE);
		throw(except);
	};
	TSommet* pSommet = mipSOMGROListeSommets[iOldNum];
	pSommet->SOMSetNum(iNewNum);

	for (TArc* pArc : vpARCGROListeArcs) {
		if (pArc.ARCGetSommetDepart() == iOldNum) {
			pArc.ARCSetSommetDepart(iNewNum);
		}
		if (pArc.ARCGetSommetArrivee() == iOldNum) {
			pArc.ARCSetSommetArrivee(iNewNum);
		}
	}

	mipSOMGROListeSommets[iNewNum] = pSommet;
	mipSOMGROListeSommets.erase(iOldNum);
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROSupprimerSommet(int iNum) throw(CException)
{
	auto itSommet = mipSOMGROListeSommets.find(iNum);
	if (itSommet == mipSOMGROListeSommets.end()) {
		CException except;
		except.EXCModifier(GROEXC_SOMMET_ABSENT);
		throw(except);
	}
	TSommet* sommetASupprimer = itSommet->second;
	auto itArc = vpARCGROListeArcs.begin();

	while (itArc != vpARCGROListeArcs.end()) {
		TArc* pArc = *itArc;
		if (pArc->ARCGetSommetDepart() == iNum || pArc->ARCGetSommetArrivee() == iNum) {

			TSommet* pSommetDepart = mipSOMGROListeSommets[pArc->ARCGetSommetDepart()];
			TSommet* pSommetArrive = mipSOMGROListeSommets[pArc->ARCGetSommetArrivee()];

			if (pSommetDepart) { pSommetDepart->SOMSupprimerArcSortant(pArc); }
			if (pSommetArrive) { pSommetArrive->SOMSupprimerArcArrivant(pArc); }

			delete pArc;
			itArc = vpARCGROListeArcs.erase(itArc);
		}
		else {
			itArc++;
		}
	}

	delete sommetASupprimer;
	mipSOMGROListeSommets.erase(itSommet)
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAfficher(ostream& os) const
{
	os << "Sommets :\n";
	for (const auto& pair : mipSOMGROListeSommets) {
		os << "	Sommet " << pair.first << "\n";
	}
	os << "Arcs: \n";
	for (const TArc* pArc : vpARCGROListeArcs) {
		os << "	" << pArc->ARCGetSommetDepart()
			<< " -> " << pArc->ARCGetSommetArrivee() << "\n";
	}
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAjouterArc(int iDepart, int iArrivee)
{
	TSommet* pDepart = GROGetSommet(iDepart);
	TSommet* pArrivee = GROGetSommet(iArrivee);

	if (!pDepart || !pArrivee) {
		CException except;
		except.EXCModifier(GROEXC_SOMMET_ARC_ABSENT);
		throw(except);
	}

	for (TArc* pArc : vpARCGROListeArcs) {
		if (pArc->ARCGetSommetDepart() == iDepart && pArc->ARCGetSommetArrivee() == iArrivee) {
			CException except;
			except.EXCModifier(GROEXC_ARC_DEJA_PRESENT);
			throw(except);
		}
	}

	TArc* pNouvelArc = new TArc(iDepart, iArrivee);
	vpARCGROListeArcs.push_back(pNouvelArc);

	pDepart->SOMAjouterArcSortant(pNouvelArc);
	pArrivee->SOMAjouterArcEntrant(pNouvelArc);

}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROSupprimerArc(int iDepart, int iArrivee)
{
	auto itArc = find_if(
		vpARCGROListeArcs.begin(),
		vpARCGROListeArcs.end(),
		[iDepart, iArrivee](TArc* pArc) {
			return pArc->ARCGetSommetDepart() == iDepart &&
				pArc->ARCGetSommetArrivee() == iArrivee;
		}
	);

	if (itArc == vpARCGROListeArcs.end()) {
		CException except;
		except.EXCModifier(GROEXC_ARC_ABSENT);
		throw(except);
	}

	TArc* pArc = *itArc;

	TSommet* pDepart = GROGetSommet(iDepart);
	TSommet* pArrivee = GROGetSommet(iArrivee);

	if (pDepart) { pDepart->SOMSupprimerArcSortant(pArc); };
	if (pArrivee) { pArrivee->SOMSupprimerEntrant(pArc); };

	delete pArc;
	vpARCGROListeArcs.erase(itArc);
}

template<typename TSommet, typename TArc>
inline CGraphOrient<TSommet, TArc>* CGraphOrient<TSommet, TArc>::GROInverser() const
{
	CGraphOrient<TSommet, TArc>* pGrapheInverse = new CGraphOrient();

	for (const auto& paire : mipSOMGROListeSommets) {
		pGrapheInverse->GROAjouterSommet(paire.first)
	}

	for (const TArc* pArc : vpARCGROListeArcs) {
		pGrapheInverse->GROAjouterArc(pArc->ARCGetSommetArrivee(), pArc->ARCGetSommetDepart());
	}
	
	return pGrapheInverse;
	
}
