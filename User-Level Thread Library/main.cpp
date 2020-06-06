#include <iostream>
#include "threadlibrary.h"
#include <vector>
#include <string>

using namespace std;

void* funcA(void *param) {
	string* str_param = (string*)param;

	cout << "Entered funcA: " << *str_param << endl;
	thread_yield();
	cout << "Exit funcA: " << *str_param << endl;

	return nullptr;
}

void* funcB(void *param) {
	int* int_param = (int *)param;

	cout << "Entered funcB:" << *int_param << endl;
	thread_yield();
	cout << "Exit funcB:" << *int_param << endl;

	return nullptr;
}

void* funcC(void *param) {
	cout << "Entered func C" << endl;
	thread_yield();
	cout << "Exit func C" << endl;

	return nullptr;
}

void* fibo(void *param) {

	int* int_param = (int *)param;
	int* ret = new int;

	if (*int_param <= 1) {
		*ret = 1;
		return ret;
	}

	int* val1 = new int;
	int* val2 = new int;
	void* void1 = new int;
	void* void2 = new int;



	*val1 = *int_param - 1;
	*val2 = *int_param - 2;
	int a = add_new_thread(fibo, val1);
	int b = add_new_thread(fibo, val2);
	wait_thread(a, &void1);
	wait_thread(b, &void2);
	val1 = (int*)void1;
	val2 = (int*)void2;
	*ret = *val1 + *val2;
	thread_exit(ret);

	return nullptr;

}
int waitI;
int waitID;
int waitID2;

void* funcK(void* param) {
	for (int i = 0; i < 3; i++) {
		cout << "111111111" << endl;
		wait_thread(waitID, nullptr);
		cout << "222222222" << endl;
		thread_yield();
	}

	thread_exit(nullptr);
	return nullptr;
}

void* funcD2(void* param) {
	for (int i = 0; i < 5; i++) {
		cout << "=)=)=)=)=)" << endl;
		if (i == 1)wait_thread(waitID2, nullptr);
		else thread_yield();
		cout << "(:(:(:(:(:" << endl;
		thread_yield();
	}
	return nullptr;
}


void* funcD(void* param) {
	for (int i = 0; i < 5; i++) {
		cout << "=)=)=)=)=)" << endl;
		thread_yield();
		cout << "(:(:(:(:(:" << endl;
		thread_yield();
	}
	return nullptr;
}

void* funcE(void* param) {
	for (int i = 0; i < 5; i++) {
		cout << "*********" << endl;
		thread_yield();
		cout << "&&&&&&&&&&" << endl;
		thread_yield();
	}
	return nullptr;
}

void* funcI(void* param) {
	int* int_param = (int *)param;
	int beg_param = *int_param;
	for (int i = 0; i < 5; i++) {
		cout << "First printout: " << (*int_param)++ << endl;
		thread_yield();
		cout << "Second printout: " << (*int_param)++ << endl;
		thread_yield();
	}
	cout << "Hope you got: " << beg_param + 9 << endl;
	return nullptr;
}

void* funcJ(void* param) {
	cout << "IN FUNCTION J" << endl;
	void* v_ret = nullptr;
	thread_id_* id = nullptr;

	if (param != nullptr) {
		id = (thread_id_*)param;

		wait_thread(*id, &v_ret);
	}
	else
		thread_yield();
	string* ret = new string("SUCCESS");
	if (param != nullptr)
		cout << "Thread ID of waiting thread: " << *id << " " << "Return Value: " << *(float*)v_ret << endl;
	cout << "OUT OF FUNCTION J" << endl;
	return ret;
}

void* funcDD(void* param) {
	void* v_ret = nullptr;
	thread_id_* id = nullptr;
	for (int i = 0; i < 5; i++) {
		cout << "=)=)=)=)=)" << endl;
		thread_yield();
		cout << "(:(:(:(:(:" << endl;
		if (param != nullptr) {
			id = (thread_id_*)param;

			wait_thread(*id, &v_ret);
		}
		else
			thread_yield();
	}
	float* ret = new float;
	*ret = 911;

	if (param != nullptr)
		cout << "Thread ID of waiting thread:: " << *id << " " << "Return Value: " << *(float*)v_ret << endl;
	thread_exit(ret);
	return nullptr;
}

void* funcEE(void* param) {
	void* v_ret = nullptr;
	thread_id_* id = nullptr;
	for (int i = 0; i < 5; i++) {
		cout << "*********" << endl;
		if (param != nullptr) {
			id = (thread_id_*)param;

			wait_thread(*id, &v_ret);
		}
		else
			thread_yield();

		cout << "&&&&&&&&&&" << endl;
		thread_yield();
	}
	float* ret = new float;
	*ret = 2020.2;

	if (param != nullptr)
		cout << "Thread ID of waiting thread:: " << *id << " " << "Return Value: " << *(float*)v_ret << endl;

	thread_exit(ret);
	return nullptr; //doesnt do anything
}

void* funcII(void* param) {
	int* int_param = (int *)param;
	int number = 0;

	void* v_ret = nullptr;
	thread_id_* id = nullptr;

	if (param != nullptr) {
		number = *int_param;
	}
	else
		number = 555;

	if (param != nullptr) {
		id = (thread_id_*)param;

		wait_thread(*id, &v_ret);
	}
	else
		thread_yield();

	for (int i = 0; i < 5; i++) {
		cout << "First printout: " << number++ << endl;
		thread_yield();
		cout << "Second printout: " << number++ << endl;
		thread_yield();
	}


	float* f = new float;
	*f = 3.98;

	if (param != nullptr)
		cout << "Thread ID of waiting thread:: " << id << " " << "Return Value: " << *(float*)v_ret << endl;

	return f;
}



int main() {


	thread_library_init();//begin


	cout << '\n' << "SIMPLE YIELD" << '\n' << endl;

	string testa("TEST ONE");
	int b = 888;
	add_new_thread(funcA, &testa);
	add_new_thread(funcB, &b);
	add_new_thread(funcC, nullptr);
	wait_thread(99999, nullptr);
	thread_yield();
	thread_yield();
	thread_yield();
	thread_yield();
	thread_yield();
	cout << '\n' << "SIMPLE YIELD ENDS" << '\n' << endl;
	///////////////////////////


	//////////////////////////////////
	cout << '\n' << "LOOPED YIELDS" << '\n' << endl;
	int h = 5;

	add_new_thread(funcD, nullptr);
	add_new_thread(funcE, nullptr);
	add_new_thread(funcI, &h);

	for (int ii = 0; ii < 50; ++ii) {
		thread_yield();

	}

	cout << '\n' << "LOOPED YIELDS  ENDS" << '\n' << endl;
	/////////////////////////////

	///////////////////////////////////////////////////////////
	cout << '\n' << "SIMPLE WAIT" << '\n' << endl;


	waitI = add_new_thread(funcK, nullptr);
	waitID = add_new_thread(funcD2, nullptr);
	waitID2 = add_new_thread(funcE, nullptr);
	wait_thread(waitI, nullptr);

	puts("");

	cout << '\n' << "SIMPLE WAIT END" << '\n' << endl;


	///////////////////////////////////////////////////////

	//////////////////////////////////
	cout << '\n' << "WAITS AND RETURNS" << '\n' << endl;
	vector<thread_id_> IDvector(4);
	vector<void*> ret(4);
	//J waits for DD, which waits for EE, which waits for II


	IDvector[0] = add_new_thread(funcII, nullptr);
	IDvector[1] = add_new_thread(funcEE, &IDvector[0]);
	IDvector[2] = add_new_thread(funcDD, &IDvector[1]);
	IDvector[3] = add_new_thread(funcJ, &IDvector[2]);


	wait_thread(IDvector[3], &ret[3]);
	wait_thread(IDvector[2], &ret[2]);
	wait_thread(IDvector[1], &ret[1]);
	wait_thread(IDvector[0], &ret[0]);
	puts(" ");
	cout << "ThreadID: " << IDvector[0] << " Float value (Expected 3.98): " << *(float*)ret[0] << endl;
	cout << "ThreadID: " << IDvector[1] << " Float value (Expected 2020.2): " << *(float*)ret[1] << endl;
	cout << "ThreadID: " << IDvector[2] << " Float value (Expected 911): " << *(float*)ret[2] << endl;
	cout << "ThreadID: " << IDvector[3] << " String (Expected 'SUCCESS'): " << *(string*)ret[3] << endl;


	thread_library_init(); //random init popping up here and there should not affect anything
	cout << '\n' << "WAITS AND RETURNS END" << '\n' << endl;
	/////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	cout << '\n' << "FIBONACCI RECURSIVE THREADS." << '\n' << endl;

	vector<int*> fibo_results(12);

	for (int fibo_v = 0; fibo_v < 12; fibo_v++) {
		void* fibo_ret;
		int fx = add_new_thread(fibo, &fibo_v);
		wait_thread(fx, &fibo_ret);
		fibo_results[fibo_v] = (int*)fibo_ret;
	}

	cout << "FIBONACCI SEQUENCE (EXPECTED: [1,1,2,3,5,8,13,21,34,55,89,144,]): RESULTS ([";
	for (int i = 0; i < 12; i++) {
		cout << *fibo_results[i] << ",";
	}
	puts("])");



	cout << '\n' << "FIBONACCI RECURSIVE THREAD ENDS" << '\n' << endl;
	cin.get();
	thread_exit(nullptr);

	cin.get();
}