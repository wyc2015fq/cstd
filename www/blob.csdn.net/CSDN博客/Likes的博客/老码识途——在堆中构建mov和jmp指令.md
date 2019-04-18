# 老码识途——在堆中构建mov和jmp指令 - Likes的博客 - CSDN博客
2018年08月04日 11:53:34[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：113
```cpp
// asmjmp.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <malloc.h>
int gi;
void * address;//这里是一个变量的定义，跟函数没有一点关系
void * buildCode()
{
	//我们要构建的代码如下
	// mov gi, 18
	// jmp address;
	//mov gi, 被赋值整数；指令是10个字节组成C7 05 gi的地址四字节 被赋值整数四字节
	//jmp address;6字节，前两个为ff 25，后4字节为地址
	//因此我们需要分配16个字节
	char * code = (char *)malloc(16);
	char * pMov = code;
	char * pJmp = code + 10;
	char * pAddress;
	//构建mov gi, 18
	pMov[0] = 0xc7;
	pMov[1] = 0x05;
	pAddress = pMov + 2;
	*((int *)pAddress) = (int)&gi;
	*((int *)(pAddress + 4)) = 18;
	//构建jmp address
	pJmp[0] = 0xff;
	pJmp[1] = 0x25;
	*((int *)(&pJmp[2])) = (int)&address;//jmp是跳到address变量中存的地址lb1
	return code;
}
void main()
{	
	void * code = buildCode();//调用buildCode在堆中构建代码
	_asm {
		mov address, offset _lb1;	//现在address全局变量中存的就是_lb1所在的地址
	}
	gi = 12;
	printf("gi = %d\n", gi);
	_asm jmp code;//执行我们自己构建的代码
	gi = 13;
	_lb1:
	printf("gi = %d\n", gi);//打印的结果为18，而不是12
	getchar();
}
```
1、在堆中分配一块内存，在其中构建相应的代码并执行。其实已经越过了一般的代码机制，在数据段中执行指令
2、_asm中的代码如果只有一条可以不加{ }
3、_asm中的代码可以不加分号。表面上只是一个语法规定，但背后有深刻原因：每条指令的长度都是相同的。每当计算机识别出是那一条指令，读取多长是明确的，不用再适用分号进行区分。
