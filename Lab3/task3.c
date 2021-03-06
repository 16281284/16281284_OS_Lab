#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t *empty=NULL;
sem_t *full=NULL;
char buffer[10];

void *worker1(void *arg)
{
	
	for(int i=0;i<10;i++)
	{
		sem_wait(empty);
		scanf("%c",&buffer[i]);
		sem_post(full);
		if(i==9)    i=-1;
	}	
	return NULL;
}
void *worker2(void *arg)
{
	for(int i=0;i<10;i++)
	{
		sem_wait(full);
		printf("%c \n",buffer[i]);
		sem_post(empty);
		sleep(1);
		if(i==9)    i=-1;
	}	
	return NULL;
}

int main(int argc,char *argv[])
{
	empty=sem_open("empty",O_CREAT,0666,10);//初始缓存空间为10
	full=sem_open("full",O_CREAT,0666,0);//初始没有字符
	
	pthread_t p1,p2;
	pthread_create(&p1,NULL,worker1,NULL);
	pthread_create(&p2,NULL,worker2,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	
	sem_close(empty);
	sem_close(full);
	sem_unlink("empty_");
	sem_unlink("full_");
	return 0;
}
