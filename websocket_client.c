// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "websocket_client.h"
#include "wifly.h"
#include "serial.h"

websocket_module websocket;

unsigned char websocket_open(void)
{
	printf("GET %s HTTP/1.1\r\n", DIR);
	printf("Host: %s\r\n", HOST);
	printf("Upgrade: websocket\r\n");
	printf("Connection: Upgrade\r\n");
	printf("Sec-WebSocket-Key: %s\r\n", KEY);
	if (wifly_send("Sec-WebSocket-Version: 13\r\n\r\n", ACK_KEY, 5))
	{
		// wait for double \r\n to terminate header before we can accept websocket data
		if (!serial_tokenmatch("\r\n\r\n", 4, 5)) return 0;
		
		serial_clearreceiver();	// make sure its clean for recieving actual data
		return 1;
	}
	return 0;
}

// note: serial receiver should be cleared after authentication process has completed
unsigned char websocket_recpacket(void)
{
	unsigned char i;
	unsigned char length;
	
	if (!(serialdata.recindex >= 1)) return 0;
		
	if (serialdata.receiver[0] != 0x81) // invalid packet
	{
		websocket_recerror();
		return 0;
	}
	
	if (!(serialdata.recindex >= 2)) return 0;	//second byte contains length
	
	length = serialdata.receiver[1];
	
	//check if the whole packet is there now that we know the length
	if (!(serialdata.recindex >= (2 + length))) return 0;
	
	for (i = 0; i < length; i++)
	{
		websocket.receiver[i] = serialdata.receiver[i + 2];
	}
	websocket.receiver[length] = '\0'; //null terminate it so it can be handled as a string
	serial_clearreceiver();	// clear for next receive
	
	return 1;
}

void websocket_recerror(void)
{
	//check for partial close token match incase host closed connection
	if (serialdata.receiver[0] == '*') return;
	
	wifly_closeconn(); // assume connections still open but invalid data has been received
}

void websocket_sendpacket(unsigned char * data)
{
	unsigned char i;
	unsigned char x;
	unsigned char mask[4];
	unsigned int random;
	unsigned char length;
	
	length = strlen(data);

	if (length > 128)	// not implemented
		return;	

	// create the mask
	for(i = 0; i < 2; i++)
	{
		random = (unsigned int)rand();
		mask[i] = (unsigned char)(random & 0x00FF);
		mask[i+1] = (unsigned char)((random >> 8) & 0x00FF);
	}
	
	putch(0x81);	// header (one frame)
	putch(length | 0x80);

	for(i = 0; i < 4; i++)	// push mask
	{
		putch(mask[i]);
	}

	for(i = 0; i < length; i++)
	{
		x = i % 4;
		putch(data[i] ^ mask[x]);
	}
}

