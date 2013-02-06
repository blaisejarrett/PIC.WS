// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#include <string.h>
#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "wifly.h"

wifly_module wifly = {0, 0};

unsigned char wifly_connect(const unsigned char * ip, const unsigned char * port)
{	
	if (!wifly_cmdmode(1)) return 0;
	
	serial_clearreceiver();
	printf("open %s %s\r\n", ip, port);
	
	if (serial_tokenmatch("*OPEN*", 6, 10))
	{
		serial_clearreceiver();
		wifly.cmd_mode = 0;
		wifly.conn_open = 1;
		return 1;
	}
	return 0;
}

unsigned char wifly_closeconn(void)
{
	if (!wifly.conn_open) return 1;
	if (!wifly_cmdmode(1)) return 0;
	
	serial_clearreceiver();
	printf("close\r\n");
	
	if (serial_tokenmatch("*CLOS*", 6, 5))
	{
		serial_clearreceiver();
		wifly.conn_open = 0;
		return 1;
	}
	return 0;
}

unsigned char wifly_chkclose(void)
{
	if (serial_stokenmatch("*CLOS*", 6))
	{
		serial_clearreceiver();
		wifly.conn_open = 0;
		return 1;
	}
	
	return 0;
}

unsigned char wifly_cmdmode(unsigned char mode)
{
	if (mode)
	{
		if (!wifly.cmd_mode)
		{
			if (wifly_send("$$$", "CMD", 3)) 
			{
				wifly.cmd_mode = 1;
				return 1;
			}
			else
				return 0;
		}
		return 1;
	} else {
		if (wifly.cmd_mode)
		{
			if (wifly_send("exit\r\n", "EXIT", 3)) 
			{
				wifly.cmd_mode = 0;
				return 1;
			}
			else
				return 0;
		}
	}
	return 0;
}

unsigned char wifly_send(const unsigned char * send, const unsigned char * ack, unsigned char timeout)
{
	if (ack != 0)
	{
		serial_clearreceiver();
		if (send != 0)
			printf("%s", send);
		
		return serial_tokenmatch(ack, strlen(ack), timeout);	
	}
	
	printf("%s", send);
	return 1;
}