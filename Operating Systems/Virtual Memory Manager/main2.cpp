/***************************************************************************************************************
Name: Adam Gomes
ID: 1105982
COSC 4330

Note: The program does not accept a command line argument. The input file has to be named "hw3input.txt".
      The program will need to be compiled with -lpthread.
      Ex. g++ main.cpp -lpthread

Program:
    This program simulates a virtual memory manager with paged segmentation and a simulated disk. It runs an input file
    "hw3input.txt" fifteen times, each time with a different page replacement and disk-scheduling algorithm combination.
    The page replacement algorithms used are:
            FIFO (First-In-First-Out)
            LRU (Least Recently Used)
            LFU (Least Frequently Used) - with LRU as a tie-breaker
            OPT-Lookahead-X(Optimum with Lookahead of X)
            WS (Working Set)
    Each page replacement algorithm will be run with each of the following disk-scheduling algorithms:
            FIFO (First-In-First-Out)
            SSTF (Shortest Seek Time First)
            SCAN (Elevator)
    This will give a total of 15 combinations (5 page replacement algorithms * 3 disk-sheduling algorithms).

Implementation:

	The program will first read	everything from the input file and will store them in the appropriate data structures.
	It will then create shared memory for use between the following processes:
                Page Fault Handler
                Page Replacement Process
                Disk Driver Process
                Paging Disk Process
    These processes are synchronized by the use of semaphores (there is one for each of the aforementioned processes, one for
    each process's segment and page table, and one for any other shared memory that is used).
    The role of these processes and the order in which they operate are explained below:
            1) Main Process:
                The main process starts reading requests. Before handling a request, the main process will wait on the semaphore
                of the requesting process. This will ensure that it is not reading outdated information.
                After waiting, the main process checks if the page requested is already in main memory (using the frames table).
                If the request is a page hit, the main process updates counters if necessary and proceeds to the next request.
                If it is a page fault, the main process writes the process id and address to shared memory and signals the
                semaphore for the page fault handler, frees the requesting process's semaphore and waits for a response.
            2) Page Fault Handler
                When the page fault handler is signaled, it reads the information from the shared memory, locks the requesting
                process's semaphore and sends a signal back to the main process so that it can keep running. (This increases
                productivity as the main process does not have to wait for the page fault to be fully handled before reading the
                next request, which may not be a page fault).
                The page fault checks if there are still available pages in the requesting process's memory space. If there is,
                it calls the disk driver, waits for the page to be retrieved and then updates the necessary variables. It then
                releases the requesting process's semaphore and waits until the main process needs it again.
                If there is no available pages, it calls the page replacement process and waits for a response.
            3) Page Replacement Process
                The page replacement is responsible for finding which page to replace, depending on the current page replacement
                policy in effect.
                It calls the appropriate function to find the page to replace, calls the disk driver to write the page being replaced and
                send the the missing page. It then updates the necessary variables and signals the page fault handler when it is done.
            4) Disk Driver
                The disk driver process calls the paging disk and waits for it be completed, signaling the calling process (either the
                page fault handler or the page replacement process).
            5) Paging Disk
                The paging disk process uses the following disk-scheduling algorithms to decide which track to go to first when the disk
                queue is filled (size = number of I/O requests in the disk queue):
                        FIFO (First-In-First-Out)
                        SSTF (Shortest Seek Time First)
                        SCAN (Elevator)
                The FIFO algorithm handles requests in the order that they arrive.
                The SSTF algorithm chooses the closest track to the current track and goes there first.
                The SCAN algorithm looks at the first request for each process in the disk queue and sorts them in ascending or descending order
                    (depending on the direction of the head). It then tries to go to those tracks from the current track it is on.
                    If none of those tracks can be visited, the track goes all the way to 0 or maxtrack and reverses direction.
                    This continues until all of the tracks are visited.
                  Note: If the direction the head is traveling needs to be changed, the time it takes to go all the way to the end of the
                  track is included in the seek time.

    Disk Scheduling Assumptions:
    1) To keep the memory requests for each process in the correct order for the SSTF and SCAN algorithms, the first incoming memory request for each
       process is compared when making a decision about which track to go to next. For example, if the first process requested the memory on track 2
       before the memory on track 4, even if track 4 is closer to the current track, the algorithm will look at the distance of track 2 instead.
       It will then compare this seek time to the first memory request for the other processes and make a decision based on that.
    2) The program starts the disk head at track 0, but while the program is running, the current track will be saved and disk activity will continue
       from the last track the disk visited. For SCAN, it will also continue in the same direction.

    Waiting Processes Queue:
        Since there is a minimum number of page frames that need to be unused, the total number of usable page frames is calculated (total - min).
        From this, the maximum number of running process is determined by taking the floor((totalFrames - minNbOfFreeFrames) / nbPagesPerProcess).
        A counter keeps track of the number of requesting process's running. Once that maximum number is reached, any new requesting processes has
        to wait until a running process has ended.
        Once a running process has ended, the main process starts process from the waiting queue until it is empty or there are no more
        requests that can be handled. It then goes back to processing from the main request list.
        This back-and-forth happens to ensure that the memory requests are handled as close as possible to the order in which they came in.

Output:
    The program will output to the console the number of page faults and page hits per process, as well
    as the overall total page hits and page faults at the end of each run.
	Note: For the working set, it will print the size of the working set for a process after each one of it's request is completed. This
	will allow you to see the changing size of the set.
	The program will also output the total seek time and the average seek time for each of the disk-scheduling algorithms.
	The average is calculated by dividing the total seek time by the number of page faults. This result is displayed as a truncated integer.
***************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sstream>
#include<iomanip>
#include <math.h>
#include <string.h>
#include <errno.h>
using namespace std;

/***************************Structure Declarations***************************/
struct AddressRequests //struct to store address requests extracted from input file
{
    int processID; //process id matching the address
    string addr; //address of the page being requested

    AddressRequests(int id, string ad)
    {
        processID = id;
        addr = ad;
    }
};
struct SharedAddress //structure to communicated among the processes
{
    int index; //stores the pid of the requesting process
    int addr; //stores the address that is being requested
    int readWrite; // 0 for read, 1 for write
    bool finished; //true if all the requests have been completed
    int totalSeekTime; //total seek time for disk requests
    bool upwards; //true if track is moving towards the maxtrack

};
struct PageCounter //keeps track of counters needed for the different schedules
{
    int counter; //counters for the different replacement algorithms
    int segment; //stores the index for the process's segement table
    int page; //stores the index for the process's page table
    int frame; //stores the frame where the page is stored
    int frequency; //counter for how frequently a page is requested
};
struct RunningProcesses
{
    int processName; //stores the name of the process (ex. 101, 102)
    bool active; //true if request is being processed
};
struct DiskQueue
{
    int processID;
    int track;

    DiskQueue (int pid, int newTrack)
    {
        processID = pid;
        track = newTrack;
    }
};

/***************************Function Declarations***************************/
int readFile(); //Extracts information from input file and stores them in the appropriate variables
void createSharedMemory(); //Create shared memory
void cleanupSharedMemory(); //Calls unlink and shmctl to delete semaphores and shared memory
void processRequests(int, int); //Runs the program with FIFO page replacement policy
void runLRUScheduler(); //Runs the program with Least Recently Used page replacement policy
void runLFUScheduler(); //Runs the program with Least Frequency Used page replacement policy with LRU tie-breaker
void runOPTScheduler(); //Runs the program with OPT-Lookahead page replacement policy
void runWSScheduler(); //Runs the program with Working Set page replacement policy
int parseProcessID(string, int&); //Parses string to get process id
int getProcessID(int); //Returns index of process
string convertToBinary(string); //Converts the address from hexadecimal to binary and returns it as a string
void getBits(int*, string); //Extracts segment, page and offset in bits from the string parameter and writes it to array parameter
int binaryToDecimal(int); //Helper function to convert a binary number to decimal
string padAddress(int); //Helper function to pad 0's to the front of the integer to get length=addressLength
int findFreeFrame(); //Returns the index of the next free frame in the frameTable, -1 if none was found
void increaseCounters(int, int); //Increases counters of each of the current process's page, setting the current processed page to 0
int findLargestCounter(int); //Returns the index of the current process's page with the highest counter
int findSmallestFrequency(int); //Returns the index of the current process's page with the smallest frequency
int findFrame(PageCounter*, int, int); //Returns index of the current process's page which matches the key parameter
void getLookAhead(int, int, int); //Updates the array with the look ahead memory requests for a process
int getLookAheadReplacement (int); //Returns page# which has smallest lookahead priority (no upcoming memory requests or furthest one)
void updateWorkingSet(int, int); //Updates the size of the working set when a match occurs
int getWSSize(int); //Returns size of the working set
void endProcess(int); //Releases all memory held by process
void startDisk(int, int, int); //Disk driver process that simulates reading/writing to disk by calling the sleep() function
int fifo(vector<DiskQueue>&, int&); //returns the total seek time needed to process all the requests in the vector using FIFO
int sstf(vector<DiskQueue>&, int&); //returns the total seek time needed to process all the requests in the vector u sing SSTF
int scan(vector<DiskQueue>&, int&, int, bool&); //returns the total seek time needed to process all the requests in the vector using SCAN
void getFirstRequests(vector<DiskQueue>, vector<DiskQueue>&); //gets each process's first request in the disk queue
int findSST(vector<DiskQueue>, vector<DiskQueue>, int); //returns the index of the vector that has the sst from the int parameter
void sortDiskQueue(vector<DiskQueue>, vector<DiskQueue>&, bool); //Sorts a diskqueue in ascending or descending order
string addToString(string, int); //Appends an int to the string parameter and returns the result

/***************************Variable Declarations***************************/
int totalFrames; //Stores the total number of frames in the frameTable
int pgsPerSegment; //Stores the number of pages in each segment
int pgSizeBytes; //Stores the pg size in bytes (gets converted to bits to find segment, page and offset)
int pgsPerProcess; //Number of pages per process
int sgmsPerProcess; //Number of segments per process
int lookAhead; //Look ahead window size for OPT
int minFrames; //Minimum number of frames to keep free in the frames table
int maxFrames; //Maximum number of frames to use in the frames table
int nbProcesses; //Number of processes requesting memory
int maxTrack; //Maximum track number on disk
int nbDiskRequests; //Number of I/O requests in the disk queue
int totalRequests= 0; //Total number of memory requests
vector<int> processIDS; //Stores the actual values of the process (ex. 100, 101, 102...)
//vector<int> diskFrameSize; //Stores sizes of the disk page frame size for each process
vector< vector<int> > diskLUTable; //Stores corresponding page frames on disk for each process
vector<int> diskFrames; //Page frame table for the disk
vector<AddressRequests> addressRequests; //Stores each address request to be processed
int maxRunningProcesses; //Stores the maximum number of processes that can run at one time
vector<int> waitingIDs; //stores ids of waiting requests
vector<string> waitingAddresses; //stores addresses of waiting requests
/***Shared Memory Variables***/
int *frameTable; //Stores page address occupying each frame
SharedAddress *shAddr; //Used for communication between main process, fault handler, page replacement and disk driver
RunningProcesses *runningProcesses; //Array that stores each process's name and active status
PageCounter *counters; //Stores the counters used for the different replacement algorithms
int *table; //stores segment table and page table of each process
int *processPtr; //stores the shared memory id for each process's table
int *pgsUsedPerProcess; //stores current number of pgs used for each process
int addressLength; //stores the length of the address in bits
int *optArray; //used to keep track of next X requests for lookahead replacement algorithm
/***Address bits variables***/
int segmentBits;
int pageBits;
int offsetBits;
int nbSegmentsPerProcess;
/****************Start Main Function****************/
int main()
{
    readFile();
    for (int i=0; i<5; i++)
    {
        switch(i)
        {
        case 0:
        {
            cout << "********************************************" << endl;
            cout << "*       FIFO Replacement Algorithm         *" << endl;
            cout << "********************************************" << endl;
            for (int j=0; j<3; j++)
            {
                createSharedMemory();
                processRequests(i, j);
                cleanupSharedMemory();
            }
            cout << "********************************************" << endl << endl;
            break;
        }
        case 1:
        {
            cout << "********************************************" << endl;
            cout << "*       LRU Replacement Algorithm          *" << endl;
            cout << "********************************************" << endl;
            for (int j=0; j<3; j++)
            {
                createSharedMemory();
                processRequests(i, j);
                cleanupSharedMemory();
            }
            cout << "********************************************" << endl << endl;
            break;
        }
        case 2:
        {
            cout << "********************************************" << endl;
            cout << "*        LFU Replacement Algorithm         *" << endl;
            cout << "********************************************" << endl;
            for (int j=0; j<3; j++)
            {
                createSharedMemory();
                processRequests(i, j);
                cleanupSharedMemory();
            }
            cout << "********************************************" << endl << endl;
            break;
        }
        case 3:
        {
            cout << "********************************************" << endl;
            cout << "*   OPT-Lookahead Replacement Algorithm    *" << endl;
            cout << "********************************************" << endl;
            for (int j=0; j<3; j++)
            {
                createSharedMemory();
                void *ptr = mmap(NULL, sizeof(int)*lookAhead, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
                if (ptr == MAP_FAILED)
                {
                    cout << "Error creating frame table map: " << strerror(errno) << "\nProgram will now exit." << endl;
                    exit(1);
                }
                optArray = (int*) ptr;

                processRequests(i, j);
                cleanupSharedMemory();
                munmap(optArray, sizeof(int)*lookAhead);
            }
            cout << "********************************************" << endl << endl;
            break;
        }
        case 4:
        {
            cout << "********************************************" << endl;
            cout << "*    Working Set Replacement Algorithm     *" << endl;
            cout << "********************************************" << endl;
            for (int j=0; j<3; j++)
            {
                createSharedMemory();
                processRequests(i, j);
                cleanupSharedMemory();
            }
            cout << "********************************************" << endl << endl;
            break;
        }
        }
    }

	return 0;
}

int readFile()
{
	ifstream file("hw3input.txt");

	if (!file.is_open())
	{
        cout << "hw3input.txt was not found. Please make sure input file is correct." << endl;
        exit(1);
	}

	// get lines from input file
	string str;
	vector<string> lines;

	while (getline(file, str))
		lines.push_back(str);

	file.close();

	unsigned int counter = 0; // counter for the current line to be processed
	totalFrames = atoi(lines[counter++].c_str());
	pgsPerSegment = atoi(lines[counter++].c_str());
	pgSizeBytes = atoi(lines[counter++].c_str());
	pgsPerProcess = atoi(lines[counter++].c_str());
	lookAhead = atoi(lines[counter++].c_str());
	minFrames = atoi(lines[counter++].c_str());
	maxFrames = atoi(lines[counter++].c_str());
	nbProcesses = atoi(lines[counter++].c_str());
	maxTrack = atoi(lines[counter++].c_str());
	nbDiskRequests = atoi(lines[counter++].c_str());

	totalFrames = totalFrames - minFrames;

    diskLUTable.resize(nbProcesses);
	for (int i=0; i<nbProcesses; i++) //store process ids to a vector
	{
        int pos = 0;
        processIDS.push_back(parseProcessID(lines[counter], pos));
        int diskFrameSize = atoi(lines[counter++].substr(pos+1, string::npos).c_str());
        diskLUTable[i].resize(diskFrameSize);
        for (int j=0; j<diskFrameSize; j++)
            diskLUTable[i][j] = atoi(lines[counter++].c_str());
    }

    while (counter < lines.size()) //get memory requests from input file
    {
        vector <AddressRequests> temp;
        int pos = 0;
        addressRequests.push_back(AddressRequests(parseProcessID(lines[counter], pos),
            convertToBinary(lines[counter].substr(pos, string::npos))));
        counter++;
    }
    addressLength = strlen(addressRequests[0].addr.c_str());

    //get bits for segment, page and offset
    int bitSize = addressRequests[0].addr.size();
    offsetBits = (int)log2(pgSizeBytes);
    pageBits = (int)log2(pgsPerSegment);
    segmentBits = bitSize - offsetBits - pageBits;
    nbSegmentsPerProcess = (int)pow(2,segmentBits);

    maxRunningProcesses = (int)floor(totalFrames/pgsPerProcess);

	return 0;
}

void createSharedMemory()
{
    //unlink semaphores and close shared memory
    sem_unlink("main1105982");
    sem_unlink("pageFault1105982");
    sem_unlink("pageReplacement1105982");
    sem_unlink("diskDriver1105982");
    sem_unlink("shAddr1105982");
    sem_unlink("pagingDisk1105982");
    for (int i=0; i<nbProcesses; i++)
        sem_unlink(addToString("process1105982",i).c_str());

    /*****************************************************************/
    //create shared memory for frames table
    void *ptr = mmap(NULL, sizeof(int)*totalFrames, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        cout << "Error creating frame table map: " << strerror(errno) << "\nProgram will now exit." << endl;
        exit(1);
    }
    frameTable = (int*) ptr;
    for (int i=0; i<totalFrames; i++)
        frameTable[i] = -1;

    /*****************************************************************/
    //create shared memory for communication between processes (shAddr)
    ptr = mmap(NULL, sizeof(SharedAddress), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        cout << "Error creating shared address map: " << strerror(errno) << "\nProgram will now exit." << endl;
        exit(1);
    }
    shAddr = (SharedAddress*) ptr;

    /*****************************************************************/
    //create shared memory for pgs used per process
    ptr = mmap(NULL, sizeof(int)*nbProcesses, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        cout << "Error creating pgsPerProcess map: " << strerror(errno) << "\nProgram will now exit." << endl;
        exit(1);
    }
    pgsUsedPerProcess = (int*) ptr;
    for (int i=0; i<nbProcesses; i++)
        pgsUsedPerProcess[i] = 0;

    /*****************************************************************/
    //create shared memory for running processes
    ptr = mmap(NULL, sizeof(RunningProcesses)*nbProcesses, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        cout << "Error creating pgsPerProcess map: " << strerror(errno) << "\nProgram will now exit." << endl;
        exit(1);
    }
    runningProcesses = (RunningProcesses*) ptr;
    for (int i=0; i<nbProcesses; i++)
    {
        runningProcesses[i].processName = processIDS[i];
        runningProcesses[i].active = false;
    }

    /*****************************************************************/
    //create shared memory for replacement algorithm counters (FIFO, LRU, LFU)
    ptr = mmap(NULL, sizeof(PageCounter)*nbProcesses*pgsPerProcess, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        cout << "Error creating counters map: " << strerror(errno) << "\nProgram will now exit." << endl;
        exit(1);
    }
    counters = (PageCounter*) ptr;
    for (int i=0; i<nbProcesses; i++)
    {
        for (int j=0; j<pgsPerProcess; j++)
        {
            counters[i*pgsPerProcess +j].counter = -1;
            counters[i*pgsPerProcess +j].frame = -1;
            counters[i*pgsPerProcess +j].frequency = 0;
            counters[i*pgsPerProcess +j].segment = -1;
            counters[i*pgsPerProcess +j].page = -1;
        }
    }

    /*****************************************************************/
    //create shared memory for tables
    key_t key = 87521;
    int shmID;
    processPtr = new int[nbProcesses];
    for (int i=0; i<nbProcesses; i++)
    {
        int shSize = sizeof(int)*nbSegmentsPerProcess*pgsPerSegment;
        shmID = shmget(key, shSize, IPC_CREAT|0666);
        if (shmID < 0)
        {
            cout << "Error creating shared memory: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }
        if ((table = (int*) shmat(shmID, NULL, 0)) == (int*)-1)
        {
        cout << "Error assigning shared memory: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }

        //initialize tables to -1
        for (int j=0; j<nbSegmentsPerProcess; j++)
        {
            for (int k=0; k<pgsPerSegment; k++)
                table[j*pgsPerSegment +k] = -1;
        }
        processPtr[i] = shmID; //store the shmID for that process for later access
        key++;
    }
}

void cleanupSharedMemory()
{
    /*****************************************************************/
    //unlink semaphores and close shared memory
    sem_unlink("main1105982");
    sem_unlink("pageFault1105982");
    sem_unlink("pageReplacement1105982");
    sem_unlink("diskDriver1105982");
    sem_unlink("shAddr1105982");
    sem_unlink("pagingDisk1105982");
    for (int i=0; i<nbProcesses; i++)
    {
        sem_unlink(addToString("process1105982",i).c_str());
        shmctl(processPtr[i], IPC_RMID, NULL);
    }
    munmap(frameTable, sizeof(int)*totalFrames);
    munmap(shAddr, sizeof(SharedAddress));
    munmap(pgsUsedPerProcess, sizeof(int)*nbProcesses);
    munmap(runningProcesses, sizeof(RunningProcesses)*nbProcesses);
    munmap(counters, sizeof(PageCounter)*nbProcesses*pgsPerProcess);

    delete[] processPtr;
}

void processRequests(int replacementAlgorithm, int diskAlgorithm)
{
    //Create fault handler, disk replacement and disk driver processes
    int cid[4];
    int pid = -1;
    for (int i=0; i<4; i++)
    {
        if ((cid[i] = fork()) == -1)
        {
            cout << "Error while forking: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }

        if (cid[i] == 0)
        {
            pid = i;
            break; //prevents additional processes from being created
        }
    }

    /*****************************************************************/
    //Switch statement for the processes
    switch(pid)
    {
    case 0: //page fault handler
    {
        /*****************************************************************/
        //create semaphores
        sem_t *mainSem = sem_open("main1105982", O_CREAT, 0666, 0);
        sem_t *pfSem = sem_open("pageFault1105982", O_CREAT, 0666, 0);
        sem_t *prSem = sem_open("pageReplacement1105982", O_CREAT, 0666, 0);
        sem_t *ddSem = sem_open("diskDriver1105982", O_CREAT, 0666, 0);
        sem_t *sharedSem = sem_open("shAddr1105982", O_CREAT, 0666, 1);
        if (mainSem==SEM_FAILED || pfSem==SEM_FAILED || prSem==SEM_FAILED || ddSem==SEM_FAILED)
        {
            cout << "Error creating semaphores in page fault handler: " << strerror(errno) << "\nProgram will now exit." << endl;
            _exit(1);
        }
        //create semaphore for each process's table
        sem_t *sem[nbProcesses];
        for (int i=0; i<nbProcesses; i++)
        {
            sem[i] = sem_open(addToString("process1105982",i).c_str(), O_CREAT, 0666, 1);
            if (sem[i]==SEM_FAILED)
            {
                cout << "Error creating semaphore for processes in page fault handler: " << strerror(errno) << "\nProgram will now exit." << endl;
                exit(1);
            }
        }

        /*****************************************************************/
        while (true) //loop until all requests are completed
        {
            sem_wait(pfSem);
            sem_wait(sharedSem); //locked shared memory
            if (shAddr->finished)
                break;
            int pid = shAddr->index;
            sem_wait(sem[pid]); // lock shared memory of the requesting process's page table
            sem_post(mainSem); //signal to main process, so that it can keep processing more requests
            if (pgsUsedPerProcess[pid] == pgsPerProcess) // call page fault handler
            {
                sem_post(prSem);
                sem_wait(pfSem);
            }
            else //nb of pages used is less than max nb pgs per process
            {
                shAddr->readWrite = 0; //read signal
                sem_post(ddSem); //signal disk driver to read from the disk
                sem_wait(pfSem); //waiting for the disk driver to finish
                string address = padAddress(shAddr->addr); //read from the information retrieved from disk
                table = (int*)shmat(processPtr[pid], NULL, 0); //attach table of the requesting process
                int bitArray[3];
                getBits(bitArray, address); //get segement table index and page table index
                int frame = findFreeFrame(); //find a free frame
                frameTable[frame] = atoi(address.c_str()); //put address in frame table
                table[bitArray[0]*pgsPerSegment + bitArray[1]] = frame; //put frame number in page table
				if (replacementAlgorithm == 4) //working set algorithm
				{
					int index;
					for (int i=0; i<pgsPerProcess; i++)
					{
						if (counters[pid*pgsPerProcess +i].frame == -1)
						{
							index = i;
							break;
						}
					}
					counters[pid*pgsPerProcess +index].segment = bitArray[0];
					counters[pid*pgsPerProcess +index].page = bitArray[1];
					counters[pid*pgsPerProcess +index].frame = frame;
					increaseCounters(pid, index);
					updateWorkingSet(pid, frame);
					pgsUsedPerProcess[pid] = pgsUsedPerProcess[pid] + 1; //update pages used per process
				}
				else //add page to frames table and update variables
				{
					counters[pid*pgsPerProcess +pgsUsedPerProcess[pid]].segment = bitArray[0];
					counters[pid*pgsPerProcess +pgsUsedPerProcess[pid]].page = bitArray[1];
					counters[pid*pgsPerProcess +pgsUsedPerProcess[pid]].frame = frame;
					increaseCounters(pid, pgsUsedPerProcess[pid]);
					pgsUsedPerProcess[pid] = pgsUsedPerProcess[pid] + 1; //update pages used per process
				}
            }
            sem_post(sem[pid]); //release requesting process's page tables
            sem_post(sharedSem); //release shared memory
        }
        sem_post(prSem);
        //sem_post(sharedSem);
        _exit(0);
    }
    case 1: //page replacement process - FIFO
    {
        /*****************************************************************/
        //create semaphores
        sem_t *prSem = sem_open("pageReplacement1105982", O_CREAT, 0666, 0);
        sem_t *pfSem = sem_open("pageFault1105982", O_CREAT, 0666, 0);
        sem_t *ddSem = sem_open("diskDriver1105982", O_CREAT, 0666, 0);
        if (prSem==SEM_FAILED || pfSem==SEM_FAILED || ddSem==SEM_FAILED)
        {
            cout << "Error creating semaphore in page replacement process: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }

        /*****************************************************************/
        while (true) //loop until all requests are completed
        {
            sem_wait(prSem);
            if (shAddr->finished)
                break;

            int pid = shAddr->index;
            int pgToReplace; //stores index of page to replace
            table = (int*)shmat(processPtr[pid], NULL, 0); //attach requesting process's page table

			if (replacementAlgorithm == 2) //LFU algorithm
				pgToReplace = findSmallestFrequency(pid); //find index of the page with the smallest frequency
			else if (replacementAlgorithm == 3) //lookahead algorithm
				pgToReplace = getLookAheadReplacement(pid); //get page with largest lookahead value
			else
				pgToReplace = findLargestCounter(pid); //get page# of page to replace
            int segment = counters[pid*pgsPerProcess +pgToReplace].segment;
            int page = counters[pid*pgsPerProcess +pgToReplace].page;
            int frame = table[segment*pgsPerSegment +page]; //find frame to replace

            //signal disk driver to write the page and wait for new page to be retrieved
            shAddr->readWrite = 1;
            sem_post(ddSem); //signal disk driver to write address to disk
            sem_wait(prSem); //wait for disk driver to finish
            string address = padAddress(shAddr->addr); //read information retrieved from disk

            int bitArray[3];
            getBits(bitArray, address); //get segment page index and page table index

            table[segment*pgsPerSegment +page] = -1; //reset page to be replaced in page table
            frameTable[frame] = atoi(address.c_str()); //put address in frame table
            table[bitArray[0]*pgsPerSegment + bitArray[1]] = frame; //put frame number in page table
            counters[pid*pgsPerProcess +pgToReplace].segment = bitArray[0];
            counters[pid*pgsPerProcess +pgToReplace].page = bitArray[1];
            counters[pid*pgsPerProcess +pgToReplace].frame = frame;
			if (replacementAlgorithm == 4) //update size of working set
				updateWorkingSet(pid, frame);
            increaseCounters(pid, pgToReplace);
            sem_post(pfSem); //signal to page fault handler that page replacement is completed
        }
        sem_post(ddSem);
        _exit(0);
    }
    case 2: //disk driver process
    {
        /*****************************************************************/
        //create semaphores
        sem_t *ddSem = sem_open("diskDriver1105982", O_CREAT, 0666, 0);
        sem_t *pfSem = sem_open("pageFault1105982", O_CREAT, 0666, 0);
        sem_t *prSem = sem_open("pageReplacement1105982", O_CREAT, 0666, 0);
        sem_t *pdSem = sem_open("pagingDisk1105982", O_CREAT, 0666, 0);
        if (ddSem==SEM_FAILED || pfSem==SEM_FAILED || pdSem==SEM_FAILED)
        {
            cout << "Error creating semaphore in disk driver process: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }
        /*****************************************************************/
        while (true) //loop until all requests are completed
        {
            sem_wait(ddSem);
            if (shAddr->finished)
                break;
            sem_post(pdSem); //signal paging disk
            sem_wait(ddSem); //waiting for paging disk to complete
            if (shAddr->readWrite == 0)
                sem_post(pfSem); //signal page fault that address has been read from disk
            else
                sem_post(prSem); //signal page replacement that address has been written/read from disk
        }
        sem_post(pdSem);
        _exit(0);
    }
    case 3: //paging disk
    {
        /*****************************************************************/
        //create semaphores
        sem_t *pdSem = sem_open("pagingDisk1105982", O_CREAT, 0666, 0);
        sem_t *ddSem = sem_open("diskDriver1105982", O_CREAT, 0666, 0);
        if (pdSem==SEM_FAILED || ddSem==SEM_FAILED)
        {
            cout << "Error creating semaphore in disk driver process: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }

        vector<DiskQueue> diskRequests;
        int currentTrack = 0;
        /*****************************************************************/
        while (true) //loop until all requests are completed
        {
            sem_wait(pdSem);
            if (shAddr->finished)
            {
                if (diskRequests.size() == 0)
                    break;
            }
            else
            {
                string address = padAddress(shAddr->addr);
                int page = binaryToDecimal(atoi(address.substr(0, offsetBits).c_str()));
                int track = diskLUTable[shAddr->index][page];
                diskRequests.push_back(DiskQueue(shAddr->index, track));
            }

            if (diskRequests.size() == nbDiskRequests || shAddr->finished) //disk queue is full
            {
                if (diskAlgorithm == 0) //FIFO algorithm
                    shAddr->totalSeekTime = shAddr->totalSeekTime + fifo(diskRequests, currentTrack);
                else if (diskAlgorithm == 1) //SSTF algorithm
                    shAddr->totalSeekTime = shAddr->totalSeekTime + sstf(diskRequests, currentTrack);
                else if (diskAlgorithm == 2) //SCAN algorithm
                    shAddr->totalSeekTime = shAddr->totalSeekTime + scan(diskRequests, currentTrack, maxTrack, shAddr->upwards);
                else
                    cout << "Invalid disk-scheduling algorithm!" << endl;
            }

            if (shAddr->finished)
                break;
            sem_post(ddSem);
        }
        _exit(0);
    }
    case -1: //main process
    {
        /*****************************************************************/
        //Create semaphores
        sem_t *mainSem = sem_open("main1105982", O_CREAT, 0666, 0);
        sem_t *pfSem = sem_open("pageFault1105982", O_CREAT, 0666, 0);
        sem_t *sharedSem = sem_open("shAddr1105982", O_CREAT, 0666, 1);
        if (mainSem==SEM_FAILED || pfSem==SEM_FAILED || sharedSem==SEM_FAILED)
        {
            cout << "Error creating semaphore in main process: " << strerror(errno) << "\nProgram will now exit." << endl;
            exit(1);
        }
        //create semaphore for each process's table
        sem_t *sem[nbProcesses];
        for (int i=0; i<nbProcesses; i++)
        {
            sem[i] = sem_open(addToString("process1105982",i).c_str(), O_CREAT, 0666, 1);
            if (sem[i]==SEM_FAILED)
            {
                cout << "Error creating semaphore for processes in main process. Program will now exit";
                exit(1);
            }
        }
        /*****************************************************************/
        //Declare and initialize variables
        totalRequests = addressRequests.size();
        if (totalRequests > 0)
            shAddr->finished=false;
        else
            shAddr->finished=true;
        shAddr->totalSeekTime = 0;
        shAddr->upwards = true;

        int request = 0; //counter for current request being processed
        bool done = false; //set to true if there are no waiting processes
        bool flag = false; //signal to start processing from waiting queue
        int waitingRequest = 0; //counter for current queue request being processed
        int nbRunningProcesses = 0; //stores number of processes currently running

        int nbMisses[nbProcesses]; //stores number of page faults for each process
        int nbHits[nbProcesses]; //stores number of page hits for each fault
        for (int i=0; i<nbProcesses; i++)
        {
            nbMisses[i] = 0;
            nbHits[i] = 0;
        }

        /*****************************************************************/
        //Main loop
        while (request < totalRequests || waitingIDs.size() >0)
        {
            //cout << "Waiting queue size: " << waitingIDs.size() << endl;
            string address;
            int pid;

            if(flag) //process waiting request queue
            {
                if (waitingRequest < waitingIDs.size()) //processes are waiting
                {
                    address = waitingAddresses[waitingRequest];
                    pid = getProcessID(waitingIDs[waitingRequest]);
                    if (nbRunningProcesses < maxRunningProcesses) //memory space available
                    {
                        runningProcesses[pid].active = true; //activate process
                        nbRunningProcesses++; //increase nb of running processes
                    }
                    else if (nbRunningProcesses == maxRunningProcesses)
                    {
                        if (!runningProcesses[pid].active)
                        {
                            waitingRequest++;
                            continue;
                        }
                    }
                    waitingIDs.erase(waitingIDs.begin() +waitingRequest);
                    waitingAddresses.erase(waitingAddresses.begin() +waitingRequest);
                }
                else //if there are no more waiting requests or no more that can be processed, go back to main requests
                {
                    flag = false;
                    waitingRequest = 0;
                    request--;
                    continue;
                }
            }
            //check if process can run or if it has to wait
            else if (nbRunningProcesses == maxRunningProcesses) //add request to waiting queue
            {
                address = addressRequests[request].addr;
                pid = getProcessID(addressRequests[request].processID);
                if (!runningProcesses[pid].active)
                {
                    waitingIDs.push_back(addressRequests[request].processID);
                    waitingAddresses.push_back(address);
                    request++;
                    continue;
                }
            }
            else //add process id to running array if it is not in there
            {
                address = addressRequests[request].addr;
                pid = getProcessID(addressRequests[request].processID);
                if (!runningProcesses[pid].active)
                {
                    runningProcesses[pid].active = true;
                    nbRunningProcesses++;
                }
            }
            if (address == "-1") //terminate process
            {
                sem_wait(sem[pid]);
                endProcess(pid);
                sem_post(sem[pid]);
                nbRunningProcesses--;
                request++;
                waitingRequest = 0;
                if (waitingIDs.size() > 0) //activate waiting queue if requests are waiting
                    flag = true;
                continue;
            }
            //check if page is already loaded into memory
            sem_wait(sem[pid]); //lock process's table
            table = (int*)shmat(processPtr[pid], NULL, 0); //attach the process's table
            int bitArray[3]; //declare array to store seg#, page# and offset
            getBits(bitArray, address); //returns 0=segment#, 1=page#, 2=offset#

            if (table[bitArray[0]*pgsPerSegment + bitArray[1]] == -1) //page fault
            {
                sem_wait(sharedSem);
                shAddr->index = pid;
                shAddr->addr = atoi(address.c_str());
				if (replacementAlgorithm == 3) //lookahead
				{
                    if (flag)
                        getLookAhead(waitingIDs[waitingRequest], waitingRequest, 1);
                    else
                        getLookAhead(addressRequests[request].processID, request+1, 0);
				}
                sem_post(pfSem); //notify page fault handler
                sem_post(sem[pid]); //release process's table
                sem_post(sharedSem); //release shared memory
                sem_wait(mainSem);
                nbMisses[pid] = nbMisses[pid] + 1;
            }
            else //page hit
            {
				if (replacementAlgorithm == 1) //lru replacement algorithm
				{
					int frame = findFrame(counters, pid, table[bitArray[0]*pgsPerSegment + bitArray[1]]);
					increaseCounters(pid, frame);
				}
				else if (replacementAlgorithm == 2 || replacementAlgorithm == 3) //lfu and lookahead replacement algorithm
				{
					int frame = findFrame(counters, pid, table[bitArray[0]*pgsPerSegment + bitArray[1]]);
					counters[pid*pgsPerProcess +frame].frequency++;
					increaseCounters(pid, frame);
				}
				else if (replacementAlgorithm == 4) ////working set algoritm replacement algorithm
				{
					int frame = findFrame(counters, pid, table[bitArray[0]*pgsPerSegment + bitArray[1]]);
					increaseCounters(pid, frame);
					updateWorkingSet(pid, frame);
				}
                sem_post(sem[pid]);
                nbHits[pid] = nbHits[pid] + 1;
            }

            if (replacementAlgorithm == 4 && diskAlgorithm == 0) //working set algorithm
                cout << "Process " << runningProcesses[pid].processName << " working set size is: " << getWSSize(pid) << endl;

            if (!flag)
                request++;
        }
        sem_wait(sharedSem);
        shAddr->finished = true;
        sem_post(pfSem);
        sem_post(sharedSem);
        for (int i=0; i<4; i++) //wait for all child processes to exit
            waitpid(cid[i], NULL, 0);


        /*****************************************************************/
        //Output
        int nbPageFaults = 0;
        int nbPageHits = 0;

        if (diskAlgorithm == 0)
        {
            for (int i=0; i<nbProcesses; i++)
            {
                cout << "Process " << i << " misses: " << nbMisses[i] << endl;
                cout << "Process " << i << " hits: " << nbHits[i] << endl;
                nbPageFaults = nbPageFaults + nbMisses[i];
                nbPageHits = nbPageHits + nbHits[i];
            }
            cout << "Total number of page faults is: " << nbPageFaults << endl;
            cout << "Total number of page hits is: " << nbPageHits << endl;
        }
        else
        {
            for (int i=0; i<nbProcesses; i++)
            {
                nbPageFaults = nbPageFaults + nbMisses[i];
                nbPageHits = nbPageHits + nbHits[i];
            }
        }
        if (diskAlgorithm == 0)
        {
            cout << "FIFO Total seek time is: " << shAddr->totalSeekTime << endl;
            cout << "FIFO Average seek time is: " << shAddr->totalSeekTime / nbPageFaults << endl;
        }
        if (diskAlgorithm == 1)
        {
            cout << "SSTF Total seek time is: " << shAddr->totalSeekTime << endl;
            cout << "SSTF Average seek time is: " << shAddr->totalSeekTime / nbPageFaults << endl;
        }
        if (diskAlgorithm == 2)
        {
            cout << "SCAN Total seek time is: " << shAddr->totalSeekTime << endl;
            cout << "SCAN Average seek time is: " << shAddr->totalSeekTime / nbPageFaults << endl;
        }
    }
    }
}

int parseProcessID(string str, int& pos)
{
	string s = "";
	while (str[pos] != ' ')
        s = s + str[pos++];
    return atoi(s.c_str());
}

int getProcessID(int key)
{
    for (int i=0; i<nbProcesses; i++)
    {
        if (runningProcesses[i].processName == key)
            return i;
    }
    return -1;
}

string convertToBinary(string str)
{
    int pos = str.find('x') + 1;
    str = str.substr(pos, string::npos);
    string binary = "";
    bool flag = false;

    for (int unsigned i=0; i < str.length(); i++)
    {
        switch (str[i])
        {
        case '0': binary += "0000"; break;
        case '1': binary += "0001"; break;
        case '2': binary += "0010"; break;
        case '3': binary += "0011"; break;
        case '4': binary += "0100"; break;
        case '5': binary += "0101"; break;
        case '6': binary += "0110"; break;
        case '7': binary += "0111"; break;
        case '8': binary += "1000"; break;
        case '9': binary += "1001"; break;
        case 'A': binary += "1010"; break;
        case 'B': binary += "1011"; break;
        case 'C': binary += "1100"; break;
        case 'D': binary += "1101"; break;
        case 'E': binary += "1110"; break;
        case 'F': binary += "1111"; break;
        case '-': flag = true; break;
        }
        if (flag)
            return "-1";
    }
    return binary;
}

void getBits(int bitArray[], string address)
{
    bitArray[0] = binaryToDecimal(atoi(address.substr(0, segmentBits).c_str()));
    address = address.substr(segmentBits, address.size());

    bitArray[1] = binaryToDecimal(atoi(address.substr(0, pageBits).c_str()));
    address = address.substr(pageBits, address.size());

    bitArray[2] = binaryToDecimal(atoi(address.c_str()));
}

int binaryToDecimal (int binary)
{
    int decimal = 0;
    for(int i=0; binary > 0; i++) {

        if(binary % 10 == 1) {
            decimal += (1 << i);
        }
        binary /= 10;
    }
    return decimal;
}

string padAddress(int addr)
{
    stringstream ss;
    ss << setw(addressLength) << setfill('0') << addr;
    return ss.str();
}

int findFreeFrame()
{
    for (int i=0; i<totalFrames; i++)
    {
        if (frameTable[i] == -1)
            return i;
    }
    return -1;
}

void increaseCounters(int pid, int key)
{
    for (int i=0; i<pgsPerProcess; i++)
    {
        if (i == key)
            counters[pid*pgsPerProcess +i].counter = 0;
        else if (counters[pid*pgsPerProcess +i].counter != -1)
            counters[pid*pgsPerProcess +i].counter = counters[pid*pgsPerProcess +i].counter + 1;
    }
}

int findLargestCounter(int pid)
{
    int largest = 0;
    for (int i=1; i<pgsPerProcess; i++)
    {
        if (counters[pid*pgsPerProcess +i].counter > counters[pid*pgsPerProcess +largest].counter)
            largest = i;
    }
    return largest;
}

int findSmallestFrequency(int pid)
{
    int smallest = 0;
    for (int i=1; i<pgsPerProcess; i++)
    {
        if (counters[pid*pgsPerProcess +i].frequency < counters[pid*pgsPerProcess +smallest].frequency)
            smallest = i;
        else if (counters[pid*pgsPerProcess +i].frequency == counters[pid*pgsPerProcess +smallest].frequency)
        {
            if (counters[pid*pgsPerProcess +i].counter > counters[pid*pgsPerProcess +smallest].counter)
                smallest = i;
        }
    }
    return smallest;
}

int findFrame(PageCounter *arr, int pid, int key)
{
    for (int i=0; i<pgsPerProcess; i++)
    {
        if (arr[pid*pgsPerProcess +i].frame == key)
            return i;
    }
    return -1;
}

void getLookAhead(int pid, int current, int flag)
{
    int nbMatches = 0;
    for (int i=0; i<lookAhead; i++)
    {
        if (flag == 0)
        {
            while(current < addressRequests.size())
            {
                if (addressRequests[current].processID == pid)
                {
                    optArray[i] = atoi(addressRequests[current].addr.c_str());
                    current++;
                    nbMatches++;
                    break;
                }
                current++;
            }
        }
        else if (flag == 1)
        {
            while(current < waitingIDs.size())
            {
                if (waitingIDs[current] == pid)
                {
                    optArray[i] = atoi(waitingAddresses[current].c_str());
                    current++;
                    nbMatches++;
                    break;
                }
                current++;
            }
        }
    }
    if (nbMatches == 0)
    {
        for (int i=0; i<lookAhead; i++)
            optArray[i] = -1;
    }
    else if (nbMatches < lookAhead)
    {
        for (int i=nbMatches; i<lookAhead; i++)
            optArray[i] = -1;
    }
}

int getLookAheadReplacement(int pid)
{
    //the largest number in the array will be the page that has the longest time before it is repeated and should be replaced
    int arr[lookAhead];
    int nb = 10000;
    int segBits[lookAhead];
    int pgBits[lookAhead];
    for (int i=0; i<lookAhead; i++)
    {
        int bits[3];
        getBits(bits, padAddress(optArray[i]));
        segBits[i] = bits[0];
        pgBits[i] = bits[1];
    }
    for (int i=0; i<pgsPerProcess; i++)
    {
        arr[i] = 10000;
        for (int j=0; j<lookAhead; j++)
        {
            if (counters[pid*pgsPerProcess +i].segment == segBits[j] && counters[pid*pgsPerProcess +i].page == pgBits[j])
            {
                if (arr[i] == 10000)
                {
                    arr[i] = i;
                    break;
                }
            }
        }
    }

    int largest = 0;
    for (int i=1; i<lookAhead; i++)
    {
        if (arr[i] > arr[largest])
            largest = i;
        else if (arr[i] == arr[largest])
        {
            if (i < pgsPerProcess)
            {
                if (counters[pid*pgsPerProcess +i].counter > counters[pid*pgsPerProcess +largest].counter)
                    largest = i;
            }
        }
    }
    for (int i=0; i<pgsPerProcess; i++)
        if (arr[largest])

    return largest;
}

void updateWorkingSet(int pid, int frame)
{
    for (int i=0; i<pgsPerProcess; i++)
    {
        if (counters[pid*pgsPerProcess +i].frame != frame)
        {
            if (counters[pid*pgsPerProcess +i].counter >= lookAhead)
            {
                table[counters[pid*pgsPerProcess +i].segment*pgsPerSegment + counters[pid*pgsPerProcess +i].page] = -1;
                frameTable[counters[pid*pgsPerProcess +i].frame] = -1;
                counters[pid*pgsPerProcess +i].frame = -1;
                counters[pid*pgsPerProcess +i].counter = -1;
                counters[pid*pgsPerProcess +i].segment = -1;
                counters[pid*pgsPerProcess +i].page = -1;
                pgsUsedPerProcess[pid]--;
            }
        }
    }
}

int getWSSize(int pid)
{
    int sz =0;
    for (int i=0; i<pgsPerProcess; i++)
    {
        if (counters[pid*pgsPerProcess + i].counter != -1)
            sz++;
    }
    return sz;
}

void endProcess(int pid)
{
    for (int i=0; i<pgsPerProcess; i++)
    {
        if(counters[pid*pgsPerProcess +i].frame != -1)
        {
            int segment = counters[pid*pgsPerProcess +i].segment;
            int page = counters[pid*pgsPerProcess +i].page;
            int frame = table[segment*pgsPerSegment +page];
            frameTable[frame] = -1;
            counters[pid*pgsPerProcess +i].counter = -1;
            counters[pid*pgsPerProcess +i].frame = -1;
            counters[pid*pgsPerProcess +i].segment = -1;
            counters[pid*pgsPerProcess +i].page = -1;
            counters[pid*pgsPerProcess +i].frequency = -1;
            runningProcesses[pid].active = false;
            pgsUsedPerProcess[pid] = 0;
        }
    }
}

int fifo(vector<DiskQueue>& requests, int& currentTrack)
{
    int totalSeekTime = 0;

    while (requests.size() > 0)
    {
        totalSeekTime = totalSeekTime + abs(requests[0].track - currentTrack);
        currentTrack = requests[0].track;
        requests.erase(requests.begin() + 0);
    }

    return totalSeekTime;
}

int sstf(vector<DiskQueue>& requests, int& currentTrack)
{
    int totalSeekTime = 0;
    while (requests.size() > 0)
    {
        vector<DiskQueue> firstRequestPerProcess;
        getFirstRequests(requests, firstRequestPerProcess); //compares only the first request of each process to maintain order
        int index = findSST(requests, firstRequestPerProcess, currentTrack);
        totalSeekTime = totalSeekTime + abs(requests[index].track - currentTrack);
        currentTrack = requests[index].track;
        requests.erase(requests.begin() + index);
    }

    return totalSeekTime;
}

int scan(vector<DiskQueue>& requests, int& currentTrack, int maxTrack, bool& upwards)
{
    int totalSeekTime = 0;
    while (requests.size() > 0) //go through requests
    {
        vector<DiskQueue> firstRequestPerProcess;
        getFirstRequests(requests, firstRequestPerProcess); //compares only the first request of each process to maintain order
        sortDiskQueue(requests, firstRequestPerProcess, upwards);
        vector<DiskQueue> deletedRequests;

        for (int i=0; i<firstRequestPerProcess.size(); i++)
        {
            int index = firstRequestPerProcess[i].track;
            if (upwards)
            {
                if (requests[index].track > currentTrack)
                {
                    totalSeekTime = totalSeekTime + abs(requests[index].track - currentTrack);
                    currentTrack = requests[index].track;
                    deletedRequests.push_back(firstRequestPerProcess[i]);
                }
            }
            else
            {
                if (requests[index].track < currentTrack)
                {
                    totalSeekTime = totalSeekTime + abs(requests[index].track - currentTrack);
                    currentTrack = requests[index].track;
                    deletedRequests.push_back(firstRequestPerProcess[i]);
                }
            }
        }

        if (deletedRequests.size() == 0) //change direction of head
        {
            if (upwards)
            {
                totalSeekTime = totalSeekTime + abs(maxTrack - currentTrack);
                currentTrack = maxTrack;
                upwards = false;
            }
            else
            {
                totalSeekTime = totalSeekTime + abs(0 - currentTrack);
                currentTrack = 0;
                upwards = true;
            }
        }
        else //delete processed requests
        {
            bool flag = false;
            for (int i=0; i<deletedRequests.size() && !flag; i++)
            {
                flag = true;
                for (int j=0; j<deletedRequests.size()-1; j++)
                {
                    if (deletedRequests[j+1].track > deletedRequests[j].track)
                    {
                        DiskQueue temp = deletedRequests[j];
                        deletedRequests[j] = deletedRequests[j+1];
                        deletedRequests[j+1] = temp;
                        flag = false;
                    }
                }
            }

            for (int i=0; i<deletedRequests.size(); i++)
            {
                int index = deletedRequests[i].track;
                requests.erase(requests.begin() + index);
            }
        }
    }

    return totalSeekTime;
}

void getFirstRequests(vector<DiskQueue> requests, vector<DiskQueue>& firstRequestPerProcess)
{
    for (int i=0; i<requests.size(); i++)
    {
        bool flag = false; //true if first request for that process exists
        for (int j=0; j<firstRequestPerProcess.size(); j++)
        {
            if (requests[i].processID == firstRequestPerProcess[j].processID)
                flag = true;
        }
        if (!flag)
            firstRequestPerProcess.push_back(DiskQueue(requests[i].processID, i));
    }
}

int findSST(vector<DiskQueue> requests, vector<DiskQueue> firstRequestPerProcess, int currentTrack)
{
    int index = firstRequestPerProcess[0].track;
    int sst = abs(requests[index].track - currentTrack);
    int result = index;
    for (int i=1; i <firstRequestPerProcess.size(); i++)
    {
        index = firstRequestPerProcess[i].track;
        int temp = abs(requests[index].track - currentTrack);
        if (temp < sst)
        {
            result = index;
            sst = temp;
        }
    }
    return result;
}

void sortDiskQueue(vector<DiskQueue> requests, vector<DiskQueue>& firstRequestPerProcess, bool upwards)
{
    bool flag = false;
    for (int i=0; i<firstRequestPerProcess.size() && !flag; i++)
    {
        flag = true;
        for (int j=0; j<firstRequestPerProcess.size()-1; j++)
        {
            int index1 = firstRequestPerProcess[j].track;
            int index2 = firstRequestPerProcess[j+1].track;
            if (upwards) //sort in ascending order
            {
                if (requests[index2].track < requests[index1].track)
                {
                    DiskQueue temp = firstRequestPerProcess[j];
                    firstRequestPerProcess[j] = firstRequestPerProcess[j+1];
                    firstRequestPerProcess[j+1] = temp;
                    flag = false;
                }
            }
            else //sort in descending order
            {
                if (requests[index2].track > requests[index1].track)
                {
                    DiskQueue temp = firstRequestPerProcess[j];
                    firstRequestPerProcess[j] = firstRequestPerProcess[j+1];
                    firstRequestPerProcess[j+1] = temp;
                    flag = false;
                }
            }
        }
    }
}

string addToString(string str, int nb)
{
    stringstream ss;
    ss << nb;
    str = str + ss.str();
    return str;
}
