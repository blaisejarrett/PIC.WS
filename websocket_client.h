// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#define HOST "192.168.1.205:9000"
#define DIR "/"
#define KEY "dGhlIHNhbXBsZSBub25jZQ=="
#define ACK_KEY "s3pPLMBiTxaQ9kYGzzhZRbK+xOo="
#define MAX_SIZE 128

typedef struct {
	unsigned char receiver[MAX_SIZE];	// null terminated text string
} websocket_module;
extern websocket_module websocket;


/* DESCRIPTION: Sends the appropriate header to set up websocket ---- */
/*              connection, should be called after a connection is -- */
/*              established. ---------------------------------------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: returns 1 if the handshake was successful ---------------- */
/* NOTE: Does not close connection if handshake fails --------------- */
unsigned char websocket_open(void);

/* DESCRIPTION: Sends a text string over the open websocket --------- */
/*              connection. ----------------------------------------- */
/* PARAM: The null terminated string to send ------------------------ */
/* RETURN: none ----------------------------------------------------- */
/* NOTE: Does not check if connection is open ----------------------- */
void websocket_sendpacket(unsigned char * data);

/* DESCRIPTION: Checks if there's new received websocket data ------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: 1 if there is new data, 0 if there is not ---------------- */
/* NOTE: If it finds data it gets coppied to websocket.reiver ------- */
unsigned char websocket_recpacket(void);

/* DESCRIPTION: Called when websocket_recpacket() encounters an error */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void websocket_recerror(void);

