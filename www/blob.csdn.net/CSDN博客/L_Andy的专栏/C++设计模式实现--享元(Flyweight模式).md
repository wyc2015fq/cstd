# C++设计模式实现--享元(Flyweight模式) - L_Andy的专栏 - CSDN博客

2014年06月30日 16:17:39[卡哥](https://me.csdn.net/L_Andy)阅读数：722


# 一、描述

设计模式中的享元模式，避免大量拥有相同内容的小类的开销(如耗费内存),使大家共享一个类(元类).

**问题**

在面向对象系统的设计何实现中,创建对象是最为常见的操作。这里面就有一个问题:如果一个应用程序使用了太多的对象,就会造成很大的存储开销。特别是对于大量轻量级(细粒度)的对象,比如在文档编辑器的设计过程中,我们如果为没有字母创建一个对象的话,系统可能会因为大量的对象而造成存储开销的浪费。例如一个字母“a”在文档中出现了100000 次,而实际上我们可以让这一万个字母“a”共享一个对象,当然因为在不同的位置可能字母“a”有不同的显示效果(例如字体和大小等设置不同),在这种情况我们可以为将对象的状态分为“外部状态”和“内部状态”,将可以被共享(不会变化)的状态作为内部状态存储在对象中,而外部对象(例如上面提到的字体、大小等)我们可以在适当的时候将外部对象最为参数传递给对象(例如在显示的时候,将字体、大小等信息传递给对象)。 



**其典型的结构图为: **


![](https://img-blog.csdn.net/20131023135047453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmV4dWVmZW5neWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



可以从图 2-1 中看出,Flyweight 模式中有一个类似 Factory 模式的对象构造工厂

FlyweightFactory,当客户程序员(Client)需要一个对象时候就会向 FlyweightFactory 发出请求对象的消息 GetFlyweight()消息,FlyweightFactory 拥有一个管理、存储对象的“仓库”(或者叫对象池,vector 实现),GetFlyweight()消息会遍历对象池中的对象,如果已经存在则直接返回给 Client,否则创建一个新的对象返回给 Client。当然可能也有不想被共享的对象(例如结构图中的
 UnshareConcreteFlyweight),但不在本模式的讲解范围,故在实现中不给出。 

# 二、实例

**如上所描述的信息，创建类图：**

![](https://img-blog.csdn.net/20131023135255312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmV4dWVmZW5neWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**本人的工程目录：**

![](https://img-blog.csdn.net/20131023135328296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmV4dWVmZW5neWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


注释：

main:客户程序员(Client)

FlyweightFactory:“仓库”(对象池), 

Flyweight：对象池中的对象

ConcreteFlyweight：被共享的对象

**代码：**

仓库”(对象池)：FlyweightFactory类

FlyweightFactory.h


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #ifndef __Flyweight__FlyweightFactory__
- #define __Flyweight__FlyweightFactory__
- 
- #include <iostream>
- #include "Flyweight.h"
- #include <string>
- #include <vector>
- #include "ConcreteFlyweight.h"
- using std::cout;  
- using std::endl;  
- using std::string;  
- using std::vector;  
- 
- class FlyweightFactory  
- {  
- public:  
-     FlyweightFactory();  
-     ~FlyweightFactory();  
-     Flyweight* GetFlyweight(const string &key);  
- private:  
-     vector<Flyweight*> _fly;  
- 
- };  
- 
- #endif /* defined(__Flyweight__FlyweightFactory__) */

FlyweightFactory.cpp


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #include "FlyweightFactory.h"
- 
- FlyweightFactory::FlyweightFactory()  
- {  
- 
- }  
- 
- FlyweightFactory::~FlyweightFactory()  
- {  
- 
- }  
- 
- Flyweight* FlyweightFactory::GetFlyweight(const string &key)  
- {  
-     vector<Flyweight*>::iterator it = _fly.begin();  
- 
- for (; it != _fly.end(); it++)  
-     {  
- if ((*it)->GetIntrinsicState() == key)  
-         {  
-             cout<<"already created by users...."<<endl;  
- return *it;  
-         }  
- 
-     }  
-     Flyweight *fn = new ConcreteFlyweight(key);  
- 
-     _fly.push_back(fn);  
- return fn;  
- 
- }  


对象池中的对象:Flyweight类

Flyweight.h


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #ifndef __Flyweight__Flyweight__
- #define __Flyweight__Flyweight__
- 
- #include <iostream>
- #include <string>
- using std::string;  
- 
- class Flyweight  
- {  
- public:  
-     Flyweight(void);  
- virtual ~Flyweight(void);  
- virtualvoid Operation(const string& extrinsicState);  
-     string GetIntrinsicState();  
- protected:  
-     Flyweight(string intrinsicState);  
- 
- private:  
-     string _intrinsicState;  
- 
- };  
- 
- 
- #endif /* defined(__Flyweight__Flyweight__) */

Flyweight.cpp


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #include "Flyweight.h"
- 
- Flyweight::Flyweight(void)  
- {  
- 
- }  
- 
- Flyweight::Flyweight(string intrinsicState)  
- {  
- this->_intrinsicState = intrinsicState;  
- }  
- 
- Flyweight::~Flyweight()  
- {  
- 
- }  
- 
- void Flyweight::Operation(const string &extrinsicState)  
- {  
- 
- }  
- 
- string Flyweight::GetIntrinsicState()  
- {  
- returnthis->_intrinsicState;  
- }  


被共享的对象:ConcreteFlyweight

ConcreteFlyweight.h


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #ifndef __Flyweight__ConcreteFlyweight__
- #define __Flyweight__ConcreteFlyweight__
- 
- #include <iostream>
- #include "Flyweight.h"
- class ConcreteFlyweight:public Flyweight  
- {  
- public:  
-     ConcreteFlyweight(void);  
-     ConcreteFlyweight(string intrinsicState);  
-     ~ConcreteFlyweight();  
- void Operation(const string& extrinsicState);  
- 
- };  
- 
- #endif /* defined(__Flyweight__ConcreteFlyweight__) */

ConcreteFlyweight.cpp


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #include "ConcreteFlyweight.h"
- using std::cout;  
- using std::endl;  
- 
- ConcreteFlyweight::ConcreteFlyweight(void)  
- {  
- 
- }  
- 
- ConcreteFlyweight::ConcreteFlyweight(string intrinsicState)  
- {  
-     cout<<"ConcreteFlyweight Build....."<<intrinsicState<<endl;  
- }  
- 
- ConcreteFlyweight::~ConcreteFlyweight()  
- {  
- 
- }  
- 
- void ConcreteFlyweight::Operation(const string &extrinsicState)  
- {  
-     cout<<"ConcreteFlyweight:内蕴["<<this->GetIntrinsicState()<<"] 外 蕴["<<extrinsicState<<"]"<<endl;  
- }  

客户程序员(Client)

main.cpp


**[cpp]**[view
 plain](http://blog.csdn.net/rexuefengye/article/details/12970207#)[copy](http://blog.csdn.net/rexuefengye/article/details/12970207#)

- #include <iostream>
- #include "Flyweight.h"
- #include "ConcreteFlyweight.h"
- #include "FlyweightFactory.h"
- usingnamespace std;  
- 
- 
- int main(int argc, constchar * argv[])  
- {  
- 
-     FlyweightFactory *fc = new FlyweightFactory();  
-     Flyweight* fw1 = fc->GetFlyweight("hello");  
-     Flyweight* fw2 = fc->GetFlyweight("world!");  
-     Flyweight* fw3 = fc->GetFlyweight("hello2");  
- // insert code here...
-     std::cout << "Hello, World!\n";  
- return 0;  
- }  

结果如下:

![](https://img-blog.csdn.net/20131023140739750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmV4dWVmZW5neWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


