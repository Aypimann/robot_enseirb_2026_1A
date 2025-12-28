#include "core.h"

#define PY_SSIZE_T_CLEAN
//#define Py_LIMITED_API 0x030D0000
#include <Python.h>

point_list* th_list = NULL;


static PyObject* add_th_point(PyObject* self, PyObject* args){
  float x, y, z;

  if (!PyArg_ParseTuple(args, "ddd", &x, &y, &z)) {
    perror("Error: add_th_point: cannot read args.");
    return NULL;
  }

  point_list new_one = {x, y, z, th_list};
  th_list = malloc(sizeof(point_list));
  *th_list = new_one;

  return Py_BuildValue("");
}

static PyObject* py_flush_th_point(PyObject* self, PyObject* args){
  free_point_list(th_list);
  th_list = NULL;
  
  return Py_BuildValue("");
}

static PyObject* get_th_point_nb(PyObject* self, PyObject* args){
  int count = 0;
  for(point_list* l = th_list; l; l = l->next) count ++;

  return PyLong_FromLong(count);
}

static PyMethodDef find_rot_methods[] = {
  {"add_th_point", add_th_point, METH_VARARGS,
    "Add a point in the set of knwon point in space."},
  {"flush_th_point", py_flush_th_point, METH_VARARGS,
    "Reset set of knwon point."},
  {"get_th_point_nb", get_th_point_nb, METH_VARARGS,
    "Get the number of known point."},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef find_rot_module = {
  PyModuleDef_HEAD_INIT,
  "find_rot",
  NULL,
  -1,
  find_rot_methods
};

PyMODINIT_FUNC PyInit_find_rot(void){
  return PyModule_Create(&find_rot_module);
}

