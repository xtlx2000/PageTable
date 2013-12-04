/*
Enormous library of functions that help to modularize this program
*/

#include "modules.h"
#include "student_func.c"
#include "interface.c"

//execute the operation on the memory location; return 
void doOp(int operation, int pAddress, int time, int vAddress){
   if(vAddress > -1 && pAddress > -1)
      updateCache(pAddress, vAddress);

   program.currentRunningSum += time;
}//doOP

//used to update the cache whenever there is a do op performed
void updateCache(int pAddress, int vAddress){
   //1.  check if there is a free frame
   int idx = checkForFreeTLB();
   if(idx < 0){
      if(v)
         puts("CACHE EVICTION");
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

//move the new information to the cache
void updateCacheTable(struct TLBEntry *thisTLB, int pAddress, int vAddress, int frameNum){
   thisTLB->pAddress = pAddress;
   thisTLB->vAddress = vAddress;
   thisTLB->frameNum = frameNum;
}//updateCacheTable

// calculate the running average at the end of each loop
void calcAverage( void ) {
   program.runningAverage = (float)program.currentRunningSum/(float)program.runNumber;
}//calcAverage

//check to see if the page table entry is already in the page table
int checkMainMemory(int pageRequested){
	int idx = checkMainMemoryEntry(pageRequested);
   //printf("check page table: %d\n", idx);
	if (idx > -1) {//if it wasn't, check if the PTE is at least in the page checkmainMemory
      doOp(line.currentOperation, mainMemory[idx].pAddress, MMtime, mainMemory[idx].vAddress);

      //2.) check if the page needs to be dirtied or not
      if(line.currentOperation == 1)   //indicates a write
         mainMemory[idx].dirtyBit = 1;

		return idx;
   }
	return -1; //indicates it wasn't in the page table already need to grab from memory
}//checkmainMemory

//check to see if the actual page is in the TLB
int checkMainMemoryEntry(int pageRequested){
	int i;
   for ( i=0; i<numFrames; i++ ) {
      if (mainMemory[i].vAddress == pageRequested)
   	    return i;
   }

   return -1; //indicates that it was not in memory
}//checkmainMemoryEntry

//bring in a page table page to memory
void pageTablePageFault(int pageTablePageRequested){
   //1.  check if there is a free page table page
   int idx = checkForFreeFrame();
   if(idx < 0){
      if(v)
         puts("PAGE TABLE PAGE EVICTION");
      //1.a if there isn't one, evict someone then bring in the new page table page
      idx = evictPageTablePage(); // pick a page table page to evict
   }
   updatePageDirectory(idx, pageTablePageRequested);
}//pageTablePageFault

//grabs the page table page that contains the address linked to the requested page
int grabPTP(int pageRequested){
   int idx;
   for(idx=0; idx<PTES; idx++){
      if((idx+1)*4096*1024 > pageRequested){
         return idx;
      }
   }
   return -1; //indicates if the it could not find a page
}//pageRequested

//loop through all the page directory frames and see if your page table page needs to be brought into memory
int checkPageDirectory(int pageTablePageRequested){
   int i;
   for(i=0; i<numPageTablePages; i++){
      if(pageDirectory[i].idx == pageTablePageRequested){       //the page table page was in memory
         return i;
      }
   }//for

   return -1; //indicates the page table page was not in memmory
}//checkPageDirectory

//replace the page table page you evicted with a new one
void updatePageDirectory(int idx, int pageTablePageRequested){
   pageDirectory[idx].idx        	= pageTablePageRequested;
   pageDirectory[idx].startAddress  = pageTablePageRequested*4096;
   pageDirectory[idx].dirtyBit   	= 0;
}//updatePageDirectory

//check to see if there is a free frame; 
//loop through all the frames in main memory until you find one that has the property of empty
int checkForFreeFrame(void){
	int idx;
	for(idx=0; idx<numFrames; idx++){
		if(mainMemory[idx].vAddress == -1 &&
         processWorkingSets[line.processId].availWorkingSet > 
         processWorkingSets[line.processId].curWorkingSet)  {  
            return idx; //return the frame idx for the empty frame
      }			
	}
	return -1;  //return -1 on fail
}//checkForFreeFrame

//given an address, this function will find the page that is associated with that address
int grabPTE(int address){
	int idx, ret = -1, pageTablePageRequested;

   //1.) first, find the page that we need given the address read from the file;
   for(idx=0; idx<PTES; idx++){
      if((idx+1)*4096 > address){
         ret = idx*4096;
         break;
      }
   }//for 

   //2.) Next, decide which page table type we have, so that we can add the proper amount of delays
   //    for the simulation. In the case of multi-level, we might have to bring in additional pages
   switch(pageTableType){
      case 0:  //single level: just add some time that it would 
               //              take to make it through the PTEs  

         doOp(1, -1, singleLevelTime, -1); 
         break;
      case 1:  //multi-level: check if the page table page 
               //             you need is in memory before 
               //             looking up your page

         if(ret > -1)
            pageTablePageRequested = grabPTP(ret);
         if(pageTablePageRequested != -1){
            idx = checkPageDirectory(pageTablePageRequested);
            if(idx == -1) //page table page was NOT in memory
               pageTablePageFault(pageTablePageRequested);
         }
         break;
      case 2:  //inverted page table: hash into the table, then
               //                     if there is a conflict, add some time
               //                     based on how many conflict there were
         hashTable[ ((line.processId+1) * address) % modNum ]++; 
         if (hashTable[ ((line.processId+1) * address) % modNum ] > 1) {
            //add some time only if the hash produces a collison
            doOp(1,-1,hashTable[((line.processId+1)*address)%modNum]*collisionPercentage,-1); 
            //printf("There has been a collision.\n");
         }
         break;
   }
	return ret; //-1 for failure to find a page; otherwise, return the start address of the page you want
}//grabPage

//check if the page is in the tlb, and handle anything that should happen if it is; if it isn't return 0
int checkTLB(int pageRequested){
	int idx;
	//printf("Checking the TLB\n");

	idx = checkTLBEntry(pageRequested);
	if(idx > -1){	//check if it was in the TLB
		//1.)	do operation on that address
		doOp(line.currentOperation, grabTLBEntry(idx), TLBtime, -1); //no need to update the cache, because its already there

		//2.)	check if the page needs to be dirtied or not
		if(line.currentOperation == 1)	//indicates a write
			mainMemory[TLB[idx].frameNum].dirtyBit = 1;
		
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

//remove all TLB entries from the TLB
void flushCache(void){
   int idx;
   for(idx=0; idx<TLBEntries; idx++){
      struct TLBEntry thisTLB;

      thisTLB.vAddress = -1;
      thisTLB.pAddress = -1;
      thisTLB.frameNum = -1;
      thisTLB.dirtyBit =  0;
      TLB[idx] = thisTLB;
   }
}//flushCache

//remove all frames from the the page table array
void flushMainMemory(void){
   int idx;
   for(idx=0; idx<numFrames; idx++){
      struct frame thisFrame;

      thisFrame.vAddress      = -1;
      thisFrame.pAddress      = idx*4096;
      thisFrame.dirtyBit      = 0;
      thisFrame.referenceBit  = 0;
      thisFrame.processId     = -1;

      mainMemory[idx]       = thisFrame;
   }
}//flushmainMemory

//trigger the sequence of events that occur during a page fault
void pageFault(int pageRequested){
   //1.	check if there is a free frame
	int idx = checkForFreeFrame();
	if(idx < 0){
		//1.a) evict someone using the replacement algorithms; check if the page is dirty
      if  (processWorkingSets[line.processId].availWorkingSet ==
         processWorkingSets[line.processId].curWorkingSet) {
         if(v)
            puts("OWN PAGE EVICTION");
         idx = evictPage();
      } else {
         if(v)
            puts("PAGE EVICTION");
		   idx = evictPage();

      }
	} 
   //2.) bring the page into the page table
	updateMainMemory(&mainMemory[idx], pageRequested);
}//pageFault

//bring the page into the page table for future use
void updateMainMemory(struct frame *thisFrame, int pageRequested){
   // WSaddition
   processWorkingSets[line.processId].curWorkingSet++;

   //bring the page into main memory
   doOp(0, -1, DISKtime, -1);
   thisFrame->vAddress = pageRequested;
   thisFrame->dirtyBit = 0;
   thisFrame->referenceBit= 0;
   thisFrame->processId = line.processId;
}//updatemainMemory

void initWorkingSet(struct workingSets *thisWorkingSet){
   int i;
   thisWorkingSet-> availWorkingSet = INITWS;
   thisWorkingSet-> curWorkingSet = 0;
   thisWorkingSet-> pageFaultCursor = 0;
   for (i= 0; i< WSW; i++ )
      thisWorkingSet-> pageFaults[i] = 0;
}//initWorkingSet

int markWSPage(struct workingSets *thisWorkingSet, int fault){
   thisWorkingSet->pageFaults[thisWorkingSet->pageFaultCursor] = fault;
   thisWorkingSet->pageFaultCursor++;
   thisWorkingSet->pageFaultCursor = thisWorkingSet->pageFaultCursor%WSW;

   return calcWSPageFaults(thisWorkingSet);
}//markPageFault

int calcWSPageFaults(struct workingSets *thisWorkingSet){
   int numPageFaults = 0;
   int i;
   for (i=0; i<WSW; i++)
      if(thisWorkingSet->pageFaults[i]==1) numPageFaults++;

   return numPageFaults;
}//calcWSPageFaults