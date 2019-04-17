# 【Multiprocessing】Python多进程内存共享数据队列SMQueue - CD's Coding - CSDN博客





2018年09月30日 12:05:33[糖果天王](https://me.csdn.net/okcd00)阅读数：417








## 0x00 前言

自从先前研究了下Python的多进程计算（[原文链接](https://blog.csdn.net/okcd00/article/details/80849284)）之后，

深深地感受到多进程处理的美好，并决定运用到模型训练时，

作为 **feed data** 的数据处理模块使用，实现工具类 **SharedMemoryQueue**。
但是众所周知，python的进程间通讯比较复杂，更别提大量进程的时候……想想就头疼……

但是python本身就是万金油，为啥我非要用python的机制呢，用python调用C不就好啦！

简要记录一下解决方案（我觉得聪明的小伙伴看到这个import，就已经懂了一半了）：
```python
```python
from ctypes import c_int, c_float, cast, POINTER
from multiprocessing.sharedctypes import Array, Value
```
```

## 0x01 基本思路

在C语言中，声明变量时是可以开一块内存区域的，

并且，在知道这块内存指针的情况下可以通过指针调用这一块数据。

然后对于当前需求中这种 “生产者—消费者” 的模式：

1）生产者为多个，即多个进程分别生产数据，谁生产好了谁就写一块；

2）消费者为一个，即一个进程每次需求数据，从已生产数据中拿一块；

那么就很明了了，队列这种数据结构就可以解决这个问题。
举个例子说明一下具体流程（即生产者—消费者模式）：

对于【A（即预设每块数据占用内存量） x 10 （即队列最大长度）】的内存，

开多个进程生成数据（生产者），对于每个进程，生成之后把不足预设占用内存的部分padding（补零），

然后写入队列（这里需要写入锁，体现为 `with write_lock`），将写入指针后移一个 A 的偏移地址，

当偏移地址超过预设最大长度（此处为 `10`）的时候，回到前面第一块内存的位置（取模 `write_idx % queue_size`操作），

特别的，当 `write_idx - read_idx` 等于预设队列最大长度即队列已满的时候进入等待状态，

对于读取数据的进程（消费者），直接向队列读取当前读取（此处为了防止多个消费者也加了读取锁），

如果数据的使用不需要padding，读取完毕后记得把先前padding的部分去掉，并将读取指针后移一个 A 的偏移地址。
## 0x02 Source Code

> 
**Code 思路来源于 @lihongwei / @lhw446**

个人进行了Feature的增加与修改，包括但不限于： **冷启动** 与 **超时重置**的处理，

主要修改在基于该队列的**HVD**processor，就放在下篇里再写吧~
```python
```python
# coding=utf8
# ========================================================
#   Copyright (C) 2017-2018 All rights reserved.
# 
#   filename : shared_memory_queue.py
#   author   : lihongwei / lhw446@qq.com
#   update   : chendian / okcd00@qq.com
#   date     : 2017-11-28
#   desc     : a shared memory queue for data processor
# ========================================================

import time
import numpy as np
import multiprocessing
from ctypes import c_int, c_float, cast, POINTER
from multiprocessing.sharedctypes import Array, Value


class SMQueue(object):
    ''' a shared memory queue for data processor '''

    # pylint: disable=protected-access
    def __init__(self, queue_size, f_data_size, i_data_size):
        queue_size += 1  # plus 1 is for the one consumer space
        f_cdatasets = Array('f', np.zeros((queue_size * f_data_size), dtype=np.float32))
        i_cdatasets = Array('i', np.zeros((queue_size * i_data_size), dtype=np.int32))
        self.f_cbuffer = f_cdatasets._obj._wrapper
        self.i_cbuffer = i_cdatasets._obj._wrapper
        self.read_idx = Value('i', 0)
        self.write_idx = Value('i', 0)
        self.queue_size = queue_size
        self.f_data_size = f_data_size
        self.i_data_size = i_data_size
        self.read_lock = multiprocessing.Lock()
        self.write_lock = multiprocessing.Lock()

    def get(self, time_gap=0.1, time_out=1234, cold_boot=False):
        ''' get f_data, i_data from queue '''
        with self.read_lock:
            time_cnt = 0
            while self.read_idx.value == self.write_idx.value:
                time_cnt += 1
                if time_cnt >= time_out and not cold_boot:
                    return None, None
                time.sleep(time_gap)
            index = self.read_idx.value % self.queue_size
            f_buffer_ptr = cast(self.f_cbuffer.get_address() + index * self.f_data_size * 4,
                                POINTER(c_float))
            i_buffer_ptr = cast(self.i_cbuffer.get_address() + index * self.i_data_size * 4,
                                POINTER(c_int))
            f_data = np.ctypeslib.as_array(f_buffer_ptr, shape=(self.f_data_size, ))
            i_data = np.ctypeslib.as_array(i_buffer_ptr, shape=(self.i_data_size, ))
            self.read_idx.value += 1
            return f_data, i_data

    def put(self, f_data, i_data, time_gap=0.1):
        ''' put f_data and i_data to queue '''
        with self.write_lock:
            # only use queue_size-1 space
            while self.write_idx.value - self.read_idx.value == self.queue_size - 1:
                time.sleep(time_gap)
            index = self.write_idx.value % self.queue_size
            f_buffer_ptr = cast(self.f_cbuffer.get_address() + index * self.f_data_size * 4,
                                POINTER(c_float))
            i_buffer_ptr = cast(self.i_cbuffer.get_address() + index * self.i_data_size * 4,
                                POINTER(c_int))
            o_f_data = np.ctypeslib.as_array(f_buffer_ptr, shape=(self.f_data_size,))
            o_i_data = np.ctypeslib.as_array(i_buffer_ptr, shape=(self.i_data_size,))
            o_f_data[:] = f_data
            o_i_data[:] = i_data
            self.write_idx.value += 1

    def push(self, f_data, i_data, time_gap=0.1):
        self.put(f_data, i_data, time_gap)

    def reset(self):
        self.read_idx = Value('i', 0)
        self.write_idx = Value('i', 0)

    def queue_info(self):
        return "{}/{}".format(self.write_idx.value - self.read_idx.value, self.queue_size)
```
```









