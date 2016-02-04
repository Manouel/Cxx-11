#include "Etudiant.h"
#include <iostream>

using namespace std;


int main()
{
	vector<Etudiant> etudiants;
	
	
	etudiants.push_back(Etudiant("Lucie", {4, 6}));
	etudiants.push_back(Etudiant("Sophie", {10, 13}));
	etudiants.push_back(Etudiant("Pierre David", {18, 20}));
	
	float moyenne = 0.0;
	for (Etudiant e : etudiants)
	{
		moyenne += e.moyenne();
	}
	moyenne = moyenne / etudiants.size();
	
	cout << "Moyenne des étudiants : " << moyenne << endl;
	

	return 0;
}
