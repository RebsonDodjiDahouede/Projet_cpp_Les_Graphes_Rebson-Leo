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

#include "CException.h"

using namespace std;

// Codes d'exception 
#define GFREXC_FICHIER_INTROUVABLE    10
#define GFREXC_FORMAT_INVALIDE        11
#define GFREXC_NB_SOMMETS_INCOHERENT  12
#define GFREXC_NB_ARCS_INCOHERENT     13

class GraphFileReader
{
};
#endif //GRAPH_FILE_READER_H

