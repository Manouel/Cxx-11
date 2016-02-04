#include "Etudiant.h"

using namespace std;

Etudiant::Etudiant(string nomE, vector<int> listeNotes) : nom(nomE), notes(listeNotes)
{

}

Etudiant::~Etudiant()
{

}

float Etudiant::moyenne() const
{
	float m = 0.0;
	for (int i = 0; i < notes.size(); i++)
	{
		m += notes[i];
	}
	
	m = m / notes.size();
	return m;
}
