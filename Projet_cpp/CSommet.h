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
#include "CArc.h"

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
	~CSommet() = default;

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
	const vector<CArc*>& SOMGetArcEntrants() { return vpSOMArcEntrants; };

	/* *************************************************************
	* SOMGetArcSortants
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : Une référence sur le vecteur des arcs sortants
	* Entraine : Rien
	* **************************************************************/
	const vector<CArc*>& SOMGetArcSortants() { return vpSOMArcSortants; };


	void SOMAjouterArcEntrants(CArc* pArcDepart);
	void SOMAjouterArcSortants(CArc* pArcArrivee);
	void SOMSupprimerArcEntrants(int iNum);
	void SOMSupprimerArcSortants(int iNum);

};
#endif //CSOMMET_H

