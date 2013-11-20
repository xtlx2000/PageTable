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
//#include "modules.h" //hey bud! I moved a bunch of the defines and
                     //  extra functionality functions in here
                     //I figured they might get pretty lengthy
//#include "modules.c"

#define BITS 32
#define ADDRESSPACE 4294967296
#define PAGESIZE 4096
#define PTES ADDRESSPACE/PAGESIZE
#define MAXTLB PTES/2
#define MAXTIME 9999999

FILE *fp; //input file pointer

// global param variables and defaults
int maxPages = 500;
int maxTLB = 10;
int MMtime = 2;
int TLBtime = 1;
int DISKtime = 5;
int pageReplAlgo = 1;
int pageTableType = 1;

int v=0; // Verbosity

void getParams( int argc, char* argv[]){

   if ( argc >= 2 ) { // flags or params are set
      int i;
      for (i=1 ; i<argc ; i++) {
         // User specified file 
         if (strncmp(argv[i],"file=", 5)==0 ) {
            fp = fopen(argv[i]+5, "r");
            if (fp == NULL) {
               printf("Can't open input file: %s\n", argv[i]+5);
               printf("Loading default file. \n");
            }
            else {
               printf("File option engaged: %s\n", argv[i]+5);
            }
         // Verbose option 
         } else if (strcmp(argv[i],"-verbose")==0 || strcmp(argv[i],"-v")==0) {
            printf("Verbose option engaged.\n");
            v = 1;
         // Very verbose option 
         } else if (strcmp(argv[i],"-veryverbose")==0 || strcmp(argv[i],"-vv")==0) {
            printf("Very Verbose option engaged.\n");
            v = 2;
         // Number of pages specified
         } else if (strncmp(argv[i],"Pages=", 6)==0 ) {
            int value;
            value = atoi(argv[i]+6);
            if ( value<1 || value>PTES ) {
               printf("usage: Pages={1-%d}\n", (int)PTES);
            } else {
               maxPages = value;
               printf("Number of pages set to %d.\n", maxPages);
            }
         // Max Number of TLB page table entries specified
         } else if (strncmp(argv[i],"TLB=", 4)==0 ) {
            int value;
            value = atoi(argv[i]+4);
            if ( value<1 || value>MAXTLB ) {
               printf("usage: TLB={1-%d}\n", (int)MAXTLB);
            } else {
               maxTLB = value; 
               printf("Max number of TLB entries set to %d.\n", maxTLB);
            }
         // time needed to read and write main memory specified
         } else if (strncmp(argv[i],"MMtime=", 7)==0 ) {
            int value;
            value = atoi(argv[i]+7);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: MMtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               MMtime = value;
               printf("Time needed to read/write a main memory");
               printf("page set to %d (nsec).\n", value);
            }
            // time needed to read/write a page table entry in the TLB specified 
         } else if (strncmp(argv[i],"TLBtime=", 8)==0 ) {
            int value;
            value = atoi(argv[i]+8);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: TLBtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               TLBtime = value;
               printf("Time needed to read/write a page table entry");
               printf(" in the TLB set to %d (nsec).\n", TLBtime);
            }
            // time needed to read/write a page to/from disk specified
         } else if (strncmp(argv[i],"DISKtime=", 9)==0 ) {
            int value;
            value = atoi(argv[i]+9);
            if ( value<1 || value>MAXTIME ) {
               printf("usage: DISKtime={1-%d} (nsec)\n", MAXTIME);
            } else {
               DISKtime = value;
               printf("Time needed to read/write a page to/from disk");
               printf(" set to %d (nsec).\n", DISKtime);
            }
         // Page replacement algorithm specified
         } else if (strncmp(argv[i],"PR=",3)==0) {
            int value;
            value = atoi(argv[i]+3);
            if ( value<1 || value>3 ) {
               printf("usage: PageRep={1=FIFO, 2=LRU, 3=MFU}\n");
            } else {
               pageReplAlgo = value;
               printf("Page Replacement set to %d.\n", pageReplAlgo);
            }
         // Page table type specified
         } else if (strncmp(argv[i],"PT=",3)==0) {
            int value;
            value = atoi(argv[i]+3);
            if ( value<1 || value>3 ) {
               printf("usage: PT={1=Single level, 2=Directory, 3=Inverted}\n");
            } else {
               pageTableType = value;
               printf("Page Table type set to %d.\n", pageTableType);
            }
         // incorrect flag
         } else {
               printf( "usage: %s -verbose || -v  ||\n" , argv[0]);
               printf("\t   -veryverbose || -vv ||\n");
               printf("\t      -showcmds || -s  ||\n");
               printf("\tpages={number of pages available}\n");
               printf("\tTLB={number of PTE's available to TLB}\n");
               printf("\tMMtime={time needed to read/write a main memory page}\n");
               printf("\tTLBtime={time needed to read/write a page table entry in the TLB}\n");
               printf("\tDISKtime={time needed to read/write a page to/from disk}\n");
               printf("\tPR={1=FIFO, 2=LRU, 3=MFU}\n");
               printf("\tPT={1=Single level, 2=Directory, 3=Inverted}\n\n\n");

         }
      }
   }
}

int main ( int argc, char* argv[]) {

   

   int PID; // to be placed by line struct
   char RW;
   uint addr;

   getParams(argc, argv);
   
   if ( fp == NULL) {
      fp = fopen("trace.out", "r");
         printf("Default file loaded: trace.out\n");
         if (fp == NULL) {
            printf("Can't open default input file: trace.out");
         }

   }
   // main code goes here
   while ( fscanf(fp, "%d %c %x", &PID, &RW, &addr) != EOF ) {
      //printf("%d %c 0x%x\n", PID, RW, addr);
   }

   // end main code

   if ( fp != NULL) { fclose(fp);printf("File closed.\n");}

 
}