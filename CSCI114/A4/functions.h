/*
	Shannon
	17-106, 8:30
	25/5
	Function header file of ass4 - rpg
*/
// constants
const int MapDim = 10;
const int WeaponCount = 9;

const char LightMagenta[] = "\033[01;35m";
const char Magenta[] = "\033[22;35m";
const char Blue[] = "\033[22;34m";
const char Green[] = "\033[22;32m";
const char Red[] = "\033[22;31m";
const char Yellow[] = "\033[01;33m";
const char Console[] = "\033[0m";

// structs
struct weapon
{
	char name[100];
	int strength;
};

struct player
{
	int x, y, hp, equipped;
	char name[100];
	bool gameover;
	weapon weapons[WeaponCount];
};

struct monster
{
	int hp;
};

// prototypes
void ReadMap(int map[][MapDim], player p[], bool cont);
void ReadWeapons(player p[], bool cont);
void WriteMap(int map[][MapDim], player p[]);
void WriteWeapons(player p[]);

void MovePlayer(int map[][MapDim], player& p, char input);
void RunEvent(int map[][MapDim], player& p);

bool SaveFileExists();
int DisplayQuit();

void Duel(player &p1, player &p2);
