#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>

// Static variables to store allocated memory and GList pointers
static GHashTable *allocated_memory_set = NULL;
static GHashTable *glist_allocated_memory_set = NULL;

// Initialize the memory manager
void initialize_memory_manager() {
    allocated_memory_set = g_hash_table_new(NULL, NULL);
    glist_allocated_memory_set = g_hash_table_new(NULL, NULL);
}

// Allocate memory using calloc and add it to memory manager
void *allocate_memory_with_calloc(size_t nmemb, size_t size) {
    void *address = calloc(nmemb, size);
    if (address != NULL) {
        g_hash_table_insert(allocated_memory_set, address, NULL);
    }
    return address;
}

// Allocate memory using malloc and add it to memory manager
void *allocate_memory_with_malloc(size_t size) {
    void *address = malloc(size);
    if (address != NULL) {
        g_hash_table_insert(allocated_memory_set, address, NULL);
    }
    return address;
}

// Check if the given address is a heap allocation managed by the memory manager
bool is_managed_memory(void *address) {
    return (allocated_memory_set != NULL && g_hash_table_contains(allocated_memory_set, address));
}

// Deallocate memory and remove it from the memory manager
void deallocate_memory(void *address) {
    if (address != NULL && g_hash_table_remove(allocated_memory_set, address)) {
        free(address);
    }
}

// Add a GList to the memory manager
void add_glist_to_memory_manager(GList *head) {
    if (head != NULL) {
        g_hash_table_insert(glist_allocated_memory_set, head, NULL);
    }
}

// Remove a GList from the memory manager and free its memory
void deallocate_glist(GList *head) {
    if (head != NULL && g_hash_table_remove(glist_allocated_memory_set, head)) {
        g_list_free(head);
    }
}

// Deallocate all memory managed by the memory manager, including GLists
void deallocate_all_memory() {
    if (allocated_memory_set == NULL || glist_allocated_memory_set == NULL) {
        fprintf(stderr, "Error: Hash table is NULL\n");
        return;
    }

    // Free memory associated with keys in allocated_memory_set
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, allocated_memory_set);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        free(key);
    }

    // Free memory associated with keys (GLists) in glist_allocated_memory_set
    g_hash_table_iter_init(&iter, glist_allocated_memory_set);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        g_list_free((GList *) key);
    }
}

// Destroy the memory manager and release all associated resources
void destroy_memory_manager() {
    deallocate_all_memory();
    g_hash_table_destroy(allocated_memory_set);
    g_hash_table_destroy(glist_allocated_memory_set);
    allocated_memory_set = NULL;
    glist_allocated_memory_set = NULL;
}
