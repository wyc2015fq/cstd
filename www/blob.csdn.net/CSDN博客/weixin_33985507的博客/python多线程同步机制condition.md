# python多线程同步机制condition - weixin_33985507的博客 - CSDN博客
2017年09月24日 11:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import threading
import time
def customer(cond):
    t = threading.currentThread()
with cond:
# wait()方法创建了一个名为waiter的锁，并且设置锁的状态为locked。这个waiter锁用于线程间的通讯
        cond.wait()
print '{}: Resource is available to consumer'.format(t.name)
def producer(cond):
    t = threading.currentThread()
with cond:
print '{}: Making resource available'.format(t.name)
        cond.notifyAll()
if __name__ == "__main__":
    cond = threading.Condition()
    c1 = threading.Thread(target=customer, args=(cond,), name='c1')
    c2 = threading.Thread(target=customer, args=(cond,), name='c2')
    p1 = threading.Thread(target=producer, args=(cond,), name='p1')
    c1.start()
    c2.start()
    p1.start()
print 'Main end'
