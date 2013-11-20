/*
	Init all the functions so that we can use them whenever
*/

//defines
#define FIFO_num 	0
#define LRU_num 	1
#define MFU_num 	2
#define CACHE_SIZE	500 //TODO: pick this size
#define BITS 32
#define ADDRESSPACE 2^BITS
#define PAGESIZE 4096
#define PTES ADDRESSPACE/PAGESIZE

//globals
int frameReplacementAlg = 0;
int TLB[CACHE_SIZE];

//structs
struct page{
	int startAddress;
	int endAddress;
	int dirtyBit;
};

struct data{
	int currentLine;
	int currentAddress;
	int currentOperation;
} line;

//used to init all global variables from parameters and malloc space for the data structures
void initilization(void);

//execute the operation on the memory location; return 
void doOp(int operation, int location);

//check to see if the requested page has already been translated in the near future
int checkTLB(void);

//if the TLB contains the memory translation, grab it and use it to create the physical address
void grabTLBEntry(void);

//check to see if the page table entry is already in the page table
int checkPageTable(void);

//translate the virtual address found in the page table to the physical address
void translateAddress(void);

//check to see if the address we are looking for is valid
int checkValidAddress(int address);

//check to see if the requested data was found in disk
int checkDiskFound(int address);

//check to see if there is a free frame
int checkForFreeFrame(void);

//evict a frame based on a replacement algorithm set by one of the global variables
void evict(void);

//check to see if the page is dirty
int checkDirty(struct page *thisPage);

//gracefully alert there was a seg fault
void segFault(void);

//bring the page from disk into memory
void updateMemory(struct page *thisPage);

//bring the page into the page table for future use
void updatePageTable(struct page *thisPage);

//add time to the performance metric
void addTime(struct performance, int time);

//move the control flow back to the main loop and read the next line
void nextLine(void);

//trigger the sequence of events that occur during a page fault
void pageFault(struct page *thisPage);