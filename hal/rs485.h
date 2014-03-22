/*****************************************************************************
 * RS485.h
 *
 * Implements a RS485 2-wire driver on top of a STM32 Cortex-M3 UART/USART.
 ****************************************************************************/
#ifndef _RS485
#define _RS485

class rs485
{
protected:
	USART_TypeDef * USARTx;

public:

	rs485(USART_TypeDef * ux = NULL)
	{
		USARTx = ux;
	}

	void SetUSART(USART_TypeDef * ux)
	{
		USARTx = ux;
	}

	void Init(uint32_t baudrate,uint16_t parity,uint16_t databits, uint16_t stopbits, uint16_t flowcontrol)
	{
		SerialInit(baudrate,parity,databits, stopbits, flowcontrol);
	}
};

#endif
