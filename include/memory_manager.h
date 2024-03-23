// memory_manager.h
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

#define MAX_ALLOCATIONS 1000

void add_allocation(void *address);

void add_allocation_with_duplicate_check(void *address);

void deallocate_all_memory();

void cancel_all_threads();


#endif // MEMORY_MANAGER_H
