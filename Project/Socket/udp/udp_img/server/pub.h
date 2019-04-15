/*
 * pub.h
 *
 *  Created on: 2017年7月1日
 *      Author: Administrator
 */

#ifndef PUB_H_
#define PUB_H_
#define SOCKET int

void getfilename(char *filename, char *name);
int init_socket();
SOCKET create_send_socket();
SOCKET create_recv_socket(int port);
int send_work(const char *hostname, int port, char *filename);
int recv_work(int port);

#endif /* PUB_H_ */
