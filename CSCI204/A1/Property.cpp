#include "Property.h"

using namespace std;

Property::Property() {

}

Property::Property(int Id, string Address, string Type, int Price) {
	this->id = Id;
	this->address = Address;
	this->type = Type;
	this->price = Price;
}

Property::~Property() {

}

string Property::Print() {
    stringstream ss;
	ss << this->id << "; " << this->address << "; " << this->type << "; " << this->price << endl;
	return ss.str();
}
