/*
	Shannon
	17-106, 8:30
	25/5
	Main file of ass4 - rpg
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "functions.h"

using namespace std;

int main()
{
	// seed
	srand(time(NULL));	

	// vars
	int map[MapDim][MapDim];
	player players[2];
	bool running = true;

	int option = 0;
	if (SaveFileExists())
	{
		while (option < 1 || option > 2)
		{
			cout << "Do you want to:\n1) Restart the game?\n2) Continue the game?" << endl;
			cin >> option;
		}
	}

	ReadMap(map, players, option == 2);
	ReadWeapons(players, option == 2);

	// run game
	while (running)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!players[i].gameover)
			{
				// get input
				char input;
				cout << "\nWhich way do you want to go " << players[i].name  << "?" << endl;
				cin >> input;

				// move
				if (input == 'n' || input == 's' || input == 'e' || input == 'w')
				{
					MovePlayer(map, players[i], input);

					// print map
					for (int y = 0; y < 10; y++)
					{
						for (int x = 0; x < 10; x++)
						{
							if (!players[0].gameover && players[0].x == x && players[0].y == y)
								cout << Red << "p1" << Console;
							else if (!players[1].gameover && players[1].x == x && players[1].y == y)
								cout << Yellow << "p2" << Console;
							else 
								cout << map[y][x];
							cout << "\t";
						}
						cout << endl;
					}
				}
				// display quit
				else if (input == 'q')
				{
					int displayOption;
					do
					{
						displayOption = DisplayQuit();
						if (displayOption == 1)
						{
							WriteMap(map, players);
							WriteWeapons(players);
						}
						else if (displayOption == 2)
							return 0;
						else if (displayOption == 3)
							break;
					} while (displayOption < 1 || displayOption > 3);
				}
				else 
				{
					cout << "error: invalid instruction." << endl;
					continue;
				}

				if (players[0].x == players[1].x && players[0].y == players[1].y && !players[0].gameover && !players[1].gameover)
				{
					if (i == 0) Duel(players[0], players[1]);
					else if (i == 1) Duel(players[1], players[0]);
				}
				else 
					RunEvent(map, players[i]);

				if (map[players[i].y][players[i].x] == 999)
				{
					cout << players[i].name << " have survived the dungeon. " << players[i].name << " wins!" << endl;
					return 0;
				}
			}
		}

		if (players[0].gameover && players[1].gameover) 
		{
			cout << "Both Players have failed. The end." << endl;
			return 0;
		}
	}
}


