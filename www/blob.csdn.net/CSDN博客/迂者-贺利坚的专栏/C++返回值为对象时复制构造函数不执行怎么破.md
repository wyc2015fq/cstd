
# C++返回值为对象时复制构造函数不执行怎么破 - 迂者-贺利坚的专栏 - CSDN博客

2016年03月25日 11:42:43[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：5012所属专栏：[代码侦探](https://blog.csdn.net/column/details/bugkiller.html)



先说点背景知识，调用复制构造函数的三种情况：
1.当用类一个对象去初始化另一个对象时。
2.如果函数形参是类对象。
3.如果函数返回值是类对象，函数执行完成返回调用时。
在辅导学生上机时，有同学第3点提出异议。有教材上的例题为证：

```python
#include <iostream>
using namespace std;
class Point     //Point 类的定义
{
public:
    Point(int xx=0, int yy=0)
    {
        x = xx;    //构造函数，内联
        y = yy;
    }
    Point(const Point& p); //复制构造函数
    void setX(int xx)
    {
        x=xx;
    }
    void setY(int yy)
    {
        y=yy;
    }
    int getX() const
    {
        return x;    //常函数（第5章）
    }
    int getY() const
    {
        return y;    //常函数（第5章）
    }
private:
    int x, y; //私有数据
};
//成员函数的实现
Point::Point (const Point& p)
{
    x = p.x;
    y = p.y;
    cout << "Calling the copy constructor " << endl;
}
//形参为Point类对象的函数
void fun1(Point p)
{
    cout << p.getX() << endl;
}
//返回值为Point类对象的函数
Point fun2()
{
    Point a(1, 2);
    return a;
}
//主程序
int main()
{
    Point a(4, 5);	//第一个对象A
    Point b = a;	//情况一，用A初始化B。第一次调用复制构造函数
    cout << b.getX() << endl;
    fun1(b);	//情况二，对象B作为fun1的实参。第二次调用复制构造函数
    b = fun2();	//情况三，函数的返回值是类对象，函数返回时调用复制构造函数
    cout << b.getX() << endl;
    return 0;
}
```
证据是，在CodeBlocks中，运行结果是：![](https://img-blog.csdn.net/20160325111852011)
而不是期望的：
![](https://img-blog.csdn.net/20160325111736588)
显然，第3种情况下，复制构造函数没有被执行。
确定问题后，我知道道理是对的，看过的几本书，厚的、薄的，都是这么写的。会不会是编译器的差别？CodeBlocks用的是gcc。gcc开源，跟标准的变化跟得紧，莫不是第3种情况已经成了老黄历，而各种书来不及变？
我让她到VC++6.0中运行。一会儿她的反馈，在VC++6.0中复制构造函数执行了。
真相明白了。
这个问题需要有个交待。
回家后再翻各种书，无果。网络搜索，CSDN上有个贴子[《函数返回值是对象，是调用了拷贝构造函数？》](http://bbs.csdn.net/topics/390803716)，其中大家给的结论，是gcc做了优化，返回值为对象时，不再产生临时对象，因而不再调用复制构造函数。
看来不是标准发生变化。
那如果一定想要让这个构造函数执行呢？只需让忽略gcc不要搞这个优化就行了。贴子中给了个线索，在新浪博客《[命名返回值优化](http://blog.sina.com.cn/s/blog_4ab8464c0100kybj.html)》。文章称通过搜索知道“这是一个称为命名返回值优化的问题，而且g++的这个编译优化竟然没有直接的关闭方法给出解决办法”。作者是用命令行工作的，他后来解决的办法，是在编译命令中加上“-fno-elide-constructors”参数，例g++ -fno-elide-constructors testReturn.cpp 。
我的学生还处在用IDE的阶段。本文的价值来了，如何在CodeBlocks下也忽略这个优化项呢？
在CodeBlocks中，通过菜单依次选：settings->Compiler...，在Global compiler settings部分，选择Other options，在文本框中写入“-fno-elide-constructors”，如图，然后就可以ok啦。
![](https://img-blog.csdn.net/20160325113847412)
然后，如同苦难的公主终于和王子过上了幸福的生活一样，期望的结果有了。
![](https://img-blog.csdn.net/20160325111736588)
“-fno-elide-constructors”选项起了作用，有图为证。下面中加上这个参数后，编译完看到的提示信息：
![](https://img-blog.csdn.net/20160325114154431)

