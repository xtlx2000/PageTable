/*
Interface.c

All function used for grabbing parameters either from the parameters file, 
or for interacting with the input file given for the simulation
*/

//used to init all global variables from parameters and malloc space for the data structures
void initialization(void){
   //malloc space for our datastructures
   mainMemory     = malloc(sizeof(struct frame)*numFrames);
   program.runNumber = 0;

   //init our empty page table
   flushMainMemory();

   //init TLB
   flushCache();
   
   int idx;
   switch(pageTableType){
      case 1: //multi
         pageDirectory  = (struct pageTablePage*) malloc(sizeof(struct pageTablePage)*numPageTablePages);
         for(idx = 0; idx<numPageTablePages; idx++){
            struct pageTablePage thisPageTablePage;

            thisPageTablePage.idx            = -1;
            thisPageTablePage.startAddress   = idx*4096;

            pageDirectory[idx]               = thisPageTablePage;
         }
         break;
      case 2: //inverted
         hashTable      = (int*) malloc(sizeof(int)*modNum); 
         break;
     }

   // init working sets 
   for(idx = 0; idx< NUMPROCESSES; idx++ )
      initWorkingSet(&processWorkingSets[idx]);

   line.processId = -1;
   int i;
/*   for(i=0; i<numFrames; i++){
      printf("accessing mainMemory[%d]: %d\n", i, mainMemory[i]);
   }
   for(i=0; i<numPageTablePages; i++){
      printf("accessing pageDirectory[%d]: %d\n", i, pageDirectory[i]);
   }
   for(i=0; i<modNum; i++){
      printf("accessing hashTable[%d]: %d\n", i, hashTable[i]);
   }*/
}//initilization

//move the control flow back to the main loop and read the next line
//redo is used to control page faults when the come back to this step
   //0 - page fault (read same line)
   //1 - read next line
int readNextLine(int redo){
   //if there was a page fault before, don't update 
   //to the next line, just read it again
   int ret = 0;
   if(!redo){
      int PID; 
      char RW;
      uint addr;
      ret = fscanf(fp, "%d %c %x", &PID, &RW, &addr);
      grabNextLine(PID, RW, addr);
   }

   return ret; //EOF on fail
}//nextLine

//update the globall struct line, so that it contains the next line's attributes
void grabNextLine(int PID, char RW, uint addr){
   program.runNumber++;
   line.previousProcessId = line.processId;
   line.processId         = PID;
   line.currentAddress    = addr;
   if(RW == 'W'){
      line.currentOperation = 1;
   } else {
      line.currentOperation = 0;
   }

   //if there was a process switch, we need to flush the cache entries and flush all the pages from the frames
   //this is because our simulation does not distinguish between two virtual address from different processes
   if(line.previousProcessId != line.processId){
      flushCache();
      flushMainMemory();
   }
   return;
}//grabNextLine

//if you are using the command line to load, the parameters, this function will take care of that
void getParams( int argc, char* argv[]){
   if ( argc >= 2 ) { // flags or params are set
      int i;
      for (i=1 ; i<argc ; i++) {
         // User specified input file 
         if (strncmp(argv[i],"file=", 5)==0 ) {
            fp = fopen(argv[i]+5, "r");
            if (fp == NULL) {
               printf("Can't open input file: %s\n", argv[i]+5);
               printf("Loading default file. \n");
            }
            else {
               printf("File option engaged: %s\n", argv[i]+5);
            }
         // User specified param file 
         } else if (strncmp(argv[i],"paramFile=", 10)==0 ) {
            printf("Parameter file option engaged.\n");
            paramFileName = argv[i]+10;
            printf("Will attempt to read parameters from: %s.\n", paramFileName);
         // Verbose option 
         } else if (strcmp(argv[i],"-verbose")==0 || strcmp(argv[i],"-v")==0) {
            printf("Verbose option engaged.\n");
            v = 1;
         // Very verbose option 
         } else if (strcmp(argv[i],"-veryverbose")==0 || strcmp(argv[i],"-vv")==0) {
            printf("Very Verbose option engaged.\n");
            v = 2;
         // // Number of pages specified
         // } else if (strncmp(argv[i],"Pages=", 6)==0 ) {
         //    int value;
         //    value = atoi(argv[i]+6);
         //    if ( value<1 || value>PTES ) {
         //       printf("usage: Pages={1-%d}\n", (int)PTES);
         //    } else {
         //       maxPages = value;
         //       printf("Number of pages set to %d.\n", maxPages);
         //    }
         // // Max Number of TLB page table entries specified
         // } else if (strncmp(argv[i],"TLB=", 4)==0 ) {
         //    int value;
         //    value = atoi(argv[i]+4);
         //    if ( value<1 || value>MAXTLB ) {
         //       printf("usage: TLB={1-%d}\n", (int)MAXTLB);
         //    } else {
         //       TLBEntries = value; 
         //       printf("Max number of TLB entries set to %d.\n", TLBEntries);
         //    }
         // // time needed to read and write main memory specified
         // } else if (strncmp(argv[i],"MMtime=", 7)==0 ) {
         //    int value;
         //    value = atoi(argv[i]+7);
         //    if ( value<1 || value>MAXTIME ) {
         //       printf("usage: MMtime={1-%d} (nsec)\n", MAXTIME);
         //    } else {
         //       MMtime = value;
         //       printf("Time needed to read/write a main memory");
         //       printf("page set to %d (nsec).\n", value);
         //    }
         //    // time needed to read/write a page table entry in the TLB specified 
         // } else if (strncmp(argv[i],"TLBtime=", 8)==0 ) {
         //    int value;
         //    value = atoi(argv[i]+8);
         //    if ( value<1 || value>MAXTIME ) {
         //       printf("usage: TLBtime={1-%d} (nsec)\n", MAXTIME);
         //    } else {
         //       TLBtime = value;
         //       printf("Time needed to read/write a page table entry");
         //       printf(" in the TLB set to %d (nsec).\n", TLBtime);
         //    }
         //    // time needed to read/write a page to/from disk specified
         // } else if (strncmp(argv[i],"DISKtime=", 9)==0 ) {
         //    int value;
         //    value = atoi(argv[i]+9);
         //    if ( value<1 || value>MAXTIME ) {
         //       printf("usage: DISKtime={1-%d} (nsec)\n", MAXTIME);
         //    } else {
         //       DISKtime = value*1000;
         //       printf("Time needed to read/write a page to/from disk");
         //       printf(" set to %d (msec).\n", DISKtime);
         //    }
         // // Page replacement algorithm specified
         // } else if (strncmp(argv[i],"PR=",3)==0) {
         //    int value;
         //    value = atoi(argv[i]+3);
         //    if ( value<1 || value>3 ) {
         //       printf("usage: PageRep={1=FIFO, 2=LRU, 3=MFU}\n");
         //    } else {
         //       pageReplAlgo = value;
         //       printf("Page Replacement set to %d.\n", pageReplAlgo);
         //    }
         // // Page table type specified
         // } else if (strncmp(argv[i],"PT=",3)==0) {
         //    int value;
         //    value = atoi(argv[i]+3);
         //    if ( value<1 || value>3 ) {
         //       printf("usage: PT={1=Single level, 2=Directory, 3=Inverted}\n");
         //    } else {
         //       pageTableType = value;
         //       printf("Page Table type set to %d.\n", pageTableType);
         //    }
         // // Working Set Window specified
         // } else if (strncmp(argv[i],"WSW=",4)==0) {
         //    int value;
         //    value = atoi(argv[i]+4);
         //    if ( value<1 || value>MAXWSW ) {
         //       printf("usage: WSW={1-%d}\n", MAXWSW);
         //    } else {
         //       pageTableType = value;
         //       printf("Working Set Window set to %d.\n", pageTableType);
         //    }
         // incorrect flag
         } else {
            printf( "usage: %s -verbose || -v  ||\n" , argv[0]);
            printf("\t   -veryverbose || -vv ||\n");
            printf("\t      -showcmds || -s  ||\n");
            printf("\tfile={name of input file}\n");
            printf("\tparamFile={name of param file}\n");
            // printf("\tpages={number of pages available}\n");
            // printf("\tTLB={number of PTE's available to TLB}\n");
            // printf("\tMMtime={time needed to read/write a main memory page}\n");
            // printf("\tTLBtime={time needed to read/write a page table entry in the TLB}\n");
            // printf("\tDISKtime={time needed to read/write a page to/from disk}\n");
            // printf("\tPR={1=FIFO, 2=LRU, 3=MFU}\n");
            // printf("\tPT={1=Single level, 2=Directory, 3=Inverted}\n");
            // printf("\tWSW={number of instructions in the working set window}\n\n");
         }
      }
   }
}

//print the current status of the main memory, and the page table
void visual(void){
   if(v){
      int i;
      printf("%s %f %s\n", "Running average: ", program.runningAverage, "ns");
      printf("%s %d %s\n\n", "Running total: ", program.currentRunningSum, "ns");
      printf("***AFTER READING NEXT LINE***\nop: %d addr: 0x%X id: %d\n", line.currentOperation, line.currentAddress, line.processId);
      
      printf("mainMemory: [ ");
      for(i=0; i<numFrames; i++){
         if(mainMemory[i].vAddress != -1)
            printf("0x%X ", mainMemory[i].vAddress);
      }
      printf("]\n");

      printf("TLB: [ ");
      for(i=0; i<TLBEntries; i++){
         if(TLB[i].vAddress != -1)
            printf("0x%X ", TLB[i].vAddress);
      }

      printf("]\n");   
      if(pageTableType == 1){
         printf("pageDirectory: [");
         for(i=0; i<numPageTablePages; i++){
            if(pageDirectory[i].idx != -1)
              printf("0x%X ", pageDirectory[i].startAddress);

         }
         printf("]\n");
      }

      puts("**************************\n");
   }
}//visual

//load all the paramets from the parameters file
void loadParams(char *paramFileName){
   FILE *paramFP;

   paramFP = fopen(paramFileName, "r");
   printf("Parameter file loaded: %s\n", paramFileName);
   if (paramFP == NULL) {
      printf("Can't open parameter file: %s\n", paramFileName);
      paramFP = fopen("params.txt", "r");
      printf("Loading default parameter file: params.txt\n");
      if (paramFP == NULL) {
         printf("Can't open parameter file: params.txt\n");
         printf("Using default parameters.\n");
      }
   }

   char paramName[22];
   int paramNum;
   int cnt = 0;
   const char paramNameList[12][22] = {"maxPages", "TLBEntries", "MMtime", "TLBtime", "DISKtime",
      "pageReplAlgo", "cacheReplAlgo", "pageTablePageReplAlgo", "pageTableType",
      "WSW", "numFrames", "numPageTablePages"}; // TODO add collisionPercentage and modNum

   int paramNumList[12];

   while( fscanf(paramFP, "%s %d %*[^\n]", paramName, &paramNum) != EOF) {
      if (!strcmp(paramName, paramNameList[cnt])){
         printf("%21s:\t%d\n", paramName, paramNum);
         paramNumList[cnt]= paramNum;
      }
      cnt++;
   }
   if ( paramFP != NULL) { fclose(paramFP);printf("Parameter file closed.\n\n");}

      // max number of pages
      if ( paramNumList[0]<1 || paramNumList[0]>PTES ) {
         printf("usage: Pages={1-%d}\n", (int)PTES);
      } else {
         maxPages = paramNumList[0];
         printf("Number of pages set to %d.\n", maxPages);
      }
      // Max Number of TLB page table entries specified
      if ( paramNumList[1]<1 || paramNumList[1]>MAXTLB ) {
         printf("usage: TLB={1-%d}\n", (int)MAXTLB);
      } else {
         TLBEntries = paramNumList[1]; 
         printf("Max number of TLB entries set to %d.\n", TLBEntries);
      }
      // time needed to read and write main memory specified
      if ( paramNumList[2]<1 || paramNumList[2]>MAXTIME ) {
         printf("usage: MMtime={1-%d} (nsec)\n", MAXTIME);
      } else {
         MMtime = paramNumList[2];
         printf("Time needed to read/write a main memory");
         printf("page set to %d (nsec).\n", MMtime);
      }
      // time needed to read/write a page table entry in the TLB specified 
      if ( paramNumList[3]<1 || paramNumList[3]>MAXTIME ) {
         printf("usage: TLBtime={1-%d} (nsec)\n", MAXTIME);
      } else {
         TLBtime = paramNumList[3];
         printf("Time needed to read/write a page table entry");
         printf(" in the TLB set to %d (nsec).\n", TLBtime);
      }
      // time needed to read/write a page to/from disk specified
      if ( paramNumList[4]<1 || paramNumList[4]>MAXTIME ) {
         printf("usage: DISKtime={1-%d} (msec)\n", MAXTIME);
      } else {
         DISKtime = paramNumList[4]*1000;
         printf("Time needed to read/write a page to/from disk");
         printf(" set to %d (msec).\n", DISKtime/1000);
      }
      // Page replacement algorithm specified
      if ( paramNumList[5]<0 || paramNumList[5]>2 ) {
         printf("usage: PageRep={0=FIFO, 1=LRU, 2=MFU}\n");
      } else {
         pageReplAlgo = paramNumList[5];
         printf("Page Replacement set to %d.\n", pageReplAlgo);
      }
      // Cache replacement algorithm specified
      if ( paramNumList[6]<0 || paramNumList[6]>2 ) {
         printf("usage: CacheRepl={0=FIFO, 1=LRU, 2=MFU}\n");
      } else {
         cacheReplAlgo = paramNumList[6];
         printf("Cache Replacement set to %d.\n", pageReplAlgo);
      }
      // Page Table replacement algorithm specified
      if ( paramNumList[7]<0 || paramNumList[7]>2 ) {
         printf("usage: PageTablePageRepl={0=FIFO, 1=LRU, 2=MFU}\n");
      } else {
         pageTablePageReplAlgo = paramNumList[7];
         printf("Page Talbe Page Replacement set to %d.\n", pageReplAlgo);
      }
      // Page table type specified
      if ( paramNumList[8]<0 || paramNumList[8]>2 ) {
         printf("usage: PT={0=Single level, 1=Directory, 2=Inverted}\n");
      } else {
         pageTableType = paramNumList[8];
         printf("Page Table type set to %d.\n", pageTableType);
      }
      // Working Set Window specified
      if ( paramNumList[9]<1 || paramNumList[9]>MAXWSW ) {
         printf("usage: WSW={1-%d}\n", MAXWSW);
      } else {
         WSW = paramNumList[9];
         printf("Working Set Window set to %d.\n", WSW);
      }
      // Number of Frames specified
      if ( paramNumList[10]<1 || paramNumList[10]>PTES ) {
         printf("usage: numFrames={1-%d}\n", (int)PTES);
      } else {
         numFrames = paramNumList[10];
         printf("Number of frames set to %d.\n", numFrames);
      }
      // Number of Page Table Pages specified
      if ( paramNumList[11]<1 || paramNumList[11]>MAXPTP ) {
         printf("usage:numPageTablePages={1-%d}\n", MAXPTP);
      } else {
         numPageTablePages = paramNumList[11];
         printf("Number of Page Table Pages set to %d.\n", numPageTablePages);
      }
}//loadParams

//opens the input file
void openInputFile(void){
   if ( fp == NULL) {
      fp = fopen("trace.out", "r");
         printf("Default file loaded: trace.out\n");
         if (fp == NULL) {
            printf("Can't open default input file: trace.out");
         }
   }
}//openInputFile

//run the main loop
void run(void){
   if(v)
      printf("CURRENT LINE NUME: %d\n", program.runNumber);
   pageRequested = grabPTE(line.currentAddress);
   if(pageRequested != -1){
      //check if it was in the TLB
      int TLBCheck = checkTLB(pageRequested);
      if(!TLBCheck){
         //check if it was in the page table
         int pageTableCheck = checkMainMemory(pageRequested);
         if(pageTableCheck == -1){
            numPageFaults = markWSPage(&processWorkingSets[line.processId], 1);
            if(v){
               puts("PAGE FAULT");
               printf("Addding %d ns for a disk hit\n", DISKtime);
            }
            pageFault(pageRequested);
            redo = 1;
         } else {//checkPageTable
            if(!redo)
               numPageFaults = markWSPage(&processWorkingSets[line.processId], 0);
            if(v){
               puts("FOUND IT IN THE PAGE TABLE");
               printf("Adding %d ns for a memory hit\n", MMtime);
            }
            redo = 0;
         }
      } else {//checkTLB
         numPageFaults = markWSPage(&processWorkingSets[line.processId], 0);
         if(v){
            puts("FOUND IT IN THE CACHE");
            printf("Adding %d ns for a cache hit\n", TLBtime);
         }
      }

   } else 
      printf("ERROR: SEG FAULT\n");

   if(program.runNumber > WSW){
      if ( numPageFaults < MINPAGEFAULT ) {
         // remove allocated pages from the working set for that process
         processWorkingSets[line.processId].availWorkingSet--;
         int idx = evictPage();
         struct frame *thisFrame = &mainMemory[idx];
         thisFrame->vAddress     = -1;
         thisFrame->dirtyBit     = 0;
         thisFrame->referenceBit = 0;
         thisFrame->processId    = -1;
      } 

      if ( numPageFaults > MAXPAGEFAULT ) {
         // add free pages to the working set for that process
         processWorkingSets[line.processId].availWorkingSet++;
      }
   }
}//run