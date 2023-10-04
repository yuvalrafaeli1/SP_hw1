#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

static void freefree(double **xrr, int l)
{
    int a;
    for (a=0;a<l;a++)
    {
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
    double **X = (double **) malloc(N * sizeof(double *));
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
static PyObject* ddg(PyObject* self, PyObject *args){/*code*/}
static PyObject* norm(PyObject* self, PyObject *args){/*code*/}
static PyObject* symnmf(PyObject* self, PyObject *args){/*code*/}

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

