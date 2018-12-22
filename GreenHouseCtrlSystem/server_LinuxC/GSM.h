/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   文件名称：GSM.h
*   创 建 者：yanweijun
*   创建日期：2018年06月09日
*   描    述：
*
*   更新日志：
*
================================================================*/
#ifndef _GSM_H
#define _GSM_H
struct message_info{
    char cnnu[16];
    char phnu[16];
    char message[128];
};

struct pdu_info {
    char cnswap[32];
    char phswap[32];
};
struct _GSM
{
	struct message_info minfo;
	struct pdu_info pinfo;
	int fd;
	char buf[13];
	int flag;
};
void serial_init(int fd);
void swap(char number[],char swap[]);
int sendy(int fd,char *cmgf,char *cmgs,char *message);
int send_zh_message(int fd,struct message_info info,char pho_rcv_no[12],int i);
void * client_GSM(void *arg);	//connfd;
#endif /* _GSM_H */
