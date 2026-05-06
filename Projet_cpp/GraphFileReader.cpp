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
    ifstream fichier(sGFRFileName);
    if (!fichier.is_open())
    {
        CException except;
        except.EXCModifier(GFREXC_FICHIER_INTROUVABLE);
        throw except;
    }

    string sLigne;
    int iNBSommets = 0;
    int iNBArcs = 0;

    // ---- Lecture NBSommets ----
    if (!getline(fichier, sLigne))
    {
        CException except;
        except.EXCModifier(GFREXC_FORMAT_INVALIDE);
        throw except;
    }
    iNBSommets = GFRExtraireEntier(sLigne, "NBSommets");

    // ---- Lecture NBArcs ----
    if (!getline(fichier, sLigne))
    {
        CException except;
        except.EXCModifier(GFREXC_FORMAT_INVALIDE);
        throw except;
    }
    iNBArcs = GFRExtraireEntier(sLigne, "NBArcs");

    CGraphOrient<CSommet, CArc>* pGraphe = new CGraphOrient<CSommet, CArc>();

    try
    {
        // ---- Section Sommets=[ ----
        if (!getline(fichier, sLigne) || sLigne.find("Sommets=[") == string::npos)
        {
            CException except;
            except.EXCModifier(GFREXC_FORMAT_INVALIDE);
            throw except;
        }

        int iCompteurSommets = 0;
        while (getline(fichier, sLigne) && sLigne.find(']') == string::npos)
        {
            // Ignore les lignes vides
            if (sLigne.empty()) continue;

            int iNum = GFRExtraireEntier(sLigne, "Numero");
            pGraphe->GROAjouterSommet(iNum);
            iCompteurSommets++;
        }

        // Vérification de cohérence
        if (iCompteurSommets != iNBSommets)
        {
            CException except;
            except.EXCModifier(GFREXC_NB_SOMMETS_INCOHERENT);
            throw except;
        }

        // ---- Section Arcs=[ ----
        if (!getline(fichier, sLigne) || sLigne.find("Arcs=[") == string::npos)
        {
            CException except;
            except.EXCModifier(GFREXC_FORMAT_INVALIDE);
            throw except;
        }

        int iCompteurArcs = 0;
        while (getline(fichier, sLigne) && sLigne.find(']') == string::npos)
        {
            if (sLigne.empty()) continue;

            int iDebut = 0;
            int iFin = 0;
            GFRExtraireArc(sLigne, iDebut, iFin);
            pGraphe->GROAjouterArc(iDebut, iFin);
            iCompteurArcs++;
        }

        // Vérification de cohérence
        if (iCompteurArcs != iNBArcs)
        {
            CException except;
            except.EXCModifier(GFREXC_NB_ARCS_INCOHERENT);
            throw except;
        }
    }
    catch (...)
    {
        // Nettoyage mémoire si une erreur survient après new
        delete pGraphe;
        throw; // Relance l'exception originale vers l'appelant
    }

    return pGraphe;
}

