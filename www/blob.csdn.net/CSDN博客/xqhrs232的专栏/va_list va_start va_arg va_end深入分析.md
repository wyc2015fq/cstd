# va_list/va_start/va_arg/va_end深入分析 - xqhrs232的专栏 - CSDN博客
2016年10月19日 14:40:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：194
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/justinzhang/archive/2011/09/29/2195969.html](http://www.cnblogs.com/justinzhang/archive/2011/09/29/2195969.html)
相关文章
1、关于C/C++中可变参数的详细介绍(va_list,va_start,va_arg,va_end)----[http://www.jb51.net/article/41868.htm](http://www.jb51.net/article/41868.htm)
va_list/va_start/va_arg/va_end这几个宏，都是用于函数的可变参数的。
我们来看看在vs2008中，它们是怎么定义的：
   1:  ///stdarg.h   2:  #define va_start _crt_va_start   3:  #define va_arg _crt_va_arg   4:  #define va_end _crt_va_end   5:      6:  ///vadefs.h   7:  #define _ADDRESSOF(v)   ( &reinterpret_cast<constchar &>(v) )   8:  typedef char *  va_list;   9:  #define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )  10:  #define _crt_va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )  11:  #define _crt_va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )  12:  #define _crt_va_end(ap)      ( ap = (va_list)0 )
再看看各个宏的功能是什么？
- va_list用于声明一个变量，我们知道函数的可变参数列表其实就是一个字符串，所以va_list才被声明为字符型指针，这个类型用于声明一个指向参数列表的字符型指针变量，例如：va_list ap;//ap:arguement pointer
- va_start(ap,v),它的第一个参数是指向可变参数字符串的变量，第二个参数是可变参数函数的第一个参数，通常用于指定可变参数列表中参数的个数。
- va_arg(ap,t),它的第一个参数指向可变参数字符串的变量，第二个参数是可变参数的类型。
- va_end(ap) 用于将存放可变参数字符串的变量清空（赋值为NULL).
> 
我们看一段具有可变参数列表的函数来求数组和的代码：
   1:  /*   2:  *   3:  *功能： 宏va_arg()用于给函数传递可变长度的参数列表。    4:  *首先，必须调用va_start() 传递有效的参数列表va_list和函数强制的第一个参数。第一个参数代表将要传递的参数的个数。    5:  *其次，调用va_arg()传递参数列表va_list 和将被返回的参数的类型。va_arg()的返回值是当前的参数。    6:  *再次，对所有的参数重复调用va_arg()    7:  *最后，调用va_end()传递va_list对完成后的清除是必须的。    8:  *   9:  *时间：2011年8月17日22:34:04  10:  *作者：张超  11:  *Email:uestczhangchao@gmail.com  12:  *  13:  */  14:    15:    16:  #include "X:\编程练习\C-C++\global.h"  17:    18:  #if va_arg==stdon  19:  #include <stdio.h>  20:  #include <stdarg.h>  21:  #include <stdlib.h>  22:    23:  //第一个参数指定了参数的个数  24:  int sum(int number,...)  25:  {  26:      va_list vaptr;  27:  int i;  28:  int sum = 0;  29:      va_start(vaptr,number);  30:  for(i=0; i<number;i++)  31:      {  32:          sum += va_arg(vaptr,int);  33:      }  34:      va_end(vaptr);  35:  return sum;  36:  }  37:    38:    39:  int main()  40:  {  41:      printf("%d\n",sum(4,4,3,2,1));  42:      system("pause");  43:  return 0;  44:  }  45:    46:  #endif
- va_start的功能是要把，ap指针指向可变参数的第一个参数位置处，
    #define _crt_va_start(ap,v) ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
   先取第一个参数的地址，在sum函数中就是取number的地址并且将其转化为char *的(因为char *的指针进行加减运算后，偏移的字节数才与加的数字相同, 如果为int *p,那么p+1实际上将p移动了4个字节)，然后加上4(__INITSIZEOF(number)=(4+3)&~3)，这样就将ap指向了可变参数字符串的第一个参数。
  #define _INTSIZEOF(n)
 ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int)
 - 1) )
> 
- 
以int所占的字节为标准进行对其操作。
- 如果int占四字节，则以四字节对齐为标准读取数据。
- va_arg是要从ap中取下一个参数。
       #define _crt_va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
对于这个宏，哥纠结了很久，最后终于搞清楚了，究其原因就是自己C语言功底不扎实，具体表现在没有搞清楚赋值表达式的值是怎么运作的。我们看这个宏,首先是ap = ap + __INTSIZEOF(t)。注意到，此时ap已经被改变了，它已经指向了下一个参数，我们令x=ap + __INTSIZEOF(t);那么括号内就变成了(x – __INTSIZEOF(t))，但是这里没有赋值运算符，所以ap的值没有发生变化，此时ap仍然指向的是当前参数的下一个参数的位置，也就是说ap指向的位置比当前正在处理的位置超前了一个位置。其实写成下面的形式就简单明了了:
    #define   va_arg(ap,t)   (*(t   *)((ap   +=   _INTSIZEOF(t)),   ap   -   _INTSIZEOF(t))   )
分析：为什么要将ap指向当前处理参数的下一个参数了？经过上面的分析，我们知道va_start(ap,v）已经将ap指向了可变参数列表的第一个参数了，以后我们每一步操作都需要将ap移动到下一个参数的位置，由于我们每次使用可变参数的顺序是:va_start(ap,v)—>va_arg(ap,t)；这样我们在第一次去参数的时候，其实ap已经指向了第二个参数开始的位置，所以我们用表达式的方式获得一个指向第一个参数的临时指针，这样我们就可以采用这种一致的方式来处理可变参数列表。（感觉没表达的十分清楚，希望各位朋友纠正~~~~~~）。下图是我的例子程序中去参数的情况（时间仓促，画得很丑，请原谅）：[](http://images.cnblogs.com/cnblogs_com/justinzhang/201109/20110929221542214.png)- va_end(ap)  将声明的ap指针置为空，因为指针使用后最后设置为空。
参考资料：- [http://topic.csdn.net/u/20110830/15/a3630fc4-3c5f-4a1e-bbee-949ba7b4cbe0.html](http://topic.csdn.net/u/20110830/15/a3630fc4-3c5f-4a1e-bbee-949ba7b4cbe0.html)
- [http://topic.csdn.net/u/20070120/12/e8b7363b-6404-4d91-9307-01e5ed996f3d.html](http://topic.csdn.net/u/20070120/12/e8b7363b-6404-4d91-9307-01e5ed996f3d.html)
