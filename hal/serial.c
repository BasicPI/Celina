/*****************************************************************************
 * serial.c
 *
 * driver for usart3 This code assumes it is a MAX485 that needs to be
 * enabled via PB2 - high on send, low on receive. The top part is USART3
 * specific code, while the serial driver is the init, put and get at
 * bottom.
 *
 * In this version we just use the manual copy and assume we have sufficient
 * CPU. A smarter version should set up DMA's.
 *
 * RS485 2-wire: We are sending on the same line as we receive, so sending
 * bytes will trick the IRQ to believe we are receiving bytes as well. To deal
 * with this we need to ignore received characters during a send sequence.
 *
 * Interface:
 *
 * 		SerialInit		Initialize serial line
 * 		SerialPut		Send a character
 * 		SerialGet		Get input characters
 *
 * TODO : This needs work
 ****************************************************************************/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stdio.h"
#include "fifo.h"
//#include "rtXML.h"

#define USART3_FIFO_Size 1024

char USART3_FIFO[USART3_FIFO_Size];

/*****************************************************************************
 * USART1 IRQ Handler
 *
 * Called when a character is received on USART3. At this point we just add
 * the character to the receive FIFO
 ****************************************************************************/
void USART1_IRQHandler(void)
{
	char c;
	char buf[100];

    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
		c = (char)USART_ReceiveData(USART1);
		FIFOWriteByte(USART3_FIFO,&c);
	}
}

/*******************************************************************************
* USART3_GPIO_Configuration
*******************************************************************************/
void USART3_GPIO3_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************************************************************
* USART3_Configuration
*******************************************************************************/
void USART3_Configuration(uint32_t baudrate,uint16_t parity,uint16_t databits, uint16_t stopbits, uint16_t flowcontrol)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = databits;
	USART_InitStructure.USART_StopBits = stopbits;
	USART_InitStructure.USART_Parity = parity;
	USART_InitStructure.USART_HardwareFlowControl = flowcontrol;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART3 */
	USART_Cmd(USART1, ENABLE);
	//USART_HalfDuplexCmd(USART1, ENABLE);
 }

void USART3_NVIC3_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : UARTSend
* Description    : Send a string to the UART.
* Input          : - pucBuffer: buffers to be printed.
*                : - ulCount  : buffer's length
* Output         : None
* Return         : None
*******************************************************************************/
void UART1Send(const unsigned char *pucBuffer, unsigned long ulCount)
{
	GPIOC->ODR |= GPIO_Pin_14;

    while(ulCount--)
    {
        USART_SendData(USART1, (uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
	GPIOC->ODR ^= GPIO_Pin_14;
}

void SerialInit(uint32_t baudrate,uint16_t parity,uint16_t databits, uint16_t stopbits, uint16_t flowcontrol)
{
	FIFOCreate(USART3_FIFO, USART3_FIFO_Size);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* NVIC Configuration */
    USART3_NVIC3_Configuration();

    USART3_GPIO3_Configuration();

    USART3_Configuration(baudrate,parity,databits,stopbits,flowcontrol);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void SerialPut(char c)
{
	GPIOC->ODR |= GPIO_Pin_All;

	USART_SendData(USART1, (uint16_t) c);
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{
	}
	GPIOC->ODR ^= GPIO_Pin_All;
}

/*****************************************************************************
 * SerialGet
 *
 * Copy FIFO content to a buffer and free FIFO buffer.
 ****************************************************************************/
int SerialGet(char * buf, int * size)
{
}

