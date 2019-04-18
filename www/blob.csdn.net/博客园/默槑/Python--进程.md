# Python--进程 - 默槑 - 博客园







# [Python--进程](https://www.cnblogs.com/modaidai/p/6999590.html)





## 　　　　　　　　使用multiprocessing进行进程管理

## 简单的创建进程
import multiprocessing

def worker(num):"""thread worker function"""print'Worker:', num
    returnif __name__ =='__main__':
    jobs =[]for i in range(5):
        p = multiprocessing.Process(target=worker, args=(i,))
        jobs.append(p)
        p.start()
确定当前的进程，即是给进程命名，方便标识区分，跟踪
import multiprocessing
import time

def worker():
    name = multiprocessing.current_process().name
    print name,'Starting'
    time.sleep(2)print name,'Exiting'def my_service():
    name = multiprocessing.current_process().name
    print name,'Starting'
    time.sleep(3)print name,'Exiting'if __name__ =='__main__':
    service = multiprocessing.Process(name='my_service',
                                      target=my_service)
    worker_1 = multiprocessing.Process(name='worker 1',
                                       target=worker)
    worker_2 = multiprocessing.Process(target=worker)# default name

    worker_1.start()
    worker_2.start()
    service.start()
## 守护进程

守护进程就是不阻挡主程序退出，自己干自己的 `mutilprocess.setDaemon(True)`

就这句

等待守护进程退出，要加上join,join可以传入浮点数值，等待n久就不等了
import multiprocessing
import time
import sys

def daemon():
    name = multiprocessing.current_process().name
    print'Starting:', name
    time.sleep(2)print'Exiting :', name

def non_daemon():
    name = multiprocessing.current_process().name
    print'Starting:', name
    print'Exiting :', name

if __name__ =='__main__':
    d = multiprocessing.Process(name='daemon',
                                target=daemon)
    d.daemon =True

    n = multiprocessing.Process(name='non-daemon',
                                target=non_daemon)
    n.daemon =False

    d.start()
    n.start()

    d.join(1)print'd.is_alive()', d.is_alive()
    n.join()
## 终止进程

最好使用 poison pill，强制的使用terminate()

注意 terminate之后要join，使其可以更新状态
import multiprocessing
import time

def slow_worker():print'Starting worker'
    time.sleep(0.1)print'Finished worker'if __name__ =='__main__':
    p = multiprocessing.Process(target=slow_worker)print'BEFORE:', p, p.is_alive()

    p.start()print'DURING:', p, p.is_alive()

    p.terminate()print'TERMINATED:', p, p.is_alive()

    p.join()print'JOINED:', p, p.is_alive()
## 进程的退出状态
- == 0 未生成任何错误
- 
> 
0 进程有一个错误，并以该错误码退出


- < 0 进程由一个-1 * exitcode信号结束
import multiprocessing
import sys
import time

def exit_error():
    sys.exit(1)def exit_ok():returndef return_value():return1def raises():raiseRuntimeError('There was an error!')def terminated():
    time.sleep(3)if __name__ =='__main__':
    jobs =[]for f in[exit_error, exit_ok, return_value, raises, terminated]:print'Starting process for', f.func_name
        j = multiprocessing.Process(target=f, name=f.func_name)
        jobs.append(j)
        j.start()

    jobs[-1].terminate()for j in jobs:
        j.join()print'%15s.exitcode = %s'%(j.name, j.exitcode)
## 日志

方便的调试，可以用logging
import multiprocessing
import logging
import sys

def worker():print'Doing some work'
    sys.stdout.flush()if __name__ =='__main__':
    multiprocessing.log_to_stderr()
    logger = multiprocessing.get_logger()
    logger.setLevel(logging.INFO)
    p = multiprocessing.Process(target=worker)
    p.start()
    p.join()
## 派生进程

利用class来创建进程，定制子类
import multiprocessing

classWorker(multiprocessing.Process):def run(self):print'In %s'%self.name
        returnif __name__ =='__main__':
    jobs =[]for i in range(5):
        p =Worker()
        jobs.append(p)
        p.start()for j in jobs:
        j.join()
## python进程间传递消息

这一块我之前结合SocketServer写过一点，见Python多进程

一般的情况是Queue来传递。
import multiprocessing

classMyFancyClass(object):def __init__(self, name):self.name = name

    def do_something(self):
        proc_name = multiprocessing.current_process().name
        print'Doing something fancy in %s for %s!'% \
            (proc_name,self.name)def worker(q):
    obj = q.get()
    obj.do_something()if __name__ =='__main__':
    queue = multiprocessing.Queue()

    p = multiprocessing.Process(target=worker, args=(queue,))
    p.start()

    queue.put(MyFancyClass('Fancy Dan'))# Wait for the worker to finish
    queue.close()
    queue.join_thread()
    p.join()import multiprocessing
import time

classConsumer(multiprocessing.Process):def __init__(self, task_queue, result_queue):
        multiprocessing.Process.__init__(self)self.task_queue = task_queue
        self.result_queue = result_queue

    def run(self):
        proc_name =self.name
        whileTrue:
            next_task =self.task_queue.get()if next_task isNone:# Poison pill means shutdownprint'%s: Exiting'% proc_name
                self.task_queue.task_done()breakprint'%s: %s'%(proc_name, next_task)
            answer = next_task()self.task_queue.task_done()self.result_queue.put(answer)returnclassTask(object):def __init__(self, a, b):self.a = a
        self.b = b
    def __call__(self):
        time.sleep(0.1)# pretend to take some time to do the workreturn'%s * %s = %s'%(self.a,self.b,self.a *self.b)def __str__(self):return'%s * %s'%(self.a,self.b)if __name__ =='__main__':# Establish communication queues
    tasks = multiprocessing.JoinableQueue()
    results = multiprocessing.Queue()# Start consumers
    num_consumers = multiprocessing.cpu_count()*2print'Creating %d consumers'% num_consumers
    consumers =[Consumer(tasks, results)for i in xrange(num_consumers)]for w in consumers:
        w.start()# Enqueue jobs
    num_jobs =10for i in xrange(num_jobs):
        tasks.put(Task(i, i))# Add a poison pill for each consumerfor i in xrange(num_consumers):
        tasks.put(None)# Wait for all of the tasks to finish
    tasks.join()# Start printing resultswhile num_jobs:
        result = results.get()print'Result:', result
        num_jobs -=1
## 进程间信号传递

Event提供一种简单的方法，可以在进程间传递状态信息。事件可以切换设置和未设置状态。通过使用一个可选的超时值，时间对象的用户可以等待其状态从未设置变为设置。
import multiprocessing
import time

def wait_for_event(e):"""Wait for the event to be set before doing anything"""print'wait_for_event: starting'
    e.wait()print'wait_for_event: e.is_set()->', e.is_set()def wait_for_event_timeout(e, t):"""Wait t seconds and then timeout"""print'wait_for_event_timeout: starting'
    e.wait(t)print'wait_for_event_timeout: e.is_set()->', e.is_set()if __name__ =='__main__':
    e = multiprocessing.Event()
    w1 = multiprocessing.Process(name='block', 
                                 target=wait_for_event,
                                 args=(e,))
    w1.start()

    w2 = multiprocessing.Process(name='nonblock', 
                                 target=wait_for_event_timeout, 
                                 args=(e,2))
    w2.start()print'main: waiting before calling Event.set()'
    time.sleep(3)
    e.set()print'main: event is set'

################################################################################################################################################################################################################################

先使用:
j.start()
j_1.start()
再使用:
j.join()　　　　　　#也就是先启动后上锁
j_1.join()　　　　　#join的意思是把当前的分进程运行完了之后才运行下面的进程


################################################################################################################################################################################################################################
multiprocess.Lock()
当多线程需要共享资源的时候，Lock可以用来避免访问的冲突

 lock1 = multiprocessing.Lock()
    lock2 = multiprocessing.Lock()
    lock3 = multiprocessing.Lock()
    lock4 = multiprocessing.Lock()
    lock5 = multiprocessing.Lock()
    lock6 = multiprocessing.Lock()
    lock7 = multiprocessing.Lock()
    lock8 = multiprocessing.Lock()



















