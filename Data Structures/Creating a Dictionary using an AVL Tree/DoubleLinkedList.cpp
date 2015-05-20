#include "DoubleLinkedList.h"

// Default contructor
DoubleLinkedList::DoubleLinkedList()
{
	head = tail = current = NULL;
}

//Destructor
DoubleLinkedList::~DoubleLinkedList()
{
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
	delete tail;
}

//Adds a node to the end
int DoubleLinkedList::addNode(string word, int count)
{
	Node* newNode = new (nothrow) Node(word, count);
	if (newNode == NULL)
		return 1;
		
	if (head == NULL)
	{
		head = newNode;
		tail = newNode;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
	
	return 0;
}

// Adds a node into the linked list before the current node parameter
DoubleLinkedList::Node* DoubleLinkedList::insertNode(string word, Node* insert)
{
	Node *newNode = new (nothrow) Node(word); // Creates new node with data = word
	if (newNode == NULL)
		return insert = NULL;

	if(head == NULL)
	{
		head = newNode;		
		tail = newNode;
		insert = newNode;
	}
	else 
	{
		if(insert == head) // Inserting before the head
		{
			newNode->next = insert;
			newNode->prev = NULL;
			insert->prev = newNode;
			head = newNode;
			insert = newNode;
		}
		else 
		{
			if (insert == NULL) // Inserting after the tail
			{
				newNode->prev = tail;
				newNode->next = NULL;
				tail->next = newNode;
				tail = newNode;
				insert = newNode;
			}
			else
			{
				newNode->next = insert;
				newNode->prev = insert->prev;
				insert->prev->next = newNode;
				insert->prev = newNode;
				insert = newNode;
			}
		}
	}
	return insert;
}

// Finds position in list for word to be inserted alphabetically and calls insertNode
int DoubleLinkedList::createList(string word, Node* list)
{
	while (list!=NULL)
	{
		int cmp = list->data.compare(word);
		if (cmp == 0)
		{
			list->count++;
			return 1;
		}
		else if (cmp > 0)
		{
			Node* temp = insertNode(word, list);
			if (temp == NULL) // Check from memory allocation failure
				return 1;
			temp->count++;
			return 0;
		}
		list = list->next;
	}
	Node* temp = insertNode(word, list);
	if (temp == NULL) // Check from memory allocation failure
		return 1;
	temp->count++;
	return 0;
}

// Creates a union list using the list to add as the first parameter and the new union list as the second parameter
int DoubleLinkedList::createUnion(Node* list1, Node* list2)
{
	while (list1 != NULL)
	{
		if (addNode(list1->data, list1->count)==1)
			return 1;
		list1 = list1->next;
	}
	current = head;
	
	while (list2!=NULL)
	{
		bool flag = false;
		while (current!=NULL && !flag)
		{
			int cmp = current->data.compare(list2->data);
			if (cmp == 0)
			{
				current->count += list2->count;
				flag = true;
			}
			else if (cmp > 0)
			{
				Node* temp = insertNode(list2->data, current);
				if (temp == NULL) // Check from memory allocation failure
					return 1;
				temp->count += list2->count;
				flag = true;
			}
			else
				current = current->next;
		}
		if (!flag)
		{
			Node* temp = insertNode(list2->data, current);
			if (temp == NULL) // Check from memory allocation failure
				return 1;
			temp->count += list2->count;
		}
		list2 = list2->next;
	}
	return 0;
}

// Creates a union list using the lists to add as the first and second parameters and the new intersection list 	as the second parameter
int DoubleLinkedList::createIntersection(Node* list1, Node* list2)
{
	while (list2 != NULL)
	{
		Node* c = list1;
		while (c != NULL)
		{
			if (list2->data.compare(c->data)==0)
			{
				addNode(list2->data, 1);
				if (tail == NULL) // Check from memory allocation failure
					return 1;
			}
			c = c->next;
		}
		list2 = list2->next;
	}
	return 0;
}

// Function to print the linked list
void DoubleLinkedList::writeForward(ofstream &output)
{
	current = head;
	while (current != NULL)
	{
		output << current->data << " " << current->count << endl;
		current = current->next;
	}
}

// Function to print the linked list in reverse
void DoubleLinkedList::writeReverse(ofstream &output)
{
	current = tail;
	while (current != NULL)
	{
		output << current->data << " " << current->count << endl;
		current = current->prev;
	}
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
	delete tail;
}