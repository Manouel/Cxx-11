#include <iostream>

using namespace std;

class A 
{
	public:
		virtual void f(float f1)
		{
			cout << "A::f(" << f1 << ")" << endl;
		}
};
	
class B : public virtual A 
{
	public:
		virtual void f(float f2) override
		{
			cout << "B::f(" << f2 << ")" << endl;
		}
};

int main()
{
 	A a;
 	a.f(1.1);
 	
 	B b;
 	b.f(2.2);
 	
 	A *a2 = new A();
 	a2->f(3.3);
 	
 	B *b2 = new B();
 	b2->f(4.4);
 	
 	A *ab = new B();
 	ab->f(5.5);

	return 0;
}
