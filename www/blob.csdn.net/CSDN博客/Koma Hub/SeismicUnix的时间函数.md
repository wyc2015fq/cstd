# SeismicUnix的时间函数 - Koma Hub - CSDN博客
2018年12月18日 21:37:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：36
```cpp
#include <sys/time.h>
#include <sys/resource.h>
float 
cpusec()
{
	struct rusage rusage;
	getrusage(RUSAGE_SELF,&rusage);
	return ((float)((double)(rusage.ru_utime.tv_sec)+
		1.0e-6*(double)(rusage.ru_utime.tv_usec)));
}
```
```cpp
#include <time.h>
#ifndef CLK_TCK
#define CLK_TCK	64
#endif
#if (_IBMR2||__hp9000s800)
#define CLOCK_UNIT	1000000
#else
#define CLOCK_UNIT	CLK_TCK
#endif
float
cputime()
{
	return clock() / (float) CLOCK_UNIT;
}
```
```cpp
#ifdef __GNUC__
#	include <time.h>
#else
#	include <sys/time.h>
#endif
float
wallsec()
{
#ifdef __GNUC__
	static int    firsttime = 1;
	static time_t firstsec     ;
	if ( firsttime ) {
		firsttime = 0;
		firstsec  = time ((time_t *)NULL);
		return (0.0);
	} else
		return ((float)(time((time_t *)NULL) - firstsec));
#else
	struct timeval tp;
	struct timezone tzp;
	static int firsttime=1;
	static long firstsec,firstusec;
	long sec,usec;
	gettimeofday(&tp,&tzp);
	if (firsttime) {
		firsttime=0;
		firstsec = tp.tv_sec;
		firstusec = tp.tv_usec;
		return(0.0);
	} else {
		sec = tp.tv_sec-firstsec;
		usec = tp.tv_usec-firstusec;
		return((float)sec+1.0e-6*(float)usec);
	}
#endif
}
```
```cpp
#include <time.h>
float
walltime()
{
	static int firsttime = 1;	/* First entry?                 */
	static time_t lasttime;		/* Save return for next entry   */
	time_t t1, t2;			/* Resp. last and current times */
	if (firsttime) {
		firsttime = 0;
		lasttime = time(&t1);
		return 0.0;
	} else {
		t1 = lasttime;
		lasttime = time(&t2);
		return (float) difftime(t2, t1);
	}
}
```
