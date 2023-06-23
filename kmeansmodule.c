#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct
{
    double *centroid;
    double *pointsum;
    int pointsNUM;
} cluster;

static void freefree(double **xrr, int l){
    int a;
    for (a=0;a<l;a++){
        free(xrr[a]);
    }
}


static int assign_centroids(double* p, cluster* clusters, int k, int size) {
    int clusterforminvalue = -1;
    double *centroidforclus;
    double dis;
    double minv = dBL_MAX; /* in <float.h> */
    int i;
    for (i = 0 ; i < k ; i++) {
        centroidforclus = clusters[i].centroid;
        dis = caldistance(centroidforclus, p, size);
        if (dis < minv) {
             minv = dis;
            clusterforminvalue = i;}  }
    return clusterforminvalue;
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

static int whichcluster(cluster* clusters, double* points, int k, int N, int size) {
    double *p;
    int mostclosest;
    int a;
    int b;
    p = (double*) calloc (size, sizeof(double));
    if (p == NULL){
        printf("An Error Has Occurred");
        return 0;}
    for (a = 0; a < k; a++) {
        for (b = 0; b < size; b++)
        {
            clusters[a].pointsum[b] = 0.0 ;
        }
        clusters[a].pointsNUM = 0;
    }
    for (a=0; a < N; a++){
        for (b = 0; b < size; b++){
            p[b] = points[a*size + b];
        }
        mostclosest = assign_centroids(p, clusters, k, size);
        clusters[mostclosest].pointsNUM += 1 ;
        for (b = 0; b < size; b++){
            clusters[mostclosest].pointsum[b] += p[b] ;
        }
    }
    free(p);
    return 1;
}

static int conversion(cluster *clusters, double * previouscent, int k, int vecLENGTH, double e){
    double v;
    int a;
    int b;
    for (a = 0; a < k; a++){
        for (b = 0; b < vecLENGTH; b ++){
            v = fabs(previouscent[a * vecLENGTH + b] - clusters[a].centroid[b]);
            if (v > e){return 0;}} }
    return 1;
}


static void updatee(cluster* clusters, int k, int size) { /*update for centroids*/
    int a;
    int b;
    double f;
    for (a=0; a < k; a++){
        for (b=0 ; b < size; b++)
        {
            f=clusters[a].pointsum[b] / clusters[a].pointsNUM
            clusters[a].centroid[b] = f;
        }

    }

}

void error(){
        fprintf (stderr, "An Error Has Occurred!\n");
}


static double **mainkmeans(double *points, double **centroids, int linecnt, int size, int k, int max, double e)
{
    int iterr,con;
    iterr = 0;
    cluster *clusters;
    Con = 0;
    int f,i,j,mm;
    double *oldP;
    oldP = (double*) calloc(k * size, sizeof(double));
    if(oldP==NULL){
        error();
        return NULL;}
    clusters = (cluster*) calloc(k, sizeof(cluster));
    if(clusters==NULL){
        error();
        return NULL;}
    for (i = 0; i < k; i++) {
        clusters[i].centroid = (double*) calloc(size, sizeof(double));
        if(clusters[i].centroid==NULL){
            error();
            return NULL;}
        clusters[i].pointsum = (double*) calloc(size, sizeof(double));
        if(clusters[i].pointsum==NULL){
            error();
            return NULL;}
        for (j = 0; j < size; j ++){
            clusters[i].centroid[j] = centroids[i][j]; 
            clusters[i].pointsum[j] = centroids[i][j]; }
        clusters[i].pointsNUM = 1;}
    while (iterr < max && Con == 0) {
        f = whichcluster(clusters, points, k, linecnt , size);
        if (f == 0){
            return NULL; }
        for (i = 0; i < k; i ++){
            for (j = 0; j < size; j++){
                oldP[i * size + j] = clusters[i].centroid[j];}}
        updatee(clusters, k, size);
        Con = conversion(clusters, oldP, k, size, e);
        iterr++;}
    for(i=0; i < k; i++){
        for(j = 0; j < size; j++){
            centroids[i][j]=clusters[i].centroid[j];}}
    for(mm=0;mm<k;mm++){
        free(clusters[mm].centroid);
        free(clusters[mm].pointsum);}
    free(clusters);
    free(oldP);
    return centroids;
}

static PyObject* fit(PyObject* self, PyObject *args)
{
    int k, n, d, max_iter, i, j, castingfailed;
    double *points, **centroids, **finalcentroids;
    double epsilon;
    PyObject *pythonCentroids, *pythonVectors, *vector, *centroid, *coordinate, *pythonFinalCentroids, *pythonCentroid;
    castingfailed = 0;
    if (!PyArg_ParseTuple(args, "OOiiiid", &pythonCentroids, &pythonVectors, &max_iter, &n, &d, &k, &epsilon))
        return NULL;
    if (!PyList_Check(pythonCentroids))
        return NULL;
    if (!PyList_Check(pythonVectors))
        return NULL;
    points = (double*) calloc(n *d, sizeof(double));
    if(points==NULL){
        error();
        return NULL;}
    centroids = (double **)calloc(k, sizeof(double *));
    if(centroids==NULL){
        error();
        return NULL;}
    for (i = 0; i < k; i++){
        centroids[i] = (double *)calloc(d, sizeof(double));
        if(centroids[i]==NULL){
            error();
            return NULL;
        }
    }
    for (i = 0; i < n; i++) {
        vector = PyList_GetItem(pythonVectors, i);
        if (!PyList_Check(vector)){
            continue;
        }
        for (j = 0; j < d; j++) {
            coordinate = PyList_GetItem(vector, j);
            points[i*d + j] = PyFloat_AsDouble(coordinate); 
            if (PyErr_Occurred() && points[i*d + j]  == -1.0){
                castingfailed = 1;
                break;
            }
        }
        if (castingfailed == 1){
            break;
        }
    }
    for (i = 0; i < k; i++) {
        centroid = PyList_GetItem(pythonCentroids, i);
        if (!PyList_Check(centroid)){
            continue;
        }
        for (j = 0; j < d; j++) {
            coordinate = PyList_GetItem(centroid, j);
            centroids[i][j] = PyFloat_AsDouble(coordinate); 
            if (PyErr_Occurred() && centroids[i][j]  == -1.0){
                castingfailed = 1;
                break;
            }
        }
        if (castingfailed == 1){
            break;
        }
    }
    finalcentroids = NULL;
    if (castingfailed == 0){
        finalcentroids = mainkmeans(points, centroids, n, d, k, max_iter, epsilon);
        if (finalcentroids == NULL)
        {
            return NULL;
        }
    }
    pythonFinalCentroids = PyList_New(k);
    if (pythonFinalCentroids == NULL)
        return NULL;
    
    for (i = 0; i < k; i++)
    {
        pythonCentroid = PyList_New(d);
        if (pythonCentroid == NULL)
            return NULL;
        for (j = 0; j < d; j++)
        {
            PyList_SET_ITEM(pythonCentroid, j, Py_BuildValue("d", finalcentroids[i][j]));
        }
        PyList_SetItem(pythonFinalCentroids, i, Py_BuildValue("O", pythonCentroid));
    }
    free(points);
    freefree(centroids,k);
    free(centroids);
    return pythonFinalCentroids;
}

static PyMethodDef kmeansMethods[] = {
    {"fit",                   /* the Python method name that will be used */
      (PyCFunction) fit, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters
accepted for this function */
      PyDoc_STR("A C code that implement kmeans++ and The function receives from Python: two arrays of centroids and vectors and the max iteration(int) and Number of required clusters(k) and N(int) and d(int) and epsilon(double)")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
}; 

static struct PyModuleDef defmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
   kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&defmodule);
    if (!m) {
        return NULL;
    }
    return m;
}
