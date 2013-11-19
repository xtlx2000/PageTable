/*
Small library of functions that help to modularize this program
*/

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
	for(idx; idx<CACHE_SIZE; idx++){
		if(TLB[idx] == address)
			return idx;
	}
   return 0; //indicates that it was not in the TLB
}//checkTLB

//if the TLB contains the memory translation, grab it and use it to create the physical address
int grabTLBEntry(int idx){
	printf("grabbing the TLB memory translation\n");
	return TLB[idx];
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

//check to see if the requested data was found in disk
int checkDiskFound(int address){
	printf("checking to see if the address is in disk", address);
	return 0;
}//checkDiskFound

//check to see if there is a free frame
int checkForFreeFrame(void){
	printf("checking if there is a free frame\n");
	return 1;
}//checkForFreeFrame

//evict a frame based on a replacement algorithm set by one of the global variables
void evict(void){
	printf("evicting a frame to make room for someone else\n");
	switch(frameReplacementAlg){
		case 0: 
			printf("evicting a frame base on the LRU algorithm");
			break;
		case 1:
			printf("evicting a frame base on the FIFO algorithm");
			break;
		case 2:
			printf("evicting a frame base on the MFU algorithm");
			break
	}
}//evict

//check to see if the page is dirty
int checkDirty(struct page *thisPage){
	printf("checking if the page is dirty\n");
	return thisPage->dirtyBit;
}//checkDirty

//bring the page from disk into memory
void updateMemory(struct page *thisPage){
	printf("bringing the page into memory\n");
}//updateMemory

//bring the page into the page table for future use
void updatePageTable(struct page *thisPage){
	printf("bringing the page into the page table\n");
}//updatePageTable

//add time to the performance metric
void addTime(struct performance, int time){
	printf("adding %d time to the total time\n", time);
}//addTime

//given an address, this function will find the page that is associated with that address
struct page grabPage(int address){
	struct page thisPage;
	printf("finding the page associated with this address\n");
	return thisPage;
}//grabPage

//update the gloabl struct line, so that it contains the next line's attributes
void grabNextLine(){
	printf("reading the next line (line %d) from the input file\n", line.currentLine);
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
	
	//if there was a page fault before, don't update to the next line, just read it again
	if(redo)
		grabNextLine();

	struct page thisPage = grabPage(line.currentAddress);
	
	if(idx = checkTLB()){
		//1.)	hash into the TLB data structure and grab the physical translation
		int frameNumber = grabTLBEntry(idx);

		//2.)	combine it with the offset to create the full address
		int physicalAddress = translateAddress();

		//3.)	do operation on that address
		doOp(line.currentOperation, physicalAddress);

		//4.)	check if the page needs to be dirtied or not
		if(line.currentOperation == 1) //indicates a write

	}
}//nextLine

//gracefully alert there was a seg fault
void segFault(void){
	printf("ERROR: Segmentation Fault!\n");
	line.currentLine++;
	readNextLine(1);
}//segFault

//trigger the sequence of events that occur during a page fault
int pageFault(struct page *thisPage){
	printf("PAGE FAULT!");
	
	//1.)	first check if the address is not valid or is not on the disk
	if(!checkValidAddress(thisPage->address) || !checkDiskFound(thisPage->address)){
		segFault();
		return 0;
	}

	//2.	check if there is a free frame
	if(!checkForFreeFrame()){
		//2.a) evict someone using the replacement algorithms; check if the page is dirty
		evict();
		checkDirty(thisPage);
	}

	//3.)	bring the page into memory
	updateMemory(thisPage);

	//4.)	bring the page into the page table
	updatePageTable(thisPage);

	//5.)	roll back to the previous line so that we can try it now the page has been loaded
	readNextLine(0);

	print("END PAGE FAULT, TRY AGAIN!");
	return 1; //indicates a successful page fault return
}//pageFault