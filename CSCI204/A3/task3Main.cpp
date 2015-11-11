#include "Customer.h"
#include <map>
#include <utility>
#include <fstream>
#include <iostream>

using namespace std;

int main(){
    map<int, Customer> Customers;

    // output from binary
    char* filename = new char[100];
    cout << "Input a file name to save data: ";
    cin >> filename;
    cout << endl;

    ifstream fin(filename, ios::in | ios::binary);
    fin.seekg(0, std::ifstream::end);
    int numfiles = fin.tellg() / sizeof(Customer);
    fin.seekg(0);

    cout << "Load customer records from binary file." << endl;
    for (int i = 0; !fin.eof() && i < numfiles; i++) {
        fin.seekg(i * sizeof(Customer));
        Customer cust;
        fin.read((char*)&cust, sizeof(Customer));
        Customers[cust.id] = cust;
    }

    cout << numfiles << " accounts have been loaded."  << endl << endl;
    for( map<int,Customer>::iterator ii = Customers.begin(); ii != Customers.end(); ++ii)
    {
       cout << ii->second;
    }

    fin.close();
}
