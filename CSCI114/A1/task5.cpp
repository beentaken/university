/* 
Name: Shannon
Lab: 17-106 8:30 - 10:30
Modification Date: 14/3/2012 12:58PM
File: Task 5 for assignment 1 - Enter three numbers and 
      print the result of a formula 

1.
This program will return incorrect results if x < y or if the product of x and y is equal to z.
2.
This happens as you cannot sqrt a negative number or divide by zero.

3.
What occurs is called a logic error, this error is not picked up by the compiler as the code is still legal c++.

4. 
DO
	DO 
		CIN X
		CIN Y
	WHILE x < y

	CIN Z	
	
WHILE  z EQUALS x * y
*/


#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	double x, y, z;

	cout << "Enter a number x = ";
	cin >> x;

	cout << "Enter a number y = ";
	cin >> y;

	cout << "Enter a number z = ";
	cin >> z;


	cout <<"sqrt(x - y) /(z - x * y) = " << sqrt(x - y) /(z - x * y) << endl;
	return 0;
}
