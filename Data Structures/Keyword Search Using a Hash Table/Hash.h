#ifndef HASH_H
#define Hash_H

#include "DoubleLinkedList.h"

class Hash
{
	struct KeywordsList
	{
		string data;
		DoubleLinkedList* filenames;
		KeywordsList* next;
		KeywordsList()
		{
			data = "";
			filenames = new DoubleLinkedList();
			next = NULL;
		}
		~KeywordsList()
		{
			delete filenames;
		}
	};

public:
	KeywordsList *head;
	KeywordsList *current;
	KeywordsList *tail;

	Hash(); // Default constructor
	~Hash(); // Destructor
	void addKeyword(string); // Adds keyword to list
	unsigned int folding(string, unsigned int); // Hash function using folding
	int print();
};
#endif