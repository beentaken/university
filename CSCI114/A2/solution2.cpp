/*
	Shannon
	17-106 Wed, 8:30
	3.4.12
	Get temperature and scale then output the equivalant temperature in other scales.
*/
#include <iostream>

using namespace std;

int main()
{
	// vars
	float temperature;
	char scale;

	// get input
	cout << "Please enter the temperature to convert:" << endl;
	cin >> temperature;

	cout << "Please enter the scale to convert from (C, K, F):" << endl;
	cin.ignore(20, '\n');	
	cin.get(scale);

	// print equivalant temperature in other scales
	switch (scale)
	{
		case 'C':
			cout << "The equivalent Kelvin is " << temperature + 273.15f << " degrees." << endl;
			cout << "The equivalent Fahrenheit is " << temperature * 9.0 / 5.0 + 32 << " degrees." << endl;
			break;
		case 'K':
			cout << "The equivalent Celsius is " << temperature - 273.15f << " degrees." << endl;
			cout << "The equivalent Fahrenheit is " << temperature * 9.0 / 5.0 - 459.67f << " degrees." << endl;
			break;
		case 'F':
			cout << "The equivalent Kelvin is " << (temperature + 459.67f) * 5.0 / 9.0 << " degrees."  << endl;
			cout << "The equivalent Celsius is " << (temperature - 32) * 5.0 / 9.0 << " degrees." << endl;
			break;
	}
}
