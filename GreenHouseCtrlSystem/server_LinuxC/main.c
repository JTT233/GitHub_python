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
#include <signal.h>
#include "GSM.h"
#include "M0.h"
#include "SRV.h"
#define PORT 8080

pthread_t client_M0_pthread;
pthread_t client_GSM_pthread;
pthread_t client_CAM_pthread;
pthread_t client_SRV_pthread;

pthread_rwlock_t mutex_RM0;
pthread_rwlock_t mutex_WM0;
pthread_rwlock_t mutex_GSM;
pthread_rwlock_t mutex_CAM;
pthread_mutex_t mutex_SRV;

struct sockaddr_in srvaddr;
struct sockaddr_in cltaddr;
socklen_t addrlen;

int sync_flag = 2;
//int cltfd;

int main(int argc, char *argv[])
{
	//signal(SIGPIPE, SIG_IGN);
	int ret;
	int errno;

	pthread_rwlock_init(&mutex_RM0, NULL);
	pthread_rwlock_init(&mutex_WM0, NULL);
	pthread_rwlock_init(&mutex_GSM, NULL);
	pthread_mutex_init(&mutex_SRV, NULL);
#if 1
	/* zigbee thread */
	ret = pthread_create(&client_M0_pthread, NULL, client_M0, NULL);
	if (ret) {
		errno = ret;
		perror("create zigbee thread");
		exit(EXIT_FAILURE);
	} else
		printf("create zigbee thread success\n");

	ret = pthread_detach(client_M0_pthread);
	if (ret) {
		errno = ret;
		perror("detach zigbee thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach zigbee thread success\n");
#endif
#if 1
	/* gsm thread */
	ret = pthread_create(&client_GSM_pthread, NULL, client_GSM, NULL);
	if (ret) {
		errno = ret;
		perror("create gsm thread");
		exit(EXIT_FAILURE);
	} else
		printf("create gsm thread success\n");

	ret = pthread_detach(client_GSM_pthread);
	if (ret) {
		errno = ret;
		perror("detach gsm thread\n");
		exit(EXIT_FAILURE);
	} else
		printf("detach gsm thread success\n");
#endif
	while (1) {
		pthread_mutex_lock(&mutex_SRV);
		if (sync_flag == 0) {
			pthread_mutex_unlock(&mutex_SRV);
			pthread_mutex_destroy(&mutex_SRV);
			break;
		} else {
			pthread_mutex_unlock(&mutex_SRV);
			sleep(1);
		}
	}

	/* android server thread */
	int listenfd;
	char buf[256];
	/* 1. 创建服务器（创建一socket套接字）；socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("server->socket");
		return -1;
	}
	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	printf("create listenfd = %d success\n", listenfd);

	/* 2. 设置服务器的IP地址和端口号（将socket和服务器的IP地址和端口号进行绑定）；bind */
	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("192.168.2.100");
	ret = bind(listenfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr));

	printf("port : %d\n", ntohs(srvaddr.sin_port));
	if (ret == -1) {
		perror("server->bind");
		return -1;
	}
	printf("xin bind success !\n");

	/* 3. 启动监听（启动服务器）；  listen */
	ret = listen(listenfd, 1024);
	if (ret == -1) {
		perror("server->listen");
		return -1;
	}
	printf("listen success !\n");

	/* main thread, process environment data form m0 board or process short message */
	while (1) {
		int connfd = accept(listenfd, (struct sockaddr *)&cltaddr, &addrlen);
		ret = pthread_create(&client_SRV_pthread, NULL, thread_srv, (void *)&connfd);
		if (ret) {
			errno = ret;
			perror("create android server thread\n");
			exit(EXIT_FAILURE);
		} else
			printf("create android server thread success\n");

		ret = pthread_detach(client_SRV_pthread);
		if (ret) {
			errno = ret;
			perror("detach android server thread\n");
			exit(EXIT_FAILURE);
		} else
			printf("detach android server thread success\n");
		sleep(1);
	}
	pthread_rwlock_destroy(&mutex_RM0);
	pthread_rwlock_destroy(&mutex_WM0);
	pthread_rwlock_destroy(&mutex_GSM);

	exit(EXIT_SUCCESS);
}
