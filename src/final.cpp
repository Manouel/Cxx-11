#include <iostream>

using namespace std;

class A 
{
	public:
		virtual void f() final
		{
			cout << "A::f" << endl;	
		}
};

class B : public virtual A
{
	public:
		void f(int i)
		{
			cout << "B::f(" << i << ")" << endl;	
		}
};



int main()
{
	A a;
	a.f();
	
	B b;
	b.f(1);

	A *a2 = new A();
	a2->f();
	
	B *b2 = new B();
	b2->f(2);
	
	A *ab = new B();
	ab->f(3);

	return 0;
}
