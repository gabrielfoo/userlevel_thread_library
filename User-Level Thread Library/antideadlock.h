#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "threadlibrary.h"


void AddToAdjacentList(TCBlock* currentNode, TCBlock* targetNode);
bool CycleCheck(TCBlock* currentNode);
void RemoveFromAdjacentList(TCBlock* currentNode, TCBlock* targetNode);
void ClearVisited(TCBlock* globalNode);

#endif