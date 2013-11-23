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
      redo = 0;
      int pageRequested = grabPTE(line.currentAddress);
      if(pageRequested != -1){
         //check if it was in the TLB
         if(!checkTLB(pageRequested)){
            //check if it was in the page table
            if(!checkPageTable(pageRequested)){
               pageFault(pageRequested);
               redo = 1;
            }//checkPageTabl
         }//checkTLB*/
      } else {
         printf("ERROR: You are bad at this.\n");
         segFault();
      }
      calcAverage();
   }//while

   printf("%s %f\n", "Average Access Time: ", program.runningAverage);

   //close the file
   if ( fp != NULL) { fclose(fp);printf("File closed.\n\n");}
}//main