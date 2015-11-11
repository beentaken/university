/**********************************************
* List.h - Contains declaration of class LinkedList and struct Node
* Shannon Archer, 4220912, sja998, 29/9/12
***********************************************/
struct Node
{
	int Item;
	Node *Next;
};

class LinkedList
{
	public:
		LinkedList();
		LinkedList(const LinkedList& list);
		~LinkedList();
		LinkedList& operator = (const LinkedList& list);

		void AddHead(int Item);		// adds item to head
		bool RemoveHead(int &Item);	// removes item from head
		void AddTail(int Item);		// adds item to tail
		bool RemoveTail(int &Item);	// removes item from tail
		void Print();			// prints list. eg 12 34 21 26
		Node* GetHead() const;
	private:
		Node *Head;
};
