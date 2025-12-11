#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "tree_calc_private.h"
#include "differenciator_operator.h"
#include "tree_differenciator.h"
#include "tree_diff_optimize.h"
#include "tables.h"
#include "tree_errors.h"
#include "tree_dsl.h"
#include "tree_calc_latex.h"


TreeCalc* Differenciator(TreeCalc* tree_function, char variable_diff, ErrorType* error, StepBuffer* buffer) { //возвращаем указатель на продифф дерево

    ASSERT(tree_function)
    ASSERT(error)
    ASSERT(buffer);

    Node* curr_node = ROOT(TREE(tree_function));
    if (curr_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    MAKE_EMPTY_CALC_TREE(new_tree_diff)
    if (new_tree_diff == NULL) { *error |= NULL_POINTER_TO_TREE; return NULL; }

    AddStepToBuffer(buffer, tree_function, error);

    new_tree_diff -> variable_array = tree_function -> variable_array;
    Node* new_root_node = CalculateDerivative(tree_function, variable_diff, curr_node, TREE(new_tree_diff), error, buffer);
    if (new_root_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    (new_tree_diff -> tree) -> root = new_root_node;

    /*TreeCalc* unoptimized_copy = CreateNewTreeForLatex(tree_function, new_root_node, error);
    if (unoptimized_copy == NULL) {
        *error |= MEMORY_ALLOC_ERROR;
        DestroyTreeCalc(new_tree_diff);
        return NULL;
    }*/


    //AddStepToBuffer(buffer, unoptimized_copy, error);

    //DumpTree(new_tree_diff, __FILE__, __LINE__);

    //new_tree_diff -> tree = OptimizeDiffTree(new_tree_diff -> tree, error);

    //DumpTree(new_tree_diff, __FILE__, __LINE__);

    return new_tree_diff;

}

Node* CalculateDerivative(TreeCalc* tree_function, char variable_diff, Node* curr_node,
                          Tree* new_tree_diff, ErrorType* error, StepBuffer* buffer) {//поиск производной от произвольного узла

    ASSERT(tree_function)
    ASSERT(curr_node)
    ASSERT(new_tree_diff)
    ASSERT(error)
    ASSERT(buffer)

    GET_TYPE_NODE(type_node, curr_node)
    struct ArrayVar* array_var = tree_function -> variable_array;
    if (array_var == NULL) { *error |= NULL_POINTER_TO_ARRAY; return NULL; }

    Node* root_node = NULL;

    switch (type_node) {

    case CONST_VALUE: {
                        root_node = DifferenciationConstFunction(curr_node, new_tree_diff, error);
                        break;
                      }

    case VARIABLE:    { root_node = DifferenciationVarFunction(curr_node, tree_function,
                                                               variable_diff, new_tree_diff, error);
                        break;
                      }

    case OPERATION:   { root_node = DifferenciationOpFunction(curr_node, tree_function,
                                                              variable_diff, new_tree_diff, error, buffer);

                        AddDiffAndOptimizeTreeToBuffer(tree_function, root_node, new_tree_diff,
                                                       buffer, error);

                        break; }

    default:          assert(0);
                      break;

    }

    return root_node;

}


Node* DifferenciationOpFunction(Node* current_node, TreeCalc* tree_function,
                                char variable_diff, Tree* new_tree_diff, ErrorType* error,
                                StepBuffer* buffer) {

    ASSERT(current_node)
    ASSERT(tree_function)
    ASSERT(new_tree_diff)
    ASSERT(error)
    ASSERT(buffer)

    GET_OPERATION_TYPE(current_node_type, current_node)
    struct OperationDiffTable* diff_op_table = CreateOperationDiffTable(error);
    if (diff_op_table == NULL) { *error |= NULL_POINTER_TO_TABLE; return NULL; }

    Node* diff_node = NULL;

    for (int index = 0; index < COUNT_OPERATIONS; index++) {

        if (current_node_type == diff_op_table[index].op_type) {//
            diff_node = diff_op_table[index].differentiate(current_node, tree_function,
                                                           variable_diff, new_tree_diff, error, buffer);
            return diff_node;
        }
    }

    *error |= INVALID_NODE_TYPE;
    return NULL;

}


Node* NewNode(enum TypeNode node_type, TreeElement element,
              Node* left, Node* right, Tree* new_tree_diff,
              ErrorType* error) {

    ASSERT(new_tree_diff)
    ASSERT(error)

    Node* new_node = CreateNode(element, node_type, new_tree_diff, error);
    if (new_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    new_node -> left_branch  = left;
    new_node -> right_branch = right;

    if (!IS_NULL(left))  left  -> parent = new_node;
    if (!IS_NULL(right)) right -> parent = new_node;

    return new_node;

}

Node* CopySubtree(Node* tree_func_curr_node, Tree* new_tree_diff, ErrorType* error) {

    ASSERT(error)

    if (tree_func_curr_node == NULL) return NULL;

    Node* copy_curr_node = CreateNode(tree_func_curr_node -> node_element,
                                     tree_func_curr_node -> type_node, new_tree_diff, error);
    if (copy_curr_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    copy_curr_node -> left_branch  = CopySubtree(LEFT(tree_func_curr_node),  new_tree_diff, error);
    copy_curr_node -> right_branch = CopySubtree(RIGHT(tree_func_curr_node), new_tree_diff, error);

    if (!IS_NULL(LEFT(copy_curr_node)))  (copy_curr_node -> left_branch) -> parent  = copy_curr_node;
    if (!IS_NULL(RIGHT(copy_curr_node))) (copy_curr_node -> right_branch) -> parent = copy_curr_node;

    return copy_curr_node;

}

Node* DifferenciationConstFunction(Node* curr_node, Tree* new_tree_diff, ErrorType* error) {

    ASSERT(curr_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    TreeElement tree_element; tree_element.const_value = 0;
    Node* root_node = MAKE_NEW_CONST_NODE(tree_element, new_tree_diff)
    if (root_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    return root_node;

}

Node* DifferenciationVarFunction(Node* curr_node, TreeCalc* tree_function, char variable_diff, Tree* new_tree_diff,
                                 ErrorType* error) {

    ASSERT(curr_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    TreeElement tree_element_0; tree_element_0.const_value = 0;
    TreeElement tree_element_1; tree_element_1.const_value = 1;
    Node* root_node;

    int index_var = GET_INDEX_VAR(curr_node);

    if (GET_VARIABLE_NAME(tree_function, index_var) == variable_diff)
         root_node = MAKE_NEW_CONST_NODE(tree_element_1, new_tree_diff)
    else
         root_node = MAKE_NEW_CONST_NODE(tree_element_0, new_tree_diff)

    if (root_node == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    return root_node;

}


TreeCalc* CreateNewTreeForLatex(TreeCalc* tree_function, Node* root_node, ErrorType* error) {

    ASSERT(tree_function)
    ASSERT(root_node)
    ASSERT(error)

    TreeCalc* tree_state = CreateEmptyCalcTree(error);
    if (tree_state == NULL) { *error |= MEMORY_ALLOC_ERROR; return NULL; }

    (tree_state -> tree) -> root = CopySubtree(root_node, tree_state->tree, error);
    if ((tree_state -> tree) -> root == NULL && root_node != NULL) {
        DestroyTreeCalc(tree_state);
        *error |= NULL_POINTER_TO_NODE;
        return NULL;
    }

    for (int i = 0; i < tree_function -> var_count; i++) {
        tree_state -> variable_array[i] = tree_function -> variable_array[i];
    }

    tree_state -> var_count    = tree_function -> var_count;
    tree_state -> var_capacity = tree_function -> var_capacity;

    return tree_state;

}


void AddDiffAndOptimizeTreeToBuffer(TreeCalc* tree_function, Node* root_node, Tree* new_tree_diff,
                                    StepBuffer* buffer, ErrorType* error) {

    ASSERT(tree_function)
    ASSERT(root_node)
    ASSERT(new_tree_diff)
    ASSERT(buffer)
    ASSERT(error)

    TreeCalc* tree_state_before = CreateNewTreeForLatex(tree_function, root_node, error);
    ASSERT(tree_state_before)
    AddStepToBuffer(buffer, tree_state_before, error);

    Node* root_copy = CopySubtree(root_node, new_tree_diff, error);
    ASSERT(root_copy)

    MAKE_EMPTY_TREE(mini_tree_to_optimize);
    ASSERT(mini_tree_to_optimize)
    mini_tree_to_optimize->root = root_copy;

    mini_tree_to_optimize = OptimizeDiffTree(mini_tree_to_optimize, error);

    TreeCalc* tree_state_after = CreateNewTreeForLatex(tree_function, mini_tree_to_optimize->root, error);
    ASSERT(tree_state_after)
    tree_state_after->tree = mini_tree_to_optimize;
    AddStepToBuffer(buffer, tree_state_after, error);

}
