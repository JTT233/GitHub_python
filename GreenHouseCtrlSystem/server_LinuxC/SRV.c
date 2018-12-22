/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   文件名称：SRV.c
*   创 建 者：yanweijun
*   创建日期：2018年06月09日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <error.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <unistd.h>
#include "GSM.h"
#include "M0.h"
extern struct sockaddr_in srvaddr;
extern struct sockaddr_in cltaddr;
extern socklen_t addrlen;
extern struct _M0 M0;
extern pthread_rwlock_t mutex_RM0;
extern pthread_rwlock_t mutex_WM0;
extern pthread_rwlock_t mutex_GSM;
extern struct _GSM GSM;
void *thread_srv(void *arg)
{

	int connfd = *(int *)arg;
	char buf[32];
	int ret;
	int count;
//loop:	
	//int connfd = accept(listenfd, (struct sockaddr *)&cltaddr, &addrlen);
	printf("SRV----pthread!\n");
	read(connfd,buf,sizeof(buf));
#if 1
	if(!strncmp(buf,"GSM",3))
	{
		printf("GSMshoudao\n");
		sleep(1);
		while(1) 
		{
			//pthread_rwlock_rdlock(&mutex_GSM);
            //printf("test L58 GSM.buf\n");
			if(GSM.flag)
			{
				//memset(GSM.buf,0,sizeof(GSM.buf));
				ret = read(connfd,GSM.buf,sizeof(GSM.buf));
                //printf("test L58 GSM.buf\n");
				if (ret<0)
				{
					perror("read:");
					return NULL;
				}
				int i;
				for(i = 0;i < 13;i++)
				{
					printf("%c ",GSM.buf[i]);
				}
				printf("\n");
				GSM.flag = 0;
			}
			//pthread_rwlock_unlock(&mutex_GSM);
		}
	}
                sleep(2);
#endif
#if 1
	if(!strncmp(buf,"RM0",3))
	{
		printf("RM0shoudaoRM0\n");
		while(1) 
		{
			//pthread_rwlock_rdlock(&mutex_WM0);
			pthread_rwlock_wrlock(&mutex_RM0);
			if(M0.rflag)
			{
			ret = write(connfd, M0.read_buff, sizeof(M0.read_buff));
			M0.rflag= 0;
			if (ret < 0)
			{
				perror("write");
				break;
			}
			pthread_rwlock_unlock(&mutex_RM0);
			}
		}
	}
#endif
#if 1
	if(!strncmp(buf,"WM0",3))
	{
		printf("WM0shoudaoWM0\n");
		while(1) 
		{
			pthread_rwlock_rdlock(&mutex_WM0);
			if(!M0.wflag)
			{
				//memset(M0.write_buff,0,sizeof(M0.write_buff));
				sleep(1);
				ret = read(connfd, M0.write_buff, sizeof(M0.write_buff));
				M0.wflag = 1;
				if (ret < 0)
				{
					perror("write");
					break;
				}
			}
			pthread_rwlock_unlock(&mutex_WM0);
		}
	}
#endif
	printf("server thread exited\n");
	pthread_exit(NULL);
}
