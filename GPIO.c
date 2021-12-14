#include "stm32f10x.h"                  // Device header
#include "option.h"
#include "GPIO.h"

/**********************************/
/*							Macro							*/
/**********************************/

#define			PORT_COUNT		4

/**********************************/
/*				Private Variable				*/
/**********************************/

static unsigned  char GPIOA_isEnable = 0;
static unsigned  char GPIOB_isEnable = 0;
static unsigned  char GPIOC_isEnable = 0;
static unsigned  char GPIOD_isEnable = 0;

/**********************************/
/*				Private Function				*/
/**********************************/

static int Check_PortNum(unsigned int portNum)
{
	if(portNum < PORT_COUNT)
	{
		return RET_OK;
	}
	
	return RET_ERR;
}

/**********************************/
/*				Public Function					*/
/**********************************/

int GPIO_CLK_Enable(unsigned int portNum)//Enable GPIO RCC Clock_&_Check if GPIO RCC Clock is Enable
{
	if(Check_PortNum(portNum) == RET_ERR)
	{
		return RET_ERR;
	}
	
	if((portNum == PA) && (GPIOA_isEnable != ENABLED))
	{
		RCC->APB2ENR |= 0x04;				//Enable PA
		GPIOA_isEnable = ENABLED;
		return RET_OK;
	}
	else if((portNum == PB) && (GPIOB_isEnable != ENABLED))
	{
		RCC->APB2ENR |= 0x08;				//Enable PB
		GPIOB_isEnable = ENABLED;
		return RET_OK;
	}
	else if((portNum == PC) && (GPIOC_isEnable != ENABLED))
	{
		RCC->APB2ENR |= 0x10;				//Enable PC
		GPIOC_isEnable = ENABLED;
		return RET_OK;
	}
	else if((portNum == PD) && (GPIOD_isEnable != ENABLED))
	{
		RCC->APB2ENR |= 0x20;				//Enable PD
		GPIOD_isEnable = ENABLED;
		return RET_OK;
	}
	
	return RET_OK;
}

int GPIO_CLK_Disable(unsigned int portNum)//Disable GPIO RCC Clock_&_Check if GPIO RCC Clock is Disable
{
	if(Check_PortNum(portNum) == RET_ERR)
	{
		return RET_ERR;
	}
	
	if((portNum == PA) && (GPIOA_isEnable != DISABLED))
	{
		RCC->APB2ENR &= 0xfffffffb;				//Disable PA
		GPIOA_isEnable = DISABLED;
		return RET_OK;
	}
	else if((portNum == PB) && (GPIOB_isEnable != DISABLED))
	{
		RCC->APB2ENR &= 0xfffffff7;				//Disable PB
		GPIOB_isEnable = DISABLED;
		return RET_OK;
	}
	else if((portNum == PC) && (GPIOC_isEnable != DISABLED))
	{
		RCC->APB2ENR &= 0xffffffef;				//Disable PC
		GPIOC_isEnable = DISABLED;
		return RET_OK;
	}
	else if((portNum == PD) && (GPIOD_isEnable != DISABLED))
	{
		RCC->APB2ENR &= 0xffffffdf;				//Disable PD
		GPIOD_isEnable = DISABLED;
		return RET_OK;
	}
	
	return RET_OK;
}

int GPIO_Pin_Cnf(unsigned int io_cnf, unsigned int portNum, unsigned int pinNum)
{
	if((io_cnf > 15) || (pinNum > 15))			//Bits 1111(0xf)
	{
		return RET_ERR;
	}
	
	GPIO_TypeDef *GPIOx;
	unsigned int i, j;
	
	if((portNum == PA) && (GPIOA_isEnable == ENABLED))
	{
		GPIOx = GPIOA;
	}
	else if((portNum == PB) && (GPIOB_isEnable == ENABLED))
	{
		GPIOx = GPIOB;
	}
	else if((portNum == PC) && (GPIOC_isEnable == ENABLED))
	{
		GPIOx = GPIOC;
	}
	else if((portNum == PD) && (GPIOD_isEnable == ENABLED))
	{
		GPIOx = GPIOD;
	}
	else return RET_ERR;
	
	i = (pinNum % 8) * 4;		//i Shift Steps (1 Shift Step = 4 bits)
	j = 0xf;
	j = j << i;
	j = ~j;									//j = 0xfff..0..ff (Used to Clear Original State)
	
	//[0] and [io_cnf] positions are decided by pinNum
	if(pinNum < 8)
	{
		GPIOx->CRL &= j;															//Clear	0xff...[0]........ff
		GPIOx->CRL |= ((unsigned int )io_cnf << i);		//Set  	0x00...[io_cnf]...00
		return RET_OK;
	}
	else if(pinNum < 16)
	{
		GPIOx->CRH &= j;
		GPIOx->CRH |= ((unsigned int )io_cnf << i);
		return RET_OK;
	}
	
	return RET_ERR;
}

int GPIO_Pin_Write(unsigned int portNum, unsigned int pinNum, unsigned int value)
{
	if((Check_PortNum(portNum) == RET_ERR) || (pinNum >= 16))
	{
		return RET_ERR;
	}
	
	GPIO_TypeDef *GPIOx;
	if(portNum == PA)
	{
		GPIOx = GPIOA;
	}
	else if(portNum == PB)
	{
		GPIOx = GPIOB;
	}
	else if(portNum == PC)
	{
		GPIOx = GPIOC;
	}
	else if(portNum == PD)
	{
		GPIOx = GPIOD;
	}
	else
	{
		return RET_ERR;
	}
	
	if(value == GPIO_HIGH)
	{
		GPIOx->ODR |= (1 << pinNum);
	}
	else if(value == GPIO_LOW)
	{
		GPIOx->ODR &= (~(1 << pinNum));
	}
	else
	{
		return RET_ERR;
	}
	
	return RET_OK;
}

int GPIO_Pin_Toggle(unsigned int portNum, unsigned int pinNum)
{
	if((Check_PortNum(portNum) == RET_ERR) || (pinNum >= 16))
	{
		return RET_ERR;
	}
	
	GPIO_TypeDef *GPIOx;
	if(portNum == PA)
	{
		GPIOx = GPIOA;
	}
	else if(portNum == PB)
	{
		GPIOx = GPIOB;
	}
	else if(portNum == PC)
	{
		GPIOx = GPIOC;
	}
	else if(portNum == PD)
	{
		GPIOx = GPIOD;
	}
	else
	{
		return RET_ERR;
	}
	
	GPIOx->ODR ^= (1 << pinNum);
	
	return RET_OK;
}

