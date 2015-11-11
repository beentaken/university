/*
	Shannon
	17-106, 8:30
	25/5
	Function file of ass4 - rpg
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include "functions.h"

using namespace std;

///
//    Read data from map file
///
void ReadMap(int map[][MapDim], player p[], bool cont)
{
	// get file
	ifstream infile;
	if (cont)
		infile.open("currentMap.txt");
	else
		infile.open("map.txt");

	// read in player pos
	for (int i = 0; i < 2; i++)
	{
		infile >> p[i].x;
		infile >> p[i].y;
		if (cont)
			infile >> p[i].hp;
		else 
			p[i].hp = 500;

		p[i].gameover = false;
	}
	strcpy(p[0].name, "\033[01;31mPlayer1\033[0m");
	strcpy(p[1].name, "\033[01;33mPlayer2\033[0m");

	// read in rest of map
	for (int i = 0; i < MapDim; i++)
	{
		for (int j = 0; j < MapDim; j++) 
		{
			int mapvalue;
			infile >> mapvalue;
			// if 9 generate random
			if (mapvalue == 9)
				mapvalue = rand() % 7 + 1;

			map[i][j] = mapvalue;
		}
	}

	infile.close();
}

///
//    Read data from weapon files
///
void ReadWeapons(player p[], bool cont)
{
	// get file
	ifstream infile;
	
	for (int j = 0; j < 2; j++)
	{
		// open appropriate file
		infile.close();
		if (cont)
		{
			if (j == 0)
				infile.open("p1CurrentWeapon.txt");
			else if (j == 1)
				infile.open("p2CurrentWeapon.txt");
		}
		else 
			infile.open("weapon.txt");
		
		// get weapons
		for (int i = 0; i < WeaponCount; i++)
		{
			infile.getline(p[j].weapons[i].name, 100);
			infile >> p[j].weapons[i].strength;
			infile.get();
		}

		// randomise the weapons list
		for (int i = 0; i < 20; i++)
		{
			int first = (rand() % WeaponCount), second = (rand() % WeaponCount);

			weapon temp = p[j].weapons[first];
			p[j].weapons[first] = p[j].weapons[second];
			p[j].weapons[second] = temp;
		}

		// equip weapon to player
		p[j].equipped = 0;
		for (int i = 1; i < WeaponCount; i++)
		{
			if (p[j].weapons[p[j].equipped].strength <= 0)
				p[j].equipped = i;
			else 
				break;
		}
	}

	infile.close();
}

///
//   Write data to map file
///
void WriteMap(int map[][MapDim], player p[])
{
	ofstream outfile("currentMap.txt");

	// write player values
	outfile << p[0].x << " " << p[0].y << " " << p[0].hp << " " << p[1].x << " " << p[1].y << " " << p[1].hp << "\n";

	// write map values
	for (int i = 0; i < MapDim; i++)
	{
		for (int j = 0; j < MapDim; j++)
		{
			outfile << map[i][j] << " ";
		}
		outfile << "\n";
	}

	outfile.close();
}

///
//   Write data to weapons files
///
void WriteWeapons(player p[])
{
	ofstream outfile;

	for (int j = 0; j < 2; j++)
	{
		outfile.close();
		if (j == 0)
			outfile.open("p1CurrentWeapon.txt");
		else
			outfile.open("p2CurrentWeapon.txt");

		// write weapons
		for (int i = 0; i < WeaponCount; i++)
		{
			outfile << p[j].weapons[i].name << "\n" << p[j].weapons[i].strength << "\n";
		}
	}
	outfile.close();
}

///
//   Move player to a square
///
void MovePlayer(int map[][MapDim], player& p, char input)
{
	bool moved = false;
	
	switch (input)
	{
		case 'n':
			if (p.y - 1 >= 0 && map[p.y - 1][p.x] >= 0)
			{
				p.y--;
				p.hp -= 5;
				moved = true;
			}
			else
				cout << p.name << " cannot go north right now." << endl;
			break;
		case 's':
			if (p.y + 1 < MapDim && map[p.y + 1][p.x] >= 0)
			{
				p.y++;
				p.hp -= 5;
				moved = true;
			}
			else
				cout << p.name << " cannot go south right now." << endl;
			break;
		case 'e':
			if (p.x + 1 < MapDim && map[p.y][p.x + 1] >= 0)
			{
				p.x++;
				p.hp -= 5;
				moved = true;
			}
			else 
				cout << p.name << " cannot go east right now." << endl;
			break;
		case 'w':
			if (p.x - 1 >= 0 && map[p.y][p.x - 1] >= 0)
			{
				p.x--;
				p.hp -= 5;
				moved = true;
			}
			else
				cout << p.name << " cannot go west right now." << endl;
			break;
		default:
			cout << "Movement not recognised." << endl;
			break;
	}

	if (moved)
		cout << p.name << " has " << Blue << p.hp << Console << "HP left." << endl;
}

///
//    Run the event for the player
///
void RunEvent(int map[][MapDim], player& p)
{
	// 50+ weapon strength
	if (map[p.y][p.x] == 1)
	{
		p.weapons[p.equipped].strength += 50;
		map[p.y][p.x] = 0;

		cout << "Bonus! " << p.name << " adds" << Green << " 50" << Console << " strength to " << Magenta << p.weapons[p.equipped].name 
		     << "\n" << p.weapons[p.equipped].name << Console << " 's strength is " << Blue << p.weapons[p.equipped].strength << Console << " ." << endl;
	}
	// Monster fight
	else if (map[p.y][p.x] > 1 && map[p.y][p.x] < 5)
	{
		monster m;
		m.hp = rand() % 500 + 1;

		cout << p.name << " encounters a" << LightMagenta << " monster" << Console << " with " << Blue << m.hp << Console << " HP." << endl;
		while (m.hp > 0 && !p.gameover)
		{
			int damage = p.weapons[p.equipped].strength;

			cout << Magenta << p.weapons[p.equipped].name << Console << "'s strength is " << Blue 
				<< p.weapons[p.equipped].strength << Console << "." << endl;
			cout << p.name << " fights the " << LightMagenta << "monster" << Console << " with " 
				<< Magenta << p.weapons[p.equipped].name << Console << "." << endl;
		
			p.weapons[p.equipped].strength -= m.hp;
			if (p.weapons[p.equipped].strength <= 0)
			{
				p.weapons[p.equipped].strength = -1;
				cout << Magenta << p.weapons[p.equipped].name << Console << " is destroyed." << endl;
			}

			m.hp -= damage;

			if (m.hp <= 0)
			{
				cout << "The " << LightMagenta << "monster " << Console << "dies." << endl;
				map[p.y][p.x] = 0;
			}
			else
			{
				cout << "The " << LightMagenta << "monster " << Console << "loses " << Red << damage << Console << " HP." << endl;
				cout << "The " << LightMagenta << "monster " << Console << "has " << Blue << m.hp << Console << " HP left." << endl;
			
				p.hp -= 20;

				cout << p.name << " loses " << Red << "20 " << Console << "HP." << endl;
				
				if (p.hp > 0) cout << p.name << " has " << Blue << p.hp << Console << " HP left." << endl;
				else
				{
					p.gameover = true;
					cout << "You have lost all your health. " << Red << "Game Over." << Console << endl;
				}

				if (p.weapons[p.equipped].strength <= 0)
				{
					bool changed = false;
					for (int i = 0; i < WeaponCount; i++)
					{
						if (p.weapons[i].strength > 0 && !changed)
						{
							p.equipped = i;
							changed = true;
						}
					}
					if (changed) cout << p.name << " is now equipped with " << Magenta << p.weapons[p.equipped].name 
							  << Console << "." << endl;
					else 
					{
						cout << p.name << " has no more weapons to use." << Red << " Game Over" << Console << "." << endl;
						p.gameover = true;
					}
				}
			}
		}
	}
	// Health Potion
	else if (map[p.y][p.x] == 5)
	{
		p.hp += 100;
		map[p.y][p.x] = 0;
		cout << "You find a potion and drink it.\nYou gain " << Green << "100" << Console << " HP.\nYou have " << Blue << p.hp << Console << " HP left." << endl;
	}
	// Poison Potion
	else if (map[p.y][p.x] == 6)
	{
		p.hp -= 100;
		map[p.y][p.x] = 0;
		cout << "You find a potion and drink it.\nYou lose " << Red << "100" << Console << " HP." << endl;
		if (p.hp > 0) cout << "You have " << Blue << p.hp << Console << " HP left." << endl;
		else 
		{
			p.gameover = true;
			cout << "You have lost all your health. " << Red << "Game Over." << Console << endl;
		}
	}
	// Trap
	else if (map[p.y][p.x] == 7)
	{
		p.gameover = true;
		map[p.y][p.x] = 0;
		cout << "It's a trap!" << Red << " Game Over." << Console << endl;
	}
}

bool SaveFileExists()
{
	// if currentWeapon.txt && currentMap.txt exists return true
	ifstream inCurrentMapFile("currentMap.txt");

	if (inCurrentMapFile.good())
	{
		inCurrentMapFile.close();
		return true;
	}
	return false;
}

int DisplayQuit()
{
	cout << "Do you want to:" << endl;
	cout << "1)  Save your current progress?" << endl;
	cout << "2)  Quit the game?" << endl;
	cout << "3)  Cancel and return to the game?" << endl;

	int input;
	cin >> input;

	return input;
}

void Duel(player &p1, player &p2)
{
	while (!p1.gameover && !p2.gameover)
	{
		// p1 strikes first
		int damage = p1.weapons[p1.equipped].strength;

		cout << Magenta << p1.weapons[p1.equipped].name << Console << "'s strength is " << Blue 
			<< p1.weapons[p1.equipped].strength << Console << "." << endl;
		cout << p1.name << " fights " << p2.name << " with " 
			<< Magenta << p1.weapons[p1.equipped].name << Console << "." << endl;

		p1.weapons[p1.equipped].strength -= p2.hp;
		if (p1.weapons[p1.equipped].strength <= 0)
		{
			p1.weapons[p1.equipped].strength = -1;
			cout << Magenta << p1.weapons[p1.equipped].name << Console << " is destroyed." << endl;
		}

		p2.hp -= damage;

		cout << p2.name << " loses " << Red << damage << Console << "HP." << endl;			

		if (p2.hp <= 0)
		{
			cout << p2.name << " dies." << endl;
			p2.gameover = true;
		}
		// then p2
		else 
		{
				
			cout << p2.name << " has " << Blue << p2.hp << Console << " HP left." << endl;

			damage = p2.weapons[p2.equipped].strength;

			cout << Magenta << p2.weapons[p2.equipped].name << Console << "'s strength is " << Blue 
				<< p2.weapons[p2.equipped].strength << Console << "." << endl;
			cout << p2.name << " fights " << p1.name << " with " 
				<< Magenta << p2.weapons[p2.equipped].name << Console << "." << endl;

			p2.weapons[p2.equipped].strength -= p1.hp;
			if (p2.weapons[p2.equipped].strength <= 0)
			{
				p2.weapons[p2.equipped].strength = -1;
				cout << Magenta << p2.weapons[p2.equipped].name << Console << " is destroyed." << endl;
			}

			p1.hp -= damage;
			cout << p1.name << " loses " << Red << damage << Console << "HP." << endl;

			if (p1.hp <= 0)
			{
				cout << p1.name << " dies." << endl;
				p1.gameover = true;
			}
			else 
				cout << p1.name << " has " << Blue << p1.hp << Console << " HP left." << endl;
		}

		if (p1.weapons[p1.equipped].strength <= 0 && !p1.gameover)
		{
			bool changed = false;
			for (int i = 0; i < WeaponCount; i++)
			{
				if (p1.weapons[i].strength > 0 && !changed)
				{
					p1.equipped = i;
					changed = true;
				}
			}
			if (changed) cout << p1.name << " is now equipped with " << Magenta << p1.weapons[p1.equipped].name 
					  << Console << "." << endl;
			else 
			{
				cout << p1.name << " has no more weapons to use." << Red << " Game Over" << Console << "." << endl;
				p1.gameover = true;
			}
		}
		if (p2.weapons[p2.equipped].strength <= 0 && !p2.gameover)
		{
			bool changed = false;
			for (int i = 0; i < WeaponCount; i++)
			{
				if (p2.weapons[i].strength > 0 && !changed)
				{
					p2.equipped = i;
					changed = true;
				}
			}
			if (changed) cout << p2.name << " is now equipped with " << Magenta << p2.weapons[p2.equipped].name 
					  << Console << "." << endl;
			else 
			{
				cout << p2.name << " has no more weapons to use." << Red << " Game Over" << Console << "." << endl;
				p2.gameover = true;
			}
		}
	}
}
