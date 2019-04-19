# python 多线程编程 - 宇宙浪子的专栏 - CSDN博客
2014年09月17日 00:24:35[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：265标签：[Python																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
一）线程基础
1、创建线程：
thread模块提供了start_new_thread函数，用以创建线程。start_new_thread函数成功创建后还可以对其进行操作。
其函数原型：
    start_new_thread(function,atgs[,kwargs])
其参数含义如下：
    function: 在线程中执行的函数名
    args:元组形式的参数列表。 
    kwargs: 可选参数，以字典的形式指定参数
方法一：通过使用thread模块中的函数创建新线程。
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- >>> import thread  
- >>> def run(n):  
- for i in range(n):  
- print i  
- 
- 
- >>> thread.start_new_thread(run,(4,))   #注意第二个参数一定要是元组的形式
- 53840
- 
- 
- 1
- >>>   
- 2
- 3
- KeyboardInterrupt  
- >>> thread.start_new_thread(run,(2,))  
- 17840
- 
- 
- 1
- >>>   
- thread.start_new_thread(run,(),{'n':4})  
- 39720
- 
- 
- 1
- >>>   
- 2
- 3
- thread.start_new_thread(run,(),{'n':3})  
- 32480
- 
- 
- 1
- >>>   
- 2
>>> import thread
>>> def run(n):
	for i in range(n):
		print i
		
>>> thread.start_new_thread(run,(4,))   #注意第二个参数一定要是元组的形式
53840
1
>>> 
2
3
KeyboardInterrupt
>>> thread.start_new_thread(run,(2,))
17840
1
>>> 
thread.start_new_thread(run,(),{'n':4})
39720
1
>>> 
2
3
thread.start_new_thread(run,(),{'n':3})
32480
1
>>> 
2
方法二：通过继承threading.Thread创建线程
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- >>> import threading  
- >>> class mythread(threading.Thread):  
- def __init__(self,num):  
-         threading.Thread.__init__(self)  
- self.num = num  
- def run(self):               #重载run方法
- print'I am', self.num  
- 
- 
- >>> t1 = mythread(1)  
- >>> t2 = mythread(2)  
- >>> t3 = mythread(3)  
- >>> t1.start()           #运行线程t1
- I am  
- >>>  1
- t2.start()  
- I am  
- >>>  2
- t3.start()  
- I am  
- >>>  3
>>> import threading
>>> class mythread(threading.Thread):
	def __init__(self,num):
		threading.Thread.__init__(self)
		self.num = num
	def run(self):               #重载run方法
		print 'I am', self.num
		
>>> t1 = mythread(1)
>>> t2 = mythread(2)
>>> t3 = mythread(3)
>>> t1.start()           #运行线程t1
I am
>>>  1
t2.start()
I am
>>>  2
t3.start()
I am
>>>  3
方法三：使用threading.Thread直接在线程中运行函数。
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- import threading  
- >>> def run(x,y):  
- for i in range(x,y):  
- print i  
- 
- >>> t1 = threading.Thread(target=run,args=(15,20)) #直接使用Thread附加函数args为函数参数
- 
- >>> t1.start()  
- 15
- >>>   
- 16
- 17
- 18
- 19
import threading
>>> def run(x,y):
	for i in range(x,y):
		print i
>>> t1 = threading.Thread(target=run,args=(15,20)) #直接使用Thread附加函数args为函数参数
>>> t1.start()
15
>>> 
16
17
18
19
二)Thread对象中的常用方法：
1、isAlive方法：
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- >>> import threading  
- >>> import time  
- >>> class mythread(threading.Thread):  
- def __init__(self,id):  
-         threading.Thread.__init__(self)  
- self.id = id  
- def run(self):  
-         time.sleep(5)    #休眠5秒
- printself.id  
- 
- 
- >>> t = mythread(1)  
- >>> def func():  
-     t.start()  
- print t.isAlive()    #打印线程状态
- 
- 
- >>> func()  
- True
- >>> 1
>>> import threading
>>> import time
>>> class mythread(threading.Thread):
	def __init__(self,id):
		threading.Thread.__init__(self)
		self.id = id
	def run(self):
		time.sleep(5)    #休眠5秒
		print self.id
		
>>> t = mythread(1)
>>> def func():
	t.start()
	print t.isAlive()    #打印线程状态
	
>>> func()
True
>>> 1
2、join方法：
原型：join([timeout]) 
    timeout: 可选参数，线程运行的最长时间
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- import threading  
- >>> import time     #导入time模块
- >>> class Mythread(threading.Thread):  
- def __init__(self,id):  
-         threading.Thread.__init__(self)  
- self.id = id  
- def run(self):  
-         x = 0
-         time.sleep(20)  
- printself.id  
- 
- 
- >>> def func():  
-     t.start()  
- for i in range(5):  
- print i  
- 
- 
- >>> t = Mythread(2)  
- >>> func()  
- 0
- 1
- 2
- 3
- 4
- >>> 2
- def func():  
-     t.start()  
-     t.join()  
- for i in range(5):  
- print i  
- 
- 
- >>> t = Mythread(3)  
- >>> func()  
- 3
- 0
- 1
- 2
- 3
- 4
- >>>   
import threading
>>> import time     #导入time模块
>>> class Mythread(threading.Thread):
	def __init__(self,id):
		threading.Thread.__init__(self)
		self.id = id
	def run(self):
		x = 0
		time.sleep(20)
		print self.id
		
>>> def func():
	t.start()
	for i in range(5):
		print i
		
>>> t = Mythread(2)
>>> func()
0
1
2
3
4
>>> 2
def func():
	t.start()
	t.join()
	for i in range(5):
		print i
		
>>> t = Mythread(3)
>>> func()
3
0
1
2
3
4
>>> 
3、线程名：
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- >>> import threading  
- >>> class mythread(threading.Thread):  
- def __init__(self,threadname):  
-         threading.Thread.__init__(self,name=threadname)  
- def run(self):  
- printself.getName()  
- 
- 
- >>>   
- >>> t1 = mythread('t1')  
- >>> t1.start()  
- t1  
- >>>   
>>> import threading
>>> class mythread(threading.Thread):
	def __init__(self,threadname):
		threading.Thread.__init__(self,name=threadname)
	def run(self):
		print self.getName()
		
>>> 
>>> t1 = mythread('t1')
>>> t1.start()
t1
>>> 
 4、setDaemon方法
在脚本运行的过程中有一个主线程，如果主线程又创建了一个子线程，那么当主线程退出时，会检验子线程是否完成。如果子线程未完成，则主线程会在等待子线程完成后退出。
当需要主线程退出时，不管子线程是否完成都随主线程退出，则可以使用Thread对象的setDaemon方法来设置。
三）线程同步
1.简单的线程同步
使用Thread对象的Lock和RLock可以实现简单的线程同步。对于如果需要每次只有一个线程操作的数据，可以将操作过程放在acquire方法和release方法之间。如：
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- # -*- coding:utf-8 -*-
- import threading  
- import time  
- class mythread(threading.Thread):  
- def __init__(self,threadname):  
-         threading.Thread.__init__(self,name = threadname)  
- def run(self):  
- global x                #设置全局变量
- #       lock.acquire()          #调用lock的acquire方法
- for i in range(3):  
-             x = x + 1
-         time.sleep(2)  
- print x  
- #       lock.release()          #调用lock的release方法
- #lock = threading.RLock()        #生成Rlock对象
- t1 = []  
- for i in range(10):  
-     t = mythread(str(i))  
-     t1.append(t)  
- x = 0#将全局变量的值设为0
- for i in t1:   
-     i.start()  
- 
- E:/study/python/workspace>xianchengtongbu.py  
- 3
- 6
- 9
- 12
- 15
- 18
- 21
- 24
- 27
- 30
# -*- coding:utf-8 -*-
import threading
import time
class mythread(threading.Thread):
	def __init__(self,threadname):
		threading.Thread.__init__(self,name = threadname)
	def run(self):
		global x                #设置全局变量
#		lock.acquire()          #调用lock的acquire方法
		for i in range(3):
			x = x + 1
		time.sleep(2)
		print x
#		lock.release()          #调用lock的release方法
#lock = threading.RLock()        #生成Rlock对象
t1 = []
for i in range(10):
	t = mythread(str(i))
	t1.append(t)
x = 0                   #将全局变量的值设为0
for i in t1: 
	i.start()
E:/study/python/workspace>xianchengtongbu.py
3
6
9
12
15
18
21
24
27
30
如果将lock.acquire()和lock.release(),lock = threading.Lock()删除后保存运行脚本，结果将是输出10个30。30是x的最终值，由于x是全局变量，每个线程对其操作后进入休眠状态，在线程休眠的时候，python解释器就执行了其他的线程而是x的值增加。当所有线程休眠结束后，x的值已被所有线修改为了30，因此输出全部为30。
2、使用条件变量保持线程同步。
Python的Condition对象提供了对复制线程同步的支持。使用Condition对象可以在某些事件触发后才处理数据。Condition对象除了具有acquire方法和release的方法外，还有wait方法、notify方法、notifyAll方法等用于条件处理。
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- # -*- coding:utf-8 -*-
- import threading  
- class Producer(threading.Thread):  
- def __init__(self,threadname):  
-         threading.Thread.__init__(self,name = threadname)  
- def run(self):  
- global x  
-         con.acquire()  
- if x == 1000000:  
-             con.wait()  
- #   pass
- else:  
- for i in range(1000000):  
-                 x = x + 1
-             con.notify()  
- print x  
-         con.release()  
- class Consumer(threading.Thread):  
- def __init__(self,threadname):  
-         threading.Thread.__init__(self,name = threadname)  
- def run(self):  
- global x   
-         con.acquire()  
- if x == 0:  
-             con.wait()  
- #pass
- else:  
- for i in range(1000000):  
-                 x = x - 1
-             con.notify()  
- print x   
-         con.release()  
- con = threading.Condition()  
- x = 0
- p = Producer('Producer')  
- c = Consumer('Consumer')  
- p.start()  
- c.start()  
- p.join()  
- c.join()  
- print x  
- 
- E:/study/python/workspace>xianchengtongbu2.py  
- 1000000
- 0
- 0
# -*- coding:utf-8 -*-
import threading
class Producer(threading.Thread):
	def __init__(self,threadname):
		threading.Thread.__init__(self,name = threadname)
	def run(self):
		global x
		con.acquire()
		if x == 1000000:
			con.wait()
		#	pass
		else:
			for i in range(1000000):
				x = x + 1
			con.notify()
		print x
		con.release()
class Consumer(threading.Thread):
	def __init__(self,threadname):
		threading.Thread.__init__(self,name = threadname)
	def run(self):
		global x 
		con.acquire()
		if x == 0:
			con.wait()
			#pass
		else:
			for i in range(1000000):
				x = x - 1
			con.notify()
		print x 
		con.release()
con = threading.Condition()
x = 0
p = Producer('Producer')
c = Consumer('Consumer')
p.start()
c.start()
p.join()
c.join()
print x
E:/study/python/workspace>xianchengtongbu2.py
1000000
0
0
线程间通信：
Event对象用于线程间的相互通信。他提供了设置信号、清除信宏、等待等用于实现线程间的通信。
1、设置信号。Event对象使用了set()方法后，isSet()方法返回真。
2、清除信号。使用Event对象的clear（）方法后，isSet（）方法返回为假。
3、等待。当Event对象的内部信号标志为假时，则wait()方法一直等到其为真时才返回。还可以向wait传递参数，设定最长的等待时间。
**[python]**[view plain](http://blog.csdn.net/guopengzhang/article/details/5458091#)[copy](http://blog.csdn.net/guopengzhang/article/details/5458091#)[print](http://blog.csdn.net/guopengzhang/article/details/5458091#)[?](http://blog.csdn.net/guopengzhang/article/details/5458091#)
- # -*- coding:utf-8 -*-
- import threading  
- class mythread(threading.Thread):  
- def __init__(self,threadname):  
-         threading.Thread.__init__(self,name = threadname)  
- def run(self):  
- global event  
- if event.isSet():  
-             event.clear()  
-             event.wait()   #当event被标记时才返回
- printself.getName()  
- else:  
- printself.getName()  
-             event.set()  
- event = threading.Event()  
- event.set()  
- t1 = []  
- for i in range(10):  
-     t = mythread(str(i))  
-     t1.append(t)  
- for i in t1:  
-     i.start()  
本文转自：http://blog.csdn.net/guopengzhang/article/details/5458091
