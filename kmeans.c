# include <stdio.h>
# include <stdlib.h>
# include <math.h>


struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
    int centroid;
};
 void error(){
        fprintf (stderr, "An Error Has Occurred!\n");
        exit (1);
}
void validate_args(int k, int iter, int N){
    if ((k>=N)||(k<=1)){
        fprintf (stderr, "Invalid number of clusters!\n");
        exit (1); 
    } 
    if ((iter<=1)||(iter>=1000)){
        fprintf (stderr, "Invalid maximum iteration!\n");
        exit (1); 
    }
}
double comp_distance(struct vector *a, double *cent) {
    double sum = 0;
    struct cord *a_cord = a->cords;
    int i = 0;
    while (a_cord != NULL) {
        sum += pow((a_cord->value - cent[i]), 2);
        a_cord = a_cord->next;
        i++;
    }
    return sqrt(sum);
}

double comp_distance_arrays(double *cent_a, double *cent_b, int dim) {
    double sum = 0;
    int i = 0;
    
    while (i < dim) {
        sum += pow((cent_a[i] - cent_b[i]), 2);
        i++;
    }
    
    return sqrt(sum);
}

void assign_centroids(struct vector *head, int k, double **centroids){
    struct vector *current_vector = head;
    while (current_vector != NULL){
        int i;
        for ( i=0 ; i<k; i++){
            if (comp_distance(current_vector, centroids[i]) < (comp_distance(current_vector, centroids[current_vector->centroid])))
                current_vector->centroid = i ;
        }
        current_vector = current_vector->next;
    }
}

double update_centroids(struct vector *head,int dim, int k, double **centroids) {
    double max_change = -1, change;
    int i;
    double *new_i = malloc(sizeof(double)*dim);
    for (i=0; i < k; i++) { /*iterates every centroid*/ 
        
        int cnt = 0,j,a;
        struct vector *current_vector = head;
        struct cord *current_cord;
        
        while (current_vector!= NULL) {
            if (current_vector->centroid == i) {
                cnt++;
                current_cord = current_vector->cords;
                for ( j = 0; j < dim; j++) {
                    new_i[j] += current_cord->value;
                    current_cord = current_cord->next;
                }
            }
            current_vector = current_vector->next;
        }
        if(cnt!=0){
            for ( a = 0; a < dim; a++) {
                new_i[a] = new_i[a] / cnt; /*calculating average*/ 
            }
        }
        change = comp_distance_arrays(centroids[i], new_i, dim);
        
        /*Update centroids*/ 
        for (a = 0; a < dim; a++) {
            centroids[i][a] = new_i[a];
            new_i[a] = 0.0;
        }
        
        if ((max_change < 0) || (change > max_change))
            max_change = change;
    }
    free(new_i);
    return max_change;
}

void free_all(struct vector *head, int k, double **centroids){
    struct vector *current_vector = head;
    struct vector *temp_vec;
    int i;
    while (current_vector != NULL) {
        struct cord *current_cord = current_vector->cords;
        while (current_cord != NULL){
            struct cord *temp = current_cord;
            current_cord = current_cord->next;
            free(temp);
        }
        temp_vec = current_vector;
        current_vector = current_vector->next;
        free(temp_vec);
    }
    for (i=0; i<k; i++){
        free(centroids[i]);
    }
    free (centroids);
}


int main (int argc, char *argv[]) {
    int k, iter, N=0 , dim, d=1, i, j, curr_iter;
    double n, eps, max_change;
    char c;
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    double **centroids;

    k = atoi (argv[1]);
    if (argc == 3) 
        iter = atoi (argv[2]);
    else if(argc == 2)
        iter = 200;
    else
         error();
        
                                /*initial 2d linked lists as shown in class*/
    
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;



    while (scanf("%lf%c", &n, &c) == 2){
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;    
            if (N==0)                
                dim = d;               /*checking there are no different dimensions*/ 
            else if (d != dim)
               error();
            d=1;                      

            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL; 
            N+=1;
            continue;
        }
        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        d+=1;
    }
    free(curr_cord);
    curr_vec = head_vec;
    while(curr_vec->next->cords != NULL){
        curr_vec = curr_vec->next;
    }
    free(curr_vec->next);
    curr_vec->next = NULL;


    validate_args(k, iter, N);         /*check that args are vaild*/
                                    /*creating 2D centroids array*/
    centroids = (double **)malloc(sizeof(double *) * k);
    for (i=0; i<k; i++){
        double *centroid = (double *)malloc(sizeof(double) * dim);
        centroids[i] = centroid;
    }

                                    /*inserting the first k vectors into centroids*/
    curr_vec = head_vec;
    for (i = 0; i < k; i++) {
        curr_cord = curr_vec->cords;
        for (j = 0; j < dim; j++) {
                centroids[i][j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
                                        /*running the algorithm*/
    curr_iter = 1;
    eps = 0.001;
    assign_centroids(head_vec, k, centroids);
    max_change = update_centroids(head_vec, dim, k, centroids);

    while ((max_change>=eps)&&(curr_iter<iter)){
        curr_iter++;
        assign_centroids(head_vec, k, centroids);
        max_change = update_centroids(head_vec, dim, k, centroids);
    }
                                    /* printing the result*/
    for(i=0; i<k; i++){
        for (j=0; j<dim; j++){
            if(j==(dim-1))
                printf("%.4f\n", centroids[i][j]);
            else
                printf("%.4f%s", centroids[i][j], ",");
        }
    }
    free_all(head_vec, k,  centroids);
    exit(0);
}
