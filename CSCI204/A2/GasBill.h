#ifndef GASBILL
#define GASBILL

#include "Bill.h"
#include <iostream>

class GasBill : public Bill {
    friend std::istream& operator>>(std::istream&, GasBill&);
    friend std::ostream& operator<<(std::ostream&, const GasBill&);
private:
    double previousReading, currentReading;
    static double heatingValue, pressureFactor, rate, supplyCharge;
public:
    GasBill();
    ~GasBill();
    virtual double GSTTotal() const;
    virtual double total() const;

    virtual void input(std::ifstream&);
    virtual void output(std::ofstream&);
    virtual void print();

    static void changeRates();
};

#endif // ELECTRICITYBILL
