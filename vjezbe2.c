#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (64)

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
    printf("The list is empty. :(\n\n");
  for (temp; temp; temp = temp->next)
    printf ("Name: %s\tSurname: %s\tBirthyear: %d\n",
	    temp->name, temp->surname, temp->birthYear);

  return 0;
}

Position FindBySurname(char* surname, Position first)
{
    Position temp=first;
    
    for(temp;temp;temp=temp->next)
        if(strcmp(temp->surname,surname)==0)
            return temp;
    
    if(!temp)
        printf("There is no Easter Bunny, there is no Tooth fairy and there is no %s!\n\n", surname);
    return temp;
}

int Delete(char* surname, Position head)
{
    Position temp=head;
    Position toDelete=NULL;
    
    for(temp;temp->next;temp=temp->next)
        if(strcmp(temp->next->surname,surname)==0)
            break;
    toDelete=temp->next;
    temp->next=toDelete->next;
    free(toDelete);
    
    return 0;
}

int main ()
{
  Person head = {.name = {0},.surname = {0},.birthYear = 0,.next = NULL };
  Position p = &head;
  
  AddStart(&head, "Ante", "Antic", 1997);
  AddEnd(&head, "Mate", "Matic", 2001);
  AddStart(&head, "Toma", "Tomic", 1999);
  
  PrintList(p->next);
  printf("\n");
  
  Delete("Antic", &head);
  Delete("Matic", &head);
  Delete("Tomic", &head);
  
  PrintList(p->next);
  
  FindBySurname("Dujic", head.next);
  
  AddEnd(&head, "Mate", "Matic", 2001);
  AddStart(&head, "Ante", "Antic", 1997);
  AddEnd(&head, "Toma", "Tomic", 1999);
  
  PrintList(p->next);
  
  return 0;
}


