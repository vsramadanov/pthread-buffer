# pthread-buffer
This project implements a multithread circular buffer.
There is the pull of threads, which produce tasks and 
the pull of threads that consume tasks. Concurrency 
protected is implemented using one mutex and two semaphores


# building
This project can be built through Unix makefile by command:
```
$ make
```
The building directory is ./bin/ Building tree is separated 
from the sourse tree.
Also available commands:
```
$ make clean
```

# using
Run the command:
```
$ ./bin/runme
```
Number of consumer and production threads in the pull can be 
chosen with proper flags. Also buffer length, delay time for consumer
and producer can be configured too.
Usage: runme [OPTION]...
run the pull of threads, which are concurrent for the buffer
    -p      producer threads number
    -c      consumer threads number
    -L      the buffer length
    -r      minimum delay time for consumers
    -R      maximum delay time for consumers
    -W      maximum delay time for producers
    -w      minimum delay time for producers
