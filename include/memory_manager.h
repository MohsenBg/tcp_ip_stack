// memory_manager.h
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdbool.h>

// Function to initialize the memory manager
void initialize_memory_manager();

// Function to allocate memory using calloc and add it to the managed memory set
void *allocate_memory_with_calloc(size_t nmemb, size_t size);

// Function to allocate memory using malloc and add it to the managed memory set
void *allocate_memory_with_malloc(size_t size);

// Function to check if the given address is a heap allocation managed by the memory manager
bool is_managed_memory(void *address);

// Function to add an allocation to the managed memory set
void add_allocation(void *address);

// Function to deallocate memory and remove it from the managed memory set
void deallocate_memory(void *address);

// Function to deallocate all memory managed by the memory manager
void deallocate_all_memory();

#endif // MEMORY_MANAGER_H
