#include "Bill.h"

using namespace std;

istream& operator>>(istream &in, Bill &b) {

    return in;
}

ostream& operator<<(ostream &out, const Bill &b) {
    out << b.billerName;

    return out;
}

Bill::Bill() {

}

Bill::~Bill() {

}

void Bill::input(ifstream &fin) {
    getline(fin, billerName, ';');
    fin >> billerCode;
    fin.get();
    getline(fin, referenceNumber, ';');
    fin >> accountNumber;
    fin.get();
    getline(fin, accountName, ';');
    getline(fin, address, ';');

    fin >> startDate;
    fin.get();
    fin >> startMonth;
    fin.get();
    fin >> startYear;
    fin.get();

    fin >> endDate;
    fin.get();
    fin >> endMonth;
    fin.get();
    fin >> endYear;
    fin.get();

    fin >> dueDate;
    fin.get();
    fin >> dueMonth;
    fin.get();
    fin >> dueYear;
    fin.get();
}

void Bill::output(ofstream &out) {
    out << billerName << ';' << billerCode << ';';
    out << referenceNumber << ';' << accountNumber << ';';
    out << accountName << ';' << address << ';';
    out << startDate << '/' << startMonth << '/' << startYear << ';';
    out << endDate << '/' << endMonth << '/' << endYear << ';';
    out << dueDate << '/' << dueMonth << '/' << dueYear << ';';
}

void Bill::print() {}

double Bill::GSTTotal() const {
    double gst = 0;


    return gst;
}

double Bill::total() const {
    double gst = 0;


    return gst;
}


