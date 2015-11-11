/*
	Shannon
	17-106, 8:30
	task2 - structs

	d. myBook2 = myBook1;
	   myBook2 = { myBook1.Title, myBook1.Author, myBook1.Price };
*/

#include <iostream>
#include <cstring>

using namespace std;

struct BookInfo
{
	char Title[50], Author[30];
	float Price;
};

int main()
{
	BookInfo myBook1 = { "The Da Vinci Code", "Dan Brown", 24.20 };

	BookInfo myBook2 = myBook1;
	strcpy(myBook2.Title, "Digital Fortress");
	myBook2.Price = 14.78;

	cout << "Title\tAuthor\tPrice" << endl;
	cout << myBook1.Title << "\t" << myBook1.Author << "\t$" << 4myBook1.Price << endl;
	cout << myBook2.Title << "\t" << myBook2.Author << "\t$" << myBook2.Price << endl;
}
