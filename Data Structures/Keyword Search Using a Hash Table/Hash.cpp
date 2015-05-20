#include "Hash.h"

// Default constructor
Hash::Hash()
{
	head = tail = current = NULL;
}

// Destructor
Hash::~Hash()
{
	while(head != NULL)  // Iterate through list until pointer head = NULL
	{
		current = head->next; // Assigns current to head
		delete head; // Deletes current node
		head = current; // Updates head
	}
	delete tail;
}

void Hash::addKeyword(string newData)
{
	KeywordsList* newKeyword = new KeywordsList();
	newKeyword->data = newData;
	if (head == NULL)
	{
		head = newKeyword;
		tail = newKeyword;
	}
	else
	{
		tail->next = newKeyword;
		tail = newKeyword;
		tail->next = NULL;
	}
}

// Hash function
unsigned int Hash::folding(string word, unsigned int size)
{
	int length = word.length();
	unsigned int sum = 0;
	for (int i=0; i<length; i++)
		sum = sum * 131 + word[i];
	return sum % size;
}

// Print hash table
int Hash::print()
{
	int collisions = 0;
	current = head;
	while(current != NULL)
	{
		collisions++;
		// cout << "Keyword: " << current->data << endl;
		current->filenames->current = current->filenames->head;
		// cout << "Filenames: ";
		while (current->filenames->current != NULL)
		{
			// cout << current->filenames->current->data << " ";
			current->filenames->current = current->filenames->current->next;
		}
		// cout << endl;
		current = current->next;
	}
	if (collisions > 0)
		collisions = collisions -1;
	return collisions;
}