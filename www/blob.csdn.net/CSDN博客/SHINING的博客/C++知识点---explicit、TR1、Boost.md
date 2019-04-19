# C++知识点---explicit、TR1、Boost - SHINING的博客 - CSDN博客
2013年05月09日 11:16:25[snsn1984](https://me.csdn.net/snsn1984)阅读数：1793标签：[C++																[编程																[语言																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
                
关键字：explicit
1、除非有明显的理由想要定义隐式转换，否则，单形参构造函数应该为explicit。
将构造函数设置为explicit可以避免错误，并且当转换有用时。用户可以显式地构造对象。
2、explicit关键字只能用于类内部的构造函数声明上。在类的定义体外部所做的定义上不再
重复它。
参考：《C++ Primer》 12.4.4
TR1
TRI ("Technical Report I") 是一份规范,描述加入 C++ 标准程序库的诸多新机
能。这些机能以新的 class templates 和 function templates 形式体现,针对的题目
有 hash tables, reference-counting smart pointers , regular expressions,以及更多。
所有 TRl 组件都被置于命名空间 tr1内,后者嵌套于命名空间 std 内。
参考： 《Effective C++》第三版中文版  导读部分
Boost
Boost 是个组织,亦是一个网站 (http://boost. org) ,提供可移植、同僚复审、源
码开放的 C++ 程序库。大多数 TRl 机能是以 Boost 的工作为基础。在编译器厂
商于其 C++ 程序库中含入 TRl 之前,对那些搜寻 TRl 实现品的开发人员而言,
Boost 网站可能是第一个逗留点。 Boost 提供比 TRl 更多的东西,所以无论如何
值得了解它。
参考： 《Effective C++》第三版中文版  导读部分

