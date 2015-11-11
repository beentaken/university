/*
	Shannon
	17-106 Wed, 8:30
	5.4.12
	Reads two positive integers and output all integer 
	numbers smaller than the threshold but greater than 9
*/
#include <iostream>

using namespace std;

int main()
{
	// vars
	int base;
	short int threshold;

	// get input
	cout << "Enter base in the range 100 to 999:" << endl;
	cin >> base;

	cout << "Enter threshold in the range 10 to 99:" << endl;
	cin >> threshold;

	// extract numbers
	short int d1, d2, d3;

	d1 = base / 100 % 10;
	d2 = base / 10 % 10;
	d3 = base % 10;

	// output variations
	cout << "Look at what I can make :D" << endl;

	if (d1 * 10 + d2 < threshold && d1 * 10 + d2 > 9) cout << d1 * 10 + d2 << " ";
	if (d1 * 10 + d3 < threshold && d1 * 10 + d3 > 9) cout << d1 * 10 + d3 << " ";
	if (d2 * 10 + d1 < threshold && d2 * 10 + d1 > 9) cout << d2 * 10 + d1 << " ";
	if (d2 * 10 + d3 < threshold && d2 * 10 + d3 > 9) cout << d2 * 10 + d3 << " ";
	if (d3 * 10 + d1 < threshold && d3 * 10 + d1 > 9) cout << d3 * 10 + d1 << " ";
	if (d3 * 10 + d2 < threshold && d3 * 10 + d2 > 9) cout << d3 * 10 + d2 << " ";	

	cout << endl;
}
