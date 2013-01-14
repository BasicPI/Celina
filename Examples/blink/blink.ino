#include <Celina.h>

void setup()
{
   Serial.begin(9600);
}

int LedOn(long led)
{
  analogWrite((int)led,255);
}

int LedOff(long led)
{
  analogWrite((int)led,0);
  //delay(1);
}

int Stat(long l)
{
  long low,high,avg;
  long rtlow,rthigh,rtavg;
  Celina::GetStats(&low, &high, &avg,&rtlow, &rthigh, &rtavg);
  Serial.print("Statistics low: ");
  Serial.print(low);
  Serial.print(" avg: ");
  Serial.print(avg);
  Serial.print(" high: ");
  Serial.println(high);
}

void loop()
{
  Celina rtos;
  Serial.println("Starting Celina...");
  rtos.AddTask(LedOn, 10L, Celina_DataCycle, CelinaMode_Always, 32L);

  rtos.AddTask(LedOn, 2L, Celina_DataCycle, CelinaMode_Average, 50L);
  rtos.AddTask(LedOff, 2L, Celina_DataCycle, CelinaMode_Average, 200L);

  rtos.AddTask(LedOn, 3L, Celina_DataCycle, CelinaMode_Average, 50L);
  rtos.AddTask(LedOff, 3L, Celina_DataCycle, CelinaMode_Average, 200L);

  rtos.AddTask(LedOn, 4L, Celina_DataCycle, CelinaMode_Average, 50L);
  rtos.AddTask(LedOff, 4L, Celina_DataCycle, CelinaMode_Average, 400L);

  rtos.AddTask(LedOn, 5L, Celina_DataCycle, CelinaMode_Average, 50L);
  rtos.AddTask(LedOff, 5L, Celina_DataCycle, CelinaMode_Average, 800L);
  
  rtos.AddTask(LedOn, 6L, Celina_DataCycle, CelinaMode_Idle, 100L);
  rtos.AddTask(LedOff, 6L, Celina_DataCycle, CelinaMode_Interval, 800L);

  rtos.AddTask(LedOn, 7L, Celina_RTCycle, CelinaMode_Average, 32L);
  rtos.AddTask(LedOff, 7L, Celina_RTCycle, CelinaMode_Average, 64L);

  rtos.AddTask(LedOn, 8L, Celina_RTCycle, CelinaMode_Average, 32L);
  rtos.AddTask(LedOff, 8L, Celina_RTCycle, CelinaMode_Average, 64L);

  rtos.AddTask(LedOn, 9L, Celina_RTCycle, CelinaMode_Average, 32L);
  rtos.AddTask(LedOff, 9L, Celina_DataCycle, CelinaMode_Average, 64L);

  rtos.AddTask(LedOff, 10L, Celina_DataCycle, CelinaMode_Always, 32L);
  
  rtos.AddTask(Stat, 10L, Celina_DataCycle, CelinaMode_Statistics, 32L);
  rtos.SetStatType(CelinaStat_CPU);
  rtos.Run();
}


