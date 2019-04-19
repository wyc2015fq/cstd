# Iterator模式详解--设计模式（22） - fanyun的博客 - CSDN博客
2016年07月08日 08:58:14[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1006标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Iterator模式来源：**
        迭代器（Iterator）模式，又叫做游标（Cursor）模式。GOF给出的定义为：提供一种方法访问一个容器（container）对象中各个元素，而又不需暴露该对象的内部细节。从定义可见，迭代器模式是为容器而生。
**Iterator模式作用：**
(1).它支持以不同的方式遍历一个聚合复杂的聚合可用多种方式进行遍历，如二叉树的遍历，可以采用前序、中序或后序遍历。迭代器模式使得改变遍历算法变得很容易: 仅需用一个不同的迭代器的实例代替原先的实例即可，你也可以自己定义迭代器的子类以支持新的遍历，或者可以在遍历中增加一些逻辑，如有条件的遍历等。
(2).迭代器简化了聚合的接口有了迭代器的遍历接口，聚合本身就不再需要类似的遍历接口了，这样就简化了聚合的接口。
(3).在同一个聚合上可以有多个遍历每个迭代器保持它自己的遍历状态，因此你可以同时进行多个遍历。
(4).此外，Iterator模式可以为遍历不同的聚合结构（需拥有相同的基类）提供一个统一的接口，即支持多态迭代。
简单说来，迭代器模式也是Delegate原则的一个应用，它将对集合进行遍历的功能封装成独立的Iterator，不但简化了集合的接口，也使得修改、增加遍历方式变得简单。从这一点讲，该模式与Bridge模式、Strategy模式有一定的相似性，但Iterator模式所讨论的问题与集合密切相关，造成在Iterator在实现上具有一定的特殊性
**Iterator模式UML结构图如图1所示：**
![](https://img-blog.csdn.net/20160708085028046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Iterator模式构成：**
(1).迭代器类（lterator）：迭代器类负责定义访问和遍历元素的接口。
(2).具体迭代器类（ConcreteIterator）：具体迭代器类要实现迭代器接口，并要记录遍历中的当前位置。
(3).容器类（Aggregate）：容器类负责提供创建具体迭代器类的接口。
(4).具体容器类（Concretelterator）：具体容器类实现创建具体迭代器类的接口——这个具体迭代器类与该容器的结构相关。
**Iterator模式使用场景：**
(1).访问一个容器对象的内容而无需暴露它的内部表示。
(2).支持对容器对象的多种遍历。
(3).为遍历不同的容器结构提供一个统一的[接口](http://baike.baidu.com/view/159864.htm)（多态迭代）。
**Iterator模式代码示例：**
**Iterator.h**
```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Iterator
{
public:
    Iterator(){};
    virtual ~Iterator(){};
    virtual string First() = 0;
    virtual string Next() = 0;
    virtual string GetCur() = 0;
    virtual bool IsEnd() = 0;
};
class Aggregate
{
public:
    virtual int Count() = 0;
    virtual void Push(const string& strValue)=0;
    virtual string Pop(const int nIndex)=0;
    virtual Iterator* CreateIterator() = 0;
};
class ConcreteIterator : public Iterator
{
public:
    ConcreteIterator(Aggregate* pAggregate):m_nCurrent(0),Iterator()
    {
        m_Aggregate = pAggregate;
    }
    string First()
    {
        return m_Aggregate->Pop(0);
    }
    string Next()
    {
        string strRet;
        m_nCurrent++;
        if(m_nCurrent < m_Aggregate->Count())
        {
            strRet = m_Aggregate->Pop(m_nCurrent);
        }
        return strRet;
    }
    string GetCur()
    {
        return m_Aggregate->Pop(m_nCurrent);
    }
    bool IsEnd()
    {
        return ((m_nCurrent >= m_Aggregate->Count()) ? true: false);
    }
private:
    Aggregate* m_Aggregate;
    int m_nCurrent;
};
class ConcreteAggregate : public Aggregate
{
public:
    ConcreteAggregate():m_pIterator(NULL)
    {
        m_vecItems.clear();
    }
    ~ConcreteAggregate()
    {
        if(NULL != m_pIterator)
        {
            delete m_pIterator;
            m_pIterator = NULL;
        }
    }
    Iterator* CreateIterator()
    {
        if(NULL == m_pIterator)
        {
            m_pIterator = new ConcreteIterator(this);
        }
        return m_pIterator;
    }
    int Count()
    {
        return m_vecItems.size();
    }
    void Push(const string& strValue)
    {
        m_vecItems.push_back(strValue);
    }
    string Pop(const int nIndex)
    {
        string strRet;
        if(nIndex < Count())
        {
            strRet = m_vecItems[nIndex];
        }
        return strRet;
    }
private:
    vector<string> m_vecItems;
    Iterator* m_pIterator;
};
```
**Main.cpp**
```cpp
#include "iterator.h"
int main()
{
    ConcreteAggregate* pName = NULL;
    pName = new ConcreteAggregate();
    if(NULL != pName)
    {
        pName->Push("hello");
        pName->Push("word");
        pName->Push("cxue");
    }
    Iterator* iter = NULL;
    iter = pName->CreateIterator();
    if(NULL != iter)
    {
        string strItem = iter->First();
        while(!iter->IsEnd())
        {
            cout << iter->GetCur() << " is ok" << endl;
            iter->Next();
        }
    }
    system("pause");
    return 0;
}
```
**Iterator****模式优缺点总结：**
Iterator模式优点：
(1).迭代器模式它将对集合进行遍历的功能封装成独立的Iterator，不但简化了集合的接口，也使得修改、增加遍历方式变得简单。
(2).避免了容器的细节暴露给外部访问者。使得设计符合“单一职责原则”。
(3).在迭代器模式中，具体迭代器角色和具体容器角色是耦合在一起的——遍历算法是与容器的内部细节紧密相关的。为了使客户程序从与具体迭代器角色耦合的困境中脱离出来，避免具体迭代器角色的更换给客户程序带来的修改，迭代器模式抽象了具体迭代器角色，使得客户程序更具一般性和重用性。这被称为[多态](http://baike.baidu.com/view/126521.htm)迭代。
Iterator模式缺点：
(1).在一般的底层集合支持类中，我们往往不愿“避轻就重”将集合设计成集合
 + Iterator 的形式，而是将遍历的功能直接交由集合完成，以免犯了“过度设计”的诟病，但是，如果我们的集合类确实需要支持多种遍历方式（仅此一点仍不一定需要考虑
 Iterator模式，直接交由集合完成往往更方便），或者，为了与系统提供或使用的其它机制，如STL算法，保持一致时，Iterator模式才值得考虑。
**Iterator模式使用总结：**
       如果系统涉及的容器较多，可以考虑使用迭代器模式，简化接口，如果系统较简单，直接用模板比较好。iterator模式的目的是提供一种方法访问一个容器对象中各个元素，而又不需暴露该对象的内部细节。也就是说提供一套可以访问不同容器的遍历方法而避免容器内部细节的暴露，也符合设计的“单一职责原则！
