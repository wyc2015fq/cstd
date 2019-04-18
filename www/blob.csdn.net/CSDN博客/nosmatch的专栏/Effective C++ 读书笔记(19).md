# Effective C++ 读书笔记(19) - nosmatch的专栏 - CSDN博客
2013年07月13日 15:42:33[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：450
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款19：设计class犹如设计type
> 
条款主要的是介绍在设计class的时候应该注意的问题，只要有以下几点：
1.新的type的对象应该如何被构造和析构，主要是指的新的type的构造函数和析构函数的定义
2.对象的初始化和对象的赋值有什么样的区别，主要是新type的复制构造函数和赋值函数的定义
3.新的type的对象如果被passed by value 将以为这什么？
4.什么是新的type的“合法值”，在class类中包含各种的成员变量，这些成员变量的取值范围是什么？
5.你的新的type需要配合每个继承图系吗？主要指type是否是从被的class继承，还有就是该type会否会称为别的class的父类
6.你的新type需要什么类型的转化，主要是指的隐式转化和显示转化
7.什么样的操作符合函数对该新type来说是合理的？主要是指的该type需要定义的member函数
8.谁该取用新的type的成员，主要是新type的public/private/protected函数或者friend函数
9.什么是新的type的“未声明接口”
10.你的type有多么的一般化？如果他是一个type的家族，那么可以试着写下class template
11.是否真的需要一个type类型？如果定义dervie class 只是为base class添加新的技能，那么可以优先考虑non-member和
friend函数
请记住：
- class设计即使type的设计，在定义一个新的type的时候，请确定你已经考虑过本条款所讨论的主题
