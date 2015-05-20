#include "SingleLinkedList.h"

SingleLinkedList::SingleLinkedList()
{
	head = tail = current = NULL;
}

SingleLinkedList::~SingleLinkedList()
{
	Node* current;
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
}

void SingleLinkedList::addNode(string newData)
{
	Node* newNode = new Node();
	newNode->data = newData;
	if (head == NULL)
	{
		head = newNode;
		tail = newNode;
	}
	else
	{
		tail->next = newNode;
		tail = newNode;
		tail->next = NULL;
	}
}