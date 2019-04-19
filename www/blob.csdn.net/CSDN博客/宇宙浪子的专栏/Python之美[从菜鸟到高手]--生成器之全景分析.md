# Python之美[从菜鸟到高手]--生成器之全景分析 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 15:47:51[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：361标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)

yield指令，可以暂停一个函数并返回中间结果。使用该指令的函数将保存执行环境，并且在必要时恢复。
生成器比迭代器更加强大也更加复杂，需要花点功夫好好理解贯通。
看下面一段代码：
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- def gen():  
- for x in xrange(4):  
-         tmp = yield x  
- if tmp == 'hello':  
- print'world'
- else:  
- print str(tmp)  
     只要函数中包含yield关键字，该函数调用就是生成器对象。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- g=gen()  
- print g   #<generator object gen at 0x02801760>
- print isinstance(g,types.GeneratorType) #True
    我们可以看到，gen()并不是函数调用，而是产生生成器对象。
   生成器对象支持几个方法，如gen.next() ,gen.send() ,gen.throw()等。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- print g.next() # 0
    调用生成器的next方法，将运行到yield位置，此时暂停执行环境，并返回yield后的值。所以打印出的是1，暂停执行环境。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- print g.next() #None  1
     再调用next方法，你也许会好奇，为啥打印出两个值，不急，且听我慢慢道来。
     上一次调用next,执行到yield 0暂停，再次执行恢复环境，给tmp赋值(注意：这里的tmp的值并不是x的值，而是通过send方法接受的值)，由于我们没有调用send方法，所以
tmp的值为None,此时输出None，并执行到下一次yield x,所以又输出1.
      到了这里，next方法我们都懂了，下面看看send方法。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- print g.send('hello') #world  2
      上一次执行到yield 1后暂停，此时我们send('hello')，那么程序将收到‘hello'，并给tmp赋值为’hello',此时tmp=='hello'为真，所以输出'world',并执行到下一次yield 2,所以又打印出2.（next()等价于send(None)）
      当循环结束，将抛出StopIteration停止生成器。
      看下面代码：
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- def stop_immediately(name):  
- if name == 'skycrab':  
- yield'okok'
- else:  
- print'nono'
- 
- s=stop_immediately('sky')  
- s.next()  
正如你所预料的，打印出’nono',由于没有额外的yield，所以将直接抛出StopIteration。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- nono  
- Traceback (most recent call last):  
-   File "F:\python workspace\Pytest\src\cs.py", line 170, in <module>  
-     s.next()  
- StopIteration  
      看下面代码，理解throw方法，throw主要是向生成器发送异常。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- def mygen():  
- try:  
- yield'something'
- except ValueError:  
- yield'value error'
- finally:  
- print'clean'#一定会被执行
- gg=mygen()  
- print gg.next() #something
- print gg.throw(ValueError) #value error  clean
     调用gg.next很明显此时输出‘something’,并在yield ‘something’暂停，此时向gg发送ValueError异常，恢复执行环境，except  将会捕捉，并输出信息。
     理解了这些，我们就可以向协同程序发起攻击了，所谓协同程序也就是是可以挂起，恢复，有多个进入点。其实说白了，也就是说多个函数可以同时进行，可以相互之间发送消息等。
     这里有必要说一下multitask模块(不是标准库中的),看一段multitask使用的简单代码：
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- def tt():  
- for x in xrange(4):  
- print'tt'+str(x)  
- yield
- 
- def gg():  
- for x in xrange(4):  
- print'xx'+str(x)  
- yield
- 
- t=multitask.TaskManager()  
- t.add(tt())  
- t.add(gg())  
- t.run()  
结果：
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- tt0  
- xx0  
- tt1  
- xx1  
- tt2  
- xx2  
- tt3  
- xx3  
   如果不是使用生成器，那么要实现上面现象，即函数交错输出，那么只能使用线程了，所以生成器给我们提供了更广阔的前景。 
   如果仅仅是实现上面的效果，其实很简单，我们可以自己写一个。主要思路就是将生成器对象放入队列，执行send(None)后，如果没有抛出StopIteration,将该生成器对象再加入队列。
**[python]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/10201327#)
- class Task():  
- def __init__(self):  
- self._queue = Queue.Queue()  
- 
- def add(self,gen):  
- self._queue.put(gen)  
- 
- def run(self):  
- whilenotself._queue.empty():  
- for i in xrange(self._queue.qsize()):  
- try:  
-                     gen= self._queue.get()  
-                     gen.send(None)  
- except StopIteration:  
- pass
- else:  
- self._queue.put(gen)  
- 
- t=Task()  
- t.add(tt())  
- t.add(gg())  
- t.run()  
  当然，multitask实现的肯定不止这个功能，有兴趣的童鞋可以看下源码，还是比较简单易懂的。
