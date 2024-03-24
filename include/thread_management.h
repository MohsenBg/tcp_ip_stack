#ifndef TCPIP_THREAD_MANAGEMENT_H
#define TCPIP_THREAD_MANAGEMENT_H

#include <pthread.h>

#define MAX_THREADS 100

void add_thread(pthread_t thread);

void cancel_all_threads();

void wait_for_all_threads();


#endif //TCPIP_THREAD_MANAGEMENT_H
