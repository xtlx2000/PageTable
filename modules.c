/*
Small library of functions that help to modularize this program
*/


#include "modules.h"

//used to init all global variables from parameters and malloc space for the data structures
void initialization(void){
   program.runNumber = 0;

   
   //init our empty page table
   int idx;
	for(idx = 0; idx<PTES; idx++){
		struct frame thisFrame;

      thisFrame.vAddress      = -1;
      thisFrame.pAddress      = idx*4096;
		thisFrame.dirtyBit		= 0;
		thisFrame.referenceBit 	= 0;

		pageTable[idx] 		= thisFrame;
	}

   //init TLB
   for(idx = 0; idx<TLBEntries; idx++){
      struct TLBEntry thisTLB;

      thisTLB.vAddress = -1;
      thisTLB.pAddress = -1;
      thisTLB.frameNum = -1;
      thisTLB.dirtyBit =  0;
      TLB[idx] = thisTLB;
   }
}//initilization

//execute the operation on the memory location; return 
void doOp(int operation, int pAddress, int time, int vAddress){
   if(vAddress > -1)
      updateCache(pAddress, vAddress);

   program.currentRunningSum += time;
}//doOP


//used to update the cache whenever there is a do op performed
void updateCache(int pAddress, int vAddress){
   //1.  check if there is a free frame
   int idx = checkForFreeTLB();
   if(idx < 0){
      //1.a) evict someone using the replacement algorithms; check if the page is dirty
      idx = evictTLB();
   } 

   //2.) bring the new info into the cache location
   updateCacheTable(&TLB[idx], pAddress, vAddress, idx);
}//updateCache

int checkForFreeTLB(){
   int idx;
   for(idx=0; idx<TLBEntries; idx++){
      if(TLB[idx].vAddress == -1)
         return idx; //return the frame idx for the empty frame         
   }
   return -1;        //return -1 on fail
}//checkForFreeTLB

//chooses one of the TLB entries to evict
int evictTLB(void){
   int retVal;
   struct TLBEntry evictedTLBEntry;
   switch(pageReplAlgo){
      case 0: //FIFO
         //printf("evicting a frame base on the LRU algorithm");
         evictedTLBEntry = TLB[FIFOindex_cache];
         retVal = FIFOindex_cache;
         break;
      case 1: // LRU
         //printf("evicting a frame base on the FIFO algorithm");
         //check for dirty and reference bit
         break;
      case 2: // MFU
         //printf("evicting a frame base on the MFU algorithm");
         break;
      default:
         retVal = -1;
   }

   if(evictedTLBEntry.dirtyBit){
      struct frame thisFrame;
      thisFrame.vAddress = evictedTLBEntry.vAddress;
      thisFrame.pAddress = evictedTLBEntry.pAddress;
      writeToDisk(thisFrame);
   }
   
   //return idx to evicted frame
   return retVal;
}//evictTLB

//move the new information to the cache
void updateCacheTable(struct TLBEntry *thisTLB, int pAddress, int vAddress, int frameNum){
   thisTLB->pAddress = pAddress;
   thisTLB->vAddress = vAddress;
   thisTLB->frameNum = frameNum;
   if (cacheReplAlgo = 0)
      FIFOindex_cache = PTES - (FIFOindex_cache + 1 % PTES);
}//updateCacheTable


// calculate the running average at the end of each loop
void calcAverage( void ) {
   program.runningAverage = (float)program.currentRunningSum/(float)program.runNumber;
}

//check to see if the page table entry is already in the page table
int checkPageTable(int pageRequested){
	int idx = checkPageTableEntry(pageRequested);
   //printf("check page table: %d\n", idx);
	if (idx > -1) {//if it wasn't, check if the PTE is at least in the page checkPageTable
      doOp(line.currentOperation, pageTable[idx].pAddress, MMtime, pageTable[idx].vAddress);

      //2.) check if the page needs to be dirtied or not
      if(line.currentOperation == 1)   //indicates a write
         pageTable[idx].dirtyBit = 1;

		return idx;
   }
	return -1; //indicates it wasn't in the page table already need to grab from memory
}//checkPageTable

//check to see if the actual page is in the TLB
int checkPageTableEntry(int pageRequested){
   //printf("Checking to see if the requested page table entry is in the page table\n");
   int i;
   //printf("if(%X == %X)\n", pageTable[0].vAddress, pageRequested);
   for ( i=0; i<PTES; i++ ) {
      if (pageTable[i].vAddress == pageRequested)
         return i;
   }
   return -1; //indicates the page table does not contain the page table entry
}//checkPageTableEntry

//check to see if the address we are looking for is valid
int checkValidAddress(int address){	
	//printf("checking to see if 0x%X is a valid address\n", address);
   if (address >= 0 || address <= ADDRESSPACE)
      return 1; //indicates a valid address
   return 0;
}//checkValidAddress

//check to see if the requested data was found in disk
int checkDiskFound(int address){
	//printf("checking to see if the address is in disk", address);
	return 1; //indicates a valid address
}//checkDiskFound

//check to see if there is a free frame; 
//loop through all the frames in main memory until you find one that has the property of empty
int checkForFreeFrame(void){
	//printf("checking if there is a free frame\n");
	int idx;
	for(idx=0; idx<PTES; idx++){
		if(pageTable[idx].vAddress == -1){
         //printf("THERE WAS A FREE FRAME @ %d\n", idx);
         return idx; //return the frame idx for the empty frame
      }
			
	}
	return -1;  		//return -1 on fail
}//checkForFreeFrame

//evict a frame based on a replacement algorithm set by one of the global variables
int evict(void){
	//printf("evicting a frame to make room for someone else\n");
   int retVal;
   struct frame evictedFrame;
   switch(pageReplAlgo){
		case 0: //FIFO
			//printf("evicting a frame base on the LRU algorithm");
			evictedFrame = pageTable[FIFOindex_page];
         retVal = FIFOindex_page;
			break;
		case 1: // LRU
			//printf("evicting a frame base on the FIFO algorithm");
         //check for dirty and reference bit
			break;
		case 2: // MFU
			//printf("evicting a frame base on the MFU algorithm");
			break;
      default:
         retVal = -1;
	}

	if(evictedFrame.dirtyBit){
      puts("DIRTY BIT");
      printf("Adding %d ns for a disk hit", DISKtime);
		writeToDisk(evictedFrame);
   }

		//return idx to evicted frame
	return retVal;
}//evict

//check to see if the frame is dirty
int checkDirtyPTE(struct frame *thisFrame){
	//printf("checking if the page is dirty\n");
	return thisFrame->dirtyBit;
}//checkDirtyPTE

//given an address, this function will find the page that is associated with that address
int grabPTE(int address){
	int idx;
	for(idx=0; idx<PTES; idx++){
		if((idx+1)*4096 > address){
			return (idx)*4096;
		}
	}
	return -1; //indicates if the it could not find a page
}//grabPage

//check if the page is in the tlb, and handle anything that should happen if it is; if it isn't return 0
int checkTLB(int pageRequested){
	int idx;
	//printf("Checking the TLB\n");

	/*testing simpliest case*/
	/*struct TLBEntry thisTLB;
	thisTLB.virtualAddress = 50;
	thisTLB.physicalAddress = 100;
	TLB[10] = thisTLB;*/

	idx = checkTLBEntry(pageRequested);
	if(idx > -1){	//check if it was in the TLB
		//1.)	do operation on that address
		doOp(line.currentOperation, grabTLBEntry(idx), TLBtime, -1); //no need to update the cache, because its already there

		//2.)	check if the page needs to be dirtied or not
		if(line.currentOperation == 1)	//indicates a write
			pageTable[TLB[idx].frameNum].dirtyBit = 1;
		
		return 1;
	} 
	return 0; //indicates that it wasn't in the TLB; check the page table 	
}//checkTLB

//check to see if the requested page has already been translated in the near future
int checkTLBEntry(int address){
	//printf("checking the TLB entry\n");
	int idx;
	for(idx= 0; idx<TLBEntries; idx++){
		if(TLB[idx].vAddress == address) //found the address in the TLB, return the idx
			return idx;
	}
   return -1; //indicates that it was not in the TLB
}//checkTLB

//if the TLB contains the memory translation, grab it and use it to create the physical address
int grabTLBEntry(int idx){
	//printf("grabbing the TLB memory translation\n");
	struct TLBEntry thisTLBEntry = TLB[idx];	
	return thisTLBEntry.pAddress;
}//grabTLBEntry

//move the control flow back to the main loop and read the next line
//redo is used to control page faults when the come back to this step
	//0 - page fault (read same line)
	//1 - read next line
int readNextLine(int redo){
	//1.)   TODO: update working sets goes here


	//2.)   if there was a page fault before, don't update to the next line, just read it again
   int ret = 0;
	if(!redo){
      ret = fscanf(fp, "%d %c %x", &PID, &RW, &addr);
		grabNextLine(PID, RW, addr);
   }

	return ret; //EOF on fail
}//nextLine

//update the gloabl struct line, so that it contains the next line's attributes
void grabNextLine(int PID, char RW, uint addr){
   program.runNumber++;
	line.processId		= PID;
	line.currentAddress = addr;
	if(RW == 'W'){
		line.currentOperation = 1;
   } else {
      line.currentOperation = 0;
   }
	return;
}//grabNextLine

//gracefully alert there was a seg fault
void segFault(void){
	//printf("ERROR: Segmentation Fault!\n");
}//segFault

//write a dirty page to the disk; it needs to be updated
void writeToDisk(struct frame evictedFrame){
   doOp(1, evictedFrame.vAddress, DISKtime, -1); //this is just a write back, no need to update the cache
	//printf("writing to disk\n");
}//writeToDisk

//trigger the sequence of events that occur during a page fault
void pageFault(int pageRequested){
	//printf("PAGE FAULT on 0x%X!\n", pageRequested);

   //1.	check if there is a free frame
	int idx = checkForFreeFrame();
	if(idx < 0){
		//1.a) evict someone using the replacement algorithms; check if the page is dirty
		idx = evict();
	} 

	//2.)	bring the page into the page table
   //printf("pageTable[%d].vAddress = 0x%X, pageRequested = %d\n", idx, pageTable[idx].vAddress, pageRequested);
	updatePageTable(&pageTable[idx], pageRequested);
   //printf("pageTable[%d].vAddress = 0x%X, pageRequested = %d\n", idx, pageTable[idx].vAddress, pageRequested);

	//printf("END PAGE FAULT, TRY AGAIN!\n");
}//pageFault

//bring the page into the page table for future use
void updatePageTable(struct frame *thisFrame, int pageRequested){
   //bring the page into main memory
   readFromDisk(thisFrame);
   thisFrame->vAddress = pageRequested;
   thisFrame->dirtyBit = 0;
   thisFrame->referenceBit= 0;
   if (pageReplAlgo = 0)
      FIFOindex_page = PTES - (FIFOindex_page + 1 % PTES);
}//updatePageTable

//add the time it takes to read from the disk and bring the page into main memory
void readFromDisk(struct frame *thisFrame){
   doOp(0, thisFrame->vAddress, DISKtime, -1); //this is just a read from, no need to update the cache
}//readFromDisk

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
               TLBEntries = value; 
               printf("Max number of TLB entries set to %d.\n", TLBEntries);
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
               DISKtime = value*1000;
               printf("Time needed to read/write a page to/from disk");
               printf(" set to %d (msec).\n", DISKtime);
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

//print the current status of the main memory, and the page table
void visual(void){
   int i;
   printf("***AFTER READING LINE %d***\nop: %d add: 0x%X\n", program.runNumber, line.currentOperation, line.currentAddress);
   printf("mainMemory: [ ");
   for(i=0; i<PTES; i++){
      if(pageTable[i].vAddress != -1){
         printf("0x%X ", pageTable[i]);
      } else {
         //printf("(empty frame) ");
      }
   }
   printf("]\n");

   printf("TLB: [ ");
   for(i=0; i<TLBEntries; i++){
      if(TLB[i].vAddress != -1){
         printf("0x%X ", TLB[i].vAddress);
      } else {
         //printf("(empty TLB) ");
      }
   }
   printf("]\n");   
}//visual