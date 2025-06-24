# HEAP_MANAGEMENT-PROJECT
This project implements a simple memory management system in C, simulating allocation, deallocation, fragmentation handling, and memory compaction. The memory is modeled as a fixed-size array of 200 units, where each unit represents a block of memory. The system maintains two linked lists:

Free List: Keeps track of available free blocks.

Allocated List: Tracks memory currently in use.

Features
Best-Fit Allocation: Finds the smallest free block that fits the requested size to minimize fragmentation.

Deallocation: Frees allocated blocks based on their start address.

Free List Merge: Adjacent free blocks are merged automatically after deallocation to reduce external fragmentation.

Memory Compaction: Moves allocated blocks towards the start of memory to eliminate gaps and create larger contiguous free blocks when allocation fails due to fragmentation.

Interactive Menu: Users can allocate, free, display memory status, compact memory, or exit through a simple console interface.

Usage
The program runs interactively, allowing users to test dynamic memory operations. It demonstrates core principles of memory management used in operating systems, including block management, fragmentation reduction, and compaction.

