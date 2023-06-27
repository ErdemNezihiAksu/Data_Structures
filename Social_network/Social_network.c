#include "SN_graph.h"
#include <stdio.h>
#include <stdlib.h>

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
        char c;
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
            break;
        case 6:
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
                label:
                scanf(" %c",&c);
                if(c != 'y' && c != 'n')
                {
                    printf("Invalid option!\n> ");
                    goto label;
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