# python基础(十三) cmd命令调用 - _天枢 - 博客园
## [python基础(十三) cmd命令调用](https://www.cnblogs.com/yhleng/p/8202528.html)
2018-01-05 09:21 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8202528)
# [python cmd命令调用](http://www.cnblogs.com/lrw3716740/p/5158494.html)
关于python调用cmd命令：
主要介绍两种方式：
**1.python的OS模块。**
　　OS模块调用CMD命令有两种方式：os.popen(),os.system(). 都是用当前进程来调用。
os.system是无法获取返回值的。当运行结束后接着往下面执行程序。用法如：OS.system("ipconfig").
OS.popen带返回值的，如何获取返回值。如
　　p=os.popen(cmd)
　　print p.read().得到的是个字符串。
　　这两个都是用当前进程来调用，也就是说它们都是阻塞式的。
**2.管道subprocess模块。**
　　运行原理会在当前进程下面产生子进程。
sub=subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
　　sub.wait()
　　print sub.read()
