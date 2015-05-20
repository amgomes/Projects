#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
using namespace std;

class Process
{

private:
    int processNum;
	int deadline;
	int computationTime;
	bool finished;
	bool missedDeadline;
    vector< vector<int> > instructions; //[processNum][request/release][resource1][resource2]....

public:
    Process();
    void setProcessNum(int);
	int getProcessNum();
	void setDeadline(int);
	int getDeadline();
	void setComputationTime(int);
	int getComputationTime();
	void setFinished(bool);
	bool getFinished();
	void setMissedDeadline(bool);
	bool getMissedDeadline();
	void addInstruction(vector<int>);
	vector<int> getInstruction(int);
	~Process();
};

#endif
