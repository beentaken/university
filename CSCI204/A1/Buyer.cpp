#include "Buyer.h"

using namespace std;

Buyer::Buyer() {

}

Buyer::Buyer(string Name, string Address, string Phone) {
	this->name = Name;
	this->address = Address;
	this->phone = Phone;
}

Buyer::~Buyer() {

}

string Buyer::Print() {
    stringstream ss;
	ss << this->name << "; " << this->address << "; " << this->phone << endl;
	return ss.str();
}
