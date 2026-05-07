#include <iostream>
#include <string>
#include "CGraph.h"   // inclut CGraphOrient.h, CSommet.h et CArc.h
#include "GraphFileReader.h"

using namespace std;

// ================================================================
// Outils de test
// ================================================================
static int g_iTestsTotal = 0;
static int g_iTestsReussis = 0;

static void TestAssert(bool bCondition, const string& sNomTest)
{
    g_iTestsTotal++;
    if (bCondition)
    {
        g_iTestsReussis++;
        cout << "[OK]    " << sNomTest << "\n";
    }
    else
    {
        cout << "[ECHEC] " << sNomTest << "\n";
    }
}

// ================================================================
// Tests CArc — tout est public, on teste directement
// ================================================================
static void TesterCArc()
{
    cout << "\n===== TESTS CArc =====\n";

    // Constructeur par defaut
    CArc arcDefaut;
    TestAssert(arcDefaut.ARCGetSommetDepart() == 0, "CArc() : sommet de depart initialise a 0");
    TestAssert(arcDefaut.ARCGetSommetArrivee() == 0, "CArc() : sommet d'arrivee initialise a 0");

    // Constructeur avec parametres
    CArc arc1(3, 7);
    TestAssert(arc1.ARCGetSommetDepart() == 3, "CArc(3,7) : sommet de depart = 3");
    TestAssert(arc1.ARCGetSommetArrivee() == 7, "CArc(3,7) : sommet d'arrivee = 7");

    // Setters
    arc1.ARCSetSommetDepart(10);
    arc1.ARCSetSommetArrivee(20);
    TestAssert(arc1.ARCGetSommetDepart() == 10, "ARCSetSommetDepart : valeur modifiee a 10");
    TestAssert(arc1.ARCGetSommetArrivee() == 20, "ARCSetSommetArrivee : valeur modifiee a 20");

    // Operateur ==
    CArc arc2(10, 20);
    TestAssert(arc1 == arc2, "operator== : deux arcs egaux retourne true");

    CArc arc3(1, 2);
    TestAssert(!(arc1 == arc3), "operator== : deux arcs differents retourne false");
}

// ================================================================
// Tests CSommet seulement ce qui est public est testé directement.
// Les arcs (protected) sont testés via CGraphOrient.
// ================================================================
static void TesterCSommet()
{
    cout << "\n===== TESTS CSommet (partie publique) =====\n";

    // Constructeur par defaut
    CSommet sommetDefaut;
    TestAssert(sommetDefaut.SOMGetNum() == 0, "CSommet() : numero initialise a 0");

    // Constructeur avec numero
    CSommet sommet5(5);
    TestAssert(sommet5.SOMGetNum() == 5, "CSommet(5) : numero initialise a 5");

    // Listes vides par defaut
    TestAssert(sommet5.SOMGetArcsEntrants().empty(), "CSommet : arcs entrants vides a la creation");
    TestAssert(sommet5.SOMGetArcsSortants().empty(), "CSommet : arcs sortants vides a la creation");
}

// ================================================================
// Tests CSommet via CGraphOrient
// (listes d'arcs entrants/sortants, impossible a tester autrement)
// ================================================================
static void TesterCSommetViaGraphe()
{
    cout << "\n===== TESTS CSommet (via CGraphOrient) =====\n";

    CGraphOrient<CSommet, CArc> graphe;
    graphe.GROAjouterSommet(1);
    graphe.GROAjouterSommet(2);
    graphe.GROAjouterSommet(3);
    graphe.GROAjouterArc(1, 2);
    graphe.GROAjouterArc(3, 1);

    CSommet* pS1 = graphe.GROGetSommet(1);
    CSommet* pS2 = graphe.GROGetSommet(2);
    CSommet* pS3 = graphe.GROGetSommet(3);

    // Sommet 1 : arc sortant vers 2, arc entrant depuis 3
    TestAssert(pS1->SOMGetArcsSortants().size() == 1,
        "Sommet 1 : 1 arc sortant apres ajout de l'arc (1->2)");
    TestAssert(pS1->SOMGetArcsEntrants().size() == 1,
        "Sommet 1 : 1 arc entrant apres ajout de l'arc (3->1)");

    // Sommet 2 : 1 arc entrant depuis 1, aucun sortant
    TestAssert(pS2->SOMGetArcsEntrants().size() == 1,
        "Sommet 2 : 1 arc entrant apres ajout de l'arc (1->2)");
    TestAssert(pS2->SOMGetArcsSortants().empty(),
        "Sommet 2 : aucun arc sortant");

    // Sommet 3 : 1 arc sortant vers 1, aucun entrant
    TestAssert(pS3->SOMGetArcsSortants().size() == 1,
        "Sommet 3 : 1 arc sortant apres ajout de l'arc (3->1)");
    TestAssert(pS3->SOMGetArcsEntrants().empty(),
        "Sommet 3 : aucun arc entrant");

    // Verification du contenu des arcs dans les listes
    TestAssert(pS1->SOMGetArcsSortants()[0]->ARCGetSommetArrivee() == 2,
        "Sommet 1 : l'arc sortant pointe bien vers le sommet 2");
    TestAssert(pS1->SOMGetArcsEntrants()[0]->ARCGetSommetDepart() == 3,
        "Sommet 1 : l'arc entrant provient bien du sommet 3");

    // Suppression d'un arc -> verification de la mise a jour des listes
    graphe.GROSupprimerArc(1, 2);
    TestAssert(pS1->SOMGetArcsSortants().empty(),
        "Sommet 1 : liste sortants vide apres suppression arc (1->2)");
    TestAssert(pS2->SOMGetArcsEntrants().empty(),
        "Sommet 2 : liste entrants vide apres suppression arc (1->2)");
}

// ================================================================
// Tests CGraphOrient
// ================================================================
static void TesterCGraphOrient()
{
    cout << "\n===== TESTS CGraphOrient =====\n";

    CGraphOrient<CSommet, CArc> graphe;

    // --- Ajout de sommets ---
    graphe.GROAjouterSommet(1);
    graphe.GROAjouterSommet(2);
    graphe.GROAjouterSommet(3);

    TestAssert(graphe.GROGetSommet(1) != nullptr, "GROGetSommet : sommet 1 trouve");
    TestAssert(graphe.GROGetSommet(99) == nullptr, "GROGetSommet : sommet inexistant retourne nullptr");
    TestAssert(graphe.GROGetSommets().size() == 3, "GROGetSommets : 3 sommets dans le graphe");

    // --- Exception : doublon de sommet ---
    bool bExceptionSommetDouble = false;
    try { graphe.GROAjouterSommet(1); }
    catch (CException&) { bExceptionSommetDouble = true; }
    TestAssert(bExceptionSommetDouble, "GROAjouterSommet : exception levee si sommet deja present");

    // --- Ajout d'arcs ---
    graphe.GROAjouterArc(1, 2);
    graphe.GROAjouterArc(2, 3);
    graphe.GROAjouterArc(3, 1);
    TestAssert(graphe.GROGetArcs().size() == 3, "GROAjouterArc : 3 arcs dans le graphe");

    // --- Exception : arc vers sommet inexistant ---
    bool bExceptionArcSommetAbsent = false;
    try { graphe.GROAjouterArc(1, 99); }
    catch (CException&) { bExceptionArcSommetAbsent = true; }
    TestAssert(bExceptionArcSommetAbsent, "GROAjouterArc : exception si sommet absent");

    // --- Exception : arc en double ---
    bool bExceptionArcDouble = false;
    try { graphe.GROAjouterArc(1, 2); }
    catch (CException&) { bExceptionArcDouble = true; }
    TestAssert(bExceptionArcDouble, "GROAjouterArc : exception si arc deja present");

    // --- Modification de sommet ---
    graphe.GROModifierSommet(3, 10);
    TestAssert(graphe.GROGetSommet(3) == nullptr, "GROModifierSommet : l'ancien numero disparait");
    TestAssert(graphe.GROGetSommet(10) != nullptr, "GROModifierSommet : le nouveau numero existe");

    // Verification que les arcs sont mis a jour
    bool bArc2vers10 = false;
    bool bArc10vers1 = false;
    for (CArc* pArc : graphe.GROGetArcs())
    {
        if (pArc->ARCGetSommetDepart() == 2 && pArc->ARCGetSommetArrivee() == 10) bArc2vers10 = true;
        if (pArc->ARCGetSommetDepart() == 10 && pArc->ARCGetSommetArrivee() == 1)  bArc10vers1 = true;
    }
    TestAssert(bArc2vers10, "GROModifierSommet : arc (2->3) renomme en (2->10)");
    TestAssert(bArc10vers1, "GROModifierSommet : arc (3->1) renomme en (10->1)");

    // --- Suppression d'arc ---
    graphe.GROSupprimerArc(1, 2);
    TestAssert(graphe.GROGetArcs().size() == 2, "GROSupprimerArc : 2 arcs restants apres suppression");

    // Exception : arc absent
    bool bExceptionArcAbsent = false;
    try { graphe.GROSupprimerArc(1, 2); }
    catch (CException&) { bExceptionArcAbsent = true; }
    TestAssert(bExceptionArcAbsent, "GROSupprimerArc : exception si arc absent");

    // --- Suppression de sommet ---
    graphe.GROSupprimerSommet(10);
    TestAssert(graphe.GROGetSommet(10) == nullptr, "GROSupprimerSommet : sommet supprime");
    TestAssert(graphe.GROGetSommets().size() == 2, "GROSupprimerSommet : 2 sommets restants");
    TestAssert(graphe.GROGetArcs().size() == 0, "GROSupprimerSommet : l'arc lie au sommet est supprime");

    // Exception : sommet absent
    bool bExceptionSommetAbsent = false;
    try { graphe.GROSupprimerSommet(10); }
    catch (CException&) { bExceptionSommetAbsent = true; }
    TestAssert(bExceptionSommetAbsent, "GROSupprimerSommet : exception si sommet absent");

    // --- Inversion ---
    CGraphOrient<CSommet, CArc> graphe2;
    graphe2.GROAjouterSommet(1);
    graphe2.GROAjouterSommet(2);
    graphe2.GROAjouterSommet(3);
    graphe2.GROAjouterArc(1, 2);
    graphe2.GROAjouterArc(2, 3);

    CGraphOrient<CSommet, CArc>* pInverse = graphe2.GROInverser();

    TestAssert(pInverse->GROGetSommets().size() == 3, "GROInverser : meme nombre de sommets");
    TestAssert(pInverse->GROGetArcs().size() == 2, "GROInverser : meme nombre d'arcs");

    bool bTrouve2vers1 = false;
    bool bTrouve3vers2 = false;
    for (CArc* pArc : pInverse->GROGetArcs())
    {
        if (pArc->ARCGetSommetDepart() == 2 && pArc->ARCGetSommetArrivee() == 1) bTrouve2vers1 = true;
        if (pArc->ARCGetSommetDepart() == 3 && pArc->ARCGetSommetArrivee() == 2) bTrouve3vers2 = true;
    }
    TestAssert(bTrouve2vers1, "GROInverser : arc (1->2) devient (2->1)");
    TestAssert(bTrouve3vers2, "GROInverser : arc (2->3) devient (3->2)");

    delete pInverse;
}

// ================================================================
// Demonstration : scenario complet proche du main du sujet
// ================================================================
static void DemonstrationGraphe()
{
    cout << "\n===== DEMONSTRATION =====\n";

    CGraphOrient<CSommet, CArc> graphe;
    graphe.GROAjouterSommet(1);
    graphe.GROAjouterSommet(2);
    graphe.GROAjouterSommet(3);
    graphe.GROAjouterArc(1, 2);
    graphe.GROAjouterArc(2, 3);
    graphe.GROAjouterArc(3, 1);

    cout << "\n--- Graphe original ---\n";
    graphe.GROAfficher();

    CGraphOrient<CSommet, CArc>* pInverse = graphe.GROInverser();
    cout << "\n--- Graphe inverse ---\n";
    pInverse->GROAfficher();

    delete pInverse;
}

static void TesterCGraph()
{
    cout << "\n===== TESTS CGraph =====\n";

    CGraph<CSommet, CArc> graphe;
    graphe.GROAjouterSommet(1);
    graphe.GROAjouterSommet(2);
    graphe.GROAjouterSommet(3);

    graphe.CGRAjouterArete(1, 2);
    graphe.CGRAjouterArete(2, 3);

    // Une arête = deux arcs internes
    TestAssert(graphe.GROGetArcs().size() == 4,
        "CGraph : 2 aretes = 4 arcs internes");

    // Les deux sens sont bien présents
    CSommet* pS1 = graphe.GROGetSommet(1);
    CSommet* pS2 = graphe.GROGetSommet(2);
    TestAssert(pS1->SOMGetArcsSortants().size() == 1,
        "CGraph : sommet 1 a 1 arc sortant");
    TestAssert(pS1->SOMGetArcsEntrants().size() == 1,
        "CGraph : sommet 1 a 1 arc entrant (depuis 2)");
    TestAssert(pS2->SOMGetArcsSortants().size() == 2,
        "CGraph : sommet 2 a 2 arcs sortants (vers 1 et 3)");

    // Suppression d'une arête
    graphe.CGRSupprimerArete(1, 2);
    TestAssert(graphe.GROGetArcs().size() == 2,
        "CGraph : suppression arête 1-2 -> 2 arcs restants");
    TestAssert(pS1->SOMGetArcsSortants().empty(),
        "CGraph : sommet 1 n'a plus d'arc sortant");

    // Affichage (vérification visuelle)
    cout << "\n--- Affichage CGraph ---\n";
    graphe.GROAfficher();
}

static bool MainTestFunction() 
{
    cout << "===== LANCEMENT DES TESTS =====\n";

    try
    {
        TesterCArc();
        TesterCSommet();
        TesterCSommetViaGraphe();
        TesterCGraphOrient();
        DemonstrationGraphe();
        TesterCGraph();
    }
    catch (CException& e)
    {
        cout << "Erreur lors des tests, (code : "
            << e.EXCLire() << ")\n";
        return 1;
    }

    cout << "\n===== BILAN =====\n";
    cout << g_iTestsReussis << " / " << g_iTestsTotal << " tests reussis.\n";

    return (g_iTestsReussis == g_iTestsTotal) ? 0 : 1;
}

// ================================================================
// main
// ================================================================
int main()
{
    string sNomFichier;

    cout << "Entrez le nom du fichier graphe : ";
    cin >> sNomFichier;

    try
    {
        GraphFileReader reader(sNomFichier);
        CGraphOrient<CSommet, CArc>* pGraphe = reader.GFRCreateGraphFromTextFile();

        cout << "\n--- Graphe original ---\n";
        pGraphe->GROAfficher();

        CGraphOrient<CSommet, CArc>* pInverse = pGraphe->GROInverser();

        cout << "\n--- Graphe inverse ---\n";
        pInverse->GROAfficher();

        delete pInverse;
        delete pGraphe;
    }
    catch (CException& e)
    {
        cout << "Erreur lors de la lecture du fichier (code : "
            << e.EXCLire() << ")\n";
        return 1;
    }
    cout << "\nAppuyez sur Entrée pour quitter...";
    cin.ignore();
    cin.get();
    return 0;
}