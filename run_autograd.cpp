#include <iostream>
#include <algorithm>
#include <memory>
#include "Test.h"
using namespace std;

int main(int argc, char **argv) {
    shared_ptr<Test> x = make_Test();

    return 0;
}