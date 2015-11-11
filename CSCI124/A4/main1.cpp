/**********************************************
* Main1.cpp - Contains testing code for implementation of class LinkedList
* Shannon Archer, 4220912, sja998, 29/9/12
***********************************************/

#include <iostream>
#include "List.h"

using namespace std;

int main()
{
	LinkedList *L = new LinkedList, *L2, *L3, *L4;

	L->AddHead(12);
	L->AddHead(50);
	L->AddHead(34);
	L->AddTail(56);
	L->AddTail(79);

	L2 = new LinkedList(*L);
	
	cout << "List1: "; L->Print();	
	cout << "List2: "; L2->Print();

	cout << endl;

	int i = 0;
	L2->RemoveHead(i);
	cout << "Removed " << i << " from start of list 2." << endl;
	L2->RemoveHead(i);	
	cout << "Removed " << i << " from start of list 2." << endl;

	L->RemoveTail(i);	
	cout << "Removed " << i << " from end of list 1." << endl;
	L->RemoveTail(i);	
	cout << "Removed " << i << " from end of list 1." << endl;

	cout << endl;
	cout << "List1: "; L->Print();
	cout << "List2: "; L2->Print();

	cout << endl << "Assign list 2 to list 3 and list 4." << endl;
	
	L3 = new LinkedList();
	L4 = new LinkedList();
	*L3 = *L4 = *L2;

	
	cout << endl;
	cout << "List1: "; L->Print();
	cout << "List2: "; L2->Print();
	cout << "List3: "; L3->Print();
	cout << "List4: "; L4->Print();

	cout << endl << "Remove Head, Tail and then add 70 to the Head of list 3." << endl;
	L3->RemoveHead(i);
	L3->RemoveTail(i);
	L3->AddHead(70);

	cout << endl << "Add 78 to Head, 23 to Tail, 89 to Head of list 4." << endl;
	L4->AddHead(78);
	L4->AddTail(23);
	L4->AddHead(89);

	cout << endl;
	cout << "List1: "; L->Print();
	cout << "List2: "; L2->Print();
	cout << "List3: "; L3->Print();
	cout << "List4: "; L4->Print();
	
	return 0;
}
