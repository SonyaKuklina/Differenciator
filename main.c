#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "structs_calc.h"
#include "tree_calc_operations.h"
#include "tree_calc_private.h"
#include "tree_calc_graph.h"

#include "tree_calc_latex.h"
#include "tree_differenciator.h"
#include "differenciator_operator.h"
#include "read_from_file.h"
#include "tree_errors.h"
#include "tree_dsl.h"
#include "tree_errors.h"

#define DUMP_TREE(x) DumpTree((x), __FILE__, __LINE__)


int main(void) {

    ErrorType error = SUCCESS;
    struct Tree_calc* tree_read = CreateEmptyCalcTree(&error);
    int var_diff = 0;
    if (ReadTreeFromFile(tree_read, &var_diff) == INCORRECT) return INCORRECT;
   // DUMP_TREE(tree_read);

    StepBuffer* buffer = CreateStepBuffer(var_diff, &error);

    struct Tree_calc* new_tree = Differenciator(tree_read, var_diff, &error, buffer);
    WriteBufferToLatex(buffer);
    //if (WriteDerivative(new_tree, tree_read, var_diff) == INCORRECT) return INCORRECT;

    return CORRECT;


}
