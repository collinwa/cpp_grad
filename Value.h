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
    sub   
};

class Value {
public:
    // static interface for value construction
    static shared_ptr<Value> make_Value();
    static shared_ptr<Value> make_Value(double val);
    static shared_ptr<Value> make_Value(Value& a1, Value& a2);
    static shared_ptr<Value> make_Value(double val, Value& a1, 
        Value& a2, op_t op);

    // constructors -- users shouldn't call these 
    Value();
    Value(double val);
    Value(double val, Value& a1, Value& a2, op_t op);

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

    // setters
    void set_indegree(int indegree) { this->indegree = indegree; }
    void set_outdegree(int outdegree) { this->outdegree = outdegree; }
    void set_op(op_t op) { this->op = op; }

    // modifiers
    void inc_outdegree() { outdegree++; }
    void inc_indegree() { indegree++; }
    void dec_indegree() { indegree--; }

    // overloaded operators;
    friend Value& operator+(Value& lhs, Value& rhs);
    friend Value& operator-(Value& lhs, Value& rhs);
    friend Value& operator*(Value& lhs, Value& rhs);
    friend Value& operator/(Value& lhs, Value& rhs);
    friend bool operator>(Value& rhs, Value& lhs);
    friend bool operator<(Value& rhs, Value& lhs);

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

    shared_ptr<Value> d;
    int identifier;
};

#endif