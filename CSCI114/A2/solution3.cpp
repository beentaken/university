/*
	Shannon
	17-106 Wed, 8:30
	3.4.12
	Get positive integer number no longer then 5 digits and list the total number of zero digits, odd digits and even digits.
*/
#include <iostream>

using namespace std;

int main()
{
	// vars
	int num = 0, zero = 0, odd = 0, even = 0;

	// prompt for input
	cout << "Enter a positive number no greater than 99,999:" << endl;
	cin >> num;

	// check if input meets conditions
	if (num >= 0 && num <= 99999)
	{	
		for (int i=1; i<num; i*=10)
		{
			// get digit
			int digit = num / i % 10; 
		
			// count digit
			if (digit == 0) zero++;
			if (digit % 2 == 0) even++;
			else odd++;
		}
	}
	else
		cout << "Silly, thats not a positive number no greater than 99,999." << endl;

	// output counts
	cout << "Total zeroes: " << zero << endl;
	cout << "Total odd numbers: " << odd << endl;
	cout << "Total even numbers: " << even << endl;
}
