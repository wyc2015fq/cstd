# python 操作多线程和多进程 - sxf_123456的博客 - CSDN博客
2019年03月20日 22:15:49[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：22标签：[python 操作多进程和多线程](https://so.csdn.net/so/search/s.do?q=python 操作多进程和多线程&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
"""
python 标准模块，提供ProcessPoolExecutor 进程异步调用
ThreadPoolExecutor 线程异步调用
"""
from concurrent.futures.process import ProcessPoolExecutor
from concurrent.futures.thread import ThreadPoolExecutor
import time
def print_data(num):
    print(num)
    time.sleep(1)
    return num
if __name__ == '__main__':
    # 1、使用进程池 提供异步调用
    p_pool = ProcessPoolExecutor(3)  # 进程池设为3，默认是CPU核数
    # for i in range(10):
    #     p_pool.submit(print_data, i)  # 异步提交任务
    # 使用map 代替 for 循环
    p_pool.map(print_data, range(10))
    # 相当于进程池 p_pool.close()+p_pool.join()操作
    p_pool.shutdown(wait=True)  # True:等待池内所有任务执行完毕,回收资源继续操作;False 立即返回，不等待池内任务是否完成
    print("------------------------")
    # 2、使用线程池 提供异步调用
    th_pool = ThreadPoolExecutor(3)  # 线程池设为3， 默认是CPU核数
    # for i in range(10):
    #     th_pool.submit(print_data, i)
    # 使用map 代替 for 循环
    th_pool.map(print_data, range(10))
    th_pool.shutdown(wait=True)
"""
使用 multprocessing 模块 提供Process单个进程、Pool进程池
"""
from multiprocessing import Process
from multiprocessing import Pool
import time
def print_data(num):
    print(num)
    time.sleep(2)
    return num
if __name__ == '__main__':
    # 采用单个子进程
    for i in range(10):
        p = Process(target=print_data, args=(i,))
        p.start()  # 启动子进程
        p.join()  # 等待子进程执行完毕
    # 采用进程池
    pool = Pool(3)  # 设置进程池为3，默认值是CPU的核数，如果CPU核数是4，至少要提交5个任务才能看到等待效果
    for i in range(10):
        pool.apply_async(print_data, args=(i,))
    pool.close()  # 调用close 不能再添加进程
    pool.join()  # 等待所有子进程执行完毕
"""
借助 队列Queue 实现多进程之间通信
"""
from multiprocessing import Queue, Process
import time
def write_queue(queue):
    for i in range(10):
        queue.put(i)
    queue.put(None)
def read_queue(queue):
    while True:
        value = queue.get()
        if value is None:
            break
        print(value)
        time.sleep(1)
if __name__ == '__main__':
    queue = Queue()  # 创建队列，默认设定队列的存放最大数据量是256M
    p_w = Process(target=write_queue, args=(queue,))  # 创建往队列写进程
    p_r = Process(target=read_queue, args=(queue,))  # 创建从队列读进程
    p_w.start()  # 启动写进程
    p_r.start()  # 启动读进程
    p_w.join()  # 等待写进程执行完毕
    p_r.join()  # 等待读进程执行完毕
"""
多个任务可以由多进程执行，也可以采用一个进程内多个线程执行
进程由若干个线程组成，一个进程中至少一个线程
"""
import threading, time
def print_data(num):
    print(num)
    time.sleep(1)
    return num
if __name__ == '__main__':
    # 多线程执行
    for i in range(10):
        t = threading.Thread(target=print_data, args=(i,))
        t.start()
        t.join()
"""
多进程和多线程区别:
多进程中，同一个变量，各自有一份拷贝存在于各个进程中，互不影响
多线程中，变量是共享的，多个线程对同一变量进行修改时，出现变量值错误
需要对多线程加锁进行解决
"""
import threading, time
balance = 0  # 定义一个全局变量
# lock = threading.Lock()
def change_data(num):
    # global balance
    # balance = balance + num
    # balance = balance - num
    print(num)
def run_thread(n):
    for i in range(1000):
        # lock.acquire()  # 获取锁
        try:
            change_data(n)
        finally:
            pass
            # lock.release()  # 释放锁
if __name__ == '__main__':
    th1 = threading.Thread(target=run_thread, args=(5,))  # 创建线程1
    th2 = threading.Thread(target=run_thread, args=(8,))  # 创建线程2
    th1.run()
    th2.run()
    # th1.setDaemon(True)  # 设置线程1在后台运行
    # th2.setDaemon(True)  # 设置线程2在后台运行
    # th1.start()  # 启动线程1
    # th2.start()  # 启动线程2
    # th1.join()  # 等待线程1执行完毕
    # th2.join()  # 等待线程2执行完毕
    print(balance)
