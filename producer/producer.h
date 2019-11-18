#ifndef __PRODUCER_H
#define __PRODUCER_H

#include<unistd.h>

#include <circle_buffer/circle_buffer.h>


typedef struct
{
	int id;
	useconds_t sleep_time;
	int max_task_time;
	int min_task_time;
	int max_generation_time;
	int min_generation_time;
	task_buffer_t* task_dst;
} producer_args_t;


task_t make_task(producer_args_t* args);
void* workerProducer(void* producer_args);

#endif
