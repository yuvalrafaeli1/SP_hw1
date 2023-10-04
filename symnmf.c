#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void print_matrix(double **matrix, int row, int col) { /*print the matrix for me only*/
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col - 1; j++) {
            printf("%.4f,", matrix[i][j]);
        }

        printf("%.4f", matrix[i][col - 1]);
        printf("\n");
    }
}

void error()
{
        fprintf (stderr, "An Error Has Occurred!\n");
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

double **sym(double **X, int N, int d) /*X={x1,x2,x3,.....xn} , xi=(c1,c2,c3,c4,....,cd)*/
{
    int a;
    int b;
    int val;
    double **A;
    A = (double **) malloc(N * sizeof(double *));
    if(A == NULL)
    {
        error();
        return NULL;
    }
    for (a = 0; a < N; a++) 
    {
        A[a] = (double *) malloc(N * sizeof(double));
        if(A[a]==NULL)
        {
            error();
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
                A[a][b] = 0;
            }
        }
    }
    return A;
}

double **ddg(double **X, int N, int d) 
{
    int a;
    int b;
    double **matsym;
    double **D;
    double di;
    matsym = sym(X, N, d);
    D = (double **) malloc(N * sizeof(double *));
    if(D == NULL)/*not sure*/
    {
        error();
        return NULL;
    }
    for (a = 0; a < N ; a++) 
    {
        D[a] = (double *) calloc(N, sizeof(double));
        if(D[a]==NULL)/*not sure*/
        {
            error();
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
        D[i][i]=di;
    }
    freefree(matsym);
    free(matsym);
    return D;
}

double **mat_multiply_plus(double **A_mat, double **B_mat,int row1,int column1 ,int column2,int P)/*p=0 => normal , p=1 => with colm+row*/
{
    int i, j, z;
    int N;
    double **resultMat;
    if (p==0)
    {
        N=row1;
        resultMat = (double **) malloc(N * sizeof(double*));
        if(resultMat==NULL)
        {
            error();
            return NULL;
        }
        for (i = 0; i < N; i++) 
        {
            resultMat[i] = (double *) malloc(N * sizeof(double));
            if(resultMat[i]==NULL)
            {
                error();
                return NULL;
            }
        }
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                resultMat[i][j] = 0;
                for (z = 0; z < N; z++) 
                {
                    resultMat[i][j] += A_mat[i][z] * B_mat[z][j];
                }
            }
        }
        return resultMat;
    }
    else
    {
        resultMat = (double **) malloc(row1 * sizeof(double*));
        if(resultMat==NULL)
        {
            error();
            return NULL; 
        }
        for (i = 0; i < row1; i++) 
        {
            resultMat[i] = (double *) malloc(column2 * sizeof(double));
            if(resultMat[i]==NULL)
            {
                error();
                return NULL;  
            }
        }
        for (i = 0; i < row1; i++)
        {
            for (j = 0; j < column2; j++) 
            {
                resultMat[i][j] = 0;
                for (z = 0; z < column1; z++) 
                {
                    resultMat[i][j] += A_mat[i][z] * B_mat[z][j];
                }
            } 
        }
        return resultMat;
    }
    
}

double **norm(double **X,int N, int d)
{
    int a;
    int b;
    double **matsym1;
    double **D;
    double **matsym2;
    double **W;
    double **D1;
    double **D2;
    double **mult;
    matsym1=sym(X,N,d);
    matsym2=sym(X,N,d);/*instead of copy it*/
    D=ddg(matsym2,N,d);
    /*cal D1,d2 => D pow -1\2*/
    D1 = (double **) malloc(PointCount * sizeof(double*));
    if(D1==NULL)
    {
        error();
        return NULL;
    }
    D2 = (double **) malloc(PointCount * sizeof(double*));
    if(D2==NULL)
    {
        error();
        return NULL;
    }
    for (a = 0; a <N; a++) 
    {
        D1[a] = (double *) malloc(N * sizeof(double));
        if(D1[a]==NULL)
        {
            error();
            return NULL;
        }
        D2[a] = (double *) malloc(N * sizeof(double));
        if(D2[a]==NULL)
        {
            error();
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
    mult=mat_multiply_plus(D1,matsym1,N,0,0,0);
    W=mat_multiply_plus(mult,D2,N,0,0,0);
    freefree(D);
    freefree(matsym1);
    freefree(matsym2);
    free(D);
    free(matsym1);
    free(matsym2);
    return W;
}

double **AtoAT(double** A,int N)
{
    double **AT;
    int a;
    int b;
    AT=(double **) malloc(N * sizeof(double *));
    if(AT == NULL)
    {
        error();
        return NULL;
    }
    for (a = 0; a < N; a++) 
    {
        AT[a] = (double *) malloc(N * sizeof(double));
        if(AT[a]==NULL)
        {
            error();
            return NULL;
        }
    }
    for(a=0;a<N;a++)
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

double **symnmf(double **X, double **H0, int N, int d, int k)
{
    double e= 1*exp(-4);
    double beta=0.5;
    double helpp;
    int max=300;
    int a;
    int b;
    int iterr=0;
    int con=0;
    double **H1;
    double **W;
    double **WmultH0;
    double **H0multH0T;
    double **H0multH0TmultH0;
    double **H0T;
    W=norm(X,N,d);
    H0T=AtoAT(H0,N);
    H1 = (double **) malloc(N * sizeof(double*));
    if(H1==NULL)
    {
        error();
        return NULL;
    }
    for (a = 0; a < N; a++) 
    {
        H1[a] = (double *) malloc(k * sizeof(double));
        if(H1[a]==NULL)
        {
            error();
            return NULL;
        }
    }
    /*update H*/
    while (iterr < max && con == 0)
    {
        WmultH0= mat_multiply_plus(W,H0,N,N,k,1);
        H0multH0T= mat_multiply_plus(H0,H0T,N,k,N,1);
        H0multH0TmultH0= mat_multiply_plus(H0multH0T,H0,N,N,k,1);
        /*cal H1*/
        for(a=0;a<N;a++)
        {
            for(b=0;b<k;b++)
            {
                helpp= 1 - beta + (beta*(WmultH0[a][b]/H0multH0TmultH0[a][b]));
                H1[a][b]=H0[a][b]*helpp;
            }
        }
        /*check the condition*/
        if(Pause_mode(H0,H1,N,k)>e)
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

/*without the main and calc the dimension and pointsnumber */
