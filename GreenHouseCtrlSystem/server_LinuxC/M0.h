/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   文件名称：M0.h
*   创 建 者：yanweijun
*   创建日期：2018年06月09日
*   描    述：
*
*   更新日志：
*
================================================================*/
#ifndef _M0_H
#define _M0_H
struct _M0
{
	int fd;
	unsigned char read_buff[36];
	unsigned char write_buff[36];
	int rflag;
	int wflag;

};
int set_M0opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int open_M0usb(void);
void * client_M0(void *arg);	//connfd;

#endif /* _M0_H */
