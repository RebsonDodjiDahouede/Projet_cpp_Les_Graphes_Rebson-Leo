#ifndef CGRAPHORIENT_H
#define CGRAPHORIENT_H
/***************************************************************
* CLASSE : CGraphOrient
* **************************************************************
* ROLE : Cette classe va permettre de créer un objet Graphe orienté
* **************************************************************
* VERSION : 1.2
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

using namespace std;

#define GROEXC_SOMMET_DEJA_PRESENTE 2
#define GROEXC_SOMMET_ABSENT        3
#define GROEXC_SOMMET_ARC_ABSENT    4
#define GROEXC_ARC_DEJA_PRESENT     5
#define GROEXC_ARC_ABSENT           6

template<typename TSommet, typename TArc>
class CGraphOrient
{
    // ATTRIBUTS
    static_assert(is_base_of<CSommet, TSommet>::value, "TSommet doit hériter de CSommet");
    static_assert(is_base_of<CArc, TArc>::value, "TArc doit hériter de CArc");

protected:
    map<int, TSommet*> mipSOMGROListeSommets;
    vector<TArc*>      vpARCGROListeArcs;

public:

    // CONSTRUCTEURS
    CGraphOrient() = default;

    /* *************************************************************
    * ~CGraphOrient
    * **************************************************************
    * Correction bug #8 : on itère sur les paires de la map et on
    * delete paire.second (le TSommet*). L'ancienne version tentait
    * de copier pair<int,TSommet*> dans un TSommet — ne compilait pas.
    * **************************************************************/
    virtual ~CGraphOrient()
    {
        for (TArc* pArc : vpARCGROListeArcs)
            delete pArc;

        for (auto& paire : mipSOMGROListeSommets)
            delete paire.second;
    }

    // MÉTHODES

    /* *************************************************************
    * GROGetSommet
    * **************************************************************
    * Entrée : Entier
    * Nécessite : Rien
    * Sortie : Pointeur sur le sommet, nullptr si absent
    * Entraine : Rien
    * **************************************************************/
    TSommet* GROGetSommet(int iNum) const;  // Correction bug #10 : throw() retiré

    /* *************************************************************
    * GROAjouterSommet
    * **************************************************************
    * Entrée : Entier
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : Lance CException(GROEXC_SOMMET_DEJA_PRESENTE) si doublon
    * **************************************************************/
    void GROAjouterSommet(int iNum);        // Correction bug #10

    /* *************************************************************
    * GROModifierSommet
    * **************************************************************
    * Entrée : Ancien numéro, nouveau numéro
    * Nécessite : iOldNum présent, iNewNum absent
    * Sortie : Rien
    * Entraine : Lance CException si préconditions non respectées
    * **************************************************************/
    void GROModifierSommet(int iOldNum, int iNewNum);  // Correction bug #10

    /* *************************************************************
    * GROSupprimerSommet
    * **************************************************************
    * Entrée : Entier
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : Lance CException(GROEXC_SOMMET_ABSENT) si absent
    * **************************************************************/
    void GROSupprimerSommet(int iNum);      // Correction bug #10

    /* *************************************************************
    * GROAfficher
    * **************************************************************
    * Entrée : Flux de sortie (cout par défaut)
    * Nécessite : Rien
    * Sortie : Rien
    * Entraine : Affichage du graphe sur le flux
    * **************************************************************/
    virtual void GROAfficher(ostream& os = cout) const;

    /* *************************************************************
    * GROGetArcs
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Référence constante sur le vecteur des arcs
    * Correction bug #9 : méthode const retournant un const vector&
    * **************************************************************/
    const vector<TArc*>& GROGetArcs() const { return vpARCGROListeArcs; }

    /* *************************************************************
    * GROGetSommets
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Référence constante sur la map des sommets
    * Correction bug #9 : méthode const retournant un const map&
    * **************************************************************/
    const map<int, TSommet*>& GROGetSommets() const { return mipSOMGROListeSommets; }

    /* *************************************************************
    * GROAjouterArc
    * **************************************************************
    * Entrée : Numéro sommet de départ et d'arrivée
    * Nécessite : Les deux sommets existent dans le graphe
    * Sortie : Rien
    * Entraine : Lance CException si préconditions non respectées
    * **************************************************************/
    void GROAjouterArc(int iDepart, int iArrivee);

    /* *************************************************************
    * GROSupprimerArc
    * **************************************************************
    * Entrée : Numéro sommet de départ et d'arrivée
    * Nécessite : L'arc existe dans le graphe
    * Sortie : Rien
    * Entraine : Lance CException(GROEXC_ARC_ABSENT) si absent
    * **************************************************************/
    void GROSupprimerArc(int iDepart, int iArrivee);

    /* *************************************************************
    * GROInverser
    * **************************************************************
    * Entrée : Rien
    * Nécessite : Rien
    * Sortie : Pointeur sur un nouveau graphe avec tous les arcs inversés
    * Entraine : L'appelant est responsable du delete du graphe retourné
    * **************************************************************/
    CGraphOrient<TSommet, TArc>* GROInverser() const;

    /* *************************************************************
    * operator[]
    * **************************************************************
    * Entrée : Numéro du sommet
    * Nécessite : Le sommet existe (lance std::out_of_range sinon)
    * Sortie : Référence sur le sommet
    * Correction bug #11 : déréférencement du TSommet* retourné par at()
    * **************************************************************/
    TSommet& operator[](int iNumSommet) { return *mipSOMGROListeSommets.at(iNumSommet); }
};

#endif // CGRAPHORIENT_H


// ================================================================
// IMPLÉMENTATIONS
// (obligatoirement dans le .h pour les templates)
// ================================================================

template<typename TSommet, typename TArc>
inline TSommet* CGraphOrient<TSommet, TArc>::GROGetSommet(int iNum) const
{
    auto it = mipSOMGROListeSommets.find(iNum);
    return (it != mipSOMGROListeSommets.end()) ? it->second : nullptr;
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAjouterSommet(int iNum)
{
    if (mipSOMGROListeSommets.count(iNum))
    {
        CException except;
        except.EXCModifier(GROEXC_SOMMET_DEJA_PRESENTE);
        throw except;
    }
    mipSOMGROListeSommets[iNum] = new TSommet(iNum);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROModifierSommet(int iOldNum, int iNewNum)
{
    if (!mipSOMGROListeSommets.count(iOldNum))
    {
        CException except;
        except.EXCModifier(GROEXC_SOMMET_ABSENT);
        throw except;
    }
    if (mipSOMGROListeSommets.count(iNewNum))
    {
        CException except;
        except.EXCModifier(GROEXC_SOMMET_DEJA_PRESENTE);
        throw except;
    }

    TSommet* pSommet = mipSOMGROListeSommets[iOldNum];

    /* Correction bug #14 : pArc est un TArc* (pointeur),
       il faut utiliser -> et non . pour accéder aux membres.
       Correction bug #15 : SOMSetNum ne met plus à jour les arcs,
       on effectue la mise à jour ici via le vecteur global d'arcs
       — c'est CGraphOrient qui est responsable de la cohérence. */
    for (TArc* pArc : vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() == iOldNum)
            pArc->ARCSetSommetDepart(iNewNum);

        if (pArc->ARCGetSommetArrivee() == iOldNum)
            pArc->ARCSetSommetArrivee(iNewNum);
    }

    pSommet->SOMSetNum(iNewNum);            // Uniquement le numéro interne du sommet
    mipSOMGROListeSommets[iNewNum] = pSommet;
    mipSOMGROListeSommets.erase(iOldNum);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROSupprimerSommet(int iNum)
{
    auto itSommet = mipSOMGROListeSommets.find(iNum);
    if (itSommet == mipSOMGROListeSommets.end())
    {
        CException except;
        except.EXCModifier(GROEXC_SOMMET_ABSENT);
        throw except;
    }

    TSommet* pSommetASupprimer = itSommet->second;
    auto itArc = vpARCGROListeArcs.begin();

    while (itArc != vpARCGROListeArcs.end())
    {
        TArc* pArc = *itArc;
        if (pArc->ARCGetSommetDepart() == iNum || pArc->ARCGetSommetArrivee() == iNum)
        {
            /* On utilise GROGetSommet (retourne nullptr si absent)
               plutôt que operator[] (qui insèrerait un élément manquant). */
            TSommet* pSommetDepart = GROGetSommet(pArc->ARCGetSommetDepart());
            TSommet* pSommetArrivee = GROGetSommet(pArc->ARCGetSommetArrivee());

            if (pSommetDepart)  pSommetDepart->SOMSupprimerArcSortant(pArc);

            /* Correction bug #16 : la méthode s'appelle SOMSupprimerArcEntrant,
               pas SOMSupprimerArcArrivant. */
            if (pSommetArrivee) pSommetArrivee->SOMSupprimerArcEntrant(pArc);

            delete pArc;
            itArc = vpARCGROListeArcs.erase(itArc);
        }
        else
        {
            ++itArc;
        }
    }

    delete pSommetASupprimer;
    mipSOMGROListeSommets.erase(itSommet);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAfficher(ostream& os) const
{
    /* Correction bug #12 : "\\n" affichait littéralement \n.
       Les chaînes utilisent maintenant "\n" normalement. */
    os << "Sommets :\n";
    for (const auto& paire : mipSOMGROListeSommets)
        os << "  Sommet " << paire.first << "\n";

    os << "Arcs :\n";
    for (const TArc* pArc : vpARCGROListeArcs)
        os << "  " << pArc->ARCGetSommetDepart()
        << " -> " << pArc->ARCGetSommetArrivee() << "\n";
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline void CGraphOrient<TSommet, TArc>::GROAjouterArc(int iDepart, int iArrivee)
{
    TSommet* pDepart = GROGetSommet(iDepart);
    TSommet* pArrivee = GROGetSommet(iArrivee);

    if (!pDepart || !pArrivee)
    {
        CException except;
        except.EXCModifier(GROEXC_SOMMET_ARC_ABSENT);
        throw except;
    }

    for (TArc* pArc : vpARCGROListeArcs)
    {
        if (pArc->ARCGetSommetDepart() == iDepart &&
            pArc->ARCGetSommetArrivee() == iArrivee)
        {
            CException except;
            except.EXCModifier(GROEXC_ARC_DEJA_PRESENT);
            throw except;
        }
    }

    TArc* pNouvelArc = new TArc(iDepart, iArrivee);
    vpARCGROListeArcs.push_back(pNouvelArc);

    pDepart->SOMAjouterArcSortant(pNouvelArc);
    pArrivee->SOMAjouterArcEntrant(pNouvelArc);
}

// ----------------------------------------------------------------

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

    if (itArc == vpARCGROListeArcs.end())
    {
        CException except;
        except.EXCModifier(GROEXC_ARC_ABSENT);
        throw except;
    }

    TArc* pArc = *itArc;

    TSommet* pDepart = GROGetSommet(iDepart);
    TSommet* pArrivee = GROGetSommet(iArrivee);

    if (pDepart)  pDepart->SOMSupprimerArcSortant(pArc);

    /* Correction bug #17 : la méthode s'appelle SOMSupprimerArcEntrant,
       pas SOMSupprimerEntrant. */
    if (pArrivee) pArrivee->SOMSupprimerArcEntrant(pArc);

    delete pArc;
    vpARCGROListeArcs.erase(itArc);
}

// ----------------------------------------------------------------

template<typename TSommet, typename TArc>
inline CGraphOrient<TSommet, TArc>* CGraphOrient<TSommet, TArc>::GROInverser() const
{
    CGraphOrient<TSommet, TArc>* pGrapheInverse = new CGraphOrient<TSommet, TArc>();

    for (const auto& paire : mipSOMGROListeSommets)
        pGrapheInverse->GROAjouterSommet(paire.first);

    for (const TArc* pArc : vpARCGROListeArcs)
        pGrapheInverse->GROAjouterArc(pArc->ARCGetSommetArrivee(),
            pArc->ARCGetSommetDepart());

    return pGrapheInverse;
}