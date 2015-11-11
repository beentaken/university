#include "Customer.h"
#include "MyList.h"
#include <fstream>

using namespace std;

int main(){
    int num;
    cout << "Input number of records: ";
    cin >> num;

    MyList<Customer> custList;

    cout << "Input " << num << " records: " << endl;
    for (int i = 0; i < num; i++){
        Customer *cust = new Customer();
        cin >> *cust;
        custList.push_back(*cust);
    }


    // input to binary
    char* filename = new char[100];
    cout << "Input a file name to save data: ";
    cin >> filename;

    ofstream fout(filename, ios::out | ios::binary);

    int numfiles = 0;
    for (MyList<Customer>::iterator<Customer> i = custList.Begin(); i != custList.End(); ++i) {
        Customer cust = i.ptr->data;
        fout.write((char*)&cust, sizeof(Customer));
        numfiles++;
    }

    cout << numfiles << " customer records saved." << endl << endl;
    fout.close();

    // output from binary
    ifstream fin(filename, ios::in | ios::binary);
    cout << "Load customer records from binary file." << endl;
    for (int i = 0; !fin.eof() && i < numfiles; i++) {
        fin.seekg(i * sizeof(Customer));
        Customer cust;
        fin.read((char*)&cust, sizeof(Customer));
        cout << cust;
    }

    fin.close();
}




















