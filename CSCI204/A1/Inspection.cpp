#include "Inspection.h"

using namespace std;

Inspection::Inspection() {
	numBookings = 0;
}

Inspection::~Inspection() {
	delete [] bookings;
}

void Inspection::addNewBooking(Agent Agent, Buyer Buyer, Property Prop, int Day, int Month, int Year, int Hour, int Minute, string Comments) {
	// create booking
	bookings[numBookings] = new Booking();

	bookings[numBookings]->agent = Agent;
	bookings[numBookings]->buyer = Buyer;
	bookings[numBookings]->prop = Prop;

	bookings[numBookings]->day = Day;
	bookings[numBookings]->month = Month;
	bookings[numBookings]->year = Year;
	bookings[numBookings]->hour = Hour;
	bookings[numBookings]->minute = Minute;

	bookings[numBookings]->feedback = Comments;

	numBookings++;
}

void Inspection::searchBookings(int AgentId, Booking *Bookings[], int &NumBookings) {
	NumBookings = 0;

	for (int i = 0; i < numBookings; i++) {
		if (bookings[i]->agent.id == AgentId)
			Bookings[NumBookings++] = bookings[i];
	}
}

void Inspection::saveBookings(string filename) {
    ofstream iout(filename.c_str());
    for (int i = 0; i < this->numBookings; i++) {
        iout << "Agent: " << bookings[i]->agent.Print();
        iout << "\n\n";
        iout << "Property: " << bookings[i]->prop.Print();
        iout << "\n\n";
        iout << "be inspected by a buyer:\n\n";
        iout << bookings[i]->agent.Print();
        iout << "\n\n";
        iout << "at " << bookings[i]->day << "/" << bookings[i]->month << "/" << bookings[i]->year << " " << bookings[i]->hour << ":" << bookings[i]->minute;
        iout << "\n\n";
        iout << "Feedback: " << bookings[i]->feedback;
        iout << "\n\n";
    }
    iout.close();
}
