// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#include <htc.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "timer.h"

serial_module serialdata;

void serial_setup(void)
{
	RX_TRIS = 1; // accept input on RX
	TX_TRIS = 0; // output on TX
	TXSTA = TX_SETUP;
	RCSTA = RX_SETUP;	
	SPBRG = BAUD_LSB;
	SPBRGH = BAUD_MSB;

	//clear buffer
	serial_clearreceiver();

	TXIE = 0;
	PEIE = 1;
	RCIE = 1;
}

void serial_int(void)
{
	//loop incomming data if full
	if (serialdata.recindex >= BUFFSIZE)
		serialdata.recindex = 0;

	serialdata.receiver[serialdata.recindex] = RCREG;

	serialdata.recindex++;
}

// return index + 1 of match
// 0 if no match
unsigned char serial_tokenmatch(const unsigned char * token, unsigned char tokenlen, unsigned char timeout)
{
	unsigned char buff_index = 0;
	unsigned char match_index = 0;

	if (timeout != 0)
		timer_start(timeout);

	while(1)
	{
		if (timeout != 0 && timer_hasstopped())
			return 0;

		if (serialdata.receiver[buff_index] == token[match_index])
		{
			// check if it was the last in the token
			if (match_index == tokenlen - 1)
			{
				// its been found!
				if (buff_index >= (tokenlen - 1))
					return buff_index + 2 - tokenlen;
				else
					return buff_index + BUFFSIZE + 2 - tokenlen;
			} else {
				// not the last match
				match_index++;
			}
		} else {
			// no match - restart
			match_index = 0;
		}

		buff_index++;
		if (buff_index == BUFFSIZE)
			buff_index = 0;
	}

	return 0;
}

//single token match (runs once)
unsigned char serial_stokenmatch(const unsigned char * token, unsigned char tokenlen)
{
	unsigned char i;
	unsigned char buff_index = 0;
	unsigned char match_index = 0;

	if (tokenlen > BUFFSIZE)
		return 0;

	for (i = 0; i < (BUFFSIZE + tokenlen); i++)
	{
		// allow for some overlap
		if (i >= BUFFSIZE)
			buff_index = i - BUFFSIZE;
				else
			buff_index = i;

		if (serialdata.receiver[buff_index] == token[match_index])
		{
			// check if it was the last in the token
			if (match_index == tokenlen - 1)
			{
				// its been found!
				if (buff_index >= (tokenlen - 1))
					return buff_index + 2 - tokenlen;
				else
					return buff_index + BUFFSIZE + 2 - tokenlen;
			} else {
				// not the last match
				match_index++;
			}
		} else {
			// no match - restart
			match_index = 0;
		}
	}

	return 0;
}

void serial_clearreceiver(void)
{
	unsigned char i;

	serialdata.recindex = 0;
	for(i=0;i<BUFFSIZE;i++)
	{
		serialdata.receiver[i] = 0x00;
	}
}

void putch(unsigned char byte) 
{
	/* output one byte */
	while(!TXIF)	/* set when register is empty */
		continue;
	TXREG = byte;
}

