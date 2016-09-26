#ifndef FRUITJUICER_H
#define FRUITJUICER_H

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <iostream>
#include "Python.h"

namespace juicer {

//Pointer to function types.
typedef void (*pyVecFunc)(std::vector<std::string> fruitsOut, void *user_data);
typedef void (*pyCharFunc)(char* name, void *user_data);
		
class FruitJuicer
 { 
	public:
	FruitJuicer(const pyVecFunc &user_func, void *user_data, const std::vector<std::string> &fruitsIn): pyVectorFunction(user_func), usrData(user_data), fruits(fruitsIn), count(0)
	 {		
		 // Make sure the GIL has been created since we need to acquire it in our callback to safely call into the python application.
		 if (! PyEval_ThreadsInitialized())
		 {
		  PyEval_InitThreads();
		 }
		 
		 //Demonstrate callback to Python from a separate C++ thread.
		 std::cout << "C++   ; Acquiring GIL...";
		 //Acquire Python GIL
		 PyGILState_STATE gstate;
		 gstate = PyGILState_Ensure();		
		 std::cout << "Launching thread...\n";
		 std::thread t(&FruitJuicer::run_juicer, this);
		 t.join();//need to wait for thread completion before releasing GIL		
		 //Release Python GIL
		 PyGILState_Release(gstate);
		 std::cout << "C++   ; GIL Released.\n";
		}

		~FruitJuicer(){}
		
		//Callback Python function element-by-element, pass data as char*.
		size_t squeeze(pyCharFunc pfc, void* ud)
		{
			for(auto &fruit : fruits)
				{
				//std::vector manages memory to convert std::string to writable char* acceptable to the callback function.
				std::vector<char> writable(fruit.begin(), fruit.end());
				writable.push_back('\0');
				pfc(&writable[0], ud);//get the char* using &writable[0] or &*writable.begin()
				++count;//keep count
				}
       return count;
		}
			
		private:
		std::vector<std::string> fruits;
		pyVecFunc pyVectorFunction;
		void* usrData;
		std::atomic<size_t> count;

		//Runs in a thread and performs callback to Python passing std::vector<std::string> to be converted to a Python list.
		void run_juicer()
		{
			pyVectorFunction(fruits, usrData);
			count += fruits.size();//keep count
		}
	};
}
#endif
