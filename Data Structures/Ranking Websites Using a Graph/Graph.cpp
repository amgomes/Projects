#include "Graph.h"

// Default constructor
Graph::Graph()
{
	graphName = "";
	nbWebpages = 0;
	head = current = tail = NULL;
}

// Constructor to set the graph name
Graph::Graph(string newName)
{
	graphName = newName;
	nbWebpages = 0;
	head = current = tail = NULL;
}

// Destructor
Graph::~Graph()
{
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
	delete tail;
}

// Adds a vertex
int Graph::addVertex(string newWebpage)
{
	Vertex* newVertex = new (nothrow) Vertex(newWebpage);
	if (newVertex == NULL)
		return 1;
		
	if (head == NULL)
	{
		head = newVertex;
		tail = newVertex;
	}
	else
	{
		tail->next = newVertex;
		tail = newVertex;
	}
	
	return 0;
}

// Print
void Graph::print()
{
	current = head;
	while(current != NULL)
	{
		cout << "\nWebpage: " << current->webpage << endl;
		current->edge->current = current->edge->head;
		cout << "Links webpage points to:\n";
		while (current->edge->current != NULL)
		{
			cout << "  " << current->edge->current->link << endl;
			current->edge->current = current->edge->current->next;
		}
		cout << "InDegree: " << current->inDegree << endl;
		cout << "OutDegree: " << current->outDegree << endl;
		cout << "Broken Links: " << current->brokenLinks << endl;
		cout << "Sink: ";
		if (!current->outDegree > 0)
			cout << "true";
		else cout << "false";
		cout << endl << endl;
		current = current->next;
	}
}