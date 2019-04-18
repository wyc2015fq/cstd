# C++ new用法的正确方法应该如何操作？ - ljx0305的专栏 - CSDN博客
2012年09月19日 15:31:26[ljx0305](https://me.csdn.net/ljx0305)阅读数：462标签：[c++																[string																[buffer																[delete																[编译器																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
- 
C++ new用法的正确方法应该如何操作？
- 
在这篇文章中我们将会根据其两种不同的含义分别介绍一下它的影音方法，希望能给大家带来一些帮助。
[C++](http://developer.51cto.com/art/200509/3641.htm)编程语言中有一种叫做new的术语。不过这一概念是比较模糊的，有些人把它理解为new函数，但是有的人又会把它理解为new运算符。那么它的真正含义又是如何的呢？在这里我们将会通过对C++ new用法的介绍，帮助大家一起分析一下。
**C++ new用法之一 new运算符**
最常用的是作为运算符的new，比如：
string *str = new string(“test new”);
作为运算符，new和sizeof一样，是C++内置的，你不能对它做任何的改变，除了使用它。
new会在堆上分配一块内存，并会自动调用类的构造函数。
**C++ new用法之二 new函数**
第二种就是new函数，其实new运算符内部分配内存使用的就是new函数，原型是：
void *operator new(size_t size);
new函数返回的是一个void指针，一块未经初始化的内存。如你所见，这和C语言的malloc行为相似，你可以重载new函数，并且增加额外的参数，但是必须保证第一个参数必须是size_t类型，它指明了分配内存块的大小，C++允许你这么做，当然一般情况下这是不必要的。如果重载了new函数，在使用new操作符时调用的就是你重载后的new函数了。
如果使用new函数，和语句string *str = new string(“test new”)相对的代码大概是如下的样子：
- string *str = (string*)operator new(sizeof(string));  
- str.string(“test new”);   
- // 当然这个调用时非法的，但是编译器是没有这个限制的 
这还不算完，还有第三种的new存在。
**C++ new用法之三 placement new**
第三种，placement new，这也是new作为函数的一种用法，它允许你在一块已存在的内存上分配一个对象，而内存上的数据不会被覆盖或者被你主动改写，placement new同样由new操作符调用，调用格式是：
new (buffer) type(size_t size);
先看看下面的代码：
- char str[22];  
- int data = 123;  
- int *pa = new (&data) int;  
- int *pb = new (str) int(9); 
结果*pa = 123（未覆盖原数据），而*pb = 9（覆盖原数据），可以看到placement new 并没有分配新的内存，也可以使用在栈上分配的内存，而不限于堆。
为了使用placement new 你必须包含<new>或者<new.h>
其实placement new和第二种一样，只不过多了参数，是函数new的重载，语法格式为：
void *operator new(size_t, void* buffer);
它看起来可能是这个样子：
void *operator new(size_t, void* buffer) { return buffer;}
和new对应的就是delete了，需要回收内存啊，不然就泄漏了，这个下次再写吧，回忆一下今天的内容先。
**总结**
1. 函数new
void *operator new(size_t size); 在堆上分配一块内存，和placement new（void *operator new(size_t, void* buffer)）; 在一块已经存在的内存上创建对象，如果你已经有一块内存，placement new会非常有用，事实上，它STL中有着广泛的使用。
2. 运算符new
最常用的new，没什么可说的。
3. 函数new不会自动调用类的构造函数，因为它对分配的内存类型一无所知；而运算符new会自动调用类的构造函数。
4. 函数new允许重载，而运算符new不能被重载。
5. 紧接着就是对应的delete。
以上就是对C++ new用法的详细介绍。
