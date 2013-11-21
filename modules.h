/*
	Init all the functions so that we can use them whenever
*/

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
int maxTLB = 10;
int MMtime = 2;
int TLBtime = 1;
int DISKtime = 5;
int pageReplAlgo = 1;
int pageTableType = 1;
int WSW = 5;

int v = 0; // Verbosity

struct frame{
	int idx;				//index into the mainMemory array
	int address;			//the frame address refers to the physical address
	int empty;				//is the frame empty?
};

struct PTE{
	int *frame;				//points to its associated frame
	int address;			//the virtual address that goes with this page
	int dirtyBit;			//is the frame dirty?
	int referenceBit;		//has the frame been referenced?
};

struct TLBEntry{
	int virtualAddress;		//used to find if the address from the line is in the TLB
	int physicalAddress;	//the translated virtual address
};

struct data{
	int processId;			//id for the current process
	int currentAddress;		//the address that belongs in that line
	int currentOperation;	//the operation to perform on the currentAddress
} line;

struct perforamance{
	double runningAverage;	//keep a running average of the total access time for each instruction
	int runNumber;			//the number of lines that you have read
} program;

struct processWorkingSets{
	int processesWS[5];
	int pageFault[5];
};

//globals
int frameReplacementAlg = 0;
struct TLBEntry TLB[MAXTLB];
struct frame mainMem[PTES];

//used to init all global variables from parameters and malloc space for the data structures
void initilization(void);

//execute the operation on the memory location; return 
void doOp(int operation, int location);

//check to see if the requested page has already been translated in the near future
int checkTLB(int address);

//if the TLB contains the memory translation, grab it and use it to create the physical address
int grabTLBEntry(int idx);

//check to see if the page table entry is already in the page table
int checkPageTable(struct PTE *thisPTE);

//translate the virtual address found in the page table to the physical address
void translateAddress(struct PTE *thisPTE);

//check to see if the address we are looking for is valid
int checkValidAddress(int address);

//check to see if the requested data was found in disk
int checkDiskFound(int address);

//check to see if there is a free frame
int checkForFreeFrame(void);

//evict a frame based on a replacement algorithm set by one of the global variables
int evict(void);

//check to see if the page is dirty
int checkDirtyPTE(struct PTE *thisPTE);

//gracefully alert there was a seg fault
void segFault(void);

//bring the page into the page table for future use
void updatePageTable(struct frame *thisFrame, struct PTE *thisPTE);

//add time to the performance metric
void addTime(int time);

//move the control flow back to the main loop and read the next line
void nextLine(void);

//trigger the sequence of events that occur during a page fault
int pageFault(struct PTE *thisPTE);
