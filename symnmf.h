#ifndef SYMNMF_H
#define SYMNMF_H
void errorC();
static void freefree(double **xrr, int N);
double **symC(double **X, int N, int d); /*X={x1,x2,x3,.....xn} , xi=(c1,c2,c3,c4,....,cd)*/
double **ddgC(double **X, int N, int d) ;
double **MATmultMAT(double **A_mat, double **B_mat,int row1,int column1 ,int column2,int P);/*p=0 => normal , p=1 => with colm+row*/
double **normC(double **X,int N, int d);
double **AtoAT(double** A,int N,int k);
double Pause_mode(double **H1,double **H2,int N, int k);
double **symnmfC(double **W, double **H0, int N, int k);
void printM(int N1, int N2, double **A); /*print matrix for me*/
#endif
