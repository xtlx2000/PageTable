/*
Small library of functions that help to modularize this program
*/


#include "modules.h"

//used to init all global variables from parameters and malloc space for the data structures
void initialization(void){
   printf("initing everything\n");
   program.runNumber = 1;

   int idx;
	for(idx = 0; idx<PTES; idx++){
		struct frame thisFrame;
		struct PTE thisPTE;

		thisPTE.frameNum		= idx;
		thisPTE.address 		= idx*4096;
		thisPTE.dirtyBit		= 0;
		thisPTE.referenceBit 	= 0;

		thisFrame.empty			= 1;
		thisPTE.frame 			= &thisFrame;
		pageTable[idx] 			= thisPTE;
	}
}//initilization

//execute the operation on the memory location; return 
void doOp(int operation, int location){
   printf("doing the %d operation on memory location 0x%X\n", operation, location);
}//doOP


//check to see if the page table entry is already in the page table
int checkPageTable(struct PTE *thisPTE){
	int idx = checkPageTableEntry(thisPTE);
	if (idx) //if it wasn't, check if the PTE is at least in the page checkPageTable
		return idx;

	return 0; //indicates it wasn't in the page table already need to grab from memory
}//checkPageTable

//check to see if the actual page is in the TLB
int checkPageTableEntry(struct PTE *thisPTE){
   printf("Checking to see if the requested page table entry is in the page table\n");
   return 0; //indicates the page table does not contain the page table entry
}//checkPageTableEntry

//check to see if the address we are looking for is valid
int checkValidAddress(int address){	
	printf("checking to see if 0x%X is a valid address\n", address);
	return 1; //indicates a valid address
}//checkValidAddress

//check to see if the requested data was found in disk
int checkDiskFound(int address){
	printf("checking to see if the address is in disk", address);
	return 1; //indicates a valid address
}//checkDiskFound

//check to see if there is a free frame; 
//loop through all the frames in main memory until you find one that has the property of empty
int checkForFreeFrame(void){
	printf("checking if there is a free frame\n");
	int idx;
/*	for(idx=0; idx<PTES; idx++){
		if(mainMemory[idx]->empty == 1)
			return idx;	//return the frame idx for the empty frame
	}*/
	return -1;  		//return -1 on fail
}//checkForFreeFrame

//evict a frame based on a replacement algorithm set by one of the global variables
int evict(void){
	printf("evicting a frame to make room for someone else\n");
/*
	switch(frameReplacementAlg){
		case 0: //LRU+
			printf("evicting a frame base on the LRU algorithm");
			//check for dirty and reference bit
			break;
		case 1:
			printf("evicting a frame base on the FIFO algorithm");
			break;
		case 2:
			printf("evicting a frame base on the MFU algorithm");
			break
	}

	if(checkDirty(evictedFrame))
		writeToDisk(evictedFrame);*/
		//return idx to evicted frame
	return -1; //return on fail
}//evict

//check to see if the frame is dirty
int checkDirtyPTE(struct PTE *thisPTE){
	printf("checking if the page is dirty\n");
	return thisPTE->dirtyBit;
}//checkDirtyPTE

//bring the page into the page table for future use
void updatePageTable(struct frame *thisFrame, struct PTE *thisPTE){
	printf("bringing the page into the page table\n");
}//updatePageTable

//add time to the performance metric
void addTime(int time){
	printf("adding %d time to the total time\n", time);
}//addTime

//given an address, this function will find the page that is associated with that address
int grabPTE(int address, struct PTE *thisPTE){
	int idx;
	for(idx=0; idx<PTES; idx++){
		if(idx*4096 > address){
			thisPTE = &pageTable[idx];
			return idx;
		}
	}
	return 0; //indicates if the it could not find a page
}//grabPage

//check if the page is in the tlb, and handle anything that should happen if it is; if it isn't return 0
int checkTLB(struct PTE *thisPTE){
	int idx;
	printf("Checking the TLB\n");

	/*testing simpliest case*/
	struct TLBEntry thisTLB;
	thisTLB.virtualAddress = 50;
	thisTLB.physicalAddress = 100;
	TLB[10] = thisTLB;

	idx = checkTLBEntry(thisPTE->address);
	if(idx > -1){	//check if it was in the TLB
		//1.)	do operation on that address
		doOp(line.currentOperation, grabTLBEntry(idx));

		//2.)	check if the page needs to be dirtied or not
		if(line.currentOperation == 1)	//indicates a write
			thisPTE->dirtyBit = 1;
		
		return 1;
	} 
	return 0; //indicates that it wasn't in the TLB; check the page table 	
}//checkTLB

//check to see if the requested page has already been translated in the near future
int checkTLBEntry(int address){
	printf("checking the TLB entry\n");
	int idx;
	for(idx= 0; idx<MAXTLB; idx++){
		if(TLB[idx].virtualAddress == address) //found the address in the TLB, return the idx
			return idx;
	}
   return -1; //indicates that it was not in the TLB
}//checkTLB

//if the TLB contains the memory translation, grab it and use it to create the physical address
int grabTLBEntry(int idx){
	printf("grabbing the TLB memory translation\n");
	struct TLBEntry thisTLBEntry = TLB[idx];	
	return thisTLBEntry.physicalAddress;
}//grabTLBEntry

//translate the virtual address found in the page table to the physical address
void translateAddress(struct PTE *thisPTE){
	//TODO: set the frame pointer on the PTE
	//TODO: create new TLB to place in the cache
   printf("translating address\n");
	thisPTE->address = 1; //address goes here
}//translateAddress


//move the control flow back to the main loop and read the next line
//redo is used to control page faults when the come back to this step
	//0 - page fault (read same line)
	//1 - read next line
int readNextLine(int redo){
	//1.) TODO: update working sets goes here
	//if there was a page fault before, 
	//don't update to the next line, just read it again
	int ret = fscanf(fp, "%d %c %x", &PID, &RW, &addr);
/*	printf("Process ID: %d; Operation: %c; Address: 0x%x\n",PID, RW, addr);*/
	if(!redo)
		grabNextLine(PID, RW, addr);

	return ret;
}//nextLine

//update the gloabl struct line, so that it contains the next line's attributes
void grabNextLine(int PID, char RW, uint addr){
	line.processId		= PID;
	line.currentAddress = addr;
	if(RW == 'W')
		line.currentOperation = 1;

	printf("line processId %d\n address %x\n operation %d\n", line.processId, line.currentAddress, line.currentOperation);
	return;
}//grabNextLine

//gracefully alert there was a seg fault
void segFault(void){
	printf("ERROR: Segmentation Fault!\n");
}//segFault

//write a dirty page to the disk; it needs to be updated
void writeToDisk(struct PTE thisPTE){
	printf("writing to disk\n");

}//writeToDisk

//trigger the sequence of events that occur during a page fault
int pageFault(struct PTE *thisPTE){
	printf("PAGE FAULT!\n");
	
/*	//1.)	first check if the address is not valid 
	if(!checkValidAddress(thisPTE->physicalAddress) ){
		segFault();
		return 1; //triggers the next read to go to the next line
	}*/

	//2.	check if there is a free frame
	int idx;
	struct frame *thisFrame;
	if(idx = checkForFreeFrame() < 0){
		//2.a) evict someone using the replacement algorithms; check if the page is dirty
		idx = evict();
	} 
	/*thisFrame = grabFreeFrame(idx);

	//3.)	bring the page into the page table
	updatePageTable(thisFrame, thisPTE);*/

	//4.)	roll back to the previous line so that we can try it now the page has been loaded
	printf("END PAGE FAULT, TRY AGAIN!\n");
	return 1; //indicates to read the next line
}//pageFault

void getParams( int argc, char* argv[]){

   if ( argc >= 2 ) { // flags or params are set
      int i;
      for (i=1 ; i<argc ; i++) {
         // User specified file 
         if (strncmp(argv[i],"file=", 5)==0 ) {
            fp = fopen(argv[i]+5, "r");
            if (fp == NULL) {
               printf("Can't open input file: %s\n", argv[i]+5);
               printf("Loading default file. \n");
            }
            else {
               printf("File option engaged: %s\n", argv[i]+5);
            }
         // Verbose option 
         } else if (strcmp(argv[i],"-verbose")==0 || strcmp(argv[i],"-v")==0) {
            printf("Verbose option engaged.\n");
            v = 1;
         // Very verbose option 
         } else if (strcmp(argv[i],"-veryverbose")==0 || strcmp(argv[i],"-vv")==0) {
            printf("Very Verbose option engaged.\n");
            v = 2;
         // Number of pages specified
         } else if (strncmp(argv[i],"Pages=", 6)==0 ) {
            int value;
            value = atoi(argv[i]+6);
            if ( value<1 || value>PTES ) {
               printf("usage: Pages={1-%d}\n", (int)PTES);
            } else {
               maxPages = value;
               printf("Number of pages set to %d.\n", maxPages);
            }
         // Max Number of TLB page table entries specified
         } else if (strncmp(argv[i],"TLB=", 4)==0 ) {
            int value;
            value = atoi(argv[i]+4);
            if ( value<1 || value>MAXTLB ) {
               printf("usage: TLB={1-%d}\n", (int)MAXTLB);
            } else {
               maxTLB = value; 
               printf("Max number of TLB entries set to %d.\n", maxTLB);
            }
         // time needed to read and write main memory specified
         } else if (strncmp(argv[i],"MMtime=", 7)==0 ) {
            int value;
            value = atoi(argv[i]+7);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: MMtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               MMtime = value;
               printf("Time needed to read/write a main memory");
               printf("page set to %d (nsec).\n", value);
            }
            // time needed to read/write a page table entry in the TLB specified 
         } else if (strncmp(argv[i],"TLBtime=", 8)==0 ) {
            int value;
            value = atoi(argv[i]+8);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: TLBtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               TLBtime = value;
               printf("Time needed to read/write a page table entry");
               printf(" in the TLB set to %d (nsec).\n", TLBtime);
            }
            // time needed to read/write a page to/from disk specified
         } else if (strncmp(argv[i],"DISKtime=", 9)==0 ) {
            int value;
            value = atoi(argv[i]+9);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: DISKtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               DISKtime = value;
               printf("Time needed to read/write a page to/from disk");
               printf(" set to %d (nsec).\n", DISKtime);
            }
         // Page replacement algorithm specified
         } else if (strncmp(argv[i],"PR=",3)==0) {
            int value;
            value = atoi(argv[i]+3);
            if ( value<1 || value>3 ) {
               printf("usage: PageRep={1=FIFO, 2=LRU, 3=MFU}\n");
            } else {
               pageReplAlgo = value;
               printf("Page Replacement set to %d.\n", pageReplAlgo);
            }
         // Page table type specified
         } else if (strncmp(argv[i],"PT=",3)==0) {
            int value;
            value = atoi(argv[i]+3);
            if ( value<1 || value>3 ) {
               printf("usage: PT={1=Single level, 2=Directory, 3=Inverted}\n");
            } else {
               pageTableType = value;
               printf("Page Table type set to %d.\n", pageTableType);
            }
         // Working Set Window specified
         } else if (strncmp(argv[i],"WSW=",4)==0) {
            int value;
            value = atoi(argv[i]+4);
            if ( value<1 || value>MAXWSW ) {
               printf("usage: WSW={1-%d}\n", MAXWSW);
            } else {
               pageTableType = value;
               printf("Working Set Window set to %d.\n", pageTableType);
            }
         // incorrect flag
         } else {
               printf( "usage: %s -verbose || -v  ||\n" , argv[0]);
               printf("\t   -veryverbose || -vv ||\n");
               printf("\t      -showcmds || -s  ||\n");
               printf("\tpages = {number of pages available}\n");
               printf("\tTLB = {number of PTE's available to TLB}\n");
               printf("\tMMtime = {time needed to read/write a main memory page}\n");
               printf("\tTLBtime = {time needed to read/write a page table entry in the TLB}\n");
               printf("\tDISKtime = {time needed to read/write a page to/from disk}\n");
               printf("\tPR = {1=FIFO, 2=LRU, 3=MFU}\n");
               printf("\tPT = {1=Single level, 2=Directory, 3=Inverted}\n");
               printf("\tWSW = {number of instructions in the working set window}\n\n");
         }
      }
   }
}
