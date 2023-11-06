#ifndef HexBoard_h
#define HexBoard_h
#include <iostream>
#include <vector>

using namespace std;

typedef struct node //HexBoard is basicly a graph that consists of Nodes. Vertices vector is the adjacents of that node.
{
    char status;
    int row;
    int col;
    vector<struct node*> vertices;
}Node;

class HexBoard
{
    public:
        HexBoard();
        void makeMove();
        void printBoard() const;
        bool gameOver();

    private:
        vector<vector<Node> >graph;
        char player;
        int size;
        int emptyCells;
        void Menu();
        void addEdge(const int row,const int col);
        void switchlayer(){player = (player == 'x') ? 'o' : 'x';}
        vector<Node*> DFS(Node& n);
};

#endif