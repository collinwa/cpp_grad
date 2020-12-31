#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Value.h"

vector< shared_ptr<Value> > gradient_tape;

// default constructor
Value::Value() {
    op = input;
    indegree = 0;
    outdegree = 0;
    val = 0.0;

    srand( time(NULL) ); // for generating node identifiers
    identifier = rand();
}

// construct from a Value from double
Value::Value(double val) {
    op = input;
    indegree = 0; 
    outdegree = 0; 
    this->val = val;

    srand( time(NULL) ); // for generating node identifiers
    identifier = rand();
}

Value::Value(double val, Value& lhs, Value& rhs, op_t op) {
    // set current node attributes
    this->val = val;
    this->lhs = lhs.get_self();
    this->rhs = rhs.get_self();
    this->op = op;
    indegree = 2;
    outdegree = 0;

    srand( time(NULL) ); // for generating node identifiers
    identifier = rand();
}

// return a shared_ptr to the current Value
shared_ptr<Value> Value::get_self() {
    return self.lock();
}

// set the current Value
void Value::set_self(weak_ptr<Value>& self) {
    this->self = self; 
}

// set the current descendant
void Value::set_desc(shared_ptr<Value>& descendant) {
    this->d = descendant;
}

// create value, add to gradient_tape, return shared_ptr
shared_ptr<Value> Value::make_Value() {    

    // construct tmp; add it to gradient_tape
    shared_ptr<Value> tmp = make_shared<Value>(0.0);

    // copy construct a weak_ptr
    weak_ptr<Value> to_save = tmp;
    gradient_tape.push_back(tmp);

    // save a weak_ptr to the current node
    tmp->set_self(to_save);
    return tmp;
}

// create value, add to gradient_tape, return shared_ptr
shared_ptr<Value> Value::make_Value(double val) {

    // construct a shared_ptr
    shared_ptr<Value> tmp = make_shared<Value>(val);

    // copy construct a weak_ptr
    weak_ptr<Value> to_save = tmp;
    gradient_tape.push_back(tmp);

    // save a weak_ptr to the current node
    tmp->set_self(to_save);
    return tmp;
}

// note how in the constructor, we don't actually do anything with the parents
// in my opinion, the constructor should strictly deal with values that are 
// internal to the class
shared_ptr<Value> Value::make_Value(double val, Value& lhs, Value& rhs, 
    op_t op) {

    // construct the Value using the constructor
    shared_ptr<Value> tmp = make_shared<Value>(val, lhs, rhs, op);

    // save to gradient_tape
    weak_ptr<Value> to_save = tmp;
    gradient_tape.push_back(tmp);

    // update self pointer for the constructed value
    tmp->set_self(to_save);

    // set parent node attributes
    // increment outdegree
    lhs.inc_outdegree();
    rhs.inc_outdegree();
    // set descendant of the parents to the constructed node
    lhs.set_desc(tmp);
    rhs.set_desc(tmp);

    return tmp;
}
Value::~Value() {
    // if Value is going out of scope, then there are no more shared_ptrs
    // so the ancestors don't exist anymore. Since everything inside of a 
    // Value is stack-allocated, it'll all disappear. Thus, we just need to
    // decrement the indegree of the child
    (*d).dec_indegree();
}

// opertors for constructing the desired values 
Value& operator+(Value& lhs, Value& rhs) {
    shared_ptr<Value> tmp = Value::make_Value(lhs.get_val() + rhs.get_val(),
        lhs, rhs, add);

    return *tmp;
}


Value& operator*(Value& lhs, Value& rhs) {
    shared_ptr<Value> tmp = Value::make_Value(lhs.get_val() * rhs.get_val(),
        lhs, rhs, mult);

    return *tmp;
}

Value& operator-(Value& lhs, Value& rhs) {
    shared_ptr<Value> tmp = Value::make_Value(lhs.get_val() - rhs.get_val(),
        lhs, rhs, sub);

    return *tmp;
}

Value& operator/(Value& lhs, Value& rhs) {
    shared_ptr<Value> tmp = Value::make_Value(lhs.get_val() / rhs.get_val(),
        lhs, rhs, divide);

    return *tmp;
}
