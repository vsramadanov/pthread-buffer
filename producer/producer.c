#include<stdlib.h>
#include<stdio.h>

#include "producer.h"

task_t
make_task(producer_args_t * args)
{
	static _Atomic int task_id = 0;

	task_t task;
	task.id = task_id;
	task.producer = args->id;
	task.value = rand() % 
	        (args->max_task_time - args->min_task_time) + 
	        args->min_task_time;

	task_id++;

	return task;
}

void*
workerProducer(void* producer_args)
{
	producer_args_t* args = (producer_args_t*)producer_args;
	printf("start producer thread with %3d id\n", args->id);

	while(1)
	{
		int sleep_time = rand() % 
		    (args->max_generation_time - args->min_generation_time) +
		    args->min_generation_time;
		    
		usleep(sleep_time);
		
  		printf("producer %3d start waiting on the semaphore\n", args->id);
        sem_wait(&args->task_dst->sem_write);
  		printf("producer %3d run\n", args->id);
  		
	    pthread_mutex_lock(&args->task_dst->mut);
		
		int ind = get_write_index(args->task_dst);
		args->task_dst->tasks[ind] = make_task(args);

    	pthread_mutex_unlock(&args->task_dst->mut);
		
		
		sem_post(&args->task_dst->sem_read);
	}

	pthread_exit(NULL);
}

