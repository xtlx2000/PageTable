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

int PID; // to be replaced by line struct
char RW;
uint addr;

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

void initialization(void);
void doOp(int operation, int location);
int checkTLB(struct PTE *thisPTE, struct TLBEntry *thisTLB);
int grabTLBEntry(int idx);
int checkPageTable(struct PTE *thisPTE);
int checkPageTableEntry(struct PTE *thisPTE);
void translateAddress(struct PTE *thisPTE);
int checkValidAddress(int address);
int checkDiskFound(int address);
int checkForFreeFrame(void);
int evict(void);
int checkDirtyPTE(struct PTE *thisPTE);
void segFault(void);
void updatePageTable(struct frame *thisFrame, struct PTE *thisPTE);
void addTime(int time);
int readNextLine(int redo);
int pageFault(struct PTE *thisPTE);
int checkTLBEntry(int address);
void getParams( int argc, char* argv[]);