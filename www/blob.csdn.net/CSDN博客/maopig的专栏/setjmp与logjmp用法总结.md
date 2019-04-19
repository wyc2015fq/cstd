# setjmp与logjmp用法总结 - maopig的专栏 - CSDN博客
2011年12月21日 23:43:09[maopig](https://me.csdn.net/maopig)阅读数：1092
setjmp/logjmp的最大用途是错误恢复，只要还没有从主函数退出，一旦发现一个不可恢复的错误，可以把主控制转移大主函数循环，并从那从新开始。使用时必须包含头文件<setjmp.h>
setjmp与longjmp结合使用时，它们必须有严格的先后执行顺序，也即先调用setjmp函数，之后再调用longjmp函数，以恢复到先前被保存的“程序执行点”。(从哪儿来，回哪儿去）否则，如果在setjmp调用之前，执行longjmp函数，将导致程序的执行流变的不可预测，很容易导致程序崩溃而退出。
  setjmp与logjmp的使用场合:
1.人们对于goto语句的忌讳,很多的专业书籍以及专业人士号召限制goto语句的使用,此时,setjmp与longjmp对goto语句有了很好的替代作用.
2.goto语句有一个局限性,它只能在函数内部跳转.而setjmp与longjmp可以在整个程序全局中跳转,实现"长跳转",弥补了goto功能的局限.
3.使用setjmp和longjmp可以捕捉程序中的异常,并采取异常处理机制.
一个例子  使用setjmp,longjmp处理异常.
  1 #include<stdio.h>
  2 #include<setjmp.h>
  3 jmp_buf jumper;
  4 void exception();
  5 int deal_exception();
  6 
  7 main()
  8 {
  9         int value;
 10         int i=0;
 11 
 12         value=setjmp(jumper);
 13         if(0==value){
 14                 exception();
 15         }
 16         else{
 17                 switch(value)
 18                 {
 19                         case 1:
 20                                 printf("解决异常情况[%d]\n",value);
 21                                 break;
 22                         case 2:
 23                                  printf("解决异常情况[%d]\n",value);
 24                                  break;
 25                         case 3:
 26                                  printf("解决异常情况[%d]\n",value);
 27                                  break;
 28 
 29                         default:
 30
 31                                  break;
 32                 }
 33         }
 34 }
 35 void exception()
 36 {
 37         int _err_no;
 38         if(_err_no=3){
 39                 printf("出现异常情况[%d]\n",_err_no);
 40                 longjmp(jumper,_err_no);
 41         }
 42         return;
 43 }
结果：
[root@localhost test]# gcc error.c
[root@localhost test]# ./a.out
出现异常情况[3]
解决异常情况[3]
[root@localhost test]# 
### [C语言的try and catch机制](http://blog.csdn.net/e_zkz/article/details/2812376)
C语言具有的类似try and catch机制,unix下机制也相同。
VC例子：
#include <stdio.h>
#include <setjmp.h>
jmp_buf mark;  // 保存stack环境的变量 
void main(void)
{
int jmpret;
int result;
jmpret = setjmp( mark );  @1@ // 保存当前stack环境到mark变量
if( jmpret == 0 )
{
scanf("%d",&result);
if(result<0) {
longjmp( mark, -1 );  // 发生错误，恢复保存的stack环境，执行跳转到@1@处，jmpret的值为-1
return;
     // 此语句永远不被执行
} else {
printf("Normal!");
return;
}
} else {
// 错误处理
printf("An exception occured!");
}
}
longjmp在Standard C中的说明：
1.用longjmp跳转时保证静态变量有正确的值
2.auto变量只保证拥有volatile类型和在setjmp和longjmp之间没有改变的值。
解释：
1.静态变量保持为在longjmp调用时的值
2.在setjmp和longjmp之间没有改变的值仍然是原值，（很简单，因为从来就没有改变过）
对于一些auto-nonvolatile的值，可能在循环中，编译器为了优化性能，用寄存器来缓存，longjmp跳转时没有赋值，用volatile修饰能阻止编译器优化。
这两个函数都要包含头文件setjmp.h。而且它们在处理出现在深层函数嵌套的错误情况时很有用处。
    setjmp这个函数很有意思，虽然是一个函数，可是却可以返回两个不同的值。当第一次直接调用setjmp时，返回值为0。当从longjmp函数返回时，setjmp函数的返回值为longjmp的第二个参数的值。
    那么在什么地方调用setjmp呢？我们希望当从longjmp函数返回时，程序从哪里接着开始运行，我们就在哪里调用setjmp。看个小实例，你就明白是怎么回事了。
    #include<stdio.h>
    #include<setjmp.h>
    jmp_buf ebuf;
    void f2(void);
   int main(void)
   {
     int i;
     printf("1");
     i=setjmp(ebuf);
     if(i==0)                                              //第一次执行到这里时，值为0，所以接下来执行f2()
     {
   f2();
   printf("This will not be printed."); 
     }
     printf("%d",i);                                    //由于从longjmp返回时，i=3，不执行if，所以执行该行
     return 0;
}
   void f2(void)
{
   printf("2");                                          
   longjmp(ebuf,3);                     //longjmp函数返回，回到setjmp的位置，使得setjmp返回值为3
} 
    函数最后的执行结果为123，嘻嘻。
      longjmp注意：
　　1.不要假象寄存器类型的变量将总会保持不变。在调用longjmp之后，通过setjmp所返回的控制流中，例程中寄存器类型的变量将不会被恢复。
　　2.不要使用longjmp函数来实现把控制流，从一个中断处理例程中传出，除非被捕获的异常是一个浮点数异常。在后一种情况下，如果程序通过调用 _fpreset函数，来首先初始化浮点数包后，它是可以通过longjmp来实现从中断处理例程中返回。
　　3. 在C++程序中，小心对setjmp和longjmp的使用，应为setjmp和longjmp并不能很好地支持C++中面向对象的语义。因此在C++程序中，使用C++提供的异常处理机制将会更加安全。把setjmp和longjmp组合起来，原来它这么厉害
使用setjmp时必须使用头文件setjmp.h。
#include "setjmp.h"
jmp_buf jmpbuffer; 
int setjmp(jmp_buf jmpbuffer); 
void longjmp(jmp_buf jumpbuffer, int retval); 
其中 jmpbuffer 是相关的程序栈的环境上下文。 
初始化jmpbuffer之后， setjmp第一次调用的时候会返回 0。 
longjmp跳转到setjmp处，其中第二个参数retval就是传递给setjmp， 作为setjmp的返回值。但是需要主要的是，如果retval设置为0， 即这样调用的时候 longjmp(jumpbuffer, 0), setjmp会返回1。
函数setjmp()的行为很特别，如果直接调用它，它便将所有与当前处理器相关的状态信息（比如指令指针的内容，栈指针等）保存到jmp_buf中并返回0。在这种情况下，它的表现与一个普通的函数一样。然而，如果使用同一个jmp_buf调用longjmp()，则函数返回时就好像刚从setjmp()中返回时一样-----又回到刚刚从setjmp()返回的地方。这一次，返回值是调用longjmp()时所使用的第二个参数，因此可通过这个值判断程序是从longjmp()返回的
