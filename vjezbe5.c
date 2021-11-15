#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _StackElement;
typedef struct _StackElement* Pointer;
typedef struct _StackElement
{
    double number;
    Pointer next;
}StackElement;

Pointer CreateNewElement(double number);
int InsertAfter(Pointer p, Pointer newElement);
int Push(Pointer head, double number);
int DeleteAfter(Pointer p);
int Pop(double* resultDestination, Pointer head);
int PerformOperation(Pointer head, char operation);
int CalculatePostfixFromFile(double* resultDestination);
int DeleteAll(Pointer head);
int Menu();

int main()
{
    Menu();
    
    return 0;
}

int Menu()
{
    int action=0;
    double result=0;
    
    while(1)
    {
        printf("1 -> Calculate postfix from a file\n"
                "2 -> Exit the program\n");
        scanf("%d", &action);
        
        switch(action)
        {
            
            case(1):
            {
                CalculatePostfixFromFile(&result);
                printf("The result is: %.2lf\n", result);
                break;
            }
            case(2):
            {
                return 0;
            }
            default:
            {
                printf("Wrong input. Try again.\n");
                break;
            }
        }
    }
    return -1;
}

Pointer CreateNewElement(double number)
{
    Pointer newElement=NULL;

    newElement=(Pointer)malloc(sizeof(StackElement));
    if(!newElement)
    {
        perror("Couldn't allocate memmory.");
        return NULL;
    }

    newElement->number=number;
    newElement->next=NULL;

    return newElement;
}

int InsertAfter (Pointer p, Pointer newElement)
{
  newElement->next = p->next;
  p->next = newElement;

  return 0;
}

int DeleteAfter(Pointer p)
{
    Pointer toDelete=NULL;
    
    toDelete=p->next;
    p->next=toDelete->next;
    free(toDelete);
    
    return 0;
}

int DeleteAll(Pointer head)
{
    Pointer toDelete=NULL;
    Pointer p=head;
    
    for(p;p->next;p)
    {
        toDelete=p->next;
        p->next=toDelete->next;
        free(toDelete);
    }
    
    return 0;
}

int Push(Pointer head, double number)
{
    Pointer newElement=NULL;
    
    newElement=CreateNewElement(number);
    if(!newElement)
    {
        return -1;
    }
    
    InsertAfter(head, newElement);
    
    return 0;
}

int Pop(double* resultDestination, Pointer head)
{
    Pointer first=head->next;
    
    if(!first)
    {
        perror("Invalid postfix. Please check your file.\n");
        return -1;
    }
    
    *resultDestination=first->number;
    DeleteAfter(head);
    
    return 0;
}

int PerformOperation(Pointer head, char operation)
{
    double operand1=0;
    double operand2=0;
    double result=0;
    
	if (Pop(&operand2, head) != 0)
	{
		return -1;
	}
    
	if (Pop(&operand1, head) != 0)
	{
		return -2;
	}
	
	switch(operation)
	{
	    case '+':
	        result=operand1+operand2;
	        break;
	    case '-':
	        result=operand1-operand2;
	        break;
	    case '*':
	        result=operand1*operand2;
	        break;
	    case '/':
	        if(operand2==0)
	        {
	            printf("Can't divide with zero!");
	            return -3;
            }
	        result=operand1/operand2;
	    default:
	        printf("Operation %c is not supported!\n", operation);
	        return -4;
	        
	}
    
    return Push(head, result);
}

int CalculatePostfixFromFile(double* resultDestination)
{int t=0;
    FILE* f=NULL;
    int fileLenght=0;
    char* buffer=NULL;
    char* currentBuffer =NULL;
    char temp[50]={0};
    int numBytes = 0;
    int status = 0;
    double number = 0;
    char operation = 0;
    StackElement head ={.next = NULL, .number=0};
    
    while(1)
    {
        printf("Enter the name of the .txt file you wish to read from:\n");
        scanf(" %s", temp);
        if(strlen(temp)>4&&strcmp(&temp[strlen(temp)-4],".txt"))
            strcat(temp, ".txt");
        f=fopen(temp, "r");
        if(!f)
        {
            printf("Couldn't open file. Check file name and try again.\n");
        }
        else    
            break;
    }
    
    fseek(f, 0, SEEK_END);
    fileLenght = ftell(f);
    
    buffer = (char*)calloc(fileLenght+1, sizeof(char));
    if(!buffer)
    {
        perror("Couldn't allocate memory.\n");
        return -2;
    }
    
    rewind(f);
    fread(buffer, sizeof(char), fileLenght, f);
    fclose(f);
    
    currentBuffer = buffer;
    
    while(strlen(currentBuffer)>0)
    {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
        if(status==1)
        {
            status = Push(&head, number);
            if(status)
            {
                free(buffer);
                DeleteAll(&head);
                return -3;
            }
            currentBuffer += numBytes;
        }
        else
        {
            sscanf(currentBuffer, " %c %n", &operation, &numBytes);
            status = PerformOperation(&head, operation);
            if(status)
            {
                free(buffer);
                DeleteAll(&head);
                return -4;
            }
            currentBuffer += numBytes;
        }
    }
    
    free(buffer);
    
    status=Pop(resultDestination, &head);
    if(status)
    {
        DeleteAll(&head);
        return -5;
    }
    
    if(head.next)
    {
        printf("Invalid postfix. Please check file.\n");
        return -5;
    }
    
    return 0;
}









