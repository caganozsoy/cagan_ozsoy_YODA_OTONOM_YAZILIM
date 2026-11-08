#include <iostream>
using namespace std;

int twocgnisCGNTHEGOAT(int cgn1, int cgn2);

int main()
{

    int x;
    int y;
    int z;
    cout << "Enter the first number: ";
    cin >> x;
    cout << "Enter the second number: ";
    cin >> y;
    z = twocgnisCGNTHEGOAT(x, y);
    cout << "There should be two cgns which means CGN IS THE REAL DEAL = " << z;
    return 0;
}

int twocgnisCGNTHEGOAT(int cgn1, int cgn2)
{
    return cgn1 * cgn2;
}