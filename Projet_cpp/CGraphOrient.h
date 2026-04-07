#ifndef CGRAPHORIENT_H
#define CGRAPHORIENT_H
/***************************************************************
* CLASSE : CSommet
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Graphe orienté
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/
#include <map>
#include <vector>
#include <iostream>
#include "CSommet.h"
#include "CArc.h"

/* std -> pour utiliser le stdout */
using namespace std;

/* TYPES :
*   Pas de type particulier de déclaré
*/

/* Variables :
*   Pas de Variable globale de déclaré
*/
#define VALEUR_DEJA_PRESENTE 2
#define VALEUR_ABSENTE 3

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
	void GROModifierSommet(int iNewNum) throw(CException);

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
	vector<Tarc*>& GROGetArcs() const { return vpARCGROListeArcs; };

	/* *************************************************************
	* GROGetSommets
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Le dictionnaire contenant les sommets du graphe
	* Entraine : Rien
	* **************************************************************/
	map<int, TSommet*>& GROGetSommets() const { return mipSOMGROListeSommets; };
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
	if (mipSOMGROListeSommets.find(iNum) != mipSOMGROListeSommets.end()) {
		CException except;
		except.EXCModifier(VALEUR_DEJA_PRESENTE);
		throw(except);
		
	}
	TSommet* pSommetAAjouter = new TSommet(iNum);
	mipSOMGROListeSommets[iNum] = pSommetAAjouter;
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROModifierSommet(int iNewNum) throw(CException)
{
	if (mipSOMGROListeSommets.find(iNum) != mipSOMGROListeSommets.end()) {
		CException except;
		except.EXCModifier(VALEUR_DEJA_PRESENTE);
		throw(except);
	}
}

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROSupprimerSommet(int iNum) throw(CException)
{
	auto itSommet = mipSOMGROListeSommets.find(iNum);
	if (itSommet == mipSOMGROListeSommets.end()) {
		CException except;
		except.EXCModifier(VALEUR_ABSENTE);
		throw(except);
	}
	TSommet* sommetASupprimer = itSommet->second;
	auto itArc = vpARCGROListeArcs.begin();

	while (itArc != vpARCGROListeArcs.end()) {
		TArc* pArc = *itArc;
		if (pArc->ARCGetSommetDepart() == iNum || pArc->ARCGetSommetArrivee() == iNum) {

			TSommet* pSommetDepart = mipSOMGROListeSommets[pArc->ARCGetSommetDepart()];
			TSommet* pSommetArrive = mipSOMGROListeSommets[pArc->ARCGetSommetArrivee()];

			pSommetDepart->SOMSupprimerArcSortant(pArc);
			pSommetArrive->SOMSupprimerArcArrivant(pArc);

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
