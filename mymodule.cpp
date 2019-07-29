#include <Python.h>
#include <iostream>

class Foo {
    PyObject_HEAD
    public:
        Foo() {
            std::cout << "Empty Constructor Called" << std::endl;
        }
        Foo(int val) {
            this->setup(val);
            std::cout << "Valued Constructor Called " << val << std::endl;
        }

        void setup(int val) {
            this->val = val;
            std::cout << "in setup " << this->val << std::endl;
        }

        int getValue() const {
            std::cout << "in get val " << this->val << std::endl;
            return this->val;
        }
        ~Foo() {
          std::cout << "In C++ destrcutor" << std::endl;
        }

    private:
        int val;
};


static PyObject *Foo_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {

  std::cout << "Allocating Memory" << std::endl;

  Foo *self = (Foo *)type->tp_alloc(type, 0);

  std::cout << self << std::endl;

  return (PyObject *)self;
}

static PyObject* Foo_init(Foo *self, PyObject *args) {
     int val;
    if (!PyArg_ParseTuple(args, "i", &val))
      return NULL;

    self->setup(val);

    return Py_BuildValue("");

}

static void Foo_dealloc(Foo *self) {
  self->~Foo();
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *Foo_get(Foo *self) {
  return Py_BuildValue("i", self->getValue());
}

static PyMethodDef Foo_methods[] = {
    {"get", (PyCFunction)Foo_get, METH_NOARGS, "Get Value"},
    {NULL} /* Sentinel */
};

static PyTypeObject FooType = {
    PyVarObject_HEAD_INIT(NULL, 0) "mymodule.Foo",  /* tp_name */
    sizeof(Foo),                              /* tp_basicsize */
    0,                                        /* tp_itemsize */
    (destructor)Foo_dealloc,                  /* tp_dealloc */
    0,                                        /* tp_print */
    0,                                        /* tp_getattr */
    0,                                        /* tp_setattr */
    0,                                        /* tp_reserved */
    0,                                        /* tp_repr */
    0,                                        /* tp_as_number */
    0,                                        /* tp_as_sequence */
    0,                                        /* tp_as_mapping */
    0,                                        /* tp_hash  */
    0,                                        /* tp_call */
    0,                                        /* tp_str */
    0,                                        /* tp_getattro */
    0,                                        /* tp_setattro */
    0,                                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "Foo objects",                            /* tp_doc */
    0,                                        /* tp_traverse */
    0,                                        /* tp_clear */
    0,                                        /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    0,                                        /* tp_iter */
    0,                                        /* tp_iternext */
    Foo_methods,                              /* tp_methods */
    0,                                        /* tp_members */
    0,                                        /* tp_getset */
    0,                                        /* tp_base */
    0,                                        /* tp_dict */
    0,                                        /* tp_descr_get */
    0,                                        /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    (initproc)Foo_init,                       /* tp_init */
    0,                                        /* tp_alloc */
    Foo_new,                                  /* tp_new */
};

static struct PyModuleDef mymodule_definition = {
    PyModuleDef_HEAD_INIT,
    "mymodule",
    "example module containing Foo class",
    -1,
    NULL,
};

PyMODINIT_FUNC PyInit_mymodule(void) {
  Py_Initialize();
  PyObject *m = PyModule_Create(&mymodule_definition);

  if (PyType_Ready(&FooType) < 0)
    return NULL;

  Py_INCREF(&FooType);
  PyModule_AddObject(m, "Foo", (PyObject *)&FooType);

  return m;
}