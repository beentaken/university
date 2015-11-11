/***********************************************************************************
 * CSCI 124 - ass2.cpp - Contains function definitions for pet database program
 * Shannon Archer
 * sja998
 * 8/19/2012
 ***********************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
using namespace std;

// ============== Constants ==========================================

const char cDataFileName[] = "pets.txt";
const int cMaxRecs = 100;
const int cMaxChars = 30;


// ============= User Defined types ==================================

enum   StatusType{Lost,Found};
enum   PetType{Dog,Cat};
enum   GenderType{Male,Female,Unknown};
struct AgeType{ int Yrs; int Mths;}; // -1 if unknown

struct PetRecord{
	StatusType Status;           // Lost or Found
	PetType Type;                // Dog or Cat
	GenderType Gender;           // Male, Female or Unknown
	char Breed[cMaxChars];       // Bread of dog or cat. eg Bull terrier
	AgeType Age;                 // Age in years and mths. (-1 -1 if unknown)
	char Colour[cMaxChars];      // Colour of pet. eg tan
	char Location[cMaxChars];    // Suburb where found or lost. eg Mt Keira
	char PhoneNo[cMaxChars];     // Ph. of person to contact
};


// ============= Global Data =========================================

PetRecord *gPetRecs[cMaxRecs];
int gNumRecs=0;


// ============= Private Function Prototypes =========================

void DisplayRecord(int i); // Displays record i on screen
void StringToLower(char* string);


// ============= Public Function Definitions =========================

void ReadFile()
{
	ifstream fin;
	fin.open(cDataFileName);
	if (!fin.good()){
		cerr << "Could not open file!\n";
		exit(1);
	}
	gNumRecs=0;
	int i;
	for(i=0;i<cMaxRecs;i++){
        char Tmp[cMaxChars];
		fin >> Tmp;
		if(fin.fail())break;
		gPetRecs[i] = new PetRecord;
		switch(Tmp[0]){
			case'l': gPetRecs[i]->Status = Lost; break;
			case'f': gPetRecs[i]->Status = Found; break;
		}
		fin >> Tmp;
		switch(Tmp[0]){
			case'c': gPetRecs[i]->Type = Cat; break;
			case'd': gPetRecs[i]->Type = Dog; break;
		}
		fin >> Tmp;
		switch(Tmp[0]){
			case'm': gPetRecs[i]->Gender = Male;    break;
			case'f': gPetRecs[i]->Gender = Female;  break;
			default: gPetRecs[i]->Gender = Unknown; break;
		}
		fin.ignore(cMaxChars, '\n'); //eat tailing '\n'
		fin.getline(gPetRecs[i]->Breed,cMaxChars,'\n');
		fin >> gPetRecs[i]->Age.Yrs>>gPetRecs[i]->Age.Mths;
		fin.ignore(cMaxChars, '\n'); //eat tailing '\n'
		fin.getline(gPetRecs[i]->Colour,cMaxChars);
		fin.getline(gPetRecs[i]->Location,cMaxChars);
		fin.getline(gPetRecs[i]->PhoneNo,cMaxChars);
	}
	gNumRecs=i;
	fin.close();
	cout<< "\nThere are "<< gNumRecs <<" records in the Lost and Found Pet Database\n";
}

void DisplayRecs()
{// Displays records one at a time
	for(int i=0;i<gNumRecs;i++){
		DisplayRecord(i);
		cout<<"Display next record (y/n)> ";
		char Ans;
		cin>>Ans;
		cout<<endl;
		if(Ans=='n') return;
	}
}

void AddRecord()
{
     // Adds a new record to data file
	PetRecord *p = new PetRecord;
	cout << "Add Record to Database\n(Enter your pets details)\n(Enter ? or -1 if detail is unknown)" << endl;
	cout << "++++++++++++++++++++++++++++++" << endl;

	// get info
	char status, type, gender;


	cout << "Is the pet lost or found?\t(l/f)\t=> ";
	cin >> status;
	switch(status){
			case'l': p->Status = Lost; break;
			case'f': p->Status = Found; break;
		}

	cout << "Is the pet a dog or a cat?\t(d/c)\t=> ";
	cin >> type;
	switch(type){
			case'c': p->Type = Cat; break;
			case'd': p->Type = Dog; break;
		}

	cout << "What gender is the pet?\t\t(m/f)\t=> ";
	cin >> gender;
	switch(gender){
			case'm': p->Gender = Male; break;
			case'f': p->Gender = Female; break;
		}


	cout << "What breed is the pet?\t\t\t=> ";
    	cin.ignore(cMaxChars, '\n'); // eat \n char
	cin.getline(p->Breed, cMaxChars);
	if (strcmp(p->Breed, "?") == 0 || strcmp(p->Breed, "-1") == 0) strcpy(p->Breed, "unknown");

	cout << "What age is the pet?\t\t(yy mm)\t=> ";
	cin >> p->Age.Yrs;
	cin >> p->Age.Mths;

    	cout << "What colour is the pet?\t\t\t=> ";
    	cin.ignore(cMaxChars, '\n'); // eat \n char
	cin.getline(p->Colour, cMaxChars);
	if (strcmp(p->Colour, "?") == 0 || strcmp(p->Colour, "-1") == 0) strcpy(p->Colour, "unknown");

	cout << "In what suburb was the pet lost?\t=> ";
	cin.getline(p->Location, cMaxChars);
	if (strcmp(p->Location, "?") == 0 || strcmp(p->Location, "-1") == 0) strcpy(p->Location, "unknown");

	cout << "What is your phone number?\t\t=> ";
	cin.getline(p->PhoneNo, cMaxChars);
	if (strcmp(p->PhoneNo, "?") == 0 || strcmp(p->PhoneNo, "-1") == 0) strcpy(p->PhoneNo, "unknown");

	gPetRecs[gNumRecs] = p;
	gNumRecs++;

	// add to file
	ofstream fout(cDataFileName, ios::app);
	switch(p->Status){
		case Lost:  fout << "lost\n"; break;
		case Found: fout << "found\n"; break;
		default: fout << "unknown\n"; break;
	}
	switch(p->Type){
		case Cat: fout << "cat\n"; break;
		case Dog: fout << "dog\n"; break;
		default: fout << "unknown\n"; break;
	}
	switch(p->Gender){
		case Male:   fout << "male\n";  break;
		case Female: fout << "female\n"; break;
		case Unknown:
		default: fout << "unknown\n"; break;
	}
    	StringToLower(p->Breed);
	fout << p->Breed << '\n';
	fout << p->Age.Yrs << " " << p->Age.Mths << "\n";

	StringToLower(p->Colour);
    	fout << p->Colour << "\n";

    	StringToLower(p->Location);
	fout << p->Location << "\n";

	StringToLower(p->PhoneNo);
    	fout << p->PhoneNo<< "\n";

	cout << "++++++++++++++++++++++++++++++" << endl;
	cout << "A new record has been added to the database" << endl;
	cout << gNumRecs << " records are in the database" << endl;
}

void SearchArray()
{
    	PetRecord *p = new PetRecord;
    	cout << "Search Database\n(Enter your pets details)\n(Enter ? or -1 if detail is unknown)" << endl;
    	cout << "++++++++++++++++++++++++++++++" << endl;

	// get info
	char status, type, gender;

	cout << "Search lost or found pets?\t(l/f)\t=> ";
    	cin >> status;
	switch(status){
			case'l': p->Status = Lost; break;
			case'f': p->Status = Found; break;
		}

    	cout << "Search for a dog or a cat?\t(d/c)\t=> ";
	cin >> type;
	switch(type){
			case'c': p->Type = Cat; break;
			case'd': p->Type = Dog; break;
		}

	cout << "What gender is the pet?\t\t(m/f)\t=> ";
	cin >> gender;
	switch(gender){
			case'm': p->Gender = Male; break;
			case'f': p->Gender = Female; break;
		}


	cout << "What breed to search for?\t\t=> ";
    	cin.ignore(cMaxChars, '\n'); // eat \n char
	cin.getline(p->Breed, cMaxChars);
	if (strcmp(p->Breed, "?") == 0 || strcmp(p->Breed, "-1") == 0) strcpy(p->Breed, "unknown");

	cout << "What age to search for?\t(yy mm)\t\t=> ";
	cin >> p->Age.Yrs;
	cin >> p->Age.Mths;

    	cout << "What colour is the pet?\t\t\t=> ";
    	cin.ignore(cMaxChars, '\n'); // eat \n char
	cin.getline(p->Colour, cMaxChars);
	if (strcmp(p->Colour, "?") == 0 || strcmp(p->Colour, "-1") == 0) strcpy(p->Colour, "unknown");

	cout << "In what suburb was the pet lost?\t=> ";
	cin.getline(p->Location, cMaxChars);
	if (strcmp(p->Location, "?") == 0 || strcmp(p->Location, "-1") == 0) strcpy(p->Location, "unknown");

	cout << "What is your phone number?\t\t=> ";
	cin.getline(p->PhoneNo, cMaxChars);
	if (strcmp(p->PhoneNo, "?") == 0 || strcmp(p->PhoneNo, "-1") == 0) strcpy(p->PhoneNo, "unknown");

	// search records
	bool *keep = new bool[gNumRecs];
	for (int i = 0; i < gNumRecs; i++)
	{
		keep[i] = true;
		if ((gPetRecs[i]->Status == Lost && status == 'f') || (gPetRecs[i]->Status == Found && status == 'l'))
		   keep[i] = false;

		if ((gPetRecs[i]->Type == Cat && type == 'd') || (gPetRecs[i]->Type == Dog && type == 'c'))
		   keep[i] = false;

		if ((gPetRecs[i]->Gender == Male && (gender == 'f' || gender == 'u')) || (gPetRecs[i]->Gender == Female && (gender == 'm' || gender == 'u')))
		   keep[i] = false;

		if (p->Age.Yrs > 0)
		   if (p->Age.Yrs != gPetRecs[i]->Age.Yrs) keep[i] = false;

		if (p->Age.Mths > 0)
		   if (p->Age.Mths != gPetRecs[i]->Age.Mths) keep[i] = false;

		StringToLower(p->Breed);
		if (strcmp(gPetRecs[i]->Breed, p->Breed) != 0 && strcmp(p->Breed, "unknown") != 0)
		   	keep[i] = false;

		StringToLower(p->Colour);
		if (strcmp(gPetRecs[i]->Colour, p->Colour) != 0 && strcmp(p->Colour, "unknown") != 0)
		   	keep[i] = false;

		StringToLower(p->Location);
		if (strcmp(gPetRecs[i]->Location, p->Location) != 0 && strcmp(p->Location, "unknown") != 0)
		   	keep[i] = false;

		StringToLower(p->PhoneNo);
		if (strcmp(gPetRecs[i]->PhoneNo, p->PhoneNo) != 0 && strcmp(p->PhoneNo, "unknown") != 0)
		   	keep[i] = false;

		if (!keep[i])
		{
			PetRecord *tmp;
			tmp = gPetRecs[i];
			for (int j = i; j < gNumRecs; j++)
				gPetRecs[j] = gPetRecs[j+1];

			delete tmp;
			gNumRecs--;
			i--;
		}
    	}

	delete [] keep;
	delete p;

	cout << endl << "There are " << gNumRecs << " pets in the database \nthat match the search criteria" << endl << endl;

	for (int i = 0; i < gNumRecs; i++)
	{
		DisplayRecord(i);
	}
}

void CleanUp()
{// Deletes all dynamic data in gPetRecs array
    for (int i=0; i < gNumRecs; i++)
    {
        delete gPetRecs[i];
    }
}

// ============= Private Functions Definitions =========================



void DisplayRecord(int i)
{// Displays record i on screen
	cout<<"Status:        ";
	switch(gPetRecs[i]->Status){
		case Lost:  cout << "lost\n"; break;
		case Found: cout << "found\n";break;
	}
	cout<<"Type  :        ";
	switch(gPetRecs[i]->Type){
		case Cat: cout << "cat\n";break;
		case Dog: cout << "dog\n";break;
	}
	cout<<"Gender:        ";
	switch(gPetRecs[i]->Gender){
		case Male:   cout << "male\n";  break;
		case Female: cout << "female\n";break;
		case Unknown: cout << "unknown\n"; break;
	}
	cout<<"Breed:         "<<gPetRecs[i]->Breed<<endl;
	cout<<"Age:           "<<gPetRecs[i]->Age.Yrs<<" Yrs "<<gPetRecs[i]->Age.Mths<<" Mths\n";
	cout<<"Colour:        "<<gPetRecs[i]->Colour<<endl;
	cout<<"Location:      "<<gPetRecs[i]->Location<<endl;
	cout<<"Phone No:      "<<gPetRecs[i]->PhoneNo<<endl<<endl;
}

// =============== Super Private Function Definitions ========================
void StringToLower(char* string)
{
	for (int i = 0; i < cMaxChars && string[i] != '\0'; i++)
	{
		string[i] = tolower(string[i]);
	}
}

