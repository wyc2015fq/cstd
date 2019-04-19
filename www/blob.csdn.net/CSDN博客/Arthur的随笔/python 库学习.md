# python 库学习 - Arthur的随笔 - CSDN博客
2011年09月26日 15:47:21[largetalk](https://me.csdn.net/largetalk)阅读数：877
今天学习了几个pythonku
1. multiprocess
```python
import os, time
from multiprocessing import *
def test(x):
    print current_process().pid, x
    time.sleep(1)
if __name__ == "__main__":
    print "main:", os.getpid()
    p = Pool(5)
    p.map(test, range(12))
```
```python
import os, time, sys, atexit
from multiprocessing import *
def test(i):
    list.append("{0:02} {1}".format(i, current_process().pid))
    time.sleep(0.01)
    return i*2
if __name__ == "__main__":
    manager = Manager()
    list = manager.list()
    pool = Pool(3)
    ret = pool.map(test, range(10), 3)
    pool.close()
    pool.join()
    for x in sorted(list): print x
    print ret
```
2. random
```python
a = random(10)
random.shuffle(a)
random.uniform(1, 20)
a = Random()
a.seed(1)
[a.randint(1,100) for x in range(10)]
b=Random()
b.seed(1)
[b.randint(1, 100) for x in range(10)]
```
3.heapq
```python
from heapq import *
from random import *
rand = sample(range(10), 10) # 生成随机整数列表
heap = []
for i in rand: heappush(heap, i) # 将随机数压入
for i in range(len(rand)): heappop(heap) # 从堆弹出整数，总是返回最小值。
nlargest(3, range(10))
nsmallest(5, range(10))
```
