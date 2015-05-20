#ifndef EDGE_H
#define EDGE_H

#include <string>
using namespace std;

class Edge
{
	struct Link
	{
	public:
		string link;
		bool isBroken;
		Link *next;
		
		Link() // Default constructor
		{
			link = "";
			isBroken = true;
			next = NULL;
		}
		Link(const string &newLink, bool broken) // Constructor to set link value
		{
			link = newLink;
			isBroken = broken;
			next = NULL;
		}
	};
	
public:
	Link* head; // Pointer to head of graph
	Link* current; // Pointer used to traverse graph
	Link* tail; // Pointer to last node added in graph
	
	Edge(); // Default constructor
	~Edge(); // Destructor
	int addLink(string, bool);
};
#endif