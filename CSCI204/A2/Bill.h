#ifndef BILL
#define BILL

#include <string>
#include <iostream>
#include <fstream>

class Bill {
    friend std::istream& operator>>(std::istream&, Bill&);
    friend std::ostream& operator<<(std::ostream&, const Bill&);
protected:
    std::string billerName, accountName, referenceNumber, address;
    int billerCode, accountNumber;
    int dueDate, dueMonth, dueYear;
    int startDate, startMonth, startYear;
    int endDate  , endMonth  , endYear;
    double amountDue, totalGST;
public:
    Bill();
    ~Bill();

    virtual void input(std::ifstream&);
    virtual void output(std::ofstream&);
    virtual void print();

    virtual double GSTTotal() const;
    virtual double total() const;
};

#endif // BILL
