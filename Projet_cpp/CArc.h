#ifndef CARC_H
#define CARC_H
/***************************************************************
* CLASSE : CArc
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Arc de graphes 
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/
#include <iostream>

/* std -> pour utiliser le stdout */
using namespace std;

/* TYPES :
*   Pas de type particulier de déclaré
*/

/* Variables :
*   Pas de Variable globale de déclaré
*/

class CArc
{
	// ATTRIBUTS
protected:
	int iARCSommetDepart;
	int iARCSommetArrivee;

	// CONSTRUCTEURS
public:
	CArc() : iARCSommetArrivee(0), iARCSommetDepart(0) {};
	CArc(int iSommetDepart, int iSommetArrivee) : iARCSommetDepart(iSommetDepart), iARCSommetArrivee(iSommetArrivee) {};
	virtual ~CArc() = default;

	// MÉTHODES
	/* *************************************************************
	* ARCGetSommetDepart
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : La valeur du sommet de départ de l'arc
	* Entraine : rien
	* **************************************************************/
	int ARCGetSommetDepart() const { return iARCSommetDepart; };

	/* *************************************************************
	* ARCGetSommetArrivé
	* **************************************************************
	* Entrée : Rien
	* Nécessite : Rien
	* Sortie : La valeur du sommet d'arrivée de l'arc
	* Entraine : rien
	* **************************************************************/
	int ARCGetSommetArrivee() const { return iARCSommetArrivee; };

	/* *************************************************************
	* ARCGetSommetDepart
	* **************************************************************
	* Entrée : La nouvelle valeur du sommet de départ de l'arc
	* Nécessite : Rien 
	* Sortie : Rien
	* Entraine : La modification des la valeur du sommet de départ
	* **************************************************************/
	void ARCSetSommetDepart(int iSommetDepart) { iARCSommetDepart = iSommetDepart; };

	/* *************************************************************
	* ARCGetSommetArrivee
	* **************************************************************
	* Entrée : La nouvelle valeur du sommet d'arrivée de l'arc
	* Nécessite : Rien
	* Sortie : Rien
	* Entraine : La modification des la valeur du sommet d'arrivée
	* **************************************************************/
	void ARCSetSommetArrivee(int iSommetArrivee) { iARCSommetArrivee = iSommetArrivee; };

	/* *************************************************************
	* operator==
	* **************************************************************
	* Entrée : Référence sur un objet de la classe
	* Nécessite : Rien
	* Sortie : Bool si les attributs sont égaux
	* Entraine : La modification des la valeur du sommet d'arrivée
	* **************************************************************/
	bool operator==(const CArc& ARCParam) const { return iARCSommetArrivee == ARCParam.iARCSommetArrivee && iARCSommetDepart == ARCParam.iARCSommetDepart; };
};
#endif //CARC_H
