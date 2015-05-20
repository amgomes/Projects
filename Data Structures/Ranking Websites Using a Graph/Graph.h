#ifndef GRAPH_H
#define GRAPH_H

#include "Edge.h"
#include <iostream>
using namespace std;

class Graph
{	
	struct Vertex // Vertex struct
	{
	public:
		string webpage;
		int inDegree;
		int outDegree;
		// bool isSink;
		int brokenLinks;
		Vertex *next;
		Edge *edge;
		
		Vertex() // Default constructor
		{
			webpage = "";
			inDegree = outDegree = brokenLinks = 0;
			// isSink = true;
			next = NULL;
			edge = new Edge();
		}
		Vertex(const string &newName) // Constructor to set name value
		{
			webpage = newName;
			inDegree = outDegree = brokenLinks = 0;
			// isSink = true;
			next = NULL;
			edge = NULL;
			edge = new Edge();
		}
	};
	
public:
	string graphName;
	int nbWebpages;
	
	Vertex* head; // Pointer to head of graph
	Vertex* current; // Pointer used to traverse graph
	Vertex* tail; // Pointer to last node added in graph
	
	Graph(); // Default constructor
	Graph(string); // Constructor to set graphName
	~Graph(); // Destructor
	
	int addVertex(string); // Adds a new vertex
	void print();
};
#endif