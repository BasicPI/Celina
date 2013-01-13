/*****************************************************************************
 * Celina.h
 *
 * Celina Real Time Operating System kernal.
 *
 * See documentation for description of Celina usage.
 *
 * Created 5.jul.2001/JVB as a C library, rewritten as C++ under the name
 * Celina 2.dec.2012
 *
 * License
 *
 * Copyright (c) 2012, Jan Berger. All rights reserved.
 * email:janvb@live.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:*
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
#ifndef _Celina
#define _Celina

#define CELINA_RTTASKS 5
#define CELINA_DATATASKS 20

#define Celina_DataCycle 0
#define Celina_RTCycle 1

/*****************************************************************************
 * Celina contains statistics for DataCycle and RTCycle that are usefully for
 * testing. Uncommenting the defines below will include statistics in the
 * compilation.
 ****************************************************************************/
#define CELINA_STATISTICS 1
#define CELINA_CPUSTATS 1

class Celina;

/*****************************************************************************
 * Task
 *
 * Task entry used in schedule tables. Hold info needed to execute and
 * schedule tasks.
 ****************************************************************************/
class Task
{
    protected:
        unsigned char act:1;
            // indicate if entry is active

        unsigned char signal:1;
            // indicate if task is signaled or not. Act as a counter and will
            // be called once for each increment allowing the signal to be
            // a queue counter. This mode is always available as any task can
            // be signalled.

        int (*proc)(long);
            // ptr to task

        long p1;
            // parameter to pass to task

        unsigned char mode;
            // scheduling scheme
            #define CelinaMode_Always 0
            #define CelinaMode_Signalled 1
            #define CelinaMode_Interval 2
            #define CelinaMode_Average 3
            #define CelinaMode_Watchdog 4
            #define CelinaMode_Statistics 5
            #define CelinaMode_Idle 6


        unsigned long timer;
            // timer for mode 2 and 3

        unsigned long lastrun;
            // time last runned.

friend class Celina;

};

/*****************************************************************************
 * Celina
 *
 * Celina core with utilities and scheduler. You should only declare one
 * instance of this class normally.The default design is usage of two cycles,
 * DataCycle and RTCycle. These are defined as static tables, so you need to
 * estimate the number of tasks you need. A typical good design is to run
 * very critical I/O jobs on the RTCycle, use them to process buffers and
 * signal a data-cycle task to do the actual work.
 ****************************************************************************/
class Celina
{
    protected:
        static Task DataCycle[CELINA_DATATASKS];
            // Data cycle task array

        static Task RTCycle[CELINA_RTTASKS];
            // RT cycle task array

        volatile bool bTerminate;
            // flag set if Celina is to terminate.

        static long rtlhigDCycle;
        static long rtllowDCycle;
        static long rtlaccDCycle;
        static long rtlcntDCycle;

        /* running work set */
        static long rtlhigDCycleW;
        static long rtllowDCycleW;
        static long rtlaccDCycleW;
        static long rtlcntDCycleW;

        static long rtlstatClear;

        static long lhigDCycle;
        static long llowDCycle;
        static long laccDCycle;
        static long lcntDCycle;

        /* running work set */
        static long lhigDCycleW;
        static long llowDCycleW;
        static long laccDCycleW;
        static long lcntDCycleW;

        static long lstatClear;
        static int ltaskDCycle;
        static unsigned char statType;
            #define CelinaStat_CPU 0
            #define CelinaStat_Cycle 1

        static volatile unsigned char critical;

        unsigned long lengthDataCycle;

 //       static long test;

    public:
        Celina();
            // constructor

        static unsigned long GetTime();
            // Get time in ms. Can be elapsed time since start

        static void SetTime(unsigned long tm);
            // Set time in ms

        void RunDataCycle();
            // Will perform a single loop through DataCycle tasks. Should be called from
            // Run().

        static void RunRTCycle();
            // Will perform a single loop through RTCycle tasks. Should be called from an
            // timer operated interrupt on regular basis.

        void Run();
            // Run Celina. Will install RTCycle and start executing DataCycle

        int AddTask(int (*task)(long),long p1,int cycle,char mode, unsigned long timer);
            // Add a task to either DataCycle or RT cycle.
            //       task        ptr to task function. Must be declared as
            //
            //                      int task(long p1)
            //
            //       p1          4 byte parameter to use as "p1" when calling the task.
            //
            //       cycle       DataCycle or RTCycle
            //
            //       mode        Scheduling scheme.
            //
            //                      CelinaMode_Always 0
            //                      CelinaMode_Signalled 1
            //                      CelinaMode_Interval 2
            //                      CelinaMode_Average 3
            //                      CelinaMode_Watchdog 4
            //                      CelinaMode_Statistics 5
            //                      CelinaMode_Idle 6
            //
            //       timer       Elapsed time in ms for mode 2 & 3 schemes.
            //
            //  Return Value: Task handle. -1= failure to add task. Data Cycle task return
            //                task entry + 1. RT tasks will in addition have 0x1000 bit
            //                set in the handle to distinguish from dc handles.

        static int SignalTask(int task, unsigned char increment);
            // signal a task. Will increment the signal counter for the task. This
            // will cause the task to be executed in the next cycle and decrease
            // the signal counter by 1.

        static void GetStats(long *low, long *high, long *avg,long *rtlow, long *rthigh, long *rtavg);
            // helper function to retrieve the last computed stats.

        void SetDCStatTask(int task);
            // Set a task that will be signalled when a new set of data
            // cycle statistics have been calculated.

        void SetStatType(int stat);

        void EnterCriticalSection()
            // Prevent RTCycle from running. This is a simple mutex form that
            // stop RTCycle until LeaveCriticalSection is called. This might cause
            // RTCycle to skip a turn, so use with care.
            {
                critical=1;
            }

        void LeaveCriticalSection()
            // Re-enable RTCycle.
            {
                critical=0;
            }

        void Exit();
            // Terminate Celina

    protected:
        static int ExecuteTask(Task * task);

        static void InstallRT();
            // Start RT Cycle

        static void RemoveRT();
            // Stop RT Cycle
};

#endif

