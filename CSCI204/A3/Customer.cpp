#include "Customer.h"
#include <iomanip>

Customer::Customer() {
    id = 0;
}

Customer::~Customer() {

}

std::ostream& operator<<(std::ostream& out, const Customer& customer) {
    out << customer.id << " ";
    out << std::setw(15) << std::left << customer.firstname;
    out << customer.lastname << std::endl;

    return out;
}

std::istream& operator>>(std::istream& in, Customer& customer) {
    in >> customer.id;
    in >> customer.firstname;
    in >> customer.lastname;

    return in;
}
