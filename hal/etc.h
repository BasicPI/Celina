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
