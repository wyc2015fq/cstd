# python 多线程 - wsp_1138886114的博客 - CSDN博客





2018年11月22日 17:29:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：98标签：[python 多线程](https://so.csdn.net/so/search/s.do?q=python 多线程&t=blog)
个人分类：[Python基础](https://blog.csdn.net/wsp_1138886114/article/category/7678650)

所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)












### 文章目录
- [一、简单入门](#_1)
- [二、多线程](#_22)
- [2.1 单线程（串行）](#21__39)
- [2.2 多线程—未开启线程等待](#22__65)
- [2.3 多线程—开启线程等待](#23__104)
- [2.4 多线程—添加线程列表](#24__142)
- [三、多线程—获取返回值](#_193)




### 一、简单入门

```python
import time
from threading import Thread

def say_hellow(name):
    time.sleep(2)
    print('%s say hello to friends '%name)

if __name__=='__main__':
    t=Thread(target=say_hellow,args=('zhangsan',))  #传入（函数名，参数）
    t.start()
    print('主线程')

"""
输出：
主线程
zhangsan say hello to friends
"""
```

### 二、多线程

Python通过两个标准库thread和threading提供对线程的支持。thread提供了低级别的、原始的线程以及一个简单的锁。

**threading 模块提供的其他方法：**
- `threading.currentThread()`: 返回当前的线程变量。
- `threading.enumerate()`: 返回一个包含正在运行的线程的list。正在运行指线程启动后、结束前，不包括启动前和终止后的线程。
- `threading.activeCount()`: 返回正在运行的线程数量，与len(threading.enumerate())有相同的结果。

**Thread类提供了以下方法:**
- `run()`: 用以表示线程活动的方法。
- `start()`:启动线程活动。
- `join([time])`: 等待至线程中止。这阻塞调用线程直至线程的join() 方法被调用中止-正常退出或者抛出未处理的异常-或者是可选的超时发生。
- `isAlive()`: 返回线程是否活动的。
- `getName()`: 返回线程名。
- `setName()`: 设置线程名。

##### 2.1 单线程（串行）

```python
from time import ctime,sleep

def music():
    for i in range(2):
        print ("我正在听歌，此时时间为：%s" %ctime())
        sleep(1)

def movies():
    for i in range(2):
        print ("我正在看电影，此时时间为：%s" %ctime())
        sleep(5)

if __name__ == '__main__':
    music()
    movies()
    print ("娱乐完成 %s" %ctime())
    
输出：
我正在听歌，此时时间为：Thu Nov 22 10:42:04 2018
我正在听歌，此时时间为：Thu Nov 22 10:42:05 2018
我正在看电影，此时时间为：Thu Nov 22 10:42:06 2018
我正在看电影，此时时间为：Thu Nov 22 10:42:11 2018
娱乐完成 Thu Nov 22 13:42:16 2018
```

##### 2.2 多线程—未开启线程等待

```python
import threading
from time import ctime,sleep


def music(name):
    for i in range(2):
        print("{}正在听歌 。此时时间为：{}".format(name,ctime()))
        sleep(1)

def movies(movies_name):
    for i in range(2):
        print ("我在看电影{}。此时时间为：{}" .format(movies_name,ctime()))
        sleep(5)

threads = []
t1 = threading.Thread(target=music,args=('张三',))
threads.append(t1)
t2 = threading.Thread(target=movies,args=('举起手来',))
threads.append(t2)

if __name__ == '__main__':
    for t in threads:
        """
        setDaemon(True) 将线程声明为守护线程，必须在start() 方法调用之前设置。
        """
        t.setDaemon(True)
        t.start()
    print ("娱乐完成 %s" %ctime())

输出：    
张三正在听歌 。此时时间为：Thu Nov 22 11:13:29 2018
我在看电影举起手来。此时时间为：Thu Nov 22 11:13:29 2018
娱乐完成 Thu Nov 22 11:13:29 2018

（显然上面函数循环未完成就被中止了：父线程执行完后，没有等待子线程，
直接就退出了，同时子线程也一同结束。）
```

##### 2.3 多线程—开启线程等待

添加`t.join()`开启等待就好了

```python
import threading
from time import ctime,sleep


def music(name):
    for i in range(2):
        print("{}正在听歌 。此时时间为：{}".format(name,ctime()))
        sleep(1)

def movies(movies_name):
    for i in range(2):
        print ("我在看电影{}。此时时间为：{}" .format(movies_name,ctime()))
        sleep(5)

threads = []
t1 = threading.Thread(target=music,args=('张三',))
threads.append(t1)
t2 = threading.Thread(target=movies,args=('举起手来',))
threads.append(t2)

if __name__ == '__main__':
    for t in threads:
        t.setDaemon(True)
        t.start()
    for t in threads:
        t.join()
    print ("娱乐完成 %s" %ctime())

输出：
张三正在听歌 。此时时间为：Thu Nov 22 11:19:30 2018
我在看电影举起手来。此时时间为：Thu Nov 22 11:19:30 2018
张三正在听歌 。此时时间为：Thu Nov 22 11:19:31 2018
我在看电影举起手来。此时时间为：Thu Nov 22 11:19:35 2018
娱乐完成 Thu Nov 22 11:19:40 2018
```

##### 2.4 多线程—添加线程列表

```python
from time import sleep, ctime
import threading


def muisc(func):
    for i in range(2):
        print('开始娱乐：{}；此时时间为：{}'.format(func, ctime()))
        sleep(2)

def move(func):
    for i in range(2):
        print('开始娱乐：{}；此时时间为：{}'.format(func, ctime()))
        sleep(5)

def player(name):
    r = name.split('.')[1]
    if r == 'mp3':
        muisc(name)
    elif r == 'mp4':
        move(name)
    else:
        print('error: 格式错误!')

list = ['同桌的你.mp3', '阿凡达.mp4']
threads = []
files = range(len(list))

# 创建线程
for i in files:
    t = threading.Thread(target=player, args=(list[i],))
    threads.append(t)

if __name__ == '__main__':
    # 启动线程
    for i in files:
        threads[i].start()
    for i in files:
        threads[i].join()

    # 主线程
    print('娱乐结束！此时时间为:%s' % ctime())

输出：
开始娱乐：同桌的你.mp3；此时时间为：Thu Nov 22 13:53:42 2018
开始娱乐：阿凡达.mp4；此时时间为：Thu Nov 22 13:53:42 2018
开始娱乐：同桌的你.mp3；此时时间为：Thu Nov 22 13:53:44 2018
开始娱乐：阿凡达.mp4；此时时间为：Thu Nov 22 13:53:47 2018
娱乐结束！此时时间为:Thu Nov 22 13:53:52 2018
```

### 三、多线程—获取返回值

```python
from collections import deque

queue = deque(["Eric", "John", "Michael"])

queue.append("Terry")           # Terry 入队(从后面添加)
queue.append("Graham")          # Graham 入队

queue.popleft()                 # 队首元素出队
print(queue)                    # 队列中剩下的元素
#输出: deque(['John', 'Michael', 'Terry', 'Graham']))
```

```python
from time import ctime
import threading
import collections

adress_name = ['广州', '北京']
cldas_sum = collections.deque()


class MyThread(threading.Thread):
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.name = name
        self.func = func
        self.args = args
        self.result = self.func(*self.args)

    def get_result(self):
        try:
            return self.result
        except Exception:
            return None


def loop(adress_n):
    cldas_values = []
    for k in range(4):
        cldas_values.append(adress_n + str(k)) #组合地名+数字
    cldas_sum.append(cldas_values)
    print(id(cldas_values))
    return cldas_sum


def main():
    print('start at', ctime())
    threads = []
    for i in range(len(adress_name)):
        t = MyThread(loop, (adress_name[i],), loop.__name__)
        threads.append(t)

    for i in range(len(adress_name)):  # 将任务分发到每个线程，完成后再开始执行start方法
        threads[i].start()
    for i in range(len(adress_name)):  # join()方法等待线程完成
        threads[i].join()
    print("threads[0]:",threads[0].get_result())
    print("threads[1]:",threads[1].get_result())
    print('done at:', ctime())


if __name__ == '__main__':
    main()

输出：
start at Mon Dec  3 21:37:18 2018
2443873654152
2443873874376
threads[0]: deque([['广州0', '广州1', '广州2', '广州3'], ['北京0', '北京1', '北京2', '北京3']])
threads[1]: deque([['广州0', '广州1', '广州2', '广州3'], ['北京0', '北京1', '北京2', '北京3']])
done at: Mon Dec  3 21:37:18 2018
```

需要注意的是：

（1）如果多个线程共用一个公共数据，那么我们需要做的就是将这个公共数据设置成队列格式，要不然多个线程共同访问这个数据可能会出错，需要加锁。设置成队列比加锁再放锁效率高多了

（2）线程之间同一个变量id都不一样，还是不知道是否其他线程会涉足另一个线程
鸣谢
[https://www.cnblogs.com/yeayee/p/4952022.html](https://www.cnblogs.com/yeayee/p/4952022.html)




