#include "Bill.h"
#include "ElectricityBill.h"
#include "GasBill.h"
#include "TelephoneBill.h"
#include <iostream>
#include <fstream>

using namespace std;

struct Node
{
	Bill *Item;
	Node *Next;
};

class LinkedList
{
	public:
		LinkedList();
		~LinkedList();

		void AddHead(Bill *Item);		// adds item to head
		bool RemoveHead();	// removes item from head
		void Clear();
		Node* GetHead() const;
	private:
		Node *Head;
};


// constructor
LinkedList::LinkedList()
{
	Head = NULL;
}

// destructor
LinkedList::~LinkedList()
{
	while (Head)
	{
		RemoveHead();
	}
}

void LinkedList::Clear()
{
    while (Head)
	{
		RemoveHead();
	}
}

// adds item to head
void LinkedList::AddHead(Bill *Item)
{
	Node *node = new Node;
	node->Item = Item;
	node->Next = Head;

	Head = node;
}

// removes item from head
bool LinkedList::RemoveHead()
{
	if (Head)
	{
		Node *t = Head->Next;
		delete Head;

		Head = t;
		return true;
	}
	else
		return false;
}

Node* LinkedList::GetHead() const
{
	return Head;
}

int main() {
    LinkedList bills;

    int selection = 10;

    string choices = "1. Input electricity bill data;\n2. Input gas bill data;\n3. Input telephone bill data;\n4. Set electricity bill rates;\n5. Set gas rate;\n6. Save bill data in a text file;\n7. Load bill data from a text file;\n0. Quit.\nYour choice: ";

    while (selection > 0) {
        cout << choices;
        cin >> selection;
		cout << endl;

		Bill *bptr;
        char billName[50];
        ofstream fout;
        ifstream fin;

        if (selection == 1) {
			bptr = new ElectricityBill();
			cin >> *((ElectricityBill *)bptr);
            bills.AddHead(bptr);
        } else if (selection == 2) {
			bptr = new GasBill();
			cin >> *((GasBill *)bptr);
            bills.AddHead(bptr);
        } else if (selection == 3) {
			bptr = new TelephoneBill();
			cin >> *((TelephoneBill *)bptr);
            bills.AddHead(bptr);
        } else if (selection == 4) {
            ElectricityBill::changeRates();
        } else if (selection == 5) {
            GasBill::changeRates();
        } else if (selection == 6) {
            cout << "\nText file name: ";
            cin.ignore(100, '\n');
            cin.getline(billName, 50, '\n');

            fout.open(billName);
            int i = 0;
            for (Node *node = bills.GetHead(); node != NULL; node = node->Next) {
                node->Item->output(fout);
                i++;
            }

            cout << i << " bills have been saved." << endl;
        } else if (selection == 7) {
            cout << "\nText file name: ";
            cin.ignore(100, '\n');
            cin.getline(billName, 50, '\n');

            bills.Clear();
            fin.open(billName);

            int i = 0;
            while (fin.good() && !fin.eof()) {
                Bill *bill;
                char c = fin.get();

                if (c == 'E') {
                    bill = new ElectricityBill();
                    bill->input(fin);
                } else if (c == 'G') {
                    bill = new GasBill();
                    bill->input(fin);
                } else if (c == 'T') {
                    bill = new TelephoneBill();
                    bill->input(fin);
                }

                if (c == 'E' || c == 'G' || c == 'T') {
                    bills.AddHead(bill);
                    i++;
                }
            }

            cout << fin.eof() << i << " bills have been loaded." << endl;


            for (Node *node = bills.GetHead(); node != NULL; node = node->Next) {
                node->Item->print();
                i++;
            }
        } else if (selection == 0) {
            exit(0);
        }
    }
}

