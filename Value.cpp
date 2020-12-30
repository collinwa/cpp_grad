#include <iostream>
#include <algorithm>
#include "Value.h"

Value::Value() {
    op = free;
    indegree = 0;
    outdegree = 0;
    val = 0.0;
}

Value::Value(Value& to_copy) {
    // get the indegree and outdegree
    indegree = to_copy.get_indegree();
    outdegree = to_copy.get_outdegree();

}

int Value::get_indegree() {
    return indegree;
}

int Value::get_outdegree() {
    return outdegree;
}

