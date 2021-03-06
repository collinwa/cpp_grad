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
    relu_op,
    exp_op
};

// the interface you should use to make values
class Value {
public:

    // constructors -- users shouldn't call these 
    Value();
    Value(double val);
    Value(double val, Value& lhs, op_t op); // unary construction
    Value(double val, Value& a1, Value& a2, op_t op); // binary construction

    // setters for DAG-related objects
    void set_desc(shared_ptr<Value>& descendant);
    void replace_desc(shared_ptr<Value>& new_v,shared_ptr<Value>& old_v);
    void set_self(weak_ptr<Value>& self);

    // getters 
    int get_indegree() { return indegree; }
    int get_outdegree() { return outdegree; }    
    int get_topo_indegree() { return topo_indegree; }
    int get_topo_outdegree() { return topo_outdegree; }

    op_t get_op() { return op; }
    double get_val() { return val; }    
    double get_grad_l() { return grad_l; } 
    double get_grad_r() { return grad_r; } 
    weak_ptr<Value> get_l_ancs() { return lhs; } 
    weak_ptr<Value> get_r_ancs() { return rhs; } 
    bool is_grad_enabled() { return this->take_grad; }
    bool is_alive() { return alive; }

    // get a pointer to the value
    shared_ptr<Value> get_self() { return self.lock(); };

    // setters
    void set_indegree(int indegree) { this->indegree = indegree; }
    void set_outdegree(int outdegree) { this->outdegree = outdegree; }
    void set_topo_indgr() { topo_indegree = indegree; }
    void set_topo_outdgr() { topo_outdegree = outdegree; }

    void set_op(op_t op) { this->op = op; }
    void set_grad(double grad) { 
        this->grad = grad;
        #ifdef PRINTGRAD 
        cout << "grad set to: " << grad << endl;
        #endif
    }

    void set_alive(bool alive) { this->alive = alive; }

    // modifiers
    void inc_indegree() { indegree++; }
    void inc_outdegree() { outdegree++; }
    void dec_indegree() { indegree--; }
    void dec_outdegree() { outdegree--; }

    void inc_topo_indgr() { topo_indegree++; }
    void inc_topo_outdgr() { topo_outdegree++; }
    void dec_topo_indgr() { topo_indegree--; }
    void dec_topo_outdgr() { topo_outdegree--; }    

    void set_grad_disabled() { this->take_grad = false; }
    void set_grad_enabled() {this->take_grad = true; }

    // backpropagation
    void backward();
    void topo_backward();
    void compute_lr_derivatives();

    // chain rule until we hit the base
    void diff(Value& backprop, shared_ptr<Value> base);

    // non-linear activations
    Value& relu();
    Value& exp();

    // overloaded arithmetic operators;
    friend Value& operator+(Value& lhs, Value& rhs);
    friend Value& operator-(Value& lhs, Value& rhs);
    friend Value& operator*(Value& lhs, Value& rhs);
    friend Value& operator/(Value& lhs, Value& rhs);

    // overloaded boolean operators
    friend bool operator<(const Value& rhs, const Value& lhs);
    friend double chain_rule(shared_ptr<Value> cur, shared_ptr<Value> stop);

    // overloaded I/O operators
    friend ostream& operator<<(ostream& os, Value& t);

    // destructor
    ~Value();

private:
    // gradient-related instance vars
    double val;
    double grad; // gradient of interest

    // grad_l and grad_r are derivative of output w.r.t. left/right input
    double grad_l;
    double grad_r;

    op_t op;
    bool take_grad;
    bool alive;

    // DAG-related instance vars
    int indegree;
    int outdegree;
    int topo_indegree;
    int topo_outdegree;

    // ancestors and self pointer
    weak_ptr<Value> lhs;
    weak_ptr<Value> rhs;
    weak_ptr<Value> self;

    void toposort();
    shared_ptr<Value> replace_node(shared_ptr<Value>& to_replace);

    // list of descendants
    vector< shared_ptr<Value> > d;
    vector< double > desc_grads;
    // node id
    int identifier;
};

// interface for declaring values 
shared_ptr<Value> make_Value();
shared_ptr<Value> make_Value(double val);
shared_ptr<Value> make_Value(double val, Value& lhs, op_t op);
shared_ptr<Value> make_Value(double val, Value& lhs, Value& rhs, op_t op);

// private method for printing all nodes in the gradient tape
void _print_nodes();
void _free_nodes();

#endif