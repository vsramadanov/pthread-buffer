#include<stdio.h>
#include<unistd.h>

#include"consumer.h"

void*
workerConsumer(void* consumer_args)
{
	consumer_args_t* args = (consumer_args_t*)consumer_args;
	printf("start consumer thread with %d id\n", args->id);

	while(1)
	{
  		printf("consumer %3d is waiting on the semaphore\n", args->id);
	    sem_wait(&args->task_src->sem_read);
   		printf("consumer %3d run\n", args->id);
	    
	    pthread_mutex_lock(&args->task_src->mut);
	    
		int ind = get_read_index(args->task_src);
		task_t task = args->task_src->tasks[ind];
		
        pthread_mutex_unlock(&args->task_src->mut);
		
		printf("consumer %3d: read_index: %3d\n", args->id, ind);
		printf("consumer %3d: take task with id:%5d and value:%3d by %3d producer\n", 
			args->id,
			task.id,
			task.value,
			task.producer);
        
 	    sem_post(&args->task_src->sem_write);       

		usleep(args->task_src->tasks[ind].value * 1000);
	}
	pthread_exit(NULL);
}

