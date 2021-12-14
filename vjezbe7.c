#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 128

struct _BinaryTree;
typedef struct _BinaryTree* Position;
typedef struct _BinaryTree{
    int N;
    Position L;
    Position R;
}BinaryTree;

Position Insert(Position p, Position newElement);
Position CreateNewElement(int n);
int PrintInorder(Position p);
int PrintPreorder(Position p);
int PrintPostorder(Position p);
int PrintLevelorder(Position p);
int PrintLevel(Position p, int l, int current);
int MaxLevel(Position p, int l);
int Find(Position p, int n);
Position Delete(Position p, int n);
int DeleteAll(Position p);
int MainMenu(Position pRoot);

int main()
{
    Position pRoot = NULL;
    
    MainMenu(pRoot);
    
    
    return 0;
}

int MainMenu(Position pRoot)
{
    int action=0;
    
    while(1)
    {
        printf("\n1 -> Add a new element to the tree\n"
                "2 -> Print inorder\n"
                "3 -> Print preorder\n"
                "4 -> Print postorder\n"
                "5 -> Print level order\n"
                "6 -> Find an element\n"
                "7 -> Delete an element\n"
                "8 -> Exit\n");
        scanf(" %d", &action);
        switch(action)
        {
            case 1:
            {
                int n;
                printf("\n");
                printf("Enter the value you would like to add to the tree:\n");
                scanf(" %d", &n);
                pRoot=Insert(pRoot, CreateNewElement(n));
                action=0;
                break;
            }
            case 2:
            {
                printf("\n");
                PrintInorder(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 3:
            {
                printf("\n");
                PrintPreorder(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 4:
            {
                printf("\n");
                PrintPostorder(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 5:
            {
                printf("\n");
                PrintLevelorder(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 6:
            {
                int n=0;
                printf("\n");
                printf("Enter the value you would like to check for:\n");
                scanf(" %d", &n);
                if(Find(pRoot, n))
                    printf("The value %d exists in the tree.\n", n);
                else
                    printf("The value %d doesn't exist in the tree.\n", n);
                action=0;
                break;
            }
            case 7:
            {
                int n=0;
                printf("\n");
                printf("Enter the value you would like to delete:\n");
                scanf(" %d", &n);
                if(!Find(pRoot, n));
                else
                    Delete(pRoot, n);
                action=0;
                break;
            }
            case 8:
            {
                DeleteAll(pRoot);
                return 0;
            }
            default:
            {
                printf("\n");
                printf("Wrong input. Try again.\n");
                action=0;
                break;
            }
        }
    }
    
    return -1;
}

int Find(Position p, int n)
{
    if(!p)
        return 0;
    if(p->N==n)
        return n;
    else if(p->N<n)
        return Find(p->R, n);
    else if(p->N>n)
        return Find(p->L, n);
    else
        return 0;
}

Position Delete(Position p, int n)
{
    Position temp=NULL;
    if(!p)
        return NULL;
    if(p->N==n)
    {
        if(p->L)
        {
            temp=p->L;
            while(temp->R)
            {
                temp=temp->R;
            }
            temp->L=p->L;
            temp->R=p->R;
        }
        else if(p->R)
        {
            temp=p->R;
            while(temp->L)
            {
                temp=temp->L;
            }
            temp->L=p->L;
            temp->R=p->R;
        }
        p->R=NULL;
        p->L=NULL;
        free(p);
        return temp;
    }
    else if(p->N<n)
        p->R=Delete(p->R, n);
    else
        p->L=Delete(p->L, n);
    return p;
}

int DeleteAll(Position p)
{
    if(!p)
        return 0;
    if(!p->L&&!p->R)
        free(p);
    else
    {
        if(p->L)
            DeleteAll(p->L);
        if(p->R)
            DeleteAll(p->R);
    free(p);
    }
    return 0;
}

Position CreateNewElement(int n)
{
    Position newElement=NULL;
    
    newElement=(Position)malloc(sizeof(BinaryTree));
    if(!newElement)
    {
        printf("Couldn't allocate memory for the new element.\n");
        return NULL;
    }
    
    newElement->N=n;
    newElement->R=NULL;
    newElement->L=NULL;
    
    return newElement;
}

Position Insert(Position p, Position newElement)
{
    if(p == NULL)
    {
        return newElement;
    }
    else if(p->N<newElement->N)
    {
        p->R=Insert(p->R,newElement);
    }
    else if(p->N>newElement->N)
    {
        p->L=Insert(p->L,newElement);
    }
    else
    {
        free(newElement);
        return p;
    }
    return p;
}

int PrintInorder(Position p)
{
    if(!p||p->N==INT_MAX)
        return 0;
    PrintInorder(p->L);
    printf("%d ", p->N);
    PrintInorder(p->R);
    return 0;
}

int PrintPreorder(Position p)
{
    if(!p||p->N==INT_MAX)
        return 0;
    printf("%d ", p->N);
    PrintPreorder(p->L);
    PrintPreorder(p->R);
    return 0;
}

int PrintPostorder(Position p)
{
    if(!p||p->N==INT_MAX)
        return 0;
    PrintPostorder(p->L);
    PrintPostorder(p->R);
    printf("%d ", p->N);
    return 0;
}

int MaxLevel(Position p, int l)
{
    int k=0;
    l++;
    if(p->L)
        l=MaxLevel(p->L, l);
    if(p->R)
    {
        k=MaxLevel(p->R, l);
        if(k>l)
            l=k;
    }
    return l;
}

int PrintLevel(Position p, int l, int current)
{
    if(!p||p->N==INT_MAX)
        return 0;
    if(l==current)
        printf("%d ", p->N);
    PrintLevel(p->L, l, current+1);
    PrintLevel(p->R, l, current+1);
    return 0;
}

int PrintLevelorder(Position p)
{
    int l=MaxLevel(p, -1);
    int i=0;
    for(i=0;i<=l;i++)
    {
        printf("\n");
        PrintLevel(p, i, 0);
    }
    return 0;
}


















