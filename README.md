# Memory

A program that implements several contiguous memory allocation algorithms in C.

### Description

This program implements the First-Fit, Best-Fit, and Worst-Fit algorithms of contiguous memory allocation. It manages allocations within a memory pool of size MEMSIZE and can support six different requests:

1. Allocate N bytes for a process using one of the 3 allocation algorithms
2. Free all allocations for a given process
3. Show the status of the memory pool – allocated and free blocks
4. Read a script – a sequence of commands from a file, and execute them
5. Compact the allocations, making them into one contiguous block.  (This somewhat resembles the operation of a mark-sweep garbage collector in C#)
6. Exit 

---

Credits:<br>
Starter code provided by Yusuf Pisan<br>
Implementation by Dhimitri Mano
