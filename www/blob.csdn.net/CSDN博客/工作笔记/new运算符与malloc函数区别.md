# new运算符与malloc函数区别 - 工作笔记 - CSDN博客





2012年02月20日 15:26:57[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5565








相同点：

都可以进行内存管理。

不同点：

1、new 是c++中的操作符，malloc是c 中的一个函数

2、new 不止是分配内存，而且会调用类的构造函数，同理delete会调用类的析构函数，而malloc则只分配内存，不会进行初始化类成员的工作，同样free 也不会调用析构函数

3、内存泄漏对于malloc或者new都可以检查出来的，区别在于new可以指明是那个文件的那一行，而malloc没有这些信息。

4、new出来的指针是直接带类型信息的，而malloc返回的都是void指针。

5、两者用法不同

函数 malloc 的原型如下：

void * malloc(size_t size);

用 malloc 申请一块长度为length 的整数类型的内存，程序如下

int *p = (int *) malloc(sizeof(int) * length);

我们应当把注意力集中在两个要素上：“类型转换”和“sizeof”。




