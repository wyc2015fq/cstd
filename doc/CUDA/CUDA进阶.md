# CUDA进阶



# 第一篇：CUDA调试

2016年01月05日 15:02:01 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：12215



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/50462325

# 0. 写在前面

> “初学CUDA，好不容易自己写完一段cuda代码，一运行，满屏的语法bug，语法bug还好说，竟然还有逻辑bug，逻辑bug怎么改啊，wtf！！”
>
> “从别人手里接到一段CUDA代码，WTF，为什么还有bug！！还没有注释！！没有文档！！写代码的人怎么不去死啊！！”
>
> 同事的代码出bug了，找你调bug，内心独白：“tmd这写的都是啥”，“tmd这不是我写的代码，出bug为啥要找我”，“tmd这神马破代码，代码注释都没有”

由于CUDA调试工具的不完善、CUDA调试工具上手难度较高，并行思想本身就难调试等因素，CUDA调试一直都是一件很蛋疼的事情。写CUDA也有三四年了，前段时间在群里见别人问CUDA调试的问题，突然有想法写个CUDA调试的博客。自己经验尚浅，希望各位大大看过后能够在评论里指点一二，共同完善这篇博客。 
本博客只针对逻辑bug。

# 1 定位bug

出现bug的第一想法自然是定位bug。cuda比较奇特的地方在于，有时报错bug在500行，但500行出的代码没有错误，而是在1000行的地方逻辑错了，十分头疼。 
下面介绍三种我总结的定位bug方法：

## 1.1 二分法

一半一半的注释代码，定位bug。比较笨拙和麻烦，但是十分好用。

## 1.2 输出定位法

将整体代码分为几个模块，正常的CUDA代码大概可以分为数据初始化，内存申请，内存拷贝，核函数执行，结果拷贝等模块。在每个模块结束后输出标志，示例如图1。这样在调试时就可以根据输出快速定位bug大约在什么位置。如下图：

![输出定位法示例](https://img-blog.csdn.net/20160105142728771)

## 1.3 调试工具

**cuda-gdb** 
对于部分bug，可以用调试工具更快速的定位。 
在linux下，对于API调用出错等问题，cuda gdb可以直接定位在崩溃那一行。 
win下是Nsight，我不熟悉nsight，求大神补充。

**cuda-memcheck** 
而对于核函数内访存类bug，有时候明明越界了，但是运行的时候却没有报错，造成结果结果，而cuda-memcheck可以直接定位这类bug。 
建议在写完代码后，无论有没有bug，先用cuda-memcheck跑一遍。

# 2 解决bug

比较简单的bug，定位后基本看一眼就能解决。但对于复杂的bug，还是比较费劲的。

## 2.1 调试工具

单步调试，打断点。无论是cuda gdb还是Nsight，都可以定位到某一个线程上进行调试，可以说是非常强大。cuda gdb和nsight都有英文官方文档，建议大家都学一学，熟练后调试事半功倍。 
但因为大量线程随机并行执行，有时并不知道该定位到哪个线程上；线程调试不容易控制；定位到单线程调试比较费劲，费时间；教程少（虽然有官方文档），上手难度较大一些。这些都是CUDA调试工具没有被广泛接受的原因。

## 2.2 缩小数据量或线程数并在核函数中打印

大量线程并行是导致CUDA调试难度大的最大原因，尽量的减少并行量是一个非常好的降低调试难度的办法。“小并行”甚至“串行”能够大大方便调试。

在Fermi以后的架构中，可以在核函数中使用printf。 
在合理范围内缩小数据量，进而减少线程数，比如输入图像大小改为16*16。或者修改线程为<<<1,1>>>，printf打印，看是否与预期结果相同。 
祝：如果核函数运行不打印输出结果，加一个设备同步函数cudaDeviceSynchronize()即可。

# 3 预防bug

每一个写CUDAer大概都有花几个小时甚至几天调一个bug的经历。既然bug这么难调，那么预防bug就显得尤其重要了。

## 3.1 写代码前一定要完全构思好架构

社会快速发展，人的心也变得着急了。写CUDA代码之前，一定要沉得住气，多花点时间在纸上构思代码，将代码模块化，哪里容易出问题，哪里该写输出，哪里该检查。

## 3.2 函数返回结果检查

函数返回结果检查能够非常好的定位bug，是基本的编程意识。 
虽然代码可能看起来会比较冗余，示例如下，也可以参考cuda sample里的代码。

![这里写图片描述](https://img-blog.csdn.net/20160105145728383)

![这里写图片描述](https://img-blog.csdn.net/20160105145742177)

## 3.3 函数输入检查

在调用比较重要的函数时，建议用assert检查输入参数与预期值是否相同。

## 3.4 核函数内检查

举个例子，遇到的情况是在拷贝shred memory时，拷贝逻辑比较复杂。此时可以写一个检查函数，以保证拷贝的正确性。 
核函数内代码如下： 
![这里写图片描述](https://img-blog.csdn.net/20160105145950907)

![这里写图片描述](https://img-blog.csdn.net/20160105150002381)

## 3.5 练习肉眼看代码

2年后重新完善这篇博客，想了想还是加一条建议：**练习肉眼看代码的能力**。cuda调试是一件非常耗时间和精力的事情，所以在出错调试之前，不妨先肉眼看几遍代码，在脑子里过几遍，很有可能就能发现代码的bug，并且随着经验的积累，该办法的效率越高。

# 第二篇：巧用PTX

2016年01月12日 18:33:39 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：5855



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/50505885

## 写在前面

并行线程执行（Parallel Thread eXecution，PTX）代码是编译后的GPU代码的一种中间形式，它可以再次编译为原生的GPU微码。CUDA 手册传送门：[Parallel Thread Execution ISA Version 4.3](http://docs.nvidia.com/cuda/parallel-thread-execution/index.html#axzz3x1T0rHkf)

利用PTX来进行试验，我们可以解决一些在写代码时遇到的不确定问题。下面举几个例子：

1. 核函数的参数是直接放到寄存器中么？
2. 一个算法在核函数里面即可以用for来实现也可以用if判断来实现，这两个的执行效率，谁更快一些？
3. 核函数里面有75个变量，为什么编译的时候显示寄存器用量是60个？
4. CPU中二进制比乘除法效率高，GPU是否也是这样？

问题1、3可以直接通过看PTX代码得出结论，2、4还需要我们继续做一个验证性试验。下面将给出问题4的解法。

**实验中重点分为两部分，第一部分是判断nvcc编译器到底能智能到什么程度（我们都知道编译器在编译的时候会自己做一些优化，比如舍去一些你不用的变量和简化计算等等）；如果第一部分的答案是编译器编译乘除法和位移计算得到的ptx指令是相同的（就代表着相当智能了），那么结果很明显就是没有区别，就没有继续的必要。但是如果不一样，那么我们还需要做一个实验判断哪个更快一些了。**

# 1. 根据问题写出测试代码，得到测试代码的PTX代码

首先第一部分，我写了一段简易的测试代码，如下：

```
__global__ void mul(int *di) 
{ 
    int n = *di; 
    n = n * 2; 
    *di = n; 
} 

__global__ void div(int *di) 
{ 
    int n = *di; 
    n = n / 2; 
    *di = n; 
} 

__global__ void shl(int *di) 
{ 
    int n = *di; 
    n = n << 1; 
    *di = n; 
} 

__global__ void shr(int *di) 
{ 
    int n = *di; 
    n = n >> 1; 
    *di = n; 
}123456789101112131415161718192021222324252627
```

编译测试代码，编译时加 -keep参数，得到ptx文件内容如下（只摘取重要部分），顺序为乘法、除法、左移、右移：

```
mul.lo.s32       %r2, %r1, 2;1
    ld.global.s32 %r1, [%rd1+0];  // 将参数赋给寄存器 r1
    shr.s32 %r2, %r1, 31;         // 将r1右移31位，即将符号位移到最低位
    mov.s32 %r3, 1;               // r3 = 1 
    and.b32 %r4, %r2, %r3;        // 用 与运算 获取最低位，即获取r1的符号位 
    add.s32 %r5, %r4, %r1;        // 原数加上符号位赋给r5 
    shr.s32 %r6, %r5, 1;          // r5 右移一位赋给 r6 
    st.global.s32 [%rd1+0], %r6;  // 将计算结果重新赋值到 global memory中1234567
```

这里仔细解释一下除法的PTX代码 
经过分析，该计算过程能更加健壮地实现除法运算。对于所有正整数和所有非 -1 负整数，除法运算和右移的结果是没有差别的！但是对于 -1， 
-1/2的结果是0（正确结果），-1 >> 1的结果是-1（错误结果）！

```
shl.b32    %r2, %r1, 1;1
shr.s32 %r2, %r1, 1;1
```

根据ptx code 我们可以看出，乘法运算被翻译为mul指令，除法运算被翻译成了5条指令， 位移运算被翻译为shl（左移位）或shr（右移位）指令。故第一部分我们得出的结论为nvcc编译器在编译阶段对乘除法运算和位移运算的编译结果是不同的。

# 2. 验证试验

因此，进行第二部分实验，验证乘除运算和位移运算哪个速度更快一些。 
为了排除其他因素的影响，我选用了NVIDIA_CUDA-5.0_Samples里的vectorAdd（矩阵相加）源代码，对其进行了一些改动，如下：

```
__global__ void vectorMul(const int *A, int *C, int numElements) 
{ 
    int i = blockDim.x * blockIdx.x + threadIdx.x; 
    if (i < numElements) 
        C[i] = A[i] * 2; 
} 

__global__ void vectorDiv(const int *A, int *C, int numElements) 
{ 
    int i = blockDim.x * blockIdx.x + threadIdx.x; 
    if (i < numElements) 
        C[i] = A[i] / 2; 
} 

__global__ void vectorShl(const int *A, int *C, int numElements) 
{ 
    int i = blockDim.x * blockIdx.x + threadIdx.x; 
    if (i < numElements) 
        C[i] = A[i] << 1; 
} 

__global__ void vectorShr(const int *A, int *C, int numElements) 
{ 
    int i = blockDim.x * blockIdx.x + threadIdx.x; 
    if (i < numElements) 
        C[i] = A[i] >> 1; 
}123456789101112131415161718192021222324252627
```

测试数据量大小为256，1024，4096, 16384, 65536, 262144, 1048576，进行了10000次循环计算后取速度平均值，结果分别如下： 
![这里写图片描述](https://img-blog.csdn.net/20160112182304786)

从图中可以看出，除了在数据量较大（>65536）的情况下，除法运算会慢一点（慢不到0.001，大部分的时间都花在了数据传输上），四种运算方式的运行时间是没有绝对的规律。 
尽管除法运算会被翻译成较复杂的ptx指令，但GPU的执行速度非常快，所以为了保证代码的可读性，并不建议在核函数中用位移运算代替乘除运算！

# 第三篇：CUDA计时方式

2016年02月04日 00:02:32 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：7032



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/50520549

# 写在前面

内容分为两部分，第一部分为翻译《[Professional CUDA C Programming](http://search.jd.com/Search?keyword=Professional%20CUDA%20C%20Programming&enc=utf-8&wq=Professional%20CUDA%20C%20Programming&pvid=8sfe9eji.bhq02m)》 Section 2. CUDA Programming Model中的TIMING YOUR KERNEL；第二部分为自己的经验。经验不足，欢迎各位大大补充。

写CUDA，追求的就是加速比，想要得到准确的时间，计时函数就是必不可少 
计时通常分为两种情况，（1）直接得到接口函数的时间，一般用于得到加速比；（2）获得接口函数内核函数、内存拷贝函数等所耗时间，一般用于优化代码时。 
情况（1）方法有两种，CPU计时函数和GPU计时函数。 
情况（2）有三种工具nsight，nvvp，nvprof

本博客会详细介绍情况（1）的两种方法；情况（2），nsight不会用，简单介绍一下nvvp和nvprof的用法。

# CPU计时函数

在利用CPU计时函数时，要考虑的一个问题是：核函数的执行是异步执行的，所以必须加上核函数同步函数，才能得到准确的时间。 
示例代码如下：

```
double cpuSecond() {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}
double iStart = cpuSecond();
function(argument list);
cudaDeviceSynchronize();  // 同步函数
double iElaps = cpuSecond() - iStart;
123456789
```

# GPU计时函数

GPU计时函数就不需要考虑同步问题，直接用计时事件函数就可以了，示例代码如下：

```
cudaEvent_t start, stop;
float elapsedTime = 0.0;

cudaEventCreate(&start);
cudaEventCreate(&stop);
cudaEventRecord(start, 0);

function(argument list);;

cudaEventRecord(stop, 0);
cudaEventSynchronize(stop);

cudaEventElapsedTime(&elapsedTime, start, stop);

cout << elapsedTime << endl;

cudaEventDestroy(start);
cudaEventDestroy(stop);
123456789101112131415161718
```

# 如何获得精确的计时

正常情况下，第一次执行核函数的时间会比第二次慢一些。这是因为GPU在第一次计算时需要warmup。所以想要第一次核函数的执行时间是不精确的。获得精确的计时我总结为三种，如下

1. 循环执行一百次所需要计时的部分，求平均值，将第一次的误差缩小100倍。这种方法的优点是简单粗暴。但缺点也很明显：(1)程序的执行时间大大增长，特别是比较大的程序(2)要考虑内存溢出问题，C++的内存需要程序猿自己手动管理。写出执行一次不出内存溢出问题的程序很容易，但是写出循环执行一百次而不出内存溢出问题的代码就有一定难度了(3)计时不是特别准确，虽然误差已经被缩小了100倍。
2. 在计时之前先执行一个warmup函数，warmup函数随便写，比如我从cuda sample里的vectoradd作为warmup函数。这种方法的优点是程序执行时间缩短；缺点是需要在程序中添加一个函数，而且因为GPU乱序并行的执行方式，核函数的两次执行时间并不能完全保持一样。所以推荐使用方法3.
3. 先执行warmup函数，在循环10遍计时部分。

# nvvp和nvprof的用法

**nvprof**是自cuda5.0开始存在的一个命令行Profiler，你可以只用nvprof来你代码的一些执行细节。简单用法如下：

```
$ nvprof ./sumArraysOnGPU-timer
1
```

你就可以得到如下：

```
./sumArraysOnGPU-timer Starting...
Using Device 0: Tesla M2070
==17770== NVPROF is profiling process 17770, command: ./sumArraysOnGPU-timer
Vector size 16777216
sumArraysOnGPU <<<16384, 1024>>> Time elapsed 0.003266 sec
Arrays match.
......
1234567
```

关于nvprof的更多参数信息，可以使用帮助命令：

```
$ nvprof --help
1
```

**The NVIDIA Visual Profiler（nvvp）**是一款图形化界面的Profiler，也是我一直在用的Profiler。 
[简单图文教程见链接](http://wenku.baidu.com/link?url=hTgZENhDt64r97Mb4UhvLI4I-I_BQ0rmK0QmKD5TeGHQ-uhfeHGB6W5BiNTzKi2aEjZZw0YMIBDHXPoyypvsft_eg34DFltDrt0AG_DJIAK)

# 第四篇-如何实时获得GPU各种资源利用率（NVML）

2016年08月11日 12:45:51

 

大光叔叔

 

阅读数：9658

 

标签： [NVML](http://so.csdn.net/so/search/s.do?q=NVML&t=blog) 更多

个人分类： [CUDA进阶](https://blog.csdn.net/litdaguang/article/category/6056828)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/52181322

# CUDA进阶第四篇-如何实时获得GPU各种资源利用率

前几天在玩digits的时候发现在训练的过程中网页上能够动态显示显卡利用率，显存利用率和温度。突然想起来前几天有人问过这个问题并查了一下目前好像并没有类似的中文博客，所以就探究了一下这个问题。 
首先我去翻了一下digits的源码，发现是用python调用了[NVML(nvidia-management-library)](https://developer.nvidia.com/nvidia-management-library-nvml)这么个库。 
原来NVIDIA早就做好这么个库了，只是以前一直不知道。NVML已经有比较广泛的应用，比如NVIDIA-smi就是基于NVML库做的，还有一些[第三方工具](https://developer.nvidia.com/cluster-management)。 
下面开始正文，本文主要简单介绍一下NVML的安装和使用

环境：Ubuntu 14.04 cuda 7.0 
NVML的安装十分简单，下载[run文件](https://developer.nvidia.com/gpu-deployment-kit)，然后运行安装即可，安装目录选择默认。 
NVML主要包括两部分，一个lib和一个nvml.h头文件，还有其他的一些乱七八糟的东西，可以看一下安装的log文件(tmp/gdk_install_***.log)。 
这里很重要的就是我们需要知道lib文件和nvml头文件的路径，编译时需要加进去（不懂的请bing gcc -I -L）。 
但是有一个比较蛋疼的事情是这个路径针对不同的linux版本是不同的，而且在安装cuda的时候会自动给你按上一个nvml lib库，但是却没有头文件。所以到底该用哪个呢？ 
这里有一个stackoverflow上关于 [编译问题](https://stackoverflow.com/questions/36138947/nvml-code-doesnt-compile)可以供参考。

不过像我这样的懒人比比皆是，所以nvml提供了一个example(Ubuntu 14.04 的路径是/usr/src/gdk/nvml/examples)，这个文件夹下包含一个example.c和makefile，所以只需要按照这个makefile里的路径把参数添加到你的项目里即可~

Ubuntu 14.04下lib的路径是/usr/src/gdk/nvml/lib/ 
头文件路径是-I/usr/include/nvidia/gdk。 
使用时请添加头文件 #include< nvml.h > 
NVML API文档：<http://docs.nvidia.com/deploy/nvml-api/index.html>

# 第五篇-如何估算出程序的GPU加速比

2018年02月16日 20:54:49 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：1540



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/79330741

当对一个程序进行加速的时候，很多时候需要预估出程序使用GPU加速后的加速比（比如你老板不懂GPU，或者甲方会问你预估加速比等等）。从大二接触GPU加速，到现在大概有6年时间，大大小小的项目也做了十几个，很多时候都需要事先回答加速比会有多少这个问题。这里简单的说一下自己的经验，欢迎各位大神指点。

**文中的经验基于目前主流的显卡，比如GTX1080，最低也得是GTX9\**系列的。**

## 1.阿姆达尔定律

谈加速比，首先要先明白一个定律-**阿姆达尔定律**。该定律一般应用与CPU加速，可以总结为一句话-**程序中可并行代码的比例决定你增加处理器（总核心数）所能带来的速度提升的上限。**

应用在GPU加速情景上比较简单，假设程序S由A和B两个步骤组成，现在对A步骤进行GPU加速，那么**GPU加速比=A步骤加速前的时间/A步骤加速后的时间**，而不是**加速前S的总时间/加速后S的总时间。**这个非常重要，因为很多人不懂GPU加速，他们往往只关注整个程序的加速效果。而对于一个完整的程序，很多都包括一些非常耗时的操作（比如读取图片等）。这种情况下就算你对程序中的某个步骤使用GPU加速到极致，整个程序可能只感觉快了一点点，这是非常不公平的。

## 2.估算加速比

下面将列出几种常见情况。其中CPU实现的大for循环是最耗时的，其次是资源（显存和句柄等）的申请，最后是核函数级别的优化。

（1）程序CPU实现，并行度高，数据依赖低

这是最理想的情况，常见于各种较简单的图像算法，比如二值化，细化等。简单来讲就是一个大for循环，循环之间没有数据依赖。这种情况下，加速比很容易就上10+倍。当图片较大时（比如1024*1024），上几十倍也是很容易的。

（2）程序GPU实现，但是资源管理差

这是次理想的情况，程序已经由GPU核函数或者调用GPU API实现，但是资源（显存和句柄的申请和释放等）管理比较差。比如流程比较算法的算法，显存和句柄在程序中使用时才申请。这种情况下，看资源申请所占的时间比例，不过一般加速比能到几倍就很不错了。

（3）程序GPU实现，但核函数实现比较差

如果资源管理也做的比较好，一般从核函数本身入手了。对核函数进行优化（使用共享内存，条件分支优化，合并核函数等），得到的加速比一般在2X以内，超过2X的情况并不多。

# 第六篇-GPU资源（显存、句柄等）管理

2018年02月16日 21:42:32 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：1317



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/79330973

最近在加速一个图像算法，符合《[CUDA进阶第五篇-如何估算出程序的GPU加速比](http://blog.csdn.net/litdaguang/article/details/79330741)》中的第二种情况，程序由核函数和GPU API实现，但是资源管理特别差，显存和句柄在程序中使用时才申请。每次函数执行都要申请和释放一遍，非常耗费时间。

 

优化方案一：C++重构

我想到的第一个方案，就是C++重构，将所有的显存和句柄定义为成员变量，在构造函数中申请，析构函数中释放。难度比较低，但是工作量比较大，因为不同变量申请的显卡大小不一样，输入图像的大小是不一定的，所以对于每一个变量，必须申请足够大的内存，而且这种情况下，如果发生显存越界，则很难定位。而且参数名非常乱，着实费了一些劲才重构完。

优化方案二

因为这个算法中变量比较多，重构完后，输入大图像果然出现显存越界问题……真的是非常懒得去定位。某天灵光突现，可以用Tensorflow里的显存管理方式进行重构啊，先申请一块非常大的显存，程序中需要的时候直接分配即可。这样可以最大程度上的少修改原代码，并且可以有效的解决大图像情况容易出现的显存越界问题。详细代码如下。

```cpp
class MemoryManagement
{
public:
    MemoryManagement(int workspace_total_size)
    {
        workspace_total_size_ = workspace_total_size;
        CUDA_CHECK(cudaMalloc(&workspace_, workspace_total_size_));
 
        init();
    }
    ~MemoryManagement()
    {
        CUDA_CHECK(cudaFree(workspace_));
    }
 
    void AllocateMemory(void **ptr, int size)
    {
        int cache_line_size = 4;
        int tmp_size = (size + cache_line_size) / cache_line_size * cache_line_size;
        workspace_used_size_ += tmp_size;
 
        assert(workspace_used_size_ <= workspace_total_size_);
 
        *ptr = (void*)workspace_ptr_;
        workspace_ptr_ += size;
    }
 
    void Reset()
    {
        init();
    }
 
protected:
    char *workspace_;
    char *workspace_ptr_;
    int workspace_total_size_;
    int workspace_used_size_;
 
    void init()
    {
        CUDA_CHECK(cudaMemset(workspace_, 0, workspace_total_size_));
        workspace_ptr_ = workspace_;
        workspace_used_size_ = 0;
    }
};
```



 第七篇-如何获得核函数最佳线程块配置

2018年05月03日 17:41:21 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：733



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/80137754

最近在和几个小伙伴们一起翻译《[cuda-c-programming-guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html)》，我选了chapter 5，收获良多。翻译到5.2.3.1 Occupancy Calculator的时候，正好跟现在手里的任务有些重合，就做了些实验，分享给大家。有说的不对的地方欢迎拍砖。

这一小节的内容很简单，如何获得核函数最佳线程块大小配置，可以用几个API，根据kernel内寄存器和共享内存的使用量，计算出block大小，使SM占用率最大。

内容很简单，译文还没校准完，大家可以先读英文。

我之前是不会这个的，所以我是用暴力循环的方式得出速度最快时的block大小，即以32为step，测block大小从32-1024时核函数的性能，但是这种方式比较麻烦，且平台和数据量都有一定的影响，不方便集成到系统中，所以只是实验的时候玩一玩。今天翻译到这个的时候，在想，这个occupancy方式是否真的好用，就做了个实验，并得出了一些经验行的结论。

代码里的核函数太简单了，我换了个复杂点的核函数，代码如下：

```cpp
__global__ static void _bias_reluKer(float *data, float *bias, const int height, 
                                const int width)
{
    int bidx = blockIdx.x;
    int tidx = threadIdx.x;
    int span = blockDim.x;
    int i;
 
    if(bidx < height)
    {
        // copy data to shared memory
        i = tidx;
        for( ; i < width; i += span ) {
            float tmp = data[i + bidx * width] + bias[i]; // bias
            if(tmp < 0) tmp = 0.0f; 
            data[i + bidx * width] = tmp;
        }
    }
} 
```

cudaOccupancyMaxPotentialBlockSize()的结果是最佳blocksize是1024。

循环100遍，得到了总时间，实验结果如下：

```cpp
arrayCount = 819200
OccupancyMax Out:blockSize=1024, gridSize = 800
32                                      : 8.022000(ms)
64                                      : 5.382000(ms)
704                                     : 5.348000(ms)
736                                     : 5.308000(ms)
768                                     : 5.249000(ms)
192                                     : 5.199000(ms)
800                                     : 5.194000(ms)
224                                     : 5.167000(ms)
832                                     : 5.145000(ms)
864                                     : 5.093000(ms)
896                                     : 5.050000(ms)
928                                     : 4.999000(ms)
352                                     : 4.964000(ms)
960                                     : 4.939000(ms)
544                                     : 4.917000(ms)
992                                     : 4.887000(ms)
416                                     : 4.880000(ms)
384                                     : 4.877000(ms)
576                                     : 4.873000(ms)
608                                     : 4.814000(ms)
448                                     : 4.808000(ms)
672                                     : 4.792000(ms)
640                                     : 4.776000(ms)
OccupancyMax                            : 4.774000(ms)
1024                                    : 4.763000(ms)
256                                     : 4.742000(ms)
480                                     : 4.738000(ms)
96                                      : 4.662000(ms)
512                                     : 4.612000(ms)
320                                     : 4.541000(ms)
128                                     : 4.507000(ms)
288                                     : 4.492000(ms)
160                                     : 4.435000(ms)
```

从实验结果我们可以得到结论：cudaOccupancyMaxPotentialBlockSize()函数确实是有效的，blocksize=1024和最佳blocksize=160的性能才差(4.77-1.43)/100 = 0.0034ms，可以直接忽略了。

但是，是否每个核函数都有必要这么做呢？因为这种方式并不流行，起码我CUDA编程5年以来第一次见，看caffe和kaldi的源码里好像也都没用这个函数。这么多年我都是直接用经验值，block=128 or 256。然后我又试几个核函数，结果发现……：

**当核函数的计算不是十分复杂时，block=128 or 256 的性能跟Occupancy Calculator的结果差微乎其微，所以，还是直接用经验值吧……**

 



# 第八篇-CUDA性能优化指南总结

2018年05月18日 17:35:30 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：927



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/80367473

本文为《CUDA C Programming Guide》chapter 5 Performance Guidelines章节的总结。精力有限，可能没做到覆盖所有的点，期望指正。
该章的主要内容是指导如何进行性能优化。从三个大方面入手：最大化利用率;最大
化内存吞吐量;和最大化指令吞吐量。
 

# 最大化利用率

## 应用层次：

- 使用异步函数和流，最大化主机端（host）任务、设备端（device）任务和主机设备通信任务的并行性；
- 将需要使用共享内存通信的线程放到一个block内。

## 设备层次

- 将没有数据依赖，可并行的kernel使用stream加速。

## Multiprocessor 层次

- 编写kernel代码时谨慎使用寄存器和共享内存，防止影响occupancy；
- block大小应该为warp的倍数

# 最大化内存吞吐量

- 尽量使用page-locked内存；
- 尽可能的减少host和device之间的内存传输。比如（1）将多次传输合并为一次大传输；（2）可以将一些并行度不高但是需要H-D传输的任务放到GPU上，虽然这个任务在CPU上会更快一些，但是减少了H-D传输，这是一个trade-off问题。
- 访问全局内存时，要考虑对齐问题，尽可能的合访问。比如（1）使用cudaMallocPitch申请逻辑二维数组（比如图像）；（2）设计访存模型时，考虑合并访问问题；
- 不要让kernel中的变量被放到local Memory中；
- 使用shared Memory时，防止bank conflict；

# 最大化指令吞吐量

- 尽量少使用低吞吐量的算术指令;
- 在不影响最后结果的情况下使用低精度类型，使用内部计算接口而不是自己实现； 
- 使用单精度float而不是双精度double；
- 尽量减少流程控制指令，即减少条件分支。比如（1）控制条件仅取决于（threadIdx / warpSize）；（2）使用#pragma unroll展开循环。
- __fdividef（x，y）是比除法运算符更快的单精度浮点除法
- 大部分情况下，rsqrtf()效率比1.0 / sqrtf()高。
- 使用三角函数时，尽量使用单精度，且x值域较小为宜；
- 尽量使用位运算代替整数除法和模运算；
- 半精度计算，使用half2代替half
- 调用接口时，尽量保持参数类型一致，否则会有类型转换的消耗；比如函数参数是int类型，输入是char类型或者short类型，需要先将类型转化为int类型。







# 补充篇：详析各种CUDA函数计时函数

2017年08月25日 20:28:33 [大光叔叔](https://me.csdn.net/litdaguang) 阅读数：1359



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/litdaguang/article/details/77585011

> **写在前面**：之前写了一篇[CUDA进阶第三篇：CUDA计时方式](http://blog.csdn.net/litdaguang/article/details/50520549)，列出了几种当时遇到的CUDA计时方式，只是个教程式的东西，没有太多技术含量，也不太全面。前几天在CUDA Professional（45157483）群里和大佬们讨论到CUDA官方event函数在计时cpu和cpu混合代码时有问题，虎躯一震，tm这么多年要是一直用的都是错的就瞎了。今天特花时间实验探究一番。有不足之处还望各位前辈指点。

# 概要

本文分为两部分，前半部分为测验不同计时函数在计时CUDA函数的表现以及分析出的一个坑。后半部分为分析了GPGPU-sim仿真器中cudaevent计时函数的源码。

# 不同计时函数在计时CUDA函数的表现

## 实验设计

选取一段cpu和gpu混合代码（这里选择的cuda samples里的vectorAdd），分别用四种不同的计时函数统计程序运行时间进行对比。四种计时函数如下：

- gettimeofday（）
- 官方推荐的cudaEvent方式
- clock()函数
- c++中的chrono库

## 代码简单思路

vectorAdd的代码比较简单，只有一个核函数**global** void vectorAdd()和一个main()函数。 
main()函数内如下：

```cpp
__global__ void
vectorAdd(const float *A, const float *B, float *C, int numElements)
{/*节省空间删掉*/}

__global__ void warmup()
{/*预热GPU，调用一个空的核函数*/}

double cpuSecond() {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

/**
 * Host main routine
 */
int
main(int argc, char **argv)
{
    // 预热GPU
    warmup<<<1, 1>>>();
    cudaDeviceSynchronize();

    // 变量申请
    double start = 0.0f, end = 0.0f;
    float elapsedTime = 0.0;
    cudaEvent_t event_start, event_stop;
    clock_t clock_start;
    clock_t clock_end;
    std::chrono::time_point<std::chrono::system_clock> c11_start, c11_end;

    // 四种计时方式
    if(atoi(argv[1]) == 1) {
        start = cpuSecond();
    } else if(atoi(argv[1]) == 2) {
        cudaEventCreate(&event_start);
        cudaEventCreate(&event_stop);
        cudaEventRecord(event_start, 0);
    } else if(atoi(argv[1]) == 3) {
        clock_start = clock();
    } else if(atoi(argv[1]) == 4) {
        c11_start = system_clock::now();
    }
    /*vectorAdd代码，包含内存申请、初始化、拷贝、计算、拷回、释放。数据量大小为5000000*/

    if(atoi(argv[1]) == 1) {
        // 如果使用CPU计时方式，一定要加同步函数！！
        cudaDeviceSynchronize();
        end = cpuSecond();
        printf("gettimeofday time = %lfms\n", (end - start) * 1000);
    } else if(atoi(argv[1]) == 2) {
        cudaEventRecord(event_stop, 0);
        cudaEventSynchronize(event_stop);
        cudaEventElapsedTime(&elapsedTime, event_start, event_stop);
        printf("cudaevent time = %lfms\n", elapsedTime);
    } else if(atoi(argv[1]) == 3) {
        cudaDeviceSynchronize();
        clock_end= clock();
        double clock_diff_sec = ((double)(clock_end- clock_start) / CLOCKS_PER_SEC);
        printf("clock_ time: %lfms.\n", clock_diff_sec * 1000);
    }else if(atoi(argv[1]) == 4) {
        cudaDeviceSynchronize();
        c11_end = system_clock::now();
        int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (c11_end-c11_start).count();
        printf("chrono time: %dms.\n", elapsed_seconds);
    }
}
```

## 实验结果

实验平台： 
GPU ： Tesla K80 
系统 ： Centos 6 
gcc ： 4.7.2

时间统计结果如下：

| 计时方式       | Time(ms)     | 评价                           |
| -------------- | ------------ | ------------------------------ |
| gettimeofday() | 326.971769ms | 不太稳定，上下有大概20ms的浮动 |
| cudaEvent      | 328.312744ms | 上下3ms左右的浮动              |
| clock()        | 330ms        | 很稳定                         |
| chrono         | 324ms        | 上下3ms左右的浮动              |

------

从实验结果可以看出，后三种计时方式都是比较稳定，可以放心使用。

## 坑

细心的人可能会发现，我在代码最前面加了一个空的warmup函数。**这个在精确统计时间是非常重要的！！！**因为GPU第一次被调用时会消耗**不定的**时间来预热。 
如果把预热那行注释掉，得到的计时结果如下：

| 计时方式       | Time(ms)     |
| -------------- | ------------ |
| gettimeofday() | 535.159826ms |
| cudaEvent      | 346.573151ms |
| clock()        | 440.000000ms |
| chrono         | 470ms        |

可以看出，1,3,4三种CPU计时方式结果与真实结果大相径庭，cudaEvent还算比较接近。 
所以个人比较推荐的精确计时方式为：**（1）前面加warmup函数；（2）循环N(比如100次)然后求平均；（3）针对某个kernel函数，用nvvp或者nvprof看精准的时间。**

# cudaevent计时函数源码分析

[GPGPU-Sim](http://gpgpu-sim.org/)是一款cycle级别的GPU仿真器。我之前也写过几篇介绍GPGPU-sim的[博客](http://blog.csdn.net/litdaguang/article/details/44408439)。我从GPGPU-Sim的源码中找到了cudaEvent计时方式的源码，简单分析了一下。 
cudaEvent计时方式的流程如下，核心函数为cudaEventRecord(）和cudaEventElapsedTime()

```cpp
cudaEvent_t start, stop;
float elapsedTime = 0.0;

cudaEventCreate(&start);
cudaEventCreate(&stop);
cudaEventRecord(start, 0);

function(argument list);;

cudaEventRecord(stop, 0);
cudaEventSynchronize(stop);

cudaEventElapsedTime(&elapsedTime, start, stop);


cudaEventDestroy(start);
cudaEventDestroy(stop)
```

其中cudaEventRecord()函数的源码如下：

```cpp
__host__ cudaError_t CUDARTAPI cudaEventRecord(cudaEvent_t event, cudaStream_t stream)
{
    CUevent_st *e = get_event(event);
    if( !e ) return g_last_cudaError = cudaErrorUnknown;
    struct CUstream_st *s = (struct CUstream_st *)stream;
    stream_operation op(e,s);
    g_stream_manager->push(op);
    return g_last_cudaError = cudaSuccess;
}
```

其中cudaEventElapsedTime()函数的源码如下：

```cpp
__host__ cudaError_t CUDARTAPI cudaEventElapsedTime(float *ms, cudaEvent_t start, cudaEvent_t end)
{
    time_t elapsed_time;
    CUevent_st *s = get_event(start);
    CUevent_st *e = get_event(end);
    if( s==NULL || e==NULL )
        return g_last_cudaError = cudaErrorUnknown;
    elapsed_time = e->clock() - s->clock();
    *ms = 1000*elapsed_time;
    return g_last_cudaError = cudaSuccess;
}
```

可以看出cudaEventRecord()函数其实就是一个流操作，并压入一个栈中g_stream_manager。 
在cudaEventElapsedTime函数中，在栈中找到两次压入的cudaEvent，然后再调用clock()函数计算时间差。而这个clock()函数本质是一个time_t类型。**所以归根到底还是调用CPU端的计时函数进行计时**。

PS:因为GPU是商业软件，不公开内部实现细节。所以GPGPU-Sim只能模拟非常老的Fermi架构，所以这里的分析并不保证是完全正确的。































# 写在后面

[OpenCUDA](https://github.com/LitLeo/OpenCUDA)，CUDA图像算法开源项目，算法内都有详细的注释，大家一起学习。



