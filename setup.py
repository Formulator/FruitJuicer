from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

#Wrapping a C++ api
setup(
  name = 'juice',
  ext_modules=cythonize([
	Extension("juicer", ["juicer.pyx", "FruitJuicer.cpp"],
	include_dirs=["%PATH%\\Anaconda3\\envs\\py35\\include"],
	library_dirs=["%PATH%\\Anaconda3\\envs\\py35\\libs"],
  libraries=["python35"]),
	]),   
)