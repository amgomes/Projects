#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class AVL_Tree
{
	struct AVL_Node
	{
	public:
		string data;
		AVL_Node *left;
		AVL_Node *right;
		
	public:
		AVL_Node()
		{
			data = "";
			left = right = NULL;
		}
		AVL_Node(const string& name)
		{
			data = name;
			left = right = NULL;
		}
	};

public:
	AVL_Node *root;
	
	AVL_Tree(); // Constructor
	~AVL_Tree(); // Destructor
	AVL_Node* insert(AVL_Node*, string); // Function to insert a node into the tree
	AVL_Node* balance(AVL_Node*); // Function to balance the tree
	int balanceFactor(AVL_Node*); // Returns the balance factor of the parent parameter by calculating the difference between the left and right children
	int height(AVL_Node*); // Returns the max height of a parent parameter

	AVL_Node* ll_Rotation(AVL_Node*); // Function to perform a left rotation
	AVL_Node* rr_Rotation(AVL_Node*); // Function to perform a right rotation
	AVL_Node* lr_Rotation(AVL_Node*); // Function to perform a left-right rotation
	AVL_Node* rl_Rotation(AVL_Node*); // Function to perform a right-left rotation
	
	bool search(AVL_Node*, string); // Returns true if string parameter is found in the tree
	void display(AVL_Node*, int); // Displays the tree horizontally
	void deleteTree(AVL_Node*); // Deletes tree
};
#endif