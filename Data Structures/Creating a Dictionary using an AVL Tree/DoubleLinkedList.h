#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class DoubleLinkedList
{
	struct Node
	{
	public:
		string data; // Stores a word
		int count; // Stores number of times word is repeated in list
		Node *prev; // Stores address of previous node
		Node *next; // Stores address of next node
	
	public:
		// Default constructor
		Node()
		{
			data = "";
			prev = next = NULL;
			count = 0;	
		}
		// Constructor to set data value of the new node
		Node(const string &newData)
		{
			data = newData;
			prev = NULL;
			next = NULL;
			count = 0;
		}
		Node(const string &newData, int newCount)
		{
			data = newData;
			prev = NULL;
			next = NULL;
			count = newCount;
		}
	};

public:
	Node *head; // Pointer to head of linked list
	Node *tail; // Pointer to tail of linked list
	Node *current; // Pointer to traverse list
	
	DoubleLinkedList();  // Default Constructor: Head = Tail = Current
	~DoubleLinkedList(); // Destructor
	
	int addNode(string, int); //Adds node to end of list
	Node* insertNode(string, Node*); // Creates new node before the Node parameter, assigns the string parameter to data and returns the newNode
	
	int createList(string, Node*); // Finds position in list for word to be inserted alphabetically and calls insertNode
	int createUnion(Node*, Node*);	// Creates a union list using the list to add as the first parameter and the new union list as the second parameter	
	int createIntersection(Node*, Node*); // Creates a union list using the lists to add as the first and second parameters and the new intersection list 	as the second parameter
	
	void writeForward(ofstream&); // Outputs list to output file; takes string parameter as output file name
	void writeReverse(ofstream&); // Outputs list to output file in reverse; takes string parameter as output file name
	
	void deleteList(); // Deletes linked list
};
#endif