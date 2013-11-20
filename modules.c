/*
Small library of functions that help to modularize this program
*/


#include "modules.h"

//used to init all global variables from parameters and malloc space for the data structures
void initilization(void){
   printf("initing everything\n");
}//initilization

//execute the operation on the memory location; return 
void doOp(int operation, int location){
   printf("doing the %d operation on memory location 0x%X\n", operation, location);
}//doOP

//check to see if the requested page has already been translated in the near future
int checkTLB(int address){
	printf("checking the TLB\n");
	int idx = 0;
	for(idx; idx<MAXTLB; idx++){
		if(TLB[idx].virtualAddress = address)
			return idx;
	}
   return 0; //indicates that it was not in the TLB
}//checkTLB

//if the TLB contains the memory translation, grab it and use it to create the physical address
int grabTLBEntry(int idx){
	printf("grabbing the TLB memory translation\n");
	struct TLBEntry thisTLBEntry = TLB[idx];	
	return thisTLBEntry.physicalAddress;
}//grabTLBEntry

//check to see if the page table entry is already in the page table
int checkPageTable(struct PTE *thisPTE){
   printf("Checking to see if the requested page table entry is in the page table\n");
   return -1; //indicates the page table does not contain the page table entry
}//checkPageTable

//translate the virtual address found in the page table to the physical address
void translateAddress(struct PTE *thisPTE){
   printf("translating address\n");
	thisPTE->address = 1; //address goes here
}//translateAddress

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
/*	for(idx=0; idx<ADDRESS_SPACE; idx++){
		if(mainMemory[idx]->empty == 1)
			return idx;	//return the frame idx for the empty frame
	}*/
	return -1;  		//return -1 on fail
}//checkForFreeFrame

//given a certain idx,  grabs the frame at that location
struct frame *grabFreeFrame(int idx){
	return &mainMem[idx];
}//grabFreeFrame

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
struct PTE *grabPTE(int address){
	//loop through the page table entries and  figure out if any of them are in the range of
	//address
	struct PTE *thisPTE;
	printf("finding the page associated with this address\n");
	return thisPTE;
}//grabPage

//update the gloabl struct line, so that it contains the next line's attributes
void grabNextLine(){
	printf("reading the next line (line %d) from the input file\n", program.runNumber);
/*	line.currentLine = 
	line.currentAddress = 
	line.currentOperation = */
}//grabNextLine

//move the control flow back to the main loop and read the next line
//redo is used to control page faults when the come back to this step
	//0 - page fault (read same line)
	//1 - read next line
void readNextLine(int redo){
	int idx;
	
	//if there was a page fault before, 
	//don't update to the next line, just read it again
	if(redo)
		grabNextLine();

	struct PTE *thisPTE = grabPTE(line.currentAddress);
	struct TLBEntry *thisTLB;
	
	if(idx = checkTLB(1/*line.currentAddress*/) > -1){	//check if it was in the TLB
		//1.)	index into the TLB data structure and grab the physical translation
		thisTLB->physicalAddress = grabTLBEntry(idx);

		//2.)	combine it with the offset to create the full address
		translateAddress(thisPTE);

		//3.)	do operation on that address
		doOp(line.currentOperation, thisTLB->physicalAddress);

		//4.)	check if the page needs to be dirtied or not
		if(line.currentOperation == 1)	//indicates a write
			thisPTE->dirtyBit = 1;

	} else if (idx = checkPageTable(thisPTE) > -1){ //if it wasn't, check if the PTE is at least in the page table 
		
	} else {										//if neither of these, then page fault
		pageFault(thisPTE);
	}
}//nextLine

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
	
/*	//1.)	first check if the address is not valid or is not on the disk
	if(!checkValidAddress(thisPTE->physicalAddress) || !checkDiskFound(thisPTE->physicalAddress)){
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
	thisFrame = grabFreeFrame(idx);

	//3.)	bring the page into the page table
	updatePageTable(thisFrame, thisPTE);

	//4.)	roll back to the previous line so that we can try it now the page has been loaded
	printf("END PAGE FAULT, TRY AGAIN!\n");
	readNextLine(0);	
	return 1; //indicates to read the next line
}//pageFault