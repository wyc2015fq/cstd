
# python互斥锁、加锁、同步机制、异步通信知识总结 - jiahaowanhao的博客 - CSDN博客


2018年05月22日 21:01:06[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：194


[python互斥锁、加锁、同步机制、异步通信知识总结](http://cda.pinggu.org/view/25604.html)
某个线程要共享数据时，先将其锁定，此时资源的状态为“锁定”，其他线程不能更改；直到该线程释放资源，将资源的状态变成“非锁定”，其他的线程才能再次锁定该资源。互斥锁保证了每次只有一个线程进入写入操作，从而保证了多线程情况下数据的正确性。
采用f_flag的方法效率低
创建锁
mutex=threading.Lock()
锁定
mutex.acquire([blocking])\#里面可以加blocking(等待的时间)或者不加，不加就会一直等待（堵塞）
释放
mutex.release()
import threading
from threading import Thread
from threading import Lock
import time
thnum=0
\#两个线程都在抢着对这个锁进行上锁，如果有一方成功上锁，那么导致另外一方会堵塞（一直等待），到这个锁被解开为之
class MyThread(threading.Thread):
def run(self):
mutex.acquire()
for i in range(10000):
global thnum
thnum+=1
print(thnum)
mutex.release()
def test():
global thnum
mutex.acquire() \#等待可以上锁，通知而不是轮训，没有占用CPU
for i in range(10000):
thnum+=1
print(thnum)
mutex.release()\#解锁
mutex=Lock()
if __name__=='__main__':
t=MyThread()
t.start()
\#创建一把互斥锁，默认是没有上锁的
thn=Thread(target=test)
thn.start()
'''''
10000
20000
'''
只要一上锁，由多任务变为单任务,相当于只有一个线程在运行。
下面的代码相对上面加锁的时间变短了
import threading
from threading import Thread
from threading import Lock
import time
thnum=0
\#两个线程都在抢着对这个锁进行上锁，如果有一方成功上锁，那么导致另外一方会堵塞（一直等待），到这个锁被解开为之
class MyThread(threading.Thread):
def run(self):
for i in range(10000):
mutex.acquire()
global thnum
thnum+=1
mutex.release()\#释放后，都开始抢，这样上锁的时间变短
print(thnum)
def test():
global thnum
for i in range(10000):
mutex.acquire()
thnum+=1
mutex.release()\#解锁
print(thnum)
mutex=Lock()
if __name__=='__main__':
t=MyThread()
t.start()
\#创建一把互斥锁，默认是没有上锁的
thn=Thread(target=test)
thn.start()
'''''
10000
20000
'''
只有必须加锁的地方才加锁
同步：按照预定的先后顺序执行
一个运行完后，释放下一个，下一个锁定后运行，再释放下一个，下一个锁定后，运行后释放下一个..... 释放第一个
异步：
\#异步的实现
from multiprocessing import Pool
import time
import os
\#getpid()获取当前进程的进程号
\#getppid()获取当前进程的父进程号
def test():\#子进程
print("----进程池中的进程-----pid=%d,ppid=%d --"%(os.getpid(),os.getppid()))
for i in range(3):
print("-----%d----"%i)
time.sleep(1)
return "over" \#子进程执行完后返回给操作系统，返回给父进程
def test2(args):
print("-----callback func----pid=%d"%os.getpid())\#主进程调用test2
print("------callback func---args=%s"%args)
def main():
pool=Pool(3)
pool.apply_async(func=test,callback=test2)\#回调
time.sleep(5)\#收到func进程结束后的信号后，执行回调函数test2
print("----主进程-pid = %d"%os.getpid())
if __name__=="__main__":
\#main()
pool=Pool(3)
pool.apply_async(test,callback=test2)\#回调
time.sleep(5)\#收到func进程结束后的信号后，执行回调函数test2
print("----主进程-pid = %d"%os.getpid())
'''''显示结果不太正确，应该先运行test呀，再运行test2
-----callback func----pid=7044
------callback func---args=over
----主进程-pid = 7044
----进程池中的进程-----pid=3772,ppid=7044 --
-----0----
-----1----
-----2----
'''

