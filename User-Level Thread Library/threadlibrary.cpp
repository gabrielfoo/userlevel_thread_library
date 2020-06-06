#include <queue>
#include "threadlibrary.h"
#include "antideadlock.h"

TCBlock* global_head = nullptr;
std::queue<thread_id_> id_queue;

//function prototypes of accessory functions. needed by the main threadlibrary functions. similar to the Private function of a class.
void CtxSwitch(TCBlock* next, TCBlock* curr);
void funcStarter(void *(*thd_function_t)(void *), void* parameter, TCBlock* next);
TCBlock* FetchNode(TCBlock* Node, thread_id_ id);
TCBlock* FetchNextNode(TCBlock* Node);
TCBlock* FetchCurrNode();
void ClearFinishedStack();
void ClearAllNode(TCBlock** ptr_node);
thread_id_ addTCB(void *(*thd_function_t)(void *), void* parameter, TCBlock** ptr_node);
bool areThreadsFinished();

class TCBlockDeleter {
public:
	TCBlockDeleter() {}
	~TCBlockDeleter() {
		ClearAllNode(&global_head);
	}
};


void thread_exit(void *ret_value) {
	if (global_head == nullptr) exit(0);

	thread_id_ currID = id_queue.front();

	if (currID == 1) { //THIS PORTION GETS INVOKE IF MAIN CALLS THREAD_EXIT
		while (!areThreadsFinished()) 
		{
			thread_yield(); //program will exit only after all threads are cleared.
		}
		exit(0);
	}

	TCBlock* ThreadTCB = FetchNode(global_head, currID);

	ThreadTCB->returnValue = ret_value;

	ThreadTCB->status = FINISH;

	thread_yield();
}

bool areThreadsFinished() {
	TCBlock* Node = global_head;


	while (Node != nullptr) {

		if (Node->status != FINISH && Node->PID != 1) {
			return false;
		}
		Node = Node->next;
	}

	return true;
}

THREAD_ERRORS wait_thread(thread_id_ id, void **value) {

	if (global_head == nullptr)return NO_THREAD_FOUND;

	TCBlock* target = FetchNode(global_head, id);

	if (target == nullptr)return NO_THREAD_FOUND; //THREAD_NOT_FOUND enum

	//if finished, return
	if (target->status == FINISH) {
		if (value != nullptr) *value = target->returnValue; //retrieve the stuff stored in the returnValue
		return WAIT_SUCCESSFUL; //WAIT SUCCESSFUL ENUM
	}


	//check for cycles
	TCBlock* currentNode = FetchNode(global_head, id_queue.front());
	AddToAdjacentList(currentNode, target);

	if (CycleCheck(currentNode))
	{
		return THREAD_DEADLOCK;
	}
	else
	{
		ClearVisited(global_head);
	}

	//the main, essential part of wait_thread. delegates it to our scheduler in thd_yield(), and forces this function into a loop till it finishes
	while (target->status != FINISH) thread_yield();

	if (value != nullptr) *value = target->returnValue;

	RemoveFromAdjacentList(currentNode, target);

	return WAIT_SUCCESSFUL;//WAIT SUCCESSFUL ENUM

}


void thread_yield() {
	if (global_head == nullptr)return;

	//scheduling block is here. every function is dependent on this two lines of code
	TCBlock* curr = FetchCurrNode();
	TCBlock* next = FetchNextNode(global_head);

	if (next == nullptr || curr == next)return;

	CtxSwitch(next, curr);

	ClearFinishedStack();
}

void thread_sleep(int seconds) 
{

	std::time_t end_time = std::time(NULL) + seconds;

	while (std::time(NULL) < end_time)
	{
		thread_yield();
	}
}

void CtxSwitch(TCBlock* next, TCBlock* curr) {
	unsigned* curr_stk = nullptr;
	unsigned* next_stk = next->Stack;
	int count = 0;

	if (next != curr && curr != nullptr) {
		//saves context into curr
		_asm {
			pushad
			pushfd
			push ebp
			call GETip
			GETip :
			mov curr_stk, ESP
		}
	}

	if (!count) {
		if (next != curr && curr != nullptr) curr->_ESP = curr_stk;
		++count;
		if (next->status == RUNNING || next->status == FINISH) {
			unsigned* next_esp = next->_ESP;
			//switches ESP to running function
			_asm {
				mov ESP, next_esp
				ret
			}
		}
		//launch function for the first time
		_asm {
			mov ESP, next_stk
		}
		funcStarter(next->func_ptr, next->func_param, next);
	}
	//restores context
	_asm {
		pop ebp
		popfd
		popad
	}
}

void funcStarter(void *(*thd_function_t)(void *), void* parameter, TCBlock* next) {

	if (next->status == NOT_STARTED) next->status = RUNNING;

	//run the function here and save return value into the TCB
	next->returnValue = thd_function_t(parameter);

	next->status = FINISH;

	thread_yield();

}

thread_id_ add_new_thread(void *(*thd_function_t)(void *), void* parameter) {

	if (global_head == nullptr) return 0; //must init before starting

	thread_id_ id = addTCB(thd_function_t, parameter, &global_head);

	id_queue.push(id);

	return id;
}

void thread_library_init() {
	if (global_head != nullptr)return;

	static TCBlockDeleter clearsmemorybeforeexit;

	//init main/client's TCB. always running.
	global_head = new TCBlock;
	global_head->PID = 1;
	global_head->status = RUNNING;

	id_queue.push(1);
}


TCBlock* FetchCurrNode() {
	//gets current node on top. then pops and pushes it to the back of the queue
	thread_id_ currPid = id_queue.front();
	id_queue.pop();
	id_queue.push(currPid);
	return FetchNode(global_head, currPid);
}

TCBlock* FetchNextNode(TCBlock* Node) {
	//find nodes that are not finished

	int i = 0;
	thread_id_ currPid = id_queue.front();
	TCBlock* currNode = FetchNode(global_head, currPid);

	//skips FINISH nodes
	while (currNode->status == FINISH) {
		if (i++ > id_queue.size())return nullptr;
		id_queue.pop();
		id_queue.push(currPid);
		currPid = id_queue.front();
		currNode = FetchNode(global_head, currPid);
	}

	return FetchNode(global_head, currPid);
}

TCBlock* FetchNode(TCBlock* Node, thread_id_ id) {

	//as long as Node is not nullptr, goes to the next one till it finds PID
	while (Node != nullptr) {
		if (Node->PID == id) {
			return Node;
		}
		Node = Node->next;
	}
	//when Node is at nullptr, it has reached the end. return nullptr
	return nullptr;

}

void ClearAllNode(TCBlock** ptr_node) {
	TCBlock* Node = *ptr_node;

	if (Node == nullptr) {
		return;
	}
	else ClearAllNode(&(Node->next));

	if (Node->Stack != nullptr) {
		Node->Stack -= 2500;
		delete Node->Stack;
		Node->Stack = nullptr;
	}

	delete Node;
	*ptr_node = nullptr;
	return;
}



thread_id_ addTCB(void *(*thd_function_t)(void *), void* parameter, TCBlock** ptr_node) {
	static thread_id_ threadID = 888;
	TCBlock* node = *ptr_node; //was a recursion function at first..

	while (node->next != nullptr) {
		node = node->next;
	}

	if (node->next == nullptr) {
		node->next = new TCBlock;
		node->next->PID = threadID++;
		node->next->func_ptr = thd_function_t;
		node->next->func_param = parameter;
		node->next->Stack = new unsigned[2501];
		node->next->Stack += 2500;
		node->next->status = NOT_STARTED;
		node->next->next = nullptr;
		return	node->next->PID;
	}


}


void ClearFinishedStack() {
	TCBlock* Node = global_head;
	while (Node != nullptr) {
		if (Node->status == FINISH) {
			if (Node->Stack != nullptr) {
				Node->Stack -= 2500;
				delete Node->Stack;
				Node->Stack = nullptr;
			}
		}
		Node = Node->next;
	}
}