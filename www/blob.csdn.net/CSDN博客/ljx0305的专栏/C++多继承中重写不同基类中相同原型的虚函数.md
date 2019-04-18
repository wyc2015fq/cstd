# C++多继承中重写不同基类中相同原型的虚函数 - ljx0305的专栏 - CSDN博客
2008年11月09日 21:09:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：585标签：[c++																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
                   在C++多继承体系当中，在派生类中可以重写不同基类中的虚函数。下面就是一个例子：
- class CBaseA
- {
- public:
- virtualvoid TestA();
- };
- 
- class CBaseB
- {
- public:
- virtualvoid TestB();
- };
- 
- class CDerived : public CBaseA, public CBaseB
- {
- public:
- virtualvoid TestA(); // 重写基类CBaseA中的虚函数TestA() 
- virtualvoid TestB(); // 重写基类CBaseB中的虚函数TestB() 
- };
- 
- void Test()
- {
-     CDerived D;
-     CBaseA *pA = &D;
-     CBaseB *pB = &D;
- 
-     pA->TestA(); // 调用类CDerived的TestA()函数 
-     pB->TestB(); // 调用类CDerived的TestB()函数 
- }
    可是，如果两个基类中有一个相同原型的虚函数，例如下面这样：
- class CBaseA
- {
- public:
- virtualvoid Test();
- };
- 
- class CBaseB
- {
- public:
- virtualvoid Test();
- };
    怎样在派生类中重写这两个相同原型的虚函数呢？
    也许这种情况并不常见，可是这种情况却确实存在。比如说开发的时候使用的两个类库是不同的厂商提供的，或者说这两个类库是由公司的不同开发小组开发的。对前者来说，修改基类的接口是不可能的；对后者来说，修改接口的代价很大。
    如果在派生类中直接重写这个虚函数，那么2个基类的Test()虚函数都将被覆盖。这样的话就只能有一个Test()的实现，而不是像前面的例子那样有不同的实现。
- class CDerived : public CBaseA, public CBaseB
- {
- public:
- virtualvoid Test();
- };
- 
- void Test()
- {
-     CDerived D;
-     CBaseA *pA = &D;
-     CBaseB *pB = &D;
- 
- // 下面2行代码都将调用类CDerived的Test()函数 
-     pA->Test();
-     pB->Test(); 
- }
    为了实现第一个例子中的那样，在派生类CDerived中重写不同基类中相同原型的虚函数Test()，可以使用下面的方法。
    首先，不需要对2个基类进行任何修改(在实际的开发当中，修改基类的可能性非常小)。
- class CBaseA
- {
- public:
- virtualvoid Test();
- };
- 
- class CBaseB
- {
- public:
- virtualvoid Test();
- };
    现在，为这个继承体系添加2个中间类，分别从2个基类派生。
- class CMiddleBaseA : public CBaseA
- {
- private:
- // 真正的实现函数 
- // 设置为纯虚函数，在派生类里必须实现 
- virtualvoid CBaseA_Test() = 0;
- 
- // 改写继承下来的虚函数 
- // 仅仅直接调用真正的实现函数 
- virtualvoid Test() 
-     { 
-         CBaseA_Test(); 
-     }
- };
- 
- // 与类CMiddleBaseA采用相同的方法 
- class CMiddleBaseB : public CBaseB
- {
- private:
- virtualvoid CBaseB_Test() = 0;
- 
- virtualvoid Test() 
-     { 
-         CBaseB_Test(); 
-     }
- };
    然后，类CDerived以上面2个中间类作为基类来派生。分别重写上面2个基类中原型不同的纯虚函数，添加不同的实现代码。
- class CDerived : public CMiddleBaseA, public CMiddleBaseB
- {
- private:
- // 重写从中间类继承下来的虚函数 
- virtualvoid CBaseA_Test(); // 这里实际上是重写CBaseA的Test() 
- virtualvoid CBaseB_Test(); // 这里实际上是重写CBaseB的Test() 
- };
- 
- void Test()
- {
-     CDerived D;
-     CBaseA *pA = &D;
-     CBaseB *pB = &D;
- 
- // 调用类CBaseA的Test()函数 
- // 由于C++多态的特性，实际上调用的是类CDervied中的CBaseA_Test()函数 
-     pA->Test(); 
- 
- // 调用类CBaseB的Test()函数 
- // 由于C++多态的特性，实际上调用的是类CDervied中的CBaseB_Test()函数 
-     pB->Test();
- }
    现在以上面代码中的pA->Test();这行代码来说明上面的方案是怎么实现的。
    首先，由于虚函数Test()在类CBaseA的派生类CMiddleBaseA中被重写，所以这行代码会去调用类CMiddleBaseA的Test()函数；
    然后，类CMiddleBaseA的Test()函数会去调用实现函数CBaseA_Test()；
    最后，由于虚函数CBaseA_Test()在类CMiddleBaseA的派生类CDerived中被重写，所以真正调用的是类CDerived中的CBaseA_Test()函数。
    同样的道理，代码pB->Test();实际上调用的是类CDervied中的CBaseB_Test()函数。
    通过上面的方法就可以在C++多继承中重写不同基类中相同原型的虚函数。
引用：[http://blog.csdn.net/starlee/archive/2008/08/25/2825522.aspx](http://blog.csdn.net/starlee/archive/2008/08/25/2825522.aspx)
