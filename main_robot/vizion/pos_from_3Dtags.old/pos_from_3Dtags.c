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
  int len = 0;
  for(point_list* l = obs_list; l; l = l->next) len++;
  free_point_list(obs_list);
  obs_list = NULL;
  if (obs_vect_array) {
    for (int i = 0; i < len; i++) free(obs_vect_array[i]);
    free(obs_vect_array);
  }
  
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

  nb_vect++;

  return Py_BuildValue("");
}

static PyObject* py_flush_th_point(PyObject* self, PyObject* args){
  free_point_list(th_list);
  th_list = NULL;
  if (th_vect_array) {
    for (int i = 0; i < nb_vect; i++) free(th_vect_array[i]);
    free(th_vect_array);
  }
  nb_vect = 0;

  return Py_BuildValue("");
}

static PyObject* get_th_point_nb(PyObject* self, PyObject* args){
  return PyLong_FromLong(nb_vect);
}

static PyObject* py_compute(PyObject* self, PyObject* args){
  compute();
  PyObject* py_final_pos[6];
  for (int i = 0; i < 3; i++)
    py_final_pos[i] = PyFloat_FromDouble(final_rotation[i]);
  for (int i = 0; i < 3; i++)
    py_final_pos[i + 3] = PyFloat_FromDouble(final_position[i]);
  return PyTuple_Pack(6,
    py_final_pos[0], py_final_pos[1], py_final_pos[2],
    py_final_pos[3], py_final_pos[4], py_final_pos[5]
  );
}

static PyMethodDef pft_methods[] = {
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

  {"compute", py_compute, METH_VARARGS,
    "Compute right position, returns (rx, ry, rz, x, y, z)"},
  // Rotation is the blender's XYZ
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pft_module = {
  PyModuleDef_HEAD_INIT,
  "pos_from_3Dtags",
  NULL,
  -1,
  pft_methods
};

PyMODINIT_FUNC PyInit_pos_from_3Dtags(void){
  return PyModule_Create(&pft_module);
}

