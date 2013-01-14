Celina
======

RTOS for Arduino ported by Jan Berger

DESCRIPTION
===========

Celina's scheduler is designed to deal with low RAM situations. A standard threaded 
scheduler will reuire a stack per thread. This is not a realistic aproach whenn 
you only have 2k SRAMTo deal with this I introduced a simple scheme of using two 
cycles. Within each cycle you schedule tasks in sequence. The main cycle "datacycle" 
run in loop directly. The second task "RTCycle" run's on the watchdog timer with a 
resolution of 16ms. The way this works is that DataCycle run in a ever-loop on timing, 
it get interrupted by the Watchdog timer that execute RTCycle and return to DataCycle. The 
advantage is that DataCycle and RTCycle uses the same stack.

INSTALLATION
============

Installing Celina is straight forward:
 
1. In your arduino folder you will find a "libraries" folder containing other libraries. Create a folder "Celina" here with the files Celina.h, Celina.cpp - if you just sync git here it should do so.
 
2. restart your Arduino IDE
 
3. Celina should now be available in the IDE Schetch -> Import Library

FEATURES
============

- Multi-threading
- Linear scheduler
- Uses ca 200 SRAM for 10 tasks 
- Decent code footprint - ca 3k added to project.
- several timing schemes
- signalled schemes
- SIL-3 support
- Watchdog used as ticker.
 
CODE EXAMPLE
============

 #include "Celina.h"
 
 int task01(long p1)
 {
   ...
 }
 
 loop()
 {
   Celina rtos();
 
   rtos.AddTask(task01, ...);
   ...
 
   rtos.Run();
 }
