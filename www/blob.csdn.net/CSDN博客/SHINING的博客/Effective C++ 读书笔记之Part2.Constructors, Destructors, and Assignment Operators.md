# Effective C++ 读书笔记之Part2.Constructors, Destructors, and Assignment Operators - SHINING的博客 - CSDN博客
2013年05月13日 16:36:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1429标签：[类																[对象																[文档																[语言																[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=对象&t=blog)](https://so.csdn.net/so/search/s.do?q=类&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
5.Know what functions C++ silently writes and calls.
总结：编译器可以暗自为class创建default构造函数、copy构造函数、copy assginment操作符，以及析构函数。这些函数都是public的，并且是inline的。
6.Explicitly disallow the use of compiler-generated functions you to not want.
总结：为驳回编译器自动（暗自）提供的机能，可将相应的成员函数声明为private并且不予实现。使用像Uncopyable这样的base class也是一种做法。
该条目主要是通过将copy 和copy assignment两个函数，声明为private并且不实现函数，这样编译器就不会默认产生这两个函数，并且也避免了对这两个函数的直接调用。
7.Declare destructors virtual in polymorphic base classes.
总结：
第一，polymorphic(带多态性质的)base classes 应该声明一个virtual析构函数。如果class带有任何virtual函数，它就应该拥有一个virtual析构函数。
第二，classes的涉及目的如果不是要作为base classes使用，或者不是为了具备多态性(polymorphically)，就不该声明virtual析构函数。
8.Prevent exception from leaving destructors.
总结：
第一，析构函数绝对不要吐出异常。如果一个被析构函数调用的函数可能抛出异常，析构函数应该捕捉任何异常，然后吞下他们（不传播）或结束程序。
第二，如果客户需要对某个操作函数运行期间抛出的一场作出反应，那么class应该提供一个普通函数（而非在析构函数中）执行该操作。
9.Never call virtual functions during construction or destruction.
总结：
在构造和析构期间不要调用virtual函数，因为这类调用从不下降致drived class（比起当前执行构造函数和析构函数那层）。
如果实在是想调用，则可以使用drived classes将必要的构造信息向上传递至base class构造函数。但是这时候调用的函数应改为非virtual函数。
10.Have assignment operators return a reference to *this.
总结：
令赋值（assignment)操作符返回一个reference to *this.
11. Handle assignment to self in operator=.
总结：
第一，确保当前对象自我赋值时operator=有良好行为。其中技术包括比较“来源对象”和“目标对象”的地址、精心周到的语句顺序、以及copy-and-swap。
第二，确定任何函数如果操作一个以上的对象，而其中多个对象是同一个对象时，其行为仍然正确。
12. Copy all parts of an object.
如果自己写copy函数，其中有部分成员未被copy，编译器不会提醒。即使是在最高的警告级别中。
总结：
第一，copying 函数应该确保复制“对象内的所有成员变量”及“所有base class成分”。
第二，不要尝试以某个copying函数实现另一个copying函数。应该将共同机能放进第三个函数中，并由两个copying函数共同调用。
感想：这部分内容主要是关于构造函数和析构函数的。因为编译器会默认给class生成构造函数、析构函数、copy和copy assignment函数。所以这就引出了一系列的问题，如果避免别人调用系统生成的这些函数而犯错，如何自己构造这些函数，构造这些函数的时候需要注意哪些问题。这些内容就构成了本部分的全部内容，提到的这些点确实是平时很容易犯错的点，学习了之后，很有利于提高代码的质量。


