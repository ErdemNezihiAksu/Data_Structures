#ifndef Create_board
#define Create_board
#include <stdlib.h>
typedef struct
{
    int x;
    int y;
}Point;
typedef struct
{
    char board[3][3];
    int g_score;
    int h_score;
    Point position;
}Puzzle;
typedef struct n
{
    Puzzle game;
    int priority;
    struct n* parent;
    struct n*  next;
}Node;
char Menu();
int get_h_count(Puzzle *game);
void Create_Board(Node **root);
void print_board(Node *node);
#endif