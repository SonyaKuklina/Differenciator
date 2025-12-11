#ifndef TREE_CALC_OPERATIONS
#define TREE_CALC_OPERATIONS

#include "structs_calc.h"
#include "enum_calc.h"
#include "tree_errors.h"

double TreeCalcT(Node* curr_node, TreeCalc* tree_calculations, ErrorType* error);
double SubTreeCalculation(Node* curr_node, double left_subtree_value, double right_subtree_value, ErrorType* error);

#endif
