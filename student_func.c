/*
student_func.c

Used to place all student functionality that needs to be added to this module.
Each of the eviction policies contains a switch statement governing which of the
eviction policies to use. Under each one, there needs to be the correct code to 
correctly evict the corresponding section. 

FIFO is given below as an example. 

Variables you should be concerned with:
---------------------------------------
/////WHICH TYPE OF PAGE REPLACEMENT ALGORITHM YOU WANT TO USE/////
	0 - FIFO; 1 - LRU; 2 - MRU
int pageReplAlgo;			

/////WHICH TYPE OF CACHE REPLACEMENT ALGORITHM YOU WANT TO USE/////
	0 - FIFO; 1 - LRU; 2 - MRU
int cacheReplAlgo;

/////WHICH TYPE OF PAGE TABLE PAGE REPLACEMENT ALGORITHM YOU WANT TO USE///// 
	0 - FIFO; 1 - LRU; 2 - MRU
int pageTablePageReplAlgo;

/////WHICH TYPE OF PAGE TABLE YOU WANT TO USE/////
	0 - SINGLE; 1 - DOUBLE; 2 - INVERTED
int pageTableType;

/////VARIABLES USED FOR KEEPING TRACK OF FIFO ://///
	notice in the example that the fifo indices are always changed to the next index, so that
	at all times, we know which page to evict next. The following variables should behave in the
	same manner for LRU and MRU given the rules that you provide in the eviction policies below

/////FIFO INDEX VARIABLE/////
int FIFOindex_page;
int FIFOindex_page_table_page;
int FIFOindex_cache;

/////LRU INDEX VARIABLES/////
int LRUindex_page;
int LRUindex_page_table_page;
int LRUindex_cache;

/////MRU INDEX VARIABLES/////
int MRUindex_page;
int MRUindex_page_table_page;
int MRUindex_cache;

/////STRUCTS YOU SHOULD BE EDUCATED ON/////
//represents frames in memory; if vAddress is -1, then the frame is empty
struct frame{
	int vAddress;			//the virtual address that goes with this page
	int pAddress;			//the physical address that goes with this page
	int dirtyBit;			//is the frame dirty?
	int referenceBit;		//has the frame been referenced?
	int processId;			//id for the process owning this page
	int historyBit;			//used for certain eviction policies
	int referenceBit;		//used for certain eviction policies
};//frame

//represents a section of our total page table in the case you want to use a multi-level page table
struct pageTablePage{
	int idx;				//which page table partition is it?
	int startAddress;		//address that this page table page starts at
	int dirtyBit;			//is this page table page dirty?
	int historyBit;			//used for certain eviction policies
	int referenceBit;		//used for certain eviction policies
};//pageTablePage

//used to represent a TLB entry; if vAddress is -1, then the entry is empty
struct TLBEntry{
	int vAddress;			//used to find if the address from the line is in the TLB
	int pAddress;			//the translated virtual address
	int frameNum;			//page table index of this page
	int dirtyBit;			//is this cache entry dirty?
	int historyBit;			//used for certain eviction policies
	int referenceBit;		//used for certain eviction policies
};//TLBEntry
*/

////////////////////////////
//////EVICTION POLICIES/////
////////////////////////////

//when your cache is full, find one of the TLB entries you need to evict
int evictTLB(void){
   int retVal;
   struct TLBEntry evictedTLBEntry;
   switch(cacheReplAlgo){
      case 0: //FIFO
         evictedTLBEntry = TLB[FIFOindex_cache];
         retVal = FIFOindex_cache;

         //increment to the next element
         FIFOindex_cache = (FIFOindex_cache + 1) % TLBEntries;
         break;
      case 1: // LRU
      	//TODO: WRITE THH LRU EVICTION POLICY
        //NOTE: DON't FORGET TO CHECK THE REFERENCE BIT
         break;
      case 2: // MRU
      	//TODO: WRITE THE MRU EVICTION POLICY
	  	//DON'T FORGET TO CHECK THE HISTORY BIT
	    break;
      default:
         retVal = -1;
   }

	if(evictedTLBEntry.dirtyBit){
		if(v){
			puts("DIRTY CACHE BIT\n");
			printf("Adding %d ns for a disk hit\n", DISKtime);
		}
		struct frame thisFrame;
		thisFrame.vAddress = evictedTLBEntry.vAddress;
		thisFrame.pAddress = evictedTLBEntry.pAddress;
		doOp(1, -1, DISKtime, -1);
	}
   
   //return idx to evicted frame
   return retVal;
}//evictTLB

//when all of your main memory is full of pages, find one of the frames you can evict a page from
int evictPage(void){
   int retVal;
   struct frame evictedFrame;
   switch(pageReplAlgo){
		case 0: //FIFO
			evictedFrame = mainMemory[FIFOindex_page];
         	retVal = FIFOindex_page;
       		FIFOindex_page = (FIFOindex_page + 1) % numFrames;
			break;
		case 1: // LRU
     	 	//TODO: WRITE THE LRU EVICTION POLICY
     	   //NOTE: DON't FORGET TO CHECK THE REFERENCE BIT
			break;
		case 2: // MRU
	      	//TODO: WRITE THE MRU EVICTION POLICY
		  	//DON'T FORGET TO CHECK THE HISTORY BIT
		    break;
     	default:
        	retVal = -1;
	}

	if(evictedFrame.dirtyBit){
		if(v){
			puts("DIRTY BIT");
			printf("Adding %d ns for a disk hit\n", DISKtime);
		}
		doOp(1, -1, DISKtime, -1);
	}

	//return idx to evicted frame
	return retVal;
}//evictPage

//when all of your memory is full of page table pages, find one of the page table pages you can evict
int evictPageTablePage(void){
   struct pageTablePage evictedPageTablePage;
   int retVal;

    switch(pageTablePageReplAlgo){
		case 0: //FIFO
			evictedPageTablePage = pageDirectory[FIFOindex_page_table_page];
		 	retVal = FIFOindex_page_table_page;

		 	//increment to the next element
		 	FIFOindex_page_table_page = (FIFOindex_page_table_page + 1) % numPageTablePages;
		 	break;
		case 1: // LRU
			//TODO: WRITE THE LRU EVICTION POLICY
			//NOTE: DON't FORGET TO CHECK THE REFERENCE BIT
			break;
		case 2: // MRU
			//TODO: WRITE THE MRU EVICTION POLICY
			//DON'T FORGET TO CHECK THE HISTORY BIT
			break;
    	default:
        	retVal = -1;
   	}

	if(evictedPageTablePage.dirtyBit){
		if(v){
			puts("DIRTY BIT\n");
			printf("Adding %d ns for a disk hit\n", DISKtime);
		}	
      	doOp(1, -1, DISKtime, -1);
   	}  
   return retVal; //indicates the index of the pageDirector to evict 
}//evictPageTablePage

//not sure if we need this still
int evictOwnPage(void){
   int retVal;
   int index;
   struct frame evictedFrame;
	switch(pageReplAlgo){
		case 0: //FIFO
			index = FIFOindex_page;
			while (mainMemory[index].processId > -1) {
				if ( mainMemory[index].processId == line.processId ) {
					evictedFrame = mainMemory[index];
					retVal = index;
					FIFOindex_page = FIFOindex_page + 1 % numFrames;
					break;
				}
				index = (index + 1) % numFrames;
			}
			break;
		case 1: // LRU
			//TODO: WRITE THE LRU EVICTION POLICY
			//NOTE: DON't FORGET TO CHECK THE REFERENCE BIT
			break;
		case 2: // MRU
			//TODO: WRITE THE MRU EVICTION POLICY
			//DON'T FORGET TO CHECK THE HISTORY BIT
			break;
	}

	if(evictedFrame.dirtyBit){
		if(v){
			puts("DIRTY BIT");
			printf("Adding %d ns for a disk hit\n", DISKtime);
		}
		doOp(1, -1, DISKtime, -1);
	}
   return retVal;
}//evictOwnPage
