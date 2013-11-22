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
   int redo = 0;

   getParams(argc, argv);
   initialization();

   //attempt to open the file
   if ( fp == NULL) {
      fp = fopen("trace.out", "r");
         printf("Default file loaded: trace.out\n");
         if (fp == NULL) {
            printf("Can't open default input file: trace.out");
         }
   }

   // main code goes here
   while (readNextLine(redo) != EOF) {
      redo = 0;
      struct PTE *thisPTE = grabPTE(line.currentAddress);
      struct TLBEntry *thisTLB;

      //1) check if it was in the TLB
      if(!checkTLB(thisPTE, thisTLB)){
         //1.a) check if it was in the page table
         if(!checkPageTable(thisPTE)){
            pageFault(thisPTE);
            redo = 1;
         }//checkPageTabl
      }//checkTLB
   }//while

   //close the file
   if ( fp != NULL) { fclose(fp);printf("File closed.\n\n");}
}//main