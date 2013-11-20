//***************************************************************
// Author: Jake Heath and Joe Lovelace
//
// Date created: 11/18/2013
//
// Last Modified: 11/18/2013
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
#include "modules.h" //hey bud! I moved a bunch of the defines and extra functionality functions in here
                     //I figured they might get pretty lengthy
#include "modules.c"


int v=0;
int main ( int argc, char* argv[]) {
   FILE *fp;
   fp = NULL;

   int PID;
   char RW;
   uint addr;

   if ( argc >= 2 ) { // flags are set
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
            v = v + 1;
         // Very verbose option 
         } else if (strcmp(argv[i],"-veryverbose")==0 || strcmp(argv[i],"-vv")==0) {
            printf("Very Verbose option engaged.\n");
            v = v + 2;
         // Page replacement algorithm specified 
         } else if (strncmp(argv[i],"PageRep=", 8)==0 ) {
            int value;
            value = atoi(argv[i]+8);
            if ( value<1 || value>3 ) {
               printf("usage: PageRep={1-3}\n");
            }
            printf("Page Replacement set to %d.\n", value);
         // Page replacement algorithm specified 
         } else if (strncmp(argv[i],"PR=",3)==0) {
            int value;
            value = atoi(argv[i]+8);
            if ( value<1 || value>3 ) {
               printf("usage: PageRep={1-3}\n");
            }
            printf("Page Replacement set to %s.\n", argv[i]+3);
         // incorrect flag
         } else {
               printf( "usage: %s -verbose || -v  ||\n" , argv[0]);
               printf("\t  -veryverbose || -vv ||\n");
               printf("\t     -showcmds || -s  ||\n");
         }
      }
   }
   if ( fp == NULL) {
      fp = fopen("trace.out", "r");
         printf("Default file loaded: trace.out\n");
         if (fp == NULL) {
            printf("Can't open default input file: trace.out");
         }

   }
   // main code goes here
   while ( fscanf(fp, "%d %c %x", &PID, &RW, &addr) != EOF ) {
      printf("%d %c 0x%x\n", PID, RW, addr);
   }

   // end main code

   if ( fp != NULL) { fclose(fp);printf("File closed.\n");}

 
}