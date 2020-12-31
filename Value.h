#ifndef __VALUE_H__
#define __VALUE_H__
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

enum op_t {
    mult,
    input,
    add,
    divide,
    sub,
    relu_op
};

// the interface you should use to make values
class Value {
public:

    // constructors -- users shouldn't call these 
    Value();
    Value(double val);
    Value(double val, Value& lhs, op_t op); // unary construction
    Value(double val, Value& a1, Value& a2, op_t op); // binary construction

    // get a pointer to the value
    shared_ptr<Value> get_self();

    // setters for DAG-related objects
    void set_desc(shared_ptr<Value>& descendant);
    void set_self(weak_ptr<Value>& self);

    // getters 
    int get_indegree() { return indegree; }
    int get_outdegree() { return outdegree; }
    op_t get_op() { return op; }
    double get_val() { return val; }
    bool is_grad_enabled() { return take_grad; }

    // setters
    void set_indegree(int indegree) { this->indegree = indegree; }
    void set_outdegree(int outdegree) { this->outdegree = outdegree; }
    void set_op(op_t op) { this->op = op; }

    // modifiers
    void inc_outdegree() { outdegree++; }
    void inc_indegree() { indegree++; }
    void dec_indegree() { indegree--; }
    void set_grad_disabled() { this->take_grad = false; }
    void set_grad_enabled() {this->take_grad = true; }
    void backward();

    // non-linear activations
    Value& relu();

    // overloaded arithmetic operators;
    friend Value& operator+(Value& lhs, Value& rhs);
    friend Value& operator-(Value& lhs, Value& rhs);
    friend Value& operator*(Value& lhs, Value& rhs);
    friend Value& operator/(Value& lhs, Value& rhs);

    // overloaded boolean operators
    friend bool operator<(const Value& rhs, const Value& lhs);

    // overloaded I/O operators
    friend ostream& operator<<(ostream& os, Value& t);

    // destructor
    ~Value();

private:
    double val;
    double grad;

    int indegree;
    int outdegree;
    op_t op;

    weak_ptr<Value> lhs;
    weak_ptr<Value> rhs;
    weak_ptr<Value> self;

    vector< shared_ptr<Value> > d;
    int identifier;
    bool take_grad;
};

// interface for declaring values 
shared_ptr<Value> make_Value();
shared_ptr<Value> make_Value(double val);
shared_ptr<Value> make_Value(Value& a1, Value& a2);
shared_ptr<Value> make_Value(double val, Value& a1, 
    Value& a2, op_t op);

// private method for printing all nodes in the gradient tape
void _print_nodes();
void _free_nodes();

#endif