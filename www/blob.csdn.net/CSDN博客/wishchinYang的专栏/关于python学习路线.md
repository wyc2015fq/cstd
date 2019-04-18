# 关于python学习路线 - wishchinYang的专栏 - CSDN博客
2013年07月18日 14:53:31[wishchin](https://me.csdn.net/wishchin)阅读数：6600
## *[Python进阶(目录/书籍/学习路线)](http://blog.csdn.net/wklken/article/details/7907560)
忘了从哪里转的了，没办法标记哦，实在不好意思...
    目录:）
1. 简介&helloworld&开发环境配置
2.基本语法：标识符&变量&运算符
3.基本控制流
4.[基本数据结构1-列表](http://blog.csdn.net/wklken/article/details/8453107)
5.[基本数据结构2-元祖](http://blog.csdn.net/wklken/article/details/8654300)
6.[基本数据结构3-字典](http://blog.csdn.net/wklken/article/details/8654469)
7.[基本数据结构4-集合](http://blog.csdn.net/wklken/article/details/8656711)
5.数字处理  A.[数字基础](http://blog.csdn.net/wklken/article/details/8655102)  B.[相关扩展模块](http://blog.csdn.net/wklken/article/details/8655605)
6.字符串处理 A.[字符串基础](http://blog.csdn.net/wklken/article/details/6312870) B.格式化 C.正则表达式 D.编码相关
7.文件处理  A.文件基础  B.相关模块
8.异常处理
9.时间处理及相关模块
10.函数定义及参数传递
11.内建函数
12.模块和包
13.面相对象 A.基础 B.类 [C魔术方法（方法运算符重载）](http://blog.csdn.net/wklken/article/details/8126381)
14.函数式编程： A.基础  B.迭代器 C.生成器[D.装饰器](http://blog.csdn.net/wklken/article/details/8118942)
15.docstring
16.PyUnit及调试
17.垃圾回收
18.设计模式
19.元类
20.Pythonic最佳实践
21.性能最佳实践
22.命令行编程
23.多线程
24.Python相关要求
25.题目&学习路线&资源链接
PS:目测搞完后有些多，不会按照顺序发，写完一章会同步更新到目录。-20121005
     有任何问题回复到这个页面吧，其他页估计没空去一个个看!
    如果有兴趣学Python的话，可以看看。
    以下主要是一些学习过程中的梳理，希望有帮助。
1. 书籍列表：（列了下看过和没看过的一些书的看法）
<<Python cookbook>>
python2.4的，很多奇淫巧计，可以买一本
2.学习路线（针对以上书籍，可以自我调整和评估下，以下只代表个人观点）
[python][view plain](http://blog.csdn.net/wklken/article/details/7907560#)[copy](http://blog.csdn.net/wklken/article/details/7907560#)[print](http://blog.csdn.net/wklken/article/details/7907560#)[?](http://blog.csdn.net/wklken/article/details/7907560#)
1 #/usr/bin/env python  
2 #-*- coding=utf-8 -*-  
3 #@author: wklken@yeah.net  
4 #@version: 0.1  
5 #@date: 2012-08-25  
6 #@desc: python学习线路  
7   
8 step  
9 def read(book=<<A Byte of Python>>) #  网上先过一遍  
10     if 没兴趣:  
11       return  
12     else:  
13         if 没编程基础:  
14            <<Head First Python>>  
15   
16         if need py2.x:  
17            <<Python 核心技术>>  
18         elif py2.x and py3k:  
19            <<Learning Python>>  
20         if you want: #可选  
21            <<Python Tutorial>>  
22         if you have more time and energy:  
23            <<可爱的Python>>  
24            <<Programming Python>>  
25         print "Info: 基本入门了"  
26   
27         if you want go farther:  
28             if  True:#强烈建议  
29                 <<Dive Into Python>>  
30                 <<Python源码剖析>>   
31                 <<Python高级编程>> #这个，没读过，自己判定吧  
32             if  工作需要:  
33                 <<Python网络编程>>  #网络编程  
34                 <<Python在Unix和Linux系统管理中的应用>> #系统管理相关  
35                 <<The Django Book>> #web，用到django框架的  
36                 <<Django web开发指南>>  #同上  
37                 <<集体智慧编程>> #算法工程师，or 个人爱好  
38                 <<Python自然语言处理>>  
39   
40         if  you want to search for something useful:  
41             <<Python standard Library>>  
42             <<Python cookbook>>  
3.选用IDE:
这个，自己感觉用得顺手就行，关于这个，不详细介绍，网上自个搜
晒下我用的
windows下:   实验：IDLE     工程：Eclipse+pydev  （目前转用sublime text2了，有兴趣的可以试试）
Linux下:   实验: Ipython   工程: vim
4.学习方法：感觉入门无他：   看书+敲代码实验
买书的话，基本就行，貌似lz买多了，有部分感觉其实重复了
进阶，个人认为：  思考+不停写代码&重构
菜鸟阶段，继续敲代码中
5.关于这堆笔记
目标：python基本入门及进阶
基于版本： py2.7
参考书籍： 上面那堆书籍 + 网络的一些博文
整理频率：不定期哈，这个没法保证，1是比较懒，2是工作比较忙(好吧，这都是借口><)
声明：
1.本人系菜鸟一枚，这些权做分享，水平有限，错误难免，欢迎指正
2.文中引用会尽量注明，由于太杂，遗漏的希望指出，会注明.
3.转载注明出处哈，谢绝一切商业用途
补充声明：
不要浪费时间讨论语言之争，好坏优劣，自己喜欢就行
Life is short, so i use python!
The End, To Be Continue.......
wklken
2012-08-25
个人Blog: [http://wklken.sinaapp.com/](http://wklken.sinaapp.com/)
