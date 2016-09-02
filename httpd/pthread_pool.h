#ifndef PTHREAD_POOL_H_
#define PTHREAD_POOL_H_

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/*请求队列*/
typedef struct tpool_work{
	void*	(*routine)(void*);
	void	*arg;
	struct tpool_work *next;
}tpool_work_t;

/*线程管理器*/
typedef struct tpool{
	int 			shutdown;	     /*线程是否被销毁*/
	int 			max_thr_num;     /*最大线程数目*/
	pthread_t 		*thr_id;		 /*线程id数组*/
	tpool_work_t 	*queue_head;     /*线程链表*/
	pthread_mutex_t queue_lock;
	pthread_cond_t  queue_ready;
}tpool_t;


int tpool_create(int max_thr_num);

void tpool_destroy();

int tpool_add_work(void *(*routine)(void*),void *arg);

#endif //PTHREAD_POOL_H_
