#include "ScriptParser.h"

const string loadTemplate  = "load(#,'#')";
const string rankTemplate  = "rank(#,'#')";


// Constructor		
ScriptParser::ScriptParser()
{
	cmd.operation = cmd.listID= cmd.value1 = "";
}
		
// Translate operation (string) to operationCode (int)
int ScriptParser::operationCode()
{
	if  (cmd.operation== "load") return 1;
	if  (cmd.operation== "rank") return 2;
	return 0;
}

// Receives a string and updates the public property cmd
void ScriptParser::parseLine(string stringtobeparsed)
{
    string variables, values;
	unsigned int pos, posSlash;
	cmd.operation = cmd.listID= cmd.value1 = "";
	variables = match(stringtobeparsed, loadTemplate);
	if (variables!="")
		cmd.operation= "load";
	else
	{
		variables = match(stringtobeparsed, rankTemplate);
		if (variables!="")
			cmd.operation= "rank";
		else 					 
			cmd.operation= "error";
	}
	
	pos = variables.find(",");
	if (pos==string::npos)
		cmd.operation ="error";
	else
	{
		cmd.listID = variables.substr(0,pos); 
		values = variables.substr(pos+1,variables.length()-pos+1);
		cmd.value1 = values;
	}
}

string ScriptParser::match(string str1, string strtemplate)
{
	string strRet="";
	if (str1.length()<strtemplate.length()) 
		return "";
	
	int j=0;
	bool inquotes=false;
	bool isWrite = false;
	string oper="";
	bool gotseparator = false;
	for (unsigned int i=0; i<str1.length(); i++)
	{ 
		if (str1[i] != strtemplate[j])
		{
			if (strtemplate[j]=='#' )
			{ 
				if (inquotes)
				{
					if (str1[i]!='\'') 
						strRet+=str1[i];   
				}
				else
					if (isalnum(str1[i])) 
						strRet+=str1[i];							  
					else 
						return "";
				if (str1[i+1] == strtemplate[j+1]) 
					j++; 
			}
			else
				return "";
		}
		else
		{
			j++;
			oper+=str1[i];
			if (i==4 && oper=="write") 
			  isWrite=true;
			if (str1[i] ==',' && !gotseparator )
			{
			    strRet+=str1[i];
				if (isWrite) 
					gotseparator = true;
			}
			if ( str1[i] =='/')  
				strRet+=str1[i];			
			if (str1[i] =='\'' ) 
			if (!inquotes) 
				inquotes=true; 
			else 
				inquotes=false ;
		}
	}
	return strRet;
}