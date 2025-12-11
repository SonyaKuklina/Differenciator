#ifndef TREE_DIFF_OPTIMIZE
#define TREE_DIFF_OPTIMIZE

#include <stdbool.h>
#include "enum_calc.h"
#include "structs_calc.h"
#include "tree_errors.h"

Tree* OptimizeDiffTree(Tree* new_tree_diff, ErrorType* error);
size_t CollapseConstant(Tree* new_tree_diff, ErrorType* error);
size_t CollapseConstantRecurse(Node* current_node, Tree* new_tree_diff, ErrorType* error);
size_t OptimizeOperationNode(Node* current_node, Tree* new_tree_diff,
                            size_t current_size, ErrorType* error);
size_t OptimizeUnaryOperations(Node* current_node, Tree* new_tree_diff, size_t current_size,
                               ErrorType* error) ;
size_t OptimizeBinaryOperations(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizeAddOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizeMullOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizeSubOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizeDivOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizeLogOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t OptimizePowOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
void ChangeNodeNumNewTree(Node* current_node, Tree* new_diff_tree);
size_t OptimizeBinaryOperationForTwoConstants(Node* current_node, double result, size_t new_size, ErrorType* error);

double Add(double a, double b);
double Mull(double a, double b);
double Sub(double a, double b);
double Div(double a, double b);
double Log(double a, double b);
double Pow(double a, double b);

double GetResultUnaryOp(enum OperationType current_operation, double arg_value, ErrorType* error);
void ChangeCurrentNode(Node* current_node, double result, size_t* new_size);
size_t GetSizeTreeAfterBinaryOp(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error);
size_t DeleteOneSubtree(Node* current_node, Node* start_copy_subtree, Tree* new_tree_diff, size_t new_size, ErrorType* error);
size_t ReplaceSubtreeToOneNode(Node* current_node, Tree* new_tree_diff, size_t new_size, double element, ErrorType* error);
bool OperationIsBinary(enum OperationType current_operation);

#endif
