#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "tree_calc_private.h"
#include "tree_errors.h"
#include "tree_dsl.h"

#define MAX_SIZE_VARIABLE_ARRAY 100

TreeCalc* CreateEmptyCalcTree(ErrorType* error) {

    ASSERT(error);

    TreeCalc* tree_calculations = (TreeCalc*)calloc(1, sizeof(TreeCalc));
    if (tree_calculations == NULL) { *error |= NULL_POINTER_TO_TREE; return NULL; }

    tree_calculations -> tree = CreateEmptyTree(error);
    tree_calculations -> variable_array = CreateVariableArray(error);
    tree_calculations -> var_count = 0;
    tree_calculations -> var_capacity = MAX_SIZE_VARIABLE_ARRAY;
    return tree_calculations;

}

Tree* CreateEmptyTree(ErrorType* error) {

    ASSERT(error);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if (tree == NULL) { *error |= NULL_POINTER_TO_TREE; return NULL; }

    tree -> root = NULL;
    tree -> node_size = 0;
    return tree;

}

struct ArrayVar* CreateVariableArray(ErrorType* error) {

    ASSERT(error);

    struct ArrayVar* varriable_array = (struct ArrayVar*)calloc(MAX_SIZE_VARIABLE_ARRAY, sizeof(struct ArrayVar));
    if (varriable_array == NULL) { *error |= NULL_POINTER_TO_ARRAY; return NULL; }
    return varriable_array;

}

Node* CreateNode(TreeElement element, enum TypeNode type_node, Tree* tree, ErrorType* error) {

    ASSERT(tree);
    ASSERT(error);

    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == NULL) { *error |= MEMORY_ALLOC_ERROR; return NULL; }

    if (type_node != OPERATION && type_node != VARIABLE && type_node != CONST_VALUE)
         { *error |= INVALID_NODE_TYPE; return NULL; }

    switch(type_node) {

    case OPERATION:   GET_OPERATION(node) = element.operations;
                      break;
    case VARIABLE:    GET_INDEX_VAR(node) = element.index_var;
                      break;
    case CONST_VALUE: GET_CONSTANT_VALUE(node) = element.const_value;
                      break;
    default:          assert(0);
                      break;

    }

    node -> type_node    = type_node;
    node -> left_branch  = NULL;
    node -> right_branch = NULL;
    node -> parent       = NULL;
    node -> num_node     = tree -> node_size;
    (tree -> node_size)++;
    return node;

}

void DestroySubtree(Node* node) {

    if (IS_NULL(node)) return;

    DestroySubtree(LEFT(node));
    DestroySubtree(RIGHT(node));
    free(node);

}

void DestroyTree(Tree* tree) {

    if (IS_NULL(tree)) return;
    DestroySubtree(ROOT(tree));
    free(tree);

}

enum ProgrammReturn DestroyTreeCalc(TreeCalc* tree_calculations) {

    if (IS_NULL(tree_calculations)) return INCORRECT;

    DestroyTree(TREE(tree_calculations));

    tree_calculations -> var_count = 0;
    tree_calculations -> var_capacity = 0;

    free(tree_calculations -> variable_array);
    free(tree_calculations);

    return CORRECT;

}

Tree* CreateSubtreeForNode(Node* node, ErrorType* error) {

    assert(node != NULL);
    struct Tree_t* tree = CreateEmptyTree(error);
    assert(tree != NULL);
    tree -> root = node;
    return tree;

}

bool ComparisonWithZero(double n){

    return (fabs(n) <= SMALL_CONST);

}

// Удаляет поддерево и возвращает количество удаленных узлов
size_t DestroySubtreeWithCount(Node* current_node, Tree* tree) {

    if (IS_NULL(current_node)) return 0;

    if (!IS_NULL(PARENT(current_node))) {
        if (LEFT(PARENT(current_node))  == current_node)
            MAKE_NULL_NODE((current_node -> parent) -> left_branch)
        if (RIGHT(PARENT(current_node)) == current_node)
            MAKE_NULL_NODE((current_node -> parent) -> right_branch)
    }
    size_t count = 1; // считаем текущий узел

    count += DestroySubtreeWithCount(LEFT(current_node), tree);
    count += DestroySubtreeWithCount(RIGHT(current_node), tree);

    MAKE_NULL_NODE(current_node -> left_branch)
    MAKE_NULL_NODE(current_node -> right_branch)
    MAKE_NULL_NODE(current_node -> parent)

    free(current_node);
    (tree -> node_size)--;

    return count;
}



