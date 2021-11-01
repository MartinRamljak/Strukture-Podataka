#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (1024)

struct _Person;
typedef struct _Person *Position;
typedef struct _Person
{
  char name[MAX_SIZE];
  char surname[MAX_SIZE];
  int birthYear;
  Position next;
} Person;

Position CreateNewPerson (char* name, char* surname, int birthYear)
{
  Position newPerson=NULL;
  
  newPerson = (Position) malloc (sizeof (Person));
  if(!newPerson)
  {
      perror("Couldn't allocate memmory.\n");
      return NULL;
  }
  
  strcpy(newPerson->name,name);
  strcpy(newPerson->surname,surname);
  newPerson->birthYear = birthYear;
  newPerson->next=NULL;
  
  return newPerson;
}

int InsertAfter (Position p, Position newPerson)
{
  newPerson->next = p->next;
  p->next = newPerson;

  return 0;
}

Position FindLast (Position head)
{
  Position last = head;

  while (last->next)
    {
      last = last->next;
    }

  return last;
}

int AddStart (Position head, char* name, char* surname, int birthYear)
{
  Position newPerson = NULL;

  newPerson = CreateNewPerson (name, surname, birthYear);
  if (!newPerson)
    return -1;

  InsertAfter (head, newPerson);

  return 0;
}

int AddEnd (Position head, char* name, char* surname, int birthYear)
{
  Position newPerson = NULL;
  Position last=NULL;

  newPerson = CreateNewPerson (name, surname, birthYear);
  if (!newPerson)
    return -1;
  
  last=FindLast(head);
  InsertAfter (last, newPerson);

  return 0;
}

int PrintList (Position first)
{
  Position temp = first;
  
  if(!temp)
    printf("The list is empty. :(\n");
  for (temp; temp; temp = temp->next)
    printf ("Name: %-10s Surname: %-13s Birthyear: %d\n",
	    temp->name, temp->surname, temp->birthYear);
    printf("\n");

  return 0;
}

Position FindBySurname(char* surname, Position first)
{
    Position temp=first;
    
    for(temp;temp;temp=temp->next)
        if(strcmp(temp->surname,surname)==0)
            return temp;
    
    return temp;
}

Position FindPrevious(char* surname, Position head)
{
    Position temp=head;
    
    for(temp;temp->next;temp=temp->next)
        if(strcmp(temp->next->surname,surname)==0)
            return temp;
    
    return temp;
}

int Delete(char* surname, Position head)
{
    Position temp=head;
    Position toDelete=NULL;
    
    for(temp;temp->next;temp=temp->next)
        if(strcmp(temp->next->surname,surname)==0)
            {
                toDelete=temp->next;
                temp->next=toDelete->next;
                free(toDelete);
                return 0;
            }
    
    return -1;
}

int DeleteAll(Position head)
{
    Position temp=NULL;
    
    while (head->next!=NULL)
    {
        temp=head->next;
        head->next=temp->next;
        free(temp);
    }
    
    return 0;
}

int AddAfter(Position head, char* name, char* surname, int birthYear, char* after)
{
    Position temp=NULL;
    Position newPerson = NULL;
    
    temp=FindBySurname(after, head->next);
    if(!temp)
        return -1;

    newPerson = CreateNewPerson (name, surname, birthYear);
    if (!newPerson)
        return -1;

    InsertAfter (temp, newPerson);
    
    return 0;
}

int AddBefore(Position head, char* name, char* surname, int birthYear, char* before)
{
    Position temp=NULL;
    Position newPerson = NULL;
    
    temp=FindPrevious(before, head);
    if(!temp)
        return -1;

    newPerson = CreateNewPerson (name, surname, birthYear);
    if (!newPerson)
        return -1;

    InsertAfter (temp, newPerson);
    
    return 0;
}

int SortedAdd(Position head, char* name, char* surname, int birthYear)
{
    Position temp=head;
    Position newPerson=NULL;
    
    for(temp;temp->next;temp=temp->next)
    {
        if(strcmp(surname, temp->next->surname)==0)
        {
            if(strcmp(name, temp->next->name)<0)
                break;
            if(strcmp(name, temp->next->name)==0)
            {
                if(birthYear<temp->next->birthYear)
                    break;
            }
        }
        if(strcmp(surname, temp->next->surname)<0)
            break;
    }
    
    newPerson=CreateNewPerson(name, surname, birthYear);
    if (!newPerson)
        return -1;

    InsertAfter (temp, newPerson);
    
    return 0;
}

int PrintListInFile(char* fileName, Position first)
{
    Position temp=first;
    FILE* f=NULL;
    
    f=fopen(fileName, "w");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }
    
    fprintf(f, " Name       Surname       Birthyear");
    for (temp; temp; temp = temp->next)
        fprintf (f, "\n %-10s %-13s %d",
	            temp->name, temp->surname, temp->birthYear);
    printf("\n");
    
    fclose(f);
    
    return 0;
}

int ReadListFromFile(char* fileName, Position head)
{
    Position temp=head;
    FILE* f=NULL;
    char buffer[MAX_SIZE]={0};
    char name[20]={0};
    char surname[20]={0};
    int birthYear=0;
    
    f=fopen(fileName, "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }
    
    while(!feof(f))
    {
        fgets(buffer, MAX_SIZE, f);
        if(sscanf(buffer, " %s %s %d", name, surname, &birthYear)==3)
        {
            SortedAdd(temp, name, surname, birthYear);
        }
    }
    
    fclose(f);
    
    return 0;
}

int Sort(Position head)
{
    PrintListInFile("Sort.txt", head->next);
    
    DeleteAll(head);
    
    ReadListFromFile("Sort.txt", head);
    
    return 0;
}

int MainMenu(Position p)
{
    char fileName={0};
    int action=0;
    char name[20]={0};
    char surname[20]={0};
    char str[20]="0";
    int birthYear=0;
    int temp=1;
    int OnOff=1;
    
    while(OnOff)
    {
        temp=1;
        printf("\n1 -> Add a new person to the list\n"
                "2 -> Delete someone from the list\n"
                "3 -> Check if someone is on the list\n"
                "4 -> Print the list\n"
                "5 -> Delete the list\n"
                "6 -> Read the list from a file\n"
                "7 -> Sort the list\n"
                "8 -> Write the list in a file\n"
                "9 -> Exit the program\n");
        scanf("%d", &action);
        
        switch (action)
        {
            case 1:
            {
                while(temp)
                {
                    printf("\n1 -> Add to the beginning\n"
                            "2 -> Add to the end\n"
                            "3 -> Add sorted (this is pointless if the list insn't sorted already)\n"
                            "4 -> Return to previous menu\n");
                    scanf("%d", &action);
                    switch (action)
                    {
                        case 1:
                        {
                            while(1)
                            {
                                printf("Enter in order: name surname birthyear\n");
                                if(scanf(" %s %s %d", name, surname, &birthYear)==3)
                                {
                                    AddStart(p, name, surname, birthYear);
                                    break;
                                }
                                else
                                    printf("Wrong input. Try again.\n");
                            }
                            break;
                        }
                        case 2:
                        {
                            while(1)
                            {
                                printf("Enter in order: name surname birthyear\n");
                                if(scanf(" %s %s %d", name, surname, &birthYear)==3)
                                {
                                    AddEnd(p, name, surname, birthYear);
                                    break;
                                }
                                else
                                    printf("Wrong input. Try again.\n");
                            }
                            break;
                        }
                        case 3:
                        {
                            while(1)
                            {
                                printf("Enter in order: name surname birthyear\n");
                                if(scanf(" %s %s %d", name, surname, &birthYear)==3)
                                {
                                    SortedAdd(p, name, surname, birthYear);
                                    break;
                                }
                                else
                                    printf("Wrong input. Try again.\n");
                            }
                            break;
                        }
                        case 4:
                        {
                            temp=0;
                            break;
                        }
                        default:
                        {
                            printf("Wrong input. Try again.\n");
                            break;
                        }
                    }
                }
                break;
            }
            case 2:
            {
                printf("Enter the surname of the person you wish to delete\n");
                scanf(" %s", str);
                Delete(str, p);
                break;
            }
            case 3:
            {
                printf("Enter the surname of the person you wish to check\n");
                scanf(" %s", str);
                if(!FindBySurname(str,p->next))
                    printf("There is no Easter Bunny, there is no Tooth Fairy, and there is no %s!\n", str);
                else
                    printf("%s is on the list.\n", str);
                break;
            }
            case 4:
            {
                PrintList(p->next);
                break;
            }
            case 5:
            {
                DeleteAll(p);
                break;
            }
            case 6:
            {
                DeleteAll(p);
                printf("Enter the name of the .txt file you wish to read from:\n");
                scanf(" %s", str);
                strcat(str,".txt");
                ReadListFromFile(str, p);
                break;
            }
            case 7:
            {
                Sort(p);
                break;
            }
            case 8:
            {
                printf("Enter the name of the .txt file you wish to write in:\n");
                scanf(" %s", str);
                strcat(str,".txt");
                PrintListInFile(str, p->next);
                break;
            }
            case 9:
            {
                OnOff=0;
                break;
            }
            default:
            {
                printf("Wrong input. Try again.\n");
                break;
            }
        }
    }
    
    return 0;
}

int main ()
{
  Person head = {.name = {0},.surname = {0},.birthYear = 0,.next = NULL };
  Position p = &head;
  
  MainMenu(p);
  
  DeleteAll(p);
  
  return 0;
}



