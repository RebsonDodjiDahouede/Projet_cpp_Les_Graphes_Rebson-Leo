#include "CGraphOrient.h"
#include <iostream>
#include <string>

using namespace std;

// -----------------------------------
// Outils de test
// -----------------------------------

static int g_iTestsTotal = 0;
static int g_iTestsResussis = 0;

static void TestAssert(bool bCondition, const string& SNomTest) {
	g_iTestsTotal++;
	if (bCondition) {
		g_iTestsResussis++;
		cout << "[OK ! PERFECT !]" << SNomTest << endl;
	}
	else
	{
		cout << "[FLOP] " << SNomTest << endl;
	}
}


// -----------------------------------
// Tests CArc
// -----------------------------------

static void TesterCArc()
{
	cout << "\n===== TESTS CArc =====" << endl;
	cout << "Création d'un arc lambda par défaut (0,0)..." << endl;
	CArc arcLambda;
	cout << "Test des getters pour arc Lambda..." << endl;
	TestAssert(arcLambda.ARCGetSommetDepart() == 0, "CArc initialise le sommet de départ à 0 par défaut.");
	TestAssert(arcLambda.ARCGetSommetArrivee() == 0, "CArc initialise le sommet d'arrivée à 0 par défaut.");

	cout << "Création de l'arc lambda 2 (3,7)..." << endl;
	CArc arcLambda2(3, 7);
	cout << "Test des getters pour arc Lambda 2..." << endl;
	TestAssert(arcLambda2.ARCGetSommetDepart() == 3, "CArc initialise le sommet de départ à 3 avec le constructeur.");
	TestAssert(arcLambda2.ARCGetSommetArrivee() == 7, "CArc initialise le sommet d'arrivée à 7 avec le constructeur.");

	cout << "Modification (Setters) de l'arc lambda 2 (3,7) -> (6,8)..." << endl;
	arcLambda2.ARCSetSommetDepart(6);
	arcLambda2.ARCSetSommetArrivee(8);

	cout << "Test des setters..." << endl;
	TestAssert(arcLambda2.ARCGetSommetDepart() == 6, "CArc change le sommet de départ à 6.");
	TestAssert(arcLambda2.ARCGetSommetArrivee() == 8, "CArc change le sommet d'arrivée à 8.");

	cout << "Création de l'arc (6,8)..." << endl;
	CArc arcLambda3(6, 8);
	cout << "Test de l'opérateur ==..." << endl;
	TestAssert(arcLambda2 == arcLambda3, "Les deux arcs lambda 2 & 3 sont bien égaux");
}

static void TesterSommet() {
	cout << "\n===== TESTS CSommet =====" << endl;

}

int main() {
	try
	{
		TesterCArc();
		TesterSommet();
	}
	catch (const std::exception&)
	{
		cout << "Une Exception" << endl;
	}
}