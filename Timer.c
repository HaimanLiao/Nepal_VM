#include "stm32f10x.h"                  // Device header
#include "option.h"
#include "Timer.h"

static unsigned int TIM6_isEnable = 0;
static unsigned int TIM7_isEnable = 0;
static unsigned int TIM1_isEnable = 0;

int Timer_CLK_Enable(unsigned int timerNum)
{
	if((timerNum == TIMER6) && (TIM6_isEnable == 0))
	{
		RCC->APB1ENR |= 0x10;
		TIM6_isEnable = 1;
	}
	else if((timerNum == TIMER7) && (TIM7_isEnable == 0))
	{
		RCC->APB1ENR |= 0x20;
		TIM7_isEnable = 1;
	}
	else if((timerNum == TIMER1) && (TIM1_isEnable == 0))
	{
		RCC->APB2ENR |= 0x800;
		TIM1_isEnable = 1;
	}
	//else if(timerNum == TIMER1...
	else return RET_ERR;
	
	return RET_OK;
}
int Timer_CLK_Disable(unsigned int timerNum)
{
	if((timerNum == TIMER6) && (TIM6_isEnable == 1))
	{
		RCC->APB1ENR &= 0xffffffef;
		TIM6_isEnable = 0;
	}
	else if((timerNum == TIMER7) && (TIM7_isEnable == 1))
	{
		RCC->APB1ENR &= 0xffffffdf;
		TIM7_isEnable = 0;
	}
	else if((timerNum == TIMER1) && (TIM1_isEnable == 1))
	{
		RCC->APB2ENR &= 0xfffff7ff;
		TIM1_isEnable = 0;
	}
	//else if(timerNum == TIMER1...
	else return RET_ERR;
	
	return RET_OK;
}


int Timer_Cnf(unsigned int timerNum, unsigned int ARR, unsigned int PSC, unsigned char int_isEnable)
{
	TIM_TypeDef *TIMx;

	if((timerNum == TIMER6) && (TIM6_isEnable == 1))
	{
		TIMx = TIM6;
	}
	else if((timerNum == TIMER7) && (TIM7_isEnable == 1))
	{
		TIMx = TIM7;
	}
	else if((timerNum == TIMER1) && (TIM1_isEnable == 1))
	{
		TIMx = TIM1;
	}
	//else if(timerNum == TIMER1...
	else return RET_ERR;
	
	TIMx->ARR &= 0x0000;
	TIMx->PSC &= 0x0000;
	TIMx->ARR |= ARR;
	TIMx->PSC |= PSC;
	
	TIMx->EGR |= 0x1;
	TIMx->CR1 |= 0x81;		//Counter Enable + Auto-reload preload Enable
	
	if(int_isEnable == ENABLED)
	{
		TIMx->DIER |= 0x1;
	}
	else if(int_isEnable == DISABLED)
	{
		TIMx->DIER &= 0xfffe;
	}
	else return RET_ERR;
	
	return RET_OK;
}
