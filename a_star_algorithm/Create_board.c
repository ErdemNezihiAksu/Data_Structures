#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Create_board.h"

char Menu()
{
    char c;
    printf("1- Auto finish\n2- Exit\n\n>");
    scanf("%c",&c);
    return c;
}

void init_board(Node **root)
{
    *root = (Node*)malloc(sizeof(Node));
}

int get_h_count(Puzzle *game)
{
    game->h_score = 0;
    int manh_dist = 0;
    int x,y;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(game->board[i][j] == '_' || game->board[i][j] - '0' == (i*3) + j +1) 
                continue;
            x = (game->board[i][j] - '0' - 1)/3;
            y = (game->board[i][j] - '0' - 1)%3;
            manh_dist = abs(i-x) + abs(j-y);
            game->h_score += manh_dist;
        }
    }
    return game->h_score;
}

int get_inv_board(char board[][3])
{
    int inv_count = 0;
    int n;

    for(int i = 0 ; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(board[i][j] == '_')
                continue;
            n = board[i][j] - '0';
            for(int a = i; a < 3; a++)
            {
                for(int b = 0; b < 3; b++)
                {
                    if(board[a][b] == '_' || (a == i && b <= j))
                        continue;
                    if(board[a][b] - '0' < n)
                        inv_count++;
                }   
            }
        }
    }
    return inv_count;
}
void Create_Board(Node **root)
{
    char arr[] = {'1','2','3','_','4','6','7','5','8'};

label:
    for(int i = 8; i>0 ; i--)
    {
        int j = rand() % (i+1);
        char temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    init_board(root);
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            (*root)->game.board[i][j] = arr[i*3 + j];
            if(arr[i*3 + j] == '_')
            {
                (*root)->game.position.y = j;
                (*root)->game.position.x = i;
            }
        }
    }
    if(get_inv_board((*root)->game.board) % 2 != 0)
        goto label;
    if(get_h_count(&(*root)->game) == 0)
        goto label;
    (*root)->game.g_score = 0;
    (*root)->priority = (*root)->game.h_score;
    (*root)->parent = NULL;
    (*root)->next = NULL;
}

void print_board(Node *node)
{
    FILE *f = fopen("initial_state.txt","a");
    if(f == NULL)
    {
        printf("Fıle could not be open!!\n");
        return;
    }
    if(node->game.g_score != 0)
        fprintf(f,"\n\n");
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            fprintf(f,"%c ",node->game.board[i][j]);
        }
        if(i != 2)
        {
            fprintf(f,"\n");
        }
    }
    fprintf(f,"    g_score: %d, h_score: %d, f_score: %d", node->game.g_score,node->game.h_score,node->priority);
    fprintf(f,"\n\n x = %d, y = %d",node->game.position.x,node->game.position.y);
    fclose(f);
}
