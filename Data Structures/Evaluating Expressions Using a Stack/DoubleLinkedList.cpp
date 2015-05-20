#include "DoubleLinkedList.h"

// Default contructor
DoubleLinkedList::DoubleLinkedList()
{
	head = tail = NULL;
}

DoubleLinkedList::~DoubleLinkedList()
{
	Node* current;
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
}

// Adds a node into the linked list before the current node parameter
DoubleLinkedList::Node* DoubleLinkedList::insertNode(string word, Node* &current)
{
	Node *newNode = new Node(word); // Creates new node with data = word
	
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
	return current;
}

// Finds position in list for word to be inserted alphabetically and calls insertNode
void DoubleLinkedList::createList(string word, Node* current)
{
	while (current!=NULL)
	{
		int cmp = current->data.compare(word);
		if (cmp == 0)
		{
			current->count++;
			return;
		}
		else if (cmp > 0)
		{
			insertNode(word, current)->count++;
			return;
		}
		current = current->next;
	}
	insertNode(word, current)->count++;
}

// Creates a union list using the list to add as the first parameter and the new union list as the second parameter
void DoubleLinkedList::createUnion(Node* unionNode, Node* current)
{
	if (current!=NULL)
	{
		findInsertPositionUnion(current, unionNode);
		createUnion(unionNode, current->next);
	}
}

// Finds position in list for word to be inserted alphabetically and calls insertNode
void DoubleLinkedList::findInsertPositionUnion(Node* word, Node* unionNode)
{
	while (unionNode!=NULL)
	{
		int cmp = unionNode->data.compare(word->data);
		if (cmp == 0)
		{
			unionNode->count += word->count;
			return;
		}
		else if (cmp > 0)
		{
			insertNode(word->data, unionNode)->count += word->count;
			return;
		}
		unionNode = unionNode->next;
	}
	insertNode(word->data, unionNode)->count += word->count;
}

// Creates a union list using the lists to add as the first and second parameters and the new intersection list 	as the second parameter
void DoubleLinkedList::createIntersection(Node* intersection, Node* list1, Node* list2)
{
	if (list2 != NULL)
	{
		matchFoundIntersection(intersection, list1, list2);
		createIntersection(intersection, list1, list2->next);
	}
}

// Compares data from the two lists passed as parameters and if a match is found, it calls insertNode
void DoubleLinkedList::matchFoundIntersection(Node* intersection, Node* list1, Node* list2)
{
	if (list1 != NULL)
	{
		if (list2->data.compare(list1->data)==0)
			insertNode(list2->data, intersection)->count++;
		else
			matchFoundIntersection(intersection, list1->next, list2);
	}
}

// Recursive function to print the linked list
void DoubleLinkedList::writeForward(Node* current, ofstream &output)
{
	if (current != NULL)
	{
		output << current->data << " " << current->count << endl;
		writeForward(current->next, output);
	}
}

// Recursive function to print the linked list in reverse
void DoubleLinkedList::writeReverse(Node* current, ofstream &output)
{	
	if (current != NULL)
	{
		writeReverse(current->next, output);
		output << current->data << " " << current->count << endl;
	}
}

// Get and print size of the linked list to log file
int DoubleLinkedList::getSize()
{
	int size = 0; // Stores size of linked list

	if (head != NULL) // Returns true if list is not empty
	{
		Node* current = head;
		while(current->next!=NULL) // Iterate through list until current = tail
		{
			size = size + sizeof (struct Node); // Size = size + size of the current->data + 8 bytes (next and prev address)
			current = current->next;
		}
		size = size + sizeof (struct Node); // Adds size of tail to size
	}
	return size;
}