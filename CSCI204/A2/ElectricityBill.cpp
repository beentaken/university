#include "ElectricityBill.h"
#include "Bill.h"

using namespace std;

double ElectricityBill::rate1          = 0.245;
double ElectricityBill::rate1Threshold = 1750;
double ElectricityBill::rate2          = 0.264;
double ElectricityBill::supplyCharge   = 0.699;

istream& operator>>(istream &in, ElectricityBill &b) {
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

	cout << "Previous reading (kWh): ";
	in >> b.previousReading;

	cout << "Current reading (kWh): ";
	in >> b.currentReading;

	cout << endl << "Total amount due: $" << b.total() << endl;
	cout << endl << "Total GST: $" << b.GSTTotal() << endl;

    return in;
}

ostream& operator<<(ostream &out, const ElectricityBill &b) {
    out << "Electricity bill: " << endl;
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

ElectricityBill::ElectricityBill() {

}

ElectricityBill::~ElectricityBill() {

}

void ElectricityBill::input(ifstream &fin) {
    fin.ignore(100, ';');
    Bill::input(fin);
    fin >> previousReading;
    fin.get();
    fin >> currentReading;
    fin.get();

    fin.ignore(100, '\n');
}

void ElectricityBill::output(ofstream &fout) {
    fout << "E; ";
    Bill::output(fout);
    fout << previousReading << ';' << currentReading << ';';
    fout << GSTTotal() << ';' << total() << '\n';
}

void ElectricityBill::print() {
    cout << *this;
}

double ElectricityBill::GSTTotal() const {
    double gst = 0;

    gst = this->total() * 0.10;

    return gst;
}

double ElectricityBill::total() const {
    double t = 0;

    t = (currentReading - previousReading) * ElectricityBill::rate1 + ((currentReading - previousReading) - ElectricityBill::rate1Threshold) * ElectricityBill::rate2;
    t += (endDate - startDate + (endMonth - startMonth)*30 + (endYear - startYear)*364) * ElectricityBill::supplyCharge;

    return t;
}

void ElectricityBill::changeRates() {
    cout << "Set electricity rates." << endl;
    cout << "Rate 1 ($ per kWh): ";
    cin  >> rate1;

    cout << "Threshold (kWh): ";
    cin  >> rate1Threshold;

    cout << "Rate 2 ($ per kWh): ";
    cin  >> rate2;

    cout << "Supply charge rate ($ per day): ";
    cin  >> supplyCharge;

    cout << endl << "New rates for electricity bills have been set." << endl;
}
