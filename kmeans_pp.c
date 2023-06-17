# include <stdio.h>
# include <stdlib.h>
# include <math.h>

/*this function need to recive = dim(int), N(int), k (int), iter(int), eps(double), centroids ([][]double), vec_list([][]double)*/

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

void assign_centroids(int N, int dim, double **vectors, int k, double **centroids, int *cent_index){ /*changed to arrays*/
    int j;
    while (j<N){
        int i;
        for ( i=0 ; i<k; i++){
            if (comp_distance_arrays(vectors[j], centroids[i], dim) < (comp_distance_arrays(vectors[j], centroids[cent_index[j]], dim)))
                centroids[cent_index[j]] = i ;
        }
        j++;
    }
}

double update_centroids(int N,double **vectors,int dim, int k, double **centroids,int *cent_index) { /*changed to arrrays*/
    double max_change = -1, change;
    int i;
    double *new_i = malloc(sizeof(double)*dim);
    for (i=0; i < k; i++) { /*iterates every centroid*/ 
        
        int cnt = 0,j,a,s;
        for (s=0; s < N; s++) {
            if (cent_index[s] == i) {
                cnt++;
                for ( j = 0; j < dim; j++) {
                    new_i[j] += vectors[s][j];
                }
            }
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
    int k, iter, N , dim, d=1, i, j, curr_iter;
    double n, eps, max_change;
    char c;
    double **centroids, **vectors;
    int *cent_index;

    centroids = (double **)malloc(sizeof(double *) * k);
    for (i=0; i<k; i++){
        double *centroid = (double *)malloc(sizeof(double) * dim);
        centroids[i] = centroid;
    }
                                    /*need to set cent_index to N zeros*/
                                    /*need to insert the centroids from python*/
                                    /*need to update N, dim, k, iter, eps, vectors from python*/

                                    
    curr_iter = 1;
    assign_centroids(N, dim, vectors, k, centroids, cent_index);
    max_change = update_centroids(N,vectors,dim ,k, centroids, cent_index);

    while ((max_change>=eps)&&(curr_iter<iter)){
        curr_iter++;
        assign_centroids(N, dim, vectors, k, centroids, cent_index);
        max_change = update_centroids(N,vectors,dim ,k, centroids, cent_index);
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
