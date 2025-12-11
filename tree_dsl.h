#ifndef TREE_DSL_H
#define TREE_DSL_H

#include "structs_calc.h"
#include "enum_calc.h"
#include "tree_calc_private.h"
#include "tree_differenciator.h"
#include "tree_diff_optimize.h"
#include "tree_errors.h"

static inline TreeElement OperationElement(enum OperationType op_type) {

    TreeElement element;
    element.operations = op_type;
    return element;

}

static inline TreeElement ConstElement(double const_element) {

    TreeElement element;
    element.const_value = const_element;
    return element;

}

static inline Node* LeftBranch(Node* current_node) {

    assert(current_node != NULL);

    return current_node -> left_branch;
}

static inline Node* RightBranch(Node* current_node) {

    assert(current_node != NULL);

    return current_node -> right_branch;
}

static inline Node* Root(Tree* tree) {

    assert(tree != NULL);

    return tree -> root;

}

static inline Tree* TreeT(TreeCalc* tree_calc) {

    assert(tree_calc != NULL);

    return tree_calc -> tree;

}

static inline Node* Parent(Node* current_node) {

    assert(current_node != NULL);

    return current_node -> parent;

}

#define ADD_(L, R, P)    NewNode(OPERATION, OperationElement(ADD),  (L), (R), (P), error)
#define SUB_(L, R, P)    NewNode(OPERATION, OperationElement(SUB),  (L), (R), (P), error)
#define MULL_(L, R, P)   NewNode(OPERATION, OperationElement(MULL), (L), (R), (P), error)
#define DIV_(L, R, P)    NewNode(OPERATION, OperationElement(DIV),  (L), (R), (P), error)
#define POW_(L, R, P)    NewNode(OPERATION, OperationElement(POW),  (L), (R), (P), error)
#define SIN_(X, P)       NewNode(OPERATION, OperationElement(SIN),  NULL, (X), (P), error)
#define COS_(X, P)       NewNode(OPERATION, OperationElement(COS),  NULL, (X), (P), error)
#define TAN_(X, P)       NewNode(OPERATION, OperationElement(TAN),  NULL, (X), (P), error)
#define ATAN_(X, P)      NewNode(OPERATION, OperationElement(ATAN), NULL, (X), (P), error)
#define ASIN_(X, P)      NewNode(OPERATION, OperationElement(ASIN), NULL, (X), (P), error)
#define ACOS_(X, P)      NewNode(OPERATION, OperationElement(ACOS), NULL, (X), (P), error)
#define SINH_(X, P)      NewNode(OPERATION, OperationElement(SINH), NULL, (X), (P), error)
#define COSH_(X, P)      NewNode(OPERATION, OperationElement(COSH), NULL, (X), (P), error)
#define TANH_(X, P)      NewNode(OPERATION, OperationElement(TANH), NULL, (X), (P), error)
#define LN_(X, P)        NewNode(OPERATION, OperationElement(LN),   NULL, (X), (P), error)
#define EXP_(X, P)       NewNode(OPERATION, OperationElement(EXP),  NULL, (X), (P), error)

#define VAL_(X, P)       NewNode(CONST_VALUE, ConstElement(X), NULL, NULL, (P), error)

#define LEFT(X)   (X) -> left_branch
#define RIGHT(X)  (X) -> right_branch
#define ROOT(X)   (X) -> root
#define TREE(X)    TreeT((X))
#define PARENT(X)  Parent((X))

#define ASSERT(X)        assert((X) != NULL);
#define ZERO(X)          (ComparisonWithZero(GET_CONSTANT_VALUE(X)))
#define ONE(X)           (ComparisonWithZero(GET_CONSTANT_VALUE(X) - 1))

#define IS_CHILDREN_NULL(X)   (((X) -> right_branch != NULL) && ((X) -> left_branch != NULL))
#define IS_CONSTANT_VALUE(X)  ((X) -> type_node == CONST_VALUE)
#define IS_VARIABLE_VALUE(X)  ((X) -> type_node == VARIABLE)
#define IS_OPERATION(X)       ((X) -> type_node == OPERATION)
#define IS_CHILDREN_CONST(X)  (IS_CONSTANT_VALUE(LEFT(X)) && IS_CONSTANT_VALUE(RIGHT(X)))
#define IS_ZERO(X)            (IS_CONSTANT_VALUE(X) && ZERO(X))
#define IS_NULL(X)            ((X) == NULL)

#define GET_CONSTANT_VALUE(X)     (X) -> node_element.const_value
#define GET_INDEX_VAR(X)          (X) -> node_element.index_var
#define GET_OPERATION(X)          (X) -> node_element.operations
#define GET_VARIABLE_VALUE(X, Y)  (X) -> variable_array[(Y)].variable_value
#define GET_VARIABLE_NAME(X, Y)   (X)->variable_array[(Y)].variable_name

#define ADD(X, Y)   Add((X), (Y))
#define SUB(X, Y)   Sub((X), (Y))
#define MULL(X, Y)  Mull((X), (Y))
#define POW(X, Y)   Pow((X), (Y))
#define DIV(X, Y)   Div((X), (Y))
#define LOG(X, Y)   Log((X), (Y))

//проверки для логарифма
#define BASE_LOG_GREATER_ZERO(X)  assert(IS_CONSTANT_VALUE(X) && (GET_CONSTANT_VALUE(X) > 0));
#define BASE_LOG_NOT_ONE(X)       assert(IS_CONSTANT_VALUE(X) && ONE(X));
#define VALUE_LOG_FREATER_ZERO(X) assert(IS_CONSTANT_VALUE(X) && (GET_CONSTANT_VALUE(X) > 0));



//функции
#define OPTIMIZE_TWO_CONSTANT_ADD(X, Y, Z)  OptimizeBinaryOperationForTwoConstants((X), ADD(Y, Z), new_size, error);
#define OPTIMIZE_TWO_CONSTANT_SUB(X, Y, Z)  OptimizeBinaryOperationForTwoConstants((X), SUB(Y, Z), new_size, error);
#define OPTIMIZE_TWO_CONSTANT_MULL(X, Y, Z) OptimizeBinaryOperationForTwoConstants((X), MULL(Y, Z), new_size, error);
#define OPTIMIZE_TWO_CONSTANT_POW(X, Y, Z)  OptimizeBinaryOperationForTwoConstants((X), POW(Y, Z), new_size, error);
#define OPTIMIZE_TWO_CONSTANT_DIV(X, Y, Z)  OptimizeBinaryOperationForTwoConstants((X), DIV(Y, Z), new_size, error);
#define OPTIMIZE_TWO_CONSTANT_LOG(X, Y, Z)  OptimizeBinaryOperationForTwoConstants((X), LOG(Y, Z), new_size, error);
#define MAKE_EMPTY_TREE(X)                  Tree* (X) = CreateEmptyTree(error);
#define MAKE_EMPTY_CALC_TREE(X)             TreeCalc* (X) = CreateEmptyCalcTree(error);
#define COPY_SUBTREE(X, Y, Z)               Node* (X)= CopySubtree((Y), (Z), error);
#define DESTROY_SUBTREE(X)                  DestroySubtree((X));

//создание нового узла
#define MAKE_NEW_CONST_NODE(X, Y)        NewNode(CONST_VALUE, (X), NULL, NULL, (Y), error);
#define MAKE_NEW_VAR_NODE(X, Y)          NewNode(VARIABLE, (X), NULL, NULL, (Y), error);
#define MAKE_NEW_BINARY_NODE(X, Y, Z, W) NewNode(OPERATION, (X), (Y), (Z), (W), error);
#define MAKE_NEW_UNARY_NODE(X, Y, Z)     NewNode(OPERATION, (X), NULL, (Y), (Z), error);

#define MAKE_NULL_NODE(X)        (X) = NULL;
#define MAKE_CONST_VALUE_NODE(X) ((X) -> type_node) = CONST_VALUE;
#define MAKE_NODE_A_ROOT(X, Y)   ((X) -> tree) -> root = (Y);
#define GET_TYPE_NODE(X,Y)       enum TypeNode (X) = (Y) -> type_node;
#define GET_OPERATION_TYPE(X,Y)  enum OperationType (X) = (Y) -> node_element.operations;

//простые функции
double Add(double a, double b);
double Mull(double a, double b);
double Sub(double a, double b);
double Div(double a, double b);
double Log(double a, double b);
double Pow(double a, double b);


#define DIFF_MULL(dL, cL, dR, cR, T) \
    ADD_(MULL_((dL), (cR), (T)), MULL_((cL), (dR), (T)), (T))


#define DIFF_DIV(dL, cL, dR, cR, T) \
    DIV_( \
        SUB_(MULL_((dL), (cR), (T)), MULL_((cL), (dR), (T)), (T)), \
        POW_((cR), VAL_(2, (T)), (T)), \
        (T) \
    )

#define DIFF_POW(dL, cL, dR, cR, T) \
    MULL_( \
        POW_((cL), (cR), (T)), \
        ADD_( \
            MULL_((dR), LN_((cL), (T)), (T)), \
            MULL_((cR), DIV_((dL), (cL), (T)), (T)), \
            (T) \
        ), \
        (T) \
    )


#define DIFF_LOG(dL, cL, dR, cR, T) \
    SUB_( \
        DIV_((dR), MULL_((cR), LN_((cL), (T)), (T)), (T)), \
        DIV_( \
            MULL_(LN_((cR), (T)), (dL), (T)), \
            MULL_((cL), POW_(LN_((cL), (T)), VAL_(2, (T)), (T)), (T)), \
            (T) \
        ), \
        (T) \
    )

#define DIFF_LOG(dL, cL, dR, cR, T) \
    SUB_( \
        DIV_((dR), MULL_((cR), LN_((cL), (T)), (T)), (T)), \
        DIV_( \
            MULL_(LN_((cR), (T)), (dL), (T)), \
            MULL_((cL), POW_(LN_((cL), (T)), VAL_(2, (T)), (T)), (T)), \
            (T) \
        ), \
        (T) \
    )

#define DIFF_SIN(u, du, T) \
    MULL_(COS_((u), (T)), (du), (T))

#define DIFF_COS(u, du, T) \
    MULL_(MULL_(VAL_(-1, (T)), SIN_((u), (T)), (T)), (du), (T))

#define DIFF_TAN(u, du, T) \
    MULL_(DIV_(VAL_(1, (T)), POW_(COS_((u), (T)), VAL_(2, (T)), (T)), (T)), (du), (T))

#define DIFF_ATAN(u, du, T) \
    MULL_(DIV_(VAL_(1, (T)), ADD_(VAL_(1, (T)), POW_((u), VAL_(2, (T)), (T)), (T)), (T)), (du), (T))

#define DIFF_ASIN(u, du, T) \
    MULL_( \
        DIV_(VAL_(1, (T)), POW_(SUB_(VAL_(1, (T)), POW_((u), VAL_(2, (T)), (T)), (T)), VAL_(0.5, (T)), (T)), (T)), \
        (du), \
        (T) \
    )


#define DIFF_ACOS(u, du, T) \
    MULL_( \
        DIV_( \
            VAL_(-1, (T)), POW_(SUB_(VAL_(1, (T)), \
            POW_((u), VAL_(2, (T)), (T)), (T)), VAL_(0.5, (T)), (T)), (T)), \
            (du), \
            (T) \
    )


#define DIFF_SINH(u, du, T) \
    MULL_(COSH_((u), (T)), (du), (T))


#define DIFF_COSH(u, du, T) \
    MULL_(SINH_((u), (T)), (du), (T))

#define DIFF_TANH(u, du, T) \
    MULL_(SUB_(VAL_(1, (T)), POW_(TANH_((u), (T)), VAL_(2, (T)), (T)), (T)), (du), (T))

#define DIFF_LN(u, du, T) \
    MULL_(DIV_(VAL_(1, (T)), (u), (T)), (du), (T))

#define DIFF_EXP(u, du, T) \
    MULL_(EXP_((u), (T)), (du), (T))


#endif
