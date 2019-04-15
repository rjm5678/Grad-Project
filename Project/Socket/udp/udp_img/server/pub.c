/*
 * pub1.c
 *
 *  Created on: 2017年7月2日
 *      Author: Administrator
 */
#define Linux
 
#ifdef WIN
#include <WinSock2.h>
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET int
#endif

#include <stdio.h>
#include "pub.h"

//#define BUFSIZE 262144 //256K
//#define BUFSIZE 131072 //128K
//#define BUFSIZE 65536 //64K
#define BUFSIZE 32768 //64K
void getfilename(char *filename, char *name)
{
	int len = strlen(filename);
	int i;
	for (i=(len-1);i>=0;i--)
	{
		if ((filename[1] == '\\' || filename[i] == '/'))
		{
			break;
		}
	}
	strcpy(name, &filename[i+1]);
	return ;
}

int init_socket()
{
#ifdef WIN
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
#endif
	return 0;
}


SOCKET create_recv_socket(int port)
{
//	if (init_socket() == -1)
//	{
//		printf("init_socket fun error\n");
//		return 0;
//	}
	SOCKET st = socket(AF_INET, SOCK_DGRAM, 0);  //建立TCP socket
	if (st == 0)
	{
		return 0;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind failed %s\n", strerror(errno));
		return 0;
	}
	return st;
}

SOCKET create_send_socket()
{
	if (init_socket() == -1)
	{
		printf("init_socket fun error\n");
		return 0;
	}
	SOCKET st = socket(AF_INET, SOCK_DGRAM, 0);  //建立TCP socket
	if (st == 0)
	{
		return 0;
	}
	return st;
}

int send_work(const char *hostname, int port, char *filename)
{
	SOCKET st_send = create_send_socket();
	SOCKET st_recv = create_recv_socket(port + 1);
	if (st_send == 0)
	{
		printf("st_send create error\n");
		return 0;
	}
	if (st_recv == 0)
	{
		printf("st_recv create error\n");
		return 0;
	}

	FILE *fd = fopen(filename, "rb");
	if (fd == NULL)
	{
		printf("open %s failed %s\n", filename, strerror(errno));
		return 0;
	}
	char *buf = malloc(BUFSIZE);
	memset(buf, 0, BUFSIZE);
	getfilename(filename, buf);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(hostname);
	size_t rc = sendto(st_send, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
	if (rc <= 0)
	{
		printf("send failed %s\n", strerror(errno));
	}
	else
	{
		struct sockaddr_in client_addr;
#ifdef WIN
	int len = 0;
#else
	unsigned int len = 0;
#endif

	    len = sizeof(client_addr);
		memset(&client_addr, 0, sizeof(client_addr));

		memset(buf, 0, BUFSIZE);
		if ((rc = recvfrom(st_recv, buf, BUFSIZE, 0,
				(struct sockaddr *) &client_addr, &len)) <= 0)
		{

			printf("recv failed %s\n", strerror(errno));
		}
		else
		{
printf("%s\n",buf)	;
printf("rc1 = %zu\n", rc);
			if (strncmp(buf, "OK", 2) == 0)
			{
				while (1)
				{
					memset(buf, 0, BUFSIZE);
					rc = fread(buf, 1, BUFSIZE - 1, fd);
printf("rc2 = %zu\n", rc);
					if (rc <= 0)
					{
						break;
					}
					else
					{
						rc = sendto(st_send, buf, rc, 0, (struct sockaddr *) &addr, sizeof(addr));
printf("rc3 = %zu\n", rc);
						if (rc <= 0)
						{
							printf("fread failed %s\n", strerror(errno));
							break;
						}
					}
				}
			}
		}
		memset(buf, 0, BUFSIZE);
		rc = sendto(st_send, buf, 128, 0, (struct sockaddr *)&addr, sizeof(addr));//代表文件发送完毕
printf("rc3 = %zu\n", rc);
	}

	fclose(fd);
	free(buf);

#ifdef WIN
    closesocket(st_send);
    closesocket(st_recv);
    WSACleanup();
#else
    close(st_send);
    close(st_recv);
#endif
	return 1;
}

int recv_work(int port)
{
	SOCKET st_send = create_send_socket();
	SOCKET st_recv = create_recv_socket(port);
	if (st_send == 0)
	{
		printf("st_send create error\n");
		return 0;
	}
	if (st_recv == 0)
	{
		printf("st_recv create error\n");
		return 0;
	}
	char *buf = malloc(BUFSIZE);
	FILE *fd = NULL;
#ifdef WIN
	int len = 0;
#else
	unsigned int len = 1;
#endif

	struct sockaddr_in client_addr;
	len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	memset(buf, 0, BUFSIZE);
    size_t rc = recvfrom(st_recv, buf, BUFSIZE, 0, (struct sockaddr *)&client_addr, &len);
	if (rc <= 0)
	{
		printf("recv failed %s\n", strerror(errno));
	}
	else
	{
		printf("receiving %s\n", buf);
		char *s = "a.jpg";
		fd = fopen(s, "wb");
		if (fd == NULL)
		{
			printf("open %s failed %s\n", s, strerror(errno));
		}
		else
		{
			client_addr.sin_port = htons(port + 1);
			memset(buf, 0, BUFSIZE);
			strcpy(buf, "OK");
			rc = sendto(st_send, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
			if (rc <= 0)
			{
				printf("recv_work send failed %s\n", strerror(errno));
			}
			while (1)
			{
				memset(buf, 0, BUFSIZE);
				rc = recvfrom(st_recv, buf, BUFSIZE, 0,  (struct sockaddr *)&client_addr, &len);
printf("rc2 = %zu\n", rc);
				char tmp[128];
				memset(tmp, 0, sizeof(tmp));
				if (memcmp(buf, tmp, sizeof(tmp)) == 0)
					break;
				if (rc <= 0)
				{
					printf("send failed %s\n", strerror(errno));
					break;
				}
				else
				{
					fwrite(buf, 1, rc, fd);
				}
			}
		}
	}
	if (fd)
	{
		fclose(fd);
	}
	free(buf);

#ifdef WIN
    closesocket(st_send);
    closesocket(st_recv);
    WSACleanup();
#else
    close(st_send);
    close(st_recv);
#endif
	return 1;
}
