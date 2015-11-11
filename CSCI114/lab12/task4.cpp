/*
	Shannon
	17-106, 8:30
	task4 - nested structs
*/

#include <iostream>
#include <cstring>

using namespace std;


struct PublisherInfo
{
	char Name[50];
	int Year;
};

struct BookInfo
{
	char Title[50], Author[30];
	float Price;
	PublisherInfo PubInfo;
};

void printBookInfo(BookInfo);
void createBookInfo(BookInfo &);

int main()
{
	BookInfo myBook1 = { "The Da Vinci Code", "Dan Brown", 24.20, { "Double Day", 2004 } };

	BookInfo myBook2 = myBook1;
	strcpy(myBook2.Title, "Digital Fortress");
	myBook2.Price = 14.78;

	cout << "Title\t\t\tAuthor\t\tPrice\tPub. Name\tYear Pub." << endl;
	printBookInfo(myBook1);
	printBookInfo(myBook2);
}

void printBookInfo(BookInfo book)
{	
	cout << book.Title << "\t" << book.Author << "\t$" << book.Price << "\t" << book.PubInfo.Name << "\t" << book.PubInfo.Year << endl;
}

void createBookInfo(BookInfo &book)
{
	cout << "Enter the Book Title:" << endl;
	cin.getline(book.Title, 50);
	cout << "Enter the Book Author:" << endl;
	cin.getline(book.Author, 30);
	cout << "Enter the Book Price:" << endl;
	cin >> book.Price;
	cout << "Enter the Publisher Name:" << endl;
	cin.getline(book.PubInfo.Name, 50);
	cout << "Enter the Year Published:" << endl;
	cin >> book.PubInfo.Year;
}
