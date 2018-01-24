# FruitJuicer
Demonstration of Cython wrapped C++ class. 

To compile and wrap using Cython;

See http://docs.cython.org/en/latest/index.html for help getting started with Cython.

1. Modify setup.py file to point at your relevant Python installation directories & library.
2. In a terminal window navigate to the folder to which FruitJuicer was downloaded.
3. Execute; python setup.py build_ext --inplace
4. If all is well run the Python script squeeze.py; python squeeze.py
5. Observe some output to the terminal window demonstrating what happened; some fruits get squeezed.

![console_output](https://github.com/Formulator/FruitJuicer/blob/master/fruitjuicer.png)

(Tested on Windows 10 using Anaconda installation of Python 3.5 and Visual Studio 2015 Community Edition compiler)


