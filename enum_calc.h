#ifndef ENUM_DIFF
#define ENUM_DIFF

enum OperationType {

    ADD, MULL, DIV, SUB,
    POW, SIN,  COS, TAN,
    ATAN, ASIN, ACOS,
    SINH, COSH, TANH,
    LN, LOG, EXP,
    UNDETIFIED_TYPE,


};

enum TypeNode {

    OPERATION = 0,
    VARIABLE,
    CONST_VALUE,

};

enum ProgrammReturn {

    CORRECT = 0,
    INCORRECT,

};

enum Priority {

    NONE_PRIORITY = 0, //для констант и переменных
    ONE_PRIORITY,
    TWO_PRIORITY,
    THREE_PRIORITY,
    FOUR_PRIORITY,

};

#endif
