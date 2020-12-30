#ifndef __VALUE_H__
#define __VALUE_H__
#include <memory>
#include <vector>

enum op_t {
    mult,
    free,
    add,
    div,
    sub   
}

std::vector<std::shared_ptr<Value>> ALL_PARAMS;

class Value {
public:
    Value();
    Value(Value&); 
    Value(std::shared_ptr<Value>&, std::shared_ptr<Value>&);

    std::shared_ptr<Value> get_self();
    int set_desc(std::shared_ptr<Value>&);
    int get_indegree();
    int get_oudegree();
    ~Value();

private:
    double val;
    int indegree;
    int outdegree;
    op_t op;

    std::weak_ptr<Value> a1; 
    std::weak_ptr<Value> a2;
    std::weak_ptr<Value> self;

    std::shared_ptr<Value> d;
};
#endif