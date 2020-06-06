#ifndef THREAD_LIB_H
#define THREAD_LIB_H

#include <list>
#include <ctime>

enum THREAD_STATUS {
	NOT_STARTED, RUNNING, FINISH
};

typedef unsigned int thread_id_;

struct TCBlock { //thread control block : info about current threads here
	thread_id_ PID;
	void *(*func_ptr)(void *) = nullptr;
	void* func_param = nullptr;
	THREAD_STATUS status;
	unsigned* _ESP = nullptr;
	unsigned* Stack = nullptr;
	TCBlock* next = nullptr;
	void* returnValue;
	//graph
	bool visited = false;
	std::list<TCBlock*> adjacent_list;
};

enum THREAD_ERRORS
{
	NO_THREAD_FOUND,
	WAIT_SUCCESSFUL,
	THREAD_DEADLOCK,
};

//the "constructor" for the thread library "object". basically estatablishes the linked-list TCB for "main" 
void thread_library_init();
//the backbone of the thread library: pauses the current thread context switches to the next thread
void thread_yield();
//adds thread. accepts function pointer with a parameter.
thread_id_  add_new_thread(void *(*thd_function_t)(void *), void* parameter);
//thread exit is guaranteed to stop the thread when called. if called in main, will exit
void thread_exit(void *ret_value);
//its like the sleep function you usually use. this will put the current thread to sleep for X seconds and yields to other threads.
void thread_sleep(int seconds);
//guaranteed to not allow the caller thread to proceed until the thread identified by the ID is done.
THREAD_ERRORS wait_thread(thread_id_ id, void **value);




#endif
