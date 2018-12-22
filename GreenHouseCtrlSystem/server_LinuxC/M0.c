/*==============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   文件名称：M0.c
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
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "M0.h"
extern pthread_rwlock_t mutex_RM0;
extern pthread_rwlock_t mutex_WM0;
struct _M0 M0;
//extern int connM0.fd_M0;
extern pthread_mutex_t mutex_SRV;
extern int sync_flag;
int set_M0opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)   
{   
    struct termios newtio,oldtio;   
    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/   
    if  ( tcgetattr(fd,&oldtio)  !=  0) {    
        perror("SetupSerial 1");  
        printf("tcgetattr( M0.fd,&oldtio) -> %d\n",tcgetattr(fd,&oldtio));   
        return -1;   
    }   
    bzero( &newtio, sizeof( newtio ) );   
    /*步骤一，设置字符大小*/   
    newtio.c_cflag  |=  CLOCAL | CREAD;    
    newtio.c_cflag &= ~CSIZE;    
    /*设置停止位*/   
    switch( nBits )   
    {   
        case 7:   
        newtio.c_cflag |= CS7;   
        break;   
        case 8:   
        newtio.c_cflag |= CS8;   
        break;   
    }   
    /*设置奇偶校验位*/   
    switch( nEvent )   
    {   
        case 'o':  
        case 'O': //奇数   
        newtio.c_cflag |= PARENB;   
        newtio.c_cflag |= PARODD;   
        newtio.c_iflag |= (INPCK | ISTRIP);   
        break;   
        case 'e':  
        case 'E': //偶数   
        newtio.c_iflag |= (INPCK | ISTRIP);   
        newtio.c_cflag |= PARENB;   
        newtio.c_cflag &= ~PARODD;   
        break;  
        case 'n':  
        case 'N':  //无奇偶校验位   
        newtio.c_cflag &= ~PARENB;   
        break;  
        default:  
        break;  
    }   
    /*设置波特率*/   
    switch( nSpeed )   
    {   
        case 2400:   
        cfsetispeed(&newtio, B2400);   
        cfsetospeed(&newtio, B2400);   
        break;   
        case 4800:   
        cfsetispeed(&newtio, B4800);   
        cfsetospeed(&newtio, B4800);   
        break;   
        case 9600:   
        cfsetispeed(&newtio, B9600);   
        cfsetospeed(&newtio, B9600);   
        break;   
        case 115200:   
        cfsetispeed(&newtio, B115200);   
        cfsetospeed(&newtio, B115200);   
        break;   
        case 460800:   
        cfsetispeed(&newtio, B460800);   
        cfsetospeed(&newtio, B460800);   
        break;   
        default:   
        cfsetispeed(&newtio, B9600);   
        cfsetospeed(&newtio, B9600);   
        break;   
    }   
    /*设置停止位*/   
    if( nStop == 1 ) 
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;   
        /*设置等待时间和最小接收字符*/   
        newtio.c_cc[VTIME]  = 0;   
        newtio.c_cc[VMIN] = 0;   
        /*处理未接收字符*/   
        tcflush(M0.fd,TCIFLUSH);   
        /*激活新配置*/   
    }
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)   
    {   
        perror("com set error");   
        return -1;   
    }   
    printf("set done!\n");   
    return 0;
}

int open_M0usb(void)
{
    int fd;
    int ft, is_tty;
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    printf("M0.fd = %d\n", fd);
    
    if (fd == -1)
    {
        printf("open usb error\n");
        return -1;
    }
    else
    {
        printf("open ttyUSB0 successful\n");
    }
    ft = fcntl(fd, F_SETFL, 0);
    if (-1 == ft)
    {
        printf("fcntl fail\n");
    }
    else
    {
        printf("fcntl = %d\n",ft); 
    }
    is_tty = isatty(STDIN_FILENO);
    if (0 == is_tty)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty successful\n");
    }
    printf("fd = %d\n", fd);
    tcflush(fd, TCIFLUSH);
    tcflush(fd, TCOFLUSH);
    return fd;
}
void * client_M0(void *arg)	//connM0.fd;
{
//	pthread_mutex_lock(&mutex_M0);	

	pthread_mutex_lock(&mutex_SRV);
	sync_flag--;
	pthread_mutex_unlock(&mutex_SRV);
	int ret;
	int num;
	int i;
	int index_num;
	int count = 0;
	//int connM0.fd = *(int *)arg;
	//unsigned char M0.read_buff[36] = {0};
	// struct zgb_buf_t env_data;


	M0.fd = open_M0usb();
	if (-1 == M0.fd)
	{
		perror("163: open fail\n");
		return  NULL;
	}
	num = set_M0opt(M0.fd, 115200, 8, 'N', 1);
	if (num == -1)
	{
		perror("set_opt error");
		return NULL;
	}
	//memset(M0.write_buff,0,sizeof(M0.write_buff));//clear
	M0.wflag = 0;
	M0.rflag = 0;
#if 1
	while(1)
	{
		//memset(M0.read_buff,0,sizeof(M0.read_buff));//clear
		//pthread_rwlock_rdlock(&mutex_RM0);
		//memset(M0.write_buff,0,sizeof(M0.write_buff));
		//printf("%d----\n",__LINE__);
		//ret = write(M0.fd, (char *)M0.write_buff, sizeof(M0.write_buff));
#if 1		
		if(!M0.rflag)
		{
			
        	//pthread_rwlock_lock(&mutex_RM0);	
		    //pthread_rwlock_rdlock(&mutex_RM0);
			printf("test L213 bb\n");
			while((ret = read(M0.fd, M0.read_buff, 36)) > 0)
			{
				printf("test L216 bb\n");
				if(0xbb != M0.read_buff[0])
				{
					continue;
				}
				//    printf("count = %3d, read_num = %4d\n",count, read_num);
				for (i = 0; i < ret; i++)
				{
					printf("%02x ", M0.read_buff[i] & 0xff);
					fflush(stdout);
				}
				printf("\n");
				sleep(1);
				M0.rflag = 1;
			}
		}
#endif
		//pthread_rwlock_unlock(&mutex_RM0);
		printf("%d----\n",__LINE__);
		sleep(1);
#if 1

		if(M0.wflag)
		{
			int i;
			for (i = 0; i < 36; i++)
			{
				printf("%02x ", M0.write_buff[i] & 0xff);
				fflush(stdout);
			}
			printf("\n");
			ret = write(M0.fd, (char *)M0.write_buff, sizeof(M0.write_buff));
			printf("write ret = %d\n",ret);
			while (ret < 0)
			{
				perror("write");
				break;
			}
			fflush(stdout);
			printf("%d----\n",__LINE__);
			M0.wflag = 0;
		}
#endif
	}

// 用于清空输入/输出缓冲区

	//pthread_mutex_unlock(&mutex_M0);
	tcflush(M0.fd, TCIFLUSH);
	tcflush(M0.fd, TCOFLUSH);
	close(M0.fd);
	pthread_exit(NULL);
	//pthread_mutex_lock(&mutex_M0);
#endif
}

