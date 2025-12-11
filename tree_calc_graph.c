#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "tree_calc_graph.h"
#include "enum_string.h"
#define SIZE_NAME_PNG 100

void DumpTree(struct Tree_calc* tree_calculations, const char* file, const int line) {

    assert(file              != NULL);
    assert(tree_calculations != NULL);

    GraphicalDumpTree(tree_calculations, file, line);

}

void GraphicalDumpTree(struct Tree_calc* tree_calculations, const char* file, const int line) {

    assert(file              != NULL);
    assert(tree_calculations != NULL);

    CreateDotFile(tree_calculations);

    char* file_name_dot = (char*)"debug.dot";
    char file_name_png[SIZE_NAME_PNG];
    char* png_file = file_name_png;
    if (CreateNamePng(file_name_dot, png_file)) {
        CreateAndOpenHtmlFile(png_file, tree_calculations -> tree, file, line);
    }

}

void CreateDotFile(TreeCalc* tree_calculations) {

    assert(tree_calculations != NULL);

    FILE* file_dot = fopen("debug.dot", "w");
    fprintf(file_dot, "digraph G {\n");
    fprintf(file_dot, "    bgcolor = \"white\";\n");
    fprintf(file_dot, "    rankdir = TB;\n");
    fprintf(file_dot, "    nodesep = 0.5;\n");
    fprintf(file_dot, "    ranksep = 2.0;\n");
    fprintf(file_dot, "    splines = ortho;\n");
    fprintf(file_dot, "    node [style = \"filled\", shape = Mrecord,\n");
    fprintf(file_dot, "         fillcolor = lightsalmon, color = black,\n");
    fprintf(file_dot, "         width = 0.2, height = 0.4];\n");
    CreateNodesGraph(tree_calculations, (tree_calculations -> tree) -> root, file_dot);
    CreateConnectionsNodesGraph((tree_calculations -> tree) -> root, file_dot);
    fprintf(file_dot, "}");
    fclose(file_dot);

}

void CreateNodesGraph(struct Tree_calc* tree_calculations, struct Node_t* node, FILE* file_dot) {

    assert(file_dot          != NULL);
    assert(tree_calculations != NULL);

    if (node == NULL) return;

    enum TypeNode type_node = node -> type_node;

    switch (type_node) {

    case OPERATION:   {fprintf(file_dot, "node%d  [label = \"{<par> parent: %p | <type> type: %s | <el> node_element: %s | {<left> left_branch: %p | <right> right_branch: %p}}\"];\n",
                      node -> num_node, node -> parent, GetTypeNodeName(node), GetOperationName(node), node -> left_branch, node -> right_branch);
                      break;}

    case VARIABLE:    {const char name_var = tree_calculations -> variable_array[node -> node_element.index_var].variable_name;
                      fprintf(file_dot, "node%d  [label = \"{<par> parent: %p | <type> type: %s | <el> node_element: %c | {<left> left_branch: %p | <right> right_branch: %p}}\"];\n",
                      node -> num_node, node -> parent, GetTypeNodeName(node), name_var, node -> left_branch, node -> right_branch);
                      break;}

    case CONST_VALUE: {fprintf(file_dot, "node%d  [label = \"{<par> parent: %p | <type> type: %s | <el> node_element: %lg | {<left> left_branch: %p | <right> right_branch: %p}}\"];\n",
                      node -> num_node, node -> parent, GetTypeNodeName(node), node -> node_element.const_value, node -> left_branch, node -> right_branch);
                      break;}

    default:          printf("DEBUG: node=%p, type_node=%d\n", node, type_node);
                      assert(0);
                      break;

    }


    if (node -> left_branch != NULL)  CreateNodesGraph(tree_calculations, node -> left_branch, file_dot);
    if (node -> right_branch != NULL) CreateNodesGraph(tree_calculations, node -> right_branch, file_dot);

}

void CreateConnectionsNodesGraph(struct Node_t* node, FILE* file_dot) {

    assert(file_dot != NULL);
    if (node == NULL) return;

    if (node -> left_branch != NULL) {

        if (((node -> left_branch) -> parent) == node) fprintf(file_dot, "node%d:left -> node%d:par [color = coral3];\n",
                                                       node -> num_node, (node -> left_branch) -> num_node);
        else                                           fprintf(file_dot, "node%d:left -> node%d:par [color = crimson];\n",
                                                       node -> num_node, (node -> left_branch) -> num_node);

        CreateConnectionsNodesGraph(node -> left_branch, file_dot);

    }

    if (node -> right_branch != NULL) {

        if (((node -> right_branch) -> parent) == node) fprintf(file_dot, "node%d:right -> node%d:par [color = coral3];\n",
                                                        node -> num_node, (node -> right_branch) -> num_node);

        else                                            fprintf(file_dot, "node%d:right -> node%d:par [color = crimson];\n",
                                                        node -> num_node, (node -> right_branch) -> num_node);

        CreateConnectionsNodesGraph(node -> right_branch, file_dot);

    }

}

bool CreateNamePng(char* file_name_dot, char* png_file) {

    assert(file_name_dot != NULL);
    assert(png_file      != NULL);

    char* file_name = strchr(file_name_dot, '.');

    if (file_name == NULL) return false;

    long int len_file_name = file_name - file_name_dot;
    png_file = strncpy(png_file, file_name_dot, len_file_name);
    *(png_file + len_file_name) = '\0';
    png_file = strcat(png_file, ".png");

    char command[100];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", file_name_dot, png_file);
    if (system(command) == 0) return true;
    return false;

}

void CreateAndOpenHtmlFile(char* png_file, struct Tree_t* tree,
                           const char* file, const int line) {

    assert(png_file != NULL);
    assert(tree     != NULL);
    assert(file     != NULL);

    FILE* file_html = fopen("debug.html", "w");

    assert(file_html != NULL);

    fprintf(file_html, "<!DOCTYPE html>\n");
    fprintf(file_html, "<body>\n");
    fprintf(file_html, "<h3>TREE_CALCULATIONS</h3>\n");

    fprintf(file_html, "<p>DumpTree called from: %s %d</p>\n", file, line);
    fprintf(file_html, "<p> Tree has a %d elements </p>\n", tree -> node_size);

    fprintf(file_html, "<img src = \"%s\">\n", png_file);

    fprintf(file_html, "</body>\n");
    fprintf(file_html, "</html>\n");

    fclose(file_html);

    system("explorer debug.html");

}
