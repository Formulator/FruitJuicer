#squeeze.py

import time
from juicer import PyFruitJuicer

#Functions to callback
def print_list(frt_lst):
    print("Python; Squeezed everything at once: " + ', '.join(frt_lst) + "!")
		
def print_string(frt):
    print("Python; Squeezed: " + frt)
    time.sleep(1)

#Instantiate PyFruitJuicer, C++ FruitJuicer constructor will initiate a thread executing print_list multiple times as a callback. 	
jcr = PyFruitJuicer(print_list, [b"pear", b"peach", b"mango", b"apple", b"banana", b"pineapple"])#'b' to mark as bytes literals to permit passing to C++

#squeezer() returns the count of fruits handled.	print_string will be executed once for each fruit. 	
print("Total ; " + str(jcr.squeezer(print_string)) + " fruits squeezed!")

#Keep script running until C++ thread has completed work, ensures objects remain in existence.
while False == jcr.checkThreadStatus():
    time.sleep(1)