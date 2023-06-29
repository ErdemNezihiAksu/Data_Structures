#include "SN_graph.h"

graph *Create_Graph()
{
    graph *g = malloc(sizeof(graph));
    if (g == NULL)
    {
        printf("Malloc failed!!\n");
        return NULL;
    }
    g->user_count = 0;
    g->root = NULL;
    return g;
}

void Write_bin_file(graph *g, char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL)
    {
        printf("Failed to open file");
        return;
    }
    List *iterList = g->root;
    Node *iterNode;
    fwrite(&g->user_count, sizeof(int), 1, f);
    while (iterList != NULL)
    {
        fwrite(&iterList->person, sizeof(Person), 1, f);
        fwrite(&iterList->friend_count, sizeof(int), 1, f);
        iterNode = iterList->head;
        while (iterNode != NULL)
        {
            fwrite(&iterNode->buddy, sizeof(Person), 1, f);
            iterNode = iterNode->next;
        }
        iterList = iterList->next;
    }
    fclose(f);
}

void Read_bin_file(graph *g, char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Failed to open file");
        return;
    }
    List *newList, *iterList;
    Node *newNode, *iterNode;
    char c;
    fread(&g->user_count, sizeof(int), 1, f);
    for (int j = 0; j < g->user_count; j++)
    {
        newList = (List *)malloc(sizeof(List));
        fread(&newList->person, sizeof(Person), 1, f);
        fread(&newList->friend_count, sizeof(int), 1, f);
        newList->head = NULL;
        for (int i = 0; i < newList->friend_count; i++)
        {
            newNode = (Node *)malloc(sizeof(Node));
            fread(&newNode->buddy, sizeof(Person), 1, f);
            if (newList->head == NULL)
            {
                newList->head = newNode;
                iterNode = newNode;
            }
            else
            {
                iterNode->next = newNode;
                iterNode = iterNode->next;
            }
        }
        if (g->root == NULL)
        {
            g->root = newList;
            iterList = newList;
        }
        else
        {
            iterList->next = newList;
            iterList = iterList->next;
        }
    }
    iterList->next = NULL;
    fclose(f);
}

void print_graph(graph *g)
{
    if (g->root == NULL)
    {
        printf("No user\n");
        return;
    }
    List *iter = g->root;
    Node *iterNode;
    while (iter != NULL)
    {
        char string[34];
        sprintf(string, "%s (%s)", iter->person.name, iter->person.nickname);
        printf("%-34s|", string);
        iterNode = iter->head;
        while (iterNode != NULL)
        {
            if (iterNode == iter->head)
                printf("%s(%s)", iterNode->buddy.name, iterNode->buddy.nickname);
            else
                printf(", %s(%s)", iterNode->buddy.name, iterNode->buddy.nickname);
            iterNode = iterNode->next;
        }
        iter = iter->next;
        printf("\n");
    }
}

void Add_User(graph *g)
{
    List *newUser = malloc(sizeof(List));
    List *iter = g->root;

    Person newPerson;
    printf("Enter the new user's  name: ");
    scanf("%19[^\n]", newPerson.name);
    while (getchar() != '\n')
        ;
    printf("Enter %s's nickname: ", newPerson.name);
label:
    scanf("%9[^\n]", newPerson.nickname);
    while (getchar() != '\n')
        ;
    if (g->root != NULL)
    {
        while (iter != NULL)
        {
            if (strcmp(newPerson.nickname, iter->person.nickname) == 0)
            {
                printf("This nick name is in use! Choose another one: ");
                iter = g->root;
                goto label;
            }
            iter = iter->next;
        }
    }
    newUser->person = newPerson;
    newUser->friend_count = 0;
    newUser->head = NULL;
    newUser->next = NULL;

    if (g->root == NULL)
    {
        g->user_count = 1;
        g->root = newUser;
    }
    else
    {
        iter = g->root;
        while (iter->next != NULL)
            iter = iter->next;
        iter->next = newUser;
        g->user_count++;
    }
}

void Add_friend(graph *g)
{
    if (g->user_count == 0)
    {
        printf("\n No user on the network\n");
        return;
    }
    int flag1 = 0, flag2 = 0;
    Person from, to;
    List *iterList_from = g->root, *iterList_to = g->root;
    Node *newNode, *iterNode;

label:
    printf("Enter the user's nickname: ");
    scanf("%9[^\n]", from.nickname);
    while (getchar() != '\n')
        ;
    printf("Enter the friend's nickname: ");
    scanf("%9[^\n]", to.nickname);
    while (getchar() != '\n')
        ;
    if (strcmp(from.nickname, to.nickname) == 0)
    {
        printf("Person can not be friend with him/herself\n");
        goto label;
    }
    while (iterList_from != NULL && iterList_to != NULL)
    {
        if (flag1 == 0 && strcmp(from.nickname, iterList_from->person.nickname) == 0)
            flag1 = 1;
        else if (flag2 == 0 && strcmp(to.nickname, iterList_to->person.nickname) == 0)
            flag2 = 1;
        if (flag1 == 0)
            iterList_from = iterList_from->next;
        if (flag2 == 0)
            iterList_to = iterList_to->next;
        if (flag1 == 1 && flag2 == 1)
            break;
    }
    if (flag1 == 0 || flag2 == 0)
    {
        printf("One or both of these nicknames do not exist! Choose another one\n");
        iterList_from = g->root;
        iterList_to = g->root;
        flag1 = 0;
        flag2 = 0;
        goto label;
    }
    newNode = (Node *)malloc(sizeof(Node));
    newNode->buddy = iterList_to->person;
    newNode->next = NULL;
    if (iterList_from->head == NULL)
    {
        iterList_from->head = newNode;
    }
    else
    {
        iterNode = iterList_from->head;
        while(iterNode != NULL)
        {
            if(strcmp(iterNode->buddy.nickname,iterList_to->person.nickname) == 0)
            {
                printf("%s and %s are already friends\n",iterList_from->person.nickname,iterList_to->person.nickname);
                return;
            }
            if(iterNode->next == NULL)
                break;
            iterNode = iterNode->next;
        }
        iterNode->next = newNode;
    }
    iterList_from->friend_count++;
}

void Delete_user(graph *g)
{
    if (g->root == NULL)
    {
        printf("No user\n");
        return;
    }
    List *iterL, *temp;
    char nickname[10];
    int flag = 0;

    printf("Enter the nickname of the user: ");
    scanf("%9[^\n]", nickname);
    while (getchar() != '\n')
        ;
    if (strcmp(g->root->person.nickname, nickname) == 0)
    {
        if (g->root->friend_count != 0)
            Destroy_Friends(g->root);
        temp = g->root;
        g->root = g->root->next;
        free(temp);
        flag = 1;
        if (g->root == NULL)
            return;
    }
    iterL = g->root;
    while (iterL->next != NULL)
    {
        if (strcmp(iterL->next->person.nickname, nickname) == 0)
        {
            if (iterL->next->friend_count != 0)
                Destroy_Friends(iterL->next);
            temp = iterL->next;
            iterL->next = iterL->next->next;
            free(temp);
            if(iterL->next == NULL)
                flag = 2;
            else
                flag = 1;
        }
        if (iterL->friend_count != 0)
        {
            Delete_Node(iterL, nickname);
        }
        if(flag == 2)
            break;
        iterL = iterL->next;
    }
    if (iterL->friend_count != 0 && flag != 2)
        Delete_Node(iterL, nickname);

    if (flag == 0)
        printf("No such user\n");
    g->user_count--;
}

void Info_User(graph *g)
{
    if(g->root == NULL)
    {
        printf("No user\n");
        return;
    }
    List *iter = g->root;
    Node *iterN;
    char nickname[10];
    printf("Enter user's nickname: ");
    scanf("%9[^\n]",nickname);
    while(getchar()!='\n');
    while (iter != NULL)
    {
        if(strcmp(iter->person.nickname,nickname) == 0)
        {
            printf("\nName: %s\nNickname: %s\nFriend count: %d",iter->person.name,iter->person.nickname,iter->friend_count);
            if(iter->friend_count != 0)
            {
                printf("\nFriends: ");
                iterN = iter->head;
                while(iterN != NULL)
                {
                    if(iterN == iter->head)
                        printf("%s (%s)",iterN->buddy.name,iterN->buddy.nickname);
                    else
                        printf(", %s (%s)",iterN->buddy.name,iterN->buddy.nickname);
                    iterN = iterN->next;
                }
            }
            break;
        }
        iter = iter->next;
    }
    if(iter == NULL)
        printf("No such nickname");
    printf("\n");
}
int Delete_Node(List *user, const char *nickname)
{
    Node *temp, *iter;
    if (user->head == NULL)
        return 0;
    if (strcmp(user->head->buddy.nickname, nickname) == 0)
    {
        temp = user->head;
        user->head = user->head->next;
        free(temp);
        user->friend_count--;
        return 1;
    }
    iter = user->head;
    while (iter->next != NULL)
    {
        if (strcmp(iter->next->buddy.nickname, nickname) == 0)
        {
            temp = iter->next;
            iter->next = iter->next->next;
            free(temp);
            user->friend_count--;
            return 1;
        }
        iter = iter->next;
    }
    return 0;
}

void Destroy_Friends(List *user)
{
    Node *iter, *current;

    current = user->head;
    if (current != NULL)
        iter = current->next;
    while (current != NULL)
    {
        free(current);
        current = iter;
        if (iter != NULL)
            iter = iter->next;
    }
    user->head = NULL;
}

void Destory_Graph(graph *g)
{
    List *iterL, *currentL;
    Node *iterN, *currentN;

    currentL = g->root;
    if (g->root != NULL)
        iterL = currentL->next;
    while (currentL != NULL)
    {
        Destroy_Friends(currentL);
        free(currentL);
        currentL = iterL;
        if (iterL != NULL)
            iterL = iterL->next;
    }
    free(g);
}