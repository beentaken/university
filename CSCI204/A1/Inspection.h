#ifndef INSPECTION
#define INSPECTION

#include<string>
#include<iostream>
#include<sstream>
#include<fstream>

#include "Agent.h"
#include "Property.h"
#include "Buyer.h"

struct Booking {
	int day, month, year, hour, minute;
	std::string feedback;

	Agent agent;
	Buyer buyer;
	Property prop;
};

class Inspection {
	private:
		int numBookings;
		Booking *bookings[100];
	public:
		Inspection();
		~Inspection();
		void addNewBooking(Agent, Buyer, Property, int, int, int, int, int, std::string);
		void searchBookings(int, Booking*[], int&);
		void saveBookings(std::string filename);
};

#endif /* !INSPECTION */
