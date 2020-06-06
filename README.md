# userlevel_thread_library (Compile in VS2017 x86)
An implementation at a higher level of how threading works, with context switching thread yielding, thread waiting and even sleep functions provided. Something I did while learning C++ and how operating systems work. Also a popular computer science assignment. 

The code is never expected to change so I did not bother to try to organise it OOP style.

- ## threadlibrary.h
The file which you will do an #include in other applications in order to use its functions. The description for each function is in the comments. The implementation in threadlibrary.cpp contains inline assembly in order to do context switching. A linked list of Thread Control Blocks tracks thread statuses and heap addresses (used as a stack for the thread to run in). Thread Control Blocks, once created, never gets removed as I prefer my return value to always be available at all times, unlike POSIX's implementation. 

- ## main.cpp
An example of how to use the functions in threadlibrary.h. Also a preview of how the program works.

- ## accessory headers
  ### antideadlock.h
  I used a Graph data structure to check for deadlocks (caused by unwise use of the thread_wait function) by doing a cycle detection. In hindsight, a Linked List is perfectly fine but I will just leave this untouched. 
