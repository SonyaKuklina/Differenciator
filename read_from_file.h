#ifndef READ_FROM_FILE
#define READ_FROM_FILE

#include "enum_calc.h"
#include "structs_calc.h"

enum ProgrammReturn ReadTreeFromFile(TreeCalc* tree_read, int* var_diff);
Node* GetGeneralExpression(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetExpression(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetTerm(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetPower(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetBasis(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetNumber(char* buffer, TreeCalc* tree_read, int* pos, ErrorType* error);
Node* GetVariable(char* buffer, TreeCalc* tree_read, int* pos, char* varriable_or_keyword, ErrorType* error);
Node* GetMathFunction(char* buffer, TreeCalc* tree_read, int* pos,
                                enum OperationType curr_op_type, ErrorType* error);
char* ReadCurrentStr(char* buffer, int* pos);
double GetVariableValue(char var_name);
int DefineVariableIndex(TreeCalc* tree_read, char var_name);
enum OperationType GetKeywordType(char* current_str, ErrorType* error);
void SkipSpaces(char* buffer, int* pos);
int GetDiffVariable();
Node* SyntaxError(ErrorType* error);


#endif
