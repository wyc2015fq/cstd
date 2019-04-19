# python os._exit()   sys.exit()_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
			转载：http://www.cnblogs.com/gaott/archive/2013/04/12/3016355.html
# [os._exit() vs sys.exit()](http://www.cnblogs.com/gaott/archive/2013/04/12/3016355.html)
### 概述
python的程序有两中退出方式：os._exit()，
sys.exit()。本文介绍这两种方式的区别和选择。
os._exit()会直接将python程序终止，之后的所有代码都不会继续执行。
sys.exit()会引发一个异常：SystemExit，如果这个异常没有被捕获，那么python解释器将会退出。如果有捕获此异常的代码，那么这些代码还是会执行。
### 举例说明
1import os
23try:
4    os._exit(0)
5except:
6print'die.'
此处不会打出"going to die"
import sys
try:
    sys.exit(0)
except:
    print'die'finally:
    print'cleanup'
输出：
die
cleanup
### 区别
综上，sys.exit()的退出比较优雅，调用后会引发SystemExit异常，可以捕获此异常做清理工作。os._exit()直接将python解释器退出，余下的语句不会执行。
一般情况下使用sys.exit()即可，一般在fork出来的子进程中使用os._exit()
