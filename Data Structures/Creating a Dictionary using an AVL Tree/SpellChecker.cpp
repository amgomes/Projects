#include "CommandLineParser.h"
#include "ScriptParser.h"
#include "AVLTree.h"
#include "DoubleLinkedList.h"

using namespace std;

/*======================================================================================================================================================*/
/*															Struct Declaration																			*/
/*======================================================================================================================================================*/
// Structure to create array of linked lists
struct List
{
	DoubleLinkedList *list;
	string listName; // Stores name of list
	bool listEmpty; // True = list is empty
	int wordCount; // Stores number of words in list
	List()
	{
		list = new (nothrow) DoubleLinkedList();
		listName = "";
		listEmpty = true;
		int wordCount = 0;
	}
	List(const string &name)
	{
		list = new (nothrow) DoubleLinkedList();
		listName = name;
		listEmpty= true;
		wordCount = 0;
	}
	~List()
	{
		delete list;
	}
};

/*======================================================================================================================================================*/
/*															Function Declarations																		*/
/*======================================================================================================================================================*/
void logfile(string); // Outputs error messages to logfiles taking one string as a parameter
void logfile(string, string); // Outputs formatted error messages to logfiles taking two strings as parameters
int readDictionary(char*, AVL_Tree*);
int readFile(List*[], int, ScriptParser, int&); // Reads input file and creates linked list
void convertToArray(DoubleLinkedList*, string*, int); // Copies data values of double linked list into a string array
void convertToList(DoubleLinkedList*, string*, int); // Copies data value of of string array to double linked list
void mergeSort(string[], int, int, string[]);
void merge(string[], int, int, int, int, string[]);
int check(DoubleLinkedList*, DoubleLinkedList*, AVL_Tree*);
int wordSearch(string[], int, int, string); // Returns element value if string parameter is found or -1 if not
string removePunctuation(string&); // Removes all punctuation from beginning and ending of a word and returns it
string convertToLower(string&); // Converts each letter in string parameter to lower and returns the string
int getListName(List*[], string, int); // Returns element of lists that matches the listID

/*======================================================================================================================================================*/
/*															Start of Main Function																		*/
/*======================================================================================================================================================*/

int main(int argc, char* argv[])
{
/*======================================================================================================================================================*/
/*												CommandLineParser - Get command line command															*/
/*======================================================================================================================================================*/
	
	CommandLineParser clp(argv[1],';'); // Create CommandLineParser object
	if (argc !=2) // If number of argument is not correct, print error message to logfile and exit
	{
		logfile("Incorrect number of arguments. One argument must be passed.");
		return 1;
	}
	
/*======================================================================================================================================================*/
/*														Script Parser - Get script commands																*/
/*======================================================================================================================================================*/
	
	char *scriptName = clp.extract("script"); // Gets name of script file from command line
	char *dName = clp.extract("dictionary");
	ScriptParser SParser = ScriptParser(); // Creates ScriptParser object
	
	ifstream script(scriptName); // Opens scriptfile
	if(!script.is_open()) // If scriptfile cannot be opened, print error message to logfile and exit
	{
		logfile(scriptName, "could not be opened. Program will now exit");
		//clp.deleteDynamicVariables(); // Delete pointers
		return 1; // Exit main program
	}
	
/*=====================================================Declares variables needed ======================================================================*/
	List *lists[100]; // Declares an array of linked lists
	int listCounter = 0; // Counter for the number of lists array
	string line = ""; // Variable to store each line from the script file
	bool read = false; // Returns true if read command has been executed
	
/*======================================================================================================================================================*/
/*																	Dictionary																			*/
/*======================================================================================================================================================*/
	AVL_Tree *avl;
	readDictionary(dName, avl);
/*======================================================================================================================================================*/
/*															Start of ScriptFile Reading																	*/
/*======================================================================================================================================================*/
	
	while(getline(script, line)) // Retrieves each command line from the scriptfile
	{
		SParser.parseLine(line); // Parses the line to determine command needed to excute
		if (SParser.operationCode()!=1 && !read) // If first command is not read input file, print error message to logfile and exit
		{	
			logfile("Read command has not been exectuted.");
			continue;
		}
/*======================================================================================================================================================*/
/*													Switch Statement For Scriptfile Commands															*/
/*======================================================================================================================================================*/
		switch (SParser.operationCode()) // Chooses which command to execute based on the operationCode
		{
/*======================================================================================================================================================*/
/*																Case 1 - Read																			*/
/*======================================================================================================================================================*/
		
		case 1: // Operation = "Read"
		{
			int wordCount = 0;
			if(readFile(lists, listCounter, SParser, wordCount)==0) // If input file cannot be read, break
			{
				read = true; // Sets read flag to true, indicating file was read and list was created
				if (lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
					lists[listCounter]->listEmpty = false;
				
				// Initialize string array, convert linked list to array and sort array
				string *listArray = new (nothrow) string[wordCount];
				string *temp = new (nothrow) string[wordCount];
				if (listArray == NULL || temp == NULL) // Check for memory allocation excepetion
				{
					logfile("Memory allocation failed. Array cannot be created.");
					break;
				}
				
				convertToArray(lists[listCounter]->list, listArray, wordCount);
				mergeSort(listArray, 0, wordCount-1, temp);
				listCounter++;
				delete[] temp;
				
				// Convert sorted array to new linked list
				// Update name and word count of list
				lists[listCounter] = new (nothrow) List(SParser.cmd.listID); // Initializes a new list in the array
				if (lists[listCounter] == NULL || lists[listCounter]->list == NULL) // Check for memory allocation exception
				{
					logfile("Memory allocation failed. List cannot be created.");
					break;
				}
				lists[listCounter-1]->listName = "";
				convertToList(lists[listCounter]->list, listArray, wordCount);
				if (lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
					lists[listCounter]->listEmpty = false;
				lists[listCounter]->wordCount = wordCount;
				
				// for (int i=0; i <wordCount; i++)
					// cout << listArray[i] << endl;
				delete[] listArray;
				
				listCounter++; // Increments list counter after a new list is created
			}
			break;
		}
/*======================================================================================================================================================*/
/*																Case 2 - Insert																			*/
/*======================================================================================================================================================*/
		
		case 2: // Operation = "Insert"
		{
			cout << "Operation:" << SParser.cmd.operation << 
				" List Id:" << SParser.cmd.listID <<
				" Value1:" << SParser.cmd.value1 <<
				" Value2:" << SParser.cmd.value2 << endl;
			break;
		}
/*======================================================================================================================================================*/
/*																Case 3 - Delete																			*/
/*======================================================================================================================================================*/
		
		case 3: // Operation = "Delete"
		{
			cout << "Operation:" << SParser.cmd.operation << 
				" List Id:" << SParser.cmd.listID <<
				" Value1:" << SParser.cmd.value1 <<
				" Value2:" << SParser.cmd.value2 << endl;
			break;
		}
/*======================================================================================================================================================*/
/*																Case 4 - Forward Write																	*/
/*======================================================================================================================================================*/
		
		case 4: // Operation = "Write Forward"
		{
			int element = getListName(lists, SParser.cmd.listID, listCounter); // Gets array element for list
			if (element == -1) // If list cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile(SParser.cmd.listID, "cannot be found");
				break;
			}
			
			ofstream output;
			output.open(SParser.cmd.value1.c_str()); // Opens file to be written using cmd.value1 as the output file name
			
			if (!output.is_open())  // If file cannot be opened, output error message to logfile
			{
				logfile(SParser.cmd.value1, "could not be written to.");
				break;
			}
			
			lists[element]->list->writeForward(output); // Function call to write to file
			
			output.close(); // Close file
			break;
		}
/*======================================================================================================================================================*/
/*																Case 5 - Reverse Write																	*/
/*======================================================================================================================================================*/
		
		case 5: // Operation = "Write Reverse"
		{
			int element = getListName(lists, SParser.cmd.listID, listCounter);  // Gets array element for list
			if (element == -1) // If list cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile(SParser.cmd.listID, "cannot be found");
				break;
			}
			
			ofstream output; // Opens output file
			output.open(SParser.cmd.value1.c_str());
			
			if (!output.is_open())  // If file cannot be opened, output error message to logfile
			{
				logfile(SParser.cmd.value1, "could not be written to.");
				break;
			}
			
			lists[element]->list->writeReverse(output); // Function call to write to file in reverse
			
			output.close(); // Close file
			break;
		}
/*======================================================================================================================================================*/
/*																Case 6 - Union																			*/
/*======================================================================================================================================================*/
		
		case 6: // Operation = "Union"
		{
			int element1 = getListName(lists, SParser.cmd.listID, listCounter); // Gets array element for first list
			int element2 = getListName(lists, SParser.cmd.value1, listCounter); // Gets array element for second list
			
			if (element1 == -1 || element2 == -1) // If list(s) cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile("One or more lists needed to create union cannot be found");
				break;
			}
			
			lists[listCounter] = new (nothrow) List(SParser.cmd.value2); // Initializes a new list in the array
			if (lists[listCounter] == NULL || lists[listCounter]->list == NULL) // Check for memory allocation exception
			{
				logfile("Memory allocation failed. List cannot be created.");
				break;
			}
			
			if (lists[element1]->listEmpty) // If list is empty, output error message to logfile
				logfile(lists[element1]->listName, "is empty. Union will not include this list.");
			else if (lists[element2]->listEmpty) // If list is empty, output error message to logfile
				logfile(lists[element1]->listName, "is empty. Union will not include this list.");
			else // Call function to create union list
				if(lists[listCounter]->list->createUnion(lists[element1]->list->head, lists[element2]->list->head)==1)
					break;
			
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
			
			listCounter++; // Increments list counter after a new list is created
			
			break;
		}
/*======================================================================================================================================================*/
/*																Case 7 - Intersection																	*/
/*======================================================================================================================================================*/
		
		case 7: // Operation = "Intersection"
		{
			int element1 = getListName(lists, SParser.cmd.listID, listCounter); // Gets array element for first list
			int element2 = getListName(lists, SParser.cmd.value1, listCounter); // Gets array element for second list
			
			if (element1 == -1 || element2 == -1) // If list(s) cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile("One or more lists needed to create intersection cannot be found");
				break;
			}
			
			lists[listCounter] = new (nothrow) List(SParser.cmd.value2); // Initializes a new list in the array
			if (lists[listCounter] == NULL || lists[listCounter]->list == NULL) // Check for memory allocation exception
			{
				logfile("Memory allocation failed. List cannot be created.");
				break;
			}
			
			if(lists[element1]->listEmpty || lists[element1]->listEmpty) // If one of the lists are empty, print error message to logfile
				logfile("One of more lists is empty. Intersection will be empty.");
			else  // Call function to create intersection list
				lists[listCounter]->list->createIntersection(lists[element1]->list->head, lists[element2]->list->head);
			
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
			
			listCounter++; // Increments list counter after a new list is created
			break;
		}
/*======================================================================================================================================================*/
/*																Case 8 - Check																	*/
/*======================================================================================================================================================*/
		case 8: // Operation = "Check"
		{
			int element1 = getListName(lists, SParser.cmd.value1, listCounter);
			
			if (element1 == -1) // If list cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile("List to be checked cannot be found");
				break;
			}
			
			lists[listCounter] = new (nothrow) List(SParser.cmd.listID); // Initializes a new list in the array
			if (lists[listCounter] == NULL || lists[listCounter]->list == NULL) // Check for memory allocation exception
			{
				logfile("Memory allocation failed. List cannot be created.");
				return 1;
			}
			
			if(lists[element1]->listEmpty) // If list to check is empty, print error message to logfile
				logfile(lists[element1]->listName, "is empty. Cannot perform check.");
			else
				lists[listCounter]->wordCount = check(lists[element1]->list, lists[listCounter]->list, avl);
				
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
				
			listCounter++;
			break;
		}
/*======================================================================================================================================================*/
/*																Case 0 - Error																			*/
/*======================================================================================================================================================*/
		
		case 0: // Invalid command
			logfile(line, "cannot be executed.");
			break;
		}
	}
/*======================================================================================================================================================*/
/*															End of ScriptFile Reading																	*/
/*======================================================================================================================================================*/	
	script.close(); // Close the script file
	return 0;
}
/*======================================================================================================================================================*/
/*																End of Main Function																	*/
/*======================================================================================================================================================*/


/*======================================================================================================================================================*/
/*																Function Definitions																	*/
/*======================================================================================================================================================*/
void logfile(string str1)
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Error: " << str1 << endl; // Prints the error message to the logfile
	logfile.close(); // Close the logfile
}
/*======================================================================================================================================================*/

void logfile(string str1, string str2)
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Error: \"" << str1 <<"\" " << str2 << endl; // Prints the error message to the logfile
	logfile.close(); // Close the logfile
}
/*======================================================================================================================================================*/

int readDictionary(char* dict, AVL_Tree* avl)
{
	ifstream file;
	file.open(dict);
	if (!file.is_open())
	{
		logfile(dict, "could not be opened.");
		return 1;
	}
	
	string word = "";
	
	while(file >> word)
	{
		removePunctuation(word);
		convertToLower(word);
		avl->root = avl->insert(avl->root, word);
		if (avl->root == NULL)  // Check for memory allocation exception
		{
			logfile("Memory allocation failed. AVL Node cannot be created.");
			break;
		}
	}
	file.close(); // Closes input file
	return 0;	
}
/*======================================================================================================================================================*/

int readFile(List* lists[], int listCounter, ScriptParser SParser, int &count)
{
	ifstream file;
	file.open(SParser.cmd.value1.c_str()); // Opens file to be read
	if (!file.is_open())// If file cannot be opened, print error message to logfile and exit function
	{
		logfile(SParser.cmd.value1, "could not be opened.");
		return 1;
	}
	
	lists[listCounter] = new (nothrow) List(SParser.cmd.listID); // Initializes a new list in the array
	if (lists[listCounter] == NULL || lists[listCounter]->list == NULL) // Check for memory allocation exception
	{
		logfile("Memory allocation failed. List cannot be created.");
		return 2;
	}
		
	string word = ""; // Stores words from the file

	while(file >> word)
	{
		removePunctuation(word);
		if(lists[listCounter]->list->addNode(convertToLower(word), 1)==1)
		{
			logfile("Memory allocation failed. List Node cannot be created.");
			return 2;
		}
		count++;
	}

	file.close(); // Closes input file
	return 0;
}
/*======================================================================================================================================================*/

void convertToArray(DoubleLinkedList *list, string *arr, int size)
{
	int i = 0;
	list->current = list->head;
	
	while (list->current != NULL && i<size)
	{
		arr[i] = list->current->data;
		list->current = list->current->next;
		i++;
	}
}
/*======================================================================================================================================================*/

void convertToList(DoubleLinkedList* list, string *arr, int size) // Copies data value of of string array to double linked list
{
	list->addNode(arr[0], 1); // Initialize head of list
	list->current = list->head;
	
	for (int i=1; i<size; i++)
	{
		if (arr[i].compare(arr[i-1]) != 0)
		{
			list->addNode(arr[i], 1);
			list->current = list->current->next;
		}
		else
			list->current->count++;
	}
}
/*======================================================================================================================================================*/

void mergeSort(string a[], int left, int right, string b[])
{
	if(left<right)
	{
		int mid = (left + right) / 2;
		mergeSort(a, left, mid, b);
		mergeSort(a, mid+1, right, b);
		merge(a, left, mid, mid+1, right, b);
	}
}
/*======================================================================================================================================================*/

void merge(string a[], int l1, int r1, int l2, int r2, string b[])
{
	int i = l1;
	int L = l1;
	int R = r2;

	while (l1 <= r1 || l2 <= r2)
	{
		if (l1 <= r1 && l2 <= r2)
		{
			int cmp = a[l1].compare(a[l2]);
			if (cmp <= 0)
				b[i++] = a[l1++];
			else if (cmp > 0)
				b[i++] = a[l2++];
		}
		else if (l1 > r1)
			b[i++] = a[l2++];
		else if (l2 > r2)
			b[i++] = a[l1++];
		else
			logfile ("Merge sort error!");
	}
	for (int i=L; i<=R; i++)
		a[i] = b[i];
}
/*======================================================================================================================================================*/

int check(DoubleLinkedList* wordsToFind, DoubleLinkedList* wordsNotFound, AVL_Tree* avl)
{
	int count =0;
	string word;
	wordsToFind->current = wordsToFind->head;
	while (wordsToFind->current != NULL)
	{
		word = wordsToFind->current->data;
		if(!avl->search(avl->root, word))
		{
			wordsNotFound->addNode(word, 1);
			count++;
		}
		wordsToFind->current = wordsToFind->current->next;
	}
	return count;
}
/*======================================================================================================================================================*/

int wordSearch(string arr[], int l, int r, string word)
{
	while (l <= r)
	{
		int m = (l+r)/2;
		int cmp = arr[m].compare(word);
		if (cmp == 0)
			return m;
		else if (cmp < 0)
			l = m+1;
		else if (cmp > 0)
			r = m-1;
	}
	return -1;
}
/*======================================================================================================================================================*/

string convertToLower(string &word)
{
	int size = word.length();
	
	for (int i=0; i<size; i++)
		word[i] = tolower(word[i]);
	
	return word;
}
/*======================================================================================================================================================*/

string removePunctuation(string &word)
{	
	// Check for punctuation marks at the beginnning
	while(ispunct(word[0]))
			word.erase(word.begin());
	
	int size = word.length();
	
	// Check for punctuation marks at the end
	while(ispunct(word[--size]))
		word.resize(size);
	
	return word;
}
/*======================================================================================================================================================*/

int getListName(List* lists[], string name, int listCounter)
{
	for(int i=0; i<listCounter; i++)
	{
		if (name.compare(lists[i]->listName)==0)
			return i;
	}
	return -1;
}