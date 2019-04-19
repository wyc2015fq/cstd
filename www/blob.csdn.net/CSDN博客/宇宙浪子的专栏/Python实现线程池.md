# Python实现线程池 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 16:04:55[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：402

最近在做一些文本处理方面的事情，考虑到程序利用并发性可以提高执行效率（不纠结特殊反例），于是入围的Idea如使用多进程或多线程达到期望的目标，对于进程或线程的创建是有代价的，那么我们是否可以实现一个线程池来达到已创建的线程反复使用从而使代价降低到最小呢？
当然可以，要想创建一个线程池，那么必须得有个容器来模拟“池”，在Python中，队列这样的数据结构就可以帮我们解决“池”这个问题，然而随之引来的 多线程之间数据处理同步问题，好在Python中有个Queue模块帮我们解决了这一棘手的问题，那么我们就可以实现一个线程池的雏形了。
Python代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- # !/usr/bin/env python
- # -*- coding:utf-8 -*-
- 
- import Queue  
- import threading  
- import time  
- 
- class WorkManager(object):  
- def __init__(self, work_num=1000,thread_num=2):  
- self.work_queue = Queue.Queue()  
- self.threads = []  
- self.__init_work_queue(work_num)  
- self.__init_thread_pool(thread_num)  
- 
- """
-         初始化线程
-     """
- def __init_thread_pool(self,thread_num):  
- for i in range(thread_num):  
- self.threads.append(Work(self.work_queue))  
- 
- """
-         初始化工作队列
-     """
- def __init_work_queue(self, jobs_num):  
- for i in range(jobs_num):  
- self.add_job(do_job, i)  
- 
- """
-         添加一项工作入队
-     """
- def add_job(self, func, *args):  
- self.work_queue.put((func, list(args)))#任务入队，Queue内部实现了同步机制
- 
- """
-         等待所有线程运行完毕
-     """
- def wait_allcomplete(self):  
- for item inself.threads:  
- if item.isAlive():item.join()  
- 
- class Work(threading.Thread):  
- def __init__(self, work_queue):  
-         threading.Thread.__init__(self)  
- self.work_queue = work_queue  
- self.start()  
- 
- def run(self):  
- #死循环，从而让创建的线程在一定条件下关闭退出
- whileTrue:  
- try:  
-                 do, args = self.work_queue.get(block=False)#任务异步出队，Queue内部实现了同步机制
-                 do(args)  
- self.work_queue.task_done()#通知系统任务完成
- except:  
- break
- 
- #具体要做的任务
- def do_job(args):  
-     time.sleep(0.1)#模拟处理时间
- print threading.current_thread(), list(args)  
- 
- if __name__ == '__main__':  
-     start = time.time()  
-     work_manager =  WorkManager(10000, 10)#或者work_manager =  WorkManager(10000, 20)
-     work_manager.wait_allcomplete()  
-     end = time.time()  
- print"cost all time: %s" % (end-start)  
 2次开启不同的线程数运行结果如下：
Python代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- #work_manager =  WorkManager(10000, 10)
- cost all time: 100.641790867(单位：秒)  
- #work_manager =  WorkManager(10000, 20)
- cost all time：50.5233478546(单位：秒)  
 上面实现了线程池的雏形，展现了基本原理，当然要想成为通用的API需要做很多的工作，希望本文能够起到抛砖引玉的效果。
