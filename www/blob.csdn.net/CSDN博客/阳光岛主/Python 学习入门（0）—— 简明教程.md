
# Python 学习入门（0）—— 简明教程 - 阳光岛主 - CSDN博客

2013年10月25日 20:24:29[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：27650所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



朋友问我怎么能快速地掌握Python。 我想Python包含的内容很多，加上各种标准库，拓展库，乱花渐欲迷人眼，就想写一个快速的Python教程，一方面 保持言语的简洁，另一方面循序渐进，尽量让没有背景的读者也可以从基础开始学习。另外，我在每一篇中专注于一个小的概念，希望可以让人可以在闲暇时很快读完。
**小提醒**
1. 教程将专注于Python基础，语法基于Python 2.7，测试环境为Linux, 不会使用到标准库之外的模块。
2. 我将专注于Python的主干，以便读者能以最快时间对Python形成概念。
3. Linux命令行将以 $ 开始，比如 $ls, $python
4. Python命令行将以 >>> 开始，比如 >>>print 'Hello World!'
5. 注释会以 \# 开始
**建议**
1. 将教程中的命令敲到Python中看看效果。
2. 看过教程之后，可以进行一些练习。
===============================================
[Python基础01 Hello World! ](http://www.cnblogs.com/vamei/archive/2012/05/28/2521650.html)
[Python基础02 基本数据类型 ](http://www.cnblogs.com/vamei/archive/2012/05/28/2522385.html)
[Python基础03 序列 ](http://www.cnblogs.com/vamei/archive/2012/05/28/2522677.html)
[Python基础04 运算 ](http://www.cnblogs.com/vamei/archive/2012/05/29/2524376.html)
[Python基础05 缩进和选择 ](http://www.cnblogs.com/vamei/archive/2012/05/29/2524706.html)
[Python基础06 循环 ](http://www.cnblogs.com/vamei/archive/2012/05/30/2526357.html)
[Python基础07 函数](http://www.cnblogs.com/vamei/archive/2012/06/01/2529500.html)
[Python基础08 面向对象的基本概念](http://www.cnblogs.com/vamei/archive/2012/06/02/2531515.html)
[Python基础09 面向对象的进一步拓展 ](http://www.cnblogs.com/vamei/archive/2012/06/02/2532018.html)
[Python基础10 反过头来看看](http://www.cnblogs.com/vamei/archive/2012/06/02/2532274.html)

[Python进阶01 词典 ](http://www.cnblogs.com/vamei/archive/2012/06/06/2537436.html)
[Python进阶02 文本文件的输入输出 ](http://www.cnblogs.com/vamei/archive/2012/06/06/2537868.html)
[Python进阶03 模块 ](http://www.cnblogs.com/vamei/archive/2012/07/03/2574436.html)
[Python进阶04 函数的参数传递 ](http://www.cnblogs.com/vamei/archive/2012/07/08/2581264.html)
[Python进阶05 循环设计 ](http://www.cnblogs.com/vamei/archive/2012/07/09/2582435.html)
[Python进阶06 循环对象 ](http://www.cnblogs.com/vamei/archive/2012/07/09/2582499.html)
[Python进阶07 函数对象 ](http://www.cnblogs.com/vamei/archive/2012/07/10/2582772.html)
[Python进阶08 错误处理 ](http://www.cnblogs.com/vamei/archive/2012/07/10/2582787.html)
[Python进阶09 动态类型 ](http://www.cnblogs.com/vamei/archive/2012/07/10/2582795.html)

**Python快速教程总结**
[Python补充01 序列的方法](http://www.cnblogs.com/vamei/archive/2012/07/19/2599940.html)
[Python补充02 Python小技巧](http://www.cnblogs.com/vamei/archive/2012/11/06/2755503.html)
[Python补充03 Python内置函数清单](http://www.cnblogs.com/vamei/archive/2012/11/09/2762224.html)
[Python补充04 Python简史](http://www.cnblogs.com/vamei/archive/2013/02/06/2892628.html)
[Python补充05 字符串格式化 (%操作符)](http://www.cnblogs.com/vamei/archive/2013/03/12/2954938.html)
[Python补充06 Python之道](http://www.cnblogs.com/vamei/p/3138446.html)
在大家的支持下，基本上完成了快速教程的连载。我从这个过程中学到很多内容。在Python的基础之后，Python标准库自然成为下一个挑战。Python标准库的重要性在于：
1） 它是Python的一部分。
2） Python的哲学是一个问题只有一个最好的解决方法，这些标准库就为相应的问题提供了一个标准的解决方案。
3） 许多重要Python项目，比如Django, Numpy, 也都需要借用标准库。了解标准库是继续学习的前提。
希望以此抛砖引玉，向大家学习。
Python标准库的学习的主要难度在于，需要一定的操作系统的基础知识。我之前读到的Python标准库的相关教程中，都有一个共同的问题，就 是假设你已经对操作系统和网络基础有所了解。当然，如果是在系统方面经验丰富就好，但是如果很生疏，那么学习Python标准库就有很大的困难。然而，全面地学习系统知识需要大量的精力和时间。出于此，我也增加一个Linux系统相关的贴，主要用于讨论Linux的基本概念和体系。这些概念和体系可以构成一个基本的框架，了解之后，可以比较从容地学习Python标准库。你也可以选择继续读标准库，直到需要补充这些知识的时候，根据Python标准库中给出的链接，来阅读这些系统相关的文章。[Linux](http://www.cnblogs.com/vamei/archive/2012/10/10/2718229.html)[概念与体系](http://www.cnblogs.com/vamei/archive/2012/10/10/2718229.html)
[Python标准库——走马观花](http://www.cnblogs.com/vamei/archive/2012/07/18/2597212.html)
[Python标准库的学习准备](http://www.cnblogs.com/vamei/archive/2012/07/23/2605345.html)
[Python标准库01 正则表达式 (re包)](http://www.cnblogs.com/vamei/archive/2012/08/31/2661870.html)
[Python标准库02 时间与日期 (time, datetime包)](http://www.cnblogs.com/vamei/archive/2012/09/03/2669426.html)
[Python标准库03 路径与文件 (os.path包, glob包)](http://www.cnblogs.com/vamei/archive/2012/09/05/2671198.html)
[Python标准库04 文件管理 (部分os包，shutil包)](http://www.cnblogs.com/vamei/archive/2012/09/14/2684775.html)
[Python标准库05 存储对象 (pickle包，cPickle包)](http://www.cnblogs.com/vamei/archive/2012/09/15/2684781.html)
[Python标准库06 子进程 (subprocess包)](http://www.cnblogs.com/vamei/archive/2012/09/23/2698014.html)
[Python标准库07 信号 (signal包)](http://www.cnblogs.com/vamei/archive/2012/10/06/2712683.html)
[Python标准库08 多线程与同步 (threading包)](http://www.cnblogs.com/vamei/archive/2012/10/11/2720042.html)
[Python标准库09 进程信息 (部分os包)](http://www.cnblogs.com/vamei/archive/2012/10/12/2721016.html)
[Python标准库10 多进程初步 (multiprocessing包)](http://www.cnblogs.com/vamei/archive/2012/10/12/2721484.html)
[Python标准库11 多进程探索 (multiprocessing包)](http://www.cnblogs.com/vamei/archive/2012/10/13/2722254.html)
[Python标准库12 数学与随机数 (math包，random包)](http://www.cnblogs.com/vamei/archive/2012/10/26/2741702.html)
[Python标准库13 循环器 (itertools)](http://www.cnblogs.com/vamei/p/3174796.html)
Python在网络方面有很多应用，无论是作为服务器端提供HTTP服务（比如豆瓣），还是作为客户端抓取网页。Python提供了到操作系统的 socket的接口，可以直接对socket编程。此外还有各个高级包可以实现不同的网络功能。然而，Python网络应用的最大优势在于Python所融合的自身优点，比如说语法简单，可读性高，有强大的脚本功能，这些让Python的开发者可以迅速开发出优秀的网络产品。
这里将只限Python标准库中提供的网络包。
[Python网络01 原始Python服务器](http://www.cnblogs.com/vamei/archive/2012/10/30/2744955.html)
[Python网络02 Python服务器进化 ](http://www.cnblogs.com/vamei/archive/2012/10/31/2747885.html)
到现在为止，如果你一直在跟随Python教程，我预期你已经可以写出一些有用的Python程序(甚至于比较大型的程序)。如果你已经达到了自己的目的，我想你的Python学习已经可以告一段落。(恭喜你) 当然，我会继续补充一些之前部分的内容，比如一些标准库的包，比如更多的Python网络应用等等。
在下面的部分中，我想深入讨论Python的体系。Python的语法很丰富，但这些语法都是建立在一个很简单的对象模型基础上的。这一部分的内 容并不是使用Python所必须的。但如果你不满足于一个Python使用者的定位，从事一些Python开发(比如制作Python工具，比如写一个框架)，你会希望对这一对象模型有进一步的了解。此外，我也会在这一部分补充说明前面遗留的一些话题。
[Python深入01 特殊方法与多范式](http://www.cnblogs.com/vamei/archive/2012/11/19/2772441.html)
[Python深入02 上下文管理器](http://www.cnblogs.com/vamei/archive/2012/11/23/2772445.html)
[Python深入03 对象的属性](http://www.cnblogs.com/vamei/archive/2012/12/11/2772448.html)
[Python深入04 闭包](http://www.cnblogs.com/vamei/archive/2012/12/15/2772451.html)
[Python深入05 装饰器 ](http://www.cnblogs.com/vamei/archive/2013/02/16/2820212.html)


**参考推荐：**
[简明 Python 教程](http://sebug.net/paper/python/)
[Python快速教程](http://www.cnblogs.com/vamei/archive/2012/09/13/2682778.html)
[The Python Tutorial](http://docs.python.org/2/tutorial/)
[python tutorial spoint](http://www.tutorialspoint.com/python/)



