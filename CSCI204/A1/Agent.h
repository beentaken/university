#ifndef AGENT
#define AGENT

#include <string>
#include <iostream>
#include <sstream>

class Agent {
	private:
		std::string name, phone;
	public:
		int id;
		Agent();
		Agent(int, std::string, std::string);
		~Agent();

		std::string Print();
};

#endif /* !AGENT */
