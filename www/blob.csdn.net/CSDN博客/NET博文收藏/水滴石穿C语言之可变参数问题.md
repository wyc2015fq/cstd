
# 水滴石穿C语言之可变参数问题 - .NET博文收藏 - CSDN博客


2008年06月10日 12:51:00[hejishan](https://me.csdn.net/hejishan)阅读数：1349


|作者： 楚云风
|C语言中有一种长度不确定的参数，形如："…"，它主要用在参数个数不确定的函数中，我们最容易想到的例子是printf函数。
|原型：
|int printf( const char *format [, argument]... );
|使用例：
|printf("Enjoy yourself everyday!n");
|printf("The value is %d!n", value);
|这种可变参数可以说是C语言一个比较难理解的部分，这里会由几个问题引发一些对它的分析。
|注意：在C++中有函数重载（overload）可以用来区别不同函数参数的调用，但它还是不能表示任意数量的函数参数。
|问题：printf的实现
|请问，如何自己实现printf函数，如何处理其中的可变参数问题？ 答案与分析：
|在标准C语言中定义了一个头文件专门用来对付可变参数列表，它包含了一组宏，和一个va_list的typedef声明。一个典型实现如下：
|typedef char* va_list;
|\#define va_start(list) list = (char*)&va_alist
|\#define va_end(list)
|\#define va_arg(list, mode)
|((mode*) (list += sizeof(mode)))[-1]
|自己实现printf：
|\#include
|int printf(char* format, …)
|{
|va_list ap;
|va_start(ap, format);
|int n = vprintf(format, ap);
|va_end(ap);
|return n;
|}
|问题：运行时才确定的参数
|有没有办法写一个函数，这个函数参数的具体形式可以在运行时才确定？
|答案与分析：
|目前没有"正规"的解决办法，不过独门偏方倒是有一个，因为有一个函数已经给我们做出了这方面的榜样，那就是main()，它的原型是:
|int main(int argc,char *argv[]);
|函数的参数是argc和argv。
|深入想一下，"只能在运行时确定参数形式"，也就是说你没办法从声明中看到所接受的参数，也即是参数根本就没有固定的形式。常用的办法是你可以通过定义一个void *类型的参数，用它来指向实际的参数区，然后在函数中根据根据需要任意解释它们的含义。这就是main函数中argv的含义，而argc，则用来表明实际的参数个数，这为我们使用提供了进一步的方便，当然，这个参数不是必需的。
|虽然参数没有固定形式，但我们必然要在函数中解析参数的意义，因此，理所当然会有一个要求，就是调用者和被调者之间要对参数区内容的格式，大小，有效性等所有方面达成一致，否则南辕北辙各说各话就惨了。
|问题：可变长参数的传递
|有时候，需要编写一个函数，将它的可变长参数直接传递给另外的函数，请问，这个要求能否实现？
|答案与分析：
|目前，你尚无办法直接做到这一点，但是我们可以迂回前进，首先，我们定义被调用函数的参数为va_list类型，同时在调用函数中将可变长参数列表转换为va_list，这样就可以进行变长参数的传递了。看如下所示：
|void subfunc (char *fmt, va_list argp)
|{
|...
|arg = va_arg (fmt, argp); /* 从argp中逐一取出所要的参数 */
|...
|}
|void mainfunc (char *fmt, ...)
|{
|va_list argp;
|va_start (argp, fmt); /* 将可变长参数转换为va_list */
|subfunc (fmt, argp); /* 将va_list传递给子函数 */
|va_end (argp);
|...
|}
|问题：可变长参数中类型为函数指针
|我想使用va_arg来提取出可变长参数中类型为函数指针的参数，结果却总是不正确，为什么？
|答案与分析：
|这个与va_arg的实现有关。一个简单的、演示版的va_arg实现如下：
|\#define va_arg(argp, type)
|(*(type *)(((argp) += sizeof(type)) - sizeof(type)))
|其中，argp的类型是char *。
|如果你想用va_arg从可变参数列表中提取出函数指针类型的参数，例如
|int (*)()，则va_arg(argp, int (*)())被扩展为：
|(*(int (*)() *)(((argp) += sizeof (int (*)())) -sizeof (int (*)())))
|显然，（int (*)() *）是无意义的。
|解决这个问题的办法是将函数指针用typedef定义成一个独立的数据类型，例如：
|typedef int (*funcptr)()；
|这时候再调用va_arg(argp, funcptr)将被扩展为：
|(* (funcptr *)(((argp) += sizeof (funcptr)) - sizeof (funcptr)))
|这样就可以通过编译检查了。
|问题：可变长参数的获取
|有这样一个具有可变长参数的函数，其中有下列代码用来获取类型为float的实参：
|va_arg (argp, float);
|这样做可以吗？
|答案与分析：
|不可以。在可变长参数中，应用的是"加宽"原则。也就是float类型被扩展成double；char, short被扩展成int。因此，如果你要去可变长参数列表中原来为float类型的参数，需要用va_arg(argp, double)。对char和short类型的则用va_arg(argp, int)。
|问题：定义可变长参数的一个限制
|为什么我的编译器不允许我定义如下的函数，也就是可变长参数，但是没有任何的固定参数？
|int f (...)
|{
|...
|}
|答案与分析：
|不可以。这是ANSI C 所要求的，你至少得定义一个固定参数。
|这个参数将被传递给va_start()，然后用va_arg()和va_end()来确定所有实际调用时可变长参数的类型和值。
|
|



