# 多线程开发简易工具——OpenMP使用 - 迭代的是人，递归的是神 - CSDN博客





2012年03月23日 13:52:10[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：5639








最近有朋友问我tld感觉运行还是不够快，问我如何提升效率，我说用多线程，可是感觉很多编程人员认为这是个禁区，不敢涉足。但其实大家不用如此害怕，并行编程固然涉及很多系统运行机制的问题，搞不好可能会使程序崩溃，但OpenMP大大降低了并行开发的难度和复杂度。

具体使用也非常简单，不用安装任何辅助开发插件，只是在vs开发环境中简单配置一下，我这里使用的vs2008的中文版，只需如图设置，对应英文版应该是projects->properties->c++->language中的/OpenMP选项开启即可。

![](http://hi.csdn.net/attachment/201203/23/0_13324807772Eaa.gif)


接下来添加头文件#include <omp.h>，并在对应的并行代码前添加#pragma omp parallel for。一般需要并行运算的部分都是大量的循环操作。这里我试验选用for语句作为并行测试，所以#pragma omp parallel for语句都加在for语句的前面。这样就ok了，是不是真的很容易，神马，你还想要个例子，好吧，满足你，上一小段测试代码，测试一下程序的运行速度（由于出差，只能在自己的小本本测试一下，只有双核，很不给力，如果大家电脑配置够好，测试结果应该更暴力一些）



```cpp
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>

using namespace cv;

void Test (int n)
{
	for(int i = 0; i < 10000; i++)
	{
		for (int j=0; j<10000; j++)
		{
			//do nothing, just waste time
		}
	}
	printf("%d, ", n);
}

void main(void)
{
	double t = (double)getTickCount(); 

	for(int i = 0; i < 10; i++)
	{
		Test( i );
	}

	t = ((double)getTickCount() - t)/getTickFrequency();
	printf("time without OpenMP: %.3fs\n", t);

	t = (double)getTickCount(); 

#pragma omp parallel for

	for(int i = 0; i < 10; i++)
	{
		Test( i );
	}

	t = ((double)getTickCount() - t)/getTickFrequency();
	printf("time with OpenMP: %.3fs\n", t);
}
```


运行结果如图：

![](http://hi.csdn.net/attachment/201203/23/0_13324818307Z6a.gif)


一方面，通过结果可以看到，添加OpenMP模块之后，程序运行顺序不再顺序执行，而是拆分成0~4,5~9两部分分开运行（是由于我只有两个核的原因么？如果用实验室的4核处理是否变成4个执行序分开执行呢，有待出差归来验证一下）。运行时间看，也可以轻易看出来，分成两个运行序运行后，执行时间大概也节约了一半。可见OpenMP可以简单实现并行提速哦，大家可以果断试用，谁用谁知道~~



