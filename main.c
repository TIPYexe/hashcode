#include <stdio.h>
#include <stdlib.h>

int numar_END(FILE* fin, int *start) //extrag numerele pe rand, de la dr->st
{
    fseek(fin, -(*start), SEEK_END); //plec de unde am ramas

    while(fgetc(fin)!=' ') //ma duc in stanga pana dau de spatiu => pot sa dau direct fscanf
    {
        (*start)++;
        fseek(fin, -(*start), SEEK_END);
    }
    (*start)++; //il cresc de aici ca sa nu mai incarc functia tata
    int nr;
    fscanf(fin, "%d", &nr);
    return nr;
}

void stanga(FILE *fin, int *st, int *diferenta, int indice, int *k)
{
    fseek()
    fscanf(fin, "%d ", &st[0]);

    while(st[(*k)-1] < diferenta && (*k) < indice) {
        fscanf(fin, "%d ", &st[(*k)-1]);
        (*k)++;
        //st= realloc(st, (*k) * sizeof(int));
    }

    if(st[(*k)-1]>diferenta)
        (*k)--; // k este numarul de valori din st, pt ca ultimul e prea mare
    else
        diferenta-= st[(*k)-1];

    // pana acum am salvat toate elementele st->dr pana la cel mai mare numar < diferenta
    // si l-am scazut din diferenta

    while( diferenta > 0 && diferenta > st[0] && (*k)>0)
    {
        (*k)--; // parcurg invers vectorul st (de la dr->st)
        if(st[(*k)-1]<diferenta)
            diferenta-= st[(*k)-1];
        else
            st[(*k)-1]= -1;
    }

}

void dreapta(FILE *fin, int *st, int *dr, int total, int indice, int *suma, int *seek, int *pozitii, int *diferenta, int *k) // st= suma elemen din stanga; Analog pentru dr
{
    int last;
    //free(st);
    //cand ma apuc sa calc o suma din dreapta inseamna ca
    // am luat un caz nou, deci nu mai imi trebuie ce aveam in st

    while((*suma)<total && (*pozitii) < indice)
    {
        last=numar_END(fin, seek);
        (*suma)+=last;
        //dr= realloc(dr, (*pozitii) * sizeof(int));
        dr[(*pozitii) - 1] = last; //salvez numerele
        (*pozitii)++;
    }

    if((*suma)>total)
    {
        (*suma)-=last;
        (*pozitii)--;
        printf("Suma DR:%d\n", (*suma));
        *diferenta = total-(*suma);
        k=1;
        stanga(fin, st, diferenta, indice - (*pozitii), k);
    }
}

void adunare(FILE* fin, FILE* fout, int *st, int *dr, int total, int indice)
{
    int diferenta=0, nr_dreapta=0, nr_stanga, sum_dreapta=0, repetari=0, seek=0;
    int *st_best, *dr_best, repetari_best, nr_stanga_best, nr_dreapta_best, diferenta_best;
    dreapta(fin, st, dr, total, indice, &sum_dreapta, &seek, &nr_dreapta, &diferenta, &nr_stanga);
    diferenta_best = diferenta;
    while(diferenta>0)
    {
        sum_dreapta-=dr[repetari];
        nr_dreapta--;
        repetari++;
        nr_stanga=0;
        dreapta(fin, st, dr, total, indice, &sum_dreapta, &seek, &nr_dreapta, &diferenta, &nr_stanga);

        if(diferenta < diferenta_best)
        {
            free(st_best);
            free(dr_best);
            memcpy(st_best, st, sizeof(st));
            memcpy(dr_best, dr, sizeof(dr));
            repetari_best= repetari;
            nr_dreapta_best = nr_dreapta;
            nr_stanga_best = nr_stanga;
            diferenta_best=diferenta;
        }
    }

    printf("%d\n", total-diferenta);
    for(int i=0; i<nr_stanga_best; i++)
        if(st_best[i]=!-1)
            printf("%d ", i);
    for(int i=0 ; i < nr_dreapta_best; i++)
        printf("%d ", i + indice - repetari_best - nr_dreapta_best);
}

int main() {

    FILE *fin= fopen("E:\\CLion C\\Model_examen\\b_small.in", "r");
    FILE *fout= fopen("E:\\CLion C\\Model_examen\\COUT", "w");

    int total, nr;
    fscanf(fin, "%d %d", &total, &nr);

    int *st, *dr;
    dr= malloc(nr*sizeof(int));
    st= malloc(nr*sizeof(int));

    adunare(fin, fout, st, dr, total, nr);
}
