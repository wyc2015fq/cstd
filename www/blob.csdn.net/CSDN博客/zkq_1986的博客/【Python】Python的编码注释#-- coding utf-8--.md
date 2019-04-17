# 【Python】Python的编码注释# -*- coding:utf-8 -*- - zkq_1986的博客 - CSDN博客





2018年08月20日 23:05:03[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：51








# [Python的编码注释# -*- coding:utf-8 -*-](http://www.cnblogs.com/scofi/p/4868012.html)

# -*- coding:utf-8 -*-的主要作用是指定文件编码为utf-8, 因为一般默认的是ASCII码，如果要在文件里面写中文，运行时会出现乱码，加上这句之后会把文件编码强制转换为utf-8运行时会就不会出现乱码了。

声明的语法参考python的PEP http://www.python.org/dev/peps/pep-0263/

主要内容如下：

1.必须将编码注释放在第一行或者第二行

2.可选格式有

# coding=<encoding name>  
# -*- coding: <encoding name> -*-  
# vim: set fileencoding=<encoding name> :  
3. 标准中给出了只要满足下面这个正则表达式的字符串都有效：

\%^.*\n.∗\?#.*coding[:=]\s*[0-9A-Za-z-_.]\+.*$  
其中的意思就是必须包含#，且#号之前不能有字符换行等其他字符，字符串中必须包含coding后面可以跟:或者=接下来就是编码名称。





转载自：[https://blog.csdn.net/freemoses77/article/details/79466762](https://blog.csdn.net/freemoses77/article/details/79466762)



