# DSP程序开发与优化经验之二：程序运行时间测量方法总结 - leegang12的专栏 - CSDN博客
2014年02月16日 17:51:33[leegang12](https://me.csdn.net/leegang12)阅读数：3011
如何测试程序在DSP上的运行时间？
1.  TI DSP编译环境CCS3.3，在profiler菜单中，选择Enable Clock和view clock,在view
 clock窗口中可以看到程序运行的时间/周期（时钟）数。
     在view clock窗口中第2行显示的时间就是程序从第1行所在位置到第2行位置的时间，也就是你要测定的时间。
     缺点：时间cycle数据不准确，比实际的要大。如果用profiler得到的数据比你的要求好要少的话，那就说明已经达到了要求。
    克服方法：（1）多次统计；
                      （2）测量之前重新使能 Enable Clock和view clock 。
2.   使用clock
 测试程序运行时间
#include <time.h>
clock_t start,stop,overhead,t;
start=clock();
stop=clock();
overhead=start-stop;
start=clock();
****************************
中间是要测试的代码
****************************
stop=clock();
t=stop-start-overhead;
在profiler中设置Enable Clock以及时钟周期，完了单步运行后就能看见结果。
3.  利用TSCL和TSCH来测量DSP程序运行时间
#include <c6x.h>
#include <stdio.h>
void main()
{
    unsigned long long start;
    unsigned long long end; 
    TSCH=0;//Very important! Enable the registers.
    TSCL=0; //Very important! Enable the registers.
    start = _itoll (TSCH, TSCL);
    //start = TSCL;
    ...
    end = _itoll (TSCH, TSCL);
    //end = TSCL;
    printf("%d DSP-cycles\n",end-start);
}
DSP开发，测量某个函数或某段代码的cycles消耗是经常要做的事情，常用的profiling和clock()一般在simulation下使用。其实在c64x+ core上有两个计数寄存器TSCL/TSCH，它们与CPU同频，共同表示一个64-bit数，CPU运行一个cycle，该寄存器就加1，因此可以用它们来准确的测量cpu在某个执行段中消耗的cycles。一般我们只会用到TSCL这个寄存器，而TSCH是高32位，除非测试整个工程，一般用不到它。
4.   使用方法：长时间宽范围时钟测定
unsigned long long t1，t2;
t1=_itoll(TSCH,TSCL);
code_wait_test;
t2=_itoll(TSCH,TSCL);
printf(“#cycle=%d”,t2-t1);
短时间（7秒）窄范围时钟测定：
T1=TSCL;
…process code …
T2=TSCL;
Printf(“#cycle=%d”,t2-t1);
5. 采用biosAPI方式
    LgUns time1=CLK_gethtime();
    …process code …
   LgUns time2=CLK_gethtime();
   Cpucycles=(time2-time1)*CLK_cpucyclePerhtime;
   Prinf(“#cycle=%d”,Cpucycle);
<如果遇到问题，可以技术交流>
<联系方式：QQ 1279682290>

