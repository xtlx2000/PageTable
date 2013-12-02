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
#define MAXWSW 10
#define WSW 5
#define NUMPROCESSES 5
#define MINPAGEFAULT 3
#define MAXPAGEFAULT 10

FILE *fp; //input file pointer

// global param variables and defaults
int maxPages = 500;
int TLBEntries = 10;
int MMtime = 2;
int TLBtime = 1;
int DISKtime = 1000;

//selects the parameter for which kind of replacement algo to used
//TODO: fill your replacement type here
//	0 - FIFO; 1 - LRU; 2 - MFU
int pageReplAlgo = 0;
int cacheReplAlgo = 0;
int pageTablePageReplAlgo = 0;

//selects the parameter for which kind of page table we want
//	0 - single; 1 - double; 2 - inverted
int pageTableType = 1;


//const int WSW = 5;

const int numFrames = 20;//PTES; TODO: ideally it would be this value
const int numPageTablePages = 20;//this is the number of page table pages we can store in memory at once

int FIFOindex_page = 0;
int FIFOindex_page_table_page = 0;
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

struct pageTablePage{
	int idx;				//which page table partition is it?
	int startAddress;
	int dirtyBit;
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
	int totalPageFaults;	// Joe: I think this would be a good performance metric to add 
} program;

struct workingSets{
	int processWS;			// number of actual pages in the working set, could be less than WSW
	int pageFaults[WSW];	// maintains page faults for the last WSW instructions
	struct workingSetElement *head; // linked list of pages in the WS
};

struct workingSetElement{
	struct frame *workingSetFrame;
	struct frame *next;
};



//globals
struct TLBEntry TLB[MAXTLB];
struct frame pageTable[20]; //TODO: these values need to be changed to their correct parameters
struct pageTablePage pageDirectory[20];
struct workingSets processWorkingSets[NUMPROCESSES]; // TODO


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
int evictPage(void);
int evictTLB(void);
int checkDirtyPTE(struct frame *thisFrame);
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
void pageTablePageFault(int pageTablePageRequested);
int grabPTP(int pageRequested);
int checkPageDirectory(int pageTablePageRequested);
int evictPageTablePage(void);
void updatePageDirectory(int idx, int pageTablePageRequested);
