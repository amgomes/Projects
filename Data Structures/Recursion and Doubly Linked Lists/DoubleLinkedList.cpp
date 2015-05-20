#include "DoubleLinkedList.h"

// Default contructor
DoubleLinkedList::DoubleLinkedList()
{
	head = tail = current = NULL;
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
				current = tail;
				newNode->prev = current;
				newNode->next = NULL;
				current->next = newNode;
				tail = newNode;
				current = newNode;
			}
			else
			{
				newNode->next = current; // Assigns next value of newNode
				newNode->prev = current->prev; // Assigns prev value of newNode
				current->prev->next = newNode; // Assigns next value of current->prev->next
				current->prev = newNode; // Assigns prev value of current
				current=newNode;
			}
		}
	}
	return current;
}

// Finds position in list for word to be inserted alphabetically and calls insertNode
void DoubleLinkedList::createList(string word, Node* current)
{
	if (current == NULL)
		insertNode(word, current)->count++;
	else
	{
		while(current!=NULL && word[0] >= current->data[0])
		{
			if (word[0] == current->data[0])
			{
				lettersMatched(word, current);
				return;
			}
			current = current->next;
		}
		insertNode(word, current)->count++;
	}
}

// If the first letters of both words match, find the node in the list that word needs to be added before and call insertNode
void DoubleLinkedList::lettersMatched(string word, Node* current)
{
	while(current!=NULL && word[0] == current->data[0])
	{
		if (word.compare(current->data)<0)
		{
			insertNode(word, current)->count++;
			return;
		}
		else
			current = current->next;
	}
	insertNode(word, current)->count++;
}

// Returns true if string parameter is matched to a node's data in the list	
bool DoubleLinkedList::matchFound(string word, Node* current)
{
	if (current==NULL)
		return false;
	else
	{
		if (word.compare(current->data)==0)
		{
			current->count++;
			return true;
		}
		else
			return (matchFound(word, current->next));
	}
}

// Creates a union list using the list to add as the first parameter and the new union list as the second parameter
void DoubleLinkedList::createUnion(Node* unionNode, Node* current)
{
	if(current != NULL)
	{
		if ((matchFoundUnion(current, unionNode)))
			createUnion(unionNode, current->next);
		else		
		{
			findInsertPositionUnion(current, unionNode);
			createUnion(unionNode, current->next);
		}
	}
}

// Finds position in list for word to be inserted alphabetically and calls insertNode
void DoubleLinkedList::findInsertPositionUnion(Node* word, Node* unionNode)
{
	if (unionNode == NULL)
		insertNode(word->data, unionNode)->count += word->count;
	else
	{
		while(unionNode!=NULL && word->data[0] >= unionNode->data[0])
		{
			if (word->data[0] == unionNode->data[0])
			{
				lettersMatchedUnion(word, unionNode);
				return;
			}
			unionNode = unionNode->next;
		}
		insertNode(word->data, unionNode)->count += word->count;
	}
}

 // If the first letters of both words match, find the node in the list that word needs to be added before and call insertNode
void DoubleLinkedList::lettersMatchedUnion(Node* word, Node* unionNode)
{
	while(unionNode!=NULL && word->data[0] == unionNode->data[0])
	{
		if (word->data.compare(unionNode->data)<0)
		{
			insertNode(word->data, unionNode)->count += word->count;
			return;
		}
		else
			unionNode = unionNode->next;
	}
	insertNode(word->data, unionNode)->count += word->count;
}

 // Compares data from the two lists passed as parameters and if a match is found, it updates count
 bool DoubleLinkedList::matchFoundUnion(Node* word, Node* unionNode)
{
	if (unionNode==NULL)
		return false;
	else
	{
		if (word->data.compare(unionNode->data)==0)
		{
			unionNode->count += word->count;
			return true;
		}
		else
			return matchFoundUnion(word, unionNode->next);
	}
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
		current = head; // Assigns current to beginning of list
		while(current->next!=NULL) // Iterate through list until current = tail
		{
			size = size + sizeof (struct Node); // Size = size + size of the current->data + 8 bytes (next and prev address)
			current = current->next;
		}
		size = size + sizeof (struct Node); // Adds size of tail to size
	}
	return size;
}

// Deletes the linked list
void DoubleLinkedList::deleteList()
{
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
}