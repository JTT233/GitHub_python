    /*===============================================================
    *   Copyright (C) 2017 All rights reserved.
    *   
    *   文件名称：GSM.c
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
    #include "GSM.h"
    #define device_path "/dev/ttyUSB1"
    //struct _GSM GSM;
    extern pthread_rwlock_t mutex_GSM;
    extern pthread_mutex_t mutex_SRV;
    extern int sync_flag;
    struct _GSM GSM;
    #if 0
    struct message_info{
        char cnnu[16];      /*短信中心号码*/
        char phnu[16];      /*接受短信号码*/
        char message[128];  /*发送中文信息*/

    };
    struct pdu_info {
        char cnswap[32];
        char phswap[32];
    };
    #endif
    void serial_init(int fd)    /*初始化串口*/
    {
        struct termios options;
        tcgetattr(fd, &options);
        options.c_cflag |= ( CLOCAL | CREAD  );
        options.c_cflag &= ~CSIZE;
        options.c_cflag &= ~CRTSCTS;
        options.c_cflag |= CS8;
        options.c_cflag &= ~CSTOPB; 
        options.c_iflag |= IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
        tcsetattr(fd,TCSANOW,&options);

    }
    void swap(char number[],char swap[])
    {
        char ch1[] = "86";
        char tmp[16];
        int i;
        memset(swap,0,32);//清空
        memset(tmp,0,16);
        strcpy(swap,number);
        strcat(swap,"f");
        strcat(ch1,swap);
        strcpy(swap,ch1);
        for(i = 0;i <= strlen(swap) - 1;i += 2){
            tmp[i + 1] = swap[i];
            tmp[i] = swap[i + 1];

        }
        strcpy(swap,tmp);

    }
    int sendy(int fd,char *cmgf,char *cmgs,char *message)
    {
        int nread,nwrite;
        char buff[128];
        char reply[128];
        memset(buff,0,sizeof(buff));
        strcpy(buff,"AT\r");        /*\r 回车*/
        nwrite = write(fd,buff,strlen(buff));
        //printf("nwrite=%d,%s\n",nwrite,buff);
        memset(reply,0,sizeof(reply));
        sleep(1);
        nread = read(fd,reply,sizeof(reply));
        //printf("nread=%d,%s\n",nread,reply);
        memset(buff,0,sizeof(buff));
        strcpy(buff,"AT+CMGF=");
        strcat(buff,cmgf);
        strcat(buff,"\r");
        nwrite = write(fd,buff,strlen(buff));
        //printf("nwrite=%d,%s\n",nwrite,buff);
        memset(reply,0,sizeof(reply));
        sleep(1);
        nread = read(fd,reply,sizeof(reply));
        //printf("nread=%d,%s\n",nread,reply);
        memset(buff,0,sizeof(buff));
        strcpy(buff,"AT+CMGS=");
        strcat(buff,cmgs);
        strcat(buff,"\r");
        nwrite = write(fd,buff,strlen(buff));
        //printf("nwrite=%d,%s\n",nwrite,buff);
        memset(reply,0,sizeof(reply));
        sleep(1);
        nread = read(fd,reply,sizeof(reply));
        //printf("nread=%d,%s\n",nread,reply);
        memset(buff,0,sizeof(buff));
        strcpy(buff,message);
        nwrite = write(fd,buff,strlen(buff));
        //printf("nwrite=%d,%s\n",nwrite,buff);
        memset(reply,0,sizeof(reply));
        sleep(1);
        nread = read(fd,reply,sizeof(reply));
        //printf("nread=%d,%s\n",nread,reply);

    }
    int send_zh_message(int fd,struct message_info info,char pho_rcv_no[12],int i)
    {
        char cmgf[] = "0";
        char cmgs[4] = {'\0'};
        char ch2[] = "0891";
        char ch3[] = "1100";
        char ch4[] = "000800";
        char ch5[] = "0d91";
        char final[128];
        char *message[3] = 
        {
            "0a5BB691CC7740706BFF01",
            "0a5BB691CC67098D3CFF01",
            "1a751F524D4F555FC54E457761FF0C6B7B540E5FC54F1A957F7720"
        };
        struct pdu_info pdu;
        int conter = 0,flag,len;
        memset(final,0,80);

        strcpy(info.cnnu,"13800280500");
        strcpy(info.phnu,pho_rcv_no);
        flag = i;
        swap(info.phnu, pdu.phswap);
        swap(info.cnnu, pdu.cnswap);
        strcpy(final,ch2);
        strcat(final,pdu.cnswap);
        strcat(final,ch3);
        strcat(final,ch5);
        strcat(final,pdu.phswap);
        strcat(final,ch4);
        strcat(final,message[flag - 2]);
        strcat(final,"\x1a");

        len = strlen(ch3)+ strlen(ch4)+ strlen(ch5)+strlen(pdu.phswap)+ strlen(message[flag - 1]);
        sprintf(cmgs,"%d",len/2);
        //puts(final);
        sendy(fd,cmgf,cmgs,final);

    }

    void * client_GSM(void *arg)	//connfd;
    {
        pthread_mutex_lock(&mutex_SRV);
        sync_flag--;
        pthread_mutex_unlock(&mutex_SRV);

        char recv_num[12];
        strcpy(recv_num,"15608179669");
        int j = 2;
        struct message_info info;
        GSM.fd = open( "/dev/ttyUSB1", O_RDWR);
        if (GSM.fd==-1)
        {
            /* 不能打开串口一*/
            perror(" 提示错误！");
        }
        printf("open /dev/ttyUSB1 successful\n");
        GSM.flag = 0;
        while(1)
        {
            if(!GSM.flag)
            {
                printf("buf : %s \n", GSM.buf);
                int i;
                for (i = 0; i < 11; i++) //从0开始，第0-10位为中心号码
                {
                    recv_num[i] = GSM.buf[i];
                }
                printf("receive_phone_num = %s \n", recv_num);
                int k = ((int)GSM.buf[12] - '0');
                printf("k = %d\n", k);
                serial_init(GSM.fd);    /*1. 打开设备，初始化串口*/
                send_zh_message(GSM.fd,info,recv_num,j); /*2. 数据的发送和接受*/
                memset(GSM.buf, 0, sizeof(GSM.buf));
            sleep(1);
            GSM.flag = 1;
            //tcflush(GSM.fd, TCIFLUSH);
            //tcflush(GSM.fd, TCOFLUSH);
        }
    }
    // 用于清空输入/输出缓冲区
    tcflush(GSM.fd, TCIFLUSH);
	tcflush(GSM.fd, TCOFLUSH);
	close(GSM.fd);
	pthread_exit(NULL);
}

