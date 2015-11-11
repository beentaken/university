#include "Agent.h"

using namespace std;

Agent::Agent() {

}

Agent::Agent(int Id, string Name, string Phone) {
	this->id = Id;
	this->name = Name;
	this->phone = Phone;
}

Agent::~Agent() {

}

string Agent::Print() {
	stringstream ss;
	ss << this->id << "; " << this->name << "; " << this->phone << endl;
	return ss.str();
}

