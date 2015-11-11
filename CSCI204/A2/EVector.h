/* SHANNON ARCHER - SJA998 - EVECTOR.CPP class definition for EVector */
#ifndef EVECTOR
#define EVECTOR

#include <iostream>

class EVector {
    friend std::istream& operator>>(std::istream&, EVector&);
    friend std::ostream& operator<<(std::ostream&, const EVector&);
    friend EVector operator*(const EVector &, const double  &);
    friend EVector operator*(const double  &, const EVector &);
private:
    double *tuples;
    int dim;
public:
    EVector();
    EVector(const EVector &);
    ~EVector();

    EVector operator+(const EVector &);
    EVector operator-(const EVector &);
    double  operator*(const EVector &);
    EVector operator/(const double  &);

};


#endif // EVECTOR
