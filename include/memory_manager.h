#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <glib.h>
#include <stdbool.h>

// Function to initialize the memory manager
void initialize_memory_manager();

// Function to allocate memory using calloc and add it to the memory manager
void *allocate_memory_with_calloc(size_t nmemb, size_t size);

// Function to allocate memory using malloc and add it to the memory manager
void *allocate_memory_with_malloc(size_t size);

// Function to check if the given address is managed by the memory manager
bool is_managed_memory(void *address);

// Function to deallocate memory and remove it from the memory manager
void deallocate_memory(void *address);

// Function to add a GList to the memory manager
void add_glist_to_memory_manager(GList *head);

// Function to remove a GList from the memory manager and free its memory
void deallocate_glist(GList *head);

// Function to deallocate all memory managed by the memory manager, including GLists
void deallocate_all_memory();

// Function to destroy the memory manager and release all associated resources
void destroy_memory_manager();

#endif // MEMORY_MANAGER_H
