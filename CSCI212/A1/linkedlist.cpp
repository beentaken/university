#include <string>
#include <iostream>
#include <iomanip>
#include "linkedlist.h"

using namespace std;

// constructor
LinkedList::LinkedList(){
    this->head = 0;
};

// destructor
LinkedList::~LinkedList(){
    Node* node = head;
    while (node){
        node = node->next;

        delete head->latestDate;
        delete head->oldestDate;
        delete head;

        head = node;
    }
};

// insert into list in alphabetical order
void LinkedList::Insert(char user[10], int day, int month, int year, int hour, int minutes, long filesize) {

    Node* node = this->Find(user);

    // if none found, get create and insert
    if (!node) {
        // create node
        Node* node = new Node();
        strcpy(node->user, user);
        node->files = 1;

        node->latestDate = new Date();
        node->oldestDate = new Date();

        node->latestDate->day = day;
        node->latestDate->month = month;
        node->latestDate->year = year;
        node->latestDate->hour = hour;
        node->latestDate->minutes = minutes;

        node->oldestDate->day = day;
        node->oldestDate->month = month;
        node->oldestDate->year = year;
        node->oldestDate->hour = hour;
        node->oldestDate->minutes = minutes;

        node->totsize += filesize;

        // insert node in alphabetical order
        Node* current = this->head;
        if (current) {
            // iterate through list and check if node < current->next or current->next == null
            Node* prev = 0;
            while (current) {
                if (strcmp(node->user, current->user) < 0)
                    break;
                else {
                    prev = current;
                    current = current->next;
                }
            }
            node->next = current;
            if (prev) {
                prev->next = node;
            }
            else {
                this->head = node;
            }
        }
        // assign to head
        else {
            this->head = node;
        }
    }
    else {
        // add node values
        node->files += 1;
        node->totsize += filesize;

        // compare dates
        // check if earlier
        if (year <= node->oldestDate->year && month <= node->oldestDate->month && day <= node->oldestDate->day && hour <= node->oldestDate->hour && minutes <= node->oldestDate->minutes)
        {
            node->oldestDate->day = day;
            node->oldestDate->month = month;
            node->oldestDate->year = year;
            node->oldestDate->hour = hour;
            node->oldestDate->minutes = minutes;
        }
        // else check if later
        else if (year >= node->oldestDate->year && month >= node->oldestDate->month && day >= node->oldestDate->day && hour >= node->oldestDate->hour && minutes >= node->oldestDate->minutes)
        {
            node->latestDate->day = day;
            node->latestDate->month = month;
            node->latestDate->year = year;
            node->latestDate->hour = hour;
            node->latestDate->minutes = minutes;
        }
    }
}


// find node with user
Node* LinkedList::Find(char user[10]) {
    Node* node = this->head;
    if (node) {
        do {
            if (strcmp(node->user, user) == 0)
                return node;
            else
                node = node->next;
        }
        while (node);
    }
    // return 0 if not found
    return 0;
}

// print full list
void LinkedList::Print(){
    Node* current = this->head;
    cout << "User\t\tFiles\t   Oldest\t   Latest\tTotSize" << endl;
    while (current) {
        cout << current->user << "\t" << setw(5) << setfill(' ') << right << current->files << "\t";

        switch (current->oldestDate->month){
            case 0:
                cout << "Jan";
                break;
            case 1:
                cout << "Feb";
                break;
            case 2:
                cout << "Mar";
                break;
            case 3:
                cout << "Apr";
                break;
            case 4:
                cout << "May";
                break;
            case 5:
                cout << "Jun";
                break;
            case 6:
                cout << "Jul";
                break;
            case 7:
                cout << "Aug";
                break;
            case 8:
                cout << "Sep";
                break;
            case 9:
                cout << "Oct";
                break;
            case 10:
                cout << "Nov";
                break;
            case 11:
                cout << "Dec";
                break;
        }

        cout << " " << setw(2) << setfill('0') << right << current->oldestDate->day << " ";

        if (current->oldestDate->hour != 0 || current->oldestDate->minutes != 0) {
            cout << current->oldestDate->hour << ":" << current->oldestDate->minutes;
        } else {
            cout << setw(5) << setfill(' ') << right << current->oldestDate->year;
        }

        cout << "\t";

        switch (current->latestDate->month){
            case 0:
                cout << "Jan";
                break;
            case 1:
                cout << "Feb";
                break;
            case 2:
                cout << "Mar";
                break;
            case 3:
                cout << "Apr";
                break;
            case 4:
                cout << "May";
                break;
            case 5:
                cout << "Jun";
                break;
            case 6:
                cout << "Jul";
                break;
            case 7:
                cout << "Aug";
                break;
            case 8:
                cout << "Sep";
                break;
            case 9:
                cout << "Oct";
                break;
            case 10:
                cout << "Nov";
                break;
            case 11:
                cout << "Dec";
                break;
        }

        cout << " " << setw(2) << setfill('0') << right << current->latestDate->day << " ";

        if (current->latestDate->hour != 0 || current->latestDate->minutes != 0) {
            cout << current->latestDate->hour << ":" << current->latestDate->minutes;
        } else {
            cout << setw(5) << setfill(' ') << right << current->latestDate->year;
        }

        double totsize = current->totsize;
        int suffix = 0;

        while (totsize > 10) {
            totsize /= 1024.00;
            suffix++;
        }

        char suf[2];
        switch (suffix) {
            case 0:
                strcpy(suf, "B");
                break;
            case 1:
                strcpy(suf, "KB");
                break;
            case 2:
                strcpy(suf, "MB");
                break;
            case 3:
                strcpy(suf, "TB");
                break;
        }

        cout << "\t" << setw(5) << setfill('0') << setprecision(3) << fixed << left << totsize;
        cout << suf << endl;

        current = current->next;
    }
}
