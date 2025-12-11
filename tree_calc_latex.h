#ifndef TREE_CALC_LATEX
#define TREE_CALC_LATEX

#include <stdbool.h>
#include "structs_calc.h"
#include "enum_calc.h"

void WriteNodeLatex(struct Node_t* node, FILE* file_latex, enum Priority parent_prio, struct Tree_calc* tree_der, size_t* symb_count);
const char* GetOpLatexCode(enum OperationType op_type);
enum Priority GetOpPriority(enum OperationType op_type);
bool IsUnaryOperation(enum OperationType op_type);
void HeadLatex(FILE* file_latex);
StepBuffer* CreateStepBuffer(char var_diff, ErrorType* error);
void AddStepToBuffer(StepBuffer* buffer, TreeCalc* tree_state, ErrorType* error);
void WriteBufferToLatex(StepBuffer* buffer);
void DestroyStepBuffer(StepBuffer* buffer);


#endif
