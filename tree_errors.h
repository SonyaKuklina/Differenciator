#ifndef TREE_ERRORS
#define TREE_ERRORS

#include <stdio.h>
#include <stdint.h>

typedef uint32_t ErrorType;
#include "structs_calc.h"

enum Err_t {

    SUCCESS = 0,
    NULL_POINTER_TO_NODE     = 1 << 1,
    NULL_POINTER_TO_TREE     = 1 << 2,
    MEMORY_ALLOC_ERROR       = 1 << 3,
    NULL_POINTER_TO_ARRAY    = 1 << 4,
    NULL_POINTER_TO_TABLE    = 1 << 5,
    INVALID_NODE_TYPE        = 1 << 6,
    DIVISION_BY_ZERO         = 1 << 7,
    ORPHAN_NODE              = 1 << 8,
    PARENT_CHILD_MISMATCH    = 1 << 9, //родитель не ссылается на ребёнка
    INVALID_OPERATION_TYPE   = 1 << 10,
    INVALID_CONST_VALUE      = 1 << 11,
    INVALID_VARIABLE_INDEX   = 1 << 12,
    INVALID_BINARY_OP_ARGS   = 1 << 13,
    INVALID_UNARY_OP_ARGS    = 1 << 14,
    LEAF_NODE_WITH_CHILDREN  = 1 << 15,

    INVALID_VARIABLE_NAME = 1 << 16,
    NOT_BRACKET           = 1 << 17,
    NULL_POINTER          = 1 << 18,
    INVALID_BASIS         = 1 << 19,

};

/*ErrorType ValidateTree(Tree* tree, struct TreeCalc* calc_tree, ErrorType* error);
ErrorType ValidateNode(Node* node, ErrorType* error);
void PrintValidationErrors(ErrorType* error);
bool IsTreeValid(ErrorType* error);  // Проверка, что нет ошибок*/


#endif
