#include "antideadlock.h"
#include <iostream>

void AddToAdjacentList(TCBlock* currentNode, TCBlock* targetNode)
{
	currentNode->adjacent_list.push_front(targetNode);
}

bool CycleCheck(TCBlock* currentNode) //depth-first recursive traversal
{
	
	if (currentNode->visited == true) {
		return true;
	}
	else {
		currentNode->visited = true;
	}

	auto iteratorListStart = currentNode->adjacent_list.begin();
	auto iteratorListEnd = currentNode->adjacent_list.end();

	while (iteratorListStart != iteratorListEnd)
	{
		if (CycleCheck(*iteratorListStart))
		{
			return true;
		}
		iteratorListStart++;
	}
	return false;
}

void ClearVisited(TCBlock* globalNode) 
{
	TCBlock* currentNode = globalNode;

	while (currentNode != nullptr) 
	{
		currentNode->visited = false;
		currentNode = currentNode->next;
	}
}

void RemoveFromAdjacentList(TCBlock* currentNode, TCBlock* targetNode) 
{
	currentNode->adjacent_list.remove(targetNode);
}