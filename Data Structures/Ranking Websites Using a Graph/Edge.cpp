#include "Edge.h"

// Default constructor
Edge::Edge()
{
	head = current = tail = NULL;
}

// Destructor
Edge::~Edge()
{
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
	delete tail;
}

// Adds an edge
int Edge::addLink(string newName, bool broken)
{
	Link* newLink = new (nothrow) Link(newName, broken);
	if (newLink == NULL)
		return 1;
		
	if (head == NULL)
	{
		head = newLink;
		tail = newLink;
	}
	else
	{
		tail->next = newLink;
		tail = newLink;
	}
	
	return 0;
}