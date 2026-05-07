#ifndef GRAPH_FILE_READER_H
#define GRAPH_FILE_READER_H
/***************************************************************
* CLASSE : GraphFileReader
* **************************************************************
* ROLE : Module capable de lire un fichier text représentant
*		un graphe, sous la norme donnée dans le sujet, et créer
*		L'instance du graphe associée.
* **************************************************************
* VERSION : 1.0
* AUTEUR : Rebson & Leo
* DATE : 05/05/2026
* **************************************************************/

#include <string>
#include "CException.h"
#include "CGraph.h"

using namespace std;

// Codes d'exception 
#define GFREXC_FICHIER_INTROUVABLE    10
#define GFREXC_FORMAT_INVALIDE        11
#define GFREXC_NB_SOMMETS_INCOHERENT  12
#define GFREXC_NB_ARCS_INCOHERENT     13

class GraphFileReader
{
private:
    string sGFRFileName;

    /* *************************************************************
    * GFRExtraireEntier
    * **************************************************************
    * Entrée  : Une ligne et une clé (ex: "NBSommets", "Numero")
    * Nécessite : La ligne contient "cle=<entier>"
    * Sortie  : L'entier extrait après le '='
    * Entraine : Lance CException(GFREXC_FORMAT_INVALIDE) si introuvable
    * **************************************************************/
    int GFRExtraireEntier(const string& sLigne, const string& sCle) const;

    /* *************************************************************
    * GFRExtraireArc
    * **************************************************************
    * Entrée  : Une ligne du type "Debut=X, Fin=Y"
    * Nécessite : La ligne contient "Debut=" et "Fin="
    * Sortie  : iDebut et iFin par référence
    * Entraine : Lance CException(GFREXC_FORMAT_INVALIDE) si introuvable
    * **************************************************************/
    void GFRExtraireArc(const string& sLigne, int& iDebut, int& iFin) const;

public:

    /* *************************************************************
    * GraphFileReader
    * **************************************************************
    * Entrée  : Le chemin vers le fichier texte du graphe
    * Nécessite : Rien
    * Sortie  : Rien
    * Entraine : Initialise le nom de fichier
    * **************************************************************/
    GraphFileReader(const string& sParam) : sGFRFileName(sParam) {};
    ~GraphFileReader() = default;

    string& GFRGetFileName() { return sGFRFileName; };

    void GFRSetFileName(const string& sParam) { sGFRFileName = sParam; };

    /* *************************************************************
    * GFRCreateGraphFromTextFile
    * **************************************************************
    * Entrée  : Rien
    * Nécessite : Le fichier respecte le format imposé
    * Sortie  : Pointeur sur un CGraphOrient<CSommet,CArc>
    * Entraine : Lance CException si le fichier est introuvable,
    *            mal formé ou incohérent
    * **************************************************************/
    CGraphOrient<CSommet, CArc>* GFRCreateGraphFromTextFile() const;
};
#endif //GRAPH_FILE_READER_H

