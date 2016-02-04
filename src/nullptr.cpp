#include <iostream>

using namespace std;

bool pair_int(int i)
{
  cout << "test pair/impair ";
  return (i%2 == 0);
}

bool pair_int(int *i)
{
  if (!i)
  {
    cout << "pas d'entier à traiter ";
    return true;
  }
  else
    return (*i)%2 == 0;
}

int main()
{
  int i = 2;
  cout << pair_int(i) << endl;
  
  int *pi = new int;
  *pi = 3;
  
  cout << pair_int(*pi) << endl;
  cout << pair_int(NULL) << endl;
  cout << pair_int(nullptr) << endl;

  return 0;
}



