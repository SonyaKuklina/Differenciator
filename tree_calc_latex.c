#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "tree_calc_latex.h"
#include "tables.h"
#include "tree_dsl.h"

#define MAX_SIZE_BUFFER 100

#define MAX_CAPACITY_BUFFER 100

StepBuffer* CreateStepBuffer(char var_diff, ErrorType* error) {

    ASSERT(error)

    StepBuffer* buffer = (StepBuffer*)calloc(1, sizeof(StepBuffer));
    if (buffer == NULL) {
        *error |= MEMORY_ALLOC_ERROR;
        return NULL;
    }

    buffer -> capacity = MAX_CAPACITY_BUFFER;
    buffer -> steps = (DerivationStep*)calloc(buffer -> capacity, sizeof(DerivationStep));
    if (buffer -> steps == NULL) {
        free(buffer);
        *error |= MEMORY_ALLOC_ERROR;
        return NULL;
    }

    buffer -> count = 0;
    buffer -> var_diff = var_diff;

    return buffer;
}

void AddStepToBuffer(StepBuffer* buffer, TreeCalc* tree_state, ErrorType* error) {

    ASSERT(error)
    ASSERT(buffer)
    ASSERT(tree_state)

    if (buffer == NULL || tree_state == NULL) return;

    if (buffer -> count >= buffer -> capacity) {

        buffer -> capacity *= 2;
        DerivationStep* new_steps = (DerivationStep*)realloc(buffer -> steps,
                                                             buffer -> capacity * sizeof(DerivationStep));

        if (new_steps == NULL) { *error |= MEMORY_ALLOC_ERROR; return; }
        buffer -> steps = new_steps;

    }

    TreeCalc* copy_state = (TreeCalc*)calloc(1, sizeof(TreeCalc));
    if (copy_state == NULL) { *error |= MEMORY_ALLOC_ERROR; return; }

    *copy_state  = *tree_state;
    copy_state -> tree = tree_state -> tree;
    buffer -> steps[buffer -> count].tree_state = copy_state;

    buffer -> count++;

}

void WriteBufferToLatex(StepBuffer* buffer) {

    ASSERT(buffer)

    if (buffer == NULL || buffer -> count == 0) return;

    FILE* file_latex = fopen("derivative_base.tex", "w");
    if (file_latex == NULL) return;

    HeadLatex(file_latex);
    fprintf(file_latex, "\\textbf{Original expression}\n");
    fprintf(file_latex, "\\begin{align*}\n");

    size_t symb_count = 0;

    enum TypeNode type_root = ROOT(TREE(buffer -> steps[0].tree_state)) -> type_node;
    enum Priority priority = (type_root == OPERATION) ? GetOpPriority(GET_OPERATION(ROOT(TREE(buffer -> steps[0].tree_state)))) : NONE_PRIORITY;

    if (buffer->steps[0].tree_state != NULL && buffer->steps[0].tree_state->tree != NULL) {
        WriteNodeLatex(ROOT(TREE(buffer -> steps[0].tree_state)), file_latex, priority, buffer->steps[0].tree_state, &symb_count);
    }

    fprintf(file_latex, "\\end{align*}");

    /*fprintf(file_latex, "\\textbf{Derivative}\n");
    fprintf(file_latex, "\\begin{align*}\n");

    int ind = (buffer->count) - 1;

    type_root = ROOT(TREE(buffer -> steps[ind].tree_state)) -> type_node;
    priority = (type_root == OPERATION) ? GetOpPriority(GET_OPERATION(ROOT(TREE(buffer -> steps[ind].tree_state)))) : NONE_PRIORITY;

    if (buffer->steps[ind].tree_state != NULL && buffer->steps[ind].tree_state->tree != NULL) {
        WriteNodeLatex(ROOT(TREE(buffer -> steps[ind].tree_state)), file_latex, priority, buffer->steps[ind].tree_state, &symb_count);
    }
    symb_count = 0;

    fprintf(file_latex, "\\end{align*}");*/


    //то же самое только с дэшками

    fprintf(file_latex, "\\textbf{Calculating the derivative of a given expression}\n");
    fprintf(file_latex, "\\begin{align*}\n");
    fprintf(file_latex, "\\frac{d}{d%c}(", buffer -> var_diff);

    if (buffer->steps[0].tree_state != NULL && buffer->steps[0].tree_state->tree != NULL) {
        WriteNodeLatex(ROOT(TREE(buffer -> steps[0].tree_state)), file_latex, priority, buffer->steps[0].tree_state, &symb_count);
    }
    symb_count = 0;

    fprintf(file_latex, ")\n");
    fprintf(file_latex, "\\end{align*}\n");

    fprintf(file_latex, "\\textbf{Derivate without optimize}\n");
    fprintf(file_latex, "\\begin{align*}\n");
    int i = (buffer->count) - 2;

    if (buffer->steps[i].tree_state != NULL && buffer->steps[i].tree_state->tree != NULL) {
        WriteNodeLatex(ROOT(TREE(buffer -> steps[i].tree_state)), file_latex, priority, buffer->steps[i].tree_state, &symb_count);
    }
    symb_count = 0;

    fprintf(file_latex, ")\n");
    fprintf(file_latex, "\\end{align*}\n");
    int index_step = 1;

    for (int i = 1; i < buffer->count - 1 ; i += 2) {

            fprintf(file_latex, "\\textbf{Step %d}\n", index_step);
            fprintf(file_latex, "\\begin{align*}\n");

            type_root = ROOT(TREE(buffer -> steps[i].tree_state)) -> type_node;
            priority = (type_root == OPERATION) ? GetOpPriority(GET_OPERATION(ROOT(TREE(buffer -> steps[i].tree_state)))) : NONE_PRIORITY;

            if (buffer->steps[i].tree_state != NULL && buffer->steps[i].tree_state->tree != NULL) {
                WriteNodeLatex(ROOT(TREE(buffer -> steps[i].tree_state)), file_latex, priority, buffer->steps[i].tree_state, &symb_count);
            }

            fprintf(file_latex, " = ");

            if (buffer->steps[i + 1].tree_state != NULL && buffer->steps[i + 1].tree_state->tree != NULL) {
                WriteNodeLatex(ROOT(TREE(buffer -> steps[i + 1].tree_state)), file_latex, priority, buffer->steps[i + 1].tree_state, &symb_count);
            }

            fprintf(file_latex, "\\end{align*}\n");
            index_step++;
            symb_count = 0;

    }

    fprintf(file_latex, "\\end{document}\n");
    fclose(file_latex);
    int result = system("pdflatex -interaction=nonstopmode derivative_base.tex");

    system("start derivative_base.pdf");

}



void DestroyStepBuffer(StepBuffer* buffer) {
    if (buffer == NULL) return;

    for (int i = 0; i < buffer->count; i++) {
        if (buffer -> steps[i].tree_state != NULL) {
            free(buffer -> steps[i].tree_state);
        }
    }

    free(buffer -> steps);
    free(buffer);
}


void HeadLatex(FILE* file_latex) {

    fprintf(file_latex, "\\documentclass[14pt]{article}\n");
    fprintf(file_latex, "\\usepackage{amsmath}\n");
    fprintf(file_latex, "\\usepackage{amssymb}\n");
    fprintf(file_latex, "\\usepackage{array}\n");
    fprintf(file_latex, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file_latex, "\\begin{document}\n");
    fprintf(file_latex, "\\fontsize{12pt}{12pt}\\selectfont\n");

}

bool IsUnaryOperation(enum OperationType op_type) {

    return (op_type == SIN  || op_type == COS  || op_type == TAN ||
            op_type == ASIN || op_type == ACOS || op_type == ATAN ||
            op_type == SINH || op_type == COSH || op_type == TANH ||
            op_type == LN   || op_type == EXP);

}

enum Priority GetOpPriority(enum OperationType op_type) {

    struct PriorityOperation* table = CreatePriorityOpTable();//
    assert(table != NULL);

    for (int i = 0; i < COUNT_OPERATIONS; i++) {
        if (op_type == table[i].op_type) {
            enum Priority result = table[i].priority;
            free(table);
            return result;
        }
    }
    free(table);
    return NONE_PRIORITY;
}

const char* GetOpLatexCode(enum OperationType op_type) {

    struct OpTable* table = CreateOpTable();
    for (int i = 0; i < COUNT_OPERATIONS; i++) {
        if (op_type == table[i].op_type) {
            const char* result = table[i].latex_code;
            free(table);
            return result;
        }
    }
    free(table);
    return "";

}

void WriteNodeLatex(Node* node, FILE* file_latex, enum Priority parent_prio, TreeCalc* tree_der, size_t* symb_count) {

    assert(node       != NULL);
    assert(file_latex != NULL);
    assert(tree_der   != NULL);
    assert(symb_count != NULL);

    switch (node -> type_node) {

    case CONST_VALUE: {

        if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0;}

        fprintf(file_latex, "%g", node -> node_element.const_value);
        break;
    }

    case VARIABLE: {

        if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0; }

        int index_var = node -> node_element.index_var;
        char variable = tree_der -> variable_array[index_var].variable_name;
        fprintf(file_latex, "%c", variable);
        (*symb_count)++;
        break;

    }

    case OPERATION: {

        enum OperationType op_type = node->node_element.operations;
        enum Priority node_prio = GetOpPriority(op_type);
        const char* latex_code = GetOpLatexCode(op_type);

        bool need_paren = (node_prio > parent_prio);

        if (op_type == DIV) {

            if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0;}

            fprintf(file_latex, "\\frac{");
            WriteNodeLatex(node->left_branch, file_latex, NONE_PRIORITY, tree_der, symb_count);
            fprintf(file_latex, "}{");
            WriteNodeLatex(node->right_branch, file_latex, NONE_PRIORITY, tree_der, symb_count);
            fprintf(file_latex, "}");
            return;

        }

        if (op_type == POW) {

            if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0; }

            WriteNodeLatex(node->left_branch, file_latex, NONE_PRIORITY, tree_der, symb_count);
            fprintf(file_latex, "^{");
            WriteNodeLatex(node->right_branch, file_latex, NONE_PRIORITY, tree_der, symb_count);
            fprintf(file_latex, "}");
            return;

        }

        if (IsUnaryOperation(op_type)) {

            if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0; }

            fprintf(file_latex, "%s(", latex_code);
            (*symb_count) += strlen(latex_code);
            WriteNodeLatex(node->right_branch, file_latex, NONE_PRIORITY, tree_der, symb_count);
            fprintf(file_latex, ")");
            (*symb_count) += 2;
            return;

        }

        if (*symb_count >= 20 ) { fprintf(file_latex, "\\newline "); *symb_count = 0; }

        if (need_paren) fprintf(file_latex, "(");
        (*symb_count)++;

        WriteNodeLatex(node->left_branch, file_latex, node_prio, tree_der, symb_count);
        fprintf(file_latex, "%s", latex_code);
        *symb_count += strlen(latex_code);
        WriteNodeLatex(node->right_branch, file_latex, node_prio, tree_der, symb_count);

        if (need_paren) fprintf(file_latex, ")");
        (*symb_count)++;

        break;

    }

    }


}
