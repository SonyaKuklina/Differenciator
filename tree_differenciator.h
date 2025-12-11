#ifndef TREE_DIFFERENCIATOR
#define TREE_DIFFERENCIATOR

#include "structs_calc.h"
#include "enum_calc.h"
#include "tree_errors.h"

TreeCalc* Differenciator(TreeCalc* tree_function, char variable_diff, ErrorType* error, StepBuffer* buffer);
Node* CalculateDerivative(TreeCalc* tree_function, char variable_diff, Node* curr_node,
                          Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationOpFunction(Node* current_node, TreeCalc* tree_function,
                               char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* NewNode(enum TypeNode node_type, TreeElement element,
              Node* left, Node* right, Tree* new_tree_diff,
              ErrorType* error);
Node* CopySubtree(Node* tree_func_curr_node, Tree* new_tree_diff, ErrorType* error);
Node* DifferenciationConstFunction(Node* curr_node, Tree* new_tree_diff, ErrorType* error);
Node* DifferenciationVarFunction(Node* curr_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff,
                                 ErrorType* error);
TreeCalc* CreateNewTreeForLatex(TreeCalc* tree_function, Node* root_node, ErrorType* error);
void AddDiffAndOptimizeTreeToBuffer(TreeCalc* tree_function, Node* root_node, Tree* new_tree_diff,
                                    StepBuffer* buffer, ErrorType* error);

#endif
