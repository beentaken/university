/* SHANNON ARCHER - SJA998 - EVECTOR.CPP class definition for EVector */
#include "EVector.h"
#include <iostream>

using namespace std;

istream& operator>>(istream &in, EVector &e) {
    while (e.dim <= 0)
        in >> e.dim;

    delete [] e.tuples;
    e.tuples = new double[e.dim];

    for (int i = 0; i < e.dim; i++) {
        in >> e.tuples[i];
    }

    return in;
}

ostream& operator<<(ostream &out, const EVector &e) {
    out << '(';
    for (int i = 0; i < e.dim; i++)
    {
        if (i != 0)
            out << ',';

        out << e.tuples[i];
    }
    out << ')';

    return out;
}

EVector::EVector() {
    dim = -1;
    tuples = 0;
}

EVector::EVector(const EVector &e) {
    dim = e.dim;
    tuples = new double[dim];
    for (int i = 0; i < dim; i++) {
        tuples[i] = e.tuples[i];
    }
}

EVector::~EVector() {
    delete [] tuples;
}

EVector EVector::operator+(const EVector &e) {
    if (this->dim != e.dim)
        throw 1;

    EVector ev1 = this->dim > e.dim ? EVector(*this) : EVector(e);
    EVector ev2 = this->dim > e.dim ? e : *this;

    for (int i = 0; i < ev2.dim; i++) {
        ev1.tuples[i] += ev2.tuples[i];
    }

    return ev1;
}

EVector EVector::operator-(const EVector &e){
    if (this->dim != e.dim)
        throw 1;

    EVector ev1;
    ev1.dim = this->dim > e.dim ? this->dim : e.dim;
    ev1.tuples = new double[ev1.dim];

    for (int i = 0; i < ev1.dim; i++) {
        if (i < this->dim && i < e.dim)
            ev1.tuples[i] = this->tuples[i] - e.tuples[i];
        else if (i < this->dim)
            ev1.tuples[i] = this->tuples[i];
        else if (i < e.dim)
            ev1.tuples[i] = -e.tuples[i];
    }

    return ev1;
}

double EVector::operator*(const EVector &e){
    if (this->dim != e.dim)
        throw 1;

    EVector ev1 = this->dim > e.dim ? *this : e;
    EVector ev2 = this->dim > e.dim ? e : *this;
    double innerProduct = 0;

    for (int i = 0; i < ev2.dim; i++) {
        innerProduct += ev1.tuples[i] * ev2.tuples[i];
    }

    return innerProduct;
}

EVector EVector::operator/(const double  &d){
    EVector ev = EVector(*this);

    for (int i = 0; i < ev.dim; i++) {
        ev.tuples[i] /= d;
    }

    return ev;
}

EVector operator*(const EVector &e, const double  &d) {
    EVector ev = EVector(e);

    for (int i = 0; i < e.dim; i++) {
        ev.tuples[i] = e.tuples[i] * d;
    }

    return ev;
}
EVector operator*(const double  &d, const EVector &e) {
    EVector ev = EVector(e);

    for (int i = 0; i < e.dim; i++) {
        ev.tuples[i] = d * e.tuples[i];
    }

    return ev;
}
