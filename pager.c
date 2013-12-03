//***************************************************************
// Author: Jake Heath and Joe Lovelace
//
// Date created: 11/18/2013
//
// Last Modified: 11/20/2013
//
// Description: 
//
// Some interesting notes:
//    
//
// Acknowledgements: 
//***************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "modules.c"

int main ( int argc, char* argv[]) {
   //used to tell our main loop if we have page faulted;
   //if we have reread the line and do not go to the next line
   int redo = 0;
   int numPageFaults = 0;

   getParams(argc, argv);
   initialization();

   //attempt to open the file
   if ( fp == NULL) {
      fp = fopen("tmp2.txt", "r");
         printf("Default file loaded: trace.out\n");
         if (fp == NULL) {
            printf("Can't open default input file: trace.out");
         }
   }

   // main code goes here
   while (readNextLine(redo) != EOF) {
      puts("\n**************************");
      if(redo)
         printf("READING LINE %d AGAIN \n", program.runNumber);
      else 
         printf("READING LINE %d\n", program.runNumber);

      
      int pageRequested = grabPTE(line.currentAddress);
      if(pageRequested != -1){
         //check if it was in the TLB
         int TLBCheck = checkTLB(pageRequested);
         if(!TLBCheck){
            //check if it was in the page table
            int pageTableCheck = checkPageTable(pageRequested);
            if(pageTableCheck == -1){
               numPageFaults = markWSPage(&processWorkingSets[line.processId], 1);
               puts("PAGE FAULT");
               printf("Addding %d ns for a disk hit\n", DISKtime);
               pageFault(pageRequested);
               redo = 1;
            } else {//checkPageTable
               if(!redo)
                  numPageFaults = markWSPage(&processWorkingSets[line.processId], 0);

               puts("FOUND IT IN THE PAGE TABLE");
               printf("Adding %d ns for a memory hit\n", MMtime);
               redo = 0;
            }
         } else {//checkTLB*/
            numPageFaults = markWSPage(&processWorkingSets[line.processId], 0);
            puts("FOUND IT IN THE CACHE");
            printf("Adding %d ns for a cache hit\n", TLBtime);
         }

      } else 
         printf("ERROR: SEG FAULT\n");

      printf("numPageFaults: %d, \n", numPageFaults);
      if(program.runNumber > WSW){
         if ( numPageFaults < MINPAGEFAULT ) {
            // remove allocated pages from the working set for that process
            processWorkingSets[line.processId].availWorkingSet--;
            int idx = evictOwnPage();
            printf("idx from evict own page %d\n", idx);
            struct frame *thisFrame = &pageTable[idx];
            thisFrame->vAddress = -1;
            thisFrame->dirtyBit = 0;
            thisFrame->referenceBit= 0;
            thisFrame->processId = -1;
         } 

         if ( numPageFaults > MAXPAGEFAULT ) {
            // add free pages to the working set for that process
            processWorkingSets[line.processId].availWorkingSet++;
         }
      }
      
      calcAverage();
      printf("%s %f %s\n", "Running average: ", program.runningAverage, "ns");
      printf("%s %d %s\n\n", "Running total: ", program.currentRunningSum, "ns");
      visual();
      puts("**************************");
   }//while


   printf("\n%s %f %s\n", "Average Access Time: ", program.runningAverage, "ns");
   printf("%s %d %s\n", "Total Access Time: ", program.currentRunningSum, "ns");

   //close the file
   if ( fp != NULL) { fclose(fp);printf("File closed.\n\n");}

   loadParams();

}//main