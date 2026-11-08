#include <iostream>

using namespace std;

int main()
{

    int *r = new int(5);
    int *y = r;

    cout << "Value: " << *r << endl;
    cout << "Value: " << &y;
    return 0;
}

