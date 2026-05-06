#include "GraphFileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

int GraphFileReader::GFRExtraireEntier(const string& sLigne, const string& sCle) const
{
    // Cherche "cle=" dans la ligne
    string sCleEgal = sCle + "=";
    size_t pos = sLigne.find(sCleEgal);

    if (pos == string::npos)
    {
        CException except;
        except.EXCModifier(GFREXC_FORMAT_INVALIDE);
        throw except;
    }

    // Extrait la sous-chaîne après "cle=" et la convertit en entier
    // stoi s'arrête au premier caractère non numérique (virgule, espace...)
    try
    {
        return stoi(sLigne.substr(pos + sCleEgal.size()));
    }
    catch (const invalid_argument&)
    {
        CException except;
        except.EXCModifier(GFREXC_FORMAT_INVALIDE);
        throw except;
    }

}

void GraphFileReader::GFRExtraireArc(const string& sLigne, int& iDebut, int& iFin) const
{
    try
    {
        iDebut = GFRExtraireEntier(sLigne, "Debut");
        iFin = GFRExtraireEntier(sLigne, "Fin");
    }
    catch (CException&)
    {
        // Relance avec le même code — la ligne est mal formée
        CException except;
        except.EXCModifier(GFREXC_FORMAT_INVALIDE);
        throw except;
    }
}

CGraphOrient<CSommet, CArc>* GraphFileReader::GFRCreateGraphFromTextFile() const 
{

}

