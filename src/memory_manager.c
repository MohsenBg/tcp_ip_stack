#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>

static GHashTable *allocated_memory_set = NULL;

void initialize_memory_manager() {
    allocated_memory_set = g_hash_table_new(NULL, NULL);
}

// Allocate memory using calloc and add it to the managed memory set
void *allocate_memory_with_calloc(size_t nmemb, size_t size) {
    void *address = calloc(nmemb, size);
    if (address != NULL) {
        add_allocation(address);
    }
    return address;
}

// Allocate memory using malloc and add it to the managed memory set
void *allocate_memory_with_malloc(size_t size) {
    void *address = malloc(size);
    if (address != NULL) {
        add_allocation(address);
    }
    return address;
}

// Check if the given address is a heap allocation managed by the memory manager
bool is_managed_memory(void *address) {
    return (allocated_memory_set != NULL && g_hash_table_contains(allocated_memory_set, address));
}

// Add an allocation to the managed memory set
void add_allocation(void *address) {
    if (address == NULL) {
        fprintf(stderr, "Error: Address cannot be NULL\n");
        return;
    }

    if (is_managed_memory(address)) {
        fprintf(stderr, "Error: Address is already allocated\n");
        return;
    }

    g_hash_table_insert(allocated_memory_set, address, NULL);
}

// Deallocate memory and remove it from the managed memory set
void deallocate_memory(void *address) {
    if (address != NULL) {
        g_hash_table_remove(allocated_memory_set, address);
        free(address);
    }
}

// Deallocate all memory managed by the memory manager
void deallocate_all_memory() {
    if (allocated_memory_set == NULL) {
        fprintf(stderr, "Error: Hash table is NULL\n");
        return;
    }

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, allocated_memory_set);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        free(key);
    }

    g_hash_table_destroy(allocated_memory_set);
    allocated_memory_set = NULL;
}
