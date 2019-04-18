# C++中的mutable关键字 - ljx0305的专栏 - CSDN博客
2008年04月20日 18:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：465标签：[c++																[output																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
**mutalbe**的中文意思是“可变的，易变的”，跟constant（既C++中的const）是反义词。
    在C++中，**mutable**也是为了突破const的限制而设置的。被**mutable**修饰的变量，将永远处于可变的状态，即使在一个const函数中。
    我们知道，如果类的成员函数不会改变对象的状态，那么这个成员函数一般会声明成const的。但是，有些时候，我们需要在const的函数里面修改一些跟类状态无关的数据成员，那么这个数据成员就应该被**mutalbe**来修饰。
    下面是一个小例子：
class ClxTest
{
public:
void Output() const;
};
void ClxTest::Output() const
{
    cout <<"Output for test!"<< endl;
}
void OutputTest(const ClxTest& lx)
{
    lx.Output();
}
    类ClxTest的成员函数Output是用来输出的，不会修改类的状态，所以被声明为const的。
    函数OutputTest也是用来输出的，里面调用了对象lx的Output输出方法，为了防止在函数中调用其他成员函数修改任何成员变量，所以参数也被const修饰。
    如果现在，我们要增添一个功能：计算每个对象的输出次数。如果用来计数的变量是普通的变量的话，那么在const成员函数Output里面是不能修改该变量的值的；而该变量跟对象的状态无关，所以应该为了修改该变量而去掉Output的const属性。这个时候，就该我们的**mutable**出场了——只要用**mutalbe**来修饰这个变量，所有问题就迎刃而解了。
    下面是修改过的代码：
class ClxTest
{
public:
    ClxTest();
~ClxTest();
void Output() const;
int GetOutputTimes() const;
private:
mutableint m_iTimes;
};
ClxTest::ClxTest()
{
    m_iTimes =0;
}
ClxTest::~ClxTest()
{
}
void ClxTest::Output() const
{
    cout <<"Output for test!"<< endl;
    m_iTimes++;
}
int ClxTest::GetOutputTimes() const
{
return m_iTimes;
}
void OutputTest(const ClxTest& lx)
{
    cout << lx.GetOutputTimes() << endl;
    lx.Output();
    cout << lx.GetOutputTimes() << endl;
}
    计数器m_iTimes被**mutable**修饰，那么它就可以突破const的限制，在被const修饰的函数里面也能被修改。
 转自 [http://blog.csdn.net/starlee/archive/2006/12/05/1430387.aspx](http://blog.csdn.net/starlee/archive/2006/12/05/1430387.aspx)
