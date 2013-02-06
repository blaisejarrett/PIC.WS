// Copyright (c) 2011 All Right Reserved, Blaise Jarrett,
// http://blaisejarrett.com/
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#include <htc.h>
#include "timer.h"

timerdata timermodule;

void timer_setup(void)
{
	T0CON = _T0CON;
}

void timer_int(void)
{
	timermodule.raw++;

	if (timermodule.raw >= NUM_RAW_SECONDS)
	{
		timermodule.counter++;
		timermodule.raw = 0;
	}

	if ((timermodule.max != 0) && (timermodule.max <= timermodule.counter))
	{
		timer_stop();
	}
}

void timer_start(COUNTER_TYPE max)
{
	timermodule.raw = 0;
	timermodule.expired = 0;
	timermodule.counter = 0;
	timermodule.max = max;

	TMR0IE = 1;
	TMR0IF = 0;
}

void timer_stop(void)
{
	TMR0IE = 0;
	timermodule.expired = 1;
}

unsigned char timer_hasstopped(void)
{
	return timermodule.expired;
}

COUNTER_TYPE timer_gettime(void)
{
	return timermodule.counter;
}
