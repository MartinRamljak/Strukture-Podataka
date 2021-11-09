#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct _polinom;
typedef struct _polinom* Pointer;
typedef struct _polinom {
    int coef;
    int expon;
    Pointer next;
}polinom;

int ReadFromFile(Pointer head1, Pointer head2);
Pointer FindExponentPrev(Pointer head, int exp);
Pointer CreateNewElement(int coef, int expon);
int DeleteAfter(Pointer p);
int DeleteAll(Pointer head);
int SortedInsert(Pointer head, Pointer newElement);
int PrintPolynomial(Pointer head);
int PrintElement(Pointer temp);
int PolynomialSum(Pointer first1, Pointer first2, Pointer headSum);
int PolynomialProduct(Pointer first1, Pointer first2, Pointer headProduct);
int MainMenu(Pointer p1, Pointer p2, Pointer pSum, Pointer pProduct);

int main()
{
    polinom head1={.coef=0, .expon=0, .next=NULL};
    Pointer p1=&head1;
    polinom head2={.coef=0, .expon=0, .next=NULL};
    Pointer p2=&head2;
    polinom headSum={.coef=0, .expon=0, .next=NULL};
    Pointer pSum=&headSum;
    polinom headProduct={.coef=0, .expon=0, .next=NULL};
    Pointer pProduct=&headProduct;
    
    MainMenu(p1, p2, pSum, pProduct);
    
    return 0;
}

int MainMenu(Pointer p1, Pointer p2, Pointer pSum, Pointer pProduct)
{
    int action=0;
    
    while(1)
    {
        printf("\n1 -> Read 2 polynomials from a file\n"
                "2 -> Sum up the 2 last read polynomials\n"
                "3 -> Multiply the 2 last read polynomials\n"
                "4 -> Print out the 1. polynomial\n"
                "5 -> Print out the 2. polynomial\n"
                "6 -> Print out both polynomials\n"
                "7 -> Print out the sum\n"
                "8 -> Print out the product\n"
                "9 -> Exit the program\n");
        scanf("%d", &action);
        
        switch(action)
        {
            case(1):
            {
                DeleteAll(p1);
                DeleteAll(p2);
                ReadFromFile(p1,p2);
                break;
            }
            case(2):
            {
                DeleteAll(pSum);
                PolynomialSum(p1->next, p2->next, pSum);
                break;
            }
            case(3):
            {
                DeleteAll(pProduct);
                PolynomialProduct(p1->next, p2->next, pProduct);
                break;
            }
            case(4):
            {
                PrintPolynomial(p1);
                break;
            }
            case(5):
            {
                PrintPolynomial(p2);
                break;
            }
            case(6):
            {
                PrintPolynomial(p1);
                PrintPolynomial(p2);
                break;
            }
            case(7):
            {
                PrintPolynomial(pSum);
                break;
            }
            case(8):
            {
                PrintPolynomial(pProduct);
                break;
            }
            case(9):
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
    
    return 0;
}

Pointer FindExponentPrev(Pointer head, int exp)
{
    Pointer p=head;

    while(p->next!=NULL&&p->next->expon<exp)
    {
        p=p->next;
    }

    return p;
}

Pointer CreateNewElement(int coef, int expon)
{
    Pointer newElement=NULL;

    newElement=(Pointer)malloc(sizeof(polinom));
    if(!newElement)
    {
        perror("Couldn't allocate memmory.");
        return NULL;
    }

    newElement->coef=coef;
    newElement->expon=expon;
    newElement->next=NULL;

    return newElement;
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

int InsertAfter(Pointer p, Pointer newElement)
{
    newElement->next=p->next;
    p->next=newElement;
    
    return 0;
}

int SortedInsert(Pointer head, Pointer newElement)
{
    Pointer temp=NULL;
    temp=FindExponentPrev(head, newElement->expon);
    
    if(temp->next==NULL||newElement->expon<temp->next->expon)
    {
        if(newElement->coef==0)
            return 0;
        InsertAfter(temp, newElement);
    }
    else
    {
        if(temp->next->coef+newElement->coef==0)
            DeleteAfter(temp);
        else
            temp->next->coef+=newElement->coef;
        free(newElement);
    }
    
    return 0;
}

int PrintPolynomial(Pointer head)
{
    Pointer temp=head;
    
    if(!temp->next)
        return 0;
    
    temp=temp->next;
    
    if(temp->coef<0)
        printf(" -");
    
    for(temp;temp;temp=temp->next)
    {
        PrintElement(temp);
        if(temp->next==NULL)
            printf("\n");
        else if(temp->next->coef<0)
            printf(" -");
        else
            printf(" +");
    }
    
    return 0;
}

int PrintElement(Pointer temp)
{
    if(temp->coef==1&&temp->expon==0)
        printf(" 1");
    if(temp->coef==1);
    else if(temp->coef<0)
    {
        printf(" %d", (-1)*temp->coef);
    }
    else
        printf(" %d", temp->coef);
    
    if(temp->expon==0);
    else if(temp->expon<0)
        printf("x^(%d)", temp->expon);
    else
        printf("x^%d", temp->expon);
    
    return 0;
}

int ReadFromFile(Pointer head1, Pointer head2)
{
    Pointer p=NULL;
    FILE* f=NULL;
    char buffer[1024]={0};
    char temp[50]={0};
    int count=1;
    int coef=0;
    int expon=0;
    int n=0;
    Pointer newElement=NULL;

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

    fgets(buffer, 1024, f);

    while(buffer[count-1]!='\0')
    {
        if(sscanf(&buffer[count-1], " %d %d %n", &coef, &expon, &n)==2)
        {
            p=FindExponentPrev(head1, expon);
            newElement=CreateNewElement(coef, expon);
            SortedInsert(p, newElement);
        }
        else
        {
            printf("Something's wrong with the file, I can feel it.\n"
                    "The file won't be read further.\n");
            return -1;
        }
        
        count+=n;
    }
    
    count=1;
    
    fgets(buffer, 1024, f);

    while(buffer[count-1]!='\0')
    {
        if(sscanf(&buffer[count-1], " %d %d %n", &coef, &expon, &n)==2)
        {
            p=FindExponentPrev(head2, expon);
            newElement=CreateNewElement(coef, expon);
            SortedInsert(p, newElement);
        }
        else
        {
            printf("Something's wrong with the file, I can feel it.\n"
                    "The file won't be read further.\n");
            return -1;
        }
        
        count+=n;
    }

    fclose(f);
    return 0;
}

int PolynomialSum(Pointer first1, Pointer first2, Pointer headSum)
{
    Pointer pSum=headSum;
    Pointer p1=first1;
    Pointer p2=first2;
    Pointer newElement=NULL;
    
    while(p1!=NULL&&p2!=NULL)
    {
        if(p1->expon==p2->expon)
        {
        newElement=CreateNewElement(p1->coef+p2->coef, p1->expon);
            SortedInsert(pSum, newElement);
            p1=p1->next;
            p2=p2->next;
        }
        else if(p1->expon<p2->expon)
        {
            newElement=CreateNewElement(p1->coef, p1->expon);
            SortedInsert(pSum, newElement);
            p1=p1->next;
        }
        else
        {
            newElement=CreateNewElement(p2->coef, p2->expon);
            SortedInsert(pSum, newElement);
            p2=p2->next;
        }
    }
    
    if(p2==NULL);
    else p1=p2;
    
    while(p1!=NULL)
    {
        newElement=CreateNewElement(p1->coef, p1->expon);
        SortedInsert(pSum, newElement);
        p1=p1->next;
    }
    
    return 0;
}

int PolynomialProduct(Pointer first1, Pointer first2, Pointer headProduct)
{
    Pointer pProduct=headProduct;
    Pointer p1=first1;
    Pointer p2=first2;
    Pointer newElement=NULL;
    
    while(p1)
    {
        while(p2)
        {
            newElement = CreateNewElement(p1->coef*p2->coef, p1->expon+p2->expon);
            SortedInsert(pProduct, newElement);
            p2=p2->next;
        }
        p2=first2;
        p1=p1->next;
    }
    
    return 0;
}













