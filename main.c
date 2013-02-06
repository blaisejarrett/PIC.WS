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
#include "timer.h"
#include "serial.h"
#include "wifly.h"
#include "websocket_client.h"
#define _XTAL_FREQ 20000000

#define _IP "192.168.1.201"
#define _PORT "9000"

#ifdef HT98
#pragma config IESO=OFF, OSC=HS, FCMEN=OFF, BOREN=OFF, PWRT=OFF, DEBUG=OFF
#pragma config WDT=OFF
#pragma config MCLRE=OFF
#pragma config STVREN=ON, LVP=OFF, PBADEN=ON
#pragma config CPD=OFF, CPB=OFF
#else
    __CONFIG(1, OSC_HS);		//__CONFIG(1, 0x08FF);
	__CONFIG(2, BORDIS & PWRTDIS & WDTDIS); // 2nd config word
    __CONFIG(3, MCLRDIS & PBDIGITAL); // 3rd config word
	__CONFIG(4, LVPDIS); // 4th config word
	__CONFIG(5, UNPROTECT); // 5th config word
	__CONFIG(6, WRTEN); // 6th config word
#endif	

void initio(void);

void main()
{
	unsigned char message[100];
	
	initio();
	timer_setup();
	serial_setup();

	timer_start(5);	// start up timer
	while (!timer_hasstopped()){}
	
	while (1)
	{
		while(!wifly_connect(_IP, _PORT)){}
		//send header
		//check for accepting protocal upgrade
		if (!websocket_open()) {wifly_closeconn(); continue;}
		
		while(wifly.conn_open)
		{
			// check for connection close
			if (wifly_chkclose()) break;
			
			// read in websocket data
			if (websocket_recpacket())
			{
				sprintf(message, "Wifly + 18F4620 received: \"%s\"\n", websocket.receiver);
				websocket_sendpacket(message); //echo it back
			}	
		}
	}
}

void initio(void)
{
	ADCON0 = 0x00;
	ADCON1 = 0x0F;
	CMCON = 0x07;
	TRISD = 0;
	PORTD = 0;

	GIE = 1;
}

void interrupt isr(void)
{
	if (RCIF && RCIE)
	{
		serial_int();
		RCIF = 0;
	}

	if (TMR0IE && TMR0IF)
	{
		timer_int();
		TMR0IF = 0;
	}
}