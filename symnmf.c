#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "symnmf.h"
void errorC()
{
        fprintf (stderr, "An Error Has Occurred!\n");
        exit(1);
}

static void freefree(double **xrr, int N)
{
    int a;
    for (a=0;a<N;a++)
    {
        free(xrr[a]);
    }
}

static double caldistance(double *vec1, double *vec2, int dd)
{
    int a;
    double coor;
    double dis;
    dis = (double) 0;

    for(a=0; a<dd ;a++) {
        coor = (double) ((double)(vec1[a] - vec2[a]))*((double)(vec1[a] - vec2[a]));
        dis += coor; }
    return dis;
}

double **symC(double **X, int N, int d) /*X={x1,x2,x3,.....xn} , xi=(c1,c2,c3,c4,....,cd)*/
{
    int a;
    int b;
    double val;
    double **A;
    A = (double **) malloc(N * sizeof(double *));
    if(A == NULL)
    {
        errorC();
        return NULL;
    }
    for (a = 0; a < N; a++) 
    {
        A[a] = (double *) malloc(N * sizeof(double));
        if(A[a]==NULL)
        {
            errorC();
            return NULL;
        }
    }
    for (a = 0; a < N; a++) /*fill matrix NxN*/
    {
        for (b = a; b < N; b++) 
        {
            if (a != b)
            {
                A[a][b] = exp(caldistance(X[a], X[b], d) / (-2.0));
                val=A[a][b];
                A[b][a] = val;
            }
            else 
            {
                A[a][b] = 0.0;
            }
        }
    }
    return A;}

double **ddgC(double **X, int N, int d) 
{
    int a;
    int b;
    double **matsym;
    double **D;
    double di;
    matsym = symC(X, N, d);
    D = (double **) malloc(N * sizeof(double *));
    if(D == NULL)/*not sure*/
    {
        errorC();
        return NULL;
    }
    for (a = 0; a < N ; a++) 
    {
        D[a] = (double *) calloc(N, sizeof(double));
        if(D[a]==NULL)/*not sure*/
        {
            errorC();
            return NULL;
        }
    }
    for (a = 0; a < N; a++)/*fill D*/
     {
        di =0; /*di for each row*/
        for (b = 0; b < N; b++) 
        {
            di += matsym[a][b];
        }
        D[a][a]=di;
    }
    freefree(matsym, N);
    free(matsym);
    return D;
}

double **MATmultMAT(double **X, double **Y,int r1,int c1 ,int c2,int p)/*p=0 => normal , p=1 => with colm+row*/
{
    int a, b, c;
    int N;
    double **mult;
    if (p==0)
    {
        N=r1;
        mult = (double **) malloc(N * sizeof(double*));
        if(mult==NULL)
        {
            errorC();
            return NULL;
        }
        for (a = 0; a < N; a++) 
        {
            mult[a] = (double *) malloc(N * sizeof(double));
            if(mult[a]==NULL)
            {
                errorC();
                return NULL;
            }
        }
        for (a= 0; a < N; a++) 
        {
            for (b = 0; b< N; b++) 
            {
                mult[a][b] = 0;/*important to avoid errors*/
                for (c= 0; c< N; c++) 
                {
                    mult[a][b] += X[a][c] * Y[c][b];
                }
            }
        }
        return mult;
    }
    else
    {
        mult = (double **) malloc(r1 * sizeof(double*));
        if(mult==NULL)
        {
            errorC();
            return NULL; 
        }
        for (a = 0; a < r1; a++) 
        {
            mult[a] = (double *) malloc(c2 * sizeof(double));
            if(mult[a]==NULL)
            {
                errorC();
                return NULL;  
            }
        }
        for (a = 0; a< r1; a++)
        {
            for (b = 0; b < c2; b++) 
            {
                mult[a][b] = 0;/*to avoid errors*/
                for (c = 0; c < c1; c++) 
                {
                    mult[a][b] += X[a][c] * Y[c][b];
                }
            } 
        }
        return mult;
    }
    
}

double **normC(double **X,int N, int d)
{
    int a;
    int b;
    double **matsym1;
    double **D;
    double **W;
    double **D1;
    double **D2;
    double **mult;
    matsym1=symC(X,N,d);
    D=ddgC(X,N,d);
    /*cal D1,d2 => D pow -1\2*/
    D1 = (double **) malloc(N * sizeof(double*));
    if(D1==NULL)
    {
        errorC();
        return NULL;
    }
    D2 = (double **) malloc(N * sizeof(double*));
    if(D2==NULL)
    {
        errorC();
        return NULL;
    }
    for (a = 0; a <N; a++) 
    {
        D1[a] = (double *) malloc(N * sizeof(double));
        if(D1[a]==NULL)
        {
            errorC();
            return NULL;
        }
        D2[a] = (double *) malloc(N * sizeof(double));
        if(D2[a]==NULL)
        {
            errorC();
            return NULL;
        }
    }
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            D1[a][b]=0;
            D2[a][b]=D1[a][b];
        }
    }

    for(a=0; a<N; a++)
    {
        D1[a][a]=(1.0/ (sqrt(D[a][a])));
        D2[a][a]=D1[a][a];
    }
    /*fill W*/
    mult=MATmultMAT(D1,matsym1,N,0,0,0);/*normal*/
    W=MATmultMAT(mult,D2,N,0,0,0);/*normal*/
    freefree(D, N);
    freefree(matsym1, N);
    free(D);
    free(matsym1);
    return W;
}

double **AtoAT(double** A,int N,int k)
{
    double **AT;
    int a;
    int b;
    AT=(double **) malloc(k * sizeof(double *));
    if(AT == NULL)
    {
        errorC();
        return NULL;
    }
    for (a = 0; a < k; a++) 
    {
        AT[a] = (double *) malloc(N * sizeof(double));
        if(AT[a]==NULL)
        {
            errorC();
            return NULL;
        }
    }
    for(a=0;a<k;a++)
    {
        for(b=0;b<N;b++)
        {
            AT[a][b]=A[b][a];
        }

    }
    return AT;
}

double Pause_mode(double **H1,double **H2,int N, int k)
{
    double res=0;
    int a;
    int b;
    for(a=0;a<N;a++)
    {
        for(b=0;b<k;b++)
        {
            res+= (double) ((double)(H1[a][b] - H2[a][b]))*((double)(H1[a][b] - H2[a][b]));
        }

    }
    return res;
    
}

double **symnmfC(double **W, double **H0, int N, int k)
{
    double e= 0.0001;
    double bb=0.5;
    int max=300;
    int a;
    int b;
    int iterr=0;
    int con=0;
    double **H1;
    double **WmultH0;
    double **H0multH0T;
    double **H0multH0TmultH0;
    double **H0T;
    H0T=AtoAT(H0,N,k);
    H1 = (double **) malloc(N * sizeof(double*));
    if(H1==NULL)
    {
        errorC();
        return NULL;
    }
    for (a = 0; a < N; a++) 
    {
        H1[a] = (double *) malloc(k * sizeof(double));
        if(H1[a]==NULL)
        {
            errorC();
            return NULL;
        }
    }
    /*update H*/
    while (iterr < max && con == 0)
    {
        WmultH0= MATmultMAT(W,H0,N,N,k,1);
        H0T = AtoAT(H0,N,k);
        H0multH0T= MATmultMAT(H0,H0T,N,k,N,1);
        H0multH0TmultH0= MATmultMAT(H0multH0T,H0,N,N,k,1);
        /*cal H1*/
        for(a=0;a<N;a++)
        {
            for(b=0;b<k;b++)
            {
                H1[a][b]=H0[a][b]*(1.0 - bb + (bb*(WmultH0[a][b]/H0multH0TmultH0[a][b])));
            }
        }
        /*check the condition*/
        if(Pause_mode(H0,H1,N,k)<e)
        {
            con=1;
        }
        /*switch*/
        for(a=0;a<N;a++)
        {
            for(b=0;b<k;b++)
            {
                H0[a][b]=H1[a][b];
            }
        }
        iterr++;
    }

    return  H1;
}

void printM(int N1, int N2, double **A) /*print matrix for me*/
{ 
    int a;
    int b;
    for (a = 0; a < N1; a++) 
    {
        for (b = 0; b< N2- 1; b++) 
        {
            printf("%.4f,", A[a][b]);
        }
        printf("%.4f", A[a][N2 - 1]); /*without the ,*/
        printf("\n");
    }
}

int main(int argc, char *argv[]) 
{
    char *filename;
    char *goal;
    int a;
    int b;
    FILE *f = NULL;
    int N;
    int d;
    char c;
    double **X;
    int casee=0;
    double **finall=NULL;
    d=1;
    N=0;
    if (argc != 3)
    {
        errorC();
        exit(1);
    }
    goal = argv[1];
    filename = argv[2];
    if (strcmp(goal, "sym")!=0
        && strcmp(goal, "ddg")!=0
        && strcmp(goal, "norm")!=0)
    {
        errorC();
    }

    f = fopen(filename, "r");
    if (f == NULL) 
    {
        errorC();
    }
    while ((c = fgetc(f)) != EOF){ 
        if (c == '\n'){
            N++;
        }
        else if ((c==',') && (N==0)){
            d++;
        }
    }
    rewind(f);
    X = malloc(N*sizeof(double*));
    for (a=0; a<N; a++){
        X[a] = malloc(d*sizeof(double));
        for (b=0; b<d; b++){
            if (fscanf(f, "%lf,", &X[a][b]) != 1)
            {
                printf("An Error Has Occured");
                exit(1);
            }
        }
    }
    fclose(f);
    if(X== NULL)
    {
        errorC();
    }

    /*from file*/
    if(strcmp(goal, "sym") == 0) {casee = 1;}
    if(strcmp(goal, "ddg") == 0) {casee = 2;}
    if(strcmp(goal, "norm") == 0) {casee = 3;}
    switch(casee)
    {
        case 1:
            finall=symC(X,N,d);
            printM(N,N,finall); 
            break;
        case 2:
            finall=ddgC(X,N,d);
            printM(N,N,finall); 
            break;
        case 3:
           finall=normC(X,N,d);
            printM(N,N,finall); 
            break;
    }
    return 0;
}
