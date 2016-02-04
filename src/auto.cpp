#include <iostream>
#include <string>

using namespace std;


//auto h() -> decltype(1);

//auto f(int i) ;

int main()
{
	cout << "float : " << sizeof(float) << "o." << endl;
	cout << "double : " << sizeof(double) << "o." << endl;

	auto f = 3.2;
	cout << "auto : " << sizeof(f) << "o." << endl;


  auto i = 5;
  

  const auto *v = &x, u = 6;

  static auto y = 0.0;
  
  auto ii = 3;
  
  auto g() -> decltype(ii) { return 0; }
  auto h() -> decltype(ii);



  
  int x;  

  
  cout << "g() = " << g() << endl;
  cout << "h() = " << h() << endl;
  
  
  auto j = 2;
  
  auto h() -> decltype(1)   		int h() { return 1; }
  auto h() -> decltype(1) 			int h() -> decltype(1) { return 1; }
  
  
  string str = "str";
  auto f(int i) -> decltype(str) {if (i%2==0) return "pair"; else return "";}
  
  auto k = 2; k = 1.2;
  auto l = 4.2; l = 3;
  auto x = 5, *y = &x;
  auto a = 5, b = "cinq";
  
  
  int i;
  auto j = 2;
  
  decltype(i) v;
  decltype(j) v2;
  
  
  
  //cout << "f() = " << f(2) << endl;
  
  auto k = 2;
  int m = 1.2;
  
  cout << k+m << endl;
  
  int l = 1.2;
  
  cout << l << endl;
  
  auto w = 5, *z = &w;

  return 0;
}

/*
auto f(int i)
{
  if (i % 2 == 0)
    return "pair";
  else
    return;
}*/
