# c语言中static 用法总结 - h13 - 博客园
假设在test.h中定义了一个static bool g_test=false;
若test1.c和test2.c都包含test.h，则test1.c和test2.c分别生成两份g_test，在test1.c 中置g_test=true,而test2.c中仍然为false并未改变！shit！！
一、c程序存储空间布局
C程序一直由下列部分组成：
      1）正文段——CPU执行的机器指令部分；一个程序只有一个副本；只读，防止程序由于意外事故而修改自身指令；
      2）初始化数据段（数据段）——在程序中所有赋了初值的全局变量，存放在这里。
      3）非初始化数据段（bss段）——在程序中没有初始化的全局变量；内核将此段初始化为0。
      4）栈——增长方向：自顶向下增长；自动变量以及每次函数调用时所需要保存的信息（返回地址；环境信息）。
      5）堆——动态存储分。
|-----------|
|                 |
|-----------|
|    栈         |  
|-----------|
|    |            | 
|   |/           |
|                 |
|                 |
|   /|           |
|    |            | 
|-----------|
|    堆         |
|-----------|
| 未初始化  |
|-----------|
|   初始化  |
|-----------|
|  正文段   |
|-----------|
二、 面向过程程序设计中的static
1. 全局静态变量
   在全局变量之前加上关键字static，全局变量就被定义成为一个全局静态变量。
   1）内存中的位置：静态存储区（静态存储区在整个程序运行期间都存在）
   2）初始化：未经初始化的全局静态变量会被程序自动初始化为0（自动对象的值是任意的，除非他被显示初始化）
   3）作用域：全局静态变量在声明他的文件之外是不可见的。准确地讲从定义之处开始到文件结尾。
看下面关于作用域的程序：
//teststatic1.c 
void display();
extern int n; 
int main()
{
  n = 20;
  printf("%dn",n);
  display();
  return 0;
}
//teststatic2.c 
static int n;   //定义全局静态变量，自动初始化为0，仅在本文件中可见
void display()
{
  n++;
  printf("%dn",n);
}
文件分别编译通过，但link的时候teststatic２.c中的变量n找不到定义，产生错误。
定义全局静态变量的好处：
<1>不会被其他文件所访问，修改
<2>其他文件中可以使用相同名字的变量，不会发生冲突。
2. 局部静态变量
  在局部变量之前加上关键字static，局部变量就被定义成为一个局部静态变量。
  1）内存中的位置：静态存储区
  2）初始化：未经初始化的全局静态变量会被程序自动初始化为0（自动对象的值是任意的，除非他被显示初始化）
  3）作用域：作用域仍为局部作用域，当定义它的函数或者语句块结束的时候，作用域随之结束。
  注：当static用来修饰局部变量的时候，它就改变了局部变量的存储位置，从原来的栈中存放改为静态存储区。但是局部静态变量在离开作用域之后，并没有被销毁，而是仍然驻留在内存当中，直到程序结束，只不过我们不能再对他进行访问。
      当static用来修饰全局变量的时候，它就改变了全局变量的作用域（在声明他的文件之外是不可见的），但是没有改变它的存放位置，还是在静态存储区中。
3. 静态函数
  在函数的返回类型前加上关键字static，函数就被定义成为静态函数。
  函数的定义和声明默认情况下是extern的，但静态函数只是在声明他的文件当中可见，不能被其他文件所用。
  例如：
//teststatic1.c
void display();
static void staticdis(); 
int main()
{
  display();
  staticdis();
  renturn 0;
}
//teststatic2.c
void display()
{
  staticdis();
  printf("display() has been called n");
}
static void staticdis()
{
  printf("staticDis() has been calledn");
}
文件分别编译通过，但是连接的时候找不到函数staticdis（）的定义，产生错误。
 实际上编译也未过，ｖｃ２００３报告teststatic1.c中静态函数staticdis已声明但未定义　；ｂｙ　ｉｍｊａｃｏｂ
定义静态函数的好处：
<1> 其他文件中可以定义相同名字的函数，不会发生冲突
<2> 静态函数不能被其他文件所用。
存储说明符auto，register，extern，static，对应两种存储期：自动存储期和静态存储期。
auto和register对应自动存储期。具有自动存储期的变量在进入声明该变量的程序块时被建立，它在该程序块活动时存在，退出该程序块时撤销。
关键字extern和static用来说明具有静态存储期的变量和函数。用static声明的局部变量具有静态存储持续期（static storage duration），或静态范围（static extent）。虽然他的值在函数调用之间保持有效，但是其名字的可视性仍限制在其局部域内。静态局部对象在程序执行到该对象的声明处时被首次初始化。
由于static变量的以上特性，可实现一些特定功能。
1． 统计次数功能
声明函数的一个局部变量，并设为static类型，作为一个计数器，这样函数每次被调用的时候就可以进行计数。这是统计函数被调用次数的最好的办法，因为这个变量是和函数息息相关的，而函数可能在多个不同的地方被调用，所以从调用者的角度来统计比较困难。代码如下：
void count();
int main()
{
 int i;
 for (i = 1; i <= 3; i++)
  count();
  return 0;
}
void count()
{
 static num = 0;
 num++;
 printf(" I have been called %d",num,"timesn");
}
输出结果为：
I have been called 1 times.
I have been called 2 times.
I have been called 3 times.
本文来自：我爱研发网(52RD.com) - R&D大本营
详细出处：[http://www.52rd.com/Blog/Detail_RD.Blog_imjacob_5297.html](http://www.52rd.com/Blog/Detail_RD.Blog_imjacob_5297.html)
