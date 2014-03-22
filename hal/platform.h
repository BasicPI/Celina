/*****************************************************************************
 * platform.h
 *
 * Platform.h is the only file that allows #ifdef to support and hide
 * platform differences. This file is included in main.h which is included
 * in all generic c++ files, and this is the only place that should include
 * platform specific driver files.
 *
 * STM32 Note: This implementation uses CoOS as RTOS to execute threads that
 * run Celina cycles. The user can add both tasks, threads and cycles in the
 * application design to create a 2D RTOS.
 *
 * Module : Celina HAL
 ****************************************************************************/
#ifndef _PLATFORM
#define _PLATFORM

// STM32 M3 specific includes


extern "C"
{
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_rtc.h"
	#include "stm32f10x_gpio.h"
	#include "stm32F10x_usart.h"
	#include "misc.h"
	#include "stm32f10x_adc.h"
	#include "stm32f10x_bkp.h"
	#include "stm32f10x_can.h"
	#include "stm32f10x_crc.h"
	#include "stm32f10x_dbgmcu.h"
	#include "stm32f10x_dma.h"
	#include "stm32f10x_exti.h"
	#include "stm32f10x_flash.h"
	#include "stm32f10x_i2c.h"
	#include "stm32f10x_iwdg.h"
	#include "stm32f10x_pwr.h"
	#include "stm32f10x_spi.h"
	#include "stm32f10x_tim.h"
	#include "stm32f10x_wwdg.h"

	#ifdef CELINA_STM32M3
		#include <CoOS.h>
	#endif
	#include "fifo.h"
}

#include "mutex.h"

#include "SysTick.h"
#include "rtc.h"
#include "etc.h"
#include "gpio.h"

extern "C"
{
	void SerialInit(uint32_t baudrate,uint16_t parity,uint16_t databits, uint16_t stopbits, uint16_t flowcontrol);

	void SerialPutc(char c);
}

#include "rs485.h"
#include "Celina.h"

using namespace hal;

#endif

