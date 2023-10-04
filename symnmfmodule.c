#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

static void freefree(double **xrr, int l){
    int a;
    for (a=0;a<l;a++){
        free(xrr[a]);
    }
}

void error()
{
        fprintf (stderr, "An Error Has Occurred!\n");
}

static PyObject* sym(PyObject* self, PyObject *args)
{
    int N;
    int d;
    double **X;
    double **finalsym;
    int a,b;
    PyObject *pythonX;
    PyObject *xi;
    PyObject *xij;
    PyObject *python_finalsym;
    if (!PyArg_ParseTuple(args, "Oii", &pythonX, &N, &d))
        return NULL;
    if (!PyList_Check(pythonX))
        return NULL;
    X = (double **) malloc(N * sizeof(double *));
    if (X == NULL)
    {
        error();
        return NULL;
    }
    /*fill X from python*/
    for (a = 0; a < N; a++) 
    {
        xi = PyList_GetItem(pythonX, a);
        if (!PyList_Check(xi) || PyList_Size(xi) != d)
        {
            error();
            return NULL;
        }
        X[a] = (double *) malloc(N * sizeof(double));
        if (X[a] == NULL)
        {
            error();
            return NULL;
        }
        for(b=0;b<d;b++)
        {
            xij = PyList_GetItem(xi, b);
            if (!PyFloat_Check(xij))
            {
                error();
                return NULL;
            }
            X[a][b] = PyFloat_AsDouble(xij);
        }
    }
    finalsym=sym(X,N,d);
    freefree(X);
    free(X);
    if(finalsym==NULL)
    {
        error();
        return NULL;
    }
    python_finalsym= PyList_New(N);
    for (a = 0; a < N; a++) 
    {
        xi = PyList_New(N);
        for (b = 0; b< N; b++)
         {
            PyList_SetItem(xi, b, PyFloat_FromDouble(finalsym[a][b]));
        }
        PyList_SetItem(python_finalsym, a, xi);
    }
    freefree(finalsym);
    free(finalsym);
    return python_finalsym;
}
static PyObject* ddg(PyObject* self, PyObject *args)
{
    int N;
    int d;
    double **X;
    double **finalddg;
    int a,b;
    PyObject *pythonX;
    PyObject *xi;
    PyObject *xij;
    PyObject *python_finalddg;
    if (!PyArg_ParseTuple(args, "Oii", &pythonX, &N, &d))
        return NULL;
    if (!PyList_Check(pythonX))
        return NULL;
    X = (double **) malloc(N * sizeof(double *));
    if (X == NULL)
    {
        error();
        return NULL;
    }
    /*fill X from python*/
    for (a = 0; a < N; a++) 
    {
        xi = PyList_GetItem(pythonX, a);
        if (!PyList_Check(xi) || PyList_Size(xi) != d)
        {
            error();
            return NULL;
        }
        X[a] = (double *) malloc(N * sizeof(double));
        if (X[a] == NULL)
        {
            error();
            return NULL;
        }
        for(b=0;b<d;b++)
        {
            xij = PyList_GetItem(xi, b);
            if (!PyFloat_Check(xij))
            {
                error();
                return NULL;
            }
            X[a][b] = PyFloat_AsDouble(xij);
        }
    }
    finalddg=ddg(X,N,d);
    freefree(X);
    free(X);
    if(finalddg==NULL)
    {
        error();
        return NULL;
    }
    python_finalddg= PyList_New(N);
    for (a = 0; a < N; a++) 
    {
        xi = PyList_New(N);
        for (b = 0; b< N; b++)
         {
            PyList_SetItem(xi, b, PyFloat_FromDouble(finalddg[a][b]));
        }
        PyList_SetItem(python_finalddg, a, xi);
    }
    freefree(finalddg);
    free(finalddg);
    return python_finalddg;
}
static PyObject* norm(PyObject* self, PyObject *args)
{
    int N;
    int d;
    double **X;
    double **finalnorm;
    int a,b;
    PyObject *pythonX;
    PyObject *xi;
    PyObject *xij;
    PyObject *python_finalnorm;
    if (!PyArg_ParseTuple(args, "Oii", &pythonX, &N, &d))
        return NULL;
    if (!PyList_Check(pythonX))
        return NULL;
    X = (double **) malloc(N * sizeof(double *));
    if (X == NULL)
    {
        error();
        return NULL;
    }
    /*fill X from python*/
    for (a = 0; a < N; a++) 
    {
        xi = PyList_GetItem(pythonX, a);
        if (!PyList_Check(xi) || PyList_Size(xi) != d)
        {
            error();
            return NULL;
        }
        X[a] = (double *) malloc(N * sizeof(double));
        if (X[a] == NULL)
        {
            error();
            return NULL;
        }
        for(b=0;b<d;b++)
        {
            xij = PyList_GetItem(xi, b);
            if (!PyFloat_Check(xij))
            {
                error();
                return NULL;
            }
            X[a][b] = PyFloat_AsDouble(xij);
        }
    }
    finalnorm=norm(X,N,d);
    freefree(X);
    free(X);
    if(finalnorm==NULL)
    {
        error();
        return NULL;
    }
    python_finalnorm= PyList_New(N);
    for (a = 0; a < N; a++) 
    {
        xi = PyList_New(N);
        for (b = 0; b< N; b++)
         {
            PyList_SetItem(xi, b, PyFloat_FromDouble(finalnorm[a][b]));
        }
        PyList_SetItem(python_finalnorm, a, xi);
    }
    freefree(finalnorm);
    free(finalnorm);
    return python_finalnorm;
}
static PyObject* symnmf(PyObject* self, PyObject *args)
{
    int N;
    int d;
    int k;
    double **W;
    double **H;
    double **finalH;
    int a,b;
    PyObject *pythonW;
    PyObject *pythonH;
    PyObject *xi;
    PyObject *xij;
    PyObject *python_finalH;
    if (!PyArg_ParseTuple(args, "OOiii", &pythonW, &pythonH, &N, &d, &k))
        return NULL;
    if (!PyList_Check(pythonW))
        return NULL;
    if (!PyList_Check(pythonH))
        return NULL;
    W = (double **) malloc(N * sizeof(double *));
    H = (double **) malloc(N * sizeof(double *));
    if (W == NULL)
    {
        error();
        return NULL;
    }
    if (H == NULL)
    {
        error();
        return NULL;
    }
    /*fill W from python*/
    for (a = 0; a < N; a++) 
    {
        xi = PyList_GetItem(pythonW, a);
        if (!PyList_Check(xi) || PyList_Size(xi) != d)
        {
            error();
            return NULL;
        }
        W[a] = (double *) malloc(N * sizeof(double));
        if (W[a] == NULL)
        {
            error();
            return NULL;
        }
        for(b=0;b<d;b++)
        {
            xij = PyList_GetItem(xi, b);
            if (!PyFloat_Check(xij))
            {
                error();
                return NULL;
            }
            W[a][b] = PyFloat_AsDouble(xij);
        }
    }
    /*fill H from python*/
    for (a = 0; a < N; a++) 
    {
        xi = PyList_GetItem(pythonH, a);
        if (!PyList_Check(xi) || PyList_Size(xi) != d)
        {
            error();
            return NULL;
        }
        H[a] = (double *) malloc(N * sizeof(double));
        if (H[a] == NULL)
        {
            error();
            return NULL;
        }
        for(b=0;b<d;b++)
        {
            xij = PyList_GetItem(xi, b);
            if (!PyFloat_Check(xij))
            {
                error();
                return NULL;
            }
            H[a][b] = PyFloat_AsDouble(xij);
        }
    }
    finalH=symnmf(W,H,N,d,k);
    freefree(W);
    free(W);
    freefree(H);
    free(H);
    if(finalH==NULL)
    {
        error();
        return NULL;
    }
    python_finalH= PyList_New(N);
    for (a = 0; a < N; a++) 
    {
        xi = PyList_New(N);
        for (b = 0; b< N; b++)
         {
            PyList_SetItem(xi, b, PyFloat_FromDouble(finalH[a][b]));
        }
        PyList_SetItem(python_finalH, a, xi);
    }
    freefree(finalH);
    free(finalH);
    return python_finalH;
}

static PyMethodDef symnmfMethods[] = {
    {"sym",(PyCFunction) sym, METH_VARARGS, PyDoc_STR("get similarity matrix")},
    {"ddg",(PyCFunction) ddg, METH_VARARGS, PyDoc_STR(" get diagonal degree matrix")}, 
    {"norm",(PyCFunction) norm, METH_VARARGS, PyDoc_STR(" get normalized similarity matrix")},
     {"symnmf",(PyCFunction) symnmf, METH_VARARGS, PyDoc_STR(" get the final H")},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef defmodule = { PyModuleDef_HEAD_INIT, "symnmf",  NULL,   -1,    symnmfMethods};

PyMODINIT_FUNC PyInit_symnmf(void)
{
    PyObject *m;
    m = PyModule_Create(&defmodule);
    if (!m) {
        return NULL;
    }
    return m;
}


