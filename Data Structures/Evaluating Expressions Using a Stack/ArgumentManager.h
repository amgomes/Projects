#ifndef ARG_MGR_H
#define ARG_MGR_H

#include <map>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class ArgumentManager
{
	private:
		map<string, string> argumentMap;
		//void mergeMap(map<string, string> &source, map<string, string> &destination);
	public:
		ArgumentManager(){ }
		ArgumentManager(int _argc, char *argv[], char delimiter=';');
		ArgumentManager(string rawArguments, char delimiter=';');
		void parse(int argc, char *argv[], char delimiter=';');
		void parse(string rawArguments, char delimiter=';');
		string get(string argumentName);
		string toString();
		friend ostream& operator << (ostream &out, ArgumentManager &am);
};

#endif
