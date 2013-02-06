// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#define _T0CON 0b11000111	// 256 * prescaller, 8bit mode
#define COUNTER_TYPE unsigned char	// type of counter (seconds)
#define RAW_TYPE unsigned char // type of raw counter (some fraction of a second)
#define NUM_RAW_SECONDS 76 // number of raw interrupts to produces roughly 1s

typedef struct {
    COUNTER_TYPE counter;
	RAW_TYPE raw;
	COUNTER_TYPE max;
	unsigned char expired;
} timerdata;

extern timerdata timermodule;

/* DESCRIPTION: interrupt routine ----------------------------------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void timer_int(void);

/* DESCRIPTION: setup timer interrupts ------------------------------ */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void timer_setup(void);

/* DESCRIPTION: stop timer ------------------------------------------ */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: none ----------------------------------------------------- */
void timer_stop(void);

/* DESCRIPTION: start timer ----------------------------------------- */
/* PARAM: time in seconds until timer stops, use 0 to never stop ---- */
/* RETURN: none ----------------------------------------------------- */
void timer_start(COUNTER_TYPE max);

/* DESCRIPTION: interrupt routine ----------------------------------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: if the timer has automatically stopped ------------------- */
unsigned char timer_hasstopped(void);

/* DESCRIPTION: gets time ------------------------------------------- */
/* PARAM: none ------------------------------------------------------ */
/* RETURN: number of seconds since timer has started ---------------- */
COUNTER_TYPE timer_gettime(void);
