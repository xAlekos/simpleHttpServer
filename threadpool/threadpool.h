#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../utils/utils.h"
#include <pthread.h>
#include <semaphore.h>

#include "threadpool.h"

#define TASK_QUEUE_SIZE 32
#define WORKER_THREADS_NUM 6


typedef void* (*task_function_t)(void*);

typedef struct task{

task_function_t task_function;
void* args;

}task_t;

typedef struct task_queue {
    task_t* tasks;
    int front;
    int back;
}task_queue;


typedef struct thread_pool{

    pthread_t workers[WORKER_THREADS_NUM];
    bool working;
    sem_t is_there_work_sem;
    pthread_mutex_t mutex;
    task_queue* task_queue;

}thread_pool;

task_queue* task_queue_init();

int task_queue_enqueue(task_queue* task_queue, task_t task);

task_t* task_queue_dequeue(task_queue* task_queue);

void task_queue_free(task_queue* task_queue);

void* worker_job(void* arg);

int add_work(void* (*task_function)(void*),void* args,thread_pool* workers);

thread_pool* thread_pool_init();

void thread_pool_shutdown(thread_pool* pool);


#endif