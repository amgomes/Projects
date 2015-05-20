#include "CommandLineParser.h"
#include "DoubleLinkedList.h"
#include "ScriptParser.h"

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
	List()
	{
		list = new (nothrow) DoubleLinkedList();
		listName = "";
		listEmpty = true;
	}
	List(string name)
	{
		list = new (nothrow) DoubleLinkedList();
		listName = name;
		listEmpty= true;
	}
};

/*======================================================================================================================================================*/
/*															Function Declarations																		*/
/*======================================================================================================================================================*/
void logfile(string); // Outputs error messages to logfiles taking one string as a parameter
void logfile(string, string); // Outputs formatted error messages to logfiles taking two strings as parameters
void outputSize(List*); // Outputs size of list to logfile
int readFile(List*[], int, ScriptParser, bool&); // Reads input file and creates linked list
string convertToLower(string&); // Converts each letter in string parameter to lower and returns the string
int getListName(List*[], string, int); // Returns element of lists that matches the listID
/*======================================================================================================================================================*/

/*======================================================================================================================================================*/
/*															Start of Main Function																		*/
/*======================================================================================================================================================*/

int main(int argc, char* argv[])
{
/*======================================================================================================================================================*/
/*												CommandLineParser - Get command line command															*/
/*======================================================================================================================================================*/
	
	CommandLineParser clp(argv[1],','); // Create CommandLineParser object
	if (argc !=2) // If number of argument is not correct, print error message to logfile and exit
	{
		logfile("Incorrect number of arguments. One argument must be passed.");
		clp.deleteDynamicVariables(); // Delete pointers
		return 1;
	}

/*======================================================================================================================================================*/
/*														Script Parser - Get script commands																*/
/*======================================================================================================================================================*/
	
	char *scriptName = clp.extract("script"); // Gets name of scriptfile
	ScriptParser SParser = ScriptParser(); // Creates ScriptParser object
	
	ifstream script(scriptName); // Opens scriptfile
	if(!script.is_open()) // If scriptfile cannot be opened, print error message to logfile and exit
	{
		logfile(scriptName, "could not be opened. Program will now exit");
		clp.deleteDynamicVariables(); // Delete pointers
		return 1; // Exit main program
	}
	
/*=====================================================Declares variables needed ======================================================================*/
	List *lists[100]; // Declares an array of linked lists
	int listCounter = 0; // Counter for the number of lists array
	string line = ""; // Variable to store each line from the script file
	bool flag = false; // Returns true if unrecoverable error occurs
	bool read = false; // Returns true if read command has been executed
	// int size = 0; // Used in the getSize() DoubleLinkedList function to output the size of the lists to the logfile
	
/*======================================================================================================================================================*/
/*															Start of ScriptFile Reading																	*/
/*======================================================================================================================================================*/
	while(getline(script, line) && !flag) // Retrieves each command line from the scriptfile
	{
		SParser.parseLine(line); // Parses the line to determine command needed to excute
		
		if (SParser.operationCode()!=1 && !read) // If first command is not read input file, print error message to logfile and exit
		{	
			logfile("Read command has not been exectuted.");
				break;
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
			if(readFile(lists, listCounter, SParser, read)==1) // If input file cannot be read, break
				break;
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
				
			outputSize(lists[listCounter]); // Print size of list to logfile
			listCounter++; // Increments list counter after a new list is created
			break;
/*======================================================================================================================================================*/
/*																Case 2 - Insert																			*/
/*======================================================================================================================================================*/
		
		case 2: // Operation = "Insert"
			cout << "Operation:" << SParser.cmd.operation << 
				" List Id:" << SParser.cmd.listID <<
				" Value1:" << SParser.cmd.value1 <<
				" Value2:" << SParser.cmd.value2 << endl;
			break;
/*======================================================================================================================================================*/
/*																Case 3 - Delete																			*/
/*======================================================================================================================================================*/
		
		case 3: // Operation = "Delete"
			cout << "Operation:" << SParser.cmd.operation << 
				" List Id:" << SParser.cmd.listID <<
				" Value1:" << SParser.cmd.value1 <<
				" Value2:" << SParser.cmd.value2 << endl;
			break;
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
			
			lists[element]->list->writeForward(lists[element]->list->head, output); // Function call to write to file in reverse
			
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
			
			lists[element]->list->writeReverse(lists[element]->list->head, output); // Function call to write to file in reverse
			
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
			
			lists[listCounter] = new List(SParser.cmd.value2); // Initializes a new list in the array
			if (lists[listCounter]->list == NULL) // Check for memory allocation exception
			{
				logfile("Memory allocation failed. List cannot be created.");
				break;
			}
			
			if (lists[element1]->listEmpty) // If list is empty, output error message to logfile
				logfile(lists[element1]->listName, "is empty. Union will not include this list.");
			else // Call function to create union list
				lists[listCounter]->list->createUnion(lists[listCounter]->list->head, lists[element1]->list->head);
				
			if (lists[element2]->listEmpty) // If list is empty, output error message to logfile
				logfile(lists[element1]->listName, "is empty. Union will not include this list.");
			else // Call function to create union list
				lists[listCounter]->list->createUnion(lists[listCounter]->list->head, lists[element2]->list->head);
			
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
			
			outputSize(lists[listCounter]); // Print size of list to logfile
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
			
			lists[listCounter] = new List(SParser.cmd.value2); // Initializes a new list in the array
			if (lists[listCounter]->list == NULL) // Check for memory allocation exception
			{
				logfile("Memory allocation failed. List cannot be created.");
				break;
			}
			
			if(lists[element1]->listEmpty || lists[element1]->listEmpty) // If one of the lists are empty, print error message to logfile
				logfile("One of more lists is empty. Intersection will be empty.");
			else  // Call function to create intersection list
				lists[listCounter]->list->createIntersection(lists[listCounter]->list->head, lists[element1]->list->head, lists[element2]->list->head);
			
			if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
				lists[listCounter]->listEmpty = false;
			
			outputSize(lists[listCounter]); // Print size of list to logfile			
			listCounter++; // Increments list counter after a new list is created
			break;
		}
/*======================================================================================================================================================*/
/*																Case 0 - Error																			*/
/*======================================================================================================================================================*/
		
		case 0:
			// Prints invalid commands to logfile
			logfile(line, "cannot be executed.");
			break;
		}
	}
/*======================================================================================================================================================*/
/*															End of ScriptFile Reading																	*/
/*======================================================================================================================================================*/	
	script.close(); // Close the script file
	
	// Delete pointers
	for (int i=0; i<listCounter; i++)
		lists[i]->list->deleteList();
	clp.deleteDynamicVariables();
	
	for (int i=0; i<listCounter; i++)
		outputSize(lists[i]); // Print size of each linked list after delete	
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

void logfile(string str1, string str2)
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Error: \"" << str1 <<"\" " << str2 << endl; // Prints the error message to the logfile
	logfile.close(); // Close the logfile
}

void outputSize(List* lists)
{
	// Prints size to logfile
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Size of list: \"" << lists->listName << "\" = " << lists->list->getSize() << " bytes" << endl; // Prints the size of list in bytes to the logfile
	logfile.close(); // Close the logfile
}

int readFile(List* lists[],int listCounter, ScriptParser SParser, bool& read)
{
	ifstream file;
	file.open(SParser.cmd.value1.c_str()); // Opens file to be read
	if (!file.is_open())// If file cannot be opened, print error message to logfile and exit function
	{
		logfile(SParser.cmd.value1, "could not be opened.");
		return 1;
	}
			
	read = true; // Sets read flag to true, indicating file was read and list was created
	
	lists[listCounter] = new List(SParser.cmd.listID); // Initializes a new list in the array
	if (lists[listCounter]->list == NULL) // Check for memory allocation exception
	{
		logfile("Memory allocation failed. List cannot be created.");
		return 2;
	}
		
	string word = ""; // Stores words from the file

	while(file >> word)
	{
		if(lists[listCounter]->list->matchFound(convertToLower(word), lists[listCounter]->list->head));
		else
			lists[listCounter]->list->createList(convertToLower(word), lists[listCounter]->list->head);
	}
		
	file.close(); // Closes input file
	return 0;
}

string convertToLower(string &word)
{
	int size = word.length();
	
	//convert each character to lower case
	for (int i=0; i<size; i++)
		word[i] = tolower(word[i]);
	
	return word;
}
int getListName(List* lists[], string name, int listCounter)
{
	for(int i=0; i<listCounter; i++)
	{
		if (name.compare(lists[i]->listName)==0)
			return i;
	}
	return -1;
}