PageTable
=========

This is a page table simulator. The project contains the following files:

Contents:
---------

--Makefile 		:: makes the program
--interface.c 	:: library of functions that controls the programs interface (main loop, print statements, ...)
--modules.c 	:: library of functions that controls the mechanics of the simulation (pageFault, updatePageTable, ...)
--pager.h 	 	:: header used to store all global parameters and to initialize all library functionss
--pager.c 		:: main function; contains the initialization and main loop contents
--params.txt 	:: file used to load in parameters; if no paramets are set, defaults are given (see below)
--student_func.c:: library that the student will need to fill out; it contains the sections where all the eviction policies need to go
--trace.out 	:: input file of reads and writes to memory


How to Use:
-----------

1.) set the params.txt file to the appropriate simulation you desire
2.) make
3.) ./pager -v file=filename.extension (see list of command line parameters)
4.) make clean

Params.txt:
-----------

maxPages				:: the maximum number of pages you would like your computer to have 				
TLBEntries 				:: the maximum number of TLB entries you can store in the cache at one time
MMtime 					:: the time it takes to access main memory (ns)
TLBtime					:: the time it takes to access cache (ns)
DISKtime				:: the time it takes to access disk (ns)
pageReplAlgo 			:: the eviction policy used to replace pages from main memory 
cacheReplAlgo			:: the eviction policy used to replace TLB entries from the cache
pageTablePageReplAlgo 	:: the eviction policy used to replace page table pages from main memory (multi-level pt only)
pageTableType			:: the type of page table you would like to simulate
WSW						:: the size of the working set window
numFrames				:: the maximum number of frames you would like to allow your computer to store at one time
numPageTablePages		:: the mamimum number of page table page you would like to allow in memory at one time (multi-level pt only)

Command Line:
-------------

-v 			: verbose; show all logs and simulation activity
file=		: pick a file to read as input (that is not trace.out)
paramFile=	: pick a file to read parameters (that is not params.txt)

Defaults:
---------

Without any command line parameters or changes to the params.txt file, the following defaults are chosen:
maxPages 				500
TLBEntries 				10
MMtime 					2 //ns
TLBtime					1 //ns
DISKtime				1 //ns
pageReplAlgo 			0 //FIFO
cacheReplAlgo			0 //FIFO
pageTablePageReplAlgo 	0 //FIFO
pageTableType			0 //single
WSW						5
numFrames				20
numPageTablePages		20

Overview:
---------

Since this is a simulation and will be used as a demonstration about how paging works, our page translations do not give us perfect indexes into our memory data structures. Instead, we wrote the program in a way that checks a set of conditions before allowing itself to access any type of memory. At each step, there are delays (set by the params.txt) that are added to a running sum to act as the memory access time. 

For example, let's say I have the following case:

mainMemory 	[0x0 0x1000 0x2000]
TLB 		[0x0 0x1000 0x2000]

with a single level page table. I, then, want to do the following line:

1 W 0x2c00

The program will follow the following procedure (which can be visually seen in the flowcharts directory, labeled pager.pdf)
1.) Find the associated page for this address --> grabPTE()
	1.a) grabPTE() has built in delays to given the appropriate amount of time for each type of page table (these can be changed with params.txt)
2.) Using this page, we can then check the TLB if the page address has been used recently
	2.a) this counts as a cache hit/miss and time will be added to the running sum
	2.b) if it's there, use it. Proceed to the next line
3.) If it's not in the cache, try to find it in main memory
	3.a) this counts as a memory hit/miss and time will be added to the running sum
	3.b) if its there, use it. Proceed to the next line
4.) If it's not in the cache or in main memory, we have a page fault
	4.a) first, look to see if there is a free frame. If there is, use it to add your requested page to main memory (this counts as a disk hit/miss and time will be added)
	4.b) if there are no free frames, you must evict one using the page replacement algorithm
		4.b.i) if the frame you chose to evict is dirty, you must write it back to disk, which counts as a disk hit/miss and time will be added
	4.c) then you have to update your memory with your new requested page. This counts as a memory hit/miss and time will be added. 
	4.d) in the case of a page fault, we want to reread the line so that our operation can perform correctly with its page in memory


After every line read, the program will calculate its new average memory access time and record it. In the case above, the page starting at 0x2000 will be chosen. Since, that page is already in the cache, it will add a cache hit/miss time to the running sum and proceed to the next line.