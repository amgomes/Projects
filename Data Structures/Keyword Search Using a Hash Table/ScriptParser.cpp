#include "ScriptParser.h"

const string loadTemplate  = "load(#,'#')";
const string filterTemplate  = "filter(#,'#')";
const string indexTemplate  = "index(#)";
const string searchTemplate  = "search(#,'#',#)";
const string readTemplate  = "read(#,'#')";
const string insertTemplate  = "insert(#,'#/#')";
const string deleteTemplate  = "delete(#,'#')";
const string writefTemplate1  = "write(#,'#')";
const string writefTemplate2  = "write(#,'#',forward)";
const string writerTemplate  = "write(#,'#',reverse)";
const string unionTemplate   =  "union(#,#,#)";
const string intersectionTemplate   =  "intersection(#,#,#)";
const string checkTemplate  = "check(#,#)";


// Constructor		
ScriptParser::ScriptParser()
{
	cmd.operation = cmd.listID= cmd.value1 = cmd.value2= cmd.value3= 
	cmd.value4= cmd.value5= cmd.corpus= "";
}
		
// Translate operation (string) to operationCode (int)
int ScriptParser::operationCode()
{
	if  (cmd.operation== "load") return 1;
	if  (cmd.operation== "filter") return 2;
	if  (cmd.operation== "index") return 3;
	if  (cmd.operation== "search") return 4;
	if  (cmd.operation== "read") return 5;
	if  (cmd.operation== "insert") return 6;
	if  (cmd.operation== "delete")   return 7;
	if  (cmd.operation== "fwrwrite") return 8;
	if  (cmd.operation== "revwrite") return 9;
	if  (cmd.operation== "union") return 10;
	if  (cmd.operation== "intersection") return 11;
	if  (cmd.operation== "check") return 12;
	return 0;
}

// Receives a string and updates the public property cmd
void ScriptParser::parseLine(string stringtobeparsed)
{
    string variables, values;
	unsigned int pos, posSlash;
	cmd.operation = cmd.listID= cmd.value1 = cmd.value2="";
	variables = match(stringtobeparsed, loadTemplate);
	if (variables!="")
		cmd.operation= "load";
	else
	{
		variables = match(stringtobeparsed, filterTemplate);
		if (variables!="")
			cmd.operation= "filter";
		else
		{
			variables = match(stringtobeparsed, indexTemplate);
			if (variables!="")
				cmd.operation= "index";
			else
			{
				variables = match(stringtobeparsed, searchTemplate);
				if (variables!="")
					cmd.operation= "search";
				else
				{
					variables = match(stringtobeparsed, readTemplate); 
					if (variables!="") 
						cmd.operation= "read";
					else
					{
						variables = match(stringtobeparsed, insertTemplate); 
						if (variables!="")
							cmd.operation= "insert";
						else
						{
							variables = match(stringtobeparsed, deleteTemplate); 
							if (variables!="") 
								cmd.operation= "delete";
							else
							{
								variables = match(stringtobeparsed, checkTemplate); 
								if (variables!="") 
									cmd.operation= "check";
								else
								{
									variables = match(stringtobeparsed, writefTemplate1);
									if (variables!="") 
										cmd.operation= "fwrwrite";
									else
									{
										variables = match(stringtobeparsed, writefTemplate2);
										if (variables!="") 
										cmd.operation= "fwrwrite";
										else
										{
											variables = match(stringtobeparsed, writerTemplate); 
											if (variables!="")
												cmd.operation= "revwrite";
											else  
											{
												variables = match(stringtobeparsed, unionTemplate); 
												if (variables!="")
													cmd.operation= "union";
												else  
												{
													variables = match(stringtobeparsed, intersectionTemplate); 
													if (variables!="") 
														cmd.operation= "intersection";
													else 					 
														cmd.operation= "error";
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(cmd.operation == "index")
	{
		cmd.listID = variables;
		return;
	}
	pos = variables.find(",");
	if (pos==string::npos) 
		cmd.operation ="error";
	else
	{
		cmd.listID = variables.substr(0,pos); 
		values = variables.substr(pos+1,variables.length()-pos+1);
		if (cmd.operation=="search")
		{
			int count = 0;
			variables=variables.substr(pos+1,variables.length()-pos+1);
			pos = variables.find(",");
			cmd.value1 = variables.substr(0,pos); // Value1
			count++;
			variables=variables.substr(pos+1,variables.length()-pos+1);
			pos = variables.find(",");
			if (pos!=string::npos) // Value2
			{
				cmd.value2 = variables.substr(0,pos);
				count++;
				variables=variables.substr(pos+1,variables.length()-pos+1);
				pos = variables.find(",");
				if (pos!=string::npos) // Value3
				{
					cmd.value3 = variables.substr(0,pos);
					count++;
					variables=variables.substr(pos+1,variables.length()-pos+1);
					pos = variables.find(",");
					if (pos!=string::npos) // Value4
					{
						cmd.value4 = variables.substr(0,pos);
						count++;
						variables=variables.substr(pos+1,variables.length()-pos+1);
						pos = variables.find(",");
						if (pos!=string::npos) // Value5
						{
							cmd.value5 = variables.substr(0,pos);
							count++;
							variables=variables.substr(pos+1,variables.length()-pos+1);
							pos = variables.find(",");
							if (pos!=string::npos) // Corpus
							{
								cmd.corpus = variables.substr(0,pos);
								variables=variables.substr(pos+1,variables.length()-pos+1);
								cmd.nbSearches = count;
							}
							else
							{
								cmd.corpus = variables;
								cmd.nbSearches = count;
							}
						}
						else
						{
							cmd.corpus = variables;
							cmd.nbSearches = count;
						}
					}
					else
					{
						cmd.corpus = variables;
						cmd.nbSearches = count;
					}
				}
				else
				{
					cmd.corpus = variables;
					cmd.nbSearches = count;
				}
			}
			else
			{
				cmd.corpus = variables;
				cmd.nbSearches = count;
			}
		}
		else if (cmd.operation=="union" || cmd.operation=="intersection" )
		{
            variables=variables.substr(pos+1,variables.length()-pos+1);
			pos = variables.find(",");
			cmd.value1 = variables.substr(0,pos); 
			variables=variables.substr(pos+1,variables.length()-pos+1);
			//pos = variables.find(",");
			//cmd.value2 = variables.substr(pos+1,variables.length()-pos+1);
			cmd.value2= variables;
		}
		else if (cmd.operation=="insert")
		{
			posSlash = values.find("/");
			if (posSlash ==string::npos) 
				cmd.operation ="error";
				else
				{   cmd.value1=values.substr(0,posSlash);
					cmd.value2=values.substr(posSlash+1,values.length()-posSlash+1);
				}
		}
		else
			cmd.value1 =values;
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
		if(oper=="index(")
		{
			string s = str1.substr(i+1, str1.length()-1);
			int pos = s.find(")");
			strRet = s.substr(0, pos);
			return strRet;
		}
	}
	return strRet;
}