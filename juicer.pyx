# distutils: language = c++
# distutils: sources = FruitJuicer.cpp

'''
If the class you’re wrapping does not have a nullary constructor,
you must store a pointer to the wrapped class and manually allocate
and deallocate it. A convenient and safe place to do so is in the
__cinit__ and __dealloc__ methods which are guaranteed to be called
exactly once upon creation and deletion of the Python instance
'''

cdef void callbackVec(vector[string] data, void *f):
    (<object>f)([x.decode('utf-8') for x in data])

cdef void callbackChar(char* name, void *f):
    (<object>f)(name.decode('utf-8'))
		
cdef class PyFruitJuicer:   
    cdef FruitJuicer* ptr_cpp_juicer      # hold a C++ instance which we're wrapping
    def __cinit__(self, object function,  list fruits_list):
        self.ptr_cpp_juicer = new FruitJuicer(callbackVec, <void*>function, fruits_list)
    def __dealloc__(self):
        del self.ptr_cpp_juicer
    def squeezer(self, func):
        return self.ptr_cpp_juicer.squeeze(callbackChar, <void*>func)
    def checkThreadStatus(self):
        return self.ptr_cpp_juicer.isThreadFinished()