/*
	Shannon
	17-106, 8:30
	task1 - enumerated values

	i. 0
           1
           NORTH
           SOUTH
           EAST
           WEST

	j. same as i
*/

#include <iostream>

using namespace std;

enum direction { NORTH, SOUTH, EAST, WEST };

void printDirection(direction dir);

int main()
{
	
	direction dir = NORTH;
	cout << dir << endl;

	dir = SOUTH;
	cout << dir << endl;

	for (direction dir2 = NORTH; dir2 <= WEST; dir2 = static_cast<direction>(dir2+1))
		printDirection(dir2);
}

void printDirection(direction dir)
{
	switch (dir)
	{
		case NORTH:
			cout << "NORTH" << endl;
			break;
		case SOUTH:
			cout << "SOUTH" << endl;
			break;
		case EAST:
			cout << "EAST" << endl;
			break;
		case WEST:
			cout << "WEST" << endl;
			break;
	}
}
