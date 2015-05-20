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
		Node(string word)
		{
			data = word;
			prev = NULL;
			next = NULL;
			count = 0;
		}
	};

public:
	Node *head; // Head of linked list
	Node *tail; // Tail of linked list
	Node *current; // Stores value of current node to help traverse list	
	
	DoubleLinkedList();  // Default Constructor: Head = Tail = Current
	
	Node* insertNode(string, Node*&); // Creates new node before the Node parameter, assigns the string parameter to data and returns the newNode
	
	void createList(string, Node*); // Finds position in list for word to be inserted alphabetically and calls insertNode
	void lettersMatched(string, Node*); // If the first letters of both words match, find the node in the list that word needs to be added before and call insertNode
	bool matchFound(string, Node*); // Returns true if string parameter is matched to a node's data in the list
	
	void createUnion(Node*, Node*);	// Creates a union list using the list to add as the first parameter and the new union list as the second parameter
	void findInsertPositionUnion(Node*, Node*); // Finds position in list for word to be inserted alphabetically and calls insertNode
	void lettersMatchedUnion(Node*, Node*); // If the first letters of both words match, find the node in the list that word needs to be added before and call insertNode
	bool matchFoundUnion(Node*, Node*); // Compares data from the two lists passed as parameters and if a match is found, it updates count
	
	void createIntersection(Node*, Node*, Node*); // Creates a union list using the lists to add as the first and second parameters and the new intersection list 	as the second parameter
	void matchFoundIntersection(Node*, Node*, Node*); // Compares data from the two lists passed as parameters and if a match is found, it calls insertNode
	
	void writeForward(Node*, ofstream&); // Outputs list to output file; takes string parameter as output file name
	void writeReverse(Node*, ofstream&); // Outputs list to output file in reverse; takes string parameter as output file name

	int getSize(); // Prints size of the linked list to the logfile
	void deleteList(); // Deletes linked list
};
#endif