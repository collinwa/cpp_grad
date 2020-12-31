#ifndef __TEST_H__
#define __TEST_H__
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

class Test {
public:
    Test() {
        x = 0;
        cout<< "all good boi" << endl;
    }

private:
    int x;

};

//vector< shared_ptr< Test > > ALL_PARAMS;

shared_ptr<Test> make_Test() {
    shared_ptr<Test> tmp (new Test()); 
    ALL_PARAMS.push_back(tmp);
    cout<< "ref count: " << ALL_PARAMS[0].use_count() << endl;
    return tmp;
}



#endif
