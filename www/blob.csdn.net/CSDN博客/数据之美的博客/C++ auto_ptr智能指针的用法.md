# C++ auto_ptr智能指针的用法 - 数据之美的博客 - CSDN博客
2016年11月09日 17:54:30[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：210标签：[C++																[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/oppo62258801/article/category/6512065)
**C++ auto_ptr****智能指针的用法**
C++中指针申请和释放内存通常采用的方式是new和delete。然而标准C++中还有一个强大的模版类就是auto_ptr，它可以在你不用的时候自动帮你释放内存。下面简单说一下用法。
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- 用法一：  
- std::auto_ptr<MyClass>m_example(new MyClass());  
- 
- 用法二：  
- std::auto_ptr<MyClass>m_example;  
- m_example.reset(new MyClass());  
- 
- 用法三（指针的赋值操作）：  
- std::auto_ptr<MyClass>m_example1(new MyClass());  
- std::auto_ptr<MyClass>m_example2(new MyClass());  
- m_example2=m_example1;  
![](http://static.blog.csdn.net/images/save_snippets.png)
用法一：
std::auto_ptr<MyClass>m_example(new MyClass());
用法二：
std::auto_ptr<MyClass>m_example;
m_example.reset(new MyClass());
用法三（指针的赋值操作）：
std::auto_ptr<MyClass>m_example1(new MyClass());
std::auto_ptr<MyClass>m_example2(new MyClass());
m_example2=m_example1; 
则C++会把m_example所指向的内存回收，使m_example1 的值为NULL，所以在C++中，应绝对避免把auto_ptr放到容器中。即应避免下列代码：
vector<auto_ptr<MyClass>>m_example;
当用[算法](http://lib.csdn.net/base/datastructure)对容器操作的时候，你很难避免STL内部对容器中的元素实现赋值传递，这样便会使容器中多个元素被置位NULL，而这不是我们想看到的。
虽然，标准auto_ptr智能指针机制很多人都知道，但很少使用它。这真是个遗憾，因为auto_ptr优雅地解决了C++设计和编码中常见的问题，正确地使用它可以生成健壮的代码。本文阐述了如何正确运用auto_ptr来让你的代码更加安全——以及如何避免对auto_ptr危险但常见的误用，这些误用会引发间断性发作、难以诊断的bug。
为什么称它为“自动”指针？auto_ptr只是众多可能的智能指针之一。许多商业库提供了更复杂的智能指针，用途广泛而令人惊异，从管理引用的数量到提供先进的代理服务。可以把标准C++ auto_ptr看作智能指针的Ford Escort（elmar注：可能指福特的一种适合家居的车型）：一个简易、通用的智能指针，它不包含所有的小技巧，不像专用的或高性能的智能指针那么奢华，但是它可以很好的完成许多普遍的工作，它很适合日常性的使用。
auto_ptr所做的事情，就是动态分配对象以及当对象不再需要时自动执行清理。这里是一个简单的代码示例，没有使用auto_ptr所以不安全：
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- // 示例1(a)：原始代码  
- void f()  
- {  
- T* pt( new T );  
- /*…更多的代码…*/
- delete pt;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
// 示例1(a)：原始代码  
void f()
{
T* pt( new T );
      /*…更多的代码…*/
      delete pt;
} 
我们大多数人每天写类似的代码。如果f()函数只有三行并且不会有任何意外，这么做可能挺好的。但是如果f()从不执行delete语句，或者是由于过早的返回，或者是由于执行函数体时抛出了异常，那么这个被分配的对象就没有被删除，从而我们产生了一个经典的内存泄漏。
能让示例1(a)安全的简单办法是把指针封装在一个“智能的”类似于指针的对象里，这个对象拥有这个指针并且能在析构时自动删除这个指针所指的对象。因为这个智能指针可以简单的当成一个自动的对象（这就是说，它出了作用域时会自动毁灭），所以很自然的把它称之为“智能”指针：
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- // 示例1(b)：安全代码，使用了auto_ptr
- void f()  
- {  
- auto_ptr<T> pt( new T );  
- /*…更多的代码…*/
- } // 酷：当pt出了作用域时析构函数被调用，从而对象被自动删除
![](http://static.blog.csdn.net/images/save_snippets.png)
// 示例1(b)：安全代码，使用了auto_ptr
void f()
{
auto_ptr<T> pt( new T );
      /*…更多的代码…*/
} // 酷：当pt出了作用域时析构函数被调用，从而对象被自动删除 
现在代码不会泄漏T类型的对象，不管这个函数是正常退出还是抛出了异常，因为pt的析构函数总是会在出栈时被调用，清理会自动进行。
最后，使用一个auto_ptr就像使用一个内建的指针一样容易，而且如果想要“撤销”资源，重新采用手动的所有权，我们只要调用release()。
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- // 示例2：使用一个auto_ptr
- void g()  
- {  
- // 现在，我们有了一个分配好的对象
- T* pt1 = new T;  
- // 将所有权传给了一个auto_ptr对象
- auto_ptr<T> pt2(pt1);  
- // 使用auto_ptr就像我们以前使用简单指针一样，
- *pt2 = 12;          // 就像*pt1 = 12
- pt2->SomeFunc(); // 就像pt1->SomeFunc();
- // 用get()来获得指针的值
- assert( pt1 == pt2.get() );  
- // 用release()来撤销所有权
- T* pt3 = pt2.release();  
- // 自己删除这个对象，因为现在没有任何auto_ptr拥有这个对象
- delete pt3;  
- } // pt2不再拥有任何指针，所以不要试图删除它…OK，不要重复删除
![](http://static.blog.csdn.net/images/save_snippets.png)
// 示例2：使用一个auto_ptr
void g()
{
// 现在，我们有了一个分配好的对象
T* pt1 = new T;
      // 将所有权传给了一个auto_ptr对象
auto_ptr<T> pt2(pt1);
      // 使用auto_ptr就像我们以前使用简单指针一样，
*pt2 = 12;          // 就像*pt1 = 12
pt2->SomeFunc(); // 就像pt1->SomeFunc();
      // 用get()来获得指针的值
assert( pt1 == pt2.get() );
      // 用release()来撤销所有权
T* pt3 = pt2.release();
      // 自己删除这个对象，因为现在没有任何auto_ptr拥有这个对象
delete pt3;
} // pt2不再拥有任何指针，所以不要试图删除它…OK，不要重复删除
 
最后，我们可以使用auto_ptr的reset()函数来重置auto_ptr使之拥有另一个对象。如果这个auto_ptr已经拥有了一个对象，那么，它会先删除已经拥有的对象，因此调用reset()就如同销毁这个auto_ptr，然后新建一个并拥有一个新对象：
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- // 示例 3：使用reset()
- void h()  
- {  
- auto_ptr<T> pt( new T(1) );  
- pt.reset( new T(2) );         // 删除由”new T(1)”分配出来的第一个T
- } // 最后pt出了作用域，第二个T也被删除了
![](http://static.blog.csdn.net/images/save_snippets.png)
// 示例 3：使用reset()
void h()
{
auto_ptr<T> pt( new T(1) );
pt.reset( new T(2) );         // 删除由”new T(1)”分配出来的第一个T
} // 最后pt出了作用域，第二个T也被删除了 
**auto_ptr****用法：**
1. 需要包含头文件<memory>。
2. Constructor：explicit auto_ptr(X* p = 0) throw(); 将指针p交给auto_ptr对象托管。
3. Copy constructor：auto_ptr(const auto_ptr&) throw(); template<class Y> auto_ptr(const auto_ptr<Y>& a) throw(); 指针的托管权会发生转移。
4. Destructor: ~auto_ptr(); 释放指针p指向的空间。
5. 提供了两个成员函数 X* get() const throw();//返回保存的指针
6. 对象中仍保留指针 X* release() const throw();//返回保存的指针，对象中不保留指针
**auto_ptr****实现关键点：**
1. 利用特点“栈上对象在离开作用范围时会自动析构”。
2. 对于动态分配的内存，其作用范围是程序员手动控制的，这给程序员带来了方便但也不可避免疏忽造成的内存泄漏，毕竟只有编译器是最可靠的。
3. auto_ptr通过在栈上构建一个对象a，对象a中wrap了动态分配内存的指针p，所有对指针p的操作都转为对对象a的操作。而在a的析构函数中会自动释放p的空间，而该析构函数是编译器自动调用的，无需程序员操心。
多说无益，看一个最实用的例子：
**[cpp]**[view plain](#)[copy](#)
[print](#)[?](#)
- #include <iostream>
- #include <memory>
- usingnamespace std;  
- class TC  
- {  
- public:  
-              TC(){cout<<”TC()”<<endl;}  
-              ~TC(){cout<<”~TC()”<<endl;}  
- };  
- void foo(bool isThrow)  
- {  
-      auto_ptr<TC> pTC(new TC);    // 方法2
- //TC *pTC = new TC;            // 方法1
- try
-      {  
- if(isThrow)  
- throw“haha”;  
-      }  
- catch(constchar* e)  
-      {  
- //delete pTC;           // 方法1
- throw;  
-      }  
- //delete pTC;              // 方法1
- }  
- int main()  
- {  
- try
-     {  
-           foo(true);  
-     }  
- catch(…)  
-     {  
-          cout<<”caught”<<endl;  
-     }  
-     system(”pause”);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
#include <iostream>
#include <memory>
using namespace std;
class TC
{
      public:
             TC(){cout<<”TC()”<<endl;}
             ~TC(){cout<<”~TC()”<<endl;}
};
void foo(bool isThrow)
{
     auto_ptr<TC> pTC(new TC);    // 方法2
     //TC *pTC = new TC;            // 方法1
     try
     {
                  if(isThrow)
                             throw “haha”;
     }
     catch(const char* e)
     {
                 //delete pTC;           // 方法1
                 throw;
     }
     //delete pTC;              // 方法1
}
int main()
{
    try
    {
          foo(true);
    }
    catch(…)
    {
         cout<<”caught”<<endl;
    }
    system(“pause”);
} 
1. 如果采用方案1，那么必须考虑到函数在因throw异常的时候释放所分配的内存，这样造成的结果是在每个分支处都要很小心的手动 delete pTC;。
2. 如果采用方案2，那就无需操心何时释放内存，不管foo()因何原因退出，栈上对象pTC的析构函数都将调用，因此托管在之中的指针所指的内存必然安全释放。
至此，智能指针的优点已经很明了了。
但是要注意使用中的一个陷阱，那就是指针的托管权是会转移的。例如在上例中，如果 auto_ptr<TC> pTC(new TC);   auto_ptr<TC> pTC1=pTC; 那么，pTC1将拥有该指针，而pTC没有了，如果再用pTC去引用，必然导致内存错误。
要避免这个问题，可以考虑使用采用了引用计数的智能指针，例如boost::shared_ptr等。auto_ptr不会降低程序的效率，但auto_ptr不适用于数组，auto_ptr根本不可以大规模使用。 shared_ptr也要配合weaked_ptr，否则会很容易触发循环引用而永远无法回收内存。理论上，合理使用容器加智能指针，C++可以完全避免内存泄露，效率只有微不足道的下降（中型以上程序最多百分之一）。

