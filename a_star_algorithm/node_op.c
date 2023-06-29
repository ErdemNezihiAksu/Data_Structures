#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_op.h"
#include "Create_board.h"

Node* set_temp(Node* temp, Node *node, Node *r_closed, Node *r_open, int i, int j)
{
    Node* iter = r_closed;  
    while (iter != NULL && memcmp(iter->game.board, temp->game.board,sizeof(temp->game.board)) != 0)
        iter = iter->parent;
    if(iter == NULL)
    {
        temp->game.position.x = i;
        temp->game.position.y = j;
        temp->parent = node;
        temp->game.g_score = node->game.g_score + 1;
        get_h_count(&temp->game);
        temp->priority = temp->game.g_score + temp->game.h_score;
        temp->next = NULL;
        if(r_open == NULL)
        {
            r_open = temp;
        }
        else
        {
            iter = r_open;
            while(iter->next != NULL)
                iter = iter->next;
            iter->next = temp;
        }
    }
    return r_open;
}
Node* expand_status(Node *r_closed, Node *node, Point position)
{
    Node* r_open = NULL, *temp;
    int i = position.x;
    int j = position.y;

    if (i != 0) //yukarı
    {
        temp = (Node *)malloc(sizeof(Node));
        memcpy(temp->game.board, node->game.board, sizeof(node->game.board));
        temp->game.board[i][j] = temp->game.board[i - 1][j];
        temp->game.board[i - 1][j] = '_';
        r_open = set_temp(temp,node,r_closed,r_open,i-1,j);
        temp = NULL;
    }
    if (i != 2) // aşağı
    {
        temp = (Node *)malloc(sizeof(Node));
        memcpy(temp->game.board, node->game.board, sizeof(node->game.board));
        temp->game.board[i][j] = temp->game.board[i + 1][j];
        temp->game.board[i + 1][j] = '_';
        r_open = set_temp(temp,node,r_closed,r_open,i+1,j);
        temp = NULL;
    }
    if (j != 2) //sağa
    {
        temp = (Node *)malloc(sizeof(Node));
        memcpy(temp->game.board, node->game.board, sizeof(node->game.board));
        temp->game.board[i][j] = temp->game.board[i][j + 1];
        temp->game.board[i][j + 1] = '_';
        r_open = set_temp(temp,node,r_closed,r_open,i,j+1);
        temp = NULL;
    }
    if (j != 0) //sola
    {
        temp = (Node *)malloc(sizeof(Node));
        memcpy(temp->game.board, node->game.board, sizeof(node->game.board));
        temp->game.board[i][j] = temp->game.board[i][j - 1];
        temp->game.board[i][j - 1] = '_';
        r_open = set_temp(temp,node,r_closed,r_open,i,j-1);
        temp = NULL;
    }
    return r_open;
}

void print_moves(Node* node)
{
    Node *r_closed = NULL, *temp;
    FILE *f = fopen("moves.txt","a");
    
    if(f == NULL)
    {
        printf("Fıle could not be open!!\n");
        return;
    }
    while(node != NULL)
    {
        temp = (Node*)malloc(sizeof(Node));
        memcpy(temp->game.board,node->game.board,sizeof(node->game.board));
        temp->priority = node->priority;
        temp->game.h_score = node->game.h_score;
        temp->game.g_score = node->game.g_score;
        temp->next = r_closed;
        r_closed = temp;
        node = node->parent;
    }
    Node* iter = r_closed;
    while(iter != NULL)
    {
        if(iter->game.g_score != 0)
            fprintf(f,"\n\n");
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                fprintf(f,"%c ",iter->game.board[i][j]);
            }
            if(i != 2)
                fprintf(f,"\n");
        }
        fprintf(f,"    g_score: %d, h_score: %d, f_score: %d", iter->game.g_score,iter->game.h_score,iter->priority);
        iter = iter->next;
    }
    fclose(f);
    free(r_closed);
}

void print_nodes(Node *r_open)
{
    Node *iter;
    FILE *f = fopen("tree.txt","a");

    if(r_open->game.g_score != 1)
        fprintf(f,"\n\n");
    for (int i = 0; i < 3; i++)
    {
        iter = r_open;
        while (iter != NULL)
        {
            for(int j = 0; j < 3; j++)
            {
                fprintf(f,"%c ",iter->game.board[i][j]);
            }
            fprintf(f,"    ");
            iter = iter->next;
        }
        fprintf(f,"\n");
    }
    
    for(int i = 0; i < 3; i++)
    {
        iter = r_open;
        while (iter != NULL)
        {
            if(i == 0)
            fprintf(f,"g_score: %d ",iter->game.g_score);
            else if(i == 1)
            fprintf(f,"h_score: %d ",iter->game.h_score);
            else
            fprintf(f,"f_score: %d ",iter->priority);
            fprintf(f,"    ");
            iter = iter->next;
        }
        fprintf(f,"\n");
    }
    fclose(f);
}