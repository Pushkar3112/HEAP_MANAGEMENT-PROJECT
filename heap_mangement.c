#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 200

int mem[MEM_SIZE];  

typedef struct Block {
    int start;
    int size;
    struct Block* next;
} Block;

Block* freeList = NULL;
Block* allocList = NULL;

void init_mem() {
    freeList = (Block*)malloc(sizeof(Block));
    freeList->start = 0;
    freeList->size = MEM_SIZE;
    freeList->next = NULL;
    for (int i = 0; i < MEM_SIZE; i++) mem[i] = 0;
}

void show_mem() {
    printf("Memory: ");
    for (int i = 0; i < MEM_SIZE; i++) printf("%d ", mem[i]);
    printf("\nFree List: ");
    Block* temp = freeList;
    while (temp) {
        printf("[Start: %d, Size: %d] ", temp->start, temp->size);
        temp = temp->next;
    }
    printf("\nAllocated List: ");
    temp = allocList;
    while (temp) {
        printf("[Start: %d, Size: %d] ", temp->start, temp->size);
        temp = temp->next;
    }
    printf("\n");
}

void insert_sorted(Block** head, Block* newBlock) {
    if (!*head || newBlock->start < (*head)->start) {
        newBlock->next = *head;
        *head = newBlock;
        return;
    }
    Block* cur = *head;
    while (cur->next && cur->next->start < newBlock->start)
        cur = cur->next;
    newBlock->next = cur->next;
    cur->next = newBlock;
}

void merge_free() {
    Block* cur = freeList;
    while (cur && cur->next) {
        if (cur->start + cur->size == cur->next->start) {
            cur->size += cur->next->size;
            Block* temp = cur->next;
            cur->next = cur->next->next;
            free(temp);
        } else {
            cur = cur->next;
        }
    }
}

void compact_memory() {
    int writeIndex = 0;
    Block* cur = allocList;
    while (cur) {
        for (int i = 0; i < cur->size; i++) {
            mem[writeIndex + i] = 1;
        }
        cur->start = writeIndex;
        writeIndex += cur->size;
        cur = cur->next;
    }
    for (int i = writeIndex; i < MEM_SIZE; i++) {
        mem[i] = 0;
    }
    if (freeList) free(freeList);
    freeList = (Block*)malloc(sizeof(Block));
    freeList->start = writeIndex;
    freeList->size = MEM_SIZE - writeIndex;
    freeList->next = NULL;
    printf("Memory compacted.\n");
}
int allocate(int size) {
    Block* best = NULL;
    Block* bestPrev = NULL;
    Block* prev = NULL;
    Block* cur = freeList;
    
    while (cur) {
        if (cur->size >= size && (!best || cur->size < best->size)) {
            best = cur;
            bestPrev = prev;
        }
        prev = cur;
        cur = cur->next;
    }

  
    if (!best) {
        printf("Not enough memory. Attempting to compact...\n");
        compact_memory();
        
        // Retry allocation
        best = NULL;
        bestPrev = NULL;
        prev = NULL;
        cur = freeList;
        
        while (cur) {
            if (cur->size >= size && (!best || cur->size < best->size)) {
                best = cur;
                bestPrev = prev;
            }
            prev = cur;
            cur = cur->next;
        }

        if (!best) {
            printf("Allocation failed even after compaction.\n");
            return -1;
        }
    }

    int start = best->start;
    for (int i = start; i < start + size; i++) mem[i] = 1;
    
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->start = start;
    newBlock->size = size;
    newBlock->next = allocList;
    allocList = newBlock;

    best->start += size;
    best->size -= size;
    if (best->size == 0) {
        if (bestPrev) bestPrev->next = best->next;
        else freeList = best->next;
        free(best);
    }

    printf("Allocated %d units at %d\n", size, start);
    return start;
}
 

void free_mem(int start) {
    Block* prev = NULL;
    Block* cur = allocList;

    while (cur) {
        if (cur->start == start) {
            for (int i = start; i < start + cur->size; i++) mem[i] = 0;
            if (prev) prev->next = cur->next;
            else allocList = cur->next;
            Block* newFree = (Block*)malloc(sizeof(Block));
            newFree->start = start;
            newFree->size = cur->size;
            insert_sorted(&freeList, newFree);
            free(cur);
            merge_free();
            printf("Freed memory at %d\n", start);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("Free failed: Invalid start address\n");
}

int main() {
    init_mem();
    int choice, size, addr;
    
    while (1) {
        printf("\n1. Allocate\n2. Free\n3. Display Memory\n4. Compact Memory\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter size to allocate: ");
                scanf("%d", &size);
                allocate(size);
                break;
            case 2:
                printf("Enter start address to free: ");
                scanf("%d", &addr);
                free_mem(addr);
                break;
            case 3:
                show_mem();
                break;
            case 4:
                compact_memory();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
 