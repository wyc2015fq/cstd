
# Java面试题整理（一） - manduner的博客 - CSDN博客


2018年03月06日 21:08:17[Manduner_TJU](https://me.csdn.net/manduner)阅读数：100


## 1，String，StringBuffer，StringBuilder相关知识
### （1）String,StringBuffer,StringBulider的区别与联系
（1）String一旦赋值，内容不可变；而StringBuffer,StringBuilder都是内容可变的
（2）StringBuffer是同步的，数据安全，效率低；StringBuilder是不同步的，数据不安全，效率高
（3）在多线程的情况下才会考虑同步与不同步以及安全与不安全问题，所以一般情况下StringBuilder较StringBuffer常用
### （2）StringBuffer（StringBulider）和数组的区别与联系
（1）StringBuffer和数组都是装数据的容器
（2）不管向StringBuffer中添加何种数据，最终其内容是一个字符串数据；数组可以存放多种数据，但一个数组中放置的数据必须是同一种类型的数据
### （3）String作为参数传递，效果和基本类型作为参数传递是一样的
### （4）形式参数
（1）基本类型：形式参数值的改变不影响实际参数的值（例如：int等）
（2）引用类型：形式参数值的改变直接影响实际参数的值（例如： 类）
注：关于形式参数基本类型与引用类型的区别与联系，推荐一篇如下博文，作者有详细的解释
http://blog.csdn.net/javazejian/article/details/51192130
## 2，Random类相关考点
构造方法Random()与Random(long seed)的区别
（1）public Random() 没有给种子，用的是默认种子（当前时间的毫秒值）
（2）public Random(long seed)给出指定的种子
区别：给定种子后，每次得到的随机数是相同的

