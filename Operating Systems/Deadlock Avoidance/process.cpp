#include "process.h"

Process::Process()
{
    processNum = 0;
	deadline = 0;
	computationTime = 0;
	finished = false;
	missedDeadline = false;
}

void Process::setProcessNum(int num)
{
    processNum = num;
}

int Process::getProcessNum()
{
    return processNum;
}

void Process::setDeadline(int newDeadline)
{
	deadline = newDeadline;
}

int Process::getDeadline()
{
	return deadline;
}

void Process::setComputationTime(int newComputationTime)
{
	computationTime = newComputationTime;
}

int Process::getComputationTime()
{
	return computationTime;
}

void Process::setFinished(bool flag)
{
	finished = flag;
}

bool Process::getFinished()
{
	return finished;
}

void Process::setMissedDeadline(bool flag)
{
	missedDeadline = flag;
}

bool Process::getMissedDeadline()
{
	return missedDeadline;
}

void Process::addInstruction(vector<int> v)
{
    instructions.push_back(v);
}

vector<int> Process::getInstruction(int element)
{
	return instructions[element];
}

Process::~Process()
{
}
