## Python高级编程-多线程

**(一)进程线程概述：**

很多同学都听说过，现代操作系统比如Mac OS X，UNIX，Linux，Windows等，都是支持“多任务”的操作系统。

什么叫“多任务”呢？简单地说，就是操作系统可以同时运行多个任务。打个比方，你一边在用浏览器上网，一边在听MP3，一边在用Word赶作业，这就是多任务，至少同时有3个任务正在运行。还有很多任务悄悄地在后台同时运行着，只是桌面上没有显示而已。

现在，多核CPU已经非常普及了，但是，即使过去的单核CPU，也可以执行多任务。由于CPU执行代码都是顺序执行的，那么，单核CPU是怎么执行多任务的呢？

答案就是操作系统轮流让各个任务交替执行，任务1执行0.01秒，切换到任务2，任务2执行0.01秒，再切换到任务3，执行0.01秒……这样反复执行下去。表面上看，每个任务都是交替执行的，但是，由于CPU的执行速度实在是太快了，我们感觉就像所有任务都在同时执行一样。

真正的并行执行多任务只能在多核CPU上实现，但是，由于任务数量远远多于CPU的核心数量，所以，操作系统也会自动把很多任务轮流调度到每个核心上执行。

对于操作系统来说，一个任务就是一个进程（Process），比如打开一个浏览器就是启动一个浏览器进程，打开一个记事本就启动了一个记事本进程，打开两个记事本就启动了两个记事本进程，打开一个Word就启动了一个Word进程。

有些进程还不止同时干一件事，比如Word，它可以同时进行打字、拼写检查、打印等事情。在一个进程内部，要同时干多件事，就需要同时运行多个“子任务”，我们把进程内的这些“子任务”称为线程（Thread）。

由于每个进程至少要干一件事，所以，一个进程至少有一个线程。当然，像Word这种复杂的进程可以有多个线程，多个线程可以同时执行，多线程的执行方式和多进程是一样的，也是由操作系统在多个线程之间快速切换，让每个线程都短暂地交替运行，看起来就像同时执行一样。当然，真正地同时执行多线程需要多核CPU才可能实现。

我们前面编写的所有的Python程序，都是执行单任务的进程，也就是只有一个线程。如果我们要同时执行多个任务怎么办？

有三种解决方案：

一种是启动多个进程，每个进程虽然只有一个线程，但多个进程可以一块执行多个任务。

还有一种方法是启动一个进程，在一个进程内启动多个线程，这样，多个线程也可以一块执行多个任务。

当然还有第三种方法，就是启动多个进程，每个进程再启动多个线程，这样同时执行的任务就更多了

总结一下就是，多任务的实现有3种方式：

- 多进程模式；
- 多线程模式；
- 多进程+多线程模式。

线程是最小的执行单元，而进程由至少一个线程组成。如何调度进程和线程，完全由操作系统决定，程序自己不能决定什么时候执行，执行多长时间。

多进程和多线程的程序涉及到同步、数据共享的问题，编写起来更复杂。

关于进程和线程，大家总结一句话是“进程是操作系统分配资源的最小单元，线程是操作系统调度的最小单元”。

进程是具有一定独立功能的程序关于某个数据集合上的一次运行活动,进程是系统进行资源分配和调度的一个独立单位.
线程是进程的一个实体,是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位.线程自己基本上不拥有系统资源,只拥有一点在运行中必不可少的资源(如程序计数器,一组寄存器和栈),但是它可与同属一个进程的其他的线程共享进程所拥有的全部资源.
一个线程可以创建和撤销另一个线程;同一个进程中的多个线程之间可以并发执行.

进程和线程的主要差别在于它们是不同的操作系统资源管理方式。进程有独立的地址空间，一个进程崩溃后，在保护模式下不会对其它进程产生影响，而线程只是一个进程中的不同执行路径。线程有自己的堆栈和局部变量，但线程之间没有单独的地址空间，一个线程死掉就等于整个进程死掉，所以多进程的程序要比多线程的程序健壮，但在进程切换时，耗费资源较大，效率要差一些。但对于一些要求同时进行并且又要共享某些变量的并发操作，只能用线程，不能用进程。

 

**(二)多线程编程**

多任务可以由多进程完成，也可以由一个进程内的多线程完成。

我们前面提到了进程是由若干线程组成的，一个进程至少有一个线程。

由于线程是操作系统直接支持的执行单元，因此，高级语言通常都内置多线程的支持，Python也不例外，并且，Python的线程是真正的Posix Thread，而不是模拟出来的线程。

Python的标准库提供了两个模块：`_thread`和`threading`，`_thread`是低级模块，`threading`是高级模块，对`_thread`进行了封装。绝大多数情况下，我们只需要使用`threading`这个高级模块。

(1)启动一个线程就是把一个函数传入并创建`Thread`实例，然后调用`start()`开始执行:

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 def loop(x):
 2     print("%s start" % threading.current_thread().name)
 3     for i in range(x):
 4         time.sleep(1)
 5         print("%s:%d" % (threading.current_thread().name, i))
 6     print("%s stop" % threading.current_thread().name)
 7 
 8 
 9 print("%s start" % threading.current_thread().name)
10 t1 = threading.Thread(target=loop, args=(6,))
11 t1.start()
12 print("%s stop" % threading.current_thread().name)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

程序中为Thread类创建了一个实例t1，传入的参数是函数名loop以及loop函数的参数列表，利用threading.current_thread()返回当前运行的线程实例，程序运行的结果如下：

 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 MainThread start
 2 Thread-1 start
 3 MainThread stop
 4 Thread-1:0
 5 Thread-1:1
 6 Thread-1:2
 7 Thread-1:3
 8 Thread-1:4
 9 Thread-1:5
10 Thread-1 stop
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

我们从运行结果看到，主线程MainThread先于子线程Thread-1退出

多线程运行过程如下图：

![img](https://images2015.cnblogs.com/blog/998751/201702/998751-20170217165413769-145023064.png)。

如果我们希望主线程等待子线程呢？下面看看join()方法的效果：

(2)join()

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import threading
 2 import time
 3 
 4 
 5 def loop(x):
 6     print("%s start" % threading.current_thread().name)
 7     for i in range(x):
 8         time.sleep(1)
 9         print("%s:%d" % (threading.current_thread().name, i))
10     print("%s stop" % threading.current_thread().name)
11 
12 
13 print("%s start" % threading.current_thread().name)
14 t1 = threading.Thread(target=loop, args=(6,))
15 t1.start()
16 t1.join()
17 print("%s stop" % threading.current_thread().name)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

我们在上面的代码段内加入了 t1.join() ,看看运行效果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 MainThread start
 2 Thread-1 start
 3 Thread-1:0
 4 Thread-1:1
 5 Thread-1:2
 6 Thread-1:3
 7 Thread-1:4
 8 Thread-1:5
 9 Thread-1 stop
10 MainThread stop
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

从上面的运行结果看，MainThread在join之后一直停在join的地方，等待子线程Thread-1退出后才继续执行下去。

 

假如我们希望主线程退出的时候，不管子线程运行到哪里，强行让子线程退出呢？我们有 setDaemon(True) 方法:

(3)setDaemon()

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import threading
 2 import time
 3 
 4 
 5 def loop(x):
 6     print("%s start" % threading.current_thread().name)
 7     for i in range(x):
 8         time.sleep(1)
 9         print("%s:%d" % (threading.current_thread().name, i))
10     print("%s stop" % threading.current_thread().name)
11 
12 
13 print("%s start" % threading.current_thread().name)
14 t1 = threading.Thread(target=loop, args=(6,))
15 t1.setDaemon(True)
16 t1.start()
17 print("%s stop" % threading.current_thread().name)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

程序运行结果：

```
1 MainThread start
2 Thread-1 start
3 MainThread stop
```

我们看到主线程一旦退出，子线程也停止了，需要注意的是 setDaemon(True) 在 start() 之前

 

 (4)lock

多线程和多进程最大的不同在于，多进程中，同一个变量，各自有一份拷贝存在于每个进程中，互不影响，而多线程中，所有变量都由所有线程共享，所以，任何一个变量都可以被任何一个线程修改，因此，线程之间共享数据最大的危险在于多个线程同时改一个变量，把内容给改乱了。

来看看多个线程同时操作一个变量怎么把内容给改乱了：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import threading
 2 
 3 deposit = 0 # 银行存款
 4 
 5 
 6 def change_it(n):
 7     global deposit
 8     deposit = deposit + n  # 存
 9     deposit = deposit - n  # 取
10 
11 
12 def loop(n):
13     for i in range(100000):
14         change_it(n)
15 
16 t1 = threading.Thread(target=loop, args=(5,))
17 t2 = threading.Thread(target=loop, args=(8,))
18 t1.start()
19 t2.start()
20 t1.join()
21 t2.join()
22 print(deposit)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

我们定义了一个共享变量`deposit`，初始值为`0`，并且启动两个线程，先存后取，理论上结果应该为`0`，但是，由于线程的调度是由操作系统决定的，当t1、t2交替执行时，只要循环次数足够多，`deposit`的结果就不一定是`0`了(运行的结果不定，有时候是0，有时候是5，8，-8，-3等)，deposit值的偏差随着loop里循环的次数增加。

原因是因为高级语言的一条语句在CPU执行时是若干条语句，即使一个简单的计算：

 deposit = deposit + n 

也分两步：

1. 计算`deposit + n`，存入临时变量中；
2. 将临时变量的值赋给deposit。

上面的语句等价于：

```
temp = deposit + n
deposit = temp
```

(尝试将语句改为 deposit += n，发现结果总是0，说明 +=是一个原子操作)

由于temp是局部变量，两个线程各自都有自己的temp，当代码正常执行时：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 # 正常运行过程:
2 # t1: temp1 = deposit + 5  # temp1 = 0 + 5 = 5
3 # t1: deposit = temp1      # deposit = 5
4 # t1: temp1 = deposit - 5  # temp1 = 5 - 5 = 0
5 # t1: deposit =  temp1     # deposit = 0
6 # t2: temp2 = deposit + 8  # temp2 = 0 + 8 = 8
7 # t2: deposit = temp2      # deposit = 8
8 # t2: temp2 = deposit - 8  # temp2 = 8 - 8 = 0
9 # t2: deposit =  temp2     # deposit = 0
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

但是t1和t2是交替运行的，如果操作系统以下面的顺序执行t1、t2：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 # 多线程没有加锁可能的情况：
2 # t1: temp1 = deposit + 5  # temp1 = 0 + 5 = 5
3 # t2: temp2 = deposit + 8  # temp2 = 0 + 8 = 8
4 # t2: deposit = temp2      # deposit = 8
5 # t1: deposit = temp1      # deposit = 5
6 # t1: temp1 = deposit - 5  # temp1 = 5 - 5 = 0
7 # t1: deposit =  temp1     # deposit = 0
8 # t2: temp2 = deposit - 8  # temp2 = 0 - 8 = -8
9 # t2: deposit =  temp2     # deposit = -8
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

究其原因，是因为修改`deposit`需要多条语句，而执行这几条语句时，线程可能中断，从而导致多个线程把同一个对象的内容改乱了。

两个线程同时一存一取，就可能导致余额不对，你肯定不希望你的银行存款莫名其妙地变成了负数，所以，我们必须确保一个线程在修改`deposit`的时候，别的线程一定不能改。

如果我们要确保`deposit`计算正确，就要给`change_it()`上一把锁，当某个线程开始执行`change_it()`时，我们说，该线程因为获得了锁，因此其他线程不能同时执行`change_it()`，只能等待，直到锁被释放后，获得该锁以后才能改。由于锁只有一个，无论多少线程，同一时刻最多只有一个线程持有该锁，所以，不会造成修改的冲突。创建一个锁就是通过`threading.Lock()`来实现：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import threading
 2 
 3 deposit = 0 # 银行存款
 4 lock_deposit = threading.Lock()
 5 
 6 
 7 def change_it(n):
 8     global deposit
 9     deposit += deposit  # 存
10     deposit -= deposit  # 取
11 
12 
13 def loop(n):
14     for i in range(1000000):
15         lock_deposit.acquire()  # 先获取锁
16         try:
17             change_it(n)
18         finally:
19             lock_deposit.release()  # 确保释放锁
20 
21 t1 = threading.Thread(target=loop, args=(5,))
22 t2 = threading.Thread(target=loop, args=(8,))
23 t1.start()
24 t2.start()
25 t1.join()
26 t2.join()
27 print(deposit)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

当多个线程同时执行`lock.acquire()`时，只有一个线程能成功地获取锁，然后继续执行代码，其他线程就继续等待直到获得锁为止。

获得锁的线程用完后一定要释放锁，否则那些苦苦等待锁的线程将永远等待下去，成为死线程。所以我们用`try...finally`来确保锁一定会被释放。

锁的好处就是确保了某段关键代码只能由一个线程从头到尾完整地执行，坏处当然也很多，首先是阻止了多线程并发执行，包含锁的某段代码实际上只能以单线程模式执行，效率就大大地下降了。其次，由于可以存在多个锁，不同的线程持有不同的锁，并试图获取对方持有的锁时，可能会造成死锁，导致多个线程全部挂起，既不能执行，也无法结束，只能靠操作系统强制终止。

 

(5)全局解释器锁(Global Interpreter Lock)：

如果你不幸拥有一个多核CPU，你肯定在想，多核应该可以同时执行多个线程。

如果写一个死循环的话，会出现什么情况呢？

打开Mac OS X的Activity Monitor，或者Windows的Task Manager，都可以监控某个进程的CPU使用率。

我们可以监控到一个死循环线程会100%占用一个CPU。

如果有两个死循环线程，在多核CPU中，可以监控到会占用200%的CPU，也就是占用两个CPU核心。

要想把N核CPU的核心全部跑满，就必须启动N个死循环线程。

试试用Python写个死循环，启动与CPU核心数量相同的N个线程，在4核CPU上可以监控到CPU占用率仅有102%，也就是仅使用了一核。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import multiprocessing
 2 
 3 
 4 def loop():
 5     x = 0
 6     while True:
 7         x = x ^ 1
 8 
 9 for i in range(multiprocessing.cpu_count()):
10     t = threading.Thread(target=loop)
11     t.start()
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

但是用C、C++或Java来改写相同的死循环，直接可以把全部核心跑满，4核就跑到400%，8核就跑到800%，为什么Python不行呢？

因为Python的线程虽然是真正的线程，但解释器执行代码时，有一个GIL锁：Global Interpreter Lock，任何Python线程执行前，必须先获得GIL锁，然后，每执行100条字节码，解释器就自动释放GIL锁，让别的线程有机会执行。这个GIL全局锁实际上把所有线程的执行代码都给上了锁，所以，多线程在Python中只能交替执行，即使100个线程跑在100核CPU上，也只能用到1个核。

GIL是Python解释器设计的历史遗留问题，通常我们用的解释器是官方实现的CPython，要真正利用多核，除非重写一个不带GIL的解释器。

所以，在Python中，可以使用多线程，但不要指望能有效利用多核。如果一定要通过多线程利用多核，那只能通过C扩展来实现，不过这样就失去了Python简单易用的特点。

不过，也不用过于担心，Python虽然不能利用多线程实现多核任务，但可以通过多进程实现多核任务。多个Python进程有各自独立的GIL锁，互不影响。

 

(6)自定义Thread类：

 