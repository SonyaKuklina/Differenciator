#ifndef TABLES
#define TABLES

#include "structs_calc.h"
#include "enum_calc.h"

#define COUNT_UNARY_OPERATIONS 11
#define COUNT_BINARY_OPERATIONS 6
#define COUNT_OPERATIONS 17
#include "tree_errors.h"

struct UnaryOpTable* CreateUnaryOpTable(ErrorType* error);
struct BinaryOpTable* CreateBinaryOpTable(ErrorType* error);
struct OpTable* CreateOpTable();
struct KeywordsTable* CreateKeywordsTable(ErrorType* error);
struct OperationDiffTable* CreateOperationDiffTable(ErrorType* error);
struct PriorityOperation* CreatePriorityOpTable();

#endif
