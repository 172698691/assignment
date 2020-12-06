#include "mat.h"
#include <iostream>
using namespace std;

int main()
{
	mat a(2, 2);
	a.set();
	mat b=a;
	b = 1.1 * b;
	cout << a << endl;
	cout << b << endl;
	
	return 0;
}