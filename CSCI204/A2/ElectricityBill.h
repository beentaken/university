#ifndef ELECTRICITYBILL
#define ELECTRICITYBILL

#include "Bill.h"
#include <iostream>

class ElectricityBill : public Bill {
    friend std::istream& operator>>(std::istream&, ElectricityBill&);
    friend std::ostream& operator<<(std::ostream&, const ElectricityBill&);
private:
    double previousReading, currentReading;
    static double rate1, rate1Threshold, rate2, supplyCharge;
public:
    ElectricityBill();
    ~ElectricityBill();
    virtual double GSTTotal() const;
    virtual double total() const;

    virtual void input(std::ifstream&);
    virtual void output(std::ofstream&);
    virtual void print();

    static void changeRates();
};

#endif // ELECTRICITYBILL
