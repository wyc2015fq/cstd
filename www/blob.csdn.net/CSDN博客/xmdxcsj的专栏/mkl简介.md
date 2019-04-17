# mkl简介 - xmdxcsj的专栏 - CSDN博客





2016年09月23日 21:56:24[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3649








## 一、概况

### （一）下载

[下载免费版本](https://software.intel.com/en-us/intel-mkl/documentation)，注意保留serial number（安装需要）

### （二）[安装](https://software.intel.com/en-us/articles/intel-math-kernel-library-intel-mkl-2017-install-guide)
- 编辑silent.cfg 文件中的选项
- 安装需要2G的空间，默认的tmp空间不足使用**–tmp_dir**指定
`./install.sh --silent ./silent.cfg --tmp-dir /dir`
### （三）[基本概况](https://software.intel.com/zh-cn/node/528504)

### （四）[文件结构](https://software.intel.com/zh-cn/node/528507)

## 二、参数

### （一）[architecture](https://software.intel.com/zh-cn/node/528506)

IA-32 or compatible: /lib/ia32_lin 

Intel® 64 or compatible: /lib/intel64_lin 

Intel® Many Integrated Core (Intel® MIC): /lib/intel64_lin_mic
### （二）[LP64 vs ILP64](https://software.intel.com/zh-cn/node/528524)

LP64：使用32-bit的int 

ILP64：使用64-bit的int，支持更大的数组

### （三）[threading](https://software.intel.com/zh-cn/node/528527)

MKL有两种模式 

parallel: 使用OpenMP(libiomp5.so)或者TBB(libtbb.so)两种线程技术 

sequential: 不使用多线程，不建议使用
## 三、link library

### （一）[选择链接库](https://software.intel.com/zh-cn/node/528514)

选择各个层对应的库，然后增加RTL

### （二）[advisor](https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor/)

根据输出选项自动生成需要的链接库

### （三）例子

```
MKLROOT = /path/to/mkl
gcc myprog.c -L$(MKLROOT)/mkl/lib/intel64 -L$(MKLROOT)/lib/intel64_lin -I$(MKLROOT)/mkl/include -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

#-lmkl_intel_lp64: interface layer
#-lmkl_intel_thread: threading layer
#-lmkl_core: computational layer
#-liomp5: RTL(run-time library)
#-lm: 连接数学库(FFT/Laplace)
#-lpthread: libiomp5依赖于pthread库，需要在-liomp5后面添加-lpthread(顺序很重要)
```

## 四、优化

### （一）[多线程](https://software.intel.com/zh-cn/node/528539)

OpenMP有三种方式可以设定，优先级由高到低： 

- 设置MKL_NUM_THREADS或者对应的函数 

- 设置OMP_NUM_THREADS或者对应的函数 

- 使用默认的OpenMP的线程数（物理核的个数） 

TBB只能使用接口函数的形式设置 

设置相应的线程数优化
### （二）小矩阵

矩阵比较小的时候，使用MKL_DIRECT_CALL宏定义，可以忽略mkl的错误检查以及启动small-matrix kernels。

### （三）代码技巧

1.[数据对齐](https://software.intel.com/zh-cn/node/528583)

2.LAPACK packed routines 

3.FFT的数据长度为2的指数
### （四）内存管理

1.避免内存泄露 

- 设置MKL_DISABLE_FAST_MM为1或者调用mkl_disable_fast_mm()函数，可能会对性能有负影响 

- 在当前线程调用mkl_free_buffers()或者mkl_thread_free_buffers()函数
2.[使High-bandwidth Memory](https://software.intel.com/zh-cn/node/589700)

3.[使用自己的内存函数](https://software.intel.com/zh-cn/node/528565)

## 五、mkl使用

### （一）[函数参考](https://software.intel.com/en-us/mkl-reference-manual-for-c)

### （二）[使用范例](https://software.intel.com/en-us/node/529735)








