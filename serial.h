// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#define RX_TRIS TRISC7
#define TX_TRIS TRISC6
#define TX_SETUP 0x24
#define RX_SETUP 0x90
#define BAUD_LSB 0x81 //9600 bps
#define BAUD_MSB 0x00

#define BUFFSIZE 128

typedef struct {
	unsigned char receiver[BUFFSIZE];
	unsigned char recindex;
} serial_module;

extern serial_module serialdata;

void serial_int(void);

/* DESCRIPTION: sets up the serial module --------------------------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void serial_setup(void);

/* DESCRIPTION: sends a single character over serial ---------------- */
/* PARAM: the character to send ------------------------------------- */
/* RETURN: none ----------------------------------------------------- */
void putch(unsigned char byte);

/* DESCRIPTION: clears the looping receiver buffer ------------------ */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void serial_clearreceiver(void);

/* DESCRIPTION: checks serial buffer for a token match -------------- */
/* PARAM: the token, length of the token, timeout (0 = none) -------- */
/* RETURN: the index + 1 of the found token, or 0 (no token) -------- */
/* NOTE: returned index can wrap arround buffer --------------------- */
unsigned char serial_tokenmatch(const unsigned char * token, unsigned char tokenlen, unsigned char timeout);

/* DESCRIPTION: checks serial buffer for a token match ONCE --------- */
/* PARAM: the token, length of the token ---------------------------- */
/* RETURN: the index + 1 of the found token, or 0 (no token) -------- */
/* NOTE: returned index can wrap arround buffer --------------------- */
unsigned char serial_stokenmatch(const unsigned char * token, unsigned char tokenlen);
