# 用python实现的线程池 - Orisun - 博客园







# [用python实现的线程池](https://www.cnblogs.com/zhangchaoyang/articles/5188904.html)





python3标准库里自带线程池[ThreadPoolExecutor](http://pythonhosted.org/futures/#concurrent.futures.ThreadPoolExecutor)和进程池[ProcessPoolExecutor](http://pythonhosted.org/futures/#concurrent.futures.ProcessPoolExecutor)。

如果你用的是python2，那可以下载一个模块，叫[threadpool](https://pypi.python.org/pypi/threadpool)，这是线程池。对于进程池可以使用python自带的[multiprocessing.Pool](https://docs.python.org/2/library/multiprocessing.html?highlight=multiprocessing.pool#module-multiprocessing.pool)。

当然也可以自己写一个threadpool。

# coding:utf-8

import Queue
import threading
import sys
import time
import math


class WorkThread(threading.Thread):

    def __init__(self, task_queue):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.task_queue = task_queue
        self.start()
        self.idle = True

    def run(self):
        sleep_time = 0.01  # 第1次无任务可做时休息10毫秒
        multiply = 0
        while True:
            try:
                # 从队列中取一个任务
                func, args, kwargs = self.task_queue.get(block=False)
                self.idle = False
                multiply = 0
                # 执行之
                func(*args, **kwargs)
            except Queue.Empty:
                time.sleep(sleep_time * math.pow(2, multiply))
                self.idle = True
                multiply += 1
                continue
            except:
                print sys.exc_info()
                raise


class ThreadPool:

    def __init__(self, thread_num=10, max_queue_len=1000):
        self.max_queue_len = max_queue_len
        self.task_queue = Queue.Queue(max_queue_len)  # 任务等待队列
        self.threads = []
        self.__create_pool(thread_num)

    def __create_pool(self, thread_num):
        for i in xrange(thread_num):
            thread = WorkThread(self.task_queue)
            self.threads.append(thread)

    def add_task(self, func, *args, **kwargs):
        '''添加一个任务，返回任务等待队列的长度
           调用该方法前最后先调用isSafe()判断一下等待的任务是不是很多，以防止提交的任务被拒绝
        '''
        try:
            self.task_queue.put((func, args, kwargs))
        except Queue.Full:
            raise  # 队列已满时直接抛出异常，不给执行
        return self.task_queue.qsize()

    def isSafe(self):
        '''等待的任务数量离警界线还比较远
        '''
        return self.task_queue.qsize() < 0.9 * self.max_queue_len

    def wait_for_complete(self):
        '''等待提交到线程池的所有任务都执行完毕
        '''
        #首先任务等待队列要变成空
        while not self.task_queue.empty():
            time.sleep(1)
        # 其次，所以计算线程要变成idle状态
        while True:
            all_idle = True
            for th in self.threads:
                if not th.idle:
                    all_idle = False
                    break
            if all_idle:
                break
            else:
                time.sleep(1)


if __name__ == '__main__':
    def foo(a, b):
        print a + b
        time.sleep(0.01)
    thread_pool = ThreadPool(10, 100)
    '''在Windows上测试不通过，Windows上Queue.Queue不是线程安全的'''
    size = 0
    for i in xrange(10000):
        try:
            size = thread_pool.add_task(foo, i, 2 * i)
        except Queue.Full:
            print 'queue full, queue size is ', size
    time.sleep(2)















