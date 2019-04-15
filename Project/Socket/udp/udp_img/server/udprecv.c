/*
 * udprecv.c
 *
 *  Created on: 2017年6月29日
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include "pub.h"

int main(int arg, char *args[])
{
	if (arg < 2)
	{
		printf("usage:server port\n");
		return EXIT_FAILURE;
	}
	int iport = atoi(args[1]);
	if (iport == 0)
	{
		printf("port %d is invalid\n", iport);
		return EXIT_FAILURE;
	}
	printf("recv is begin\n");
	while (1)
	{
		if (recv_work(iport) == 1)
		{
			printf("recv success\n");
		}
		else
		{
			printf("recv fail\n");
		}
	}
	return EXIT_SUCCESS;
}
