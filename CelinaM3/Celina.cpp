/*****************************************************************************
 * Celina.cpp
 *
 * Celina Real Time Operating System kernal.
 *
 * See documentation for description of Celina usage.
 *
 * STM32F103CB port
 *
 * License
 *
 * Copyright (C) 2012, Jan Berger. All rights reserved.
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
#include "main.h"
	// Celina assumes it is part of an application that contains a "main.h"
	// that includes celina.h and platform specific settings needed

volatile unsigned long Celina::CelinaTickCnt=0;

/*****************************************************************************
 * Constructor
 ****************************************************************************/
Cycle::Cycle()
{
    int x;

    for(x=0; x < CELINA_DATATASKS; x++)
    {
        DataCycle[x].act=0;
    }

    for(x=0; x < CELINA_RTTASKS; x++)
    {
        RTCycle[x].act=0;
    }

    lhigDCycle = 0;
    llowDCycle = 99999;
    laccDCycle = 0;
    lcntDCycle = 0;

    lhigDCycleW = 0;
    llowDCycleW = 99999;
    laccDCycleW = 0;
    lcntDCycleW = 0;

    lstatClear = 0;
    ltaskDCycle = 0;

    rtlhigDCycle = 0;
    rtllowDCycle = 99999;
    rtlaccDCycle = 0;
    rtlcntDCycle = 0;

    rtlhigDCycleW = 0;
    rtllowDCycleW = 99999;
    rtlaccDCycleW = 0;
    rtlcntDCycleW = 0;

    lstatClear = 0;

    lengthDataCycle = 100L;
}

/*****************************************************************************
 * InstallRT
 ****************************************************************************/
void Cycle::InstallRT()
{
}

/*****************************************************************************
 ****************************************************************************/
void Cycle::RemoveRT()
{

}

/*****************************************************************************
 * GetTime
 ****************************************************************************/
unsigned long Cycle::GetTime()
{
	return ETC::millis();
}

/*****************************************************************************
 ****************************************************************************/
void Cycle::SetTime(unsigned long t)
{
 //   lTime = t;
}

/*****************************************************************************
 * IsTaskReady
 ****************************************************************************/
int Cycle::IsTaskReady(Task * task)
{
    unsigned long tNow;

    if(task->act == 0)	/* skip non active task entries		*/
        return 0;

    if(task->signal)    /* signalled tasks are ALWAYS ready */
        return 1;


    switch(task->mode)
    {
    case 0:
        return 1;
        break;
    case 2:
    case 3:
        tNow = GetTime();
        if(tNow >= task->lastrun + task->timer)
            return 1;
        break;
    }
    return 0;
}

/*****************************************************************************
 * RunDataCycle
 ****************************************************************************/
void Cycle::RunDataCycle()
{
    int x;
    u32 cycle;
    u32 tm1,tm2;
   // t1 = ETC::micros();
    tm1=ETC::micros();
u32 tt1,tt2;
    // The next block is the actual datacycle pass through loop
    for(x=0; x < CELINA_DATATASKS; x++)
    {
        if(IsTaskReady(&DataCycle[x]))
        {
            if(DataCycle[x].signal>0)
            {
                //Serial.println("signal");
                DataCycle[x].signal--;
            }
            if(DataCycle[x].mode == CelinaMode_Average)
                DataCycle[x].lastrun += DataCycle[x].timer;
            else
                DataCycle[x].lastrun = GetTime();
//tt1 = ETC::micros();
            DataCycle[x].proc(DataCycle[x].p1);
//tt2 = ETC::micros();
//if(tt2-tt1 > 1000)
//{
//	printf("Task %d is long\n",x);
//}
        }
    }

    // CPU statistics.
    //t2=ETC::micros();
    tm2=ETC::micros();
    if(tm2-tm1 > 1000)
    {
      	printf("Cycle is long %d %d\n",(unsigned int)tm1,(unsigned int)tm2);
    }

    // CPU statistics. Used to measure actual DC processing
    #ifdef CELINA_STATISTICS
        #ifdef CELINA_CPUSTATS
            if(statType == CelinaStat_CPU)
            {
                if(ltaskDCycle != 0)
                {
                    if(tm2 >= lstatClear + 1000000)
                    {
                        lhigDCycle = lhigDCycleW;
                        llowDCycle = llowDCycleW;
                        laccDCycle = laccDCycleW;
                        lcntDCycle = lcntDCycleW;

                        lhigDCycleW = 0;
                        llowDCycleW = 99999999;
                        laccDCycleW = 0;
                        lcntDCycleW = 0;

                        DataCycle[ltaskDCycle-1].proc(DataCycle[ltaskDCycle-1].p1);
                        lstatClear = tm2;
                    }
                   // printf("c=%u\n",cycle);
                    cycle = tm2-tm1;
                    if(tm2>tm1)
                    {
                    laccDCycleW += cycle;
                    lcntDCycleW ++;
                    if(cycle < llowDCycleW)
                        llowDCycleW=cycle;
                    if(cycle > lhigDCycleW)
                        lhigDCycleW=cycle;
                    }
                }
            }
        #endif
    #endif // CELINA STATISTICS

    // Idle task processing. The user have defined the length
    // of datacycle, so we will wait until this time is passed
    // and use this time to process Idle tasks, if any.
	#ifdef CELINA_IDLETIME
		static int idleIX=0;
		x=0;
		while(tm2-tm1 < (lengthDataCycle*1000))
		{
			for(; x < CELINA_DATATASKS && tm2-tm1 < (lengthDataCycle*1000); x++)
			{
				if(DataCycle[idleIX].mode == CelinaMode_Idle)
				{
					DataCycle[idleIX].proc(DataCycle[idleIX].p1);
					t2=GetTime();
					tm2=GetTime();
				}
				idleIX++;
				if(idleIX>=CELINA_DATATASKS)
					idleIX=0;
			}
			t2=GetTime();
			tm2=GetTime();
		}
	#endif

    // Cycle test statistics. Measure the accuracy on the datacycle
    #ifdef CELINA_STATISTICS
        #ifndef CELINA_CPUSTATS
            if(statType == CelinaStat_Cycle)
            {
                tm2=micros();
                // Statistics are computed at the end of a data cycle.
                if(ltaskDCycle != 0)
                {
                    if(tm2 > lstatClear + 1000)
                    {
                        lhigDCycle = lhigDCycleW;
                        llowDCycle = llowDCycleW;
                        laccDCycle = laccDCycleW;
                        lcntDCycle = lcntDCycleW;

                        lhigDCycleW = 0;
                        llowDCycleW = 99999;
                        laccDCycleW = 0;
                        lcntDCycleW = 0;
                        SignalTask(ltaskDCycle,0);
             //           Serial.println(ltaskDCycle);
                        lstatClear = tm2;
                    }
                    cycle = tm2-tm1;
                    laccDCycleW += cycle;
                    lcntDCycleW ++;
                    if(cycle < llowDCycleW)
                        llowDCycleW=cycle;
                    if(cycle > lhigDCycleW)
                        lhigDCycleW=cycle;
                }
            }
        #endif
    #endif
}

/*****************************************************************************
 * RunRTCycle
 ****************************************************************************/
void Cycle::RunRTCycle()
{
    int x;    unsigned long t1,t2,cycle;
    unsigned long tm1,tm2;
    t1 = GetTime();
    tm1=GetTime();

    if(critical==1)
        return;
    for(x=0; x < CELINA_RTTASKS; x++)
    {
        if(IsTaskReady(&RTCycle[x]))
        {
            RTCycle[x].signal=0;
            if(RTCycle[x].mode == CelinaMode_Average)
                RTCycle[x].lastrun += RTCycle[x].timer;
            else
                RTCycle[x].lastrun = GetTime();
            RTCycle[x].proc(RTCycle[x].p1);
        }
    }

	#ifdef CELINA_STATISTICS
		#ifdef CELINA_RTSTATS
			t2=GetTime();
			tm2=micros();

			if(statType == CelinaStat_CPU)
			{
				if(ltaskDCycle != 0)
				{
					if(tm2 > rtlstatClear + 1000000)
					{
						rtlhigDCycle = rtlhigDCycleW;
						rtllowDCycle = rtllowDCycleW;
						rtlaccDCycle = rtlaccDCycleW;
						rtlcntDCycle = rtlcntDCycleW;

						rtlhigDCycleW = 0;
						rtllowDCycleW = 999999999;
						rtlaccDCycleW = 0;
						rtlcntDCycleW = 0;
						rtlstatClear = tm2;
					}
					cycle = tm2-tm1;
					rtlaccDCycleW += cycle;
					rtlcntDCycleW ++;
					if(cycle < rtllowDCycleW)
						rtllowDCycleW=cycle;
					if(cycle > rtlhigDCycleW)
						rtlhigDCycleW=cycle;
				}
			}
		#endif
	#endif
}

/*****************************************************************************
 * Run
 ****************************************************************************/
void Cycle::Run()
{
	bTerminate = false;
	InstallRT();
    while(bTerminate == false)
        RunDataCycle();
    RemoveRT();
}

/*****************************************************************************
 * AddTask
 ****************************************************************************/
int Cycle::AddTask(int (*task)(long),
                    long p1,
                    int cycle,
                    char mode,
                    unsigned long timer)
{
    int x;

    if(cycle >= Celina_RTCycle)
    {
        for(x=0; x < CELINA_RTTASKS; x++)
        {
            if(RTCycle[x].act==0)
            {
                RTCycle[x].lastrun   = GetTime();
                RTCycle[x].mode      = mode;
                RTCycle[x].p1        = p1;
                RTCycle[x].proc      = task;
                RTCycle[x].signal    = 1;
                RTCycle[x].timer     = timer;
                RTCycle[x].act       = 1;
                return (x+1) | 0x1000;
            }
        }
    }
	else
    {
        for(x=0; x < CELINA_DATATASKS; x++)
        {
            if(DataCycle[x].act==0)
            {
                DataCycle[x].lastrun   = GetTime();
                DataCycle[x].mode      = mode;
                DataCycle[x].p1        = p1;
                DataCycle[x].proc      = task;
                DataCycle[x].signal    = 1;
                DataCycle[x].timer     = timer;
                DataCycle[x].act       = 1;
                if(mode == CelinaMode_Statistics)
                    ltaskDCycle=x+1;
                return x+1;
            }
        }
    }

    return -1;
}

/*****************************************************************************
 * SignalTask
 ****************************************************************************/
int Cycle::SignalTask(int task, unsigned char increment)
{
    int x = (task & 0x0fff)-1; /* and out rt flag */
    if(task & 0x1000)
    {
        if( x < CELINA_RTTASKS)
        {
			if(increment>0 || (increment ==0 && RTCycle[x].signal == 0))
			{
				RTCycle[x].signal ++;
				return 1;
			}
        }
    }
    else
    {
        if( x < CELINA_DATATASKS)
        {
			if(increment>0 || (increment ==0 && DataCycle[x].signal == 0))
			{
				DataCycle[x].signal ++;
				return 1;
			}
        }
    }
    return 0;
}

/*****************************************************************************
 ****************************************************************************/
void Cycle::SetStatType(int st)
{
    statType=(unsigned char)st;
}

/*****************************************************************************
 * GetDCStats
 ****************************************************************************/
void Cycle::GetStats(u32 *low, u32 *high, u32 *avg,
		u32 *rtlow, u32 *rthigh, u32 *rtavg)
{
    *high   = lhigDCycle;
    *low    = llowDCycle;
    *avg    = (u32)((double)laccDCycle / (double)lcntDCycle);

    *rthigh   = rtlhigDCycle;
    *rtlow    = rtllowDCycle;
    *rtavg    = (u32)((double)rtlaccDCycle / (double)rtlcntDCycle);
}

/*****************************************************************************
 * SetDCStatTask
 ****************************************************************************/
void Cycle::SetDCStatTask(int task)
{
    ltaskDCycle = task;
}
#define CFG_CPU_FREQ 	72000000
#define CFG_SYSTICK_FREQ 100
#define NVIC_ST_CTRL    (*((volatile u32 *)0xE000E010))
#define NVIC_ST_RELOAD  (*((volatile u32 *)0xE000E014))
#define RELOAD_VAL      ((u32)(( (u32)CFG_CPU_FREQ) / (u32)CFG_SYSTICK_FREQ) -1)

/*****************************************************************************
 * Init
 ****************************************************************************/
void Cycle::Init()
{
	#ifdef CELINA_STM32M3
		//CoInitOS();
		NVIC_ST_RELOAD =  RELOAD_VAL;
	    NVIC_ST_CTRL   =  0x0007;
	#endif
}
extern "C"
{
void SysTick_Handler(void)
{
    CelinaTickCnt++;                    /* Increment systerm time.                */
}
}
/*****************************************************************************
 * Exit
 ****************************************************************************/
void Cycle::Exit()
{
	bTerminate = true;
}
