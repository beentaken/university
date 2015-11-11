#include "MyList.h"
#include <iostream>

using namespace std;

int main(){
    int num;

    // push front
    cout << "Input number of integers: ";
    cin >> num;

    MyList<int> intlist1;

    cout << "Input " << num << " integers: ";
    for (int i = 0; i < num; i++) {
        int input;
        cin >> input;
        intlist1.push_front(input);
    }

    cout << "Integer list 1 (push_front(), loop by using iterator):" << endl;
    for (MyList<int>::iterator<int> i = intlist1.Begin(); i != intlist1.End(); ++i) {
        cout << i << " ";
    }
    cout << endl;

    // push back
    cout << "Input number of integers: ";
    cin >> num;

    MyList<int> intlist2;

    cout << "Input " << num << " integers: ";
    for (int i = 0; i < num; i++) {
        int input;
        cin >> input;
        intlist2.push_back(input);
    }

    cout << "Integer list 1 (push_back(), loop by using iterator):" << endl;
    for (MyList<int>::iterator<int> i = intlist2.Begin(); i != intlist2.End(); ++i) {
        cout << i << " ";
    }
    cout << endl;

    // copy constructor and front loop
    cout << "Integer list 2 = list 1 (loop by using front() and pop_front():" << endl;
    intlist2 = intlist1;

    while (intlist2.front()) {
        cout << *(intlist2.front()) << " ";
        intlist2.pop_front();
    }
    cout << endl;

    // doubles!
    cout << "Input number of doubles: ";
    cin >> num;

    MyList<double> doublelist;

    cout << "Input " << num << " doubles: ";
    for (int i = 0; i < num; i++) {
        double input;
        cin >> input;
        doublelist.push_back(input);
    }

    cout << "Double list (loop by using iterator):" << endl;
    for (MyList<double>::iterator<double> i = doublelist.Begin(); i != doublelist.End(); ++i) {
        cout << i << " ";
    }
    cout << endl;
}
