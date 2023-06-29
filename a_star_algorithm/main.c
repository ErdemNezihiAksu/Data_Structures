#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Create_board.h"
#include "node_op.h"

Node* finish(Node* node)
{
    Node *r_open, *iter, *tail;
    
    r_open = expand_status(node,node,node->game.position);
    print_nodes(r_open);
    while(1)
    { 
        node = r_open;
        iter = r_open->next;
        while(iter != NULL)
        {
            if(iter->priority < node->priority)
                node = iter;
            iter = iter->next;
        }
        if(node->game.h_score == 0)
            break;
        r_open = expand_status(node,node,node->game.position);
        tail = r_open;
        while(tail->next != NULL)
            tail = tail->next;
        iter = node->next;
        while(iter != NULL)
        {
            if(iter->priority == node->priority)
            {
                tail->next = expand_status(iter,iter,iter->game.position);
                while(tail->next != NULL)
                    tail = tail->next;
            }
            iter = iter->next;
        }
        print_nodes(r_open);
    }
    return node;
}

int main()
{
    char c;
    FILE *f;
    Node* tree, *node, *final_state;

    srand(time(NULL));
    c = Menu();
    switch (c)
    {
    case '1':
        f = fopen("tree.txt", "w");
        fclose(f);
        f = fopen("initial_state.txt","w");
        fclose(f);
        f = fopen("moves.txt", "w");
        fclose(f);
        Create_Board(&tree);
        print_board(tree);
        node = tree;
        final_state = finish(node);
        print_moves(final_state);
        break;
    
    default:
        break;
    }
    free(tree);
}