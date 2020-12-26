#ifndef __VALUE_H__
#define __VALUE_H__
#include <memory>
#include <vector>

// operation types (?) unclear if needed
enum op_type { free_param, variable_param, scalar_param};
// vector of all values that are to be used
std::vector<Value> ALL_PARAMS;

class Value {
public:
    // constructors of various types
    Value(); // default constructor
    Value(Value& to_copy); // copy constructor
    ~Value(); // destructor

    // copy assignment
    Value& operator=(const Value& to_copy);

    // various overloaded operator types that we need to define behavior for
    // multiplication of two values
    friend Value operator+(Value& p1, Value& p2);
    friend Value operator-(Value& p1, Value& p2);
    friend Value operator/(Value& p1, Value& p2);
    friend Value operator*(Value& p1, Value& p2);

    // we should use an address compare on the underlying object 
    friend Value operator==(Value& p1, Value& p2);

    // methods for computing gradients; make sure that you're computing w.r.t. each DAG root
    void backward(Value& cur_root);
    int indegree();
    int outdegree();

private:
    op_type value_t; // distinguish between different parameter types
    double current_value; // current value (obviously)
    double grad; // value of desired gradient w.r.t to this parameter

    // maintain these metrics for the toposort 
    int indegree;
    int outdegree;

    // in the DAG, maintain weak pointers to parents; strong pointers to descendents
    std::weak_ptr<Value> direct_ancestor; 
    std::shared_ptr<Value> direct_descendent;
    std::weak_ptr<Value> dag_root;
};

#endif