#ifndef VERTEXLIST_H
#define VERTEXLIST_H

#include <string>

using namespace std;

class VertexList
{
	struct Node
	{
		string webpage;
		int inDegree;
		Node* next;
		Node* prev;
		Node::Node() // Default constructor
		{
			webpage = "";
			inDegree = 0;
			next = prev = NULL;
		}
		Node::Node(const string &newWebpage, int newInDegree) // Constructor to assign webpage and inDegree variables
		{
			webpage = newWebpage;
			inDegree = newInDegree;
			next = prev = NULL;
		}
	};
public:
	Node* head;
	Node* tail;
	Node* current;
	
	VertexList();
	~VertexList();
	void insertNode(string, int);
};
#endif