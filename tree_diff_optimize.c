#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "tree_diff_optimize.h"
#include "enum_calc.h"
#include "tree_calc_private.h"
#include "tree_differenciator.h"
#include "tables.h"
#include "tree_dsl.h"

Tree* OptimizeDiffTree(Tree* new_tree_diff, ErrorType* error) {

    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t tree_size_old = new_tree_diff -> node_size;
    size_t tree_size_new = CollapseConstant(new_tree_diff, error);

    while (tree_size_old != tree_size_new) {

        tree_size_old = tree_size_new;
        tree_size_new = CollapseConstant(new_tree_diff, error);

    }

    return new_tree_diff;

}

size_t CollapseConstant(Tree* new_tree_diff, ErrorType* error) {

    ASSERT(new_tree_diff)
    ASSERT(error)

    Node* current_node = ROOT(new_tree_diff);

    size_t tree_size_new = CollapseConstantRecurse(current_node, new_tree_diff, error);

    return tree_size_new;

}


size_t CollapseConstantRecurse(Node* current_node, Tree* new_tree_diff, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t current_size = 1; //считает текущий узел

    if (!IS_NULL(LEFT(current_node)))
        current_size += CollapseConstantRecurse(LEFT(current_node), new_tree_diff, error);
    if (!IS_NULL(RIGHT(current_node)))
        current_size += CollapseConstantRecurse(RIGHT(current_node), new_tree_diff, error);

    if (IS_OPERATION(current_node))
        current_size = OptimizeOperationNode(current_node, new_tree_diff, current_size, error);

    return current_size;

}

size_t OptimizeOperationNode(Node* current_node, Tree* new_tree_diff,
                            size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    GET_OPERATION_TYPE(current_operation, current_node)
    size_t new_size = 0;

    if (OperationIsBinary(current_operation))
        new_size = OptimizeBinaryOperations(current_node, new_tree_diff, current_size, error);
    else
        new_size = OptimizeUnaryOperations(current_node, new_tree_diff, current_size, error);

    return new_size;


}

size_t OptimizeUnaryOperations(Node* current_node, Tree* new_tree_diff, size_t current_size,
                               ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    GET_OPERATION_TYPE(current_operation, current_node)
    size_t new_size = current_size;

    if ((!IS_NULL(RIGHT(current_node))) &&(!IS_NULL(LEFT(current_node)))) return current_size;
    if (!IS_CONSTANT_VALUE(RIGHT(current_node))) return current_size;

    double arg_value = GET_CONSTANT_VALUE(RIGHT(current_node));
    double result    = GetResultUnaryOp(current_operation, arg_value, error);

    ChangeCurrentNode(current_node, result, &new_size);

    return new_size;

}

size_t OptimizeBinaryOperations(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = 0;

    new_size = GetSizeTreeAfterBinaryOp(current_node, new_tree_diff, current_size, error);

    return new_size;

}

size_t OptimizeAddOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = current_size;

    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return  OPTIMIZE_TWO_CONSTANT_ADD(current_node, left_node, right_node)

    }

    // 2 Случай. Левое поддерево 0 правое поддерево не константа
    if (IS_CONSTANT_VALUE(LEFT(current_node)) && ZERO(LEFT(current_node))) {

        return DeleteOneSubtree(current_node, RIGHT(current_node), new_tree_diff, current_size, error);

    }

    // 3 Случай. Правое поддерево 0, левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ZERO(RIGHT(current_node))) {

        return DeleteOneSubtree(current_node, LEFT(current_node), new_tree_diff, current_size, error);

    }

    return new_size;

}

size_t OptimizeMullOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = current_size;

    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return OPTIMIZE_TWO_CONSTANT_MULL(current_node, left_node, right_node)

    }

    // 2 Случай. Левое поддерево 1 правое поддерево не константа
    if (IS_CONSTANT_VALUE(LEFT(current_node)) && ONE(LEFT(current_node))) {

        return DeleteOneSubtree(current_node, RIGHT(current_node), new_tree_diff, current_size, error);

    }

    // 3 Случай. Правое поддерево 1, левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ONE(RIGHT(current_node))) {

        return DeleteOneSubtree(current_node, LEFT(current_node), new_tree_diff, current_size, error);

    }

    // 4 Случай. Левое поддерево 0 или правое поддерево 0
    if (IS_CONSTANT_VALUE(LEFT(current_node))  && ZERO(LEFT(current_node))||
        IS_CONSTANT_VALUE(RIGHT(current_node)) && ZERO(RIGHT(current_node))) {

        return ReplaceSubtreeToOneNode(current_node, new_tree_diff, current_size, 0, error);

    }

    return new_size;

}

size_t OptimizeSubOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = current_size;
    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return OPTIMIZE_TWO_CONSTANT_SUB(current_node, left_node, right_node)

    }

    // 2 Случай. Правое поддерево 0, левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ZERO(RIGHT(current_node))) {

        return DeleteOneSubtree(current_node, LEFT(current_node), new_tree_diff, current_size, error);

    }

    return new_size;

}

size_t OptimizePowOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = current_size;

    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return OPTIMIZE_TWO_CONSTANT_POW(current_node, left_node, right_node)

    }

    // 2 Случай. Правое поддерево 1, левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ONE(RIGHT(current_node))) {

        return DeleteOneSubtree(current_node, LEFT(current_node), new_tree_diff, current_size, error);

    }
    /////3 случай когда правое поддерево 0
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ZERO(RIGHT(current_node))) {

        return ReplaceSubtreeToOneNode(current_node, new_tree_diff, current_size, 1, error);

    }

    return new_size;

}

size_t OptimizeDivOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    size_t new_size = current_size;

    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return OPTIMIZE_TWO_CONSTANT_DIV(current_node, left_node, right_node)

    }

    // 2 Случай. Левое поддерево 0 правое поддерево не константа
    if (IS_CONSTANT_VALUE(LEFT(current_node)) && ZERO(LEFT(current_node))) {

        return ReplaceSubtreeToOneNode(current_node, new_tree_diff, current_size, 0, error);

    }

    // 3 Случай. Правое поддерево 1, левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ONE(RIGHT(current_node))) {

        return DeleteOneSubtree(current_node, LEFT(current_node), new_tree_diff, current_size, error);

    }

    return new_size;

}

size_t OptimizeLogOperation(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    BASE_LOG_GREATER_ZERO(LEFT(current_node))
    BASE_LOG_NOT_ONE(LEFT(current_node))
    VALUE_LOG_FREATER_ZERO(RIGHT(current_node))

    size_t new_size = current_size;

    // 1 Случай. Левое и правое поддеревья - константы
    if (IS_CHILDREN_CONST(current_node)) {

        double left_node  = GET_CONSTANT_VALUE(LEFT(current_node));
        double right_node = GET_CONSTANT_VALUE(RIGHT(current_node));
        return OPTIMIZE_TWO_CONSTANT_LOG(current_node, left_node, right_node)

    }

    // 2 Случай. Правое поддерево 1 левое поддерево не константа
    if (IS_CONSTANT_VALUE(RIGHT(current_node)) && ONE(RIGHT(current_node))) {

        return ReplaceSubtreeToOneNode(current_node, new_tree_diff, current_size, 0, error);

    }

    return new_size;

}

void ChangeNodeNumNewTree(Node* current_node, Tree* new_diff_tree) {

    ASSERT(current_node)
    ASSERT(new_diff_tree)

    current_node -> num_node = new_diff_tree -> node_size;
    (new_diff_tree -> node_size)++;

    if (!IS_NULL(LEFT(current_node)))  ChangeNodeNumNewTree(LEFT(current_node), new_diff_tree);
    if (!IS_NULL(RIGHT(current_node))) ChangeNodeNumNewTree(RIGHT(current_node), new_diff_tree);

    return;

}

size_t OptimizeBinaryOperationForTwoConstants(Node* current_node, double result, size_t new_size, ErrorType* error) {

    ASSERT(current_node)

    double left_const  = GET_CONSTANT_VALUE(LEFT(current_node));
    double right_const = GET_CONSTANT_VALUE(RIGHT(current_node));

    TreeElement new_element; new_element.const_value = result;
    current_node -> node_element = new_element;
    MAKE_CONST_VALUE_NODE(current_node)

    DESTROY_SUBTREE(LEFT(current_node)); DESTROY_SUBTREE(RIGHT(current_node));
    MAKE_NULL_NODE(current_node -> left_branch)
    MAKE_NULL_NODE(current_node -> right_branch)

    return new_size - 2;

}

double Add(double a, double b) { return a + b; }
double Mull(double a, double b) { return a * b; }
double Sub(double a, double b) { return a - b; }
double Div(double a, double b) { if (b != 0) return a / b; else return NAN; }
double Log(double a, double b) { return log(b) / log(a); }
double Pow(double a, double b) { return pow(a, b); }


double GetResultUnaryOp(enum OperationType current_operation, double arg_value, ErrorType* error) {

    ASSERT(error)

    struct UnaryOpTable* unary_op_table  = CreateUnaryOpTable(error);
    if (unary_op_table == NULL) { *error |= NULL_POINTER_TO_TABLE; return 0; }

    double result = 0;
    for (int index = 0; index < COUNT_UNARY_OPERATIONS; index++) {

        if (current_operation == unary_op_table[index].op_type) {
            result = unary_op_table[index].math_function(arg_value);
            break;
        }

    }

    free(unary_op_table);
    return result;

}

void ChangeCurrentNode(Node* current_node, double result, size_t* new_size) {

    ASSERT(current_node)
    ASSERT(new_size)

    MAKE_CONST_VALUE_NODE(current_node)
    TreeElement new_element; new_element.const_value = result;
    current_node -> node_element = new_element;
    DestroySubtree(RIGHT(current_node));
    MAKE_NULL_NODE(current_node -> right_branch)
    (*new_size)--;

    return;

}

size_t GetSizeTreeAfterBinaryOp(Node* current_node, Tree* new_tree_diff, size_t current_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    struct BinaryOpTable* binary_op_table = CreateBinaryOpTable(error);
    if (IS_NULL(binary_op_table)) { *error |= NULL_POINTER_TO_TABLE; return current_size; }

    size_t new_size = current_size;
    GET_OPERATION_TYPE(current_operation, current_node)

    for (int index = 0; index < COUNT_BINARY_OPERATIONS; index++) {

        if (current_operation == binary_op_table[index].op_type) {
            new_size = binary_op_table[index].binary_op_function(current_node, new_tree_diff, current_size, error);
            break;
        }

    }

    free(binary_op_table);
    return new_size;


}

size_t DeleteOneSubtree(Node* current_node, Node* start_copy_subtree, Tree* new_tree_diff, size_t new_size, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(start_copy_subtree)

    MAKE_EMPTY_TREE(copy_subtree)
    if (copy_subtree == NULL) { *error |= NULL_POINTER_TO_TREE; return new_size; }

    Node* copy_node = CopySubtree(start_copy_subtree, copy_subtree, error);
    if (copy_node == NULL) { *error |= NULL_POINTER_TO_NODE; return new_size; }

    copy_subtree -> root = copy_node;  // создаём копию правого поддерева у текущего узла

    if (IS_NULL(start_copy_subtree)) return new_size;

    Node* new_parent = current_node -> parent;
    if (IS_NULL(new_parent)) {
        // Если нет родителя, делаем копируемое поддерево корнем
        new_tree_diff -> root = copy_node;
        new_tree_diff -> node_size = copy_subtree -> node_size;
        copy_node -> parent = NULL;
        DestroySubtree(current_node);
        //free(copy_subtree);
        return copy_subtree -> node_size;
    }

    bool is_left = (LEFT(new_parent) == current_node);
    if (is_left) new_parent -> left_branch = copy_node;
    else new_parent -> right_branch = copy_node;

    copy_node -> parent = new_parent;
    DestroySubtreeWithCount(current_node, new_tree_diff);//занумеровать узлы из нового дерева
    ChangeNodeNumNewTree(copy_node, new_tree_diff);

    free(copy_subtree);
    return new_size - 2;

}

size_t ReplaceSubtreeToOneNode(Node* current_node, Tree* new_tree_diff, size_t new_size, double element, ErrorType* error) {

    ASSERT(current_node)
    ASSERT(new_tree_diff)
    ASSERT(error)

    Node* new_parent = PARENT(current_node);
    bool is_left = false;
    if (!IS_NULL(new_parent)) is_left = (LEFT(new_parent) == current_node);

    size_t size_delete_elements = DestroySubtreeWithCount(current_node, new_tree_diff);
    new_size = new_size - size_delete_elements + 1;

    TreeElement new_element; new_element.const_value = element;
    Node* one_node = MAKE_NEW_CONST_NODE(new_element, new_tree_diff);
    if (one_node == NULL) { *error |= NULL_POINTER_TO_NODE; return new_size; }

    if (new_parent != NULL) {

        one_node -> parent = new_parent;
        if (is_left) new_parent -> left_branch = one_node;
        else new_parent -> right_branch = one_node;

    } else {

        new_tree_diff -> root = one_node;
        new_tree_diff -> node_size = 1;

    }

    return new_size;

}

bool OperationIsBinary(enum OperationType current_operation) {

    bool is_binary = false;

    switch (current_operation) {

    case SIN:  case COS:  case TAN:
    case ATAN: case ASIN: case ACOS:
    case SINH: case COSH: case TANH:
    case LN:   case EXP: break;

    case ADD: case MULL: case DIV:
    case SUB: case LOG:  case POW: {is_binary = true; break; }

    default: assert(0); break;

    }

    return is_binary;

}




