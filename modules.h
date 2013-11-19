/*
Small library of functions that help to modularize this program
*/

#define FIFO_num 	0
#define LRU_num 	1
#define MFU_num 	2

//used to init all global variables from parameters and malloc space for the data structures
void initilization(void){
   printf("initing everything\n");
}//initilization

//execute the operation on the memory location; return 
void doOp(int operation, int location){
   printf("doing the %d operation on memory location 0x%X\n", operation, location);
}//doOP

//check to see if the requested page has already been translated in the near future
int checkTLB(void){
   printf("checking the TLB\n");
   return 0;
}//checkTLB

//if the TLB contains the memory translation, grab it and use it to create the physical address
void grabTLBEntry(void){
	printf("grabbing the TLB memory translation\n");
}//grabTLBEntry

//check to see if the page table entry is already in the page table
int checkPageTable(void){
   printf("Checking to see if the requested page table entry is in the page table\n")
   return 0;
}//checkPageTable

//translate the virtual address found in the page table to the physical address
void translateAddress(void){
   printf("translating address\n");
}//translateAddress

//check to see if the address we are looking for is valid
int checkValidAddress(int address){	
	printf("checking to see if 0x%X is a valid address\n", address);
	return 0;
}//checkValidAddress

//check to see if there is a free frame
int checkForFreeFrame(void){
	printf("checking if there is a free frame\n");
	return 0;
}//checkForFreeFrame

//evict a frame based on a replacement algorithm set by one of the global variables
void evict(void){
	printf("evicting a frame to make room for someone else\n");
}//evict

//check to see if the page is dirty
int checkDirty(struct page){
	printf("checking if the page located at 0x%X is dirty\n", page.address);
	return 0;
}//checkDirty

//gracefully alert there was a seg fault
void segFault(void){
	printf("ERROR: Segmentation Fault!\n");
}//segFault

//bring the page from disk into memory
void updateMemory(struct page){
	printf("bringing the page into memory\n");
}//updateMemory

//bring the page into the page table for future use
void updatePageTable(struct page){
	printf("bringing the page into the page table\n");
}//updatePageTable

//add time to the performance metric
void addTime(struct performance, int time){
	printf("adding %d time to the total time", time);
}//addTime

//trigger the sequence of events that occur during a page fault
void pageFault(struct page){
	printf("PAGE FAULT!");
}//pageFault