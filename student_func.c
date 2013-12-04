/*
student_func.c

Used to place all student functionality that needs to be added to this module.
Each of the eviction policies contains a switch statement governing which of the
eviction policies to use. Under each one, there needs to be the correct code to 
correctly evict the corresponding section. 

FIFO is given below as an example. 
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
      case 2: // MFU
      	//TODO: WRITE THE MFU EVICTION POLICY
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
		case 2: // MFU
	      	//TODO: WRITE THE MFU EVICTION POLICY
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
		case 2: // MFU
			//TODO: WRITE THE MFU EVICTION POLICY
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
		case 2: // MFU
			//TODO: WRITE THE MFU EVICTION POLICY
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
