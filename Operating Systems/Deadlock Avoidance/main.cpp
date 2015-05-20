/***************************************************************************************************************
Name: Adam Gomes
ID: 1105982
COSC 4330

Note:	-The program does not accept a command line argument. The input file has to be named "hw1f14input.txt".
		-Processes are indexed in the program from 0 to n-1 where n is the number of processes, but displayed
			from 1 to n when output to the screen for better readability

Program:
    This program implements the deadlock avoidance algorithm, Banker's Algorithm, to avoid deadlocks 
	in Unix/Linux systems. Resources are simulated, but real processes are created using fork(). The 
	resource-requesting child processes make requests by communicating with the parent deadlock 
	handling process using pipes. The deadlock-handling process executes one instruction at a time, 
	choosing the next process to service based on each of the following schedulers:
		1. LJF with EDF-Tie-Breaker
		2. EDF with LJF-Tie-Breaker
		
Implementation:
	The program runs twice, once with the LJF with EDF-Tie-Breaker scheduler and again with 
	the EDF with LJF-Tie-Breaker scheduler. It then prints out the total deadlines missed by
	each scheduler and compares them. Note that there is a slight pause between schedulers.
	
	After each instruction is attempted, the system will print the instruction, its result and
	the the current available table, the current allocated table, the current need table and the 
	processes that have missed their deadlines.
	
	The child processes are created using fork() in a loop. There is bidirectional communication
	between the child and the parent with the use of two pipes. The child writes the first instruction
	into its write pipe and waits for a signal from the parent before continuing. That signal could be:
		1. Write the next instruction (previous instruction was successfully serviced)
		2. Write the same instruction again (when there is not enough resources or request is unsafe)
		3. Terminate (when the child has no more instructions or is requesting more than it needs)
	The parent calls the scheduler to find out which process has the highest priority and reads from the
	pipe of that child.
	
Assumptions:
	1.	If an instruction for a process requests more than it needs, that process is immediately 
		terminated.
	2.	Terminated processes have all of their allocated resources released, computation 
		time set to 0, need set to 0 and its finished flag set to true.
	3.	A process will not be serviced twice in a row, unless it is the only remaining process
		with instructions.
	4.	Deadlines are decreased every time an instruction is attempted, regardless of if the instruction
		was successful or not, such as when it requests more than what is available.
	5.	All of the computations associated with a single instruction decreases deadline by only 1.
		For example, if an instruction failed the banker's algorithm, the process was terminated and its
		resources were released, the deadline for all processes only go down by 1.
***************************************************************************************************************/

#include "process.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sstream>
#include <iomanip>
#include <limits.h>
using namespace std;

int readFile(vector<Process>&, vector<int>&, vector <vector<int> >&, int&, int&); // Gets information from input file and stores them in the appropriate variables
void getAvailability(string, vector<int>&); //Gets and stores the availability of each resource into an avail vector
void getMax(string, vector <vector<int> >&); //Gets and stores the max for each resource into an max vector
int getProcessID(string); //Parses string to get process id
void parseInstruction(vector<int>&, string, int, int); //Takes a request/release instruction and stores it into a vector with the following format: [processID][request/release][resource1][resource2]... (Note: request/release stored as 0/1
bool isEnd(string); //Returns true if string "end" is found
void createProcesses(vector<Process>&, vector<int>&, vector <vector<int> >&, vector <vector<int> >&, int, int, int, int&); //creates processes using fork
int bankersAlgorithm(vector<Process>, vector<int>, vector <vector<int> >, vector <vector<int> >, vector<int>, int, int);
bool safe(vector<Process>, vector<int>, vector <vector<int> >, vector <vector<int> >, int, int); //utility function for banker's algorithm
int LJFScheduler(vector<Process>, int); // Returns process number with largest computation time (EDF tiebreaker)
int EDFScheduler(vector<Process>, int); // Returns process number with largest computation time (EDF tiebreaker)
void grantRequest(vector<int>&, vector <vector<int> >&, vector <vector<int> >&, vector<int>, int, int); //update vectors after request is granted
void grantRelease(vector<int>&, vector <vector<int> >&, vector<int>, int, int); //update vectors after release is granted
void releaseAll(vector<int>&, vector <vector<int> >&, vector <vector<int> >&, int, int, int); //update vectors after process ends
void decreaseDeadline(vector<Process>&, int&); //decrease deadline of each process by 1, setting missed deadline flag if deadline < 0
string instrToString(vector<int>); //converts instruction vector to string
void printStatus (vector<Process>, vector<int>, vector <vector<int> >, vector <vector<int> >, int, int); //prints state of system after each instruction

int main()
{
    int LJFDeadlinesMissed;
    int EDFDeadlinesMissed;
    for (int i=0; i<2; i++)
    {
        if (i == 0)
        {
            cout << "*****************************************************" << endl;
            cout << "*                                                   *" << endl;
            cout << "*    Starting LJF with EDF-Tie-Breaker Scheduler    *" << endl;
            cout << "*                                                   *" << endl;
            cout << "*****************************************************" << endl;
        }
        else if (i == 1)
        {
            cout << "*****************************************************" << endl;
            cout << "*                                                   *" << endl;
            cout << "*    Starting EDF with LJF-Tie-Breaker Scheduler    *" << endl;
            cout << "*                                                   *" << endl;
            cout << "*****************************************************" << endl;
        }
        /*Note: All two dimensional vectors have the following format:
            rows = processes, columns = resources */
        vector<Process> process; //holds instructions, computation time and deadline of each process
        vector<int> availResources; //current availability of each resource
        vector< vector<int> > maxResources; //max need of each resource a process will need
        int nbResources; //number of total resources
        int nbProcesses; //number of total processes
        int nbDeadlinesMissed = 0;

        if (readFile(process, availResources, maxResources, nbResources, nbProcesses) == 1)
        {
            cout << "File could not be opened. Please make sure file is named ." << endl;
            exit(1);
        }

        vector< vector<int> > allocatedResources (nbProcesses, vector<int>(nbResources, 0)); //allocated resources for each process
        vector< vector<int> > need = maxResources; //stores current need of each resource for each process

        createProcesses(process, availResources, allocatedResources, need, nbResources, nbProcesses, i, nbDeadlinesMissed); // creates processes

        if (i == 0)
        {
			cout << "\n*****************************************************" << endl;
            cout << "LJF scheduler has completed. \nPrinting final state of system.";
			cout << "\n*****************************************************" << endl;
            LJFDeadlinesMissed = nbDeadlinesMissed;
        }
        else if (i == 1)
        {
			cout << "\n*****************************************************" << endl;
            cout << "EDF scheduler has completed. \nPrinting final state of system.";
            EDFDeadlinesMissed = nbDeadlinesMissed;
			cout << "\n*****************************************************" << endl;
        }
        printStatus(process, availResources, allocatedResources, need, nbResources, nbProcesses);
        sleep(5);
    }

    cout << "LJF Scheduler had " << LJFDeadlinesMissed << " missed deadines." << endl;
    cout << "EDF Scheduler had " << EDFDeadlinesMissed << " missed deadines." << endl;
    if (LJFDeadlinesMissed < EDFDeadlinesMissed)
        cout << "LJF scheduler had fewer missed deadlines!" << endl;
    else if (EDFDeadlinesMissed < LJFDeadlinesMissed)
        cout << "EDF scheduler had fewer missed deadlines!" << endl;
    else
        cout << "Both schedulers had the same amount of deadline misses!" <<endl;
	return 0;
}

//Reads input file: 'hw1f14input.txt'
int readFile(vector<Process>& process, vector<int>& avail, vector <vector<int> >& maxResources, int& nbResources, int& nbProcesses) {
	ifstream file("hw1f14input.txt");

	if (!file.is_open())
			return 1;

	// get lines from input file
	string str;
	vector<string> lines;

	while (getline(file, str))
		lines.push_back(str);

	file.close();

	unsigned int counter = 0; // counter for the current line to be processed

	//get nbResources and nbProcesses
	nbResources = atoi(lines[counter++].c_str());
	nbProcesses = atoi(lines[counter++].c_str());

    //resize vectors
	avail.resize(nbResources);
	maxResources.resize(nbResources, vector<int>(nbProcesses));
	process.resize(nbProcesses);

	// get availability of each resource
	for (int i = 0; i < nbResources; i++)
		getAvailability(lines[counter++], avail);

	//get max of each resource
	for (int i = 0; i < nbResources * nbProcesses; i++)
		getMax(lines[counter++], maxResources);

    //get process information
    //Note: instructions vectors are in the form: [processNumber][request/release][resource1][resource2]....
	while (counter < lines.size())
	{
        int element = getProcessID(lines[counter++]) - 1;
        process[element].setProcessNum(element);
        process[element].setDeadline(atoi(lines[counter++].c_str()));
        process[element].setComputationTime(atoi(lines[counter++].c_str()));

        while (!isEnd(lines[counter]) && counter < lines.size())
        {
            vector<int> instr;
            parseInstruction(instr, lines[counter++], nbResources, element);
            process[element].addInstruction(instr);
        }
        counter++;
    }

	return 0;
}

void getAvailability(string str, vector<int>& avail) {
	int pos1 = str.find('[') + 1;
	int pos2 = str.find(']');
	int pos3 = str.find('=') + 1;
	int pos4 = str.find('\0');
	int element = atoi(str.substr(pos1, pos2 - pos1).c_str());
	int nb = atoi(str.substr(pos3, pos4 - pos3).c_str());
	avail[element - 1] = nb;
}

void getMax(string str, vector <vector<int> >& m) {
	int p; // process id
	int r; // resource id
	int value;
	int pos1 = str.find('[') + 1;
	int pos2 = str.find(',');
	p = atoi(str.substr(pos1, pos2 - pos1).c_str());
	int pos3 = str.find(']');
	r = atoi(str.substr(++pos2, pos3 - pos2).c_str());
	int pos4 = str.find('=') + 1;
	int pos5 = str.find('\0');
	value = atoi(str.substr(pos4, pos5 - pos4).c_str());
	m[p - 1][r - 1] = value;
}

int getProcessID(string str) {
	int pos1 = str.find('_') + 1;
	int pos2 = str.find('\0');
	return atoi(str.substr(pos1, pos2 - pos1).c_str());
}

void parseInstruction(vector<int>& v, string str, int nb, int element)
{
    v.push_back(element);
    int pos = str.find('(');
    if (str.substr(0,pos) == "request")
        v.push_back(0);
    else if (str.substr(0,pos) == "release")
        v.push_back(1);
    else cout << "error parsing instruction" << endl;

    for (int i=0; i<nb; i++)
    {
        str = str.substr(pos+1, str.size());
        pos = str.find(',');
        v.push_back(atoi(str.substr(0, pos).c_str()));
    }
}

bool isEnd (string str) {
	if(str[0] == 'e' && str[1] == 'n' && str[2] == 'd')
		return true;
	else
		return false;
}

void createProcesses(vector<Process>& process, vector<int>& avail, vector <vector<int> >& alloc, vector <vector<int> >& need, int nbResources, int nbProcesses, int scheduler, int& misses)
{
    vector<int> cid(nbProcesses); //stores id of each created child process
    int parentToChild[nbProcesses][2]; //pipe allowing parent to write to child
    int childToParent[nbProcesses][2]; //pipe allowing child to write to parent
    bool child = false; //true if process is a child
    int childIndex = -1; //index for each child (used to make correct pipe is written too)
    int buf; //buffer used to read and write from pipes
    int result = 1; //1 if instruction was completed, 2 if instruction has to wait and 3 if process needs to terminate
    int lastProcessed = -1; //stores last process serviced to allow another to get priority next

    for (int i=0; i<nbProcesses; i++) //create children
    {
        {
            if(pipe(parentToChild[i]) == -1 || pipe(childToParent[i]) == -1)
            {
                cout << "Error creating pipe. Program will now exit.";
                exit(1);
            }

            cid[i] = fork();
            if(cid[i] == -1)
            {
                cout << "Error creating pipe. Program will now exit.";
                exit(1);
            }

            if (cid[i] == 0)
            {
                child = true;
                childIndex = i;
                break; //prevents children of children from being created
            }
        }
    }

    if (child) //child process
    {
        close(childToParent[childIndex][0]); //close read end of pipe
        close(parentToChild[childIndex][1]); //close write end of pipe

        int currentProcess = 0; //counter for current process to be executed for this process
        int signal; //1 if instruction was completed, 2 if instruction needs to wait, 3 if process needs to terminate
        while (signal != 3) //loop until all processes are told to terminate
        {
            vector<int> instr = process[childIndex].getInstruction(currentProcess);
            instr.push_back(-1); //signals end of instruction
            for (int j=0; j<instr.size(); j++) //loop through a single instruction
            {
                buf = instr[j];
                write(childToParent[childIndex][1], &buf, sizeof(buf)); //write instruction to pipe
            }
            while (read(parentToChild[childIndex][0], &signal, sizeof(signal)) == 0)
                sleep(1);
            if (signal == 1) //increment current instruction counter if instruction was completed
                currentProcess++;
        }
        close(childToParent[childIndex][1]); //close write end of pipe
        close(parentToChild[childIndex][0]); //close read end of pipe
        _exit(0); //exit child process
    }
    else //parent process
    {
        for (int i=0; i<nbProcesses; i++)
        {
            close(parentToChild[i][0]); //close read end of pipe
            close(childToParent[i][1]); //close write end of pipe
        }
        int priority;
		if (scheduler == 0) //LJF with EDF-Tie-Breaker
				priority = LJFScheduler(process, lastProcessed); //returns process with highest priority or -1 if all are finished
		else if (scheduler == 1) //EDF with LJF-Tie-Breaker
			priority = EDFScheduler(process, lastProcessed); //returns process with highest priority or -1 if all are finished
        while (priority != -1)
        {
            vector<int> instr; //stores instruction read from child
            while (read(childToParent[priority][0], &buf, sizeof(buf)) >0) //read from child
            {
                if (buf == -1) //end of instruction signal
                    break;
                instr.push_back(buf);
            }
            if (instr.size() > 0) //making sure instruction is valid
            {
                string resultString = "";
                if (instr[1] == 0) //REQUEST instruction
                {
                    int bankersResult = bankersAlgorithm(process, avail, alloc, need, instr, nbResources, nbProcesses);
                    if( bankersResult == 0) //request is safe
                    {
                        grantRequest(avail, alloc, need, instr, nbResources, nbProcesses);
                        resultString += instrToString(instr) + "is safe. Granting request.";;
                        result = 1;
                    }
                    else if (bankersResult == 1) //failed safe check
                    {
                        resultString += instrToString(instr) + "is not safe. \nProcess will wait.";
                        result = 2;
                    }
                    else if (bankersResult == 2) //requesting more resources than what is available
                    {
                        resultString += instrToString(instr) + "is requesting than what is available. \nProcess will wait.";
                        result = 2;
                    }
                    else if (bankersResult == 3) //requesting more than it's need. terminate process
                    {
                        resultString += instrToString(instr) + "is requesting more than it needs. \nProcess will terminate.";
                        result = 3;
                    }

                }
                else if (instr[1] == 1) //RELEASE instruction
                {
                    resultString += instrToString(instr) + "has been granted.";
                    grantRelease(avail, alloc, instr, nbResources, nbProcesses);
                    result = 1;
                }

				buf = result;
                if (result == 1) //decrease computation time of process and tell child to write next instruction
                {
                    process[priority].setComputationTime(process[priority].getComputationTime() -1);
                    if (process[priority].getComputationTime() == 0) //if no more instructions, send terminate signal
                    {
                        process[priority].setFinished(true); //set finished flag to true
                        releaseAll(avail, alloc, need, priority, nbResources, nbProcesses);
                        buf = 3;
                    }
                    write(parentToChild[priority][1], &buf, sizeof(buf));
                    sleep(1);
                }
                else if (result == 2) //tell child to write same instruction again
                {
                    write(parentToChild[priority][1], &buf, sizeof(buf));
                    sleep(1);
                }
                else if (result == 3) //tell process to terminate
                {
                    process[priority].setComputationTime(0); //change computation time of terminated process to 0
                    process[priority].setFinished(true); //set finished flag to true
                    releaseAll(avail, alloc, need, priority, nbResources, nbProcesses);
                    write(parentToChild[priority][1], &buf, sizeof(buf));
                    sleep(1);
                }
                decreaseDeadline(process, misses); //decrease deadline for all processes
                lastProcessed = priority;
                if (scheduler == 0) //LJF with EDF-Tie-Breaker
					priority = LJFScheduler(process, lastProcessed); //returns process with highest priority or -1 if all are finished
				else if (scheduler == 1) //EDF with LJF-Tie-Breaker
					priority = EDFScheduler(process, lastProcessed); //returns process with highest priority or -1 if all are finished

                //print results
                cout << resultString << endl;
                printStatus(process, avail, alloc, need, nbResources, nbProcesses);
            }
        }
        for (int i=0; i<nbProcesses; i++) //wait for all child processes to exit
            waitpid(cid[i], NULL, 0);
        for (int i=0; i<nbProcesses; i++) //close remaining pipes
        {
            close(parentToChild[i][1]);
            close(childToParent[i][0]);
        }
    }
}

int bankersAlgorithm(vector<Process> process, vector<int> avail, vector <vector<int> > alloc, vector <vector<int> > need, vector<int> request, int nbResources, int nbProcesses)
{
    //Note: instructions vectors are in the form: [processNumber][request/release][resource1][resource2]....
    for (int i=0; i<nbResources; i++)
    {
        if(request[i+2] > need[request[0]][i]) //requesting more than need
            return 3;
        else if (request[i+2] > avail[request[i]]) //requesting more than available
            return 2;
    }

    vector<int> availTemp(nbResources); //temporary vector for avail resources
    for (int i=0; i<nbResources; i++)
        availTemp[i] = avail[i] - request[i+2];

    vector <vector<int> > needTemp = need; //temporary vector for need of each process
    for (int i=0; i<nbResources; i++)
        needTemp[request[0]][i] = need[request[0]][i] - request[i+2];

    vector <vector<int> > allocTemp = alloc; //temporary vector for allocated resources
    for (int i=0; i<nbResources; i++)
        allocTemp[request[0]][i] = alloc[request[0]][i] + request[i+2];

    if(safe(process, availTemp, needTemp, allocTemp, nbResources, nbProcesses)) //request can be granted
    {
        //update avail, need and alloc vectors
        avail = availTemp;
        alloc = allocTemp;
        need = needTemp;
        return 0;
    }
    else //request is not safe
        return 1;
}

bool safe(vector<Process> p, vector<int> avail, vector <vector<int> > need, vector <vector<int> > alloc, int nbResources, int nbProcesses)
{
	vector<int> work = avail;
	vector<bool> finish(nbProcesses, false);

    //counts the number of processes that cannot finish, resetting once a process that can finish is found
    //if counter equals the number of processes still in the loop, the safety check fails
	int counter = 0;
	int nbProcessesLeft = nbProcesses;

	// Find i such that finish[i]=false and need[i]<=work
	while (counter < nbProcessesLeft)
	{
        for(int i=0; i<nbProcesses; i++)
        {
            bool flag = true;
            if (!finish[i])
            {
                for (int j= 0; j<nbResources; j++) //make sure there is enough of each resource for the process to finish
                {
                    if (need[i][j] > work[j])
                    {
                        flag = false;
                        break;
                    }
                }
            }
            if(flag) //if process can finish, add its allocated resources back to work(available)
            {
                for (int j=0; j<nbResources; j++)
                    work[j] = work[j] + alloc[i][j];
                finish[i] = true;
                nbProcessesLeft--; //decrement number of process left
                counter = 0; //reset counter
            }
            else if (!flag) //if process cannot finish, increment counter
                counter++;
        }
    }

    for (int i=0; i<nbProcesses; i++)
    {
        if (!finish[i])
            return false;
    }
    return true;
}

int LJFScheduler(vector<Process> process, int lastProcessed)
{
	//Note: scheduler does not return any processes that have finished or the last process that was serviced unless it is the only process remaining
    Process longest;
    longest.setProcessNum(-1);
    int counter = 0; //counter for number of finished processes
	for (int i=0; i<process.size(); i++)
	{
        if (process[i].getFinished())
            counter++;
        else if (lastProcessed != i)
        {
            if (process[i].getComputationTime() > longest.getComputationTime())
                longest = process[i];
            else if(process[i].getComputationTime() == longest.getComputationTime()) //tiebreaker is earliest deadline first
            {
                if (process[i].getDeadline() < longest.getDeadline())
                    longest = process[i];
            }
        }
	}

	if (counter == process.size()) //all processes have finished
        return -1;
    else if (counter == process.size()-1) //if there is only one process left
    {
        for (int i=0; i<process.size(); i++)
        {
            if (!process[i].getFinished())
            {
                longest = process[i];
                break;
            }
        }
    }
    return longest.getProcessNum();
}

int EDFScheduler(vector<Process> process, int lastProcessed)
{
	//Note: scheduler does not return any processes that have finished or the last process that was serviced unless it is the only process remaining
    Process earliest;
    earliest.setProcessNum(-1);
    earliest.setDeadline(INT_MAX);

    int counter = 0; //counter for number of finished processes
	for (int i=0; i<process.size(); i++)
	{
        if (process[i].getFinished())
            counter++;
        else if (lastProcessed != i)
        {
            if (process[i].getDeadline() < earliest.getDeadline())
                earliest = process[i];
            else if(process[i].getDeadline() == earliest.getDeadline()) //tiebreaker is longest job first first
            {
                if (process[i].getComputationTime() > earliest.getComputationTime())
                    earliest = process[i];
            }
        }
	}

	if (counter == process.size()) //all processes have finished
        return -1;
    else if (counter == process.size()-1) //if there is only one process left
    {
        for (int i=0; i<process.size(); i++)
        {
            if (!process[i].getFinished())
            {
                earliest = process[i];
                break;
            }
        }
    }
    return earliest.getProcessNum();
}

void grantRequest(vector<int>& avail, vector <vector<int> >& alloc, vector <vector<int> >& need, vector<int> request, int nbResources, int nbProcesses)
{
    //Note: instructions vectors are in the form: [processNumber][request/release][resource1][resource2]....
    int p = request[0];
    for (int r=0; r<nbResources; r++)
    {
        avail[r] = avail[r] - request[r+2];
        alloc[p][r] = alloc[p][r] + request[r+2];
        need[p][r] = need[p][r] - request[r+2];
    }
}

void grantRelease(vector<int>& avail, vector <vector<int> >& alloc, vector<int> release, int nbResources, int nbProcesses)
{
    //Note: instructions vectors are in the form: [processNumber][request/release][resource1][resource2]....
    int p = release[0];
    for (int r=0; r<nbResources; r++)
    {
        avail[r] = avail[r] + release[r+2];
        alloc[p][r] = alloc[p][r] - release[r+2];
    }
}

void releaseAll(vector<int>& avail, vector <vector<int> >& alloc, vector <vector<int> >& need, int i, int nbResources, int nbProcesses)
{
    for (int r=0; r<nbResources; r++)
    {
        avail[r] = avail[r] + alloc[i][r];
        alloc[i][r] = 0;
        need[i][r] = 0;
    }
}

void decreaseDeadline(vector<Process>& p, int& missed)
{
    for (int i=0; i<p.size(); i++)
    {
        p[i].setDeadline(p[i].getDeadline() - 1);
        if (p[i].getDeadline() < 0)
        {
            p[i].setMissedDeadline(true);
            missed++;
        }
    }
}

void printStatus(vector <Process> process, vector<int> avail, vector <vector<int> > alloc, vector <vector<int> > need, int nbResources, int nbProcesses)
{
	int w = 3;
	string s = "|";
	string line = "";
	for (int i=0; i<nbResources+1; i++) //line "------------"
		line += "-----";

	cout << "Available: " << endl;
	for (int i=0; i<nbResources; i++)
		cout << "  Resource " << i+1 << ": " << avail[i] << endl;

	cout << "\nAllocated: " << endl << setw(w+1) << s;
	for (int i=0; i<nbResources; i++) //prints resources
		cout << setw(w) << i+1;
	cout << endl << line << endl; //prints line
	for (int i=0; i<nbProcesses; i++)
	{
		cout  << right << setfill(' ') << setw(w) << i+1 << "|";
		for (int j=0; j<nbResources; j++)
			cout << setw(w) << alloc[i][j];
		cout << endl;
	}

	cout << endl;
	cout << "Need: " << endl << setw(w+1) << s;
	for (int i=0; i<nbResources; i++) //prints resources
		cout << setw(w) << i+1;
	cout << endl << line << endl; //prints line
	for (int i=0; i<nbProcesses; i++)
	{
		cout  << right << setfill(' ') << setw(w) << i+1 << "|";
		for (int j=0; j<nbResources; j++)
			cout << setw(w) << need[i][j];
		cout << endl;
	}

	cout << endl;
	bool flag = true;
	for (int i=0; i<nbProcesses; i++)
	{
		if (process[i].getMissedDeadline())
		{
			cout << "Process " << i+1 << " missed it's deadline." << endl;
			flag = false;
        }
    }
    if (flag)
        cout << "No processes have missed their deadline yet" << endl;

    cout << "\n**************************************************" << endl;
}

string instrToString(vector<int> v)
{
    //Note: instructions vectors are in the form: [processNumber][request/release][resource1][resource2]....
    string str;
    if (v[1] == 0)
        str += "Request ";
    else if (v[1] == 1)
        str += "Release ";
    for (int i=2; i<v.size(); i++)
    {
            stringstream ss;
            ss << v[i];
            str += "[" + ss.str() + ']';
    }
    stringstream ss;
    ss << v[0] + 1;
    str += " for Process " + ss.str() + " ";
    return str;
}
