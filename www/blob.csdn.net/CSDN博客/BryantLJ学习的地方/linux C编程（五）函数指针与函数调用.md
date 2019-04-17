# linux C编程（五）函数指针与函数调用 - BryantLJ学习的地方 - CSDN博客





2016年08月25日 22:45:50[遍地流金](https://me.csdn.net/u012177034)阅读数：3052








# 函数指针

函数指针，首先是一个指针，只不过该指针指向的是函数，而不是变量。

```cpp
#include <stdio.h>

int max(int x,int y)
{
  return (x>y)?x:y;
}
int main()
{
  int i = 0;
  int (*f)(int x,int y) = max;//定义函数指针并初始化
  //f = max;f = &max;也可使用这两种方式进行初始化
  i = f(5,4);
  printf("%d\n",i);
  return 0;
}
```

## 函数指针的定义

```
int (*f)(int x,int y);//最容易理解
int (*f)(int,int);//只说明函数的输入数据类型
int (*f)();//可忽略形参，因为只是声明一个函数指针
typedef int (*func_ptr)(); 
func_ptr f;//先定义一个函数指针类型，再用此类型来定义
```

## 函数指针的初始化

```
f = max;//函数名本来就是函数的首地址
f = &max;//取函数的地址
```

## 函数指针的调用

```
f(x,y);//f可直接调用
(*f)(x,y);//将f作为一个函数指针，先取函数，然后在调用
```

## 函数指针数组

函数指针数组的定义及初始化

```
//1.通过typedef帮助，理解简单
typedef int (*func_ptr)();
func_ptr f_tab[3] = {max,min,middle};
//2.直接定义，由于‘[]’优先级要比'*'高，直接在f后加'[]'即可
int (*f[3])() = {max,min,middle};
```

# 函数调用

主要有三种调用方式 

_stdcall：参数从右至左依次入栈；**被调用的函数清理栈**

_cdecl：参数从右至左入栈；**调用方清理栈（即由调用方完成输入参数，函数返回地址，之前的堆栈基地址，局部变量等的出栈工作）**；这样的函数可变参数！ 

_fastcall：左边开始的两个不大于4个字节的参数保存在ECX和EDX寄存器中，其余参数仍自右向左入栈；**被调用者清理栈**；速度快！
函数调用栈的示意图如下图所示： 
![函数调用栈](https://img-blog.csdn.net/20160825221728960)
**虽然三者的出栈处理方不一样，但是入栈方式都是一样的。都是调用函数先将输入参数自右向左压栈，将函数的返回地址压栈，被调用函数被执行，先将之前的堆栈基地址寄存器EBP压栈，再将一些本地的局部变量压栈，最后执行代码！**

详细的调用者及被调用者的栈帧

![这里写图片描述](https://img-blog.csdn.net/20160907091721322)

**返回值的传递方式：**每一个被调用函数都会讲返回值存放在eax寄存器里，若调用者有使用被调用者的返回值，只需从eax寄存器里读取即可。当返回值的大小特别大时（如返回一个大的结构体），此时系统系统编译时可能会将返回值的指针写入eax寄存器，引用的时候按地址引用！

windows下显式定义函数的调用方式

```
int _stdcall func(int x,int y);
int _cdecl func(int x,int y);
int func(int x,int y);//不显式定义的默认为cdecl方式调用
```

linux下按如下方式定义调用方式

```
int __attribute__((__stdcall__)) min(int x,int y)；
int __attribute__((__cdecl__)) min(int x,int y);
int min(int x,int y)；//默认为cdecl方式调用
```





