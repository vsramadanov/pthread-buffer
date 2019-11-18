#ifndef __CONSUMER_H
#define __CONSUMER_H

#include<circle_buffer/circle_buffer.h>

typedef struct
{
	int id;
	task_buffer_t* task_src;
} consumer_args_t;

void*
workerConsumer(void* consumer_args);

#endif
