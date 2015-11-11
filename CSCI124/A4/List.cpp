/**********************************************
* List.cpp - Contains implementation of class LinkedList
* Shannon Archer, 4220912, sja998, 29/9/12
***********************************************/

#include <iostream>
#include "List.h"

using namespace std;

// constructor
LinkedList::LinkedList()
{
	Head = NULL;
}

// deep copy constructor
LinkedList::LinkedList(const LinkedList& list)
{
	Node *node = list.GetHead();
	while (node)
	{	
		AddTail(node->Item);
		node = node->Next;
	}
}

// destructor
LinkedList::~LinkedList()
{
	int i = 0;
	while (Head) 
	{
		RemoveHead(i);
	}
}

// assignment operator
LinkedList& LinkedList::operator=(const LinkedList& list)
{
	// copy data
	Node *node = list.GetHead();
	Node *head;
    while (node)
	{
        Node *node2 = new Node;
    	node2->Item = node->Item;
    	node2->Next = NULL;
    	if (head)
    	{
    		Node *t = head;
    		while (t->Next) { t = t->Next; }
    
    		t->Next = node;
    	}
    	else
    		head = node;
   		
		node = node->Next;
	}
	
    // remove current data
	int i = 0;
	while (Head) 
	{
		RemoveHead(i);
	}
	
	// assign copied list to head
	Head = head;
	
	return *this;
}

// adds item to head
void LinkedList::AddHead(int Item)
{
	Node *node = new Node;
	node->Item = Item;
	node->Next = Head;

	Head = node;
}

// removes item from head
bool LinkedList::RemoveHead(int &Item)
{
	if (Head)
	{
		Node *t = Head->Next;
		Item = Head->Item;
		delete Head;
		
		Head = t; 
		return true;
	}
	else
		return false;
}

// adds item to tail
void LinkedList::AddTail(int Item)
{
	Node *node = new Node;
	node->Item = Item;
	node->Next = NULL;
	if (Head)
	{
		Node *t = Head;
		while (t->Next) { t = t->Next; }

		t->Next = node;
	}
	else
		Head = node;
}

// removes item from tail
bool LinkedList::RemoveTail(int &Item)
{
	if (Head)
	{
		Node *t = Head;
		Node *t2;

		while (Head->Next) { t2 = Head; Head = Head->Next; }

		Item = Head->Item;
		
		t2->Next = NULL;
		delete Head;

		Head = t;
		return true;
	}
	else
		return false;
}

// prints list. eg 12 34 21 26
void LinkedList::Print()
{
	Node *t = Head;
	while (t)
	{
		cout << t->Item << ' ';
		t = t->Next;
	}
	cout << endl;
}

Node* LinkedList::GetHead() const
{
	return Head;
}
