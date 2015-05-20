#include "VertexList.h"

VertexList::VertexList()
{
	head = tail = current = NULL;
}

VertexList::~VertexList()
{
	Node* current;
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
}

// Adds a node before the current node
void VertexList::insertNode(string word, int newInDegree)
{
	Node *newNode = new Node(word, newInDegree); // Creates new node with data = word
	
	if(head == NULL)
	{
		head = newNode;		
		tail = newNode;
		current = newNode;
	}
	else 
	{
		if(current == head) // Inserting before the head
		{
			newNode->next = current; // Assigns next value of newNode
			newNode->prev = NULL; // Assigns prev value of newNode
			current->prev = newNode; // Assigns prev value of current
			head = newNode;
			current = newNode;
		}
		else 
		{
			if (current == NULL) // Inserting after the tail
			{
				newNode->prev = tail;
				newNode->next = NULL;
				tail->next = newNode;
				tail = newNode;
				current = newNode;
			}
			else
			{
				newNode->next = current; // Assigns next value of newNode
				newNode->prev = current->prev; // Assigns prev value of newNode
				current->prev->next = newNode; // Assigns next value of current->prev->next
				current->prev = newNode; // Assigns prev value of current
				current = newNode;
			}
		}
	}
}