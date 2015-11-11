/*
	Shannon
	17-106 Wed, 8:30
	3.4.12
	Get 5 numbers and sort them according to criteria then print them.
*/
#include <iostream>

using namespace std;

int main()
{
	// vars
	float one, two, three, four, five, sum;

	// get input
	cout << "Enter five numbers:" << endl;
	cin >> one >> two >> three >> four >> five;

	// calc sum
	sum = one + two + three + four + five;

	// sort if sum not 0
	if (sum != 0)
	{
		// bubble sort: ascending
		while (one > two || two > three || three > four || four > five)
		{
			// swap values
			if (one > two)
			{
				two = two - one;
				one = two + one;
				two = one - two;
			}

			if (two > three)
			{
				three = three - two;
				two = three + two;
				three = two - three;
			}

			if (three > four)
			{
				four = four - three;
				three = four + three;
				four = three - four;
			}

			if (four > five)
			{
				five = five - four;
				four = five + four;
				five = four - five;
			}
		}
	}

	// print result
	if (sum >= 0)
		cout << one << ", " << two << ", " << three << ", " << four << ", " << five << "." << endl;
	else
		cout << five << ", " << four << ", " << three << ", " << two << ", " << one << "." << endl;
}
