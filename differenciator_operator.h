#ifndef DIFFERENCIATOR_OPERATOR
#define DIFFERENCIATOR_OPERATOR

#include "tree_errors.h"
#include "structs_calc.h"

Node* DifferenciationAddOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationMullOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationDivOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationSubOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationPowOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationSinOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationCosOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationTanOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationAtanOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationAsinOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationAcosOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationSinhOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationCoshOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationTanhOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationLnOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationLogOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* DifferenciationExpOperator(Node* current_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer);
Node* SubtreeCopy(Node* current_node, TreeCalc* tree_function,
                  char variable_diff, Tree* aT, ErrorType* error);
Node* NodeDerivative(Node* current_node, TreeCalc* tree_function,
                     char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer);




#endif
