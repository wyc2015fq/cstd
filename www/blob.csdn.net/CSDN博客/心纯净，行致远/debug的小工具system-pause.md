# debug的小工具system-pause - 心纯净，行致远 - CSDN博客





2018年11月15日 09:37:50[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：152








从程序中发起系统命令行调用，执行命令：pause。

作用和效果就是dos下的pause。

system("PAUSE");   和system("pause");   作用和效果一样，因为dos命令是不区分大小写的。

system("PAUSE")  是暂停的意思，等待用户信号；不然控制台程序会一闪即过，你来不及看到执行结果。

![](https://gss0.baidu.com/-fo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=8fb1fd9ada2a60595245e91c180418a3/8718367adab44aedf0cdafb7be1c8701a08bfbe6.jpg)

## 扩展资料：

system是C函数库[stdlib.h](https://www.baidu.com/s?wd=stdlib.h&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)、process.h中的一个函数。具体解释如下：

int system(char *command)：将MSDOS命令command传递给DOS执行。

而pause是DOS下的一个命令。功能是：暂停批处理的执行，显示一个中文信息：“请按任意键继续. . .”，或者英文信息：“Press any key to continue . . .”

简单的说：就是暂停程序的执行，等待任意健继续执行。

尝试了一下，如果在c语言中使用，需要加上<stdlib.h>或者<process.h>。但是如果在C++中使用只需要<iostream>就可以。

# 用途

### 可以在需要调试分析的地方加上该语句，以便能够分析观察关节部分。

# 实例

system("pause")意思就是让程序暂停一下，然后按任意键继续，初学的时候最多见于程序的末尾处，用于看运行结果，避免程序一闪而过。相同的我们还可以用getchar()，避免程序运行完直接结束而看不到运行结果。

如下图：

![](https://gss0.baidu.com/-4o3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=2347b0ba174c510fae91ea1c50690915/b21bb051f8198618927981f647ed2e738bd4e682.jpg)

执行到    system("pause");    按任意键继续

![](https://gss0.baidu.com/9vo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=0bc23e7a01d79123e0b59c729d0475ba/f3d3572c11dfa9ec4b5138656fd0f703918fc141.jpg)

贴上代码：

```cpp
#include <stdio.h>

void main()

{

printf("haha哈哈\n");

system("pause");

printf("haha哈哈\n");

system("pause");

}
```

**扩展资料：**

函数名： system 

功 能： 发出一个DOS命令 

用 法： int system(char *command);

system函数已经被收录在标准c库中，可以直接调用。



