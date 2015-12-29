#include "cs537.h"
#include "request.h"

//globals
#define MAX 100000		/* Numbers of buffers */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, full = PTHREAD_COND_INITIALIZER;
int no_buffer = 1;
int count = 0;
int fill = 0;
int use = 0;
int buffer[MAX];
int port; 
int listenfd;

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

typedef struct {
	int id;
} t_id;

//Error
void error(char *error_message)
{
	fprintf(stderr, "%s\n", error_message);
}

//Usage 
void usage(char *argv[])
{
	fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
	exit(1);
}

// CS537: Parse the new arguments too
void getargs(int *port, int *no_threads, int *no_buffer, int argc, char *argv[])
{
    if(argc != 4) 
    {
		usage(argv);
	}

    *port = atoi(argv[1]);

    *no_threads = atoi(argv[2]);
    if(*no_threads < 1)
    {
    	usage(argv);
    }

    *no_buffer = atoi(argv[3]);
    if(*no_buffer < 1)
    {
    	usage(argv);
    }

}

void put(int connfd) 
{
	buffer[fill] = connfd;
	fill = (fill + 1) % no_buffer;
	count++;
	printf("master: buffer is updated. connfd: %d buffer count: %d\n", connfd, count);

}

int get() 
{
	int connfd = buffer[use];

	use = (use + 1) % no_buffer;
	count--;
	printf("worker: buffer is consumed. connfd: %d buffer count: %d\n", connfd, count);	
	return connfd;

}


void* master_routine(void) 
{
	struct sockaddr_in clientaddr;
	int connfd = 0;
	int clientlen = 0 ;//sizeof(clientaddr);

	//for (i = 0; i < no_buffer; i++)
	for(;;) 
	{
		clientlen = sizeof(clientaddr);
		printf("master: waiting to accept connection request from clients..\n");
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		printf("master: connection request accepted. connfd: %d\n", connfd);

		printf("master: trying to lock mutex\n");
		pthread_mutex_lock(&mutex);
		printf("master: holds mutex\n");
		while (count == no_buffer) //buffer is full
		{
			printf("master: buffer is full so cond_waits empty unlocking mutex\n");
			pthread_cond_wait(&empty, &mutex); 
		}
		printf("master: buffer is not full\n");
		put(connfd);
		pthread_cond_signal(&full);
		printf("master: cond_signal full to workers\n");
		pthread_mutex_unlock(&mutex);
		printf("master: unlocks mutex\n");
	}
}

void* worker_routine(void *arg) 
{
	int connfd = 0; 
  	t_id *worker_t_id = (t_id *)arg;
	//for (i = 0; i < no_buffer; i++) 
	for(;;)
	{
		printf("worker[%d]: trying to lock mutex\n",worker_t_id->id);
		pthread_mutex_lock(&mutex);
		printf("worker[%d]: holds mutex\n",worker_t_id->id);
		while (count == 0) //buffer is empty
		{
			printf("worker[%d]: count = 0 so cond_waits full unlocking mutex\n",worker_t_id->id);
			pthread_cond_wait(&full, &mutex);
		}
		printf("worker[%d]: buffer is not empty\n",worker_t_id->id);
		connfd = get();
		pthread_cond_signal(&empty);
		printf("worker[%d]: cond_signal empty to master\n",worker_t_id->id);
		pthread_mutex_unlock(&mutex);
		printf("worker[%d]: unlocks mutex\n",worker_t_id->id);

		//handling requests
		printf("worker: handling request. connfd: %d\n", connfd);
		requestHandle(connfd);
		printf("worker: connection request is handled\n");
		Close(connfd);
		printf("worker: connection request is closed\n");
	}
}


int main(int argc, char *argv[])
{
    int ret_val, i;
    int no_threads;
    pthread_t master_t, *worker_t;
	t_id *worker_t_id;

    getargs(&port, &no_threads, &no_buffer, argc, argv);
    printf("arguments parsed \nport: %d \nno of threads: %d \nno of buffers: %d\n",port, no_threads, no_buffer);

    listenfd = Open_listenfd(port);
    printf("starting server to listen client request \nlistenfd: %d\n",listenfd);

    //create master thread
    ret_val = pthread_create(&master_t, NULL, master_routine, NULL);

    if(ret_val < 0)
    {
    	error("thread creation error");
    }

    //create worker threads
    worker_t = (pthread_t *)malloc(no_threads*sizeof(*worker_t));
    worker_t_id = (t_id *)malloc(sizeof(t_id)*no_threads);

    for(i = 0; i < no_threads; i++)
    {
    	worker_t_id[i].id = i;
		ret_val = pthread_create(&worker_t[i], NULL, worker_routine, (void *)(worker_t_id+i));    

		if(ret_val < 0)
	    {
	    	error("thread creation error");
	    }	
    }


    pthread_join(master_t, NULL);
    printf("master is done\n");

    for (i=0; i<no_threads; i++)
	{
		pthread_join(worker_t[i],NULL);
	}
	
	
	printf("worker(s) is done\n");
	free(worker_t_id);
    

}


    


 
