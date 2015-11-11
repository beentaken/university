#ifndef MYLIST
#define MYLIST

#include <iostream>

template <class T>
class MyList {
private:
        template <class T>
        class node {
            friend std::ostream& operator<<(std::ostream& out, const node& n) {
                out << n.data;
                return out;
            }
            public:
                template <class T>
                node(T t) {
                    next = 0;
                    prev = 0;
                    data = t;
                }

                T data;
                node *prev, *next;
        };

        node<T> *head, *tail;
public:
        template <class T>
        class iterator {
            friend std::ostream& operator<<(std::ostream& out, const iterator& it) {
                out << *(it.ptr);
                return out;
            }
        private:
                MyList<T> *myList;
        public:
                node<T> *ptr;

                iterator(MyList<T> *MyList, node<T>* Node){
                    myList = MyList;
                    ptr = Node;
                }
                // prefix
                iterator<T>& operator++() {
                    this->ptr = this->ptr->next;

                    return *this;
                }
                // postfix
                iterator<T> operator++(int) {
                    iterator<T> *it = this
                    this->ptr = this->ptr->next;
                    return *it;
                }
                // comparison
                friend bool operator==(const iterator<T>& lhs, const iterator<T>& rhs) {
                    return (lhs.ptr == rhs.ptr);
                }
                friend bool operator!=(const iterator<T>& lhs, const iterator<T>& rhs) {
                    return !(lhs == rhs);
                }
        };

        MyList() {
            head = 0;
            tail = 0;
        }

        ~MyList() {
            while (this->front()) {
                this->pop_front();
            }

            head = 0;
            tail = 0;
        }

        // add new node to front of list
        void push_front(T data) {
            node<T>* n = new node<T>(data);
            if (head == 0) {
                head = n;
                tail = n;
            } else {
                head->prev = n;
                n->next = head;
                head = n;
            }
        }
        // add new node to back of list
        template <class T>
        void push_back(T data) {
            node<T>* n = new node<T>(data);
            if (tail == 0) {
                head = n;
                tail = n;
            } else {
                tail->next = n;
                n->prev = tail;
                tail = n;
            }
        }
        // return head
        node<T>* front() {
            return head;
        }
        // remove head from list
        void pop_front() {
            node<T>* n = head;
            head = head->next;
            delete n;
            n = 0;
        }
        // deep copy
        MyList& operator=(MyList list) {
            while (this->front()) {
                this->pop_front();
            }

            this->head = 0;
            this->tail = 0;

            for (MyList<T>::iterator<T> i = list.Begin(); i != list.End(); ++i) {
                this->push_back(i.ptr->data);
            }

            return *this;
        }

        // return iterator at start of list
        iterator<T> Begin() {
            iterator<T> it(this, head);
            return it;
        }
        // return iterator at end of list
        iterator<T> End() {
            if (tail) {
                iterator<T> it(this, tail->next);
                return it;
            }
            return Begin();
        }
};


#endif // #MYLIST
