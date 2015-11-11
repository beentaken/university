#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node;
struct Date;

// Node definition
struct Node {
	struct Node* next;

	char user[10];
	int files;
	struct Date *latestDate, *oldestDate;

	double totsize;
};

// Date definition
struct Date {
    int day, month, year, hour, minutes;
};

// Class definition
class LinkedList
{
    private:
        Node* head;
	public:
	    LinkedList();
	    ~LinkedList();
		void Insert(char[10], int, int, int, int, int, long);
		Node* Find(char[10]);
		void Print();
};

#endif // LINKEDLIST_H
