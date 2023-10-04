/*symnmf.h: C header file*/
void error();
static void freefree(double **xrr, int N);
static double caldistance(double *vec1, double *vec2, int dd);
double **sym(double **X, int N, int d); /*X={x1,x2,x3,.....xn} , xi=(c1,c2,c3,c4,....,cd)*/
double **ddg(double **X, int N, int d) ;
double **MATmultMAT(double **A_mat, double **B_mat,int row1,int column1 ,int column2,int P);/*p=0 => normal , p=1 => with colm+row*/
double **norm(double **X,int N, int d);
double **AtoAT(double** A,int N);
double Pause_mode(double **H1,double **H2,int N, int k);
double **symnmf(double **W, double **H0, int N, int d, int k);
int Ncount(FILE *F);
int dcount(FILE *F);
void printM(int N1, int N2, double **A); /*print matrix for me*/
