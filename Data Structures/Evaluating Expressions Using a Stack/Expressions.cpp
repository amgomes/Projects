#include "ArgumentManager.cpp"
#include "OperationQueue.cpp"
#include "DoubleLinkedList.h"
#include "SingleLinkedList.h"
#include "Stack.h"

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
int readFile(List*[], int, Operation, bool&); // Reads input file and creates linked list
int infixToPostfix(Operation, SingleLinkedList*, List*[], int&); // Converts infix expression to postfix expression
int evaluateExpression(Operation, SingleLinkedList*, List*[], int&); // Evaluates postfix expression
int constructUnion(string, string, string, List*[], int); // Constructs union lists
int constructIntersection(string, string, string, List*[], int); // Constructs intersection lists
int writeFile(List*[], int, Operation); // Calls list function to write to file
void convertToLower(string&); // Converts each letter in string parameter to lower and returns the string
bool isOperator(char); // Returns true if char parameter is * or +
void getFileName(string&); // Returns file name without the ' '
int getListName(List*[], string, int); // Returns element of lists that matches the listID

/*======================================================================================================================================================*/
/*															Start of Main Function																		*/
/*======================================================================================================================================================*/

int main(int argc, char* argv[])
{
	if (argc !=2) // If number of argument is not correct, print error message to logfile and exit
	{
		logfile("Incorrect number of arguments. One argument must be passed.");
		return 1;
	}
	ArgumentManager argMgr(argv[1],','); // Instantiate an ArgumentManager object
	
	string scriptName = argMgr.get("script"); // Gets name of scriptfile
	OperationQueue operQueue(scriptName); // Instantiate a OperationQueue object
	
/*=====================================================Declares variables needed ======================================================================*/
	List *lists[100]; // Declares an array of linked lists
	SingleLinkedList *postfix; // List to store postfix expression
	int listCounter = 0; // Counter for the number of lists array
	string line = ""; // Variable to store each line from the script file
	bool flag = false; // Returns true if unrecoverable error occurs
	bool read = false; // Returns true if read command has been executed
	
/*======================================================================================================================================================*/
/*															Start of ScriptFile Reading																	*/
/*======================================================================================================================================================*/	
	while(!operQueue.isEmpty() && !flag)
	{
		Operation op = operQueue.pop(); // Gets the next script command
		
		string str = op.getName();
		convertToLower(str);
		if (str.compare("read")!=0 && !read) // If first command is not read input file, print error message to logfile and get next script command
		{	
			logfile("No input file has been read yet.");
			continue;
		}
		if (str == "read") // Read file and create list
		{
			if(readFile(lists, listCounter, op, read)==0) // If input file can be read
			{
				if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
					lists[listCounter]->listEmpty = false;
				outputSize(lists[listCounter]); // Print size of list to logfile
				listCounter++; // Increments list counter after a new list is created
			}
		}
		else if (op.isExpression()) // Evaluate expression
		{
			if (infixToPostfix(op, postfix, lists, listCounter)==1)
				logfile(op.getExpression(), "is an invalid expression.");
		}
		else if (str == "write") // Write to file
			writeFile(lists, listCounter, op);
		else
			logfile(line, "cannot be executed."); // Prints invalid commands to logfile
	}
	
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

void logfile(string str1) // Outputs error messages to logfile
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Error: " << str1 << endl; // Prints the error message to the logfile
	logfile.close(); // Close the logfile
}
/*======================================================================================================================================================*/

void logfile(string str1, string str2) // Outputs error messages to logfile
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Error: \"" << str1 <<"\" " << str2 << endl; // Prints the error message to the logfile
	logfile.close(); // Close the logfile
}
/*======================================================================================================================================================*/

void outputSize(List* lists) // Prints size to logfile
{
	ofstream logfile;
	logfile.open("logfile.txt", ios::out | ios::app); // Opens the logfile to be appended
	logfile << "Size of list: \"" << lists->listName << "\" = " << lists->list->getSize() << " bytes" << endl; // Prints the size of list in bytes to the logfile
	logfile.close(); // Close the logfile
}
/*======================================================================================================================================================*/

int readFile(List* lists[], int listCounter, Operation op, bool& read)
{
	ifstream file;
	string filename = op.getParameter(1);
	getFileName(filename);
	file.open(filename.c_str()); // Opens file to be read
	
	if (!file.is_open())// If file cannot be opened, print error message to logfile and exit function
	{
		logfile(op.getParameter(1), "could not be opened.");
		return 1;
	}
			
	read = true; // Sets read flag to true, indicating file was read and list was created
	
	lists[listCounter] = new (nothrow) List(op.getParameter(0)); // Initializes a new list in the array
	if (lists[listCounter]->list == NULL) // Check for memory allocation exception
	{
		logfile("Memory allocation failed. List cannot be created.");
		return 2;
	}
		
	string word = ""; // Stores words from the file

	while(file >> word)
	{
		convertToLower(word);
		lists[listCounter]->list->createList(word, lists[listCounter]->list->head);
	}
	file.close(); // Closes input file
	return 0;
}
/*======================================================================================================================================================*/

int infixToPostfix(Operation oper, SingleLinkedList* postfix, List* lists[], int &listCounter)
{
	Stack <char> *operators = new Stack<char>(); // Stack to store operators
	postfix = new SingleLinkedList();
	string expression = oper.getExpression();
	int size = expression.length();
	bool flag = false; // Returns true if there is an error in the expression
	
	for (int i=0; i<size; i++)
	{
		if (isOperator(expression[i]))
		{
			char op = expression[i]; // Stores operator
			if(operators->empty() || op == '(')
				operators->push(op);
			else if (op == ')') // Handles closing parentheses
			{
				while (!operators->empty() && operators->top() != '(')
				{
					char c = '\0';
					operators->pop(c);
					string s(1, c);
					postfix->addNode(s);
				}
				if (operators->empty())
					return 1; // Indicates invalid expression
				else if (operators->top() == '(')
				{
					char c = '\0';
					operators->pop(c);
				}
				else
					return 1; // Indicates invalid expression
			}
			else if(op == '*' || op == '+')
			{
				while (!operators->empty() && operators->top() != '(' && op >= operators->top())
				{
					char c = '\0';
					operators->pop(c);
					string s(1, c);
					postfix->addNode(s);
				}
				operators->push(op);
			}
		}
		else
		{
			string value = "";
			while(i!=size && !isOperator(expression[i]))
				value += expression[i++];
			postfix->addNode(value);
			i--;
		}
	}
	while (!operators->empty() && !flag)
	{
		if(operators->top() == '(')
			return 1;  // Indicates invalid expression
		else
		{
			char c = '\0';
			operators->pop(c);
			string s(1, c);
			postfix->addNode(s);
		}
	}
	if (evaluateExpression(oper, postfix, lists, listCounter)==1)
		return 1;
	return 0;
}
/*======================================================================================================================================================*/

int evaluateExpression(Operation oper, SingleLinkedList* postfix, List* lists[], int &listCounter)
{
	Stack<string> *evaluate = new Stack<string>();
	postfix->current = postfix->head;
	char element;
	
	while(postfix->current != NULL)
	{
		element = postfix->current->data[0];
		if (element!='*' && element!='+')
		{
			evaluate->push(postfix->current->data);
			postfix->current = postfix->current->next;
		}
		else if (element=='+' || element=='*')
		{
			string opList = "";
			string list1 = "";
			string list2 = "";
			if (evaluate->empty()) // First pop
				return 1;
			else
				evaluate->pop(list1);
				
			if (evaluate->empty()) // Second pop
				return 1;
			else
				evaluate->pop(list2);
			opList = list1 + element + list2;
			
			if (element == '+')
				constructUnion(list1, list2, opList, lists, listCounter);
			else if (element == '*')
				constructIntersection(list1, list2, opList, lists, listCounter);
			else
				return 1;

			evaluate->push(lists[listCounter]->listName); // Push result of new union to stack
			listCounter++;
			postfix->current = postfix->current->next;
		}
		else
			return 1;
	}
	lists[listCounter-1]->listName = oper.getName();
	return 0;
}
/*======================================================================================================================================================*/

int constructUnion(string listName1, string listName2, string unionListName, List* lists[], int listCounter)
{
	int element1 = getListName(lists, listName1, listCounter); // Gets array element for first list
	int element2 = getListName(lists, listName2, listCounter); // Gets array element for second list
	
	if (element1 == -1 || element2 == -1) // If list(s) cannot be found, print error message to logfile and go to next scriptfile command
	{
		logfile("One or more lists needed to create union cannot be found");
		return 1;
	}
	
	lists[listCounter] = new (nothrow) List(unionListName); // Initializes a new list in the array
	if (lists[listCounter]->list == NULL) // Check for memory allocation exception
	{
		logfile("Memory allocation failed. List cannot be created.");
		return 1;
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
	
	//outputSize(lists[listCounter]); // Print size of list to logfile
	return 0;
}
/*======================================================================================================================================================*/

int constructIntersection(string listName1, string listName2, string intersectionListName, List* lists[], int listCounter)
{
	int element1 = getListName(lists, listName1, listCounter); // Gets array element for first list
	int element2 = getListName(lists, listName2, listCounter); // Gets array element for second list
	
	if (element1 == -1 || element2 == -1) // If list(s) cannot be found, print error message to logfile and go to next scriptfile command
	{
		logfile("One or more lists needed to create intersection cannot be found");
		return 1;
	}
	
	lists[listCounter] = new (nothrow) List(intersectionListName); // Initializes a new list in the array
	if (lists[listCounter]->list == NULL) // Check for memory allocation exception
	{
		logfile("Memory allocation failed. List cannot be created.");
		return 1;
	}
	
	if(lists[element1]->listEmpty || lists[element2]->listEmpty) // If one of the lists are empty, print error message to logfile
		logfile("One of more lists is empty. Intersection will be empty.");
	else  // Call function to create intersection list
		lists[listCounter]->list->createIntersection(lists[listCounter]->list->head, lists[element1]->list->head, lists[element2]->list->head);

	if(lists[listCounter]->list->head != NULL) // Sets listEmpty flag to false if list is occupied
		lists[listCounter]->listEmpty = false;
	
	//outputSize(lists[listCounter]); // Print size of list to logfile		
	return 0;
}
/*======================================================================================================================================================*/
int writeFile(List* lists[], int listCounter, Operation op)
{
	ofstream output;
	string filename = op.getParameter(1);
	getFileName(filename);
	output.open(filename.c_str()); // Opens file to be written using cmd.value1 as the output file name
			
	if (!output.is_open())  // If file cannot be opened, output error message to logfile
	{
		logfile(op.getParameter(1), "could not be written to.");
		return 1;
	}
	
	int element = getListName(lists, op.getParameter(0), listCounter); // Gets array element for list
	if (element == -1) // If list cannot be found, print error message to logfile and go to next scriptfile command
	{
		logfile(op.getParameter(0), "does not exist");
		output << "List " << op.getParameter(0) << " does not exist" << endl;
		return 1;	
	}
	
	string str = op.getParameter(2);
	convertToLower(str);
	if (str.compare("reverse")==0)
		lists[element]->list->writeReverse(lists[element]->list->head, output); // Function call to write to file in reverse
	else
		lists[element]->list->writeForward(lists[element]->list->head, output); // Function call to write to file
	
	output.close(); // Close file
	return 0;
}
/*======================================================================================================================================================*/

void convertToLower(string &word) // Converts string parameter to lowercase
{
	int size = word.length();
	
	for (int i=0; i<size; i++)
		word[i] = tolower(word[i]);
}
/*======================================================================================================================================================*/

bool isOperator(char c)
{
	return (c=='+' || c=='*' || c=='(' || c==')');
}
/*======================================================================================================================================================*/

void getFileName(string& file)
{
	file.erase(file.begin());
	file.erase(file.end()-1);
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