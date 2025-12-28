#include "core.h"

#define PY_SSIZE_T_CLEAN
//#define Py_LIMITED_API 0x030D0000
#include <Python.h>


static PyObject* add_obs_point(PyObject* self, PyObject* args){
  float x, y, z;

  if (!PyArg_ParseTuple(args, "fff", &x, &y, &z)) {
    perror("Error: add_obs_point: cannot read args.");
    return NULL;
  }

  point_list new_one = {x, y, z, obs_list};
  obs_list = malloc(sizeof(point_list));
  *obs_list = new_one;

  return Py_BuildValue("");
}

static PyObject* py_flush_obs_point(PyObject* self, PyObject* args){
  free_point_list(obs_list);
  obs_list = NULL;
  
  return Py_BuildValue("");
}

static PyObject* get_obs_point_nb(PyObject* self, PyObject* args){
  int count = 0;
  for(point_list* l = obs_list; l; l = l->next) count ++;

  return PyLong_FromLong(count);
}

static PyObject* add_th_point(PyObject* self, PyObject* args){
  float x, y, z;

  if (!PyArg_ParseTuple(args, "fff", &x, &y, &z)) {
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

static PyObject* get_final_rotation_matrix(PyObject* self, PyObject* args){
  PyObject* py_final_rot_mat[9];
  for (int i = 0; i < 9; i++)
    py_final_rot_mat[i] =
      PyFloat_FromDouble(final_rotation_matrix[i]);
  return PyTuple_Pack(3,
    py_final_rot_mat[0], py_final_rot_mat[1], py_final_rot_mat[2],
    py_final_rot_mat[3], py_final_rot_mat[4], py_final_rot_mat[5],
    py_final_rot_mat[6], py_final_rot_mat[7], py_final_rot_mat[8]
  );
}

static PyMethodDef find_rot_methods[] = {
  {"add_th_point", add_th_point, METH_VARARGS,
    "Add a point in the set of knwon point in space."},
  {"flush_th_point", py_flush_th_point, METH_VARARGS,
    "Reset set of knwon point."},
  {"get_th_point_nb", get_th_point_nb, METH_VARARGS,
    "Get the number of known point."},

  {"add_obs_point", add_obs_point, METH_VARARGS,
    "Add a point in the set of observed point in space."},
  {"flush_obs_point", py_flush_obs_point, METH_VARARGS,
    "Reset set of observed point."},
  {"get_obs_point_nb", get_obs_point_nb, METH_VARARGS,
    "Get the number of observed point."},
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

