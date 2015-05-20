#include "CommandLineParser.h"

//Assigns the argument & delimiter value when the object of the class is created
CommandLineParser::CommandLineParser(char *argument,char delimit)
{
	input=argument;
	delimiter=delimit;
	variable = 0;
	temp = 0;
}

// Method to extract the value for a given variable from the command line input
char* CommandLineParser::extract(char *variableName)
{
    bool variableFound = false;
	bool variableNameFound = true;
	variable = new char[50]; // Char array to hold the value of the variable
	temp = new char[50]; // Temp array to hold a partial string to find the variable name
	int variableLength; // Length of the variable name
	int variableCount; // Counter for the variable array
	int inputLength; // Length of the command line argument
	int equalCount; // Index to find '='

	for(inputLength = 0; input[inputLength] != '\0'; inputLength++); // Get the length of the command line argument
	for(variableLength = 0; variableName[variableLength] != '\0'; variableLength++); // Get the length of the variable

	// Loop through the command line argument and find the variable name and get the variable value
	for(int i = 0; (input[i] != '\0' || input[i] != delimiter) && !variableFound && i < inputLength - variableLength + 1; i+=equalCount)
	{
		// Clear the temp array
		for(int j = 0; j < 50; j++)
			temp[j] = '\0';

		// Find the variable name
		for(equalCount = 0; equalCount < variableLength && variableNameFound; equalCount++)
		{
			temp[equalCount] = input[i+equalCount];
			
			// If the equalCount'th element of temp and variable name are not equal, we did not find the variable and we move on
			if(temp[equalCount] != variableName[equalCount])
				variableNameFound = false;
		}

		// If we have a variable name match, we check the next char to see if it is '=', otherwise we have not found the variable
		if(variableNameFound && input[i+equalCount] != '=')
			variableNameFound = false;

		// If we found the variable name followed by '=', we get the value of the variable
		if(variableNameFound) 
		{
			// Loop through the input to get the variable value
			for(variableCount = 0; input[i+equalCount+1+variableCount] != '\0' && input[i+equalCount+1+variableCount] != delimiter; variableCount++) 
				variable[variableCount] = input[i+equalCount+1+variableCount];
			variable[variableCount] = '\0'; // Set the end line flag

			return variable;
		}
		else
			variableNameFound = true; // Reset the variable name found flag to restart the search
	}

	return 0; // Return NULL if the variable name is not in the command line argument
}