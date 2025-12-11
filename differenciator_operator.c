#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "differenciator_operator.h"
#include "tree_differenciator.h"
#include "tree_dsl.h"
#include "tree_errors.h"

Node* DifferenciationAddOperator(Node* current_node, TreeCalc* tree_function,
                                char varible_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, varible_diff, aT, error, buffer);
    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, varible_diff, aT, error, buffer);

    return ADD_(dL, dR, aT);
}

Node* DifferenciationMullOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cL = SubtreeCopy(LEFT(current_node), tree_function, variable_diff, aT, error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_MULL(dL, cL, dR, cR, aT);
}

Node* DifferenciationSubOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);

    return SUB_(dL, dR, aT);
}

Node* DifferenciationDivOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cL = SubtreeCopy(LEFT(current_node), tree_function, variable_diff, aT, error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_DIV(dL, cL, dR, cR, aT);
}

Node* DifferenciationPowOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cL = SubtreeCopy(LEFT(current_node), tree_function, variable_diff, aT, error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    if (IS_CONSTANT_VALUE(cR)) {
        double cR_const = GET_CONSTANT_VALUE(cR);
        return MULL_(MULL_(VAL_(cR_const, aT), POW_(cL, VAL_(cR_const - 1, aT), aT), aT), dL, aT);
    }

    return DIFF_POW(dL, cL, dR, cR, aT);
}

Node* DifferenciationSinOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_SIN(cR, dR, aT);
}

Node* DifferenciationCosOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_COS(cR, dR, aT);
}

Node* DifferenciationTanOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_TAN(cR, dR, aT);
}

Node* DifferenciationAtanOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_ATAN(cR, dR, aT);
}

Node* DifferenciationAsinOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_ASIN(cR, dR, aT);
}

Node* DifferenciationAcosOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_ACOS(cR, dR, aT);
}

Node* DifferenciationSinhOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_SINH(cR, dR, aT);
}

Node* DifferenciationCoshOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_COSH(cR, dR, aT);
}

Node* DifferenciationTanhOperator(Node* current_node, TreeCalc* tree_function,
                                  char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_TANH(cR, dR, aT);
}

Node* DifferenciationLnOperator(Node* current_node, TreeCalc* tree_function,
                                char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_LN(cR, dR, aT);
}

Node* DifferenciationLogOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dL = NodeDerivative(LEFT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cL = SubtreeCopy(LEFT(current_node), tree_function, variable_diff, aT, error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_LOG(dL, cL, dR, cR, aT);
}

Node* DifferenciationExpOperator(Node* current_node, TreeCalc* tree_function,
                                 char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dR = NodeDerivative(RIGHT(current_node), tree_function, variable_diff, aT, error, buffer);
    Node* cR = SubtreeCopy(RIGHT(current_node), tree_function, variable_diff, aT, error);

    return DIFF_EXP(cR, dR, aT);
}

Node* NodeDerivative(Node* current_node, TreeCalc* tree_function,
                     char variable_diff, Tree* aT, ErrorType* error, StepBuffer* buffer) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* dN = CalculateDerivative(tree_function, variable_diff, current_node, aT, error, buffer);
    if (dN == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    return dN;
}

Node* SubtreeCopy(Node* current_node, TreeCalc* tree_function,
                  char variable_diff, Tree* aT, ErrorType* error) {

    ASSERT(current_node);
    ASSERT(tree_function);
    ASSERT(error);

    Node* cN = CopySubtree(current_node, aT, error);
    if (cN == NULL) { *error |= NULL_POINTER_TO_NODE; return NULL; }

    return cN;
}








