# STL中mem_fun和mem_fun_ref的用法及区别 - Soul Joy Hub - CSDN博客

2016年06月13日 10:00:55[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：472


**原文：http://www.cppblog.com/mysileng/archive/2012/12/25/196615.html**
**引子:**

怎么对容器中的所有对象都进行同一个操作？我们可能首先想到的是用循环来实现。

比如有如下的一个类：

class ClxECS{

public:

    int DoSomething() { 

    cout << "Output from method DoSomething!" << endl; // 这里以输出一句话来代替具体的操作

    return 0;

    };

};

现在定义如下一个vector：

vector<ClxECS*> vECS;

for(int i = 0; i < 13; i++){

    ClxECS *pECS = new ClxECS;

    vECS.push_back(pECS);

}

 如果要对容器vECS中的所有对象都进行DoSomething()的操作，那么下面的循环可能是首先想到的方案：

for(int i = 0; i < vECS.size(); i++)

    vECS.at(i)->DoSomething();

 当然，我们也可以用iterator：

for(vector<ClxECS*>::iterator it = vECS.begin(); it != vECS.end(); ++it)

    (*it)->DoSomething();

 但是，有很多C++的高手和牛人们都会给我们一个忠告，那就是：在处理STL里面的容器的时候，尽量不要自己写循环。

那么，我们就只好用STL算法里面的for_each了。

首先，添加如下一个函数：

int DoSomething(ClxECS *pECS)

{

    return pECS->DoSomething();

}

然后就可以用for_each来实现我们想要的功能：

for_each(vECS.begin(), vECS.end(), &DoSomething);

说了半天，似乎跟mem_fun和mem_fun_ref没有什么关系。其实，说那么多都是为了引出mem_fun和mem_fun_ref。在用for_each的时候，如果我们不添加上面的那个函数，该怎么办呢？

这个时候就该mem_fun和mem_fun_ref隆重登场了。用如下这一行代码就行了：

**for_each(vECS.begin(), vECS.end(), mem_fun(&ClxECS::DoSomething));**

实际上就是由迭代器去调用成员函数.

**例子:**

一

list<Widget *> lpw;

for_each(lpw.begin(), lpw.end(),mem_fun(&Widget::test)); // pw->test();

二

vector<Widget> vw;

for_each(vw.begin(), vw.end(),mem_fun_ref(&Widget::test)); // w.test();

三

成员函数有参数的情况：将值传入，再bind1st为this

std::for_each(m_erased.begin(), m_erased.end(),std::bind1st(std::mem_fun(&SocketSet::_replace_with_last), this));

//相当于this->_replace_with_last(iter)  //iter

**两者区别:**

mem_fun_ref的作用和用法跟mem_fun一样，唯一的不同就是：

**当容器中存放的是对象实体的时候用mem_fun_ref，**

**当容器中存放的是对象的指针的时候用mem_fun。**

