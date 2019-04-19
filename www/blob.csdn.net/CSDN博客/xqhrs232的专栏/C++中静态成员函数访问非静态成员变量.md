# C++中静态成员函数访问非静态成员变量 - xqhrs232的专栏 - CSDN博客
2013年11月14日 20:52:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：811
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/this-543273659/archive/2011/08/29/2157966.html](http://www.cnblogs.com/this-543273659/archive/2011/08/29/2157966.html)
### [C++中静态成员函数访问非静态成员变量](http://blog.csdn.net/jiahehao/article/details/3727352)
### 先看一个class
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class a
{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  static FunctionA()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     menber = 1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)private:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  int menber;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
编译上述代码，出错。原因很简单大家都知道，静态成员函数不能访问非静态成员，这是因为静态函数属于类而不是属于整个对象，静态函数中的 member可能都没有分配内存。静态成员函数没有隐含的this自变量。所以，它就无法访问自己类的非静态成员。（看过一篇很好的文章[《浅析C++中的this指针》](http://blog.csdn.net/starlee/archive/2008/01/24/2062586.aspx)介绍这个方面的详细内容）
那要想访问怎么办呢？地球人都知道只要将：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int menber;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//change the line above to:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)static int menber;
但是这个方法让我们不得不将static function内用到的成员变量都变成static的了，而且static的成员还要显式初始化，有没有更好的方法？答案是肯定的。代码说话：
class
 a
{
public:
  static FunctionA(a* _a)
  {
    _a-> menber = 1;
 cout<<_a-> menber<<endl;
 _a->f();
  }
void f()
{
 cout<<"f被调用了"<<endl;
}
private:
  int menber;
};
前提是这个类要分配了内存空间。其实这里我做的就是将一个对象指针作为静态成员函数的“this”指针，意在模仿传递非静态成员函数里this变量(在非静态成员函数中都有this指针(push在ECX中似乎))
这个想法是我在一个class中要CreateThread的时候产生的，因为Thread 的funtion都是要求static的（是这样的把？不知道为什么我写的code,Thread都是static的了。忘了哪儿看到的有这个要求了。有时间找找原因）。
可见C++是非常灵活的。
