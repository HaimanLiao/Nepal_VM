#ifndef TIMER_H
#define TIMER_H


#define			TIMER1		0
/*
#define			TIMER8		1
...
#define			TIMER14		11
*/
#define			TIMER6		12
#define			TIMER7		13

int Timer_CLK_Enable(unsigned int timerNum);
int Timer_CLK_Disable(unsigned int timerNum);
int Timer_Cnf(unsigned int timerNum, unsigned int ARR, unsigned int PSC, unsigned char int_isEnable);

#endif

