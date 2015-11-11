/*
	Shannon
	17-106, 8:30
	task5 - arrays and structures
*/
#include<iostream>
using namespace std;

struct Car
{
    char Make[20];
    char Model[20];
    int Year;
    float Price;
};

void createCar(Car &);
void getExpensive(Car[]);

int main()
{
	Car carInStock[5] = {{"Toyota", "Camry", 2002, 23290},
                         {"Holden", "Barina", 2001, 11000},
                         {"Mazda", "Astina", 1993, 8200},
                         {"Ford", "Fairmont", 1995, 8500}};

	return 0;
}

void createCar(Car &car)
{
	cout << "Enter the Make:";
	cin.getline(car.Make, 20);
	cout << "Enter the Model:";
	cin.getline(car.Model, 20);
	cout << "Enter the Year:";
	cin >> car.Year;
	cout << "Enter the Price:";
	cin >> car.Price;
}

void getExpensive(Car cars[])
{
	int exp = 0;
	for (int i = 0; i < sizeof(cars); i++)
	{
		if (cars[i].Price > cars[exp].Price)
			exp = i;
	}

	cout << "Most Expensive Car: " << cars[exp].Make << " "
	<< cars[exp].Model << " $" << cars[exp].Price << endl;
}
