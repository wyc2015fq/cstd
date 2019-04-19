# 新版本caffe脚本运行无权限，#!/usr/bin/env sh - 数据之美的博客 - CSDN博客
2017年02月21日 16:18:30[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：435
今天运行新版caffe（150818）运行脚本老师提示 无权限。 
在论坛里看到下面这个解释
应该就是没有指定bash作为此脚本的解释器，在最var这个脚本
的最上面加上#!/bin/bash看看。
我马上打开自带的一个脚本：如下
#!/usr/bin/env sh
./build/tools/caffe train --solver=examples/mnist/lenet_solver_rmsprop.prototxt
所以把上边 的#!/usr/bin/env
 sh， 改为  #!/bin/bash 即可运行。 不再有权限限制
下面找个一个作者为什么这么写脚本的解释
在[Linux](http://lib.csdn.net/base/linux)的一些脚本里，需在开头一行指定脚本的解释程序，如：
#!/usr/bin/env [Python](http://lib.csdn.net/base/python)
再如：
#!/usr/bin/env perl
#!/usr/bin/env zimbu
但有时候也用
#!/usr/bin/python
和
#!/usr/bin/perl
那么 env到底有什么用？何时用这个呢？
脚本用env启动的原因，是因为脚本解释器在linux中可能被安装于不同的目录，env可以在系统的PATH目录中查找。同时，env还规定一些系统环境变量。
执行一下 env 命令后看看打印的内容
可以用env来执行程序：
[mark@localhost ~]$ env python
Python 2.6.6 (r266:84292, Dec  7 2011, 20:38:36)
[GCC 4.4.6 20110731 (Red Hat 4.4.6-3)] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> 
而如果直接将解释器路径写死在脚本里，可能在某些系统就会存在找不到解释器的兼容性问题。有时候我们执行一些脚本时就碰到这种情况。
