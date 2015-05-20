#ifndef SCRIPTPARSER_H
#define SCRIPTPARSER_H

#include <string>
#include <iostream>

using namespace std;

class ScriptParser  
{
	struct cmdLine
	{
		string operation; // Insert/delete/read
		string listID;    // Identification of the list
		string value1;    // Parameter string
		string value2;    // Second parameter string (used only for search/insert operations)
		string value3;	  // Third parameter string (used only for search operations)
		string value4;    // Fourth parameter string (used only for search operations)
		string value5;    // Fifth parameter string (used only for search operations)
		string corpus;    // Used to store name of corpus in search operations
		int nbSearches;	  // Used to store the number of search values (1-5) in search operations
	};
	
public:
	cmdLine cmd; // Cmd: This property contains the parsed instruction
	ScriptParser(); // Constructor
	int operationCode(); // Translate operation (string) to operationCode (int)
	void parseLine(string); // Receives a string and updates the public property cmd
	string match(string, string);
};
#endif