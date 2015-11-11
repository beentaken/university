/* 
Name: Shannon
Lab: 17-106 8:30 - 10:30
Modification Date: 14/3/2012 12:58PM
File: Task 3 - A C++ program given below reads an integer number 4 digits long and        	displays the largest digit and position of the digit. 

      For example an input 4562 produces the output:
      The largest digit is 6 at position 3.
*/

#include <iostream>

using namespace std;

int main()
{
	int number;
 	int digit1, digit2, digit3, digit4;
 	int largest, position;

	cout << "Enter a number 4 digits long" << endl; cin >> number;

	if ( number < 1000 || number > 9999 )
	{
		cout << "Incorrect number !" << endl;
		return 1;
	}

	digit4 = number % 10;
	digit3 = (number/10)%10;
	digit2 = (number/100)%10;
	digit1 = (number/1000)%10;

	largest = digit1;
	position = 1;

	if (largest < digit2)
	{
		largest = digit2;
		position = 2;
	}
	if (largest < digit3)
	{
		largest = digit3;
		position = 3;
	}
	if (largest < digit4)
	{
		largest = digit4;
		position = 4;
	}

	cout << "The largest digit is " << largest << " at position " << position << endl;

	return 0;
}
