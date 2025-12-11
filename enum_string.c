#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "enum_string.h"
#include "enum_calc.h"
#include "structs_calc.h"

const char* GetOperationName(struct Node_t* node) {

    assert(node != NULL);

    const char* operation_name[] = {

        "ADD",  "MULL", "DIV", "SUB",
        "POW",  "SIN",  "COS", "TAN",
        "ATAN", "ASIN", "ACOS",
        "SINH", "COSH", "TANH",
        "LN",   "LOG",  "EXP",

    };

    enum OperationType type_el = node -> node_element.operations;//
    assert(type_el >= 0 && type_el <= 17);
    return operation_name[type_el];
}

const char* GetTypeNodeName(struct Node_t* node) {

    assert(node != NULL);

    const char* type_node_name[] = {

        "OPERATION",
        "VARIABLE",
        "CONST_VALUE",

    };

    enum TypeNode type_node = node -> type_node;
    assert(type_node >= 0 && type_node < (sizeof(type_node_name)/sizeof(type_node_name[0])));
    return type_node_name[type_node];
}

