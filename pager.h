/*
	Init all the functions so that we can use them whenever
*/

#include <stdio.h>

#define ADDRESSPACE 4294967296
#define PAGESIZE 4096
#define PTES ADDRESSPACE/PAGESIZE
#define MAXTLB PTES/2
#define MAXTIME 9999999
#define MAXWSW 10
#define NUMPROCESSES 5
#define INITWS 5
#define MINPAGEFAULT 2
#define MAXPAGEFAULT 10
#define MAXPTP 30

FILE *fp; //input file pointer
char *paramFileName;

// global param variables and defaults
int maxPages 	= 500;
int TLBEntries 	= 10;
int MMtime 		= 2;
int TLBtime 	= 1;
int DISKtime 	= 1000;
int v 			= 0;

//used to tell our main loop if we have page faulted;
//if we have reread the line and do not go to the next line
int redo = 0;
int numPageFaults = 0;
int pageRequested;

//selects the parameter for which kind of replacement algo to used
//	0 - FIFO; 1 - LRU; 2 - MFU
int pageReplAlgo 			= 0;
int cacheReplAlgo 			= 0;
int pageTablePageReplAlgo 	= 0;

//selects the parameter for which kind of page table we want
//	0 - single; 1 - multi; 2 - inverted
int pageTableType = 0;

//times used for the page table delays
int singleLevelPercentage 	= .02;
int modNum 					= 500;				
int collisionPercentage 	= .02;

//used to regulate the working set window
int WSW = 5;

//used to regulate how many frames or page table pages we want to allow our computer to have in memory at one time
int numFrames 			= 20;//PTES; TODO: ideally it would be this value
int numPageTablePages 	= 20;//this is the number of page table pages we can store in memory at once

//EXAMPLE: used to control our FIFO indexes; tells us which one to evict next
int FIFOindex_page = 0;
int FIFOindex_page_table_page = 0;
int FIFOindex_cache = 0;


//represents frames in memory; if vAddress is -1, then the frame is empty
struct frame{
	int vAddress;			//the virtual address that goes with this page
	int pAddress;			//the physical address that goes with this page
	int dirtyBit;			//is the frame dirty?
	int referenceBit;		//has the frame been referenced?
	int processId;			//id for the process owning this page
};//frame

//represents a section of our total page table in the case you want to use a multi-level page table
struct pageTablePage{
	int idx;				//which page table partition is it?
	int startAddress;
	int dirtyBit;
};//pageTablePage

//used to represent a TLB entry; if vAddress is -1, then the entry is empty
struct TLBEntry{
	int vAddress;			//used to find if the address from the line is in the TLB
	int pAddress;			//the translated virtual address
	int frameNum;			//page table index of this page
	int dirtyBit;
};//TLBEntry

//used to represent the data grabbed from the current line
struct data{
	int processId;			//id for the current process
	int previousProcessId;	//id of the previous process
	uint currentAddress;	//the address that belongs in that line
	int currentOperation;	//the operation to perform on the currentAddress
} line;

//used to do running performance of however many lines we have read
struct perforamance{
	float runningAverage;	//keep a running average of the total access time for each instruction
	int currentRunningSum; 	//accumulated time for the current run 
	int runNumber;			//the number of lines that you have read
	int totalPageFaults;	//the total number of page faults
} program;

//used to regulate our working set window depending on the number of page faults we have encountered
struct workingSets{
	int availWorkingSet;	// number of pages available to this process
	int curWorkingSet;		// number of actual pages in the working set, could be less than WS
	int pageFaults[MAXWSW];	// maintains page faults for the last WSW instructions
	int pageFaultCursor;	// maintains position in the page fault array
};//workingSets

//globals; we will always need these
struct TLBEntry 		TLB[MAXTLB];
struct workingSets 		processWorkingSets[NUMPROCESSES];

//conditional arrays based on which page table type
struct frame 			*mainMemory; 
int 					*hashTable; 
struct pageTablePage 	*pageDirectory;
 

//function declarations
void initialization(void);
void getParams( int argc, char* argv[]);
void loadParams(char *paramFileName);
void flushMainMemory(void);
void flushCache(void);

int readNextLine(int redo);
void doOp(int operation, int pAddress, int time, int vAddress);
void grabNextLine(int PID, char RW, uint addr);

int checkTLB(int pageRequested);
int grabTLBEntry(int idx);
int checkTLBEntry(int address);
int evictTLB(void);
void updateCacheTable(struct TLBEntry *thisTLB, int pAddress, int vAddress, int frameNum);
void updateCache(int pAddresss, int vAddress);

int grabPTP(int pageRequested);
void pageTablePageFault(int pageTablePageRequested);
int checkPageDirectory(int pageTablePageRequested);
int evictPageTablePage(void);
void updatePageDirectory(int idx, int pageTablePageRequested);

int grabPTE(int address);
int checkMainMemory(int pageRequested);
int checkMainMemoryEntry(int pageRequested);
void pageFault(int pageRequested);
int checkForFreeFrame(void);
int evictPage(void);
void updateMainMemory(struct frame *thisFrame, int pageRequested);

void initWorkingSet(struct workingSets *thisWorkingSet);
int markWSPage(struct workingSets *thisWorkingSet, int fault);
int calcWSPageFaults(struct workingSets *thisWorkingSet);
int evictOwnPage(void);

void visual(void);
void openInputFile(void);
void run(void);