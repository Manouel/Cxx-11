#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <string>
#include <vector>

class Etudiant
{
	private:
		std::string nom;
		std::vector<int> notes;
		
	public:
		Etudiant(std::string nomE, std::vector<int> listeNotes);
		virtual ~Etudiant();
		virtual float moyenne() const;
};

#endif
