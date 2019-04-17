# 高质量C++总结 - 工作笔记 - CSDN博客





2012年04月13日 14:57:05[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5558标签：[c++																[string																[delete																[float																[审查																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





**运算符规则**所有的一元运算符                               建议重载为成员函数

= () [] ->                                                 只能重载为成员函数

+= -= /= *= &= |= ~= %= >>= <<=       建议重载为成员函数

所有其它运算符                                   建议重载为全局函数


**不能被重载的运算符**

在 C++运算符集合中，有一些运算符是不允许被重载的。这种限制是出于安全方面

的考虑，可防止错误和混乱。

（1）不能改变C++内部数据类型（如int,float 等）的运算符。

（2）不能重载‘.’，因为‘.’在类中对任何成员都有意义，已经成为标准用法。

（3）不能重载目前C++运算符集合中没有的符号，如#,@,$等。原因有两点，一是难以

理解，二是难以确定优先级。

（4）对已经存在的运算符进行重载时，不能改变优先级规则，否则将引起混乱。



**Const 作用：提高程序效率，提高程序健壮性**

1.1    用const 修饰函数的参数 指针传递，保护指针不被改动 值传递 函数本身产生一个临时变量，无需保护，非内部数据类型的值传递，效率低，采用引用传递，提高效率，加const修饰，保护，对内部数据类型，无需这样，不存在构造，析构过程



1.2 用const 修饰函数的返回值

 A
如果给以“指针传递”方式的函数返回值加const 修饰，那么函数返回值（即指针）

的内容不能被修改，该返回值只能被赋给加const 修饰的同类型指针。

B
如果函数返回值采用“值传递方式”，由于函数会把返回值复制到外部临时的存储

单元中，加const 修饰没有任何价值。

如果返回值不是内部数据类型，将函数A GetA(void) 改写为const A & GetA(void)

的确能提高效率。但此时千万千万要小心，一定要搞清楚函数究竟是想返回一个对象的

“拷贝”还是仅返回“别名”就可以了，否则程序会出错

C
函数返回值采用“引用传递”的场合并不多，这种方式一般只出现在类的赋值函数

中，目的是为了实现链式表达。

1.3 修饰成员函数，提高程序的健壮性



**类构造函数，析构函数，拷贝构造函数，赋值函数的定义**

String::String(const char *str) //普通构造函数

{

 if (str == NULL)

 {

  m_data = new char[1];

  *m_data = '\0';

 } 

 else

 {

  int length = strlen(str);

  m_data = new char[length+1];

  strcpy(m_data, str);

 }

}

String::String(const String &other) //拷贝构造函数

{

 int length = strlen(other.m_data);

 m_data = new char[length + 1];

 strcpy(m_data, other.m_data);

}

String & String::operator =(const String &other)

{

 //检查自赋值

 if (this == &other)

 {

  return *this;

 }

 //释放原有的内存资源

 delete []m_data;


 //分配新的内存资源，并复制内容

 int length = strlen(other.m_data);

 m_data = new char[length+1];

 strcpy(m_data, other.m_data);

 //返回本对象的引用

 return *this;

}





【建议**11-3-14**】如果可能的话，使用**PC-Lint**、**LogiScope**等工具进行代码审查。](https://so.csdn.net/so/search/s.do?q=审查&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




