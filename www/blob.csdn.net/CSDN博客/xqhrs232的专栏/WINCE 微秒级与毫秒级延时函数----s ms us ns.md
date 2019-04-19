# WINCE 微秒级与毫秒级延时函数----s/ms/us/ns - xqhrs232的专栏 - CSDN博客
2009年10月30日 10:49:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2147
 //WINCE 微秒级延时函数 
void delay_us(int n)
{ 
  LARGE_INTEGER litmp; 
  LONGLONG QPart1,QPart2; 
  double dfMinus, dfFreq, dfTim; 
  if(QueryPerformanceFrequency(&litmp)==FALSE) 
  { 
       MessageBox(NULL,TEXT("Error：QueryPerformanceFrequency"),TEXT("Error"),MB_OK);
       return;
   } 
   dfFreq = (double)litmp.QuadPart; 
   QueryPerformanceCounter(&litmp); 
   QPart1 = litmp.QuadPart; 
  do 
  { 
   QueryPerformanceCounter(&litmp); 
   QPart2=litmp.QuadPart; 
   dfMinus=(double)(QPart2-QPart1); 
   dfTim=dfMinus/dfFreq; 
   }while(dfTim <0.000001*n); 
} 
//WINCE毫秒级延时函数++++Sleep( )++++Timer定时器使用
void   delay_ms(DWORD   tmp_time)   
{   
 DWORD   start;   
 DWORD   time_i=0;   
 start=GetTickCount(); 
 while(time_i<=tmp_time)   
  {   
     time_i=GetTickCount()-start;   
  }   
}
