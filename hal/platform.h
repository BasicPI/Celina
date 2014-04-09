/*****************************************************************************
 * Platform.h
 *
 * Platform.h is the only file that should contain #ifdef to support and hide
 * platform differences. This file is included from main.h.
 *
 * Platform.h include all other headers and should be included in your
 * applications main.h
 *
 * All code will include a main.h so the path for this must exist. This
 * allows the application to control all aspects of code as it should on
 * an embedded system,
 *
 * STM32 Note: This implementation uses CoOS as RTOS to execute threads that
 * run Celina cycles. The user can add both tasks, threads and cycles in the
 * application design to create a 2D RTOS.
 *
 * Module : Celina HAL
 *
 * License
 *
 * Copyright (c) 2012, Jan Berger. All rights reserved.
 * email:janvb@live.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *	  this list of conditions and the following disclaimer.
 *
 *  * Neither the name of the authors nor the names of its contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/
#ifndef _PLATFORM
#define _PLATFORM

// Platform selections. The following defines allow you to select platform.
// This can be selected here, or even better defined in your main.h

// #define CELINA_ARDUINO_328
// #define CELINA_ARDUINO_2560
// #define CELINA_STM32M3

#include "TypeDef.h"
#include "HAL.h"

// includes
extern "C"
{
	#ifdef CELINA_STM32M3
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

		// Temporary fix on name conflict between STM32 drivers and HAL.
		#undef CRC
		#undef DAC
		#undef DMA
//		#include <CoOS.h>
	#endif
	#include "FIFO.h"
}

extern "C"
{
	void SerialInit(uint32_t baudrate,uint16_t parity,uint16_t databits, uint16_t stopbits, uint16_t flowcontrol);

	void SerialPutc(char c);
}

#include "Celina.h"
#include "ETC.h"

#include "ADC.h"
#include "Backup.h"
#include "CAN.h"
#include "CRC.h"
#include "DAC.h"
#include "DMA.h"
#include "Ethernet.h"
#include "FIFO.h"
#include "Flash.h"
#include "GPIO.h"
#include "I2C.h"
#include "Mutex.h"
#include "Power.h"
#include "PWM.h"

#include "RS485.h"

#include "RTC.h"
#include "SD.h"
#include "SysTick.h"
#include "TCP.h"
#include "Timer.h"
#include "UDP.h"


using namespace hal;
using namespace Celina;

#endif

