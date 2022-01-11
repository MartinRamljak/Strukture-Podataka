#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (1024)

struct _list;
typedef struct _list* Pointer;
typedef struct _list{
    Pointer next;
    char name[128];
    int pop;
}list;

struct _tree;
typedef struct _tree* Position;
typedef struct _tree{
    Position L;
    Position R;
    char name[128];
    Pointer next;
}tree;

int MainMenu(Position pRoot);
Pointer CreateNewCity(char* name, int pop);
Position CreateNewCountry(char* name);
Pointer InsertCity(Pointer p, Pointer newElement);
Position InsertCountry(Position p, Position newElement);
int ReadCountriesFromFile(char* fileName, Position p);
Position ReadCitiesFromFile(char* fileName, Position p);
int PrintCities(Pointer p);
int PrintAll(Position p);
Position FindCountry(char* name, Position p);
int PrintInCountryBiggerThan(Position country, int n);
int PrintCitiesBiggerThan(Pointer p, int n);
int DeleteAll(Position p);
int DeleteList(Pointer p);

int main()
{
    tree root={.next=NULL, .name="0", .L=NULL, .R=NULL};
    Position pRoot=&root;
    
    
    return MainMenu(pRoot);
}

int MainMenu(Position pRoot)
{
    int action=0;
    
    while(1)
    {
        printf("\n1 -> Read from a file\n"
                "2 -> Print everything\n"
                "3 -> Print cities in a country with a population over n\n"
                "4 -> Delete everything\n"
                "5 -> Exit\n");
        scanf(" %d", &action);
        switch(action)
        {
            case 1:
            {
                char str[20]="0";
                printf("Enter the name of the .txt file you wish to read from:\n");
                scanf(" %s", str);
                if(strlen(str)>4&&strcmp(&str[strlen(str)-4],".txt"))
                    strcat(str, ".txt");
                ReadCountriesFromFile(str, pRoot);
                action=0;
                break;
            }
            case 2:
            {
                PrintAll(pRoot);
                action=0;
                break;
            }
            case 3:
            {
                char str[20]="0";
                int n=0;
                printf("Enter the name of the country and the minimum population:\n");
                scanf(" %s %d", str, &n);
                PrintInCountryBiggerThan(FindCountry(str, pRoot), n);
                action=0;
                break;
            }
            case 4:
            {
                DeleteAll(pRoot->L);
                DeleteAll(pRoot->R);
                strcpy(pRoot->name, "0");
                pRoot->L=NULL;
                pRoot->R=NULL;
                action=0;
                break;
            }
            case 5:
            {
                DeleteAll(pRoot->L);
                DeleteAll(pRoot->R);
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

Pointer CreateNewCity(char* name, int pop)
{
    Pointer newCity=NULL;
    
    newCity=(Pointer)malloc(sizeof(list));
    if(!newCity)
    {
        printf("Couldn't allocate memory for the new element.\n");
        return NULL;
    }
    
    strcpy(newCity->name, name);
    newCity->next=NULL;
    newCity->pop=pop;
    
    return newCity;
}

Position CreateNewCountry(char* name)
{
    Position newCountry=NULL;
    
    newCountry=(Position)malloc(sizeof(tree));
    if(!newCountry)
    {
        printf("Couldn't allocate memory for the new element.\n");
        return NULL;
    }
    
    strcpy(newCountry->name, name);
    newCountry->next=NULL;
    newCountry->R=NULL;
    newCountry->L=NULL;
    
    return newCountry;
}

Pointer InsertCity(Pointer p, Pointer newElement)
{
    Pointer temp=NULL;
    
    if(!p)
        return newElement;
    
    temp=p;
    while(1)
    {
        if(temp->next!=NULL&&(temp->next->pop<newElement->pop||temp->next->pop==newElement->pop&&strcmp(temp->next->name, newElement->name)<0))
            temp=temp->next;
        else 
            break;
    }
    
    
    if(temp->next==NULL)
    {
        temp->next=newElement;
    }
    else if(strcmp(newElement->name, temp->next->name)==0)
    {
        free(newElement);
        return NULL;
    }
    else
    {
        newElement->next=temp->next;
        temp->next=newElement;
    }
    
    return NULL;
}

Position InsertCountry(Position p, Position newElement)
{
    if(p == NULL)
    {
        return newElement;
    }
    else if(strcmp(p->name, "0")==0)
    {
        strcpy(p->name, newElement->name);
        p->L=newElement->L;
        p->R=newElement->R;
        p->next=newElement->next;
        free(newElement);
    }
    else if(strcmp(p->name, newElement->name)>0)
    {
        p->L=InsertCountry(p->L,newElement);
    }
    else if(strcmp(p->name, newElement->name)<0)
    {
        p->R=InsertCountry(p->R,newElement);
    }
    else
    {
        free(newElement);
    }
    return p;
}

int ReadCountriesFromFile(char* fileName, Position p)
{
    Position temp=NULL;
    FILE* f=NULL;
    char buffer[MAX_SIZE]={0};
    char name[128]={0};
    char fName[128]={0};
    
    f=fopen(fileName, "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }
    
    while(!feof(f))
    {
        fgets(buffer, MAX_SIZE, f);
        if(sscanf(buffer, " %s %s", name, fName)==2)
        {
            temp=CreateNewCountry(name);
            ReadCitiesFromFile(fName, temp);
            InsertCountry(p, temp);
        }
    }
    
    fclose(f);
    
    return 0;
}

Position ReadCitiesFromFile(char* fileName, Position p)
{
    FILE* f=NULL;
    char buffer[MAX_SIZE]={0};
    char name[128]={0};
    int pop=0;
    
    
    f=fopen(fileName, "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return NULL;
    }
    
    while(!feof(f))
    {
        fgets(buffer, MAX_SIZE, f);
        if(sscanf(buffer, " %s %d", name, &pop)==2)
        {
            Pointer temp=CreateNewCity(name, pop);
            if(!p->next)
                p->next=temp;
            else if(p->next->pop>temp->pop)
            {
                temp->next=p->next;
                p->next=temp;
            }
            else if(p->next->pop==temp->pop&&strcmp(p->next->name, name)>0)
            {
                temp->next=p->next;
                p->next=temp;
            }
            else
                InsertCity(p->next, temp);
        }
    }
    
    fclose(f);
    
    return NULL;
}

int PrintAll(Position p)
{
    if(!p||strcmp(p->name, "0")==0)
        return 0;
    PrintAll(p->L);
    printf("%s:\n", p->name);
    PrintCities(p->next);
    PrintAll(p->R);
    
    return 0;
}

int PrintCities(Pointer p)
{
    if(!p)
        return 0;
    printf("\t%-12s - %d\n", p->name, p->pop);
    PrintCities(p->next);
    return 0;
}

Position FindCountry(char* name, Position p)
{
    Position temp=p;
    if(strcmp(temp->name, "0")==0)
        return NULL;
    else if(strcmp(temp->name, name)==0)
        return temp;
    else if(strcmp(temp->name, name)<0)
        temp=FindCountry(name, temp->R);
    else if(strcmp(temp->name, name)>0)
        temp=FindCountry(name, temp->L);
        
    return temp;
}

int PrintInCountryBiggerThan(Position country, int n)
{
    if(!country)
    {
        printf("There is no such country on the list.\n");
        return -1;
    }
    
    PrintCitiesBiggerThan(country->next, n);
    
    return 0;
}

int PrintCitiesBiggerThan(Pointer p, int n)
{
    if(!p)
        return 0;
    
    if(p->pop>=n)
        printf("\t%-12s - %d\n", p->name, p->pop);
    PrintCitiesBiggerThan(p->next, n);
    
    return 0;
}

int DeleteList(Pointer p)
{
    if(!p)
        return 0;
    DeleteList(p->next);
    free(p);
    return 0;
}

int DeleteAll(Position p)
{
    if(!p)
        return 0;
    DeleteAll(p->L);
    DeleteList(p->next);
    DeleteAll(p->R);
    free(p);
    
    return 0;
}





























