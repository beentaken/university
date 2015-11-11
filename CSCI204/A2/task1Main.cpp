/* SHANNON ARCHER - SJA998 - EVECTOR.CPP class definition for EVector */
#include "EVector.h"
#include <iostream>

using namespace std;

int main() {
    EVector e1, e2;
    double d;

    cout << "Input dimension and tuples for a Euclidean vector v1: ";
    cin >> e1;

    cout << "\nEuclidean vector v1: " << e1 << endl;

    cout << "Input dimension and tuples for a Euclidean vector v2: ";
    cin >> e2;

    cout << "Euclidean vector v2: " << e2 << endl << endl;
    try {
        cout << "v3 = v1 + v2 = " << (e1 + e2) << endl;
        cout << "v3 = v1 - v2 = " << (e1 - e2) << endl;
        cout << "v3 = v1 * v2 = " << (e1 * e2) << endl << endl;
    } catch (int i) {
        cout << "Error: Two Euclidean vectors should be in the same Euclidean space" << endl;
    }

    cout << "Input a double value for d: ";
    cin >> d;

    cout << "\nd * v1 = " << d << " * " << e1 << " = " << (d * e1) << endl;
    cout << "v1 * d = " << e1 << " * " << d << " = " << (e1 * d) << endl;
    cout << "v1 / d = " << e1 << " / " << d << " = " << (e1 / d) << endl;
}
