#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (1024)

struct _tree;
typedef struct _tree* Position;
typedef struct _tree{
    Position L;
    Position R;
    char name[128];
    int pop;
}tree;

struct _list;
typedef struct _list* Pointer;
typedef struct _list{
    Pointer next;
    char name[128];
    Position root;
}list;

int MainMenu(Pointer heads);
Position CreateNewCity(char* name, int pop);
Pointer CreateNewCountry(char* name);
Position InsertCity(Position p, Position newElement);
int InsertCountry(Pointer head, Pointer newElement);
Pointer ReadCountriesFromFile(char* fileName, Pointer heads);
Position ReadCitiesFromFile(char* fileName, Pointer p);
int PrintCities(Position p);
int PrintAll(Pointer first);
int DeleteAllList(Pointer first);
int DeleteAllTree(Position p);
int DeleteAll(Pointer first);
Pointer FindCountry(char* name, Pointer first);
int PrintInCountryBiggerThan(Pointer country, int n);
int PrintCitiesBiggerThan(Position p, int n);
int GetHashPosition(char* country);

int main()
{
    Pointer heads=(Pointer)malloc(11*sizeof(list));
    int i=0;
    for(i=0;i<11;i++)
    {
        heads[i].next=NULL;
        strcpy(heads[i].name,"0");
        heads[i].root=NULL;
    }
    
    return MainMenu(heads);
}

int MainMenu(Pointer heads)
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
                heads=ReadCountriesFromFile(str, heads);
                if(!heads)
                    return -1;
                action=0;
                break;
            }
            case 2:
            {
                int i=0;
                for(i=0;i<11;i++)
                    PrintAll(&heads[i]);
                action=0;
                break;
            }
            case 3:
            {
                char str[20]="0";
                int n=0;
                int i=0;
                int t=0;
                printf("Enter the name of the country and the minimum population:\n");
                scanf(" %s %d", str, &n);
                for(i=0;i<11;i++)
                    t+=PrintInCountryBiggerThan(FindCountry(str, &heads[i]), n);
                if(t==-11)
                    printf("There is no such country.");
                action=0;
                break;
            }
            case 4:
            {
                int i=0;
                for(i=0;i<11;i++)
                {
                    DeleteAll(heads[i].next);
                    heads[i].next=NULL;
                }
                action=0;
                break;
            }
            case 5:
            {
                int i=0;
                for(i=0;i<11;i++)
                {
                    DeleteAll(heads[i].next);
                    heads[i].next=NULL;
                }
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

Position CreateNewCity(char* name, int pop)
{
    Position newCity=NULL;
    
    newCity=(Position)malloc(sizeof(tree));
    if(!newCity)
    {
        printf("Couldn't allocate memory for the new element.\n");
        return NULL;
    }
    
    strcpy(newCity->name, name);
    newCity->pop=pop;
    newCity->R=NULL;
    newCity->L=NULL;
    
    return newCity;
}

Pointer CreateNewCountry(char* name)
{
    Pointer newCountry=NULL;
    
    newCountry=(Pointer)malloc(sizeof(list));
    if(!newCountry)
    {
        printf("Couldn't allocate memory for the new element.\n");
        return NULL;
    }
    
    strcpy(newCountry->name, name);
    newCountry->next=NULL;
    newCountry->root=NULL;
    
    return newCountry;
}

Position InsertCity(Position p, Position newElement)
{
    if(p == NULL)
    {
        return newElement;
    }
    else if(p->pop>newElement->pop)
    {
        p->L=InsertCity(p->L,newElement);
    }
    else if(p->pop<newElement->pop)
    {
        p->R=InsertCity(p->R,newElement);
    }
    else if(strcmp(p->name, newElement->name)>0)
    {
        p->L=InsertCity(p->L,newElement);
    }
    else if(strcmp(p->name, newElement->name)<0)
    {
        p->R=InsertCity(p->R,newElement);
    }
    else
    {
        free(newElement);
    }
    return p;
}

int InsertCountry(Pointer head, Pointer newElement)
{
    Pointer temp=NULL;
    
    temp=head;
    while(1)
    {
        if(temp->next!=NULL&&strcmp(temp->next->name, newElement->name)<0)
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
        return -1;
    }
    else
    {
        newElement->next=temp->next;
        temp->next=newElement;
    }
    
    return 0;
}

list* ReadCountriesFromFile(char* fileName, Pointer heads)
{
    Pointer temp=NULL;
    Pointer temp2=NULL;
    FILE* f=NULL;
    char buffer[MAX_SIZE]={0};
    char name[128]={0};
    char fName[128]={0};
    list pFiles[11];
    int i=0;
    
    f=fopen(fileName, "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return NULL;
    }
    
    while(!feof(f))
    {
        fgets(buffer, MAX_SIZE, f);
        if(sscanf(buffer, " %s %s", name, fName)==2)
        {
            i=GetHashPosition(name);
            InsertCountry(&heads[i], CreateNewCountry(name));
            ReadCitiesFromFile(fName, heads[i].next);
        }
    }
    
    fclose(f);
    
    return heads;
}

Position ReadCitiesFromFile(char* fileName, Pointer p)
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
            Position newCity=NULL;
            
            newCity=CreateNewCity(name, pop);
            p->root=InsertCity(p->root, newCity);
        }
    }
    
    fclose(f);
    
    return p->root;
}

int PrintCities(Position p)
{
    if(!p)
        return 0;
    
    PrintCities(p->L);
    printf("\t%-12s - %d\n", p->name, p->pop);
    PrintCities(p->R);
    
    return 0;
}

int PrintAll(Pointer first)
{
    Pointer temp=first;
    while(temp)
    {
        if(strcmp(temp->name,"0")!=0)
        {
            printf("%s:\n", temp->name);
            PrintCities(temp->root);
        }
            temp=temp->next;
    }
    return 0;
}

int DeleteAllList(Pointer first)
{
    if(!first)
        return 0;
    DeleteAllList(first->next);
    free(first);
    return 0;
}

int DeleteAll(Pointer first)
{
    if(!first)
        return 0;
    
    DeleteAll(first->next);
    DeleteAllTree(first->root);
    free(first);
    return 0;
}

int DeleteAllTree(Position p)
{
    if(!p)
        return 0;
    if(!p->L&&!p->R)
        free(p);
    else
    {
        if(p->L)
            DeleteAllTree(p->L);
        if(p->R)
            DeleteAllTree(p->R);
        free(p);
    }
    return 0;
}

Pointer FindCountry(char* name, Pointer first)
{
    Pointer temp=first;
    while(temp!=NULL&&strcmp(temp->name, name)!=0)
    {
        temp=temp->next;
    }
    return temp;
}

int PrintInCountryBiggerThan(Pointer country, int n)
{
    if(!country)
    {
        return -1;
    }
    
    PrintCitiesBiggerThan(country->root, n);
    
    return 0;
}

int PrintCitiesBiggerThan(Position p, int n)
{
    if(!p)
        return 0;
    PrintCitiesBiggerThan(p->R, n);
    if(p->pop>=n)
    {
        printf("\t%-12s - %d\n", p->name, p->pop);
        PrintCitiesBiggerThan(p->L, n);
    }
    
    return 0;
}

int GetHashPosition(char* country)
{
    int i=0;
    int key=0;
    
    for(i=0;i<strlen(country)&&i<5;i++)
        key+=(int)country[i];
    
    key=key%11;
    
    return key;
}






















