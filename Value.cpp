#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Value.h"

vector< shared_ptr<Value> > gradient_tape;

void _print_nodes() {cout << endl << endl;
    cout << "-----begin node list-----" << endl;
    for (auto x : gradient_tape) {
        cout << *x;
    }
}

void _free_nodes() {
    while(gradient_tape.size() != 0) {
        gradient_tape.pop_back();
    }
}

// default constructor
Value::Value() : take_grad{true} {
    op = input;
    indegree = 0;
    outdegree = 0;
    val = 0.0;
    grad = 0.0;
    grad_l = 0.0;
    grad_r = 0.0;
    identifier = rand();
}

// construct from a Value from double
Value::Value(double val) : take_grad{true} {
    op = input;
    indegree = 0; 
    outdegree = 0; 
    this->val = val;
    grad = 0.0;
    grad_l = 0.0;
    grad_r = 0.0;
    identifier = rand();
}

Value::Value(double val, Value& lhs, Value& rhs, op_t op) : take_grad{true} {
    // set current node attributes
    this->val = val;
    this->lhs = lhs.get_self();
    this->rhs = rhs.get_self();
    this->op = op;
    indegree = 2;
    outdegree = 0;    
    grad = 0.0;
    grad_l = 0.0;
    grad_r = 0.0;
    identifier = rand();
}

Value::Value(double val, Value& lhs, op_t op) : take_grad{true} {
    // set current node attributes
    this->val = val;
    this->lhs = lhs.get_self();
    this->op = op;
    indegree = 1;
    outdegree = 0;    
    grad = 0.0;
    grad_l = 0.0;
    grad_r = 0.0;
    identifier = rand();
}

// set the current Value
void Value::set_self(weak_ptr<Value>& self) {
    this->self = self; 
}

// set the current descendant
void Value::set_desc(shared_ptr<Value>& descendant) {
    this->d.push_back(descendant) ;
}

// create value, add to gradient_tape, return shared_ptr
shared_ptr<Value> make_Value() {    

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
shared_ptr<Value> make_Value(double val) {

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
shared_ptr<Value> make_Value(double val, Value& lhs, Value& rhs, 
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

// for unary differentiable operators
shared_ptr<Value> make_Value(double val, Value& lhs, op_t op) {

    // construct the Value using the constructor
    shared_ptr<Value> tmp = make_shared<Value>(val, lhs, op);

    // save to gradient_tape
    gradient_tape.push_back(tmp);

    // update self pointer for the constructed value
    weak_ptr<Value> to_save = tmp;    
    tmp->set_self(to_save);

    // set parent node attributes
    // increment outdegree
    lhs.inc_outdegree();

    // set descendant of the parents to the constructed node
    lhs.set_desc(tmp);

    return tmp;
}

Value::~Value() {
    // if Value is going out of scope, then there are no more shared_ptrs
    // so the ancestors don't exist anymore. Since everything inside of a 
    // Value is stack-allocated, it'll all disappear. Thus, we just need to
    // decrement the indegree of the child
    for (auto dec : d) {
        dec->dec_indegree();
    }
}

// operators for constructing the desired values 
Value& operator+(Value& lhs, Value& rhs) {
    return *make_Value(lhs.val + rhs.val, lhs, rhs, add);
}


Value& operator*(Value& lhs, Value& rhs) {
    return *make_Value(lhs.val * rhs.val,lhs, rhs, mult);
}

Value& operator-(Value& lhs, Value& rhs) {
    return *make_Value(lhs.val - rhs.val, lhs, rhs, sub);
}

Value& operator/(Value& lhs, Value& rhs) {
    return *make_Value(lhs.val / rhs.val, lhs, rhs, divide);
}

// This isn't ready yet.
Value& Value::relu() {
    double swap_val = 0.0;
    if(this->val >= 0) {
        swap_val = this->val;
    }

    return *make_Value(swap_val, *get_self(), relu_op);
}

Value& Value::exp() {
    return *make_Value(::exp(val), *get_self(), exp_op);
}


// from https://stackoverflow.com/questions/45507041/how-to-check-if-weak-ptr-is-empty-non-assigned
// check if a weak_ptr is assigned 
template <typename T>
bool is_uninitialized(std::weak_ptr<T> const& weak) {
    using wt = std::weak_ptr<T>;
    return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}

// operator for printing out a node
ostream& operator<<(ostream& lhs, Value& v) {
    lhs << "-----begin node-----" << endl;
    lhs << "id: " << v.identifier << endl;
    lhs << "value: " << v.val << endl;
    lhs << "grad: " << v.grad << endl;
    lhs << "l_grad: " << v.grad_l << endl;
    lhs << "r_grad: " << v.grad_r << endl;
    lhs << "indegree: " << v.indegree << endl;
    lhs << "outdegree: " << v.outdegree << endl;
    lhs << "op_t " << v.op << endl;

    if (!is_uninitialized(v.lhs)) {
        lhs << "left ancestor: " << (v.lhs.lock())->identifier << endl;
    } else {
        lhs << "left ancestor: nullptr" << endl;
    }
    if (!is_uninitialized(v.rhs)) {
        lhs << "right ancestor: " << (v.rhs.lock())->identifier << endl;    
    } else {        
        lhs << "right ancestor: nullptr" << endl;
    }
    if (v.d.size() != 0) {
        for (auto dec : v.d) {
            lhs << "descendant: " << dec->identifier << endl;
        } 
    } else {
        lhs << "descendant: nullptr" << endl;
    }
    lhs << "------end node------" << endl;
    return lhs;    
}

struct shrd_ptr_sort {
    bool operator()(const shared_ptr<Value>& lhs, const shared_ptr<Value>& rhs) {
       return (lhs->get_indegree() >= rhs->get_indegree());
    }
};

void Value::backward() {
    // compute derivatives before copying
    for (auto& x : gradient_tape) {
        x->compute_lr_derivatives();  
    }

    make_heap(gradient_tape.begin(), gradient_tape.end(), shrd_ptr_sort());

    // get a reference to the current vector
    shared_ptr<Value> base_val = get_self();
    // ok cool
    for (auto& x : gradient_tape) {
        if (x->is_grad_enabled()) {
            // compute the double
            double tmp = chain_rule(base_val, x->get_self());
            // compute gradient for all flexible parameters
            x->set_grad(tmp);
        }
    }
}

/*
enum op_t {
    mult,
    input,
    add,
    divide,
    sub,
    relu_op,
    exp_op
};
*/

// compute derivative w.r.t. left or right operand
// unary operations only have a left operand
void Value::compute_lr_derivatives() {
    switch(op) {
        // for binary ops, we guarantee that lhs/rhs exist
        case mult:
            //d/dx x * y = y
            grad_l = rhs.lock()->get_self()->get_val();
            //d/dy x * y = x
            grad_r = lhs.lock()->get_self()->get_val();
            break;
        case input:
            grad_l = 1.0;
            grad_r = 1.0;
            break;
        case add:
            // d/dx x + y = 1.0
            grad_l = 1.0;
            // d/dy x + y = 1.0
            grad_r = 1.0;
            break;
        case divide:
            // d/dx x /y = 1/y
            grad_l = 1 / rhs.lock()->get_self()->get_val();
            // d/dy x / y = -1 * x / y^2
            grad_r = -1 * lhs.lock()->get_self()->get_val() /  
                    (rhs.lock()->get_self()->get_val() * 
                     rhs.lock()->get_self()->get_val());
            break;
        case sub:
            // d/dx x - y = 1.0
            grad_l = 1.0;
            // d/dy x - y = -1.0
            grad_r = -1.0;
            break;
        case relu_op:
            grad_l = val >= 0.0 ? 1.0 : 0.0;
            grad_r = 0.0;
            break;
        case exp_op:
            // d/dx e^x = e^x
            grad_l = val;
            grad_r = 0.0;
            break;
        default:
            break;
    }
}

double chain_rule(shared_ptr<Value>cur, shared_ptr<Value> stop) { 
   switch(cur->get_self()->get_op()) {    
        // for binary ops, we guarantee that lhs/rhs exist
        case mult:
        case divide:
        case sub:
        case add:
            return cur->get_self()->get_grad_l() * chain_rule(cur->get_l_ancs().lock(), stop)+
                   cur->get_self()->get_grad_r() * chain_rule(cur->get_r_ancs().lock(), stop);
        case input:
            return stop == cur ? 1.0 : 0.0;
        case exp_op:
        case relu_op:
            return cur->get_self()->get_grad_l() * chain_rule(cur->get_l_ancs().lock(), stop);
        default:
            return 0.0;
   } 
}