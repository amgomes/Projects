#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>
using namespace std;

template <class T>
class Stack
{
	struct Node
	{
	public:
		T data; // Stores word
		Node* next; // Stores address of next node
	};
	
private:
	Node* head;
public:
	Stack();
	~Stack();
	void push(T);
	void pop(T &);
	T top();
	bool empty();
};

/*==============================================================================================*/
/*									Function Definitions   										*/
/*==============================================================================================*/

template <class T>
Stack<T>::Stack()
{
	head = NULL;
}

template <class T>
Stack<T>::~Stack()
{
	Node* current;
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
}

template <class T>
void Stack<T>::push(T newData)
{
	if(head == NULL)
	{
		head = new Node();
		head->data = newData;
		head->next = NULL;
	}
	else
	{
		Node* newNode = new Node();
		newNode->data = newData;
		newNode->next = head;			
		head = newNode;
	}
}

template <class T>
void Stack<T>::pop(T &newData)
{
	if (empty())
		cout << "Stack is empty!" << endl;
	else
	{
		Node* current;
		newData = head->data;
		current = head;
		head = head->next;
	}	
}

template <class T>
T Stack<T>::top()
{
	return head->data;
}

template <class T>
bool Stack<T>::empty()
{
	return (head == NULL);
}

#endif