#include "GasBill.h"
#include "Bill.h"

using namespace std;

double GasBill::heatingValue   = 38.82;
double GasBill::pressureFactor = 0.9942;
double GasBill::rate           = 0.0297;
double GasBill::supplyCharge   = 0.443;

istream& operator>>(istream &in, GasBill &b) {
    cout << "Biller Name: ";
	in.ignore(100, '\n');
    getline(in, b.billerName);

    cout << "Biller Code: ";
    in >> b.billerCode;

    cout << "Reference: ";
	in.ignore(100, '\n');
    getline(in, b.referenceNumber);

    cout << "Account number: ";
    in >> b.accountNumber;

    cout << "Account name: ";
	in.ignore(100, '\n');
    getline(in, b.accountName);

    cout << "Address: ";
    getline(in, b.address);

    cout << "Start Date: ";
    in >> b.startDate >> b.startMonth >> b.startYear;

    cout << "End Date: ";
    in >> b.endDate >> b.endMonth >> b.endYear;

    cout << "Due Date: ";
    in >> b.dueDate >> b.dueMonth >> b.dueYear;

	cout << "Previous reading (Cubic meters): ";
	in >> b.previousReading;

	cout << "Current reading (Cubic meters): ";
	in >> b.currentReading;

	cout << endl << "Total amount due: $" << b.total() << endl;
	cout << endl << "Total GST: $" << b.GSTTotal() << endl;

    return in;
}

ostream& operator<<(ostream &out, const GasBill &b) {

    out << "Gas bill: " << endl;
    out << "Biller name: " << b.billerName << endl;
    out << "Biller code: " << b.billerCode << endl;
    out << "Reference number: " << b.referenceNumber << endl;
    out << "Account Number: " << b.accountNumber << endl;
    out << "Account Name: " << b.accountName << endl;
    out << "Address: " << b.address << endl;
    out << "Start date: " << b.startDate << "/" << b.startMonth << "/" << b.startYear << endl;
    out << "End date: " << b.endDate << "/" << b.endMonth << "/" << b.endYear << endl;
    out << "Due date: " << b.dueDate << "/" << b.dueMonth << "/" << b.dueYear << endl;
    out << "Previous reading: " << b.previousReading << endl;
    out << "Current reading: " << b.currentReading << endl;

    out << "Total amount due: $" << b.total() << endl;
    out << "Total GST: $" << b.GSTTotal() << endl << endl;

    return out;
}

GasBill::GasBill() {

}

GasBill::~GasBill() {

}

void GasBill::input(ifstream &fin) {
    fin.ignore(100, ';');
    Bill::input(fin);
    fin >> previousReading;
    fin.get();
    fin >> currentReading;
    fin.get();

    fin.ignore(100, '\n');
}

void GasBill::output(ofstream &fout) {
    fout << "G; ";
    Bill::output(fout);
    fout << previousReading << ';' << currentReading << ';';
    fout << GSTTotal() << ';' << total() << '\n';
}

double GasBill::GSTTotal() const {
    double gst = 0;

    gst = this->total() * 0.10;

    return gst;
}

double GasBill::total() const {
    double t = 0;

    t =  (currentReading - previousReading) * GasBill::heatingValue * GasBill::pressureFactor;
    t *= GasBill::rate;
    t += (endDate - startDate + (endMonth - startMonth)*30 + (endYear - startYear)*364) * GasBill::supplyCharge;

    return t;
}

void GasBill::print() {
    cout << *this;
}

void GasBill::changeRates() {
    cout << "Set electricity rates." << endl;
    cout << "Rate ($ per MJ): ";
    cin  >> rate;

    cout << "Heating value: ";
    cin  >> heatingValue;

    cout << "Pressure factor: ";
    cin  >> pressureFactor;

    cout << "Supply charge rate ($ per day): ";
    cin  >> supplyCharge;

    cout << endl << "New rates for gas bills have been set." << endl;
}
