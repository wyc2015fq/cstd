# random在多进程下的奇怪表现 - Arthur的随笔 - CSDN博客
2012年08月27日 01:29:11[largetalk](https://me.csdn.net/largetalk)阅读数：916标签：[random																[import																[python																[os](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
tornado源码process.py fork_process函数功能是根据cpu数等来启动多个进程, 使用得是os.fork, 但在子进程里，真正执行端口监听前调用了一个奇怪得函数_reseed_random，该函数实现如下：
```python
def _reseed_random():
    if 'random' not in sys.modules:
        return
    import random
    # If os.urandom is available, this method does the same thing as
    # random.seed (at least as of python 2.6).  If os.urandom is not
    # available, we mix in the pid in addition to a timestamp.
    try:
        seed = long(hexlify(os.urandom(16)), 16)
    except NotImplementedError:
        seed = int(time.time() * 1000) ^ os.getpid()
    random.seed(seed)
```
这个函数好像没做什么有用得功能，只是调用了random.seed，为什么用调用random.seed呢，因为random得随机数种子是在import得时候就被确定了，默认应该是当前系统时间，而通过os.fork得到得子进程中，这个种子依然有效，所以在各子进程调用random会出现随机数一样得现象。看下面代码：
```python
import random
import os, sys, time
def test():
    for i in range(10):
        if os.fork() == 0:
            import random
            print i, '---(1)--', random.random()
            print i, '---(2)--', random.random()
            return
    time.sleep(2)
def main():
    test()
if __name__ == '__main__':
    main()
=====================
执行结果：
$ python fixrandom.py 
0 ---(1)-- 0.977207861202
0 ---(2)-- 0.21855746022
9 ---(1)-- 0.977207861202
9 ---(2)-- 0.21855746022
1 ---(1)-- 0.977207861202
1 ---(2)-- 0.21855746022
8 ---(1)-- 0.977207861202
8 ---(2)-- 0.21855746022
7 ---(1)-- 0.977207861202
7 ---(2)-- 0.21855746022
6 ---(1)-- 0.977207861202
6 ---(2)-- 0.21855746022
2 ---(1)-- 0.977207861202
2 ---(2)-- 0.21855746022
4 ---(1)-- 0.977207861202
4 ---(2)-- 0.21855746022
5 ---(1)-- 0.977207861202
5 ---(2)-- 0.21855746022
3 ---(1)-- 0.977207861202
3 ---(2)-- 0.21855746022
```
可以看出所有进程得random都一致了，这就完全达不到random得期望了。
            
