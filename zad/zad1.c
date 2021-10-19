#include <stdio.h>
#include<stdlib.h>

#define MAX_LEN 128

typedef struct _student
{
    char Ime[20];
    char Prezime[20];
    double Bodovi;
    double RelativniBodovi;
}student;

int UnosPodataka(student studenti[MAX_LEN], int br)
{
    FILE* f = NULL;
    int i=0;

    f=fopen("studenti.txt", "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }

    for(i;i<br;i++)
        fscanf(f," %s %s %lf", studenti[i].Ime, studenti[i].Prezime, &studenti[i].Bodovi);
    
    fclose(f);
    return 0;
}

int main()
{
    FILE* f = NULL;
    int br=0;
    char c[MAX_LEN];
    student* studenti=NULL;
    int i=0;
    int empty=0;
    double maxBodovi=0;

    f=fopen("studenti.txt", "r");
    if(f==NULL)
    {
        printf("Neuspjesno otvaranje datoteke.");
        return -1;
    }
    empty=getc(f);
    if(empty==EOF)
    {
        printf("Datoteka je prazna.");
        return -1;
    }
    while(!feof(f))
    {
        fgets(c,MAX_LEN,f);
        br++;
    }
    fclose(f);

    studenti=(student*)malloc(br*sizeof(student));
    if(studenti==NULL)
    {
        printf("Neuspjesno alociranje memorije.");
        return -1;
    }

    UnosPodataka(studenti,br);
    
    for(i=0;i<br;i++)
        if(maxBodovi<studenti[i].Bodovi)
            maxBodovi=studenti[i].Bodovi;
            
    printf(" Ime\t\tPrezime\t\tBodovi\t\tRelativni bodovi\n");
    for(i=0;i<br;i++)
        printf(" %s\t\t%s\t\t%.2lf\t\t%.2lf\n", studenti[i].Ime, studenti[i].Prezime, studenti[i].Bodovi, (studenti[i].Bodovi/maxBodovi)*100);

    free(studenti);
    return 0;
}
