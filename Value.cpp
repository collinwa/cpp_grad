#include <iostream>
#include <algorithm>
#include "Value.h"

vector< weak_ptr<Value> > gradient_tape;

// default constructor
Value::Value() {
    op = input;
    indegree = 0;
    outdegree = 0;
    val = 0.0;
}

// construct from a Value from double
Value::Value(double val) {
    op = input;
    indegree = 0; 
    outdegree = 0; 
    this->val = val;
}

Value::Value(double val, Value& a1, Value& a2, op_t op) {
    // set current node attributes
    this->val = val;
    this->a1 = a1.get_self();
    this->a2 = a2.get_self();
    this->op = op;
    indegree = 2;
    outdegree = 0;
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
    gradient_tape.push_back(to_save);

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
    gradient_tape.push_back(to_save);

    // save a weak_ptr to the current node
    tmp->set_self(to_save);
    return tmp;
}

// note how in the constructor, we don't actually do anything with the parents
// in my opinion, the constructor should strictly deal with values that are 
// internal to the class
shared_ptr<Value> Value::make_Value(double val, Value& a1, Value& a2, 
    op_t op) {

    // construct the Value using the constructor
    shared_ptr<Value> tmp = make_shared<Value>(val, a1, a2, op);

    // save to gradient_tape
    weak_ptr<Value> to_save = tmp;
    gradient_tape.push_back(to_save);

    // update self pointer for the constructed value
    tmp->set_self(to_save);

    // update ancestor nodes
    // set parent node attributes
    a1.inc_outdegree();
    a2.inc_outdegree(); 
    a1.set_desc(tmp);
    a2.set_desc(tmp);

    return tmp;
}
Value::~Value() {
    // if Value is going out of scope, then there are no more shared_ptrs
    // so the ancestors don't exist anymore. Since everything inside of a 
    // Value is stack-allocated, it'll all disappear, and it's fine that this
    // is a no-op.
}