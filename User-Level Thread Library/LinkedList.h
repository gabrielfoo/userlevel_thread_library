#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "threadlibrary.h"

extern struct TCBlock;

thread_id_ addTCB(void *(*thd_function_t)(void *), void* parameter, TCBlock** ptr_node);
TCBlock* FetchNode(TCBlock* Node, thread_id_ id);
TCBlock* FetchNextNode(TCBlock* Node);
TCBlock* FetchCurrNode();
void ClearFinishedStack();
void ClearAllNode(TCBlock** ptr_node);


#endif