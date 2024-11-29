#include "threadpool.h"

task_queue* task_queue_init() {
    task_queue* new_task_queue = malloc(sizeof(task_queue));
    if (!new_task_queue) {
        perror("[ERROR] Task task_queue allocation failed");
        return NULL;
    }
    task_t* tasks = malloc(sizeof(task_t) * TASK_QUEUE_SIZE);
    new_task_queue->front = -1;
    new_task_queue->back = 0;
    new_task_queue->tasks = tasks;

    return new_task_queue;
}

int task_queue_enqueue(task_queue* task_queue, task_t task) {
    if ((task_queue->back + 1) % TASK_QUEUE_SIZE == task_queue->front) {
        perror("[INFO] coda task piena");
        return 1;
    }
    if (task_queue->front == -1) 
        task_queue->front = 0; 

    task_queue->tasks[task_queue->back] = task;
    task_queue->back = (task_queue->back + 1) % TASK_QUEUE_SIZE;

    return 0;
}

task_t* task_queue_dequeue(task_queue* task_queue){

    if (task_queue->front == -1 || task_queue->front == task_queue->back) {
        return NULL;
    }

    task_t* task = &task_queue->tasks[task_queue->front];
    if (task_queue->front == task_queue->back - 1) {  
        task_queue->front = -1;
        task_queue->back = 0;
    } else {
        task_queue->front = (task_queue->front + 1) % TASK_QUEUE_SIZE;
    }

    return task;
}

void task_queue_free(task_queue* task_queue) {
    free(task_queue->tasks);
    free(task_queue);

}

void* worker_job(void* arg){

   thread_pool* pool = (thread_pool*)arg;

    while(pool->working){

        sem_wait(&pool->is_there_work_sem);

        pthread_mutex_lock(&pool->mutex);

        task_t* task = task_queue_dequeue(pool->task_queue);

        pthread_mutex_unlock(&pool->mutex);

        if(task != NULL && task->task_function != NULL){
            task->task_function(task->args);
        }


   }

   return NULL;

}


int add_work(void* (*task_function)(void*),void* args, thread_pool* workers){
    
    
    if ((workers->task_queue->back + 1) % TASK_QUEUE_SIZE == workers->task_queue->front) { //Check if task_queue is full
        return 1; 
    }

    task_t task;
    task.task_function = task_function;
    task.args = args;

    sem_post(&workers->is_there_work_sem);

    task_queue_enqueue(workers->task_queue,task);

    return 0;
}

thread_pool* thread_pool_init(){

    thread_pool* new_thread_pool = malloc(sizeof(thread_pool));
    new_thread_pool -> working = true;
    sem_init(&new_thread_pool->is_there_work_sem,0,0);
    pthread_mutex_init(&new_thread_pool->mutex,NULL);
    new_thread_pool->task_queue = task_queue_init();

    for(int i = 0; i<WORKER_THREADS_NUM; i++){
        pthread_create(&new_thread_pool->workers[i],NULL, worker_job,(void*)new_thread_pool);
    }

    return new_thread_pool;

}

void thread_pool_shutdown(thread_pool* pool) {
    pool->working = false;
    
    for (int i = 0; i < WORKER_THREADS_NUM; i++) {
        sem_post(&pool->is_there_work_sem); 
    }

    for (int i = 0; i < WORKER_THREADS_NUM; i++) {
        pthread_join(pool->workers[i], NULL); 
    }

    sem_destroy(&pool->is_there_work_sem);
    pthread_mutex_destroy(&pool->mutex);
    task_queue_free(pool->task_queue);
    free(pool);
}