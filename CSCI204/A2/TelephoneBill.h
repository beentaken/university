#ifndef TELEPHONEBILL
#define TELEPHONEBILL

#include "Bill.h"
#include <iostream>

class TelephoneBill : public Bill {
    friend std::istream& operator>>(std::istream&, TelephoneBill&);
    friend std::ostream& operator<<(std::ostream&, const TelephoneBill&);
private:
    double localCalls, internationalCalls;
    static double localCallRate, lineRental, internetConnection;
public:
    TelephoneBill();
    ~TelephoneBill();
    virtual double GSTTotal() const;
    virtual double total() const;

    virtual void input(std::ifstream&);
    virtual void output(std::ofstream&);
    virtual void print();
};

#endif // TELEPHONEBILL
