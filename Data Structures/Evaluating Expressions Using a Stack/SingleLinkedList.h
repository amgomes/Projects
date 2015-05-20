#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include <string>

using namespace std;

class SingleLinkedList
{
	struct Node
	{
		string data;
		Node* next;
	};
public:
	Node* head;
	Node* tail;
	Node* current;
	
	SingleLinkedList();
	~SingleLinkedList();
	void addNode(string);
};
#endif