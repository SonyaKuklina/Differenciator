#ifndef STRUCTS_DIFF
#define STRUCTS_DIFF

#include "enum_calc.h"
#include <stddef.h>
#include "tree_errors.h"

union Node_el { //для элемента узла

    enum OperationType operations;
    double const_value;
    int index_var;

};

typedef union Node_el TreeElement;

typedef struct Node_t {

    enum TypeNode type_node;
    TreeElement node_element;
    struct Node_t* left_branch;//YES
    struct Node_t* right_branch;//NO
    struct Node_t* parent;
    int num_node;

} Node;

typedef struct Tree_t {

    struct Node_t* root;
    size_t node_size;

} Tree;

typedef struct ArrayVar {

    char variable_name;
    double variable_value;

} ArrayVar;

typedef struct Tree_calc {

    Tree* tree;
    struct ArrayVar* variable_array;
    int var_count;
    int var_capacity;

} TreeCalc;

//для латеха

typedef struct DerivationStep {

    TreeCalc* tree_state;

} DerivationStep;

typedef struct StepBuffer {

    DerivationStep* steps;
    int capacity;
    int count;
    char var_diff;

} StepBuffer;

struct OperationTable {

    enum OperationType operation_type;
    const char* operation_symbol;

};

struct UnaryOpTable {

    enum OperationType op_type;
    double (*math_function)(double);

};

struct BinaryOpTable {

    enum OperationType op_type;
    size_t (*binary_op_function)(Node*, Tree*, size_t, ErrorType*);

};

struct OpTable {

    enum OperationType op_type;
    const char* latex_code;

};

struct KeywordsTable {

    enum OperationType op_type;
    const char* keyword;
};

struct OperationDiffTable {

    enum OperationType op_type;
    struct Node_t* (*differentiate)(struct Node_t*, struct Tree_calc*, char, struct Tree_t*, ErrorType*, StepBuffer* );

};

struct PriorityOperation {

    enum OperationType op_type;
    enum Priority priority;
};



#endif
