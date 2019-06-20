#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {
	Polynomial p1 {"4x^3 -6x^2 +2x -3"};
	Polynomial p2 {"x + 2"};

	cout << "p1 =      " << p1 << endl;
	cout << "p2 =      " << p2 << endl;
	cout << "p1 + p2 = " << p1 + p2 << endl;
	cout << "p1 - p2 = " << p1 - p2 << endl;
	cout << "p1 * p2 = " << p1 * p2 << endl;
	cout << "p1 / p2 = (quotient) " << (p1 / p2).first << endl;
	cout << "p1 / p2 = (remainder) " << (p1 / p2).second << endl;
	cout << "Integral of p1 = " << Polynomial::integral(p1) << endl;
	cout << "Derivative of p1 = " << Polynomial::derivative(p1) << endl;
	
	return 0;
}
