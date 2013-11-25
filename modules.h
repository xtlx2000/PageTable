/*
	Init all the functions so that we can use them whenever
*/

#include <stdio.h>

#define BITS 32
#define ADDRESSPACE 4294967296
#define PAGESIZE 4096
#define PTES ADDRESSPACE/PAGESIZE
#define MAXTLB PTES/2
#define MAXTIME 9999999
#define MAXWSW 50

FILE *fp; //input file pointer

// global param variables and defaults
int maxPages = 500;
int TLBEntries = 10;
int MMtime = 2;
int TLBtime = 1;
int DISKtime = 1000;
int pageReplAlgo = 1;
int cacheReplAlgo = 1;
int pageTableType = 1;
int WSW = 5;

int FIFOindex_page = 0;
int FIFOindex_cache = 0;

int PID; // to be replaced by line struct
char RW;
uint addr;

int v = 0; // Verbosity

struct frame{
	int vAddress;			//the virtual address that goes with this page
	int pAddress;			//the physical address that goes with this page
	int dirtyBit;			//is the frame dirty?
	int referenceBit;		//has the frame been referenced?
};

struct TLBEntry{
	int vAddress;			//used to find if the address from the line is in the TLB
	int pAddress;			//the translated virtual address
	int frameNum;			//page table index of this page
	int dirtyBit;
};

struct data{
	int processId;			//id for the current process
	uint currentAddress;	//the address that belongs in that line
	int currentOperation;	//the operation to perform on the currentAddress
} line;

struct perforamance{
	float runningAverage;	//keep a running average of the total access time for each instruction
	int currentRunningSum; 	//accumulated time for the current run 
	int runNumber;			//the number of lines that you have read
} program;

struct processWorkingSets{
	int processesWS[5];
	int pageFault[5];
};

//globals
int frameReplacementAlg = 0;
struct TLBEntry TLB[MAXTLB];
struct frame pageTable[PTES];

void initialization(void);
void doOp(int operation, int pAddress, int time, int vAddress);
int checkTLB(int pageRequested);
int grabTLBEntry(int idx);
int grabPTE(int address);
int checkPageTable(int pageRequested);
int checkPageTableEntry(int pageRequested);
void writeToDisk(struct frame evictedFrame);
int checkValidAddress(int address);
int checkDiskFound(int address);
int checkForFreeFrame(void);
int evict(void);
int checkDirtyPTE(struct frame *thisFrame);
void segFault(void);
void updatePageTable(struct frame *thisFrame, int pageRequested);
int readNextLine(int redo);
void pageFault(int pageRequested);
int checkTLBEntry(int address);
void getParams( int argc, char* argv[]);
void grabNextLine(int PID, char RW, uint addr);
void updateCacheTable(struct TLBEntry *thisTLB, int pAddress, int vAddress, int frameNum);
void updateCache(int pAddresss, int vAddress);
void visual(void);
void readFromDisk(struct frame *thisFrame);