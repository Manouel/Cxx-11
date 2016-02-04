#include <iostream>

using namespace std;

class B
{
  public:
    B(int value)
    {
      std::cout << "B constructor : " << value << std::endl;
    }

};

class D : public B
{
  public:
    using B::B;
 
};


int main()
{
  B b(1);
  D d(2);

  B *b2 = new B(3);
  D *d2 = new D(4);
  
  B *b3 = new D(5);

  return 0;
}

