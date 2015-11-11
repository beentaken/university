/**********************************************************************
 * list.cpp - CSCI124 - Ass5 - Contains LinkedList class definition
 * <NAME> <STUDENT No.> <DATE>
 **********************************************************************/

#include <cstddef>
#include "list.h"
using namespace std;


// Compare function for LinkList class - ListDataType is defined in list.h

int ListDataCmp(const ListDataType &D1,const ListDataType &D2){ return D1 > D2;}


LinkedList::LinkedList()
{
	Head = NULL;
}

LinkedList::~LinkedList()
{
	while (!IsEmpty())
		RemoveFromHead();
}

bool LinkedList::IsEmpty()
{
	return (Head == NULL);
}

void LinkedList::AddToTail(const ListDataType& newdata)
{
	nodePtr tmp, curr;

//	create a new customer and store the information

	tmp = new node;
	tmp->data = newdata;
	tmp->next = NULL;

//	add to the end of the list if the list is not empty

	if (Head != NULL)
	{
		curr = Head;
		while (curr->next)		// is this the end?
			curr = curr->next;
		curr->next = tmp;
	}
	else
		Head = tmp;
}

ListDataType LinkedList::RemoveFromHead()
{
	ListDataType data;
	nodePtr node = Head;
	
	Head = Head->next;

	data = node->data;

	delete node;

	return data;
}

void LinkedList::Insert(const ListDataType& newdata)
{
	nodePtr curr, prev, newnode;

	curr = Head;
	prev = 0;
	while (curr && (ListDataCmp(newdata,curr->data) > 0))
	{
		prev = curr;
		curr = curr->next;
	}
	newnode = new node;
	newnode->data = newdata;	// needs copy constructor
	// new node goes after prev and before curr
	if (prev)					// if there is a previous node
		prev->next = newnode;
	else
		Head = newnode;
	newnode->next = curr;
}

void LinkedList::SetIterator()
{
	Current = Head;
}

ListDataType LinkedList::Next()
{
	
	ListDataType data = Current->data;
	Current = Current->next;		
	return data;
}

bool LinkedList::More()
{
	if (Current)
		return true;

	return false;
}

