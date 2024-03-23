//
// Created by Bagheri on 3/23/24.
//
#include <pthread.h>
#include <stdio.h>
#include "thread_management.h"

static pthread_t threads[MAX_THREADS];
static int num_threads = 0;

void add_thread(pthread_t thread) {
    if (!thread) {
        perror("thread can not be null");
        return;
    }
    if (num_threads < MAX_THREADS) {
        threads[num_threads++] = thread;
    }
}

void cancel_all_threads() {
    for (int i = 0; i < num_threads; ++i) {
        pthread_cancel(threads[i]);
    }
}

void wait_for_all_threads() {
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}

