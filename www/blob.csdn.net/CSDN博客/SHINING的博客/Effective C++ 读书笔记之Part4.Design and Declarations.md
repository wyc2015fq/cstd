# Effective C++ 读书笔记之Part4.Design and Declarations - SHINING的博客 - CSDN博客
2013年05月21日 15:44:51[snsn1984](https://me.csdn.net/snsn1984)阅读数：1414标签：[类																[语言																[入门																[读书笔记																[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=读书笔记&t=blog)](https://so.csdn.net/so/search/s.do?q=入门&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=类&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
18. Make interfaces easy to use correctly and hard to use incorrrectly.
总结：
第一，好的接口很容易被正确使用，不容易被误用。你应该在你的所有接口中努力达成这些性质。
第二，“促使正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容。
第三，“阻止误用”的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任。
第四，tr1::shared_ptr支持定制型删除器（custom deleter）。这可防范DLL问题，可被用来自动解除互斥锁等等。
19.Treat class design as type design.
1)新type的对象应该如何被创建和销毁？
2）对象的初始化和对象的赋值有什么样的差别？
3）新type的对象如果被passed by value(以值传递)，意味着什么？
4）什么是新type的“合法值”？
5）你的新type需要配合某个继承图系（inheritance graph）吗？
6）你的新type需要什么样的转换？
7）什么样的操作符和函数对此新type而言是合理的？
8）什么样的标准函数应该驳回？
9）谁该取用新type的成员？
10）什么是新type的“未声明接口”（undeclared interface）?
11）你的新type有多么一般化？
12）你真的需要一个新type吗？
总结：
Class的设计就是type的设计。在定义一个新type之前，请确定你已经考虑过本条款覆盖的所有讨论主题。
20.Prefer pass-by-reference-to-const to pass-by-value.
总结：
第一，尽量以pass-by-reference-to-const替换pass-by-value。前者通常比较高效，并可避免切割问题（slicing problem）。
第二，以上规则并不适用于内置类型，以及STL的迭代器和函数对象。对它们而言，pass-by-value往往比较适当。
21.Don't try to return a reference when you must return an object.
总结：
绝不要返回pointer或reference指向一个local stack对象，或返回reference指向一个heap-allocated对象，或返回pointer或reference指向一个local static对象而有可能同时需要多个这样的对象。条款4已经为“在单线程环境中合理返回reference指向一个local static对象”提供了一份设计实例。
22.Declare data members private.
总结：
第一，切记将成员变量声明为private。这可赋予客户访问数据的一致性、可细微划分访问控制、允诺约束条件获得保证，并提供class作者以充分的实现弹性。
第二，protected并不比public更具封装性。
23.Prefer non-member non-friend functions to member functions.
总结：
宁可拿non-member non-friend函数替换member函数。这样做可以增加封装性、包裹弹性（packaging flexibility）和技能扩充性。
24.Declare non-member functions when type conversions should apply to all parameters.
总结：
如果你需要为某个函数的所有参数（包括被this指针所指的那个隐喻参数）进行类型转换，那么这个函数必须是个non-member.
25.Consider support for a non-throwing swap.
总结：
第一，当std::swap对你的类型效率不高时，提供一个swap成员函数，并确定这个函数不抛出异常。
第二，如果你提供一个member swap，也该提供一个non-member swap用来调用前者。对于classes（而非templates），也请特化std::swap。
第三，调用swap时应针对std::swap使用using声明式，然后调用swap并且不带任何“命名空间资格修饰”。
第四，为“用户定义类型”进行std templates全特化是最好的，但是千万不要尝试在std内加入某些对std而言全新的东西。
感悟：这部分主要是介绍设计的时候需要注意的一些原则，总体来说都是在设计类的时候很容易遇见的问题。只是最后一个条款，关于swap的内容，平时用的很少。需要在用到的时候，重新再仔细看下这部分的内容。
