#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> v;
	vector<int>::iterator it;
	
	v.push_back(4);
	v.push_back(7);
	v.push_back(3);
	v.push_back(8);
	
	for (it = begin(v); it != end(v); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	return 0;
}
