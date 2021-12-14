#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128

struct _Dir;
typedef struct _Dir* Position;
typedef struct _Dir{
    char name[MAX];
    Position sibling;
    Position child;
}Dir;


Position Insert(Position current, Position newDir);
Position CreateNewDir(char name[MAX]);
int PrintAllDir(Position p, int i);
int FindParent(Position* current, Position parent, Position p, char name[MAX]);
int FindByName(Position* current, Position p, char name[MAX]);
int DeleteTree(Position p);
int MainMenu(Position pRoot);

int main()
{  
    Dir root={.name="..", .sibling=NULL, .child=NULL};
    Position pRoot = &root;
    
    MainMenu(pRoot);

    return 0;
}

int MainMenu(Position pRoot)
{
    int action=0;
    Position current = pRoot;
    char name[MAX];
    
    while(1)
    {
        printf("\n1 - md\n"
                "2 - cd dir\n"
                "3 - cd..\n"
                "4 - dir\n"
                "5 - exit\n");
        scanf(" %d", &action);
        
        switch(action)
        {
            case 1:
            {
                printf("Enter the name of the new directory you wish to create:\n");
                scanf(" %s", name);
                current->child=Insert(current->child, CreateNewDir(name));
                break;
            }
            case 2:
            {
                Position p = pRoot;
                printf("Enter the name of the directory you wish to move to:\n");
                scanf(" %s", name);
                FindByName(&current, p, name);
                break;
            }
            case 3:
            {
                Position parent = pRoot;
                Position p = pRoot;
                FindParent(&current, parent, p, name);
                break;
            }
            case 4:
            {
                PrintAllDir(pRoot, 0);
                break;
            }
            case 5:
            {
                DeleteTree(pRoot->child);
                return 0;
            }
            default:
            {
                printf("Wrong input. Try again.\n");
                break;
            }
        }
    }
}

Position Insert(Position current, Position newDir)
{
    if(current == NULL)
    {
        return newDir;
    }
    else if(strcmp(current->name, newDir->name)>0)
    {
        newDir->sibling=current;
        return newDir;
    }
    else if(strcmp(current->name, newDir->name)<0)
    {
        current->sibling=Insert(current->sibling, newDir);
        return current;
    }
    else
    {
        printf("A file or directory with such a name already exists.\n");
        free(newDir);
        return current;
    }
}

Position CreateNewDir(char name[MAX])
{
    Position newDir = NULL;

    newDir=(Position)malloc(sizeof(Dir));
    if(!newDir)
    {
        printf("Couldn't allocate memory for the new directory.\n");
        return NULL;
    }

    strcpy(newDir->name, name);
    newDir->child=NULL;
    newDir->sibling=NULL;

    return newDir;
}

int PrintAllDir(Position p, int i)
{
    int j=0;
    printf("\n");
    for(j=0;j<i;j++)
        printf("  ");
    printf(" %s", p->name);
    if(p->child)
        PrintAllDir(p->child, i+1);
    if(p->sibling)
        PrintAllDir(p->sibling, i);
    
    return 0;
}

int FindParent(Position* current, Position parent, Position p, char name[MAX])
{
    if(strcmp(p->name, name)==0)
        *current = parent;
    if(p->child)
        FindParent(current, p, p->child, name);
    if(p->sibling)
        FindParent(current, parent, p->sibling, name);
    return 0;
}

int FindByName(Position* current, Position p, char name[MAX])
{
    if(strcmp(p->name, name)==0)
        *current = p;
    if(p->child)
        FindByName(current, p->child, name);
    if(p->sibling)
        FindByName(current, p->sibling, name);
    return 0;
}

int DeleteTree(Position p)
{
    if(!p)
        return 0;
    else if(!p->sibling&&!p->child)
    {
        free(p);
        return 0;
    }
    else
    {
       if(p->child)
        {
            DeleteTree(p->child);
            p->child=NULL;
        }
        if(p->sibling)
        {
            DeleteTree(p->sibling);
            p->sibling=NULL;
        }
        free(p);
    }
    return 0;
}








