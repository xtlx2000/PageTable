//***************************************************************
// Author: Jake Heath and Joe Lovelace
//
// Date created: 11/18/2013
//
// Last Modified: 11/20/2013
//
// Description: 
//    Simulation to be used as a demonstration to show how paging works. 
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
   printf("Starting getParams\n");
   getParams(argc, argv);
   printf("Starting loadParams\n");
   loadParams(paramFileName);

   //initialize all our arrays
   initialization();

   //attempt to open the file
   openInputFile();
   while (readNextLine(redo) != EOF) {
      run();      
      calcAverage();
      visual();
   }//while

   //print average access time
   printf("\n%s %f %s\n", "Average Access Time: ", program.runningAverage, "ns");
   printf("%s %d %s\n", "Total Access Time: ", program.currentRunningSum, "ns");

   //close the file
   if ( fp != NULL) { fclose(fp);printf("File closed.\n\n");}
}//main