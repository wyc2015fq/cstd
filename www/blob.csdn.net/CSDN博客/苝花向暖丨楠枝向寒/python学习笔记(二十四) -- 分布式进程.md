# python学习笔记(二十四) -- 分布式进程 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月03日 14:36:48[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：34
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**目录**

[概念](#%E6%A6%82%E5%BF%B5)

[实现](#%E5%AE%9E%E7%8E%B0)

[注意](#%E6%B3%A8%E6%84%8F%EF%BC%9A)

## 概念

我前一个文章所讲的多线程和多进程都是在一台计算机上实现将一个工作分为多个小工作，使得多个cpu可以同时参与到工作当中，从而提高工作效率，以及提高对CPU的利用率。

但有时候一台计算机无法负担全部的工作量，我们就需要将工作分配给两台甚至多台计算机，其中一台计算机用来发布任务，其他的计算机接收任务执行并返回结果。

多进程程序在同一台机器上运行我们通过Queue来进行通信，如果是分布式进程，也就是不同计算机上的进程依旧可以使用Queue来进行通信，只不过我们需要通过multiprocessing的一个子模块manager来将Queue发布到网络上，这样其它计算机就可以获取到该Queue，从而获得里面存入的信息。

manager模块可以理解为中介，一台计算机在manger这里发布任务，另外的计算机想接任务的，到中介这里告诉中介想要获取哪个任务，然后领取。但是这个任务信息，实际上一直是在发布任务的计算机上面保存着的。只不过是通过manger可以获取到而已。

就像你想通过中介租房，但房还是属于房东，你只不过通过中介获取到房源信息罢了。

## 实现

1：发布任务相关代码  task_master.py

```python
import random, queue
from multiprocessing.managers import BaseManager

# 发布的任务存这个队列：
task_queue = queue.Queue()

# 其它计算机执行完的结果存这个队列：
result_queue = queue.Queue()

# 从BaseManage继承的QueueManager:
class QueueManager(BaseManager):
    pass

def return_task_queue():
    global task_queue
    return task_queue

def return_result_queue():
    global result_queue
    return result_queue

if __name__ == '__main__':
    # 把两个Queue都注册到网络上,callable参数关联了Queue对象：
    # 由于Queue对象需要在网络中传输，所以这里不能够使用lambda表达式
    # lambda表达式无法被序列化，所以需要自己编写一个函数来返回Queue对象
    QueueManager.register('get_task', callable=return_task_queue) # 参数1 给Manger注册Queue起个名字，后续可以通过这个名字 + 小括号，获取到该Queue对象 
    QueueManager.register('get_result', callable=return_result_queue)
    # 绑定端口5000，设置验证码'abc' 
    manager = QueueManager(address=('127.0.0.1 ', 5000), authkey=b'abc') # 我这就在一台电脑上模拟了，127.0.0.1代表本机ip地址
    # 启动Queue
    manager.start()
    # 获得通过网络访问的Queue对象: 这里必须通过manager获取到Queue，直接使用上面定义的Queue，数据放入不进去
    task = manager.get_task() # 通过名字 + 小括号，获取到Queue对象 
    result = manager.get_result()
    # 发布任务的计算机放几个任务进去：
    for i in range(10):
        n = random.randint(0, 10000)
        print('Put task %d...' % n)
        task.put(n)
    # 接受任务的计算机执行完毕将结果返回放入result后，从result队列读取结果
    print('Try get results...')
    for i in range(10):
        r = result.get(timeout=10)  # 从Queue中获取数据，如果10s内没有取到数据就会抛出异常
        print('Result: %s' % r)
    # 关闭
    manager.shutdown()
    print('master exit.')
```

2：接收任务的计算机task_worker

```python
import time, queue
from multiprocessing.managers import BaseManager

# 创建类似的QueueManager:
class QueueManager(BaseManager):
    pass

# 由于这个QueueManager只从网络上获取Queue,所以注册时只提供名字：
QueueManager.register('get_task')     # 这里的参数是发布时给Queue起的名字
QueueManager.register('get_result')   # 就是告诉manger我要获取哪个Queue

# 连接到服务器，也就是运行task_master.py的机器
server_addr = '127.0.0.1'
print('Connect to server %s...' % server_addr)

# 端口的验证码注意保持与task_master.py设置的完全一致
m = QueueManager(address=(server_addr, 5000), authkey=b'abc')
# 从网络连接：
m.connect()
# 获取Queue的对象
task = m.get_task()  
result = m.get_result()
# 从task队列取任务，并把结果写入result队列：
for i in range(10):
    try:
        n = task.get(timeout=1)  # 从Queue中获取数据，如果1s内没有获取到就会抛出异常
        print('run task %d * %d...' % (n, n))
        r = '%d * %d = %d' % (n, n, n*n)
        time.sleep(1)
        result.put(r)   # 执行完任务将数据放入result中，到这里之后，发布者就可以从result中读取到数据了
    except queue.Empty:
        print('task queue is empty.')

# 处理结束
print('worker exit.')
```

## 注意

1、使用过程中要关闭防火墙

2、callable那里不允许使用lambda表达式，因为需要将Queue进行网络传输，也就需要序列化，但是lambda表达式无法被序列化。

3、要先运行发布进程的代码，在运行执行进程的代码。

