# include <stdio.h>
# include <stdlib.h>
# include <math.h>

/*this function need to recive = k (int), iter(int), eps(double), centroids ([][]double), vec_list([][]double)*/

 void error(){
        fprintf (stderr, "An Error Has Occurred!\n");
        exit (1);
}


double comp_distance_arrays(double *cent_a, double *cent_b, int dim) {  /*calculate distance between vectors in arrays*/
    double sum = 0;
    int i = 0;
    
    while (i < dim) {
        sum += pow((cent_a[i] - cent_b[i]), 2);
        i++;
    }
    
    return sqrt(sum);
}

void assign_centroids(struct vector *head, int k, double **centroids){ /*need to change to arrays*/
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

double update_centroids(struct vector *head,int dim, int k, double **centroids) { /*need to change to arrays*/
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

void free_all(struct vector *head, int k, double **centroids){ /*need to change to arrays*/
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

    




    centroids = (double **)malloc(sizeof(double *) * k);
    for (i=0; i<k; i++){
        double *centroid = (double *)malloc(sizeof(double) * dim);
        centroids[i] = centroid;
    }

                                    /*need to insert the centroids from python*/
                                    
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
    return(0);
}
