#ifndef CUSTOMER
#define CUSTOMER

#include <iostream>

class Customer {
    friend std::istream& operator>>(std::istream&, Customer&);
    friend std::ostream& operator<<(std::ostream&, const Customer&);
private:
    char firstname[100], lastname[100];
public:
    int id;
    Customer();
    ~Customer();
};

#endif // CUSTOMER
