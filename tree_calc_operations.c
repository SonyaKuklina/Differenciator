#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "tree_calc_private.h"
#include "tree_calc_operations.h"
#include "tree_dsl.h"


double TreeCalcT(Node* curr_node, TreeCalc* tree_calculations, ErrorType* error) {

    ASSERT(curr_node)
    ASSERT(tree_calculations)
    ASSERT(error)

    if (IS_VARIABLE_VALUE(curr_node)) {

        int index = GET_INDEX_VAR(curr_node);
        return GET_VARIABLE_VALUE(tree_calculations, index);

    }
    if (IS_CONSTANT_VALUE(curr_node)) return GET_CONSTANT_VALUE(curr_node);

    double left_subtree_value = 0;
    if (!IS_NULL(LEFT(curr_node))) left_subtree_value   = TreeCalcT(LEFT(curr_node), tree_calculations, error);
    double right_subtree_value = 0;
    if (!IS_NULL(RIGHT(curr_node))) right_subtree_value = TreeCalcT(RIGHT(curr_node), tree_calculations, error);

    if (IS_OPERATION(curr_node))
        return SubTreeCalculation(curr_node, left_subtree_value, right_subtree_value, error);

    *error |= INVALID_NODE_TYPE; return NAN;

}

double SubTreeCalculation(Node* curr_node, double left_subtree_value, double right_subtree_value, ErrorType* error) {

    ASSERT(curr_node);
    ASSERT(error);

    GET_OPERATION_TYPE(operation_type, curr_node)

    switch(operation_type) {

    case ADD:    return (left_subtree_value + right_subtree_value);

    case MULL:   return (left_subtree_value * right_subtree_value);

    case SUB:    return (left_subtree_value - right_subtree_value);

    case DIV:    if (right_subtree_value != 0) return (left_subtree_value / right_subtree_value);
                 else { *error |= DIVISION_BY_ZERO; return NAN; }

    case POW:    return pow(left_subtree_value, right_subtree_value);

    case SIN:    return sin(right_subtree_value);

    case COS:    return cos(right_subtree_value);

    case TAN:    return tan(right_subtree_value);

    case ATAN:   return atan(right_subtree_value);

    case ASIN:   return asin(right_subtree_value);

    case ACOS:   return acos(right_subtree_value);

    case SINH:   return sinh(right_subtree_value);

    case COSH:   return cosh(right_subtree_value);

    case TANH:   return tanh(right_subtree_value);

    case LN:     return log(right_subtree_value);

    case LOG:    return log(right_subtree_value) / log(left_subtree_value);

    case EXP:    return exp(right_subtree_value);

    default:     assert(0);
                 return NAN;

    }

}
