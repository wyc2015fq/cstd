# Python-Cpython解释器支持的进程与线程-Day9 - lincappu - 博客园







# [Python-Cpython解释器支持的进程与线程-Day9](https://www.cnblogs.com/lincappu/p/8157525.html)





Cpython解释器支持的进程与线程

阅读目录

```
一 python并发编程之多进程
1.1 multiprocessing模块介绍
1.2 Process类的介绍
1.3 Process类的使用
1.4 进程同步（锁）
1.5 进程间通信（IPC）方式一：队列（推荐使用）
1.6 进程间通信（IPC）方式二：管道（不推荐使用，了解即可）
1.7 进程间通信方式三：共享数据（不推荐使用，了解即可）
1.8 信号量，事件...（了解即可）
1.9 进程池
二 python并发编程之多线程
2.1  threading模块
2.1.1 开启线程的两种方式（同Process）
5.1.2 在一个进程下开启多个线程与在一个进程下开启多个子进程的区别
2.1.3 练习
2.1.4 线程的join与setdaemon
2.1.5 线程相关的其他方法补充
2.2  Python GIL(Global Interpreter Lock)
2.3 同步锁
2.4 死锁与递归锁
2.5 信号量Semahpore
2.6 Event
2.7 条件Condition（了解）
2.8 定时器
2.9 线程queue
2.10 Python标准模块--concurrent.futures
三 补充：paramiko模块
四 协程
五 Greenlet
六 Gevent
```

## 一 python并发编程之多进程

### 1.1 multiprocessing模块介绍

```
python中的多线程无法利用多核优势，如果想要充分地使用多核CPU的资源（os.cpu_count()查看），在python中大部分情况需要使用多进程。Python提供了非常好用的多进程包multiprocessing。
multiprocessing模块用来开启子进程，并在子进程中执行我们定制的任务（比如函数），该模块与多线程模块threading的编程接口类似。
```

　 multiprocessing模块的功能众多：支持子进程、通信和共享数据、执行不同形式的同步，提供了Process、Queue、Pipe、Lock等组件。
`需要再次强调的一点是：与线程不同，进程没有任何共享状态，进程修改的数据，改动仅限于该进程内。`
### 1.2 Process类的介绍
`创建进程的类：`
Process([group [, target [, name [, args [, kwargs]]]]])，由该类实例化得到的对象，表示一个子进程中的任务（尚未启动）

强调：
- 
需要使用关键字的方式来指定参数

- 
args指定的为传给target函数的位置参数，是一个元组形式，必须有逗号


参数介绍：

```python
group参数未使用，值始终为None
target表示调用对象，即子进程要执行的任务
args表示调用对象的位置参数元组，args=(1,2,'egon',)
kwargs表示调用对象的字典,kwargs={'name':'egon','age':18}
name为子进程的名称
```

方法介绍：

```
p.start()：启动进程，并调用该子进程中的p.run() 
p.run():进程启动时运行的方法，正是它去调用target指定的函数，我们自定义类的类中一定要实现该方法  
 
p.terminate():强制终止进程p，不会进行任何清理操作，如果p创建了子进程，该子进程就成了僵尸进程，使用该方法需要特别小心这种情况。如果p还保存了一个锁那么也将不会被释放，进而导致死锁
p.is_alive():如果p仍然运行，返回True

p.join([timeout]):主线程等待p终止（强调：是主线程处于等的状态，而p是处于运行的状态）。timeout是可选的超时时间，需要强调的是，p.join只能join住start开启的进程，而不能join住run开启的进程
```

属性介绍：

```
p.daemon：默认值为False，如果设为True，代表p为后台运行的守护进程，当p的父进程终止时，p也随之终止，并且设定为True后，p不能创建自己的新进程，必须在p.start()之前设置

p.name:进程的名称

p.pid：进程的pid

p.exitcode:进程在运行时为None、如果为–N，表示被信号N结束(了解即可)
 
p.authkey:进程的身份验证键,默认是由os.urandom()随机生成的32字符的字符串。这个键的用途是为涉及网络连接的底层进程间通信提供安全性，这类连接只有在具有相同的身份验证键时才能成功（了解即可）
```

### 1.3 Process类的使用

```
=====================part1：创建并开启子进程的两种方式

注意：在windows中Process()必须放到# if __name__ == '__main__':下

Since Windows has no fork, the multiprocessing module starts a new Python process and imports the calling module.
If Process() gets called upon import, then this sets off an infinite succession of new processes (or until your machine runs out of resources).
This is the reason for hiding calls to Process() inside

if __name__ == "__main__"
since statements inside this if-statement will not get called upon import.
```

由于Windows没有fork，多处理模块启动一个新的Python进程并导入调用模块。
如果在导入时调用Process（），那么这将启动无限继承的新进程（或直到机器耗尽资源）。
这是隐藏对Process（）内部调用的原，使用if **name** == “**main **”，这个if语句中的语句将不会在导入时被调用。

##### 开进程的方法一:

```python
import time
import random
from multiprocessing import Process
def piao(name):
    print('%s piaoing' %name)
    time.sleep(random.randrange(1,5))
    print('%s piao end' %name)



p1=Process(target=piao,args=('egon',)) #必须加,号
p2=Process(target=piao,args=('alex',))
p3=Process(target=piao,args=('wupeqi',))
p4=Process(target=piao,args=('yuanhao',))

p1.start()
p2.start()
p3.start()
p4.start()
print('主线程')
```

##### 开进程的方法二:

```python
import time
import random
from multiprocessing import Process


class Piao(Process):
    def __init__(self,name):
        super().__init__()
        self.name=name
    def run(self):
        print('%s piaoing' %self.name)

        time.sleep(random.randrange(1,5))
        print('%s piao end' %self.name)

p1=Piao('egon')
p2=Piao('alex')
p3=Piao('wupeiqi')
p4=Piao('yuanhao')

p1.start() #start会自动调用run
p2.start()
p3.start()
p4.start()
print('主线程')
```

##### 练习1：把上周所学的socket通信变成并发的形式

```python
from socket import *
from multiprocessing import Process

server=socket(AF_INET,SOCK_STREAM)
server.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
server.bind(('127.0.0.1',8080))
server.listen(5)

def talk(conn,client_addr):
    while True:
        try:
            msg=conn.recv(1024)
            if not msg:break
            conn.send(msg.upper())
        except Exception:
            break

if __name__ == '__main__': #windows下start进程一定要写到这下面
    while True:
        conn,client_addr=server.accept()
        p=Process(target=talk,args=(conn,client_addr))
        p.start()



from socket import *

client=socket(AF_INET,SOCK_STREAM)
client.connect(('127.0.0.1',8080))


while True:
    msg=input('>>: ').strip()
    if not msg:continue

    client.send(msg.encode('utf-8'))
    msg=client.recv(1024)
    print(msg.decode('utf-8'))
```

每来一个客户端，都在服务端开启一个进程，如果并发来一个万个客户端，要开启一万个进程吗，你自己尝试着在你自己的机器上开启一万个，10万个进程试一试。
解决方法：进程池

##### part2： Process对象的其他方法或属性

```python
#进程对象的其他方法一:terminate,is_alive
from multiprocessing import Process
import time
import random

class Piao(Process):
    def __init__(self,name):
        self.name=name
        super().__init__()

    def run(self):
        print('%s is piaoing' %self.name)
        time.sleep(random.randrange(1,5))
        print('%s is piao end' %self.name)


p1=Piao('egon1')
p1.start()

p1.terminate()#关闭进程,不会立即关闭,所以is_alive立刻查看的结果可能还是存活
print(p1.is_alive()) #结果为True

print('开始')
print(p1.is_alive()) #结果为False
```

注意了：p.join(),是父进程在等p的结束，是父进程阻塞在原地，而p仍然在后台运行

##### 进程对象的其他方法二:

```python
p.daemon=True,p.join
from multiprocessing import Process
import time
import random

class Piao(Process):
    def __init__(self,name):
        self.name=name
        super().__init__()
    def run(self):
        print('%s is piaoing' %self.name)
        time.sleep(random.randrange(1,3))
        print('%s is piao end' %self.name)


p=Piao('egon')
p.daemon=True #一定要在p.start()前设置,设置p为守护进程,禁止p创建子进程,并且父进程死,p跟着一起死
p.start()
p.join(0.0001) #等待p停止,等0.0001秒就不再等了
print('开始')


from multiprocessing import Process

import time
import random
def piao(name):
    print('%s is piaoing' %name)
    time.sleep(random.randint(1,3))
    print('%s is piao end' %name)

p1=Process(target=piao,args=('egon',))
p2=Process(target=piao,args=('alex',))
p3=Process(target=piao,args=('yuanhao',))
p4=Process(target=piao,args=('wupeiqi',))

p1.start()
p2.start()
p3.start()
p4.start()
```
- 疑问:既然join是等待进程结束,那么我像下面这样写,进程不就又变成串行的了吗?
- 当然不是了,必须明确：p.join()是让谁等？
- 
很明显p.join()是让主线程等待p的结束，卡住的是主线程而绝非进程p，

- 详细解析如下：
- 进程只要start就会在开始运行了,所以p1-p4.start()时,系统中已经有四个并发的进程了
- 而我们p1.join()是在等p1结束,没错p1只要不结束主线程就会一直卡在原地,这也是问题的关键
- 
join是让主线程等,而p1-p4仍然是并发执行的,p1.join的时候,其余p2,p3,p4仍然在运行,等#p1.join结束,可能p2,p3,p4早已经结束了,这样p2.join,p3.join.p4.join直接通过检测，无需等待


所以4个join花费的总时间仍然是耗费时间最长的那个进程运行的时间

```
p1.join()
p2.join()
p3.join()
p4.join()

print('主线程')
```

上述启动进程与join进程可以简写为

```
p_l=[p1,p2,p3,p4]
 
for p in p_l:
    p.start()

for p in p_l:
    p.join()
```

##### 进程对象的其他属性:name,pid

```python
from multiprocessing import Process
import time
import random
class Piao(Process):
    def __init__(self,name):
        # self.name=name
        # super().__init__() #Process的__init__方法会执行self.name=Piao-1,
        #                    #所以加到这里,会覆盖我们的self.name=name

        #为我们开启的进程设置名字的做法
        super().__init__()
        self.name=name

    def run(self):
        print('%s is piaoing' %self.name)
        time.sleep(random.randrange(1,3))
        print('%s is piao end' %self.name)

p=Piao('egon')
p.start()
print('开始')
print(p.pid) #查看pid
```

### 1.4 进程同步（锁）

进程之间数据不共享,但是共享同一套文件系统,所以访问同一个文件,或同一个打印终端,是没有问题的,
part1：共享同一打印终端，发现会有多行内容打印到一行的现象（多个进程共享并抢占同一个打印终端，乱了）

多进程共享一个打印终端(用python2测试看两个进程同时往一个终端打印,出现打印到一行的错误)

```python
from multiprocessing import Process
import time
class Logger(Process):
    def __init__(self):
        super(Logger,self).__init__()
    def run(self):
        print(self.name)


for i in range(1000000):
    l=Logger()
    l.start()
```

part2：共享同一个文件，既然可以用文件共享数据，那么进程间通信用文件作为数据传输介质就可以了啊，可以，但是有问题：1.效率 2.需要自己加锁处理

##### 多进程共享一套文件系统

```python
from multiprocessing import Process
import time,random

def work(f,msg):
    f.write(msg)
    f.flush()


f=open('a.txt','w') #在windows上无法把f当做参数传入，可以传入一个文件名，然后在work内用a+的方式打开文件，进行写入测试
for i in range(5):
    p=Process(target=work,args=(f,str(i)))
    p.start()
```

需知：加锁的目的是为了保证多个进程修改同一块数据时，同一时间只能有一个修改，即串行的修改，没错，速度是慢了，牺牲了速度而保证了数据安全。

进程之间数据隔离，但是共享一套文件系统，因而可以通过文件来实现进程直接的通信，但问题是必须自己加锁处理

所以，就让我们帮文件当做数据库，模拟抢票（Lock互斥锁）

```python
#文件db的内容为：{"count":1}
#注意一定要用双引号，不然json无法识别
from multiprocessing import Process,Lock
import json
import time
import random
import os

def work(filename,lock): #买票
    # lock.acquire()
    with lock:
        with open(filename,encoding='utf-8') as f:
            dic=json.loads(f.read())
            # print('剩余票数: %s' % dic['count'])
        if dic['count'] > 0:
            dic['count']-=1
            time.sleep(random.randint(1,3)) #模拟网络延迟
            with open(filename,'w',encoding='utf-8') as f:
                f.write(json.dumps(dic))
            print('%s 购票成功' %os.getpid())
        else:
            print('%s 购票失败' %os.getpid())
    # lock.release()

if __name__ == '__main__':
    lock=Lock()
    p_l=[]
    for i in range(100):
        p=Process(target=work,args=('db',lock))
        p_l.append(p)
        p.start()
    for p in p_l:
        p.join()

    print('主线程')
```

基于上例，我们学习了通过使用共享的文件的方式，实现进程直接的共享，即共享数据的方式，这种方式必须考虑周全同步、锁等问题。而且文件是操作系统提供的抽象，可以作为进程直接通信的介质，与mutiprocess模块无关。

但其实mutiprocessing模块为我们提供了基于消息的IPC通信机制：队列和管道。

IPC机制中的队列又是基于（管道+锁）实现的，可以让我们从复杂的锁问题中解脱出来，

我们应该尽量避免使用共享数据，尽可能使用消息传递和队列，避免处理复杂的同步和锁问题，而且在进程数目增多时，往往可以获得更好的可获展性。

### 1.5 进程间通信（IPC）方式一：队列（推荐使用）

```
进程彼此之间互相隔离，要实现进程间通信（IPC），multiprocessing模块支持两种形式：队列和管道，这两种方式都是使用消息传递的

创建队列的类（底层就是以管道和锁定的方式实现）：
```

Queue([maxsize]):创建共享的进程队列，Queue是多进程安全的队列，可以使用Queue实现多进程之间的数据传递。

##### 参数介绍：

maxsize是队列中允许最大项数，省略则无大小限制。

##### 方法介绍：

```
主要方法：

q.put方法用以插入数据到队列中，put方法还有两个可选参数：blocked和timeout。如果blocked为True（默认值），并且timeout为正值，该方法会阻塞timeout指定的时间，直到该队列有剩余的空间。如果超时，会抛出Queue.Full异常。如果blocked为False，但该Queue已满，会立即抛出Queue.Full异常。

q.get方法可以从队列读取并且删除一个元素。同样，get方法有两个可选参数：blocked和timeout。如果blocked为True（默认值），并且timeout为正值，那么在等待时间内没有取到任何元素，会抛出Queue.Empty异常。如果blocked为False，有两种情况存在，如果Queue有一个值可用，则立即返回该值，否则，如果队列为空，则立即抛出Queue.Empty异常.
  
q.get_nowait():同q.get(False)
q.put_nowait():同q.put(False)

q.empty():调用此方法时q为空则返回True，该结果不可靠，比如在返回True的过程中，如果队列中又加入了项目。
q.full()：调用此方法时q已满则返回True，该结果不可靠，比如在返回True的过程中，如果队列中的项目被取走。
q.qsize():返回队列中目前项目的正确数量，结果也不可靠，理由同q.empty()和q.full()一样
```

##### 其他方法(了解)：

```
q.cancel_join_thread():不会在进程退出时自动连接后台线程。可以防止join_thread()方法阻塞
q.close():关闭队列，防止队列中加入更多数据。调用此方法，后台线程将继续写入那些已经入队列但尚未写入的数据，但将在此方法完成时马上关闭。如果q被垃圾收集，将调用此方法。关闭队列不会在队列使用者中产生任何类型的数据结束信号或异常。例如，如果某个使用者正在被阻塞在get()操作上，关闭生产者中的队列不会导致get()方法返回错误。
q.join_thread()：连接队列的后台线程。此方法用于在调用q.close()方法之后，等待所有队列项被消耗。默认情况下，此方法由不是q的原始创建者的所有进程调用。调用q.cancel_join_thread方法可以禁止这种行为
```

##### 应用：

```python
'''
multiprocessing模块支持进程间通信的两种主要形式:管道和队列
都是基于消息传递实现的,但是队列接口
'''

from multiprocessing import Process,Queue
import time
q=Queue(3)


#put ,get ,put_nowait,get_nowait,full,empty
q.put(3)
q.put(3)
q.put(3)
print(q.full()) #满了

print(q.get())
print(q.get())
print(q.get())
print(q.empty()) #空了
```

##### 生产者消费者模型

在并发编程中使用生产者和消费者模式能够解决绝大多数并发问题。该模式通过平衡生产线程和消费线程的工作能力来提高程序的整体处理数据的速度。

为什么要使用生产者和消费者模式

在线程世界里，生产者就是生产数据的线程，消费者就是消费数据的线程。在多线程开发当中，如果生产者处理速度很快，而消费者处理速度很慢，那么生产者就必须等待消费者处理完，才能继续生产数据。同样的道理，如果消费者的处理能力大于生产者，那么消费者就必须等待生产者。为了解决这个问题于是引入了生产者和消费者模式。

什么是生产者消费者模式

生产者消费者模式是通过一个容器来解决生产者和消费者的强耦合问题。生产者和消费者彼此之间不直接通讯，而通过阻塞队列来进行通讯，所以生产者生产完数据之后不用等待消费者处理，直接扔给阻塞队列，消费者不找生产者要数据，而是直接从阻塞队列里取，阻塞队列就相当于一个缓冲区，平衡了生产者和消费者的处理能力。

##### 基于队列实现生产者消费者模型

```python
from multiprocessing import Process,Queue
import time,random,os

def consumer(q):
    while True:
        time.sleep(random.randint(1,3))
        res=q.get()
        print('\033[45m消费者拿到了：%s\033[0m' %res)

def producer(seq,q):
    for item in seq:
        time.sleep(random.randint(1,3))
        print('\033[46m生产者生产了：%s\033[0m' %item)

        q.put(item)

if __name__ == '__main__':
    q=Queue()

    seq=('包子%s' %i for i in range(10))
    c=Process(target=consumer,args=(q,))
    c.start()
    producer(seq,q)

    print('主线程')


from multiprocessing import Process,Queue
import time,random,os


def consumer(q):
    while True:
        time.sleep(random.randint(1,3))
        res=q.get()
        if res is None:break
        print('\033[45m消费者拿到了：%s\033[0m' %res)

def producer(seq,q):
    for item in seq:
        time.sleep(random.randint(1,3))
        print('\033[46m生产者生产了：%s\033[0m' %item)

        q.put(item)

if __name__ == '__main__':
    q=Queue()

    c=Process(target=consumer,args=(q,))
    c.start()

    producer(('包子%s' %i for i in range(10)),q)
    q.put(None)
    c.join()
    print('主线程')
```

##### 创建队列的另外一个类：

```
JoinableQueue([maxsize])：这就像是一个Queue对象，但队列允许项目的使用者通知生成者项目已经被成功处理。通知进程是使用共享的信号和条件变量来实现的。

参数介绍：
maxsize是队列中允许最大项数，省略则无大小限制。
```

##### 方法介绍：

```python
JoinableQueue的实例p除了与Queue对象相同的方法之外还具有：
q.task_done()：使用者使用此方法发出信号，表示q.get()的返回项目已经被处理。如果调用此方法的次数大于从队列中删除项目的数量，将引发ValueError异常
q.join():生产者调用此方法进行阻塞，直到队列中所有的项目均被处理。阻塞将持续到队列中的每个项目均调用q.task_done（）方法为止

from multiprocessing import Process,JoinableQueue
import time,random
def consumer(q):
    while True:
        # time.sleep(random.randint(1,2))
        res=q.get()
        print('消费者拿到了 %s' %res)
        q.task_done()


def producer(seq,q):
    for item in seq:
        # time.sleep(random.randrange(1,2))
        q.put(item)
        print('生产者做好了 %s' %item)
    q.join()

if __name__ == '__main__':
    q=JoinableQueue()
    seq=('包子%s' %i for i in range(10))

    p=Process(target=consumer,args=(q,))
    p.daemon=True #设置为守护进程，在主线程停止时p也停止，但是不用担心，producer内调用q.join保证了consumer已经处理完队列中的所有元素
    p.start()

    producer(seq,q)

    print('主线程')

from multiprocessing import Process,JoinableQueue
import time,random
def consumer(name,q):
    while True:
        time.sleep(random.randint(1,2))
        res=q.get()
        print('\033[45m%s拿到了 %s\033[0m' %(name,res))
        q.task_done()


def producer(seq,q):
    for item in seq:
        time.sleep(random.randrange(1,2))
        q.put(item)
        print('\033[46m生产者做好了 %s\033[0m' %item)
    q.join()

if __name__ == '__main__':
    q=JoinableQueue()
    seq=('包子%s' %i for i in range(10))

    p1=Process(target=consumer,args=('消费者1',q,))
    p2=Process(target=consumer,args=('消费者2',q,))
    p3=Process(target=consumer,args=('消费者3',q,))
    p1.daemon=True
    p2.daemon=True
    p3.daemon=True
    p1.start()
    p2.start()
    p3.start()

    producer(seq,q)

    print('主线程')


from multiprocessing import Process,JoinableQueue
import time,random
def consumer(name,q):
    while True:
        # time.sleep(random.randint(1,2))
        res=q.get()
        print('\033[45m%s拿到了 %s\033[0m' %(name,res))
        q.task_done()


def producer(seq,q):
    for item in seq:
        # time.sleep(random.randrange(1,2))
        q.put(item)
        print('\033[46m生产者做好了 %s\033[0m' %item)
    q.join()

if __name__ == '__main__':
    q=JoinableQueue()
    seq=['包子%s' %i for i in range(10)] #在windows下无法传入生成器，我们可以用列表解析测试

    p1=Process(target=consumer,args=('消费者1',q,))
    p2=Process(target=consumer,args=('消费者2',q,))
    p3=Process(target=consumer,args=('消费者3',q,))
    p1.daemon=True
    p2.daemon=True
    p3.daemon=True
    p1.start()
    p2.start()
    p3.start()

    # producer(seq,q) #也可以是下面三行的形式，开启一个新的子进程当生产者，不用主线程当生产者
    p4=Process(target=producer,args=(seq,q))
    p4.start()
    p4.join()
    print('主线程')
```

### 1.6 进程间通信（IPC）方式二：管道（不推荐使用，了解即可）

```
管道也可以说是队列的另外一种形式，下面我们就开始介绍基于管道实现金城之间的消息传递

创建管道的类：
```

Pipe([duplex]):在进程之间创建一条管道，并返回元组（conn1,conn2）,其中conn1，conn2表示管道两端的连接对象，强调一点：必须在产生Process对象之前产生管道
`参数介绍：`
dumplex:默认管道是全双工的，如果将duplex射成False，conn1只能用于接收，conn2只能用于发送。

方法介绍：
主要方法：
conn1.recv():接收conn2.send(obj)发送的对象。如果没有消息可接收，recv方法会一直阻塞。如果连接的另外一端已经关闭，那么recv方法会抛出EOFError。
conn1.send(obj):通过连接发送对象。obj是与序列化兼容的任意对象

其他方法：

```python
conn1.close():关闭连接。如果conn1被垃圾回收，将自动调用此方法
conn1.fileno():返回连接使用的整数文件描述符
conn1.poll([timeout]):如果连接上的数据可用，返回True。timeout指定等待的最长时限。如果省略此参数，方法将立即返回结果。如果将timeout射成None，操作将无限期地等待数据到达。
 
conn1.recv_bytes([maxlength]):接收c.send_bytes()方法发送的一条完整的字节消息。maxlength指定要接收的最大字节数。如果进入的消息，超过了这个最大值，将引发IOError异常，并且在连接上无法进行进一步读取。如果连接的另外一端已经关闭，再也不存在任何数据，将引发EOFError异常。
conn.send_bytes(buffer [, offset [, size]])：通过连接发送字节数据缓冲区，buffer是支持缓冲区接口的任意对象，offset是缓冲区中的字节偏移量，而size是要发送字节数。结果数据以单条消息的形式发出，然后调用c.recv_bytes()函数进行接收    
 
conn1.recv_bytes_into(buffer [, offset]):接收一条完整的字节消息，并把它保存在buffer对象中，该对象支持可写入的缓冲区接口（即bytearray对象或类似的对象）。offset指定缓冲区中放置消息处的字节位移。返回值是收到的字节数。如果消息长度大于可用的缓冲区空间，将引发BufferTooShort异常。



基于管道实现进程间通信（与队列的方式是类似的，队列就是管道加锁实现的）：

from multiprocessing import Process,Pipe

import time,os
def consumer(p,name):
    left,right=p
    left.close()
    while True:
        try:
            baozi=right.recv()
            print('%s 收到包子:%s' %(name,baozi))
        except EOFError:
            right.close()
            break
def producer(seq,p):
    left,right=p
    right.close()
    for i in seq:
        left.send(i)
        # time.sleep(1)
    else:
        left.close()
if __name__ == '__main__':
    left,right=Pipe()

    c1=Process(target=consumer,args=((left,right),'c1'))
    c1.start()


    seq=(i for i in range(10))
    producer(seq,(left,right))

    right.close()
    left.close()

    c1.join()
    print('主进程')
```

注意：生产者和消费者都没有使用管道的某个端点，就应该将其关闭，如在生产者中关闭管道的右端，在消费者中关闭管道的左端。如果忘记执行这些步骤，程序可能再消费者中的recv()操作上挂起。管道是由操作系统进行引用计数的,必须在所有进程中关闭管道后才能生产EOFError异常。因此在生产者中关闭管道不会有任何效果，付费消费者中也关闭了相同的管道端点。

```python
管道可以用于双向通信，利用通常在客户端/服务器中使用的请求／响应模型或远程过程调用，就可以使用管道编写与进程交互的程序，如下

from multiprocessing import Process,Pipe

import time,os
def adder(p,name):
    server,client=p
    client.close()
    while True:
        try:
            x,y=server.recv()
        except EOFError:
            server.close()
            break
        res=x+y
        server.send(res)
    print('server done')
if __name__ == '__main__':
    server,client=Pipe()

    c1=Process(target=adder,args=((server,client),'c1'))
    c1.start()

    server.close()

    client.send((10,20))
    print(client.recv())
    client.close()

    c1.join()
    print('主进程')
```

注意：send()和recv()方法使用pickle模块对对象进行序列化。

### 1.7 进程间通信方式三：共享数据（不推荐使用，了解即可）

展望未来，基于消息传递的并发编程是大势所趋

即便是使用线程，推荐做法也是将程序设计为大量独立的线程集合

通过消息队列交换数据。这样极大地减少了对使用锁定和其他同步手段的需求，

还可以扩展到分布式系统中

进程间通信应该尽量避免使用本节所讲的共享数据的方式

进程间数据是独立的，可以借助于队列或管道实现通信，二者都是基于消息传递的

虽然进程间数据独立，但可以通过Manager实现数据共享，事实上Manager的功能远不止于此

A manager object returned by Manager() controls a server process which holds Python objects and allows other processes to manipulate them using proxies.

A manager returned by Manager() will support types list, dict, Namespace, Lock, RLock, Semaphore, BoundedSemaphore, Condition, Event, Barrier, Queue, Value and Array. For example,

```python
from multiprocessing import Process,Manager
import os

def foo(name,d,l):
    l.append(os.getpid())
    d[name]=os.getpid()
if __name__ == '__main__':
    with Manager() as manager:
        d=manager.dict({'name':'egon'})
        l=manager.list(['init',])

        p_l=[]
        for i in range(10):
            p=Process(target=foo,args=('p%s' %i,d,l))
            p.start()
            p_l.append(p)

        for p in p_l:
            p.join() #必须有join不然会报错

        print(d)
        print(l)
```

使用共享数据也需要自己加锁处理

### 1.8 信号量，事件...（了解即可）

```python
互斥锁 同时只允许一个线程更改数据，而Semaphore是同时允许一定数量的线程更改数据 ，比如厕所有3个坑，那最多只允许3个人上厕所，后面的人只能等里面有人出来了才能再进去，如果指定信号量为3，那么来一个人获得一把锁，计数加1，当计数等于3时，后面的人均需要等待。一旦释放，就有人可以获得一把锁

信号量与进程池的概念很像，但是要区分开，信号量涉及到加锁的概念

from multiprocessing import Process,Semaphore
import time,random

def go_wc(sem,user):
    sem.acquire()
    print('%s 占到一个茅坑' %user)
    time.sleep(random.randint(0,3)) #模拟每个人拉屎速度不一样，0代表有的人蹲下就起来了
    sem.release()

if __name__ == '__main__':
    sem=Semaphore(5)
    p_l=[]
    for i in range(13):
        p=Process(target=go_wc,args=(sem,'user%s' %i,))
        p.start()
        p_l.append(p)

    for i in p_l:
        i.join()
    print('============》')
```

python线程的事件用于主线程控制其他线程的执行，事件主要提供了三个方法 set、wait、clear。
`事件处理的机制：全局定义了一个“Flag”，如果“Flag”值为 False，那么当程序执行 event.wait 方法时就会阻塞，如果“Flag”值为True，那么event.wait 方法时便不再阻塞。`
clear：将“Flag”设置为False
set：将“Flag”设置为True

```python
#_*_coding:utf-8_*_
#!/usr/bin/env python

from multiprocessing import Process,Event
import time,random

def car(e,n):
    while True:
        if not e.is_set(): #Flase
            print('\033[31m红灯亮\033[0m，car%s等着' %n)
            e.wait()
            print('\033[32m车%s 看见绿灯亮了\033[0m' %n)
            time.sleep(random.randint(3,6))
            if not e.is_set():
                continue
            print('走你,car', n)
            break

def police_car(e,n):
    while True:
        if not e.is_set():
            print('\033[31m红灯亮\033[0m，car%s等着' % n)
            e.wait(1)
            print('灯的是%s，警车走了,car %s' %(e.is_set(),n))
            break

def traffic_lights(e,inverval):
    while True:
        time.sleep(inverval)
        if e.is_set():
            e.clear() #e.is_set() ---->False
        else:
            e.set()

if __name__ == '__main__':
    e=Event()
    # for i in range(10):
    #     p=Process(target=car,args=(e,i,))
    #     p.start()

    for i in range(5):
        p = Process(target=police_car, args=(e, i,))
        p.start()
    t=Process(target=traffic_lights,args=(e,10))
    t.start()

    print('============》')
```

### 1.9 进程池

```
开多进程的目的是为了并发，如果有多核，通常有几个核就开几个进程，进程开启过多，效率反而会下降（开启进程是需要占用系统资源的，而且开启多余核数目的进程也无法做到并行），但很明显需要并发执行的任务要远大于核数，这时我们就可以通过维护一个进程池来控制进程数目，比如httpd的进程模式，规定最小进程数和最大进程数...    

当被操作对象数目不大时，可以直接利用multiprocessing中的Process动态成生多个进程，十几个还好，但如果是上百个，上千个目标，手动的去限制进程数量却又太过繁琐，此时可以发挥进程池的功效。
```

而且对于远程过程调用的高级应用程序而言，应该使用进程池，Pool可以提供指定数量的进程，供用户调用，当有新的请求提交到pool中时，如果池还没有满，那么就会创建一个新的进程用来执行该请求；但如果池中的进程数已经达到规定最大值，那么该请求就会等待，直到池中有进程结束，就重用进程池中的进程。

```
在利用Python进行系统管理的时候，特别是同时操作多个文件目录，或者远程控制多台主机，并行操作可以节约大量的时间。



创建进程池的类：

Pool([numprocess  [,initializer [, initargs]]]):创建进程池



参数介绍：

numprocess:要创建的进程数，如果省略，将默认使用cpu_count()的值
initializer：是每个工作进程启动时要执行的可调用对象，默认为None
initargs：是要传给initializer的参数组
```

　 方法介绍：
主要方法：

```
p.apply(func [, args [, kwargs]]):在一个池工作进程中执行func(*args,**kwargs),然后返回结果。需要强调的是：此操作并不会在所有池工作进程中并执行func函数。如果要通过不同参数并发地执行func函数，必须从不同线程调用p.apply()函数或者使用p.apply_async()
p.apply_async(func [, args [, kwargs]]):在一个池工作进程中执行func(*args,**kwargs),然后返回结果。此方法的结果是AsyncResult类的实例，callback是可调用对象，接收输入参数。当func的结果变为可用时，将理解传递给callback。callback禁止执行任何阻塞操作，否则将接收其他异步操作中的结果。
   
p.close():关闭进程池，防止进一步操作。如果所有操作持续挂起，它们将在工作进程终止前完成5 P.jion():等待所有工作进程退出。此方法只能在close（）或teminate()之后调用
```

其他方法（了解部分）

```
方法apply_async()和map_async（）的返回值是AsyncResul的实例obj。实例具有以下方法
obj.get():返回结果，如果有必要则等待结果到达。timeout是可选的。如果在指定时间内还没有到达，将引发一场。如果远程操作中引发了异常，它将在调用此方法时再次被引发。
obj.ready():如果调用完成，返回True
obj.successful():如果调用完成且没有引发异常，返回True，如果在结果就绪之前调用此方法，引发异常
obj.wait([timeout]):等待结果变为可用。
obj.terminate()：立即终止所有工作进程，同时不执行任何清理或结束任何挂起工作。如果p被垃圾回收，将自动调用此函数
```

应用
提交任务，并在主进程中拿到结果（之前的Process是执行任务，结果放到队列里，现在可以在主进程中直接拿到结果）

```python
from multiprocessing import Pool
import time
def work(n):
    print('开工啦...')
    time.sleep(3)
    return n**2

if __name__ == '__main__':
    q=Pool()

    #异步apply_async用法：如果使用异步提交的任务，主进程需要使用jion，等待进程池内任务都处理完，然后可以用get收集结果，否则，主进程结束，进程池可能还没来得及执行，也就跟着一起结束了
    res=q.apply_async(work,args=(2,))
    q.close()
    q.join() #join在close之后调用
    print(res.get())

    #同步apply用法：主进程一直等apply提交的任务结束后才继续执行后续代码
    # res=q.apply(work,args=(2,))
    # print(res)
```

##### 一：使用进程池（非阻塞,apply_async）

```python
#coding: utf-8
from multiprocessing import Process,Pool
import time

def func(msg):
    print( "msg:", msg)
    time.sleep(1)
    return msg

if __name__ == "__main__":
    pool = Pool(processes = 3)
    res_l=[]
    for i in range(10):
        msg = "hello %d" %(i)
        res=pool.apply_async(func, (msg, ))   #维持执行的进程总数为processes，当一个进程执行完毕后会添加新的进程进去
        res_l.append(res)
    print("==============================>") #没有后面的join，或get，则程序整体结束，进程池中的任务还没来得及全部执行完也都跟着主进程一起结束了

    pool.close() #关闭进程池，防止进一步操作。如果所有操作持续挂起，它们将在工作进程终止前完成
    pool.join()   #调用join之前，先调用close函数，否则会出错。执行完close后不会有新的进程加入到pool,join函数等待所有子进程结束

    print(res_l) #看到的是<multiprocessing.pool.ApplyResult object at 0x10357c4e0>对象组成的列表,而非最终的结果,但这一步是在join后执行的,证明结果已经计算完毕,剩下的事情就是调用每个对象下的get方法去获取结果
    for i in res_l:
        print(i.get()) #使用get来获取apply_aync的结果,如果是apply,则没有get方法,因为apply是同步执行,立刻获取结果,也根本无需get
```

##### 二：使用进程池（阻塞,apply）

```python
#coding: utf-8
from multiprocessing import Process,Pool
import time

def func(msg):
    print( "msg:", msg)
    time.sleep(0.1)
    return msg

if __name__ == "__main__":
    pool = Pool(processes = 3)
    res_l=[]
    for i in range(10):
        msg = "hello %d" %(i)
        res=pool.apply(func, (msg, ))   #维持执行的进程总数为processes，当一个进程执行完毕后会添加新的进程进去
        res_l.append(res) #同步执行，即执行完一个拿到结果，再去执行另外一个
    print("==============================>")
    pool.close()
    pool.join()   #调用join之前，先调用close函数，否则会出错。执行完close后不会有新的进程加入到pool,join函数等待所有子进程结束

    print(res_l) #看到的就是最终的结果组成的列表
    for i in res_l: #apply是同步的，所以直接得到结果，没有get()方法
        print(i)


#coding: utf-8
import multiprocessing
import os, time, random

def Lee():
    print("\nRun task Lee-%s" %(os.getpid())) #os.getpid()获取当前的进程的ID
    start = time.time()
    time.sleep(random.random() * 10) #random.random()随机生成0-1之间的小数
    end = time.time()
    print('Task Lee, runs %0.2f seconds.' %(end - start))

def Marlon():
    print("\nRun task Marlon-%s" %(os.getpid()))
    start = time.time()
    time.sleep(random.random() * 40)
    end=time.time()
    print('Task Marlon runs %0.2f seconds.' %(end - start))

def Allen():
    print("\nRun task Allen-%s" %(os.getpid()))
    start = time.time()
    time.sleep(random.random() * 30)
    end = time.time()
    print('Task Allen runs %0.2f seconds.' %(end - start))

def Frank():
    print("\nRun task Frank-%s" %(os.getpid()))
    start = time.time()
    time.sleep(random.random() * 20)
    end = time.time()
    print('Task Frank runs %0.2f seconds.' %(end - start))

def Egon():
    print("\nRun task Egon-%s" %(os.getpid()))
    start = time.time()
    time.sleep(random.random() * 20)
    end = time.time()
    print('Task Egon runs %0.2f seconds.' %(end - start))

def Lily():
    print("\nRun task Lily-%s" %(os.getpid()))
    start = time.time()
    time.sleep(random.random() * 20)
    end = time.time()
    print('Task Lily runs %0.2f seconds.' %(end - start))

if __name__=='__main__':
    function_list=  [Lee, Marlon, Allen, Frank, Egon, Lily]
    print("parent process %s" %(os.getpid()))

    pool=multiprocessing.Pool(4)
    for func in function_list:
        pool.apply_async(func)     #Pool执行函数，apply执行函数,当有一个进程执行完毕后，会添加一个新的进程到pool中

    print('Waiting for all subprocesses done...')
    pool.close()
    pool.join()    #调用join之前，一定要先调用close() 函数，否则会出错, close()执行后不会有新的进程加入到pool,join函数等待素有子进程结束
    print('All subprocesses done.')
```

##### 多个进程池

练习2：使用进程池维护固定数目的进程（重写练习1）
复制代码

```python
#Pool内的进程数默认是cpu核数，假设为4（查看方法os.cpu_count()）
#开启6个客户端，会发现2个客户端处于等待状态
#在每个进程内查看pid，会发现pid使用为4个，即多个客户端公用4个进程
from socket import *
from multiprocessing import Pool
import os

server=socket(AF_INET,SOCK_STREAM)
server.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
server.bind(('127.0.0.1',8080))
server.listen(5)

def talk(conn,client_addr):
    print('进程pid: %s' %os.getpid())
    while True:
        try:
            msg=conn.recv(1024)
            if not msg:break
            conn.send(msg.upper())
        except Exception:
            break

if __name__ == '__main__':
    p=Pool()
    while True:
        conn,client_addr=server.accept()
        p.apply_async(talk,args=(conn,client_addr))
        # p.apply(talk,args=(conn,client_addr)) #同步的话，则同一时间只有一个客户端能访问

from socket import *

client=socket(AF_INET,SOCK_STREAM)
client.connect(('127.0.0.1',8080))


while True:
    msg=input('>>: ').strip()
    if not msg:continue

    client.send(msg.encode('utf-8'))
    msg=client.recv(1024)
    print(msg.decode('utf-8'))
```

回调函数（apply_async的扩展用法）
不需要回调函数的场景：如果在主进程中等待进程池中所有任务都执行完毕后，再统一处理结果，则无需回调函数

```python
from multiprocessing import Pool
import time,random,os

def work(n):
    time.sleep(1)
    return n**2
if __name__ == '__main__':
    p=Pool()

    res_l=[]
    for i in range(10):
        res=p.apply_async(work,args=(i,))
        res_l.append(res)

    p.close()
    p.join() #等待进程池中所有进程执行完毕

    nums=[]
    for res in res_l:
        nums.append(res.get()) #拿到所有结果
    print(nums) #主进程拿到所有的处理结果,可以在主进程中进行统一进行处理
```

需要回调函数的场景：进程池中任何一个任务一旦处理完了，就立即告知主进程：我好了额，你可以处理我的结果了。主进程则调用一个函数去处理该结果，该函数即回调函数

我们可以把耗时间（阻塞）的任务放到进程池中，然后指定回调函数（主进程负责执行），这样主进程在执行回调函数时就省去了I/O的过程，直接拿到的是任务的结果。

```python
from multiprocessing import Pool
import time,random,os

def get_page(url):
    print('(进程 %s) 正在下载页面 %s' %(os.getpid(),url))
    time.sleep(random.randint(1,3))
    return url #用url充当下载后的结果

def parse_page(page_content):
    print('<进程 %s> 正在解析页面: %s' %(os.getpid(),page_content))
    time.sleep(1)
    return '{%s 回调函数处理结果:%s}' %(os.getpid(),page_content)


if __name__ == '__main__':
    urls=[
        'http://maoyan.com/board/1',
        'http://maoyan.com/board/2',
        'http://maoyan.com/board/3',
        'http://maoyan.com/board/4',
        'http://maoyan.com/board/5',
        'http://maoyan.com/board/7',

    ]
    p=Pool()
    res_l=[]

    #异步的方式提交任务,然后把任务的结果交给callback处理
    #注意:会专门开启一个进程来处理callback指定的任务(单独的一个进程,而且只有一个)
    for url in urls:
        res=p.apply_async(get_page,args=(url,),callback=parse_page)
        res_l.append(res)

    #异步提交完任务后,主进程先关闭p(必须先关闭),然后再用p.join()等待所有任务结束(包括callback)
    p.close()
    p.join()
    print('{主进程 %s}' %os.getpid())

    #收集结果,发现收集的是get_page的结果
    #所以需要注意了:
    #1. 当我们想要在将get_page的结果传给parse_page处理,那么就不需要i.get(),通过指定callback,就可以将i.get()的结果传给callback执行的任务
    #2. 当我们想要在主进程中处理get_page的结果,那就需要使用i.get()获取后,再进一步处理
    for i in res_l: #本例中,下面这两步是多余的
        callback_res=i.get()
        print(callback_res)

'''
打印结果:
(进程 52346) 正在下载页面 http://maoyan.com/board/1
(进程 52347) 正在下载页面 http://maoyan.com/board/2
(进程 52348) 正在下载页面 http://maoyan.com/board/3
(进程 52349) 正在下载页面 http://maoyan.com/board/4
(进程 52348) 正在下载页面 http://maoyan.com/board/5
<进程 52345> 正在解析页面: http://maoyan.com/board/3
(进程 52346) 正在下载页面 http://maoyan.com/board/7
<进程 52345> 正在解析页面: http://maoyan.com/board/1
<进程 52345> 正在解析页面: http://maoyan.com/board/2
<进程 52345> 正在解析页面: http://maoyan.com/board/4
<进程 52345> 正在解析页面: http://maoyan.com/board/5
<进程 52345> 正在解析页面: http://maoyan.com/board/7
{主进程 52345}
http://maoyan.com/board/1
http://maoyan.com/board/2
http://maoyan.com/board/3
http://maoyan.com/board/4
http://maoyan.com/board/5
http://maoyan.com/board/7
'''


from multiprocessing import Pool
import time,random
import requests
import re

def get_page(url,pattern):
    response=requests.get(url)
    if response.status_code == 200:
        return (response.text,pattern)

def parse_page(info):
    page_content,pattern=info
    res=re.findall(pattern,page_content)
    for item in res:
        dic={
            'index':item[0],
            'title':item[1],
            'actor':item[2].strip()[3:],
            'time':item[3][5:],
            'score':item[4]+item[5]

        }
        print(dic)
if __name__ == '__main__':
    pattern1=re.compile(r'<dd>.*?board-index.*?>(\d+)<.*?title="(.*?)".*?star.*?>(.*?)<.*?releasetime.*?>(.*?)<.*?integer.*?>(.*?)<.*?fraction.*?>(.*?)<',re.S)

    url_dic={
        'http://maoyan.com/board/7':pattern1,
    }

    p=Pool()
    res_l=[]
    for url,pattern in url_dic.items():
        res=p.apply_async(get_page,args=(url,pattern),callback=parse_page)
        res_l.append(res)

    for i in res_l:
        i.get()

    # res=requests.get('http://maoyan.com/board/7')
    # print(re.findall(pattern,res.text))
```

## 二 python并发编程之多线程

### 2.1 threading模块

multiprocess模块的完全模仿了threading模块的接口，二者在使用层面，有很大的相似性，因而不再详细介绍
回到顶部

#### 2.1.1 开启线程的两种方式（同Process）

```python
#方式一
from threading import Thread
import time
def sayhi(name):
    time.sleep(2)
    print('%s say hello' %name)

if __name__ == '__main__':
    t=Thread(target=sayhi,args=('egon',))
    t.start()
    print('主线程')


#方式二
from threading import Thread
import time
class Sayhi(Thread):
    def __init__(self,name):
        super().__init__()
        self.name=name
    def run(self):
        time.sleep(2)
        print('%s say hello' % self.name)


if __name__ == '__main__':
    t = Sayhi('egon')
    t.start()
    print('主线程')
```

#### 2.1.2 在一个进程下开启多个线程与在一个进程下开启多个子进程的区别
- 
谁的开启速度更快？

from threading import Thread
from multiprocessing import Process
import os

def work():
print('hello')

if **name** == '**main**':
#在主进程下开启线程
t=Thread(target=work)
t.start()
print('主线程/主进程')
'''
打印结果:
hello
主线程/主进程
'''

```python
#在主进程下开启子进程
t=Process(target=work)
t.start()
print('主线程/主进程')
'''
打印结果:
主线程/主进程
hello
'''
```

- 
瞅一瞅pid？

from threading import Thread
from multiprocessing import Process
import os

def work():
print('hello',os.getpid())

if **name** == '**main**':
#part1:在主进程下开启多个线程,每个线程都跟主进程的pid一样
t1=Thread(target=work)
t2=Thread(target=work)
t1.start()
t2.start()
print('主线程/主进程pid',os.getpid())

```
#part2:开多个进程,每个进程都有不同的pid
p1=Process(target=work)
p2=Process(target=work)
p1.start()
p2.start()
print('主线程/主进程pid',os.getpid())
```


#### 2.1.3 练习

练习一：

```python
#_*_coding:utf-8_*_
#!/usr/bin/env python
import multiprocessing
import threading

import socket
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('127.0.0.1',8080))
s.listen(5)

def action(conn):
    while True:
        data=conn.recv(1024)
        print(data)
        conn.send(data.upper())

if __name__ == '__main__':

    while True:
        conn,addr=s.accept()


        p=threading.Thread(target=action,args=(conn,))
        p.start()


#_*_coding:utf-8_*_
#!/usr/bin/env python


import socket

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(('127.0.0.1',8080))

while True:
    msg=input('>>: ').strip()
    if not msg:continue

    s.send(msg.encode('utf-8'))
    data=s.recv(1024)
    print(data)
```

练习二：三个任务，一个接收用户输入，一个将用户输入的内容格式化成大写，一个将格式化后的结果存入文件

```python
from threading import Thread
msg_l=[]
format_l=[]
def talk():
    while True:
        msg=input('>>: ').strip()
        if not msg:continue
        msg_l.append(msg)

def format_msg():
    while True:
        if msg_l:
            res=msg_l.pop()
            format_l.append(res.upper())

def save():
    while True:
        if format_l:
            with open('db.txt','a',encoding='utf-8') as f:
                res=format_l.pop()
                f.write('%s\n' %res)

if __name__ == '__main__':
    t1=Thread(target=talk)
    t2=Thread(target=format_msg)
    t3=Thread(target=save)
    t1.start()
    t2.start()
    t3.start()
```

#### 2.1.4 线程的join与setdaemon

与进程的方法都是类似的，其实是multiprocessing模仿threading的接口

join与setdaemon

```python
from threading import Thread
import time
def sayhi(name):
    time.sleep(2)
    print('%s say hello' %name)

if __name__ == '__main__':
    t=Thread(target=sayhi,args=('egon',))
    t.setDaemon(True)
    t.start()
    t.join()
    print('主线程')
    print(t.is_alive())
```

#### 2.1.5 线程相关的其他方法补充

Thread实例对象的方法

```cpp
# isAlive(): 返回线程是否活动的。
  # getName(): 返回线程名。
  # setName(): 设置线程名。
```

threading模块提供的一些方法：

```python
# threading.currentThread(): 返回当前的线程变量。
  # threading.enumerate(): 返回一个包含正在运行的线程的list。正在运行指线程启动后、结束前，不包括启动前和终止后的线程。
  # threading.activeCount(): 返回正在运行的线程数量，与len(threading.enumerate())有相同的结果。


from threading import Thread
import threading
from multiprocessing import Process
import os

def work():
    import time
    time.sleep(3)
    print(threading.current_thread().getName())


if __name__ == '__main__':
    #在主进程下开启线程
    t=Thread(target=work)
    t.start()

    print(threading.current_thread().getName())
    print(threading.current_thread()) #主线程
    print(threading.enumerate()) #连同主线程在内有两个运行的线程
    print(threading.active_count())
    print('主线程/主进程')

    '''
    打印结果:
    MainThread
    <_MainThread(MainThread, started 140735268892672)>
    [<_MainThread(MainThread, started 140735268892672)>, <Thread(Thread-1, started 123145307557888)>]
    2
    主线程/主进程
    Thread-1
    '''
```

### 2.2 Python GIL(Global Interpreter Lock)

```
'''

定义：
In CPython, the global interpreter lock, or GIL, is a mutex that prevents multiple 
native threads from executing Python bytecodes at once. This lock is necessary mainly 
because CPython’s memory management is not thread-safe. (However, since the GIL 
exists, other features have grown to depend on the guarantees that it enforces.)

'''
```

结论：在Cpython解释器中，同一个进程下开启的多线程，同一时刻只能有一个线程执行，无法利用多核优势

首先需要明确的一点是GIL并不是Python的特性，它是在实现Python解析器(CPython)时所引入的一个概念。就好比C++是一套语言（语法）标准，但是可以用不同的编译器来编译成可执行代码。有名的编译器例如GCC，INTEL C++，Visual C++等。Python也一样，同样一段代码可以通过CPython，PyPy，Psyco等不同的Python执行环境来执行。像其中的JPython就没有GIL。然而因为CPython是大部分环境下默认的Python执行环境。所以在很多人的概念里CPython就是Python，也就想当然的把GIL归结为Python语言的缺陷。所以这里要先明确一点：GIL并不是Python的特性，Python完全可以不依赖于GIL

这篇文章透彻的剖析了GIL对python多线程的影响，强烈推荐看一下：[http://www.dabeaz.com/python/UnderstandingGIL.pdf](http://www.dabeaz.com/python/UnderstandingGIL.pdf)

关于GIL与Lock的比较请看2.3小节，此处只需知道：有了GIL的存在，同一时刻统一进程中只有一个线程被执行

听到这里，有的同学立马质问：进程可以利用多核，但是开销大，而python的多线程开销小，但却无法利用多核优势，也就是说python没用了，php才是最牛逼的语言？

别着急啊，老娘还没讲完呢。

要解决这个问题，我们需要在几个点上达成一致：
- 
cpu到底是用来做计算的，还是用来做I/O的？

- 
多cpu，意味着可以有多个核并行完成计算，所以多核提升的是计算性能

- 
每个cpu一旦遇到I/O阻塞，仍然需要等待，所以多核对I/O操作没什么用处


一个工人相当于cpu，此时计算相当于工人在干活，I/O阻塞相当于为工人干活提供所需原材料的过程，工人干活的过程中如果没有原材料了，则工人干活的过程需要停止，直到等待原材料的到来。

如果你的工厂干的大多数任务都要有准备原材料的过程（I/O密集型），那么你有再多的工人，意义也不大，还不如一个人，在等材料的过程中让工人去干别的活，

反过来讲，如果你的工厂原材料都齐全，那当然是工人越多，效率越高

结论：

　　对计算来说，cpu越多越好，但是对于I/O来说，再多的cpu也没用

　　当然对于一个程序来说，不会是纯计算或者纯I/O，我们只能相对的去看一个程序到底是计算密集型还是I/O密集型，从而进一步分析python的多线程有无用武之地

分析：

我们有四个任务需要处理，处理方式肯定是要玩出并发的效果，解决方案可以是：

方案一：开启四个进程

方案二：一个进程下，开启四个线程

单核情况下，分析结果:

　　如果四个任务是计算密集型，没有多核来并行计算，方案一徒增了创建进程的开销，方案二胜

　　如果四个任务是I/O密集型，方案一创建进程的开销大，且进程的切换速度远不如线程，方案二胜

多核情况下，分析结果：

　　如果四个任务是计算密集型，多核意味着并行计算，在python中一个进程中同一时刻只有一个线程执行用不上多核，方案一胜

　　如果四个任务是I/O密集型，再多的核也解决不了I/O问题，方案二胜

结论：现在的计算机基本上都是多核，python对于计算密集型的任务开多线程的效率并不能带来多大性能上的提升，甚至不如串行(没有大量切换)，但是，对于IO密集型的任务效率还是有显著提升的。

计算密集型
from threading import Thread
from multiprocessing import Process
import os
import time
def work():
res=0
for i in range(1000000):
res+=i

```
if __name__ == '__main__':
    t_l=[]
    start_time=time.time()
    # for i in range(300): #串行
    #     work()

    for i in range(300):
        t=Thread(target=work) #在我的机器上，4核cpu，多线程大概15秒
        # t=Process(target=work) #在我的机器上，4核cpu，多进程大概10秒
        t_l.append(t)
        t.start()

    for i in t_l:
        i.join()

    stop_time=time.time()
    print('run time is %s' %(stop_time-start_time))

    print('主线程')
```

I/O密集型
from threading import Thread
from multiprocessing import Process
import time
import os
def work():
time.sleep(2) #模拟I/O操作，可以打开一个文件来测试I/O,与sleep是一个效果
print(os.getpid())

```
if __name__ == '__main__':
    t_l=[]
    start_time=time.time()
    for i in range(1000):
        t=Thread(target=work) #耗时大概为2秒
        # t=Process(target=work) #耗时大概为25秒,创建进程的开销远高于线程，而且对于I/O密集型，多cpu根本不管用
        t_l.append(t)
        t.start()

    for t in t_l:
        t.join()
    stop_time=time.time()
    print('run time is %s' %(stop_time-start_time))
```

应用：

多线程用于IO密集型，如socket，爬虫，web
多进程用于计算密集型，如金融分析

### 2.3 同步锁

同进程一样

```python
import time
import threading

def addNum():
    global num #在每个线程中都获取这个全局变量
    #num-=1

    temp=num
    time.sleep(0.1)
    num =temp-1  # 对此公共变量进行-1操作

num = 100  #设定一个共享变量

thread_list = []

for i in range(100):
    t = threading.Thread(target=addNum)
    t.start()
    thread_list.append(t)

for t in thread_list: #等待所有线程执行完毕
    t.join()

print('Result: ', num)
```

锁通常被用来实现对共享资源的同步访问。为每一个共享资源创建一个Lock对象，当你需要访问该资源时，调用acquire方法来获取锁对象（如果其它线程已经获得了该锁，则当前线程需等待其被释放），待资源访问完后，再调用release方法释放锁：

```python
import threading

R=threading.Lock()

R.acquire()
'''
对公共数据的操作
'''
R.release()
```

GIL VS Lock
`机智的同学可能会问到这个问题，就是既然你之前说过了，Python已经有一个GIL来保证同一时间只能有一个线程来执行了，为什么这里还需要lock? `
　首先我们需要达成共识：锁的目的是为了保护共享的数据，同一时间只能有一个线程来修改共享的数据
`然后，我们可以得出结论：保护不同的数据就应该加不同的锁。`
　最后，问题就很明朗了，GIL 与Lock是两把锁，保护的数据不一样，前者是解释器级别的（当然保护的就是解释器级别的数据，比如垃圾回收的数据），后者是保护用户自己开发的应用程序的数据，很明显GIL不负责这件事，只能用户自定义加锁处理，即Lock
`详细的：`
因为Python解释器帮你自动定期进行内存回收，你可以理解为python解释器里有一个独立的线程，每过一段时间它起wake up做一次全局轮询看看哪些内存数据是可以被清空的，此时你自己的程序 里的线程和 py解释器自己的线程是并发运行的，假设你的线程删除了一个变量，py解释器的垃圾回收线程在清空这个变量的过程中的clearing时刻，可能一个其它线程正好又重新给这个还没来及得清空的内存空间赋值了，结果就有可能新赋值的数据被删除了，为了解决类似的问题，python解释器简单粗暴的加了锁，即当一个线程运行时，其它人都不能动，这样就解决了上述的问题， 这可以说是Python早期版本的遗留问题。　

### 2.4 死锁与递归锁

进程也有死锁与递归锁，在进程那里忘记说了，放到这里一切说了额

所谓死锁： 是指两个或两个以上的进程或线程在执行过程中，因争夺资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程称为死锁进程，如下就是死锁

```python
from threading import Thread,Lock
import time
mutexA=Lock()
mutexB=Lock()

class MyThread(Thread):
    def run(self):
        self.func1()
        self.func2()
    def func1(self):
        mutexA.acquire()
        print('\033[41m%s 拿到A锁\033[0m' %self.name)

        mutexB.acquire()
        print('\033[42m%s 拿到B锁\033[0m' %self.name)
        mutexB.release()

        mutexA.release()

    def func2(self):
        mutexB.acquire()
        print('\033[43m%s 拿到B锁\033[0m' %self.name)
        time.sleep(2)

        mutexA.acquire()
        print('\033[44m%s 拿到A锁\033[0m' %self.name)
        mutexA.release()

        mutexB.release()

if __name__ == '__main__':
    for i in range(10):
        t=MyThread()
        t.start()

'''
Thread-1 拿到A锁
Thread-1 拿到B锁
Thread-1 拿到B锁
Thread-2 拿到A锁
然后就卡住，死锁了
'''
```

解决方法，递归锁，在Python中为了支持在同一线程中多次请求同一资源，python提供了可重入锁RLock。

这个RLock内部维护着一个Lock和一个counter变量，counter记录了acquire的次数，从而使得资源可以被多次require。直到一个线程所有的acquire都被release，其他的线程才能获得资源。上面的例子如果使用RLock代替Lock，则不会发生死锁：

mutexA=mutexB=threading.RLock() #一个线程拿到锁，counter加1,该线程内又碰到加锁的情况，则counter继续加1，这期间所有其他线程都只能等待，等待该线程释放所有锁，即counter递减到0为止

### 2.5 信号量Semahpore

同进程的一样

Semaphore管理一个内置的计数器，
每当调用acquire()时内置计数器-1；
调用release() 时内置计数器+1；
计数器不能小于0；当计数器为0时，acquire()将阻塞线程直到其他线程调用release()。

实例：(同时只有5个线程可以获得semaphore,即可以限制最大连接数为5)：

```python
import threading
import time

semaphore = threading.Semaphore(5)

def func():
    if semaphore.acquire():
        print (threading.currentThread().getName() + ' get semaphore')
        time.sleep(2)
        semaphore.release()

for i in range(20):
  t1 = threading.Thread(target=func)
  t1.start()
```

与进程池是完全不同的概念，进程池Pool(4)，最大只能产生4个进程，而且从头到尾都只是这四个进程，不会产生新的，而信号量是产生一堆线程/进程
回到顶部

### 2.6 Event

同进程的一样

线程的一个关键特性是每个线程都是独立运行且状态不可预测。如果程序中的其 他线程需要通过判断某个线程的状态来确定自己下一步的操作,这时线程同步问题就 会变得非常棘手。为了解决这些问题,我们需要使用threading库中的Event对象。 对象包含一个可由线程设置的信号标志,它允许线程等待某些事件的发生。在 初始情况下,Event对象中的信号标志被设置为假。如果有线程等待一个Event对象, 而这个Event对象的标志为假,那么这个线程将会被一直阻塞直至该标志为真。一个线程如果将一个Event对象的信号标志设置为真,它将唤醒所有等待这个Event对象的线程。如果一个线程等待一个已经被设置为真的Event对象,那么它将忽略这个事件, 继续执行

```
event.isSet()：返回event的状态值；

event.wait()：如果 event.isSet()==False将阻塞线程；

event.set()： 设置event的状态值为True，所有阻塞池的线程激活进入就绪状态， 等待操作系统调度；

event.clear()：恢复event的状态值为False。
```

可以考虑一种应用场景（仅仅作为说明），例如，我们有多个线程从Redis队列中读取数据来处理，这些线程都要尝试去连接Redis的服务，一般情况下，如果Redis连接不成功，在各个线程的代码中，都会去尝试重新连接。如果我们想要在启动时确保Redis服务正常，才让那些工作线程去连接Redis服务器，那么我们就可以采用threading.Event机制来协调各个工作线程的连接操作：主线程中会去尝试连接Redis服务，如果正常的话，触发事件，各工作线程会尝试连接Redis服务。

```python
import threading
import time
import logging

logging.basicConfig(level=logging.DEBUG, format='(%(threadName)-10s) %(message)s',)

def worker(event):
    logging.debug('Waiting for redis ready...')
    event.wait()
    logging.debug('redis ready, and connect to redis server and do some work [%s]', time.ctime())
    time.sleep(1)

def main():
    readis_ready = threading.Event()
    t1 = threading.Thread(target=worker, args=(readis_ready,), name='t1')
    t1.start()

    t2 = threading.Thread(target=worker, args=(readis_ready,), name='t2')
    t2.start()

    logging.debug('first of all, check redis server, make sure it is OK, and then trigger the redis ready event')
    time.sleep(3) # simulate the check progress
    readis_ready.set()

if __name__=="__main__":
    main()
```

mysql...

```python
from threading import Thread,Event
import threading
import time,random
def conn_mysql():
    print('\033[42m%s 等待连接mysql。。。\033[0m' %threading.current_thread().getName())
    event.wait()
    print('\033[42mMysql初始化成功，%s开始连接。。。\033[0m' %threading.current_thread().getName())


def check_mysql():
    print('\033[41m正在检查mysql。。。\033[0m')
    time.sleep(random.randint(1,3))
    event.set()
    time.sleep(random.randint(1,3))

if __name__ == '__main__':
    event=Event()
    t1=Thread(target=conn_mysql) #等待连接mysql
    t2=Thread(target=conn_mysql) #等待连接myqsl
    t3=Thread(target=check_mysql) #检查mysql

    t1.start()
    t2.start()
    t3.start()
```

threading.Event的wait方法还接受一个超时参数，默认情况下如果事件一致没有发生，wait方法会一直阻塞下去，而加入这个超时参数之后，如果阻塞时间超过这个参数设定的值之后，wait方法会返回。对应于上面的应用场景，如果Redis服务器一致没有启动，我们希望子线程能够打印一些日志来不断地提醒我们当前没有一个可以连接的Redis服务，我们就可以通过设置这个超时参数来达成这样的目的：

```python
def conn_mysql():
    count=0
    while not e.is_set():
        print('%s 第 <%s> 次尝试' %(threading.current_thread().getName(),count))
        count+=1
        e.wait(0.5)
    print('%s ready to conn mysql' %threading.current_thread().getName())
    time.sleep(1)


from threading import Thread,Event
import threading
import time,random
def conn_mysql():
    while not event.is_set():
        print('\033[42m%s 等待连接mysql。。。\033[0m' %threading.current_thread().getName())
        event.wait(0.1)
    print('\033[42mMysql初始化成功，%s开始连接。。。\033[0m' %threading.current_thread().getName())


def check_mysql():
    print('\033[41m正在检查mysql。。。\033[0m')
    time.sleep(random.randint(1,3))
    event.set()
    time.sleep(random.randint(1,3))

if __name__ == '__main__':
    event=Event()
    t1=Thread(target=conn_mysql)
    t2=Thread(target=conn_mysql)
    t3=Thread(target=check_mysql)

    t1.start()
    t2.start()
    t3.start()
```

这样，我们就可以在等待Redis服务启动的同时，看到工作线程里正在等待的情况。

应用：连接池

### 2.7 条件Condition（了解）

使得线程等待，只有满足某条件时，才释放n个线程

```python
import threading
 
def run(n):
    con.acquire()
    con.wait()
    print("run the thread: %s" %n)
    con.release()
 
if __name__ == '__main__':
 
    con = threading.Condition()
    for i in range(10):
        t = threading.Thread(target=run, args=(i,))
        t.start()
 
    while True:
        inp = input('>>>')
        if inp == 'q':
            break
        con.acquire()
        con.notify(int(inp))
        con.release()


def condition_func():

    ret = False
    inp = input('>>>')
    if inp == '1':
        ret = True

    return ret


def run(n):
    con.acquire()
    con.wait_for(condition_func)
    print("run the thread: %s" %n)
    con.release()

if __name__ == '__main__':

    con = threading.Condition()
    for i in range(10):
        t = threading.Thread(target=run, args=(i,))
        t.start()
```

### 2.8 定时器

定时器，指定n秒后执行某操作

```python
from threading import Timer
 
 
def hello():
    print("hello, world")
 
t = Timer(1, hello)
t.start()  # after 1 seconds, "hello, world" will be printed
```

### 2.9 线程queue

queue队列 ：使用import queue，用法与进程Queue一样

queue is especially useful in threaded programming when information must be exchanged safely between multiple threads.

class queue.Queue(maxsize=0) #先进先出

```python
import queue

q=queue.Queue()
q.put('first')
q.put('second')
q.put('third')

print(q.get())
print(q.get())
print(q.get())
'''
结果(先进先出):
first
second
third
'''
```

class queue.LifoQueue(maxsize=0) #last in fisrt out

```python
import queue

q=queue.LifoQueue()
q.put('first')
q.put('second')
q.put('third')

print(q.get())
print(q.get())
print(q.get())
'''
结果(后进先出):
third
second
first
'''
```

class queue.PriorityQueue(maxsize=0) #存储数据时可设置优先级的队列

```
import queue

q=queue.PriorityQueue()
#put进入一个元组,元组的第一个元素是优先级(通常是数字,也可以是非数字之间的比较),数字越小优先级越高
q.put((20,'a'))
q.put((10,'b'))
q.put((30,'c'))

print(q.get())
print(q.get())
print(q.get())
'''
结果(数字越小优先级越高,优先级高的优先出队):
(10, 'b')
(20, 'a')
(30, 'c')
'''



Constructor for a priority queue. maxsize is an integer that sets the upperbound limit on the number of items that can be placed in the queue. Insertion will block once this size has been reached, until queue items are consumed. If maxsize is less than or equal to zero, the queue size is infinite.

The lowest valued entries are retrieved first (the lowest valued entry is the one returned by sorted(list(entries))[0]). A typical pattern for entries is a tuple in the form: (priority_number, data).
```

exception queue.Empty
`Exception raised when non-blocking get() (or get_nowait()) is called on a Queue object which is empty.`
exception queue.Full
`Exception raised when non-blocking put() (or put_nowait()) is called on a Queue object which is full.`
Queue.qsize()

Queue.empty() #return True if empty

Queue.full() # return True if full

Queue.put(item, block=True, timeout=None)
`Put item into the queue. If optional args block is true and timeout is None (the default), block if necessary until a free slot is available. If timeout is a positive number, it blocks at most timeout seconds and raises the Full exception if no free slot was available within that time. Otherwise (block is false), put an item on the queue if a free slot is immediately available, else raise the Full exception (timeout is ignored in that case).`
Queue.put_nowait(item)
`Equivalent to put(item, False).`
Queue.get(block=True, timeout=None)
`Remove and return an item from the queue. If optional args block is true and timeout is None (the default), block if necessary until an item is available. If timeout is a positive number, it blocks at most timeout seconds and raises the Empty exception if no item was available within that time. Otherwise (block is false), return an item if one is immediately available, else raise the Empty exception (timeout is ignored in that case).`
Queue.get_nowait()
`Equivalent to get(False).`
Two methods are offered to support tracking whether enqueued tasks have been fully processed by daemon consumer threads.

Queue.task_done()

```
Indicate that a formerly enqueued task is complete. Used by queue consumer threads. For each get() used to fetch a task, a subsequent call to task_done() tells the queue that the processing on the task is complete.

If a join() is currently blocking, it will resume when all items have been processed (meaning that a task_done() call was received for every item that had been put() into the queue).

Raises a ValueError if called more times than there were items placed in the queue.
```

Queue.join() block直到queue被消费完毕

### 2.10 Python标准模块--concurrent.futures

[https://docs.python.org/dev/library/concurrent.futures.html](https://docs.python.org/dev/library/concurrent.futures.html)

## 三 补充：paramiko模块
- 介绍：

paramiko是一个用于做远程控制的模块，使用该模块可以对远程服务器进行命令或文件操作，值得一说的是，fabric和ansible内部的远程管理就是使用的paramiko来现实。
- 下载安装

pycrypto，由于 paramiko 模块内部依赖pycrypto，所以先下载安装pycrypto
pip3 install pycrypto
pip3 install paramiko

注：如果在安装pycrypto2.0.1时发生如下错误
command 'gcc' failed with exit status 1...
可能是缺少python-dev安装包导致
如果gcc没有安装，请事先安装gcc
- 使用

SSHClient

用于连接远程服务器并执行基本命令

基于用户名密码连接：

```
import paramiko
  
# 创建SSH对象
ssh = paramiko.SSHClient()
# 允许连接不在know_hosts文件中的主机
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
# 连接服务器
ssh.connect(hostname='c1.salt.com', port=22, username='wupeiqi', password='123')
  
# 执行命令
stdin, stdout, stderr = ssh.exec_command('df')
# 获取命令结果
result = stdout.read()
  
# 关闭连接
ssh.close()


import paramiko

transport = paramiko.Transport(('hostname', 22))
transport.connect(username='wupeiqi', password='123')

ssh = paramiko.SSHClient()
ssh._transport = transport

stdin, stdout, stderr = ssh.exec_command('df')
print stdout.read()

transport.close()
```

基于公钥密钥连接：

客户端文件名：id_rsa

服务端必须有文件名：authorized_keys

```python
import paramiko
 
private_key = paramiko.RSAKey.from_private_key_file('/home/auto/.ssh/id_rsa')
 
# 创建SSH对象
ssh = paramiko.SSHClient()
# 允许连接不在know_hosts文件中的主机
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
# 连接服务器
ssh.connect(hostname='c1.salt.com', port=22, username='wupeiqi', key=private_key)
 
# 执行命令
stdin, stdout, stderr = ssh.exec_command('df')
# 获取命令结果
result = stdout.read()
 
# 关闭连接
ssh.close()


import paramiko

private_key = paramiko.RSAKey.from_private_key_file('/home/auto/.ssh/id_rsa')

transport = paramiko.Transport(('hostname', 22))
transport.connect(username='wupeiqi', pkey=private_key)

ssh = paramiko.SSHClient()
ssh._transport = transport

stdin, stdout, stderr = ssh.exec_command('df')

transport.close()


import paramiko
from io import StringIO

key_str = """-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAq7gLsqYArAFco02/55IgNg0r7NXOtEM3qXpb/dabJ5Uyky/8
NEHhFiQ7deHIRIuTW5Zb0kD6h6EBbVlUMBmwJrC2oSzySLU1w+ZNfH0PE6W6fans
H80whhuc/YgP+fjiO+VR/gFcqib8Rll5UfYzf5H8uuOnDeIXGCVgyHQSmt8if1+e
7hn1MVO1Lrm9Fco8ABI7dyv8/ZEwoSfh2C9rGYgA58LT1FkBRkOePbHD43xNfAYC
tfLvz6LErMnwdOW4sNMEWWAWv1fsTB35PAm5CazfKzmam9n5IQXhmUNcNvmaZtvP
c4f4g59mdsaWNtNaY96UjOfx83Om86gmdkKcnwIDAQABAoIBAQCnDBGFJuv8aA7A
ZkBLe+GN815JtOyye7lIS1n2I7En3oImoUWNaJEYwwJ8+LmjxMwDCtAkR0XwbvY+
c+nsKPEtkjb3sAu6I148RmwWsGncSRqUaJrljOypaW9dS+GO4Ujjz3/lw1lrxSUh
IqVc0E7kyRW8kP3QCaNBwArYteHreZFFp6XmtKMtXaEA3saJYILxaaXlYkoRi4k8
S2/K8aw3ZMR4tDCOfB4o47JaeiA/e185RK3A+mLn9xTDhTdZqTQpv17/YRPcgmwz
zu30fhVXQT/SuI0sO+bzCO4YGoEwoBX718AWhdLJFoFq1B7k2ZEzXTAtjEXQEWm6
01ndU/jhAasdfasdasdfasdfa3eraszxqwefasdfadasdffsFIfAsjQb4HdkmHuC
OeJrJOd+CYvdEeqJJNnF6AbHyYHIECkj0Qq1kEfLOEsqzd5nDbtkKBte6M1trbjl
HtJ2Yb8w6o/q/6Sbj7wf/cW3LIYEdeVCjScozVcQ9R83ea05J+QOAr4nAoGBAMaq
UzLJfLNWZ5Qosmir2oHStFlZpxspax/ln7DlWLW4wPB4YJalSVovF2Buo8hr8X65
lnPiE41M+G0Z7icEXiFyDBFDCtzx0x/RmaBokLathrFtI81UCx4gQPLaSVNMlvQA
539GsubSrO4LpHRNGg/weZ6EqQOXvHvkUkm2bDDJAoGATytFNxen6GtC0ZT3SRQM
WYfasdf3xbtuykmnluiofasd2sfmjnljkt7khghmghdasSDFGQfgaFoKfaawoYeH
C2XasVUsVviBn8kPSLSVBPX4JUfQmA6h8HsajeVahxN1U9e0nYJ0sYDQFUMTS2t8
RT57+WK/0ONwTWHdu+KnaJECgYEAid/ta8LQC3p82iNAZkpWlGDSD2yb/8rH8NQg
9tjEryFwrbMtfX9qn+8srx06B796U3OjifstjJQNmVI0qNlsJpQK8fPwVxRxbJS/
pMbNICrf3sUa4sZgDOFfkeuSlgACh4cVIozDXlR59Z8Y3CoiW0uObEgvMDIfenAj
98pl3ZkCgYEAj/UCSni0dwX4pnKNPm6LUgiS7QvIgM3H9piyt8aipQuzBi5LUKWw
DlQC4Zb73nHgdREtQYYXTu7p27Bl0Gizz1sW2eSgxFU8eTh+ucfVwOXKAXKU5SeI
+MbuBfUYQ4if2N/BXn47+/ecf3A4KgB37Le5SbLDddwCNxGlBzbpBa0=
-----END RSA PRIVATE KEY-----"""

private_key = paramiko.RSAKey(file_obj=StringIO(key_str))
transport = paramiko.Transport(('10.0.1.40', 22))
transport.connect(username='wupeiqi', pkey=private_key)

ssh = paramiko.SSHClient()
ssh._transport = transport

stdin, stdout, stderr = ssh.exec_command('df')
result = stdout.read()

transport.close()

print(result)
```

SFTPClient

用于连接远程服务器并执行上传下载

基于用户名密码上传下载

```
import paramiko
 
transport = paramiko.Transport(('hostname',22))
transport.connect(username='wupeiqi',password='123')
 
sftp = paramiko.SFTPClient.from_transport(transport)
# 将location.py 上传至服务器 /tmp/test.py
sftp.put('/tmp/location.py', '/tmp/test.py')
# 将remove_path 下载到本地 local_path
sftp.get('remove_path', 'local_path')
 
transport.close()
```

基于公钥密钥上传下载

```python
import paramiko
 
private_key = paramiko.RSAKey.from_private_key_file('/home/auto/.ssh/id_rsa')
 
transport = paramiko.Transport(('hostname', 22))
transport.connect(username='wupeiqi', pkey=private_key )
 
sftp = paramiko.SFTPClient.from_transport(transport)
# 将location.py 上传至服务器 /tmp/test.py
sftp.put('/tmp/location.py', '/tmp/test.py')
# 将remove_path 下载到本地 local_path
sftp.get('remove_path', 'local_path')
 
transport.close()


#!/usr/bin/env python
# -*- coding:utf-8 -*-
import paramiko
import uuid

class Haproxy(object):

    def __init__(self):
        self.host = '172.16.103.191'
        self.port = 22
        self.username = 'wupeiqi'
        self.pwd = '123'
        self.__k = None

    def create_file(self):
        file_name = str(uuid.uuid4())
        with open(file_name,'w') as f:
            f.write('sb')
        return file_name

    def run(self):
        self.connect()
        self.upload()
        self.rename()
        self.close()

    def connect(self):
        transport = paramiko.Transport((self.host,self.port))
        transport.connect(username=self.username,password=self.pwd)
        self.__transport = transport

    def close(self):

        self.__transport.close()

    def upload(self):
        # 连接，上传
        file_name = self.create_file()

        sftp = paramiko.SFTPClient.from_transport(self.__transport)
        # 将location.py 上传至服务器 /tmp/test.py
        sftp.put(file_name, '/home/wupeiqi/tttttttttttt.py')

    def rename(self):

        ssh = paramiko.SSHClient()
        ssh._transport = self.__transport
        # 执行命令
        stdin, stdout, stderr = ssh.exec_command('mv /home/wupeiqi/tttttttttttt.py /home/wupeiqi/ooooooooo.py')
        # 获取命令结果
        result = stdout.read()


ha = Haproxy()
ha.run()
```

## 四 协程

协程：是单线程下的并发，又称微线程，纤程。英文名Coroutine。一句话说明什么是线程：协程是一种用户态的轻量级线程，即协程是由用户程序自己控制调度的。

需要强调的是：

　　1. python的线程属于内核级别的，即由操作系统控制调度（如单线程一旦遇到io就被迫交出cpu执行权限，切换其他线程运行）

　　2. 单线程内开启协程，一旦遇到io，从应用程序级别（而非操作系统）控制切换

对比操作系统控制线程的切换，用户在单线程内控制协程的切换，优点如下：

　　1. 协程的切换开销更小，属于程序级别的切换，操作系统完全感知不到，因而更加轻量级

　　2. 单线程内就可以实现并发的效果，最大限度地利用cpu

要实现协程，关键在于用户程序自己控制程序切换，切换之前必须由用户程序自己保存协程上一次调用时的状态，如此，每次重新调用时，能够从上次的位置继续执行

（详细的：协程拥有自己的寄存器上下文和栈。协程调度切换时，将寄存器上下文和栈保存到其他地方，在切回来的时候，恢复先前保存的寄存器上下文和栈）

为此，我们之前已经学习过一种在单线程下可以保存程序运行状态的方法，即yield，我们来简单复习一下：

```python
yiled可以保存状态，yield的状态保存与操作系统的保存线程状态很像，但是yield是代码级别控制的，更轻量级
send可以把一个函数的结果传给另外一个函数，以此实现单线程内程序之间的切换
```

不用yield：每次函数调用,都需要重复开辟内存空间，即重复创建名称空间,因而开销很大
import time
def consumer(item):
# print('拿到包子%s' %item)
x=11111111111
x1=12111111111
x3=13111111111
x4=14111111111
y=22222222222
z=33333333333

```python
pass
def producer(target,seq):
    for item in seq:
        target(item) #每次调用函数,会临时产生名称空间,调用结束则释放,循环100000000次,则重复这么多次的创建和释放,开销非常大

start_time=time.time()
producer(consumer,range(100000000))
stop_time=time.time()
print('run time is:%s' %(stop_time-start_time)) #30.132838010787964


#使用yield：无需重复开辟内存空间，即重复创建名称空间,因而开销小
import time
def init(func):
    def wrapper(*args,**kwargs):
        g=func(*args,**kwargs)
        next(g)
        return g
    return wrapper

@init
def consumer():
    x=11111111111
    x1=12111111111
    x3=13111111111
    x4=14111111111
    y=22222222222
    z=33333333333
    while True:
        item=yield
        # print('拿到包子%s' %item)
        pass
def producer(target,seq):
    for item in seq:
        target.send(item) #无需重新创建名称空间,从上一次暂停的位置继续,相比上例,开销小

start_time=time.time()
producer(consumer(),range(100000000))
stop_time=time.time()
print('run time is:%s' %(stop_time-start_time)) #21.882073879241943
```

缺点：

协程的本质是单线程下，无法利用多核，可以是一个程序开启多个进程，每个进程内开启多个线程，每个线程内开启协程

协程指的是单个线程，因而一旦协程出现阻塞，将会阻塞整个线程

协程的定义（满足1，2，3就可称为协程）：

```
必须在只有一个单线程里实现并发
修改共享数据不需加锁
用户程序里自己保存多个控制流的上下文栈
附加：一个协程遇到IO操作自动切换到其它协程（如何实现检测IO，yield、greenlet都无法实现，就用到了gevent模块（select机制））
```

yield切换在没有io的情况下或者没有重复开辟内存空间的操作，对效率没有什么提升，甚至更慢，为此，可以用greenlet来为大家演示这种切换
回到顶部

## 五 Greenlet

greenlet是一个用C实现的协程模块，相比与python自带的yield，它可以使你在任意函数之间随意切换，而不需把这个函数先声明为generator

```python
from greenlet import greenlet

def test1():
    print('test1,first')
    gr2.switch()
    print('test1,sencod')
    gr2.switch()
def test2():
    print('test2,first')
    gr1.switch()
    print('test2,sencod')


gr1=greenlet(test1)
gr2=greenlet(test2)
gr1.switch()


import time
from greenlet import greenlet
def eat(name):
    print('%s eat food 1' %name)
    gr2.switch('alex飞飞飞')
    print('%s eat food 2' %name)
    gr2.switch()
def play_phone(name):
    print('%s play 1' %name)
    gr1.switch()
    print('%s play 2' %name)

gr1=greenlet(eat)
gr2=greenlet(play_phone)
gr1.switch(name='egon啦啦啦')#可以在第一次switch时传入参数，以后都不需要
```

单纯的切换（在没有io的情况下或者没有重复开辟内存空间的操作），反而会降低程序的执行速度

顺序执行
import time
def f1():
res=0
for i in range(10000000):
res+=i

```python
def f2():
    res=0
    for i in range(10000000):
        res*=i


start_time=time.time()
f1()
f2()
stop_time=time.time()
print('run time is: %s' %(stop_time-start_time)) #1.7395639419555664
```

切换
from greenlet import greenlet
import time
def f1():
res=0
for i in range(10000000):
res+=i
gr2.switch()

```python
def f2():
    res=0
    for i in range(10000000):
        res*=i
        gr1.switch()

gr1=greenlet(f1)
gr2=greenlet(f2)

start_time=time.time()
gr1.switch()
stop_time=time.time()
print('run time is: %s' %(stop_time-start_time)) #7.789067983627319
```

greenlet只是提供了一种比generator更加便捷的切换方式，仍然是没有解决遇到IO自动切换的问题
回到顶部

## 六 Gevent

Gevent 是一个第三方库，可以轻松通过gevent实现并发同步或异步编程，在gevent中用到的主要模式是Greenlet, 它是以C扩展模块形式接入Python的轻量级协程。 Greenlet全部运行在主程序操作系统进程的内部，但它们被协作式地调度。

g1=gevent.spawn()创建一个协程对象g1，

spawn括号内第一个参数是函数名，如eat，后面可以有多个参数，可以是位置实参或关键字实参，都是传给函数eat的

遇到IO阻塞时会自动切换任务

```python
import gevent
import time


def eat():
    print('eat food 1')
    gevent.sleep(2) #等饭来
    print('eat food 2')

def play_phone():
    print('play phone 1')
    gevent.sleep(1) #网卡了
    print('play phone 2')



# gevent.spawn(eat)
# gevent.spawn(play_phone)
# print('主') # 直接结束


#因而也需要join方法,进程或现场的jion方法只能join一个,而gevent的join方法可以join多个

g1=gevent.spawn(eat)
g2=gevent.spawn(play_phone)
gevent.joinall([g1,g2])
print('主')
```

上例gevent.sleep(2)模拟的是gevent可以识别的io阻塞,

而time.sleep(2)或其他的阻塞,gevent是不能直接识别的需要用下面一行代码,打补丁,就可以识别了

from gevent import monkey;monkey.patch_all()必须放到被打补丁者的前面，如time，socket模块之前

或者我们干脆记忆成：要用gevent，需要将from gevent import monkey;monkey.patch_all()放到文件的开头

```python
from gevent import monkey;monkey.patch_all()

import gevent
import time


def eat():
    print('eat food 1')
    time.sleep(2)
    print('eat food 2')

def play_phone():
    print('play phone 1')
    time.sleep(1)
    print('play phone 2')



g1=gevent.spawn(eat)
g2=gevent.spawn(play_phone)
gevent.joinall([g1,g2])
print('主')
```

同步与异步

```python
import gevent
 
def task(pid):
    """
    Some non-deterministic task
    """
    gevent.sleep(0.5)
    print('Task %s done' % pid)
 
def synchronous():
    for i in range(1,10):
        task(i)
 
def asynchronous():
    threads = [gevent.spawn(task, i) for i in range(10)]
    gevent.joinall(threads)
 
print('Synchronous:')
synchronous()
 
print('Asynchronous:')
asynchronous()
```

上面程序的重要部分是将task函数封装到Greenlet内部线程的gevent.spawn。 初始化的greenlet列表存放在数组threads中，此数组被传给gevent.joinall 函数，后者阻塞当前流程，并执行所有给定的greenlet。执行流程只会在 所有greenlet执行完后才会继续向下走。

gevent线程的一些用法
g1=gevent.spawn(func,1,,2,3,x=4,y=5)

```
g2=gevent.spawn(func2)

g1.join() #等待g1结束

g2.join() #等待g2结束
```

或者上述两步合作一步：gevent.joinall([g1,g2])

```python
g1.value#拿到func1的返回值


from gevent import monkey;monkey.patch_all()
import gevent
import requests
import time

def get_page(url):
    print('GET: %s' %url)
    response=requests.get(url)
    if response.status_code == 200:
        print('%d bytes received from %s' %(len(response.text),url))


start_time=time.time()
gevent.joinall([
    gevent.spawn(get_page,'https://www.python.org/'),
    gevent.spawn(get_page,'https://www.yahoo.com/'),
    gevent.spawn(get_page,'https://github.com/'),
])
stop_time=time.time()
print('run time is %s' %(stop_time-start_time))
```

通过gevent实现单线程下的socket并发（from gevent import monkey;monkey.patch_all()一定要放到导入socket模块之前，否则gevent无法识别socket的阻塞）

```python
from gevent import monkey;monkey.patch_all()
from socket import *
import gevent

#如果不想用money.patch_all()打补丁,可以用gevent自带的socket
# from gevent import socket
# s=socket.socket()

def server(server_ip,port):
    s=socket(AF_INET,SOCK_STREAM)
    s.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
    s.bind((server_ip,port))
    s.listen(5)
    while True:
        conn,addr=s.accept()
        gevent.spawn(talk,conn,addr)

def talk(conn,addr):
    try:
        while True:
            res=conn.recv(1024)
            print('client %s:%s msg: %s' %(addr[0],addr[1],res))
            conn.send(res.upper())
    except Exception as e:
        print(e)
    finally:
        conn.close()

if __name__ == '__main__':
    server('127.0.0.1',8080)


#_*_coding:utf-8_*_
__author__ = 'Linhaifeng'

from socket import *

client=socket(AF_INET,SOCK_STREAM)
client.connect(('127.0.0.1',8080))


while True:
    msg=input('>>: ').strip()
    if not msg:continue

    client.send(msg.encode('utf-8'))
    msg=client.recv(1024)
    print(msg.decode('utf-8'))


from threading import Thread
from socket import *
import threading

def client(server_ip,port):
    c=socket(AF_INET,SOCK_STREAM)
    c.connect((server_ip,port))

    count=0
    while True:
        c.send(('%s say hello %s' %(threading.current_thread().getName(),count)).encode('utf-8'))
        msg=c.recv(1024)
        print(msg.decode('utf-8'))
        count+=1
if __name__ == '__main__':
    for i in range(500):
        t=Thread(target=client,args=('127.0.0.1',8080))
        t.start()
```












