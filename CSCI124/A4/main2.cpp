/**********************************************
* Main2.cpp - Contains implementation of errors
* Shannon Archer, 4220912, sja998, 29/9/12
***********************************************/

#include <iostream>
#include <cmath>

using namespace std;

double DoSquareRoot(double);

int main()
{
	double Number, Result;
	// Request a numbers from the user
	cout << "Please enter a number:";
	cin >> Number;
	try
	{
		Result = DoSquareRoot(Number);
		cout<<"\nThe square root of "<<Number<< " is "<<Result<<"\n";
	}
	catch (int ex)
	{
		cout<<"\nNo square root exists for that number, next time try entering something greater than zero.\n";
	}
	return 0;
}

double DoSquareRoot(double Number)
{
	if (Number < 0) throw 1;
	double Result = sqrt(Number);
	return Result;
}
