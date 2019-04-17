# CUDA下四重循环两种并行方式效率分析 - 战斗蜗牛的专栏 - CSDN博客





2012年11月09日 21:54:40[vbskj](https://me.csdn.net/vbskj)阅读数：5401








## 1 问题描述

**原代码**：for(int m=0;m<M;m++)

                     for(intn=0;n<N;n++)

                            for(inti=0;i<I;i++)

                                   for(intj=0;j<J;j++)

                                   {

                                          //X条算术运算；（X=6）

                                          B[i][j]= pow(A[i][j],2);

                                          C[ij][]= sqrt(B[i][j]);

                                          ……

                                   }

**两种并行方式**：

1 粗粒度并行：分成X个kernel函数，每个kernel函数里面做两层循环，内层循环并行打开，每个kernel执行一条算数运算。

              Kernek_1：for(int m=0;m<M;m++)

                                   for(intn=0,n<N’n++)

                                   {

                                          B[i][j]= pow(A[i][j],2); //一条算术运算

                                   }

              Kernek_2,……，kernek_X;

2 细粒度并行：一个kernel函数，循环调用M*N次。Kernel里面做X条算术运算。

              For（int m=0;m<M;m++）

                     For(intn=0;n<N;n++)

Kernel：//X条算术运算；（X=6）

                                          B[i][j]= pow(A[i][j],2);

                                          C[ij][]= sqrt(B[i][j]);

                                          ……

注：两种并行方式结果虽然不等价，但不影响本问题分析的重点。

## 2 实验结果

实验平台：GTX 260

取M，N，I，J为256.  时间取10次运行平均值.粗细粒度并行运行时间相差1.7倍。

表1 实验结果(复杂运算pow, sqrt,sin, cos, tan, log; 简单运算+-*/+-)
|时间（ms）|复杂运算|复杂运算快速版|简单运算|
|----|----|----|----|
|粗粒度并行|5206|4463|4609|
|细粒度并行|2996|2515|2661|

## 3 性能分析

粗粒度并行调用kernel6次，细粒度并行调用kernel  256*256=65536次，但是粗粒度并行kernel函数有两次for循环。通过分析ptx文件发现每次for循环需要add/move/step/bra四条指令。因此for循环带来的指令开销影响的程序的性能，而kernel函数调用所带来的开销在此成为次要因素。

## 4 算术运算占用硬件资源

通过分析cubin文件发现：

powf/sqrtf/logf不占用local memory，而三角函数sinf/cosf/tanf使用了28B local memory。

这些函数的快速版本都不占用localmemory，他们的执行开销相当于简单算术运算。

至于复杂运算快速版本比简单运算时间略小，可能因为除法指令时间开销稍大。

## 5 粗粒度并行改进

对粗粒度并行的X个kernel合并为一个kernel，当X较大的时候可能会因为硬件资源限制影响效率，但是本实验室中X取6时合并为一个kernel，效率有一定的提升，这是因为还没有收到硬件资源的限制，比如寄存器不够强制使用local memory等。

对于简单算术运算的一组测试结果：

粗粒度并行    4046ms

细粒度并行    2505ms

开销之比      1.6倍

这是因为for循环所带来的指令开销减少的原因。



