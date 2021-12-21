#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

struct _BinaryTree;
typedef struct _BinaryTree* Position;
typedef struct _BinaryTree{
    int N;
    Position L;
    Position R;
}BinaryTree;

struct _stog;
typedef struct _stog* Pointer;
typedef struct _stog {
    int n;
    Pointer next;
}Stog;

int MainMenu(Position pRoot);
Position CreateNewElement(int n);
Position Insert(Position p, Position newElement);
int Replace(Position p);
Position GenerateTree(Position pRoot);
int DeleteAll(Position p);
int InorderToStog(Position p, Pointer h);
int PrintInorder(Position p);
int PrintListInFile(char* fileName, Pointer first);
Pointer CreateNewElementStog (int n);
int DeleteAllStog(Pointer head);
int InsertAfter (Pointer p, Pointer newElement);
Pointer FindLast (Pointer head);
int AddEnd (Pointer head, int n);

int main()
{
    Position pRoot = NULL;
    
    pRoot=GenerateTree(pRoot);
    
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
                "3 -> Replace all elements with the sum of their branches\n"
                "4 -> Print inorder in file\n"
                "5 -> Generate new tree\n"
                "6 -> Exit\n");
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
                Replace(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 4:
            {
                Stog head={.n=0, .next=NULL};
                Pointer h=&head;
                char str[20]="0";
                
                printf("Enter the name of the .txt file you wish to write in:\n");
                scanf(" %s", str);
                strcat(str,".txt");
                InorderToStog(pRoot, h);
                PrintListInFile(str, h->next);
                printf("\n");
                action=0;
                break;
            }
            case 5:
            {
                printf("\n");
                DeleteAll(pRoot);
                pRoot=NULL;
                pRoot=GenerateTree(pRoot);
                printf("\n");
                action=0;
                break;
            }
            case 6:
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

Position GenerateTree(Position pRoot)
{
    int i=0;
    Position temp=NULL;
    
    srand(time(0));
    
    for(i=0;i<10;i++)
    {
        temp=CreateNewElement(rand()%79+11);
        if(!temp)
            return NULL;
        pRoot=Insert(pRoot, temp);
    }
    
    return pRoot;
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
        p->L=Insert(p->L,newElement);
    }
    else
    {
        p->R=Insert(p->R,newElement);
    }
    return p;
}

int PrintInorder(Position p)
{
    if(!p)
        return 0;
    PrintInorder(p->L);
    printf("%d ", p->N);
    PrintInorder(p->R);
    return 0;
}

int Replace(Position p)
{
    int temp=0;
    if(p == NULL)
    {
        return 0;
    }
    else
    {
        temp=p->N;
        p->N=Replace(p->R)+Replace(p->L);
        return p->N+temp;
    }
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

Pointer CreateNewElementStog (int n)
{
  Pointer newElement=NULL;
  
  newElement = (Pointer) malloc (sizeof (Stog));
  if(!newElement)
  {
      perror("Couldn't allocate memmory.\n");
      return NULL;
  }

  newElement->n = n;
  newElement->next=NULL;
  
  return newElement;
}

int InsertAfter (Pointer p, Pointer newElement)
{
  newElement->next = p->next;
  p->next = newElement;

  return 0;
}

Pointer FindLast (Pointer head)
{
  Pointer last = head;

  while (last->next)
    {
      last = last->next;
    }

  return last;
}

int AddEnd (Pointer head, int n)
{
  Pointer newElement = NULL;

  newElement = CreateNewElementStog (n);
  if (!newElement)
    return -1;
  
  InsertAfter(FindLast(head), newElement);

  return 0;
}

int InorderToStog(Position p, Pointer h)
{
    if(!p)
        return 0;
    InorderToStog(p->L,h);
    AddEnd(h, p->N);
    InorderToStog(p->R,h);
    return 0;
}

int PrintListInFile(char* fileName, Pointer first)
{
    Pointer temp=first;
    FILE* f=NULL;
    
    f=fopen(fileName, "w");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }
    
    for (temp; temp; temp = temp->next)
        fprintf (f, "%d ", temp->n);
    
    fclose(f);
    
    return 0;
}

int DeleteAllStog(Pointer head)
{
    Pointer temp=NULL;
    
    while (head->next!=NULL)
    {
        temp=head->next;
        head->next=temp->next;
        free(temp);
    }
    
    return 0;
}










