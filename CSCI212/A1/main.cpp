#include <iostream>
#include "linkedlist.h"

using namespace std;

int main() {
    LinkedList* list = new LinkedList();

    // remove header
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );

    // loop to get input
	while (cin) {
        char user[10], cmonth[10], time[10];
        long filesize;
        int day, month, year, hour = 0, minutes = 0;

        // ignore permissions and user id
        cin.ignore(15, '\n');

        // get user
        cin.get(user, 9);

        // ignore other text
        cin.ignore(10, '\n');

        // get filesize
        cin >> filesize;

        // get date stuff
        cin.get();
        cin.get(cmonth, 4);
        cin.get();
        cin >> day;
        cin.get();
        cin >> year;
        if (cin.get() == ':') {
            hour = year;
            year = 2013;
            cin >> minutes;
        }

        // get month as int
        if (strcmp(cmonth, "Jan") == 0)
            month = 0;
        else if (strcmp(cmonth, "Feb") == 0)
            month = 1;
        else if (strcmp(cmonth, "Mar") == 0)
            month = 2;
        else if (strcmp(cmonth, "Apr") == 0)
            month = 3;
        else if (strcmp(cmonth, "May") == 0)
            month = 4;
        else if (strcmp(cmonth, "Jun") == 0)
            month = 5;
        else if (strcmp(cmonth, "Jul") == 0)
            month = 6;
        else if (strcmp(cmonth, "Aug") == 0)
            month = 7;
        else if (strcmp(cmonth, "Sep") == 0)
            month = 8;
        else if (strcmp(cmonth, "Oct") == 0)
            month = 9;
        else if (strcmp(cmonth, "Nov") == 0)
            month = 10;
        else if (strcmp(cmonth, "Dec") == 0)
            month = 12;

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );

        // insert user into list if details valid
        if (strlen(user) > 0)
            list->Insert(user, day, month, year, hour, minutes, filesize);
	}

    // print list
	list->Print();

    // delete list
    delete list;

	return 0;
}
