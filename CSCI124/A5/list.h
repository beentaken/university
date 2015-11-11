/**********************************************************************
 * list.h - CSCI124 - Ass5 - Contains LinkedList class declaration
 * <NAME> <STUDENT No.> <DATE>
 **********************************************************************/

#ifndef LIST_H_
#define LIST_H_

#include <cstddef>
using namespace std;

// Definition of data contained in LinkedList
typedef int ListDataType;  // This is going to be a linked list of ints.
                           // Note: the ListDataCmp() function also must be
                           // defined for this data type in list.cpp

class LinkedList
{
	public:
		LinkedList();                           // constructor
		~LinkedList();                          // destructor
		void AddToTail(const ListDataType&);    // previous add function
		bool IsEmpty();
		ListDataType RemoveFromHead();          // previous remove function
		void Insert(const ListDataType&);       // uses comparison to add
		void SetIterator();
		ListDataType Next();
		bool More();
	private:
		struct node;
		typedef node *nodePtr;

		struct node{
			ListDataType data;
			nodePtr next;
		};

		nodePtr Head;
		nodePtr Current;                       // Iterator pointer
};

#endif

