# typedef和#define的用法与区别 - ljx0305的专栏 - CSDN博客
2008年04月14日 23:25:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：470标签：[编译器																[扩展																[语言																[任务																[工具																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
**一、typedef的用法**
在C/C++语言中，typedef常用来定义一个标识符及关键字的别名，它是语言编译过程的一部分，但它并不实际分配[内存](http://action.vogate.com/c/c.php?r=&aid=2473&sid=6235007045036118&click=1&url=http%3A//www.zfjd.cn&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=591679&k=%25u5185%25u5B58)[空间](http://action.vogate.com/c/c.php?r=&aid=4523&sid=6235007045036118&click=1&url=http%3A//www.qqdiao.cn&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=805075&k=%25u7A7A%25u95F4)，实例像：
typedef    int       INT;
typedef    int       ARRAY[10];
typedef   (int*)   pINT;
typedef可以增强程序的可读性，以及标识符的灵活性，但它也有“非直观性”等缺点。
**二、#define的用法**
#define为一宏定义语句，通常用它来定义常量(包括无参量与带参量)，以及用来实现那些“表面似和善、背后一长串”的宏，它本身并不在编
译过程中进行，而是在这之前(预处理过程)就已经[完成](http://action.vogate.com/c/c.php?r=&aid=4492&sid=6235007045036118&click=1&url=http%3A//www.zhcw.com&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=782107&k=%25u5B8C%25u6210)了，但也因此难以发现潜在的错误及其它[代码](http://action.vogate.com/c/c.php?r=&aid=5059&sid=6235007045036118&click=1&url=http%3A//www.qzxiu.com&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=218043&k=%25u4EE3%25u7801)维护[问题](http://action.vogate.com/c/c.php?r=&aid=4501&sid=6235007045036118&click=1&url=http%3A//buytech.searchina.net.cn&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=128241&k=%25u95EE%25u9898)，它的实例像：
#define   INT             int
#define   TRUE         1
#define   Add(a,b)     ((a)+(b));
#define   Loop_10    for (int i=0; i<10; i++)
在Scott Meyer的Effective C++一书的条款1中有关于#define语句弊端的[分析](http://action.vogate.com/c/c.php?r=&aid=4606&sid=6235007045036118&click=1&url=http%3A//cn.ibtimes.com/artsinvestment/&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=253082&k=%25u5206%25u6790)，以及好的替代方法，[大家](http://action.vogate.com/c/c.php?r=&aid=5056&sid=6235007045036118&click=1&url=http%3A//www.5ya.cn/peixun/showkc.asp%3Fid%3D3904&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=422600&k=%25u5927%25u5BB6)可参看。
**三、typedef与#define的区别**
从以上的概念便也能基本清楚，typedef只是为了增加可读性而为标识符另起的新名称(仅仅只是个别名)，而#define原本在C中是为了定义常量
，到了C++，const、enum、inline的出现使它也渐渐成为了起别名的[工具](http://action.vogate.com/c/c.php?r=&aid=4744&sid=6235007045036118&click=1&url=http%3A//www.cnzz.cn&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=226432&k=%25u5DE5%25u5177)。有时很容易搞不清楚与typedef两者到底该用哪个好，如#define 
INT int这样的语句，用typedef一样可以完成，用哪个好呢？我主张用typedef，因为在早期的许多C编译器中这条语句是非法的，只是现今的
编译器又做了扩充。为了尽可能地兼容，一般都遵循#define定义“可读”的常量以及一些宏语句的任务，而typedef则常用来定义关键字、冗
长的类型的别名。
宏定义只是简单的字符串代换(原地扩展)，而typedef则不是原地扩展，它的新名字具有一定的封装性，以致于新命名的标识符具有更易定义变
量的[功能](http://action.vogate.com/c/c.php?r=&aid=5485&sid=6235007045036118&click=1&url=http%3A//www.958shop.com/&v=0&s=http%3A//www.sf.org.cn/Article/base/200608/18988.html&rn=978450&k=%25u529F%25u80FD)。请看上面第一大点代码的第三行：
typedef    (int*)      pINT;
以及下面这行:
#define    pINT2    int*
效果相同？实则不同！实践中见差别：pINT a,b;的效果同int *a; int *b;表示定义了两个整型指针变量。而pINT2 a,b;的效果同int *a, b;
表示定义了一个整型指针变量a和整型变量b。
注意：两者还有一个行尾;号的区别哦！
