#include <iostream>
#include <list>
#include <algorithm>

using namespace std;


void erathostene(int b)
{
	list<int> l;
	for (int i = 2; i < b; i++)
		l.push_back(i);
		
	while (!l.empty())
	{
		int g = *l.begin();
		cout << g << " - ";
		list<int>::iterator it = remove_if(l.begin(), l.end(), [g](int a) {
			return (a % g == 0);
		});
		l.erase(it, l.end());
	}
	
	cout << endl;
}

int main()
{
	list<char> lili;
	lili.push_back('a');
	lili.push_back('b');
	lili.push_back('c');
	
	list<char>::iterator it;
	for (it = lili.begin(); it != lili.end(); it++)
	{
		cout << *it << endl;
	}
	
	
	for (int i = 0; i < 2; i++)
	{
		for_each(lili.begin(), lili.end(), [](char& c) {
        	c++;
    	});
    }
    
    
    
	for (it = lili.begin(); it != lili.end(); it++)
	{
		cout << *it << endl;
	}
	
	erathostene(10);

	return 0;
}



