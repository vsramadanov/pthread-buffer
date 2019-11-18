#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include<circle_buffer/circle_buffer.h>
#include<producer/producer.h>
#include<consumer/consumer.h>

struct globalArgs_t {
    int num_consumers;
    int num_producers;
    int min_task_time;
    int max_task_time;
    int min_generation_time;
    int max_generation_time;
    int buffer_size;
} globalArgs;

static const char *optString = "p:c:w:W:r:R:L:h?";

static const struct option longOpts[] = {
	{ "producer-num", required_argument, NULL, 'p' },
	{ "consumer-num", required_argument, NULL, 'c' },
	{ "consumer-min-delay", required_argument, NULL, 'r' },
	{ "consumer-max-delay", required_argument, NULL, 'R' },
	{ "producer-min-delay", required_argument, NULL, 'w' },
	{ "producer-max-delay", required_argument, NULL, 'W' },
	{ "buffer-length", required_argument, NULL, 'L' },
	{ NULL, no_argument, NULL, 0 }
};

void display_usage(void)
{
	puts("\
Usage: runme [OPTION]...\n\
run the pull of threads, which are concurrent for the buffer \n\
    -p      producer threads number\n\
    -c      consumer threads number\n\
    -L      the buffer length\n\
    -r      minimum delay time for consumers\n\
    -R      maximum delay time for consumers\n\
    -W      maximum delay time for producers\n\
    -w      minimum delay time for producers\n"
);
	exit(-1);
}


int 
main( int argc, char *argv[] )
{
	int opt = 0;
	int longIndex = 0;


    /* DEFAULT OPTIONS VALUE */
    	
    globalArgs.num_consumers = 5;
    globalArgs.num_producers = 5;
    globalArgs.min_generation_time = 50;
    globalArgs.max_generation_time = 100;
    globalArgs.min_task_time = 50;
    globalArgs.max_task_time = 100;
    globalArgs.buffer_size = 100;
    

    /* READ OPTIONS VALUE */

	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	while(opt != -1) {
		switch(opt) {
			case 'p':
				globalArgs.num_producers = atoi(optarg);
				break;
				
			case 'c':
				globalArgs.num_consumers = atoi(optarg);
				break;
				
			case 'w':
				globalArgs.min_generation_time = atoi(optarg);
				break;
				
			case 'W':
				globalArgs.max_generation_time = atoi(optarg);
				break;
			
			case 'r':
				globalArgs.min_task_time = atoi(optarg);
				break;
			
			case 'R':
				globalArgs.max_task_time = atoi(optarg);
				break;
			
			case 'L':
				globalArgs.buffer_size = atoi(optarg);
				break;
			
			case 'h':	/* fall-through is intentional */
			case '?':
				display_usage();
				break;

			case 0:		/* long option without a short arg */
                /* TODO */
                break;
			default:
				break;
		}
		
		opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
	}
	
    
    task_buffer_t task_buffer;
	init_task_buffer(&task_buffer, globalArgs.buffer_size);
    
    
    /* CREATE CONSUMER THREADS */
    
	pthread_t consumer_thread[globalArgs.num_consumers];
	consumer_args_t consumer_args[globalArgs.num_consumers];
	for (int i = 0; i < globalArgs.num_consumers; i++)
	{
		consumer_args[i].id = i;
		consumer_args[i].task_src = &task_buffer;

		pthread_create(consumer_thread + i, NULL, 
		                workerConsumer, consumer_args + i);
	}


    /* CREATE PRODUCER THREADS */

	pthread_t producer_thread[globalArgs.num_producers];
	producer_args_t producer_args[globalArgs.num_producers];
	for (int i = 0; i < globalArgs.num_producers; i++)
	{
	    producer_args[i].id = i,
	    producer_args[i].min_task_time = globalArgs.min_task_time;
        producer_args[i].max_task_time = globalArgs.max_task_time;
        producer_args[i].min_generation_time = globalArgs.min_generation_time;
        producer_args[i].max_generation_time = globalArgs.max_generation_time;
		producer_args[i].task_dst = &task_buffer;
	    
	    pthread_create(producer_thread + i, NULL, 
	                    workerProducer, producer_args + i);
	}
	

    /* JOIN PRODUCER THREADS */
    
	for(int i = 0; i < globalArgs.num_producers; i++)
	{	
	    void* producer_result;
	    pthread_join(producer_thread[i], &producer_result);
    }

    /* JOIN CONSUMER THREADS */	
	
	for(int i = 0; i < globalArgs.num_consumers; i++)
	{
		void* consumer_result;
		pthread_join(consumer_thread[i], &consumer_result);
	}
	
	release_task_buffer(&task_buffer);

	
	return 0;
}
