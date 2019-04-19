# linux下获取系统时间的方法 - xqhrs232的专栏 - CSDN博客
2011年04月26日 14:36:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：946
原文地址::[http://blog.csdn.net/longyinggreat/archive/2009/07/31/4396553.aspx](http://blog.csdn.net/longyinggreat/archive/2009/07/31/4396553.aspx)
可以用 localtime 函数分别获取年月日时分秒的数值。
Linux下获得系统时间的C语言的实现方法：
1. 可以用 localtime 函数分别获取年月日时分秒的数值。
  #include<time.h>     //C语言的头文件  
  #include<stdio.h>     //C语言的I/O  
  void   main()  
  {  
  time_t   now;         //实例化time_t结构  
  struct   tm     *timenow;         //实例化tm结构指针  
  time(&now);  
  //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now  
  timenow   =   localtime(&now);  
  //localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)  
  printf("Local   time   is   %s/n",asctime(timenow));  
  //上句中asctime函数把时间转换成字符，通过printf()函数输出  
  }  
  注释：time_t是一个在time.h中定义好的结构体。而tm结构体的原形如下：  
  struct   tm  
  {  
      int   tm_sec;//seconds   0-61  
      int   tm_min;//minutes   1-59  
      int   tm_hour;//hours   0-23  
      int   tm_mday;//day   of   the   month   1-31  
      int   tm_mon;//months   since   jan   0-11  
      int   tm_year;//years   from   1900  
      int   tm_wday;//days   since   Sunday,   0-6  
      int   tm_yday;//days   since   Jan   1,   0-365  
      int   tm_isdst;//Daylight   Saving   time   indicator  
  };  
 2. 对某些需要较高精准度的需求，Linux提供了gettimeofday()。  
#include   <stdio.h>   
#include   <stdlib.h>                                                
#include   <sys/time.h>   
int  main(int argc,   char **argv)   
{  
  struct   tim   start,stop,diff;  
  gettimeofday(&start,0);  
                    //做你要做的事...  
  gettimeofday(&stop,0);  
  tim_subtract(&diff,&start,&stop);   
  printf("总计用时:%d毫秒/n",diff.tv_usec);   
}                    
int tim_subtract(struct tim *result, struct tim *x, struct tim *y)   
{   
   int nsec;   
   if ( x->tv_sec > y->tv_sec )   
      return   -1;   
   if ((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))   
      return   -1;   
   result->tv_sec = ( y->tv_sec-x->tv_sec );   
   result->tv_usec = ( y->tv_usec-x->tv_usec );   
   if (result->tv_usec<0)   
   {   
      result->tv_sec--;   
      result->tv_usec+=1000000;   
    }   
    return   0;   
}  
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/longyinggreat/archive/2009/07/31/4396553.aspx](http://blog.csdn.net/longyinggreat/archive/2009/07/31/4396553.aspx)
