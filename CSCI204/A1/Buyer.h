#ifndef BUYER
#define BUYER

#include<string>
#include<iostream>
#include<sstream>

class Buyer {
	private:
		std::string name, address;
	public:
		std::string phone;

		Buyer();
		Buyer(std::string, std::string, std::string);
		~Buyer();
		std::string Print();
};


#endif /* !BUYER */
