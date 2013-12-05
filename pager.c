//***************************************************************
// Author: Jake Heath and Joe Lovelace
//
// Date created: 11/18/2013
//
// Last Modified: 12/4/2013
//
// Description: 
//    Simulation to be used as a demonstration to show how paging works. 
//    
//
// Acknowledgements: 
//***************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "modules.c"

int main ( int argc, char* argv[]) {
   getParams(argc, argv);
   if (paramFileName)
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
   printf("%s %f %s\n", "Total Access Time: ", (float) program.currentRunningSum/1000000000, "s");
   printf("%s %d\n", "Total Page Faults:", program.totalPageFaults);

   //close the file
   if ( fp != NULL) { fclose(fp);printf("File closed.\n\n");}
}//main