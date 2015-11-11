/* 
Name: Shannon
Lab: 17-106 8:30 - 10:30
Modification Date: 14/3/2012 12:58PM
File: Task 6 for assignment 1 - Enter two numbers and print
      the sum in the form of K MMM CC
*/

#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
	// init vars
	long int first, second;

	// get first length
	cout << "Enter first line length: " << endl;
	cin >> first;
	
	// get second length
 	cout << "Enter second line length: " << endl;
	cin >> second;
	
	// calc sum
	long int sum = first + second;

	// extract values
	int k = (sum / 100000), m = (sum / 100) % 1000, cm = sum % 100;
	
	// print sum
	cout << "Sum is equal to " << k << " " << setw(3) << setfill('0') << m << " " << setw(2) << setfill('0') << cm << "." << endl;
	
	// exit
	return 0;
}
