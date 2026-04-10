#ifndef CSOMMET_H
#define CSOMMET_H
/***************************************************************
* CLASSE : CSommet
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Sommet de graphes
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/
#include <vector>
#include <iostream>
#include <algorithm>
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

class CSommet
{
	// ATTRIBUTS
protected :
	int iSOMNum;
	vector<CArc*> vpSOMArcEntrants;
	vector<CArc*> vpSOMArcSortants;
	
	// CONSTRUCTEURS
public :
	CSommet() : iSOMNum(0) {};
	CSommet(int iNum) : iSOMNum(iNum) {};
	virtual ~CSommet() = default;

	// MÉTHODES
	/* *************************************************************
	* SOMGetNum
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : La valeur du Nombre du sommet 
	* Entraine : Rien
	* **************************************************************/
	int SOMGetNum() const { return iSOMNum; };

	/* *************************************************************
	* SOMSetNum
	* **************************************************************
	* Entrée : Un entier
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : La modification de la valeur du nombre du sommet
	* **************************************************************/
	void SOMSetNum(int iNum);

	/* *************************************************************
	* SOMGetArcEntrants
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Une référence sur le vecteur des arcs entrants
	* Entraine : Rien
	* **************************************************************/
	const vector<CArc*>& SOMGetArcsEntrants() { return vpSOMArcEntrants; };

	/* *************************************************************
	* SOMGetArcSortants
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Une référence sur le vecteur des arcs sortants
	* Entraine : Rien
	* **************************************************************/
	const vector<CArc*>& SOMGetArcsSortants() { return vpSOMArcSortants; };

	/* *************************************************************
	* SOMAjouterArcEntrant
	* **************************************************************
	* Entrée : Un pointeur sur l'arc
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : L'ajout d'un arc entrant dans le vecteur des arcs entrants
	* **************************************************************/
	void SOMAjouterArcEntrant(CArc* pArcEntrant) { vpSOMArcEntrants.push_back(pArcEntrant); };

	/* *************************************************************
	* SOMAjouterArcSortant
	* **************************************************************
	* Entrée : Un pointeur sur l'arc
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : L'ajout d'un arc sortant dans le vecteur des arcs sortant
	* **************************************************************/
	void SOMAjouterArcSortant(CArc* pArcSortant) { vpSOMArcSortants.push_back(pArcSortant); };

	/* *************************************************************
	* SOMSupprimerArcEntrant
	* **************************************************************
	* Entrée : Un pointeur sur l'arc
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : La suppression d'un arc entrant dans le vecteur des arcs entrant
	* **************************************************************/
	void SOMSupprimerArcEntrant(CArc* pArcEntrantASupprimer) { vpSOMArcEntrants.erase(find(vpSOMArcEntrants.begin(), vpSOMArcEntrants.end(), pArcEntrantASupprimer)); };

	/* *************************************************************
	* SOMSupprimerArcSortant
	* **************************************************************
	* Entrée : Un pointeur sur l'arc
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : La suppression d'un arc sortant dans le vecteur des arcs sortants
	* **************************************************************/
	void SOMSupprimerArcSortant(CArc* pArcSortantASupprimer) { vpSOMArcSortants.erase(find(vpSOMArcSortants.begin(), vpSOMArcSortants.end(), pArcSortantASupprimer)); };

	/* *************************************************************
	* operator==
	* **************************************************************
	* Entrée : Une référence sur un objet CSommet
	* Nécessite : Rien
	* Sortie : Un bool
	* Entraine : Rien
	* **************************************************************/
	bool operator==(CSommet& SOMParam) { return iSOMNum == SOMParam.iSOMNum && vpSOMArcEntrants == SOMParam.vpSOMArcEntrants && vpSOMArcSortants == SOMParam.vpSOMArcSortants; };

};
#endif //CSOMMET_H

