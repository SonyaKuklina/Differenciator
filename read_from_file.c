#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "read_from_file.h"
#include "tree_differenciator.h"
#include "tree_calc_private.h"
#include "tables.h"
#include "tree_dsl.h"

enum ProgrammReturn ReadTreeFromFile(TreeCalc* tree_read, int* var_diff) {

    ASSERT(tree_read)
    ASSERT(var_diff)

    FILE* file = fopen("calculation_tree.txt", "r");
    if (file == NULL) return INCORRECT;

    fseek(file , 0 , SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* buffer = (char*)calloc(file_size, sizeof(char));
    if (buffer == NULL) return INCORRECT;

    size_t result_size = fread(buffer, sizeof(char), file_size, file);
    buffer[result_size] = '\0';
    printf("%s", buffer);

    ErrorType error = SUCCESS;

    *var_diff = GetDiffVariable();

    int pos = 0;
    (tree_read -> tree) -> root = GetGeneralExpression(buffer, tree_read, &pos, &error);
    if ((tree_read -> tree) -> root == NULL) return INCORRECT;

    free(buffer);

    return CORRECT;

}

Node* GetGeneralExpression(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    Node* main_node = GetExpression(buffer, tree_read, pos, error);

    if (buffer[*pos] != '\0') { *error |= NULL_POINTER; return SyntaxError(error); }
    return main_node;

}

Node* GetExpression(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    Node* value_1 = GetTerm(buffer, tree_read, pos, error);
    if (IS_NULL(value_1)) { *error |= NULL_POINTER; return SyntaxError(error); }
    SkipSpaces(buffer, pos);

    while (buffer[*pos] == '+' || buffer[*pos] == '-') {

        char op = buffer[*pos];
        (*pos)++;
        SkipSpaces(buffer, pos);

        Node* value_2 = GetTerm(buffer, tree_read, pos, error);
        if (IS_NULL(value_2)) { *error |= NULL_POINTER; return SyntaxError(error); }
        if (op == '+') {
            TreeElement element = OperationElement(ADD);
            value_1 = MAKE_NEW_BINARY_NODE(element, value_1, value_2, TREE(tree_read))
        } else {
            TreeElement element = OperationElement(SUB);
            value_1 = MAKE_NEW_BINARY_NODE(element, value_1, value_2, TREE(tree_read))
        }

    }

    return value_1;
}

Node* GetTerm(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    Node* value_1 = GetPower(buffer, tree_read, pos, error);
    if (IS_NULL(value_1)) { *error |= NULL_POINTER; return SyntaxError(error); }
    SkipSpaces(buffer, pos);

    while (buffer[*pos] == '*' || buffer[*pos] == '/') {

        char op = buffer[*pos];
        (*pos)++;
        SkipSpaces(buffer, pos);

        Node* value_2 = GetPower(buffer, tree_read, pos, error);
        if (IS_NULL(value_2)) { *error |= NULL_POINTER; return SyntaxError(error); }
        if (op == '*') {

            TreeElement element = OperationElement(MULL);
            value_1 = MAKE_NEW_BINARY_NODE(element, value_1, value_2, TREE(tree_read))
            SkipSpaces(buffer, pos);

        } else {

            TreeElement element = OperationElement(DIV);
            value_1 = MAKE_NEW_BINARY_NODE(element, value_1, value_2, TREE(tree_read))
            SkipSpaces(buffer, pos);

        }

    }

    return value_1;
}

Node* GetPower(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    Node* value_1 = GetBasis(buffer, tree_read, pos, error);
    if (IS_NULL(value_1)) return SyntaxError(error);
    SkipSpaces(buffer, pos);

    if (buffer[*pos] == '^') {

        TreeElement element = OperationElement(POW);
        (*pos)++;
        SkipSpaces(buffer, pos);
        Node* value_2 = GetPower(buffer, tree_read, pos, error);
        SkipSpaces(buffer, pos);
        Node* value = MAKE_NEW_BINARY_NODE(element, value_1, value_2, TREE(tree_read))
        return value;

    }

    return value_1;

}



Node* GetBasis(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    if (buffer[*pos] == '(') {

        (*pos)++;
        SkipSpaces(buffer, pos);
        Node* node = GetExpression(buffer, tree_read, pos, error);
        if (IS_NULL(node)) { *error |= NULL_POINTER; return SyntaxError(error); }

        SkipSpaces(buffer, pos);
        if (buffer[*pos] != ')') { *error |= NOT_BRACKET; return SyntaxError(error); }
        (*pos)++;
        SkipSpaces(buffer, pos);
        return node;

    }

    Node* node_num = GetNumber(buffer, tree_read, pos, error);
    if (node_num != NULL) return node_num;

    if (isalpha(buffer[*pos])) {

        char* varriable_or_keyword = ReadCurrentStr(buffer, pos);
        SkipSpaces(buffer, pos);
        enum OperationType curr_op_type = GetKeywordType(varriable_or_keyword, error);
        if (curr_op_type == UNDETIFIED_TYPE) return GetVariable(buffer, tree_read, pos, varriable_or_keyword, error);
        else return GetMathFunction(buffer, tree_read, pos, curr_op_type, error);

    }

    *error |= INVALID_BASIS;
    return SyntaxError(error);

}

Node* GetNumber(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    char* endptr;
    double value = strtod(buffer + *pos, &endptr);

    if (endptr == (buffer + *pos)) return NULL;

    *pos += (endptr - (buffer + *pos));
    SkipSpaces(buffer, pos);

    TreeElement element; element.const_value = value;
    return MAKE_NEW_CONST_NODE(element, TREE(tree_read))

}


void SkipSpaces(char* buffer, int* pos) {

    assert(buffer != NULL);
    assert(pos    != NULL);

    while ((buffer[*pos] == ' ') || (buffer[*pos] == '\n')) (*pos)++;

}

enum OperationType GetKeywordType(char* current_str, ErrorType* error) {

    ASSERT(current_str)
    ASSERT(error)

    struct KeywordsTable* keywords_op_table = CreateKeywordsTable(error);
    if (IS_NULL(keywords_op_table)) { *error |= NULL_POINTER_TO_TABLE; return UNDETIFIED_TYPE;}

    for (int index = 0; index < COUNT_UNARY_OPERATIONS; index++) {

        if (strcmp(current_str, keywords_op_table[index].keyword) == 0) {
            return keywords_op_table[index].op_type;
        }
    }

    return UNDETIFIED_TYPE;
}

int DefineVariableIndex(TreeCalc* tree_read, char var_name) { //возвращает индекс переменной в массиве

    ASSERT(tree_read)

    for (int index = 0; index < tree_read -> var_count; index++) {
        if (tree_read -> variable_array[index].variable_name == var_name) return index;
    }

    int index = tree_read -> var_count;
    tree_read -> variable_array[index].variable_name = var_name;
    tree_read -> variable_array[index].variable_value = GetVariableValue(var_name);//запрашиваем у пользователя
    (tree_read -> var_count)++;

    return index;
}

double GetVariableValue(char var_name) {

    printf("Enter the value of variable %c: ", var_name);
    double var_value = 0;
    scanf("%f", &var_value);
    return var_value;

}

int GetDiffVariable() {

    printf("By what variable will there be differentiation?: ");
    int var_diff = 0;
    scanf("%c", &var_diff);
    return var_diff;

}

char* ReadCurrentStr(char* buffer, int* pos) {

    assert(buffer != NULL);
    assert(pos    != NULL);

    int start = *pos;
    while (isalpha(buffer[*pos])) {
        (*pos)++;
    }

    int len = *pos - start;
    char* result = (char*)calloc(len + 1, sizeof(char));
    if (result == NULL) return NULL;

    strncpy(result, buffer + start, len);
    result[len] = '\0';

    return result;
}

Node* GetMathFunction(char* buffer, TreeCalc* tree_read, int* pos,
                      enum OperationType curr_op_type, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(error)

    if (buffer[*pos] != '(') { *error |= NOT_BRACKET; return SyntaxError(error); }
    (*pos)++;
    SkipSpaces(buffer, pos);

    Node* value = GetExpression(buffer, tree_read, pos, error);
    if (IS_NULL(value)) return NULL;
    SkipSpaces(buffer, pos);

    if (buffer[*pos] != ')') { *error |= NOT_BRACKET; return SyntaxError(error); }
    (*pos)++;
    SkipSpaces(buffer, pos);

    TreeElement element; element.operations = curr_op_type;
    Node* op_node = MAKE_NEW_UNARY_NODE(element, value, TREE(tree_read))
    return op_node;


}

Node* GetVariable(char* buffer, TreeCalc* tree_read, int* pos, char* varriable_or_keyword, ErrorType* error) {

    ASSERT(buffer)
    ASSERT(tree_read)
    ASSERT(pos)
    ASSERT(varriable_or_keyword)
    ASSERT(error)

    if (strlen(varriable_or_keyword) == 1) {

        char var_name = varriable_or_keyword[0];
        int var_index = DefineVariableIndex(tree_read, var_name);
        TreeElement element; element.index_var = var_index;
        Node* node = MAKE_NEW_VAR_NODE(element, tree_read -> tree)
        return node;

    }

   *error |= INVALID_VARIABLE_NAME;
    return SyntaxError(error);

}

Node* SyntaxError(ErrorType* error) {

    ASSERT(error)

    if (*error & MEMORY_ALLOC_ERROR) {
        printf("Memory allocation error!\n");
        return NULL;
    }

    if (*error & NULL_POINTER_TO_NODE) {
        printf("Null pointer to node!\n");
        return NULL;
    }

    if (*error & INVALID_VARIABLE_NAME) {
        printf("Invalid variable name!\n");
        return NULL;
    }

    if (*error & NOT_BRACKET) {
        printf("Not bracket!\n");
        return NULL;
    }

    if (*error & NULL_POINTER) {
        printf("Null pointer to current object!\n");
        return NULL;
    }

    if (*error & INVALID_BASIS) {
        printf("Invalid basis!\n");
        return NULL;
    }

    if (*error & NULL_POINTER_TO_TABLE) {
        printf("Null pointer to table!\n");
        return NULL;
    }

}
