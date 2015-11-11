#include<iostream>
#include<string>

#include "Agent.h"
#include "Inspection.h"
#include "Buyer.h"
#include "Property.h"

using namespace std;

bool options();
void newBooking();
void searchBookings();
void saveFile();

void getData(int, char*[]);
void displayData();

// collections
int numAgents, numProperties, numBuyers, numInspections;

Agent *agents;
Property *properties;
Buyer *buyers;
Inspection *insp;

int main(int argc, char *argv[]) {
	if (argc != 4)
		return 1;

	insp = new Inspection();

	// get data from txt files
	getData(argc, argv);

	// display data
	displayData();

	// print options and get input
	while(options());

	return 0;
}

bool options() {
	int a = 0;
	do {
		cout << "\n\n1. Book a new inspection.";
		cout << "\n\n2. Search inspection bookings.";
		cout << "\n\nInput choice (0-quit): ";

		cin >> a;
	} while (a < 0 || a > 2);

	// perform task
	switch (a) {
		case 0:
		    saveFile();
			exit(0);
		case 1:
			newBooking();
			break;
		case 2:
			searchBookings();
			break;
	}

    return true;
}

void getData(int argc, char *argv[]){
	// get agents
	ifstream fin(argv[1]);

	fin >> numAgents;
	fin.ignore(100, '\n');

	agents = new Agent[numAgents];

	for (int i = 0; i < numAgents; i++) {
		// get whole line as string
		string s1;
		getline(fin, s1);
		istringstream iss(s1);

		// break up into values
		int id;
		iss >> id;
		iss.get();

		string name, phone;
		getline(iss, name, ';');
		getline(iss, phone, ';');

		agents[i] = Agent(id, name, phone);
	}

	fin.close();

	// get properties
	fin.open(argv[2]);

	fin >> numProperties;
	fin.ignore(100, '\n');

	properties = new Property[numProperties];

	for (int i = 0; i < numProperties; i++) {
		// get whole line as string
		string s1;
		getline(fin, s1);
		istringstream iss(s1);

		// break up into values
		int id, price;
		iss >> id;
		iss.get();

		string address, type;
		getline(iss, address, ';');
		getline(iss, type, ';');

		iss >> price;
		iss.get();

		properties[i] = Property(id, address, type, price);
	}

	fin.close();

	// get buyers
	fin.open(argv[3]);

	fin >> numBuyers;
	fin.ignore(100, '\n');

	buyers = new Buyer[numBuyers];

	for (int i = 0; i < numBuyers; i++) {
		// get whole line as string
		string s1;
		getline(fin, s1);
		istringstream iss(s1);

		// break up into values
		string name, address, phone;
		getline(iss, name, ';');
		getline(iss, address, ';');
		getline(iss, phone, ';');

		buyers[i] = Buyer(name, address, phone);
	}

	fin.close();
}

void displayData() {
	cout << "\nLoad agents' records.\n\n";

	for (int i = 0; i < numAgents; i++)
	{
		cout << agents[i].Print();
		cout << endl;
	}

	cout << "\n\nLoad property records.\n\n";
	for (int i = 0; i < numProperties; i++)
	{
		cout << properties[i].Print();
		cout << endl;
	}


	cout << "\n\nLoad buyer records.\n\n";
	for (int i = 0; i < numBuyers; i++)
	{
		cout << buyers[i].Print();
		cout << endl;
	}

	cout << endl;
}

void newBooking() {
	Agent agent;
	Buyer buyer;
	Property prop;

	// input / output
	int agentId, propId;
	string buyerPhone, comments;

	int day, month, year, hour, minute;
	bool found = false;

	cout << "\nAdd a new inspection.";

	do {
		cout << "\n\nAgent number: ";
		cin >> agentId;

		// look for agent
		for (int i = 0; i < numAgents; i++) {
			if (agents[i].id == agentId) {
				agent = agents[i];
				found = true;
			}
		}

		if (!found)
			cout << "\nNo such agent.\n";
	}
	while (!found);

	found = false;

	do {
		cout << "\n\nBuyer Phone: ";
		cin >> buyerPhone;

		// look for buyer
		for (int i = 0; i < numBuyers; i++) {
			if (buyers[i].phone == buyerPhone) {
				buyer = buyers[i];
				found = true;
			}
		}

		if (!found)
			cout << "\nNo such buyer.\n";
	}
	while (!found);

	found = false;

	do {
		cout << "\n\nProperty id: ";
		cin >> propId;

		// look for property
		for (int i = 0; i < numProperties; i++) {
			if (properties[i].id == propId) {
				prop = properties[i];
				found = true;
			}
		}

		if (!found)
			cout << "\nNo such property.\n";
	}
	while (!found);

	cout << "\nInspection date (day month year): ";
	cin >> day >> month >> year;

	cout << "\nInspection time (hour minute): ";
	cin >> hour >> minute;

	cout << "\nComments: ";
	cin.ignore(100, '\n');
	getline(cin, comments);

	// create inspection and add to collection
	insp->addNewBooking(agent, buyer, prop, day, month, year, hour, minute, comments);
}

void searchBookings() {
	int agentId;
	Agent agent;

	bool found = false;

	do {
		cout << "\n\nInput Agent number: ";
		cin >> agentId;

		// look for agent
		for (int i = 0; i < numAgents; i++) {
			if (agents[i].id == agentId) {
				agent = agents[i];
				found = true;
			}
		}

		if (!found)
			cout << "\nNo such agent.\n";
	}
	while (!found);

	Booking *bookings[100];
	int numBookings = 0;

	insp->searchBookings(agent.id, bookings, numBookings);

    if (numBookings > 0) {
        cout << "\nThe agent: ";
        cout << agent.Print();
        cout << "\nhas the following inspections booked.\n" << endl;

        for (int i = 0; i < numBookings; i++) {
            cout << "\nProperty: ";
            cout << bookings[i]->prop.Print();

            cout << "\nby\n\n";
            cout << bookings[i]->buyer.Print();

            cout << "\nat\n\n" << bookings[i]->day << "/" << bookings[i]->month << "/" << bookings[i]->year << " " << bookings[i]->hour << ":" << bookings[i]->minute << endl;
            cout << "\nFeedback: " << bookings[i]->feedback << endl;

            cout << "\n\n\n";
        }
    } else {
        cout << "That agent has no inspections booked.\n";
    }
}

void saveFile() {
    string filename;
    cout << "\n\nSave file to: ";
    cin.ignore(100, '\n');
    getline(cin, filename);

    insp->saveBookings(filename);
    delete [] agents;
    delete [] buyers;
    delete [] properties;
    delete insp;

    cout << "\n\nThanks!\n\n";
}
