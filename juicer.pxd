from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "FruitJuicer.h" namespace "juicer":
    ctypedef void (*pyVFunc)(vector[string], void *user_data)
    ctypedef void (*pyCFunc)(char* name, void *user_data)
    cdef cppclass FruitJuicer:
        FruitJuicer(pyVFunc, void*, vector[string]) except +
        size_t squeeze(pyCFunc user_func, void *user_data)