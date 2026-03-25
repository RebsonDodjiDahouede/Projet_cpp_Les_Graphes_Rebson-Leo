#ifndef CException_H
#define CException_H

/***************************************************************
* CLASSE : CException
* **************************************************************
* ROLE : Cette classe va permettre de lever des exceptions.
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 25/03/2026
* **************************************************************
* INCLUSIONS EXTERNES :
*/



/* std -> pour utiliser le stdout */
using namespace std;

/* TYPES :
*   Pas de type particulier de déclaré
*/

/* Variables :
*   Pas de Variable globale de déclaré
*/

#define FAUX 0
class CException {
    // ATTRIBUTS
private:
    unsigned int uiEXCValeur;

    // CONSTRUCTEURS ET DESTRUCTEURS
public:
    CException() { uiEXCValeur = FAUX; }
    CException(unsigned int uiValeur) { uiEXCValeur = uiValeur; }

    // MéTHODES
    /* *************************************************************
    * EXCLire
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : La valeur de l'erreur (strictement positive)
    * Entraine : La valeur de l'exception est renvoyé
    * **************************************************************/
    unsigned int EXCLire() const { return uiEXCValeur; };

    /* *************************************************************
    * EXCModifier
    * **************************************************************
    * Entrée : uiValeur, la nouvelle valeur
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : La mise à jour de la valeur de l'exception
    * **************************************************************/
    void EXCModifier(unsigned int uiValeur) { uiEXCValeur = uiValeur; };
};

#endif
