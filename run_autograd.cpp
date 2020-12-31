#include <iostream>
#include <algorithm>
#include <memory>
#include "Value.h"
using namespace std;

int main(int argc, char **argv) {
    srand( time( NULL ));
    shared_ptr<Value> x1 = make_Value(2.0); 
    shared_ptr<Value> x2 = make_Value(3.0);
    Value& x3 = (*x1 + *x2);
    cout << x3 << endl;
    _print_nodes();
    _free_nodes();
    return 0;
}