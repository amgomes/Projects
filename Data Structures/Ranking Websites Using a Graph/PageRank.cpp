#include "CommandLineParser.h"
#include "ScriptParser.h"
#include "Graph.h"
#include "VertexList.h"

#include <fstream>
using namespace std;

/*======================================================================================================================================================*/
/*															Function Declarations																		*/
/*======================================================================================================================================================*/
void logfile(string); // Outputs error messages to logfiles taking one string as a parameter
void logfile(string, string); // Outputs formatted error messages to logfiles taking two strings as parameters
int loadPages(Graph*, string); // Opens each filelists and adds each webpage as a vertex
int getLinks(Graph*, string); // Opens a webpage and adds the links it contains
bool validLink(Graph*, string); // Checks if string parameter is a valid link and updates inDegree and outDegree
void createVList(VertexList*, Graph*); // Creates a list of vertices ranked by inDegree
void convertToArray(Graph*, string[], int); // Copies pointers to each vertex to an array
void mergeSort(string[], int, int, string[]);
void merge(string[], int, int, int, int, string[]);
// int wordSearch(string[], int, int, string); // Returns element value if string parameter is found or -1 if not
string convertToLower(string); // Converts each letter in string parameter to lower and returns the string
string removePunctuation(string); // Removes all punctuation from beginning and ending of a word and returns it
int findGraph(Graph*[], string, int); // Returns element of graph that matches the string variable

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
	ScriptParser SParser = ScriptParser(); // Creates ScriptParser object
	
	ifstream script(scriptName); // Opens scriptfile
	if(!script.is_open()) // If scriptfile cannot be opened, print error message to logfile and exit
	{
		logfile(scriptName, "could not be opened. Program will now exit");
		return 1; // Exit main program
	}
	
/*=====================================================Declares variables needed ======================================================================*/
	Graph* graph[1000]; // Declares a graph
	int counter = 0; // Counter for the number of graphs in the array
	string line = ""; // Variable to store each line from the script file
	bool load = false; // Returns true if read command has been executed
	
/*======================================================================================================================================================*/
/*															Start of ScriptFile Reading																	*/
/*======================================================================================================================================================*/
	
	while(getline(script, line)) // Retrieves each command line from the scriptfile
	{
		SParser.parseLine(line); // Parses the line to determine command needed to excute
		if (SParser.operationCode()!=1 && !load) // If first command is not read input file, print error message to logfile and exit
		{	
			logfile("Load command has not been executed.");
			continue;
		}
/*======================================================================================================================================================*/
/*													Switch Statement For Scriptfile Commands															*/
/*======================================================================================================================================================*/
		switch (SParser.operationCode()) // Chooses which command to execute based on the operationCode
		{
/*======================================================================================================================================================*/
/*																Case 1 - Load																			*/
/*======================================================================================================================================================*/
		case 1: // Operation = "Load"
		{
			graph[counter] = new Graph(SParser.cmd.listID);
			if (loadPages(graph[counter], SParser.cmd.value1) == 1) // Get webpages
				break;
			
			load = true;			
			
			graph[counter]->current = graph[counter]->head;
			while (graph[counter]->current != NULL) // Loop to get links from each webpage
			{
				if (getLinks(graph[counter], graph[counter]->current->webpage) == 1)
					break;
				graph[counter]->current = graph[counter]->current->next;
			}
			
			// graph[counter]->print();
			counter++;
			break;
		}
/*======================================================================================================================================================*/
/*																Case 2 - Rank																			*/
/*======================================================================================================================================================*/
		case 2: // Operation = "Rank"
		{
			int element = findGraph(graph, SParser.cmd.listID, counter); // Find graph array element that matches the listID
			if (element == -1) // If graph cannot be found, print error message to logfile and go to next scriptfile command
			{
				logfile(SParser.cmd.listID, "cannot be found. Cannot perform rank.");
				break;
			}
			
			int nbBrokenLinks = 0; // Keeps count of the number of broken links in the graph
			int nbSinks = 0; // Keeps track of the number of sinks in the graph
			
			graph[element]->current = graph[element]->head;
			while(graph[element]->current != NULL) // Get number of broken links and number of sinks in graph
			{
				nbBrokenLinks += graph[element]->current->brokenLinks;
				if (graph[element]->current->outDegree == 0)
					nbSinks++;
				graph[element]->current = graph[element]->current->next;
			}
			
			string *arrBrokenLinks = new string[nbBrokenLinks]; // Array to store broken links
			string *arrSinks = new string[nbSinks]; // Array to store sinks
			
			int b = 0; // Counter for broken links array
			int s = 0; // Counter for sinks array
			graph[element]->current = graph[element]->head;
			while(graph[element]->current != NULL) // Add broken links and sinks to their respective arrays
			{
				if (graph[element]->current->brokenLinks !=0) // Loop through edges and find broken links
				{
					graph[element]->current->edge->current = graph[element]->current->edge->head;
					while (graph[element]->current->edge->current != NULL)
					{
						if (graph[element]->current->edge->current->isBroken)
							arrBrokenLinks[b++] = graph[element]->current->edge->current->link + " Broken Link";
						graph[element]->current->edge->current = graph[element]->current->edge->current->next;
					}
				}
				if (graph[element]->current->outDegree == 0) // Add sinks to array
					arrSinks[s++] = graph[element]->current->webpage + " Sink";
				graph[element]->current = graph[element]->current->next;
			}
			
			// Sort the arrays
			string *tempBrokenLinks = new string[nbBrokenLinks]; // Temp array to sort broken links array
			string *tempSinks = new string[nbSinks]; // Temp array to sort sinks array
			mergeSort(arrBrokenLinks, 0, nbBrokenLinks-1, tempBrokenLinks);
			mergeSort(arrSinks, 0, nbSinks-1, tempSinks);
			
			delete[] tempBrokenLinks;
			delete[] tempSinks;
			
			// Create vertex list
			VertexList *vertex = new VertexList();
			createVList(vertex, graph[element]);
			
			// Print results
			ofstream output;
			output.open(SParser.cmd.value1.c_str()); // Opens file to be written to using cmd.value1 as the filename
			
			if (!output.is_open())  // If file cannot be opened, output error message to logfile
			{
				logfile(SParser.cmd.value1, "could not be written to.");
				break;
			}
			vertex->current = vertex->head;
			for (int i=0; i<5 && vertex->current!=NULL; i++) // Print webpages and their inDegrees
			{
				output << vertex->current->webpage << " " << vertex->current->inDegree << endl;
				vertex->current = vertex->current->next;
			}
			output << endl;
			if (nbBrokenLinks == 0) // Print broken links
				output << "No Broken Links" << endl;
			else
				for (int i=0; i<nbBrokenLinks; i++)
					output << arrBrokenLinks[i] << endl;
			output << endl;
			
			if (nbSinks == 0) // Print sinks
				output << "No Sinks" << endl;
			else
				for (int i=0; i<nbSinks; i++)
					output << arrSinks[i] << endl;
			
			output.close();
			delete[] arrBrokenLinks;
			delete[] arrSinks;
			
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

int loadPages(Graph* graph, string filename)
{
	// Opens filelists
	ifstream file;
	file.open(filename.c_str()); // Opens file to be read
	if (!file.is_open())// If file cannot be opened, print error message to logfile and exit function
	{
		logfile(filename, "could not be opened.");
		return 1;
	}
	
	string word;
	while (file >> word) // Gets webpages
	{
		if(graph->addVertex(word) == 1)	// Adds webpage to list of vertices
		{
			logfile("Memory allocation for vertex failed.");
			return 1;
		}
		graph->nbWebpages++;
	}
	
	file.close();
	return 0;
}
/*======================================================================================================================================================*/
int getLinks(Graph* graph, string filename)
{
	// Get links from the webpage
	ifstream file;
	file.open(filename.c_str()); // Opens file to be read
	if (!file.is_open())// If file cannot be opened, print error message to logfile and exit function
	{
		logfile(filename, "could not be opened.");
		return 1;
	}

	string word;
	while (file >> word)
	{
		// Parse links
		string temp = word.substr(0, 4);
		temp = convertToLower(temp);
		if (temp.compare("href") == 0) // If "href" is found
		{
			if (word[4] != '=') // Handles "href ="
			{
				file >> word;
				if (word[0] == '=')
				{
					if (word[1] == '\0')
					{
						file >> word;
						word = removePunctuation(word);
						if (filename.compare(word) != 0) // Check to see if link references itself
						{
							if (validLink(graph, word)) // If link is valid
							{
								graph->current->edge->addLink(word, false);
								graph->current->outDegree++;
							}
							else if (graph->current->edge->current == NULL) // If link has not already been added, add broken link
							{
								graph->current->edge->addLink(word, true);
								graph->current->brokenLinks++;
							}
						}
					}
				}
			}
			else if (word[4] == '=') // Handles "href="
			{
				unsigned int pos = word.find("\"");
				if (pos==string::npos)
					break;
				word = word.substr(pos+1, word.length()-pos);
				pos = word.find("\"");
				if (pos==string::npos)
					break;
				word = word.substr(0, pos);
				if (filename.compare(word) != 0) // Check to see if link references itself
				{
					if (validLink(graph, word)) // If link is valid
					{
						graph->current->edge->addLink(word, false);
						graph->current->outDegree++;
					}
					else if (graph->current->edge->current == NULL) // If link has not already been added, add broken link
					{
						graph->current->edge->addLink(word, true);
						graph->current->brokenLinks++;
					}
				}
			}
		}
	}
	file.close();
	return 0;
}
/*======================================================================================================================================================*/

bool validLink(Graph* graph, string page)
{
	Graph* temp;
	temp->current = graph->current;
	graph->current = graph->head;
	while (graph->current != NULL) // Loop to get links from each webpage
	{
		if (graph->current->webpage.compare(page) == 0) // If webpage is found, increase inDegree of webpage
		{
			temp->current->edge->current = temp->current->edge->head;
			while (temp->current->edge->current != NULL) // Check if link is already in the list
			{
				if(temp->current->edge->current->link.compare(page) == 0) // If link is already in the list, return false
				{
					graph->current = temp->current;
					return false;
				}
				temp->current->edge->current = temp->current->edge->current->next;
			}
			graph->current->inDegree++; // Increase inDegree count
			graph->current = temp->current;
			return true;
		}
		graph->current = graph->current->next;
	}
	graph->current = temp->current;
	return false;
}
/*======================================================================================================================================================*/

void createVList(VertexList* vertex, Graph* graph)
{
	graph->current = graph->head;
	while (graph->current != NULL)
	{
		if(vertex->head == NULL) // Create head of list
			vertex->insertNode(graph->current->webpage, graph->current->inDegree);
		else // Find position to add node
		{
			vertex->current = vertex->head;
			while (vertex->current != NULL)
			{
				if (vertex->current->inDegree > graph->current->inDegree)
					vertex->current = vertex->current->next;
				else break;
			}
			// Sort same inDegrees in alphabetical order
			while (vertex->current != NULL)
			{
				string page = graph->current->webpage;
				if (vertex->current->inDegree == graph->current->inDegree) // Check if inDegree is the same
				{
					if (vertex->current->webpage.compare(page) < 0) // Compare strings alphabetically
						vertex->current = vertex->current->next;
					else break;
				}
				else break;
			}
			vertex->insertNode(graph->current->webpage, graph->current->inDegree);
		}
		graph->current = graph->current->next;
	}
}
/*======================================================================================================================================================*/

void convertToArray(Graph *graph, string arr[], int size)
{
	int i = 0;
	
	graph->current = graph->head;
	while (graph->current != NULL && i<size)
	{
		arr[i] = graph->current->webpage;
		graph->current = graph->current->next;
		i++;
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

string convertToLower(string word)
{
	int size = word.length();
	
	for (int i=0; i<size; i++)
		word[i] = tolower(word[i]);
		
	return word;
}
/*======================================================================================================================================================*/

string removePunctuation(string word)
{	
	while(ispunct(word[0])) // Check for punctuation marks at the beginning of the word
			word.erase(word.begin());
	
	int size = word.length();
	
	while(ispunct(word[--size])) // Check for punctuation marks at the end of the word
		word.resize(size);
		
	return word;
}
/*======================================================================================================================================================*/

int findGraph(Graph* graph[], string name, int listCounter)
{
	for(int i=0; i<listCounter; i++)
	{
		if (name.compare(graph[i]->graphName)==0)
			return i;
	}
	return -1;
}