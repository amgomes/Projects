#include "AVLTree.h"

// Constructor
AVL_Tree::AVL_Tree()
{ root = NULL; }

// Destructor
AVL_Tree::~AVL_Tree()
{ deleteTree(root); }

// Function to insert a node into the tree
AVL_Tree::AVL_Node* AVL_Tree::insert(AVL_Node* root, string data)
{
	if (root == NULL)
	{
		root = new (nothrow) AVL_Node(data);
		if (root == NULL) // Check for memory allocation exception
			return NULL;
	}
	else if (data.compare(root->data) < 0)
	{
		root->left = insert(root->left, data);
		root = balance(root);
	}
	else if (data.compare(root->data) > 0)
	{
		root->right = insert(root->right, data);
		root = balance(root);
	}
	return root;
}

// Function to balance the tree
AVL_Tree::AVL_Node* AVL_Tree::balance(AVL_Node* temp)
{
	int bal_factor = balanceFactor(temp);
	if (bal_factor > 1)
	{
		if (balanceFactor (temp->left) > 0)
			temp = ll_Rotation(temp); // Perform a left rotation
		else
			temp = lr_Rotation(temp); // Perform a left-right rotation
	}
	else if (bal_factor < -1)
	{
		if (balanceFactor (temp->right) > 0)
			temp = rl_Rotation(temp); // Perform a right-left rotation
		else
			temp = rr_Rotation(temp); // Perform a right rotation
	}
	return temp;
}

// Returns the balance factor of the parent parameter by calculating the difference between the left and right children
int AVL_Tree::balanceFactor(AVL_Node* temp)
{
	int lHeight = height (temp->left);
	int rHeight = height (temp->right);
	int bFactor = lHeight - rHeight;
	return bFactor;
}

// Returns the max height of a parent parameter
int AVL_Tree::height(AVL_Node* temp)
{
	int h = 0;
	if (temp != NULL)
	{
		int lHeight = height(temp->left);
		int rHeight = height(temp->right);
		int maxHeight = max(lHeight, rHeight);
		h = maxHeight + 1;
	}
	return h;
}

// Function to perform a left rotation
AVL_Tree::AVL_Node* AVL_Tree::ll_Rotation(AVL_Node* parent)
{
	AVL_Node* temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

// Function to perform a right rotation
AVL_Tree::AVL_Node* AVL_Tree::rr_Rotation(AVL_Node* parent)
{
	AVL_Node* temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

// Function to perform a left-right rotation
AVL_Tree::AVL_Node* AVL_Tree::lr_Rotation(AVL_Node* parent)
{
	AVL_Node* temp;
	temp = parent->left;
	parent->left = rr_Rotation(temp);
	return ll_Rotation(parent);
}

// Function to perform a right-left rotation
AVL_Tree::AVL_Node* AVL_Tree::rl_Rotation(AVL_Node* parent)
{
	AVL_Node* temp;
	temp = parent->right;
	parent->right = ll_Rotation(temp);
	return rr_Rotation(parent);
}

// Returns true if string parameter is found in the tree
bool AVL_Tree::search(AVL_Node* temp, string data)
{
	if (temp != NULL)
	{
		int cmp = data.compare(temp->data);
		if (cmp == 0)
			return true;
		else if (cmp < 0)
			return search(temp->left, data);
		else if (cmp > 0)
			return search(temp->right, data);
		else return false;
	}
	else return false;
}

// Deletes tree
void AVL_Tree::deleteTree(AVL_Node* temp)
{
	if(temp != NULL)
	{
		deleteTree(temp->left);
		deleteTree(temp->right);
		delete temp;
	}
}