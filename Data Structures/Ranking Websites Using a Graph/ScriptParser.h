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
	};
	
public:
	cmdLine cmd; // Cmd: This property contains the parsed instruction
	ScriptParser(); // Constructor
	int operationCode(); // Translate operation (string) to operationCode (int)
	void parseLine(string); // Receives a string and updates the public property cmd
	string match(string, string);
};
#endif