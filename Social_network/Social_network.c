#include "SN_graph.h"

int Menu()
{
    int n;
    printf("\nWelcome to Social Network\n1. Add User\n2. Delete User\n3. Print Graph\n4. Add friends\n5. Delete friends\n6. Show info of a user\n7. Reset database\n8. Exit");
    label:
    printf("\n> ");
    if(scanf("%d",&n) == 1){
        if(n < 1 || n > 8)
        {
            printf("Invalid option!!");
            while(getchar()!='\n');
            goto label;
        }
    }
    else{
        printf("Invalid integer!!");
        while(getchar()!='\n');
        goto label;
    }
    return n;
}

int main()
{
    graph *g = Create_Graph();
    FILE *f = fopen("database.bin","rb");
    if(f != NULL)
    {
        Read_bin_file(g,"database.bin");
        printf("\nDatabase read succesfully...\n");
        fclose(f);
    }
    else
        printf("\nNo database entry found...\n");
    while(1)
    {
        int opt = Menu();
        char c, nickname[10];
        List *user;
        getchar();
        switch (opt)
        {
        case 1:
            Add_User(g);
            break;
        case 2:
            Delete_user(g);
            break;
        case 3:
            print_graph(g);
            break;
        case 4:
            Add_friend(g);
            break;
        case 5:
            if(g->user_count == 0)
            {
                printf("No user\n");
                break;
            }
        label:
            printf("Enter the user's nickname: ");
            scanf("%9[^\n]", nickname);
            while(getchar()!='\n');
            user = g->root;
            while(user != NULL)
            {
                if(strcmp(user->person.nickname,nickname) == 0)
                    break;
                user = user->next;
            }
            if(user == NULL)
            {
                printf("No such user\n");
                goto label;
            }
            printf("Enter %s's friend's nickname: ",user->person.name);
            scanf("%9[^\n]",nickname);
            while(getchar()!='\n');
            if(Delete_Node(user,nickname) == 0)
                printf("No such friend\n");
            break;
        case 6:
            Info_User(g);
            break;
        case 7:
            Destory_Graph(g);
            if(f != NULL)
                remove("database.bin");
            g = Create_Graph();
            break;
        default:
            if(g->root != NULL){
                printf("Do you want to save changes? (y/n): ");
                label1:
                scanf(" %c",&c);
                if(c != 'y' && c != 'n')
                {
                    printf("Invalid option!\n> ");
                    goto label1;
                }
                if(c == 'y')
                    Write_bin_file(g, "database.bin");
            }
            else if(f != NULL)
                remove("database.bin");
            Destory_Graph(g);
            return 0;
        }
    }
}