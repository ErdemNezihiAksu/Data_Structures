#ifndef SN_graph_h
#define SN_graph_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[20];
    char nickname[10];
}Person;
typedef struct mynode
{
    Person buddy;
    struct mynode *next;
}Node;
typedef struct mylist
{
    Person person;
    int friend_count;
    Node *head;
    struct mylist *next;
}List;
typedef struct
{
    int user_count;
    List *root;
}graph;
graph *Create_Graph();
void Write_bin_file(graph *g, char *filename);
void Read_bin_file(graph *g, char *filename);
void Add_User(graph *g);
void Delete_user(graph *g);
void Info_User(graph *g);
void Destroy_Friends(List *user);
int Delete_Node(List *user, const char* nickname);
void Add_friend(graph *g);
void print_graph(graph *g);
void Destory_Graph(graph *g);
#endif