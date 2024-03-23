#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>

static void *allocated_memory[MAX_ALLOCATIONS];
static int num_allocations = 0;


void add_allocation(void *address) {
    if (address == NULL) {
        fprintf(stderr, "Error: Address cannot be NULL\n");
        return;
    }

    if (num_allocations >= MAX_ALLOCATIONS) {
        fprintf(stderr, "Error: Memory allocation limit reached\n");
        return;
    }

    allocated_memory[num_allocations++] = address;
}

int check_duplication(void *address) {
    if (!address) {
        return -1;
    }

    for (int i = 0; i < num_allocations; ++i) {
        if (allocated_memory[i] == address)
            return i;
    }

    return -1;
}

void add_allocation_with_duplicate_check(void *address) {
    if (check_duplication(address) >= 0) {
        return; // Address already exists, no need to add it again
    }

    add_allocation(address);
}

void deallocate_all_memory() {
    for (int i = 0; i < num_allocations; ++i) {
        if (allocated_memory[i] != NULL) { // Check if memory block is allocated
            free(allocated_memory[i]);
            allocated_memory[i] = NULL;
        }
    }
    num_allocations = 0; // Reset the count of allocations
}
