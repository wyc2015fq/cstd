# DSP程序开发与优化经验之一：TI多核处理DSP C66XX开发经验 - leegang12的专栏 - CSDN博客
2013年12月19日 23:19:40[leegang12](https://me.csdn.net/leegang12)阅读数：2746
转：
[c66x+ccs5.1 L1P,L1D,L2 cache分析工具](http://blog.csdn.net/zzsfqiuyigui/article/details/7586632)
1.ccs5.1---Tools下的Prolife工具中，提供了L2，L1D的分析
2.对于L1 P \ccsv5\tools\compiler\c6000\doc下面的spru187t，里面有cache layout tools的介绍，这个可以对L1P cache 进行优化,也可以选择cycle approximate simulator, profile tools 有L1P的的分析
MSMC默认配置为L2，根据用户需要能配置成L3。由于配置为L3只是做了地址映射，所以物理上的访问时间应该还是一个数量级的，相差不大。这里的L2和L3的区别应该是指L2只能被L1D和L1P Cache， L3能被L2、L1D和L1P Cache。一般来说，默认使用L2的配置。用户根据自己的应用来决定是否配置为L3。最常见的需要设置MSMC为L3的场景为：需要MSMC内存为non-cacheable，需要将MSMC设置为L3 RAM。
C6678的MSMC负责处理系统中所有的master（包括8个核，以及SMS和SES接口）对MSMC SRAM和DDR3的访问请求。4M字节的MSMC SRAM有4个bank，这些bank是独立的slave，也就是说，如果两个master在同一个clock访问两个不同的bank，则两个访问可以同时完成。如果多个master在同一个clock访问同一个bank，则由MSMC内的仲裁逻辑按优先级处理。DDR3只有一个slave端口，如果多个master在同一个clock访问它，也由MSMC内的仲裁逻辑按优先级处理。MSMC默认配置为L2，根据用户需要能配置成L3。由于配置为L3只是做了地址映射，所以物理上的访问时间应该还是一个数量级的，相差不大。这里的L2和L3的区别应该是指L2只能被L1D和L1P
 Cache， L3能被L2、L1D和L1P Cache。一般来说，默认使用L2的配置。用户根据自己的应用来决定是否配置为L3。最常见的需要设置MSMC为L3的场景为：需要MSMC内存为non-cacheable，需要将MSMC设置为L3 RAM。
DSP开发中，测量某个函数或某段代码的cycles消耗是经常要做的事情，常用的profiling和clock()一般在simulation下使用，真正到了板子上做emulation时，因为要考虑到数据和被测code在板子上的存放位置和读取时间，用这种方法测结果就不那么可靠了。其实在c64x+ core上有两个计数寄存器TSCL/TSCH，它们与CPU同频，共同表示一个64-bit数，CPU运行一个cycle，该寄存器就加1，因此可以用它们来准确的测量cpu在某个执行段中消耗的cycles。一般我们只会用到TSCL这个寄存器，594MHz下，32-bit可以测试到7s，而TSCH是高32位，除非测试整个工程，一般用不到它。
具体使用时：
首先将被测函数通过Link指定到L1P，将用到的数据指定到L1D，这样做的目的是消除代码执行时的数据和指令搬移时间（否则测出来的时间包含数据和指令从片外到片内的搬移时间）
然后在被测函数或代码前对TSCL执行写操作，将寄存器A0写入TSCL，对之做初始化，即启动计数；
最后在函数结束处或被测代码段后读取TSCL寄存器的值，读取的值即为该函数或代码段消耗的cpu cycles。
切记每次测试前必须重启CPU，因为计数器只有在两个条件下才会停止计数,不能通过编程来使之停止计数:
a. 退出reset状态，即重启后 
b. CPU完全powered down 
总的来说，因为这两个寄存器是core内部的寄存器，与cpu同频，用它来测时间，是非常准的，甚至已经考虑到压缩指令包fpread声明的消耗(1cycle)，特别是测试手写汇编时更有效，甚至可以清楚的看出一个指令延时了多少个cycles。
使用方法：长时间宽范围时钟测定
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
方法二，也可以采用biosAPI方式
LgUns time1=CLK_gethtime();
…process code …
LgUns time2=CLK_gethtime();
Cpucycles=(time2-time1)*CLK_cpucyclePerhtime;
Prinf(“#cycle=%d”,Cpucycle);
[http://blog.csdn.net/zzsfqiuyigui/article/details/7599966](http://blog.csdn.net/zzsfqiuyigui/article/details/7599966)
/*-----------------------------------------------------------------------------------------------------------------------------*/
如需详细资料，请联系QQ：1279682290
资料清单：
《1》：
《2》：
《3》：
《4》：
《5》：
/*-------------------------------------------------------------------------------------------------------------------------------*/
