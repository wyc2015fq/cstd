# Python中assert函数的用法小结 - 小灰笔记 - CSDN博客





2017年01月14日 23:26:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：13384
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









写程序的时候加上更加严谨的检查的习惯是我前不久刚刚学到的，之前纵然知道这很有用也很有必要，但是从来没有主动去尝试过。年前看了一本国外一个人写的C语言数据结构与算法讲义之后，觉得以后自己的软件中也有必要加上这种类似的检测，甚至说我常用的一些小脚本也有必要把这个加进去。

从15年下半年开始，我用的脚本语言Python居多，今天就大致总结一下Python中这种检测以及终止程序的方式。

Python中常用的一种方式是适用assert，最初在教程中看到有这个用法的时候只是浏览了一下就跳过了，感觉这个东西对我来说没用。后来学习MIT的6周公开课，简单的把这个功能学了一下，不过依然没有在我自己的代码中使用。后来大量使用已经是我前面提到的时间点以后了。

assert，通常在中文译本的教程活着书籍中翻译成断言。我觉得掌握这个常用的功能还得从文字具体的含义上来，不能够简单的记住这么一个名词或者是说法。断言，在中文的含义中看来是表达时分确定、十分肯定的含义。这样，这个函数后面参数对函数结果的影响也就明确了，只有当我十分确定的情况发生时才不会有错误。

编写如下代码测试：

  1 #!/usr/bin/python

  2 

  3 import os

  4 

  5 dir_info = os.listdir('./')

  6 assert('config.PNG'in dir_info)

  7 




代码测试，结果如下：

GreydeMac-mini:vim greyzhang$ vim assert_demo.py

GreydeMac-mini:vim greyzhang$ ls

assert_demo.pycode_after.PNGcode_before.PNGconfig.PNG

GreydeMac-mini:vim greyzhang$ python assert_demo.py 

GreydeMac-mini:vim greyzhang$ rm config.PNG 

GreydeMac-mini:vim greyzhang$ ls

assert_demo.pycode_after.PNGcode_before.PNG

GreydeMac-mini:vim greyzhang$ python assert_demo.py 

Traceback (most recent call last):

  File "assert_demo.py", line 6, in <module>

    assert('config.PNG' in dir_info)

AssertionError

GreydeMac-mini:vim greyzhang$ 




可以看出，当判断的文件存在时，使用断言说明文件存在程序运行没有任何错误。而删除断言中判断的存在文件之后，程序运行时候报错。实际上，断言的条件不成立时程序是直接报错并且终止执行。这不仅仅是保证程序运行可靠的一种方式，同时也算是一种程序问题定位的一种手段。因为程序运行停止时，错误信息会给出出现错误所在的代码行，而相应的条件都是程序员自己设定的，比较容易排查。



