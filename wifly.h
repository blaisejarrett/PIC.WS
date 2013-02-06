// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

typedef struct {
	unsigned char cmd_mode;
	unsigned char conn_open;
} wifly_module;
extern wifly_module wifly;

/* DESCRIPTION: Sends data over serial to wifly module -------------- */
/* PARAM: send: The data to send (has to be null terminated) ---------*/
/* PARAM: ack: The expected responce (has to be null terminated) -----*/
/* PARAM: timeout: Number of seconds until the command fails ---------*/
/* RETURN: returns 1 for success ------------------------------------ */
unsigned char wifly_send(const unsigned char * send, const unsigned char * ack, unsigned char timeout);

/* DESCRIPTION: Opens a TCP connection ------------------------------ */
/* PARAM: ip: The IP to connect to -----------------------------------*/
/* PARAM: port: The Port to connect to -------------------------------*/
/* RETURN: returns 1 for success ------------------------------------ */
unsigned char wifly_connect(const unsigned char * ip, const unsigned char * port);

/* DESCRIPTION: Closes the TCP connection --------------------------- */
/* PARAM: none -------------------------------------------------------*/
/* RETURN: returns 1 for success ------------------------------------ */
unsigned char wifly_closeconn(void);

/* DESCRIPTION: Enter or leave command mode ------------------------- */
/* PARAM: mode: 1 = Enter CMD mode, 0 = Leave CMD mode ---------------*/
/* RETURN: returns 1 for success ------------------------------------ */
unsigned char wifly_cmdmode(unsigned char mode);

/* DESCRIPTION: Checks if the connection has been closed by the host  */
/* PARAM: none -------------------------------------------------------*/
/* RETURN: returns 1 if connection has been closed ------------------ */
/* NOTE: This should be polled -------------------------------------- */
unsigned char wifly_chkclose(void);


