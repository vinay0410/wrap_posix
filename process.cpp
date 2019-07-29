#include <Python.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <time.h>


void set(std::string key, std::string val);
std::string get(std::string key);

void set(std::string key, std::string val) {

    key = "/" + key;
    const char* ckey = key.c_str();

    int size = val.length();
    sem_t* sem = sem_open(ckey, O_CREAT, 0777, 1);

    if (sem == SEM_FAILED) {
        int err1 = errno;
        throw "Semaphore Couldn't be opened: " + std::to_string(err1);
    }

    int shm_fd = shm_open(ckey, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        throw "Shared Memory Object couldn't be created or opened";
    }

    int rv = ftruncate(shm_fd, size);

    if (rv == -1) {
        throw "Size of Shared Memory object couldn't be changed";
    }

    void* ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED) {
        throw "Shared Memory couldn't be mapped to Virtual Memory";
    }

    char* str = (char*)ptr;

    sem_wait(sem);

    strcpy(str, val.c_str());

    sem_post(sem);

    rv = munmap(ptr, size);

    if (rv == -1) {
        throw "Memory Couldn't be unlinked from Virtual Memory";
    }

    sem_close(sem);
    close(shm_fd);
}

std::string get(std::string key) {
    key = "/" + key;
    const char* ckey = key.c_str();

    sem_t* sem = sem_open(ckey, O_CREAT, 0777, 1);

    if (sem == SEM_FAILED) {
        int err1 = errno;
        throw err1;
    }

    int shm_fd = shm_open(ckey, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        throw "Shared Memory Object couldn't be created or opened";
    }

    struct stat sb;

    if (fstat(shm_fd, &sb) == -1) {
        throw "fstat Failed !";
    }

    void* ptr = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED) {
        throw "Shared Memory couldn't be mapped to Virtual Memory";
    }

    char* str = (char*)ptr;

    std::string temp;

    sem_wait(sem);

    if (strcmp(str, "") != 0)
        temp = std::string(str);

    sem_post(sem);

    int rv = munmap(ptr, sb.st_size);

    if (rv == -1) {
        throw "Memory Couldn't be unlinked from Virtual Memory";
    }

    sem_close(sem);
    close(shm_fd);
    return temp;

}

void clean(std::string key) {
    key = "/" + key;
    const char* ckey = key.c_str();

    shm_unlink(ckey);
}


class SharedSegment {

    public:
        SharedSegment(std::string key, int size) {
            key = "/" + key;
            this->ckey = key.c_str();
            this->sem = sem_open(ckey, O_CREAT, 0777, 1);
            this->size = size;

            if (this->sem == SEM_FAILED) {
                int err1 = errno;
                throw "Semaphore Couldn't be opened: " + std::to_string(err1);
            }

            this->shm_fd = shm_open(this->ckey, O_CREAT | O_RDWR, 0666);

            if (this->shm_fd == -1) {
                throw "Shared Memory Object couldn't be created or opened";
            }

            this->rv = ftruncate(this->shm_fd, this->size);

            if (this->rv == -1) {
                throw "Size of Shared Memory object couldn't be changed";
            }

            this->ptr = mmap(0, this->size, PROT_WRITE, MAP_SHARED, this->shm_fd, 0);

            if (this->ptr == MAP_FAILED) {
                throw "Shared Memory couldn't be mapped to Virtual Memory";
            }

        }

        void unlink() {
            shm_unlink(this->ckey);
        }

        void close_segment() {
            sem_close(this->sem);
            close(this->shm_fd);
            rv = munmap(ptr, size);

            if (rv == -1)
                throw "Memory Couldn't be unlinked from Virtual Memory";

        }

        ~SharedSegment() {
            this->close_segment();
        }

    private:
        const char* ckey;
        sem_t* sem;
        int shm_fd, rv, size;
        void* ptr;

};



/*******************************************************************************************************************************************************
 *  Python Bindings
 ******************************************************************************************************************************************************/


static PyObject * process_clean(PyObject *self, PyObject *args) {
    const char *command;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    clean(std::string(command));
    return Py_BuildValue("");
}

static PyObject * process_get(PyObject *self, PyObject *args) {
    const char *command;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    std::string res = get(std::string(command));
    return Py_BuildValue("s", res.c_str());
}

static PyObject *process_set(PyObject *self, PyObject *args) {
    const char* key, *val;

    if (!PyArg_ParseTuple(args, "ss", &key, &val))
        return NULL;

    set(std::string(key), std::string(val));
    return Py_BuildValue("");
}


PyObject* construct(PyObject* self, PyObject* args) {

    PyObject* sharedSegmentCapsule_;

    PyArg_ParseTuple(args, "O", &sharedSegmentCapsule_);


    SharedSegment* shm = new SharedSegment();

    Foo* foo = new Foo();
    PyObject* fooCapsule = PyCapsule_New((void*)foo, "FooPtr", NULL);
    PyCapsule_SetPointer(fooCapsule, (void *)foo);
    return Py_BuildValue("O", fooCapsule);

}

PyObject* getValue(PyObject* self, PyObject* args) {
   
    Foo* foo = (Foo*)PyCapsule_GetPointer(fooCapsule_, "FooPtr");

    int res = foo->getVal();

    return Py_BuildValue("i", res);
}

static PyMethodDef ProcessMethods[] = {
 { "get", process_get, METH_VARARGS, "Get value coressponding to a key from shared memory" },
 { "set", process_set, METH_VARARGS, "Set value corresponding to key in shared memory"},
 { "clean", process_clean, METH_VARARGS, "Destroy or clean contents corresponding to a particlular key"},
 { NULL, NULL, 0, NULL }
};

PyMethodDef cCarFunctions[] =
{

    {"construct",                   // C++/Py Constructor
      construct, METH_VARARGS,
     "Create `Car` object"},

    {"fuel_up",                     // C++/Py wrapper for `fuel_up`
      getValue, METH_VARARGS,
     "Fuel up car"},

    {NULL, NULL, 0, NULL}      // Last function description must be empty.
                               // Otherwise, it will create seg fault while
                               // importing the module.
};


static struct PyModuleDef processmodule = {
    PyModuleDef_HEAD_INIT,
    "process",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    ProcessMethods
};

PyMODINIT_FUNC
PyInit_process(void)
{
    return PyModule_Create(&processmodule);
}


int main() {

    std::string temp;

    clock_t t;
    t = clock();

    for (int i = 0; i < 100000; i++) {
        try {
            set("data", "abdqwbeujqehujqkahwsdjkwqgherjqwabndjkgqukwjehnkqwbadxukjqwghjkkbjdklbqwuikbjklhgeujkdqwbnadsxjkbquwkgedujkqwbsdjkxhqwuikhbedjkgbqwujkasgxjkqwbeukdsgq");
            temp = get("data");
        } catch (const char* e) {
            std::cout << "came here" << std::endl;
            std::cout << e << std::endl;
            sleep(30);
            exit(1);
        } catch (int e) {
            std::cout << "came here" << std::endl;
            std:: cout << e << std::endl;
            sleep(30);
            exit(1);
        }
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    std::cout << time_taken << std::endl;

    clean("data");

} 