#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include "circle_buffer.h"

int
get_write_index(task_buffer_t* buf)
{

	buf->write_index++;
	buf->write_index %= buf->size;

	return buf->write_index;
}

int
get_read_index(task_buffer_t* buf)
{
	
	buf->read_index++;
	buf->read_index %= buf->size;

	return buf->read_index;
}

int
init_task_buffer(task_buffer_t* buffer, int size)
{
	buffer->size = size;
	buffer->write_index = 0;
	buffer->read_index = 0;
	buffer->tasks = (task_t*)malloc(sizeof(task_t) * size);
	if(sem_init(&buffer->sem_write, 0, size) != 0)
	{
		fprintf(stderr, "unable to init write buffer semaphore: %s", strerror(errno));
		return -1;
	}

	if(sem_init(&buffer->sem_read, 0, 0) != 0)
	{
		fprintf(stderr, "unable to init read buffer semaphore: %s", strerror(errno));		
		return -1;
	}
	
	if(sem_init(&buffer->sem_write, 0, buffer->size) != 0)
	{
		fprintf(stderr, "unable to init write buffer semaphore: %s", strerror(errno));		
		return -1;
	}
	
	if(pthread_mutex_init(&buffer->mut, NULL) != 0)
	{
		fprintf(stderr, "unable to init mutex: %s", strerror(errno));		
		return -1;
	}

	return 0;
}

void
release_task_buffer(task_buffer_t* buffer)
{
	free(buffer->tasks);
}

