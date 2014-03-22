/*****************************************************************************
 * etc.h
 *
 * Elapsed Time Clock is a high resolution clock that start at 0 when the
 * computer starts. The actual implementation is usually by using some timer
 * to implement a concept of elapsed time, meaning the hardware will be
 * very application/platform specific.
 *
 * STM32 Note. Uses CoOS timer with SysTick running at 72Mhz. The actual call
 * to millis or micros takes ca 1 ys "as is". Note that this implementation
 * makes assumptions about systick frequency and CoOS tick frequency.
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
namespace hal
{
	class ETC
	{
	public:
		/*****************************************************************************
		 * millis
		 *
		 * Return a 23 bit unsigned elapsed millisecond counter.
		 ****************************************************************************/
		static u32 millis()
		{
			unsigned int tick;
			unsigned int l;
			tick=CoGetOSTime();
			l=SysTick->VAL;
			return (tick*10) + (720000-l)/72000;
		}

		/*****************************************************************************
		 * micros
		 *
		 * Return time in microseconds on a 32 bit unsigned integer that will wrap.
		 ****************************************************************************/
		static u32 micros()
		{
			unsigned int tick;
			unsigned int l;
			tick=CoGetOSTime();
			l=SysTick->VAL;
			return (tick*10000) + (720000-l)/72;
		}

		/*****************************************************************************
		 * sleep
		 *
		 * Suspend processing for n ms.
		 ****************************************************************************/
		static void sleep(unsigned int ms)
		{
			bool bWait=true;
			u32 t1,t2,delta;
			t1 = millis();
			while(bWait)
			{
				t2 = millis();
				if(t2>t1)
					delta =t2-t1+1;
				else if(t1 < t2)
					delta = t2 + 0xffffffff-t1;
				else {
					delta =0;
				}
				if(delta >ms)
					bWait=false;
			}
		}

	};
}
