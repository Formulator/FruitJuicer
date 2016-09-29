#ifndef FRUITJUICER_H
#define FRUITJUICER_H

#include <string>
#include <vector>
#include <atomic>
#include <future>
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
 FruitJuicer(const pyVecFunc &user_func, void *user_data, const std::vector<std::string> &fruitsIn): pyVectorFunction(user_func), usrData(user_data), fruits(fruitsIn), count(0), run(true)
  {
		// Make sure the GIL has been created since we need to acquire it in our callback to safely call into the python application.
		 if (! PyEval_ThreadsInitialized())
		{
		PyEval_InitThreads();
		}
		
		//Run member function as lambda function in a thread, crucially be able to monitor the thread status using the future
    function = std::bind(&FruitJuicer::run_juicer, this);
    future = std::async(std::launch::async, [this] {
        function();
        return true;
    });		
  }

	~FruitJuicer()
	{		
		//If future thread is still executing then terminate it by setting run = false
		//Use wait_for() with zero milliseconds to check thread status.
		auto status = future.wait_for(std::chrono::milliseconds(1));//needs to have a value > 0
		if (status != std::future_status::ready)//execution not complete
		 {
     run = false;//terminate run_juicer
     future.wait();//wait for execution to complete
		 }
		std::cout << "C++   ; FruitJuicer: Destructor Completed" << std::endl;		
	}

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
	std::function<void()> function;
	std::future<bool> future;
	std::atomic<bool> run;
	pyVecFunc pyVectorFunction;
	void* usrData;
	std::atomic<size_t> count;

	//Runs in a thread and performs callback to Python passing std::vector<std::string> to be converted to a Python list.
	void run_juicer()
	{
		size_t i(0);
		while(i < 5)
		{		
		//Demonstrate callback to Python from a separate C++ thread.
		std::cout << "C++   ; Acquiring GIL...\n";
		//Acquire Python GIL
		PyGILState_STATE gstate = PyGILState_Ensure();
		//Callback Python function
		pyVectorFunction(fruits, usrData);
		//Release Python GIL
		PyGILState_Release(gstate);
		std::cout << "C++   ; Released GIL.\n";		
		count += fruits.size();//keep count
		++i;
		}
	}
 };
}
#endif
