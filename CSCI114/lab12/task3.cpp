/*
	Shannon
	17-106, 8:30
	task3 - structs
*/

#include <iostream>
#include <cstring>

using namespace std;

struct BookInfo
{
	char Title[50], Author[30];
	float Price;
};

void printBookInfo(BookInfo);
void createBookInfo(BookInfo &);
void reducePrice(BookInfo &, float);
bool compareBookInfo(BookInfo book1, BookInfo book2);

int main()
{
	BookInfo myBook1 = { "The Da Vinci Code", "Dan Brown", 24.20 };

	BookInfo myBook2 = myBook1;
	strcpy(myBook2.Title, "Digital Fortress");
	myBook2.Price = 14.78;

	cout << "Title\tAuthor\tPrice" << endl;
	printBookInfo(myBook1);
	printBookInfo(myBook2);
}

void printBookInfo(BookInfo book)
{	
	cout << book.Title << "\t" << book.Author << "\t$" << book.Price << endl;
}

void createBookInfo(BookInfo &book)
{
	cout << "Enter the Book Title:" << endl;
	cin >> book.Title;
	cout << "Enter the Book Author:" << endl;
	cin >> book.Author;
	cout << "Enter the Book Price:" << endl;
	cin >> book.Price;
}

void reducePrice(BookInfo &bookInfo, float percent)
{
	bookInfo -= (bookInfo.Price / 100 * percent);
}

bool compareBookInfo(BookInfo book1, BookInfo book2)
{
	return (strcmp(book1.Title, book2.Title) == 0 && strcmp(book1.Author, book2.Author) == 0 && book1.Price == book2.Price);
}
