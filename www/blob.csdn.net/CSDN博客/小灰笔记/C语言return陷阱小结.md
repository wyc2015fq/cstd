# C语言return陷阱小结 - 小灰笔记 - CSDN博客





2017年02月19日 00:12:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3388








  1 #include "stdio.h"

  2 

  3 int num =
0;

  4 int value =
7;

  5 

  6 int TestFunc(void)

  7 {

  8 if(num ==
0)

  9 return

 10     value += 1;

 11 return5;

 12 }

 13 

 14 int main(void)

 15 {

 16     printf("%d\n",TestFunc());

 17 return0;

 18 }




上面这段代码的第9行应该是一个错误，但是编译器其实什么都查不出来，而是把第10行的计算结果作为了最终的返回值。程序编译运行后如下：GreydeMac-mini:exp02 greyzhang$ gcc exp02.c 

GreydeMac-mini:exp02 greyzhang$ ./a.out 

8




在C语言中，return后面可以什么参数都不加，这种用法一般是用在返回值为void的函数中。返回值不为void出现什么都不加的return会出现问题吗？我测试了一下，其实这个还真不好说，跟具体的环境有一定的关系。上面的程序修改如下：

  1 #include "stdio.h"

  2 

  3 int num =
0;

  4 int value =
7;

  5 

  6 int TestFunc(void)

  7 {

  8 if(num ==
0)

  9 return;

 10     value += 1;

 11 return5;

 12 }

 13 

 14 int main(void)

 15 {

 16     printf("%d\n",TestFunc());

 17 return0;

 18 }




Mac中程序编译运行：

GreydeMac-mini:exp02 greyzhang$ gcc exp02.c 

**exp02.c:9:2: ****error:****non-void function 'TestFunc' should return a value**

**      [-Wreturn-type]**

        return;

**        ^**

1 error generated.




由上面的信息看来，这是个错误，而且幸运地被被编译器找到了。不过，Windows下面的测试确实是不同，我目前手头没有Windows的环境，只能把之前下班后在公司的电脑中做的测试简单描述下。

测试1:Windows7下使用gcc 3.4.2进行测试，输出结果一直为16384，我不知道为什么会这样，但是怀疑是不是跟Windows的版本号有一定关系？然而，查了软件授权服务版本之后发现这个猜想似乎又不对。公司中的软件版是否是正版我不是很清楚，懒得去查了，毕竟在公司是否使用正版软件上我确实是无能无力。

测试2:Windows7的另一台电脑，使用gcc 4.9.2测试，返回结果为0。这个结果又是让我一阵子糊涂，到底是什么机制呢？难不成gcc在比较新的版本中对此软件特性进行了升级？在公司的电脑中软件安装很不自由，在同一台电脑上测试两个版本的gcc不可能，具体是不是又OS的因素我不好下定论。

看了上面的几个结果，综合对比下，我个人觉得Mac中内置的编译器LLVM做的应该是最好的处理。这种错误的隐患或者说是不确定的隐患，应该在编译阶段给出错误，至少也应该给出警告。



