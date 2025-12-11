#ifndef TREE_DIFF_PRIVATE
#define TREE_DIFF_PRIVATE

#include <stdbool.h>
#include "structs_calc.h"
#include "enum_calc.h"
#include "tree_errors.h"

#define SMALL_CONST 1e-10

Node* CreateNode(TreeElement element, enum TypeNode type_node, Tree* tree, ErrorType* error);
TreeCalc* CreateEmptyCalcTree(ErrorType* error);
Tree* CreateEmptyTree(ErrorType* error);
struct ArrayVar* CreateVariableArray(ErrorType* error);
void DestroySubtree(Node* node);
void DestroyTree(Tree* tree);
enum ProgrammReturn DestroyTreeCalc(TreeCalc* tree_calculations);
Tree* CreateSubtreeForNode(Node* node, ErrorType* error);
size_t DestroySubtreeWithCount(Node* current_node, Tree* tree);

bool ComparisonWithZero(double n);



#endif
