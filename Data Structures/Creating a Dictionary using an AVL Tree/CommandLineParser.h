#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

class CommandLineParser
{
private:
	char *input;
	char delimiter;
	char *variable;
	char *temp;

public:
	CommandLineParser(char*,char); // Constructor to set input and delimiter values
    char* extract(char*); // Method to extract the variable value
};
#endif