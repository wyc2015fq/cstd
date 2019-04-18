# Matlab中计算程序运行时间的三种方法 - 心纯净，行致远 - CSDN博客





2018年05月07日 16:15:54[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：468








注意：三种方法由于使用原理不一样，得到结果可能有一定的差距！

1、tic和toc组合(使用最多的)


计算tic和toc之间那段程序之间的运行时间，它的经典格式为





**[plain]**[view plain](https://blog.csdn.net/u010555688/article/details/24556591#)[copy](https://blog.csdn.net/u010555688/article/details/24556591#)

- tic  
- 。。。。。。。。。。  
- toc  


换句话说程序，程序遇到tic时Matlab自动开始计时，运行到toc时自动计算此时与最近一次tic之间的时间。这个有点拗口，下面我们举个例子说明





**[plain]**[view plain](https://blog.csdn.net/u010555688/article/details/24556591#)[copy](https://blog.csdn.net/u010555688/article/details/24556591#)

- % by dynamic of Matlab技术论坛  
- % see also http://www.matlabsky.com  
- % contact me matlabsky@gmail.com  
- % 2009-08-18 12:08:47   
- clc  
- tic;%tic1  
- t1=clock;  
- for i=1:3  
-     tic ;%tic2  
-     t2=clock;  
-     pause(3*rand)  
-     %计算到上一次遇到tic的时间，换句话说就是每次循环的时间  
-     disp(['toc计算第',num2str(i),'次循环运行时间：',num2str(toc)]);  
-     %计算每次循环的时间  
-     disp(['etime计算第',num2str(i),'次循环运行时间：',num2str(etime(clock,t2))]);  
-     %计算程序总共的运行时间  
-     disp(['etime计算程序从开始到现在运行的时间:',num2str(etime(clock,t1))]);  
-     disp('======================================')  
- end  
- %计算此时到tic2的时间，由于最后一次遇到tic是在for循环的i=3时，所以计算的是最后一次循环的时间  
- disp(['toc计算最后一次循环运行时间',num2str(toc)])  
- disp(['etime程序总运行时间：',num2str(etime(clock,t1))]);  


运行结果如下，大家可以自己分析下





**[plain]**[view plain](https://blog.csdn.net/u010555688/article/details/24556591#)[copy](https://blog.csdn.net/u010555688/article/details/24556591#)

- toc计算第1次循环运行时间：2.5628  
- etime计算第1次循环运行时间：2.562  
- etime计算程序从开始到现在运行的时间:2.562  
- ======================================  
- toc计算第2次循环运行时间：2.8108  
- etime计算第2次循环运行时间：2.813  
- etime计算程序从开始到现在运行的时间:5.375  
- ======================================  
- toc计算第3次循环运行时间：2.0462  
- etime计算第3次循环运行时间：2.046  
- etime计算程序从开始到现在运行的时间:7.421  
- ======================================  
- toc计算最后一次循环运行时间2.0479  
- etime程序总运行时间：7.421  


2、etime(t1,t2)并和clock配合

来计算t1，t2之间的时间差，它是通过调用windows系统的时钟进行时间差计算得到运行时间的，应用的形式





**[plain]**[view plain](https://blog.csdn.net/u010555688/article/details/24556591#)[copy](https://blog.csdn.net/u010555688/article/details/24556591#)

- t1=clock;  
- 。。。。。。。。。。。  
- t2=clock;  
- etime(t2,t1)  


3、cputime函数来完成

使用方法和etime相似，只是这个是使用cpu的主频计算的，和前面原理不同，使用格式如下





**[plain]**[view plain](https://blog.csdn.net/u010555688/article/details/24556591#)[copy](https://blog.csdn.net/u010555688/article/details/24556591#)

- t0=cputime  
- 。。。。。。。。。。。。。  
- t1=cputime-t0  


Matlab官方推荐使用tic/toc组合，When timing the duration of an event, use the tic and toc functions instead of clock or etime.

至于大家可以根据自己的喜好自己选择，但是使用tic/toc的时候一定要注意，toc计算的是与最后一次运行的tic之间的时间，不是第一个tic，更不是第二个。。。。。




转自：http://www.matlabsky.com/thread-2607-1-1.html





