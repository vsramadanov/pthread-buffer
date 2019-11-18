#ifndef __CIRCLE_BUFFER_H
#define __CIRCLE_BUFFER_H

#include<pthread.h>
#include<semaphore.h>

typedef struct
{
	int id;
	int producer;
	int value;	
} task_t;


typedef struct 
{
	task_t* tasks;
	int size;
	int write_index;
	int read_index;
	sem_t sem_read;
	sem_t sem_write;

	pthread_mutex_t mut;

} task_buffer_t;


int get_write_index(task_buffer_t* buf);
int get_read_index(task_buffer_t* buf);
int init_task_buffer(task_buffer_t* buffer, int size);
void release_task_buffer(task_buffer_t* buffer);

#endif
