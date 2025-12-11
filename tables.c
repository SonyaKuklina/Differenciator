#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "tables.h"
#include "tree_diff_optimize.h"
#include "differenciator_operator.h"
#include "tree_dsl.h"


struct UnaryOpTable* CreateUnaryOpTable(ErrorType* error) {

    struct UnaryOpTable* unary_op_table = (struct UnaryOpTable*)calloc(COUNT_UNARY_OPERATIONS, sizeof(struct UnaryOpTable));
    ASSERT(unary_op_table)

    unary_op_table[0]  = (struct UnaryOpTable){ SIN,  sin  };
    unary_op_table[1]  = (struct UnaryOpTable){ COS,  cos  };
    unary_op_table[2]  = (struct UnaryOpTable){ TAN,  tan  };
    unary_op_table[3]  = (struct UnaryOpTable){ ATAN, atan };
    unary_op_table[4]  = (struct UnaryOpTable){ ASIN, asin };
    unary_op_table[5]  = (struct UnaryOpTable){ ACOS, acos };
    unary_op_table[6]  = (struct UnaryOpTable){ SINH, sinh };
    unary_op_table[7]  = (struct UnaryOpTable){ COSH, cosh };
    unary_op_table[8]  = (struct UnaryOpTable){ TANH, tanh };
    unary_op_table[9]  = (struct UnaryOpTable){ LN,   log  };
    unary_op_table[10] = (struct UnaryOpTable){ EXP,  exp  };

    return unary_op_table;

}

struct BinaryOpTable* CreateBinaryOpTable(ErrorType* error) {

    struct BinaryOpTable* binary_op_table = (struct BinaryOpTable*)calloc(COUNT_BINARY_OPERATIONS, sizeof(struct BinaryOpTable));
    ASSERT(binary_op_table)

    binary_op_table[0]  = (struct BinaryOpTable){ ADD,  OptimizeAddOperation  };
    binary_op_table[1]  = (struct BinaryOpTable){ MULL, OptimizeMullOperation };
    binary_op_table[2]  = (struct BinaryOpTable){ SUB,  OptimizeSubOperation  };
    binary_op_table[3]  = (struct BinaryOpTable){ DIV,  OptimizeDivOperation  };
    binary_op_table[4]  = (struct BinaryOpTable){ POW,  OptimizePowOperation  };
    binary_op_table[5]  = (struct BinaryOpTable){ LOG,  OptimizeLogOperation  };

    return binary_op_table;

}

struct OpTable* CreateOpTable() {

    struct OpTable* op_table = (struct OpTable*)calloc(COUNT_OPERATIONS, sizeof(struct OpTable));
    ASSERT(op_table);

    op_table[0]  = (struct OpTable){ ADD,  " + "    };
    op_table[1]  = (struct OpTable){ MULL, "\\cdot "};
    op_table[2]  = (struct OpTable){ SUB,  " - "    };
    op_table[3]  = (struct OpTable){ DIV,  "\\frac" };
    op_table[4]  = (struct OpTable){ POW,  "^"      };
    op_table[5]  = (struct OpTable){ LOG,  "\\log"  };
    op_table[6]  = (struct OpTable){ SIN,  "\\sin"  };
    op_table[7]  = (struct OpTable){ COS,  "\\cos"  };
    op_table[8]  = (struct OpTable){ TAN,  "\\tan"  };
    op_table[9]  = (struct OpTable){ ATAN, "\\atan" };
    op_table[10] = (struct OpTable){ ASIN, "\\asin" };
    op_table[11] = (struct OpTable){ ACOS, "\\acos" };
    op_table[12] = (struct OpTable){ SINH, "\\sinh" };
    op_table[13] = (struct OpTable){ COSH, "\\cosh" };
    op_table[14] = (struct OpTable){ TANH, "\\tanh" };
    op_table[15] = (struct OpTable){ LN,   "\\ln"   };
    op_table[16] = (struct OpTable){ EXP,  "\\exp"  };

    return op_table;

}

struct KeywordsTable* CreateKeywordsTable(ErrorType* error) {

    ASSERT(error)
    struct KeywordsTable* keywords_op_table = (struct KeywordsTable*)calloc(COUNT_OPERATIONS, sizeof(struct KeywordsTable));
    if (keywords_op_table == NULL) { *error |= NULL_POINTER_TO_TABLE; return NULL; }

    keywords_op_table[0]  = (struct KeywordsTable){ SIN,  "sin"  };
    keywords_op_table[1]  = (struct KeywordsTable){ COS,  "cos"  };
    keywords_op_table[2]  = (struct KeywordsTable){ TAN,  "tan"  };
    keywords_op_table[3]  = (struct KeywordsTable){ ATAN, "atan" };
    keywords_op_table[4]  = (struct KeywordsTable){ ASIN, "asin" };
    keywords_op_table[5]  = (struct KeywordsTable){ ACOS, "acos" };
    keywords_op_table[6]  = (struct KeywordsTable){ SINH, "sinh" };
    keywords_op_table[7]  = (struct KeywordsTable){ COSH, "cosh" };
    keywords_op_table[8]  = (struct KeywordsTable){ TANH, "tanh" };
    keywords_op_table[9]  = (struct KeywordsTable){ LN,   "ln"   };
    keywords_op_table[10] = (struct KeywordsTable){ EXP,  "exp"  };
   // keywords_op_table[11] = { LOG,  "log"  };//

    return keywords_op_table;

}

struct OperationDiffTable* CreateOperationDiffTable(ErrorType* error) {

    struct OperationDiffTable* diff_op_table = (struct OperationDiffTable*)calloc(COUNT_OPERATIONS, sizeof(struct OperationDiffTable));
    ASSERT(diff_op_table);

    diff_op_table[0]  = (struct OperationDiffTable){ ADD,  DifferenciationAddOperator  };
    diff_op_table[1]  = (struct OperationDiffTable){ MULL, DifferenciationMullOperator };
    diff_op_table[2]  = (struct OperationDiffTable){ DIV,  DifferenciationDivOperator  };
    diff_op_table[3]  = (struct OperationDiffTable){ SUB,  DifferenciationSubOperator  };
    diff_op_table[4]  = (struct OperationDiffTable){ POW,  DifferenciationPowOperator  };
    diff_op_table[5]  = (struct OperationDiffTable){ SIN,  DifferenciationSinOperator  };
    diff_op_table[6]  = (struct OperationDiffTable){ COS,  DifferenciationCosOperator  };
    diff_op_table[7]  = (struct OperationDiffTable){ TAN,  DifferenciationTanOperator  };
    diff_op_table[8]  = (struct OperationDiffTable){ ATAN, DifferenciationAtanOperator };
    diff_op_table[9]  = (struct OperationDiffTable){ ASIN, DifferenciationAsinOperator };
    diff_op_table[10] = (struct OperationDiffTable){ ACOS, DifferenciationAcosOperator };
    diff_op_table[11] = (struct OperationDiffTable){ SINH, DifferenciationSinhOperator };
    diff_op_table[12] = (struct OperationDiffTable){ COSH, DifferenciationCoshOperator };
    diff_op_table[13] = (struct OperationDiffTable){ TANH, DifferenciationTanhOperator };
    diff_op_table[14] = (struct OperationDiffTable){ LN,   DifferenciationLnOperator   };
    diff_op_table[15] = (struct OperationDiffTable){ LOG,  DifferenciationLogOperator  };
    diff_op_table[16] = (struct OperationDiffTable){ EXP,  DifferenciationExpOperator  };

    return diff_op_table;

}

struct PriorityOperation* CreatePriorityOpTable() {

    struct PriorityOperation* priority = (struct PriorityOperation*)calloc(COUNT_OPERATIONS, sizeof(struct PriorityOperation));
    ASSERT(priority);

    priority[0] =  (struct PriorityOperation){ POW,  ONE_PRIORITY };
    priority[1] =  (struct PriorityOperation){ SIN,  TWO_PRIORITY };
    priority[2] =  (struct PriorityOperation){ COS,  TWO_PRIORITY };
    priority[3] =  (struct PriorityOperation){ TAN,  TWO_PRIORITY };
    priority[4] =  (struct PriorityOperation){ ATAN, TWO_PRIORITY };
    priority[5] =  (struct PriorityOperation){ ASIN, TWO_PRIORITY };
    priority[6] =  (struct PriorityOperation){ ACOS, TWO_PRIORITY };
    priority[7] =  (struct PriorityOperation){ SINH, TWO_PRIORITY };
    priority[8] =  (struct PriorityOperation){ COSH, TWO_PRIORITY };
    priority[9] =  (struct PriorityOperation){ TANH, TWO_PRIORITY };
    priority[10] = (struct PriorityOperation){ LN,   TWO_PRIORITY };
    priority[11] = (struct PriorityOperation){ LOG,  TWO_PRIORITY };
    priority[12] = (struct PriorityOperation){ EXP,  TWO_PRIORITY };
    priority[13] = (struct PriorityOperation){ MULL, THREE_PRIORITY };
    priority[14] = (struct PriorityOperation){ DIV,  THREE_PRIORITY };
    priority[15] = (struct PriorityOperation){ ADD,  FOUR_PRIORITY };
    priority[16] = (struct PriorityOperation){ SUB,  FOUR_PRIORITY };

    return priority;

}







