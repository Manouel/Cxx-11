#include <iostream>
#include <unordered_map>

using namespace std;

struct Hash
{
	// Hash
	int operator()(string s) const
	{
		int i = 0;
	
		// Calcul d'un entier associé à la string
		for (int j = 0; j < s.length(); j++)
			i = i + (j+1) * s[j];
		
		return i;
	}
};


int main()
{
	unordered_map<string, int, Hash> map;
	
	map["Lucie"] = 2;
	map["Tabouret"] = 5;
	
	
	unordered_map<string, int, Hash>::iterator it;
	
	for (it = map.begin(); it != map.end(); it++)
	{
		cout << it->second << " ";
	}
	cout << endl;

	return 0;
}
