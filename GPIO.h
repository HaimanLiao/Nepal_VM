#ifndef GPIO_H
#define GPIO_H

/**********************************/
/*							Macro							*/
/**********************************/

//Port
#define			PA			0
#define			PB			1
#define			PC			2
#define			PD			4
//Pin
#define			PIN0		0
#define			PIN1		1
#define			PIN2		2
#define			PIN3		3
#define			PIN4		4
#define			PIN5		5
#define			PIN6		6
#define			PIN7		7
#define			PIN8		8
#define			PIN9		9
#define			PIN10		10
#define			PIN11		11
#define			PIN12		12
#define			PIN13		13
#define			PIN14		14
#define			PIN15		15

//Input_&_Output Configuration
#define			INPUT_ANALOG				0x0				//Bits 0000(0)
#define			INPUTFLOATING				0x4				//Bits 0100(4)
#define			INPUT_P_UP_DOWN			0x8				//Bits 1000(8)

#define			OUTPUT_GP_PP				0x1				//Bits 0001(1)//OUTPUT_10MHZ
#define			OUTPUT_GP_OD				0x5				//Bits 0101(5)//OUTPUT_10MHZ
#define			OUTPUT_AF_PP				0x9				//Bits 1001(9)//OUTPUT_10MHZ
#define			OUTPUT_AF_OD				0xd				//Bits 1101(d)//OUTPUT_10MHZ

//GPIO Input_&_Output Value
#define			GPIO_HIGH			1
#define			GPIO_LOW			0


/**********************************/
/*				Public DataType					*/
/**********************************/


/**********************************/
/*				Extern Variable					*/
/**********************************/


/**********************************/
/*				Public Function					*/
/**********************************/

int GPIO_CLK_Enable(unsigned int portNum);		//Enable GPIO RCC Clock
int GPIO_CLK_Disable(unsigned int portNum);	//Disable GPIO RCC Clock
int GPIO_Pin_Cnf(unsigned int io_cnf, unsigned int portNum, unsigned int pinNum);
int GPIO_Pin_Write(unsigned int portNum, unsigned int pinNum, unsigned int value);
int GPIO_Pin_Toggle(unsigned int portNum, unsigned int pinNum);
//int GPIO_PIN_Read(unsigned int portNum, unsigned int pinNum);//Not Need Currently

#endif

