/*
	Shannon
	17-106 Wed, 8:30
	3.4.12
	Check if 11 characters representing a date match format DD-MON-YYY
*/
#include <iostream>

using namespace std;

int main()
{
	// vars
	short int day = -1;
	int year = -1;
	char d1, d2, m1, m2, m3, dash;

	// prompt input
	cout << "Enter a date in the format DD-MON-YYYY: " << endl;

	// get input
	cin >> d1 >> d2 >> dash;
	if (dash == '-')
	{
		cin >> m1 >> m2 >> m3 >> dash;
		if (dash == '-')
		{
			cin >> year;
		}
	}

	// get day as a number if DD are valid numbers
	if ((d1 - '0' >= 0 && d1 - '0' < 10) && (d2 - '0' >= 0 && d2 - '0' < 10))
		day = (d1 - '0') * 10 + d2 - '0';
	
	// if day and year > 0 both are valid numbers
	if (day > 0 && year >= 1000 && year < 10000)
	{
		// months with 31 days
		if (((m1 == 'J' && m2 == 'A' && m3 == 'N')
			|| (m1 == 'M' && m2 == 'A' && m3 == 'R')
			|| (m1 == 'M' && m2 == 'A' && m3 == 'Y')
			|| (m1 == 'J' && m2 == 'U' && m3 == 'L')
			|| (m1 == 'A' && m2 == 'U' && m3 == 'G')
			|| (m1 == 'O' && m2 == 'C' && m3 == 'T')
			|| (m1 == 'D' && m2 == 'E' && m3 == 'C'))
			&& day <= 31)
		{
			cout << "Date is valid." << endl;
			return 0;
		}
		// months with 30 days
		else if (((m1 == 'A' && m2 == 'P' && m3 == 'R')
			|| (m1 == 'J' && m2 == 'U' && m3 == 'N')
			|| (m1 == 'S' && m2 == 'E' && m3 == 'P')
			|| (m1 == 'N' && m2 == 'O' && m3 == 'V'))
			&& day <= 30)
		{
			cout << "Date is valid." << endl;
			return 0;
		}
		// february
		else if ((m1 == 'F' && m2 == 'E' && m3 == 'B')
			&& day <= 28)
		{
			cout << "Date is valid." << endl;
			return 0;
		}
	}
	
	// output date is invalid if valid date not found
	cout << "Date is invalid." << endl;
	return 0;
}
