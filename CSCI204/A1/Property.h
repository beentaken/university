#ifndef PROPERTY
#define PROPERTY

#include <string>
#include <iostream>
#include <sstream>

class Property {
	private:
		int price;
		std::string address, type;
	public:
		int id;
		Property();
		Property(int, std::string, std::string, int);
		~Property();

		std::string Print();
};

#endif /* !PROPERTY */
