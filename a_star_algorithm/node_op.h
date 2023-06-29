#ifndef node_op
#define node_op
#include "Create_board.h"
Node* expand_status(Node *r_closed, Node *node, Point position);
void print_moves(Node* node);
void print_nodes(Node *r_open);
#endif