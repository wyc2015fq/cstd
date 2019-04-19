# Nebula3中的委托(Delegate) - 逍遥剑客 - CSDN博客
2009年06月15日 21:58:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2249标签：[function																[callback																[编译器																[class																[c																[引擎](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
April2009增加了不少好东西, 这段时间准备静下心来看看
关于delegate这么个东西, 可能C++程序员很陌生, 但是Callback函数一定听说过吧? 其它两者本质上是一种东西
不过C++对于成员函数的回调实在是.....(别跟我提boost)
曾经用过[fastdelegate](http://www.codeproject.com/KB/cpp/FastDelegate.aspx), 确实不错. 我以为这就是C++中的完美实现了
N3中增加的[这个](http://www.codeproject.com/KB/cpp/ImpossiblyFastCppDelegate.aspx), 不比它差, 代码量更小, 呵呵
对于N3这个面向对象用到变态(同样它也极力的避免使用模板)的引擎来说, 这个简洁却又不简单的实现是最合适的. 虽然这个版本中它只是一个孤零零的类, 不保证以后会大量使用. 通常情况下, UI系统是它发挥最强力的地方吧?
两者的比较, 作者已经说得很明白了:
fastdelegate使用了一个"漏洞", 尽管它在许多C++编译器上都能工作, 但它不符合C++标准
而这个实现呢?跟boost::function和前者相比:
- 很快
- 不用动态分配内存
- 跟C++标准完全兼容
不过N3中只引入一个单参数无返回值的最简单模板, 有兴趣的可以下载[完整版本](http://www.codeproject.com/KB/cpp/ImpossiblyFastCppDelegate/SRDelegates.zip)自己研究.
写了一个测试程序, 相信一看就会用了:
#include "util/delegate.h"
using namespace Util;
typedef Delegate<int> PrintCallback;
void print(int i)
{
	n_printf("function delegate: %d/n", i);
}
class Console
{
public:
	void Print(int i)
	{
		n_printf("method delegate: %d/n", i);
	}
};
//------------------------------------------------------------------------------
/**
*/
void
main(int argc, const char** argv)
{
		PrintCallback funcPrint = PrintCallback::FromFunction<&print>();
		Console console;
		PrintCallback methPrint = PrintCallback::FromMethod<Console, &Console::Print>(&console);
		funcPrint(100);
		methPrint(200);
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090615/N3_delegate.JPG)
