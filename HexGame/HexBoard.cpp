#include "HexBoard.h"
#include <string>
#include <stack>

HexBoard::HexBoard() //Initializes the graph structure and add adjecents to each Node
{
    Menu();
    Node a;
    player = 'x';
    emptyCells = size * size;
    for (int rows = 0; rows < size; rows++)
    {
        vector<Node> row;
        for (int col = 0; col < size; col++)
        {
            a.status = '.';
            a.row = rows;
            a.col = col;
            row.push_back(a);
        }
        graph.push_back(row);
    }
    for (int rows = 0; rows < size; rows++)
    {
        for (int col = 0; col < size; col++)
        {
            addEdge(rows, col);
        }
    }
}

void HexBoard::addEdge(int row, int col)
{
    if (row > 0)
        graph[row][col].vertices.push_back(&graph[row - 1][col]);
    if (row < size - 1)
        graph[row][col].vertices.push_back(&graph[row + 1][col]);
    if (col > 0)
        graph[row][col].vertices.push_back(&graph[row][col - 1]);
    if (col < size - 1)
        graph[row][col].vertices.push_back(&graph[row][col + 1]);
    if (row < size - 1 && col > 0)
        graph[row][col].vertices.push_back(&graph[row + 1][col - 1]);
    if (row > 0 && col < size - 1)
        graph[row][col].vertices.push_back(&graph[row - 1][col + 1]);
}

void HexBoard::printBoard() const
{
    for (int i = 0; i < graph[0].size(); i++)
        cout << " " << static_cast<char>(i + 97);
    cout << endl;
    int counter = 1;
    for (vector<Node> i : graph)
    {
        string spaces(counter - counter / 10, ' '); //This line manages the space count before printing each row.
        cout << counter << spaces;
        for (Node j : i)
        {
            cout << j.status << " ";
        }
        cout << endl;
        counter++;
    }
}

void HexBoard::Menu()
{
    int choice;
    bool flag = false;

    cout << "\nWelcome to Hex Game !\n\n";
    while (!flag)
    {
        cout << "Please choose the size of the board(6,7,8,9,10,11,12): ";
        if (!(cin >> choice))
        {
            cout << "INVALID INPUT !! \n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice > 5 && choice < 13)
        {
            flag = true;
            size = choice;
        }
        else
            cout << "INVALID SIZE !!\n";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Cleans the input buffer
}

void HexBoard::makeMove()
{
    string input;
    int i, row, col;
    bool flag_inner = false, flag_outer = false;
    while (!flag_outer)
    {
        while (!flag_inner)
        {
            cout << "\nMake a move (A 1): ";
            getline(cin, input);
            if (input.length() == 3 || input.length() == 4) //Input can maximum have the lenght of 4 (e.g "L 12")
            {
                if ((65 <= input[0] && 65 + size > input[0]) && (input[1] == ' '))
                {
                    try
                    {
                        i = stoi(&input[2]); //converts the last partf of the string to int
                    }
                    catch (const invalid_argument &e) //if it isn't an integer, make it -1
                    {
                        i = -1;
                    }
                    if (i > 0 && i <= size) //checks the size
                        flag_inner = true;
                }
            }
            if (flag_inner == false)
            {
                cout << "INVALID INPUT !!";
                input.clear();
            }
        }
        row = i - 1;
        col = static_cast<int>(input[0]) - 65;
        if (graph[row][col].status == '.') //checks if the cell is empty
            flag_outer = true;
        else
        {
            input.clear();
            flag_inner = false;
            cout << "THE CELL IS NOT EMPTY !!";
        }
    }

    graph[row][col].status = player;
    emptyCells--;
}

bool HexBoard::gameOver()
{
    vector<Node *> winningPath;
    if (player == 'x')
    {
        for (int row = 0; row < size; row++) //if player is x. check the first col of each row to see if there is any begging to the winning path.
        {
            if (graph[row][0].status == player)
            {
                winningPath = DFS(graph[row][0]);
                if (!winningPath.empty())
                    break;
            }
        }
    }
    else
    {
        for (int col = 0; col < size; col++)
        {
            if (graph[0][col].status == player)
            {
                winningPath = DFS(graph[0][col]);
                if (!winningPath.empty())
                    break;
            }
        }
    }
    if (!winningPath.empty())
    {
        for (Node *nod : winningPath) //capitalize the winning path
            nod->status -= 32;
        printBoard();
        cout << "\nPLAYER " << static_cast<char>(player - 32) << " WIN !!!!" << endl;
        return true;
    }
    if (emptyCells == 0)
    {
        cout << "\nGAME OVER !!!\n";
        return true;
    }
    switchlayer();
    return false;
}

void reDirect_currentPath(vector<Node *> &currentPath, const vector<Node *> &visited, char player)
{
    Node *nod = currentPath.back();
    bool isVisited;

    for (Node *vertice : nod->vertices) //The idea is that if the node doesn't have any non-empty adjacent or all the non-empty adjacents are visited, the node should leave the path.
    {
        isVisited = false;
        if (vertice->status == player)
        {
            for (Node *visit : visited)
            {
                if (vertice == visit)
                {
                    isVisited = true;
                    break;
                }
            }
            if (isVisited == false)
                return;
        }
    }
    currentPath.pop_back();
    if (currentPath.empty())
        return;
    reDirect_currentPath(currentPath, visited, player);
}

vector<Node *> HexBoard::DFS(Node &n) //DEPTH FIRST SEARCH algrotihm to find the winning path
{
    vector<Node *> visited;
    stack<Node *> s;
    vector<Node *> currentPath; //while the algortihm explores the paths. The current path it is on is always kept.Because at the end this will be the winning path.
    Node *current; // current node in the path
    bool isVisited, isPushed;

    s.push(&n);

    while (!s.empty())
    {
        isPushed = false;
        current = s.top();
        s.pop();
        visited.push_back(current);
        currentPath.push_back(current);

        if ((player == 'x' && current->col == size - 1) || (player == 'o' && current->row == size - 1))//if the current node is on the other side of the hextable, we have the winning path.
            return currentPath;

        for (Node *nod_o : current->vertices)//explore the adjacents of the current node
        {
            isVisited = false;
            if (nod_o->status == player)
            {
                for (Node *nod_i : visited)
                {
                    if (nod_i == nod_o)
                    {
                        isVisited = true;
                        break;
                    }
                }
                if (isVisited == false)
                {
                    s.push(nod_o); //if an adjacent is not visited, it is pushed into the stack
                    isPushed = true;
                }
            }
        }
        if (!isPushed) //if nothing has been pushed into the stack, that means we are going to change the path so we need to delete the elements that are not gonna be in our new path.
        {
            currentPath.pop_back(); //The last elements is obviously gonna be deleted so we do this manually.
            if (!currentPath.empty())
                reDirect_currentPath(currentPath, visited, player); //recursive function handles the rest.
        }
    }
    return {};
}