
#define RC_INVOKED  0

#include <iostream>
#include "ginac/ginac.h"
using namespace std;
using namespace GiNaC;

int test_ginac() {
#if 0
    symbol x("x"), y("y");
    ex poly;
    for (int i = 0; i < 3; ++i)
        poly += factorial(i + 16) * pow(x, i) * pow(y, 2 - i);
    cout << poly << endl;
#endif
    return 0;
}
