#include "TelephoneBill.h"
#include "Bill.h"

using namespace std;

double TelephoneBill::localCallRate = 0.3;
double TelephoneBill::lineRental = 29.9;
double TelephoneBill::internetConnection = 35;

istream& operator>>(istream &in, TelephoneBill &b) {
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

	cout << "Number of local calls: ";
	in >> b.localCalls;

	cout << "International calls: ";
	in >> b.internationalCalls;

	cout << endl << "Total amount due: $" << b.total() << endl;
	cout << endl << "Total GST: $" << b.GSTTotal() << endl;

    return in;
}

ostream& operator<<(ostream &out, const TelephoneBill &b) {
    out << "Telephone bill: " << endl;
    out << "Biller name: " << b.billerName << endl;
    out << "Biller code: " << b.billerCode << endl;
    out << "Reference number: " << b.referenceNumber << endl;
    out << "Account Number: " << b.accountNumber << endl;
    out << "Account Name: " << b.accountName << endl;
    out << "Address: " << b.address << endl;
    out << "Start date: " << b.startDate << "/" << b.startMonth << "/" << b.startYear << endl;
    out << "End date: " << b.endDate << "/" << b.endMonth << "/" << b.endYear << endl;
    out << "Due date: " << b.dueDate << "/" << b.dueMonth << "/" << b.dueYear << endl;
    out << "Number of local calls: " << b.localCalls << endl;
    out << "International calls: " << b.internationalCalls << endl;

    out << "Total amount due: $" << b.total() << endl;
    out << "Total GST: $" << b.GSTTotal() << endl << endl;

    return out;
}

TelephoneBill::TelephoneBill() {

}

TelephoneBill::~TelephoneBill() {

}

void TelephoneBill::input(ifstream &fin) {
    fin.ignore(100, ';');
    Bill::input(fin);
    fin >> localCalls;
    fin.get();
    fin >> internationalCalls;
    fin.get();

    fin.ignore(100, '\n');
}

void TelephoneBill::output(ofstream &fout) {
    fout << "T; ";
    Bill::output(fout);
    fout << localCalls << ';' << internationalCalls << ';';
    fout << GSTTotal() << ';' << total() << '\n';
}

double TelephoneBill::GSTTotal() const {
    double gst = 0;

    gst = this->total() * 0.1;

    return gst;
}

void TelephoneBill::print() {
    cout << *this;
}

double TelephoneBill::total() const {
    double t = 0;

    t = localCalls * TelephoneBill::localCallRate;
    t += internationalCalls + TelephoneBill::lineRental + TelephoneBill::internetConnection;

    return t;
}
