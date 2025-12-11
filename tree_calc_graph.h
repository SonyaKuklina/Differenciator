#ifndef TREE_CALC_GRAPH
#define TREE_CALC_GRAPH

#include "structs_calc.h"

void DumpTree(struct Tree_calc* tree_calculations, const char* file, const int line);
void GraphicalDumpTree(TreeCalc* tree_calculations, const char* file, const int line);
void CreateDotFile(struct Tree_calc* tree_calculations);
void CreateNodesGraph(struct Tree_calc* tree_calculations, struct Node_t* node, FILE* file_dot);
void CreateConnectionsNodesGraph(struct Node_t* node, FILE* file_dot);
bool CreateNamePng(char* file_name_dot, char* png_file);
void CreateAndOpenHtmlFile(char* png_file, struct Tree_t* tree,
                           const char* file, const int line);

#endif
