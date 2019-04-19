# ctime的连续调用覆盖以前的值 - 三少GG - CSDN博客
2014年06月20日 13:27:44[三少GG](https://me.csdn.net/scut1135)阅读数：591
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
# ctime() 的连续调用覆盖以前的值
随后调用 ctime() 函数重写前一次调用的结果。下面的代码示例演示了此行为通过打印相同时间的开始时间和完成其第一个的 printf
 () 调用中的时间。但是，应该不同的开始和结束时间。若要更正这种情况下，**不要在一次调用 printf 两次调用 ctime**。完成时间则晚于开始时间，如预期的那样。
这是预期的行为。**Ctime() 函数使用一个静态缓冲区来存储其结果。**因此，当第二个
 ctime() 调用 printf 函数中，ctime() 会覆盖第一个 ctime() 调用返回的值。
/*
 * Compile options needed: None
 */ 
#include <stdio.h>
#include <time.h>
time_t start, finish=0;
void main(void)
{
  //loop until 3 seconds pass
  for (time(&start); finish-start < 3; time(&finish))
  ;
  /* If the following statement is used, the start and finish
     display times do not differ, both parameters get a copy of
     the same buffer which is overwritten twice before entering
     printf: */ 
  printf("Start time was %s and ending time was %s", ctime(&start),
     ctime(&finish));
  /* If the following statements are used instead, the start and
     finish times are different, as expected: */ 
     printf("Start time was %s", ctime(&start));
     printf("and ending time was %s\n", ctime(&finish));
}
