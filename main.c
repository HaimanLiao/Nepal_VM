#include "stm32f10x.h"                  // Device header
#include "option.h"
#include "GPIO.h"
#include "Timer.h"
#include "SysTick.h"

//Add UART Interface: 
//UART_Send(UART_TypeDef* UARTx, char* sendBuf, int num);
//UART_Recv(UART_TypeDef* UARTx, char* recvBuf, int num);
//Use Macro Like #define FRAME_TALl		0xef (1 Code Expanding(daimakuozhan) 2 Can Act Like Comment (zhushi))

static unsigned char recvFrame[4] = {0};
static unsigned int flag = 0;

int main(void)
{
//GPIO Init
	GPIO_CLK_Enable(PA);
	GPIO_Pin_Cnf(OUTPUT_GP_PP, PA, PIN2);			//Blue
	GPIO_Pin_Cnf(OUTPUT_GP_PP, PA, PIN3);			//Green
	GPIO_Pin_Cnf(OUTPUT_GP_PP, PA, PIN4);			//Red
	GPIO_CLK_Enable(PB);
	GPIO_Pin_Cnf(OUTPUT_GP_PP, PB, PIN0);			//Blue
	
//USART1 Init
	RCC->APB2ENR |= 0x1;											//Enable the Alternate Function Module
	RCC->APB2ENR |= 0x4000;										//Enable USART1 Clock
	GPIO_CLK_Enable(PA);
	GPIO_Pin_Cnf(OUTPUT_AF_PP, PA, PIN9);
	GPIO_Pin_Cnf(INPUT_P_UP_DOWN, PA, PIN10);	//Configure GPIOs (TX---PA9 + RX---PA10)
	USART1->BRR = 0x341;											//Set Up Baud Rate
	USART1->CR1 |= 0x8;												//Enable USART1 Transmit
	USART1->CR1 |= 0x4;												//Enable USART1 Receive
	USART1->CR1 |= 0x2000;										//Enable USART1 module

//RS485 Send and Receive Conctrol Init
	GPIO_CLK_Enable(PA);
	GPIO_Pin_Cnf(OUTPUT_GP_PP, PA, PIN8);
	
	__disable_irq();
//SysTick Init
	SysTick_Init(8000);														//Already "ticks - 1"
	NVIC_EnableIRQ(SysTick_IRQn);
	
	__enable_irq();
	
//Main Loop
	GPIO_Pin_Write(PA, PIN8, GPIO_LOW);						//RS485 Receive
	Delay(10);																		//Wait Until RS485 Chip is Ready (10ms)
	while(1)
	{
		while(1)	//This while is Needed, In case of process quiting (Break)
		{
			recvFrame[0] = (unsigned char)USART1->DR;		//Clear the USART1 data register
			
			if(recvFrame[0] != 0xfe)	//recvFrame[0] is initialized as 0
			{
				while((USART1->SR & 0x20) == 0x00){}			//When there is no Data, then wait
				recvFrame[0] = (unsigned char)USART1->DR;
				if(recvFrame[0] != 0xfe)break;
				
				//Make Sure That recvFrame[0] is Set 0xfe by UART Transmit Not It is Already 0xfe
				/*1 When recvFrame[3] != 0xef and recvFrame[3] != 0xfe
						There Could be an Error If recvFrame[0] is Not Set 0x00 or Others Excpet 0xfe(Frame Head)*/
				//2 When Reply to Master, recvFrame[0] Need to be Set Back to 0xfe accordingly
				recvFrame[0] = 0x00;
			}
			
			while((USART1->SR & 0x20) == 0x00){}
			recvFrame[1] = (unsigned char)USART1->DR;
			if(recvFrame[1] == 0xfe)
			{
				recvFrame[0] = 0xfe;
				break;
			}
			while((USART1->SR & 0x20) == 0x00){}
			recvFrame[2] = (unsigned char)USART1->DR;
			if(recvFrame[2] == 0xfe)
			{
				recvFrame[0] = 0xfe;
				break;
			}
			while((USART1->SR & 0x20) == 0x00){}
			recvFrame[3] = (unsigned char)USART1->DR;
			if(recvFrame[3] != 0xef)								//Frame Error
			{
				if(recvFrame[3] == 0xfe)recvFrame[0] = 0xfe;
				break;	
			}
				
			//Verify Address (recvFrame[1])
			/*
			...
			*/
			if(recvFrame[2] == 0x01)
			{
				GPIO_Pin_Write(PA, PIN2, GPIO_HIGH);	//Blue
				GPIO_Pin_Write(PA, PIN3, GPIO_LOW);
				GPIO_Pin_Write(PA, PIN4, GPIO_LOW);
			}
			else if(recvFrame[2] == 0x02)
			{
				GPIO_Pin_Write(PA, PIN2, GPIO_LOW);
				GPIO_Pin_Write(PA, PIN3, GPIO_HIGH);	//Green
				GPIO_Pin_Write(PA, PIN4, GPIO_LOW);
			}
			else if(recvFrame[2] == 0x03)
			{
				GPIO_Pin_Write(PA, PIN2, GPIO_LOW);
				GPIO_Pin_Write(PA, PIN3, GPIO_LOW);
				GPIO_Pin_Write(PA, PIN4, GPIO_HIGH);	//Red
			}
			else																		//White
			{
				GPIO_Pin_Write(PA, PIN2, GPIO_HIGH);
				GPIO_Pin_Write(PA, PIN3, GPIO_HIGH);
				GPIO_Pin_Write(PA, PIN4, GPIO_HIGH);
			}
			
			//Reply Master	
			GPIO_Pin_Write(PA, PIN8, GPIO_HIGH);		//RS485 Send
			Delay(10);															//Wait Until RS485 Chip is Ready (10ms)
			
			recvFrame[0] = 0xfe;										//Needed (recvFrame[0] = 0x00;)
			while((USART1->SR & 0x40) == 0x00){}		//Equal to 0 (true: Bit 7 = 0) means Data Register IS "Not Empty"
			USART1->DR = recvFrame[0];
			while((USART1->SR & 0x40) == 0x00){}
			USART1->DR = recvFrame[1];
			while((USART1->SR & 0x40) == 0x00){}
			USART1->DR = recvFrame[2];
			while((USART1->SR & 0x40) == 0x00){}
			USART1->DR = recvFrame[3];
			while((USART1->SR & 0x40) == 0x00){}		//Wait Until Transmiting Is Completed

			//GPIO_Pin_Toggle(PB, PIN0);
			
			GPIO_Pin_Write(PA, PIN8, GPIO_LOW);			//RS485 Receive
			Delay(10);															//Wait Until RS485 Chip is Ready (10ms)
			
			if(flag == 1)break;
		}
		
		if(flag == 1)break;
	}
	
	return 0;
}
