# C#Task执行线程及其相关问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月01日 18:54:38[boonya](https://me.csdn.net/boonya)阅读数：5813







对于多线程，我们经常使用的是Thread。在我们了解Task之前，如果我们要使用多核的功能可能就会自己来开线程，然而这种线程模型在.net 4.0之后被一种称为基于“任务的编程模型”所冲击，因为task会比thread具有更小的性能开销，不过大家肯定会有疑惑，任务和线程到底有什么区别呢？另外使用过程中也有一些陷阱，需要绕开它们才能避免出错。
#### Task和Thread的区别
- Task是架构在Thread之上的，也就是说任务最终还是要抛给线程去执行。
- Task跟Thread不是一对一的关系，比如开10个任务并不是说会开10个线程，这一点任务有点类似线程池，但是任务相比线程池有很小的开销和精确的控制。

#### Task控制台测试程序

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MyTask
{
    class Program
    {
        /// <summary>
        /// 启动方式
        /// </summary>
        public static void StartWay()
        {
            var task1 = new Task(() =>
            {
                Console.WriteLine("Hello,task");
            });
            task1.Start();

            var task2 = Task.Factory.StartNew(() =>
            {
                Console.WriteLine("Hello,task started by task factory");
            });
        }

        /// <summary>
        /// 生命周期测试
        /// </summary>
        public static void LifeCircle()
        {
            var task1 = new Task(() =>
            {
                Console.WriteLine("Begin");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Finish");
            });
            Console.WriteLine("Before start:" + task1.Status);
            task1.Start();
            Console.WriteLine("After start:" + task1.Status);
            task1.Wait();
            Console.WriteLine("After Finish:" + task1.Status);

        }

        /// <summary>
        /// 等待所有任务完成
        /// </summary>
        public static void WaitAll()
        {
            var task1 = new Task(() =>
            {
                Console.WriteLine("Task 1 Begin");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Task 1 Finish");
            });
            var task2 = new Task(() =>
            {
                Console.WriteLine("Task 2 Begin");
                System.Threading.Thread.Sleep(3000);
                Console.WriteLine("Task 2 Finish");
            });

            task1.Start();
            task2.Start();
            Task.WaitAll(task1, task2);
            Console.WriteLine("WaitAll task finished!");

        }

        /// <summary>
        /// 等待任意一个执行任务完成
        /// </summary>
        public static void WaitAny()
        {
            var task1 = new Task(() =>
            {
                Console.WriteLine("Task 1 Begin");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Task 1 Finish");
            });
            var task2 = new Task(() =>
            {
                Console.WriteLine("Task 2 Begin");
                System.Threading.Thread.Sleep(3000);
                Console.WriteLine("Task 2 Finish");
            });

            task1.Start();
            task2.Start();
            Task.WaitAny(task1, task2);
            Console.WriteLine("WaitAny task finished!");
        }

        /// <summary>
        /// 任务回调方法
        /// </summary>
        public static void ContinueWith()
        {
            var task1 = new Task(() =>
            {
                Console.WriteLine("Task 1 Begin");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Task 1 Finish");
            });
            var task2 = new Task(() =>
            {
                Console.WriteLine("Task 2 Begin");
                System.Threading.Thread.Sleep(3000);
                Console.WriteLine("Task 2 Finish");
            });


            task1.Start();
            task2.Start();
            var result = task1.ContinueWith<string>(task =>
            {
                Console.WriteLine("task1 finished!");
                return "This is task1 result!";
            });

            Console.WriteLine(result.Result.ToString());

            var result2 = task2.ContinueWith<string>(task =>
            {
                Console.WriteLine("task2 finished!");
                return "This is task2 result!";
            });

            Console.WriteLine(result2.Result.ToString());
        }

        /// <summary>
        /// 取消任务
        /// </summary>
        public static void Cancel()
        {
            var tokenSource = new CancellationTokenSource();
            var token = tokenSource.Token;
            var task = Task.Factory.StartNew(() =>
            {
                for (var i = 0; i < 1000; i++)
                {
                    System.Threading.Thread.Sleep(1000);
                    if (token.IsCancellationRequested)
                    {
                        Console.WriteLine("Abort mission success!");
                        return;
                    }
                }
            }, token);
            token.Register(() =>
            {
                Console.WriteLine("Canceled");
            });
            Console.WriteLine("Press enter to cancel task...");
            Console.ReadKey();
            tokenSource.Cancel();

            Console.ReadKey();//这句忘了加，程序退出了，看不到“Abort mission success!“这个提示
        }

        /// <summary>
        /// 函数入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Console.WriteLine("StartWay is runing");
            StartWay();
            Console.WriteLine("LifeCircle is runing");
            LifeCircle();
            Console.WriteLine("WaitAll is runing");
            WaitAll();
            Console.WriteLine("WaitAny is runing");
            WaitAny();
            Console.WriteLine("ContinueWith is runing");
            ContinueWith();
            Console.WriteLine("Cancel is runing");
            Cancel();

            Console.Read();
        }
    }
}
```

参考地址：[http://www.cnblogs.com/yunfeifei/p/4106318.html](http://www.cnblogs.com/yunfeifei/p/4106318.html)

#### 内嵌和异常

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MyTask
{
    class Program
    {
       
        /// <summary>
        /// 内部Task测试-普通方式
        /// <mark>
        /// 执行结果：
        /// Parent task finished!
        /// Flag
        /// Childen task finished!
        /// </mark>
        /// </summary>
        public static void InnerTask()
        {
            var pTask = Task.Factory.StartNew(() =>
            {
                var cTask = Task.Factory.StartNew(() =>
                {
                    System.Threading.Thread.Sleep(2000);
                    Console.WriteLine("Childen task finished!");
                });
                Console.WriteLine("Parent task finished!");
            });
            pTask.Wait();
            Console.WriteLine("Flag");
        }

        /// <summary>
        /// 内部Task测试-父子任务关联
        /// <mark>
        /// 执行结果：
        /// Parent task finished!
        /// Childen task finished!
        /// Flag
        /// </mark>
        /// </summary>
        public static void InnerTaskAttachedToParent()
        {
            var pTask = Task.Factory.StartNew(() =>
            {
                var cTask = Task.Factory.StartNew(() =>
                {
                    System.Threading.Thread.Sleep(2000);
                    Console.WriteLine("Childen task finished!");
                }, TaskCreationOptions.AttachedToParent);
                Console.WriteLine("Parent task finished!");
            });
            pTask.Wait();
            Console.WriteLine("Flag");
        }

        /// <summary>
        /// 多任务互相关联结果运算
        /// </summary>
        public static void MultiInnerTask()
        {
            Task.Factory.StartNew(() =>
            {
                var t1 = Task.Factory.StartNew<int>(() =>
                {
                    Console.WriteLine("Task 1 running...");
                    return 1;
                });
                t1.Wait(); //等待任务一完成
                var t3 = Task.Factory.StartNew<int>(() =>
                {
                    Console.WriteLine("Task 3 running...");
                    return t1.Result + 3;
                });
                var t4 = Task.Factory.StartNew<int>(() =>
                {
                    Console.WriteLine("Task 2 running...");
                    return t1.Result + 2;
                }).ContinueWith<int>(task =>
                {
                    Console.WriteLine("Task 4 running...");
                    return task.Result + 4;
                });
                Task.WaitAll(t3, t4);  //等待任务三和任务四完成
                var result = Task.Factory.StartNew(() =>
                {
                    Console.WriteLine("Task Finished! The result is {0}", t3.Result + t4.Result);
                });
            });
        }

        /// <summary>
        /// 函数入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Console.WriteLine("InnerTask is runing");
            InnerTask();
            Console.WriteLine("InnerTaskAttachedToParent is runing");
            InnerTaskAttachedToParent();
            Console.WriteLine("MultiInnerTask is runing");
            MultiInnerTask();

            Console.Read();
        }
    }
}
```

参考地址：[http://www.cnblogs.com/yunfeifei/p/4111112.html](http://www.cnblogs.com/yunfeifei/p/4111112.html)




#### **多线程带来的问题**


**1、死锁问题 **

　　前面我们学习了Task的使用方法，其中Task的等待机制让我们瞬间爱上了它，但是如果我们在调用Task.WaitAll方法等待所有线程时，如果有一个Task一直不返回，会出现什么情况呢？当然，如果我们不退出来的话，程序会一直等待下去，那么因为这一个Task的死锁，导致其他的任务也无法正常提交，整个程序"死"在那里。下面我们来写一段代码，来看一下死锁的情况：

```
var t1 = Task.Factory.StartNew(() =>
            {
                Console.WriteLine("Task 1 Start running...");
                while(true)
                {
                    System.Threading.Thread.Sleep(1000);
                }
                Console.WriteLine("Task 1 Finished!");
            });
            var t2 = Task.Factory.StartNew(() =>
            {
                Console.WriteLine("Task 2 Start running...");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Task 2 Finished!");
            });
            Task.WaitAll(t1,t2);
```

这里我们创建两个Task，t1和t2，t1里面有个while循环，由于条件一直为TRUE，所以他永远也无法退出。运行程序，结果如下：

![](https://images0.cnblogs.com/blog/622439/201411/252239244026260.png)

可以看到Task2完成了，就是迟迟等不到Task1，这个时候我们按回车是没有反应的，除非关掉窗口。如果我们在项目中遇到这种情况是令人很纠结的，因为我们也不知道到底发生了什么，程序就是停在那里，也不报错，也不继续执行。

那么出现这种情况我们该怎么处理呢？我们可以设置最大等待时间，如果超过了等待时间，就不再等待，下面我们来修改代码，设置最大等待时间为5秒(项目中可以根据实际情况设置)，如果超过5秒就输出哪个任务出错了，代码如下：

```
Task[] tasks = new Task[2];
            tasks[0] = Task.Factory.StartNew(() =>
            {
                Console.WriteLine("Task 1 Start running...");
                while(true)
                {
                    System.Threading.Thread.Sleep(1000);
                }
                Console.WriteLine("Task 1 Finished!");
            });
           tasks[1] = Task.Factory.StartNew(() =>
            {
                Console.WriteLine("Task 2 Start running...");
                System.Threading.Thread.Sleep(2000);
                Console.WriteLine("Task 2 Finished!");
            });
            
            Task.WaitAll(tasks,5000);
            for (int i = 0; i < tasks.Length;i++ )
            {
                if (tasks[i].Status != TaskStatus.RanToCompletion)
                {
                    Console.WriteLine("Task {0} Error!",i + 1);
                }
            }
            Console.Read();
```

这里我们将所有任务放到一个数组里面进行管理，调用Task.WaitAll的一个重载方法，第一个参数是Task[]数据,第二个参数是最大等待时间，单位是毫秒，这里我们设置为5000及等待5秒钟，就继续向下执行。下面我们遍历Task数组，通过Status属性判断哪些Task没有完成，然后输出错误信息。



**2、SpinLock(自旋锁)**

　　 我们初识多线程或者多任务时，第一个想到的同步方法就是使用lock或者Monitor，然而在4.0 之后微软给我们提供了另一把利器——spinLock，它比重量级别的Monitor具有更小的性能开销，它的用法跟Monitor很相似，VS给的提示如下：

![](https://images0.cnblogs.com/blog/622439/201411/252337408562274.png)

下面我们来写一个例子看一下，代码如下(关于lock和Monitor的用法就不再细说了，网上资料很多，大家可以看看)：

```
SpinLock slock = new SpinLock(false);
            long sum1 = 0;
            long sum2 = 0;
            Parallel.For(0, 100000, i =>
            {
                sum1 += i;
            });

            Parallel.For(0, 100000, i =>
            {
                bool lockTaken = false;
                try
                {
                    slock.Enter(ref lockTaken);
                    sum2 += i;
                }
                finally
                {
                    if (lockTaken)
                        slock.Exit(false);
                }
            });

            Console.WriteLine("Num1的值为:{0}", sum1);
            Console.WriteLine("Num2的值为:{0}", sum2);

            Console.Read();
```

输出结果如图：

![](https://images0.cnblogs.com/blog/622439/201411/252338314181092.png)

这里我们使用了Parallel.For方法来做演示，Parallel.For用起来方便，但是在实际开发中还是尽量少用，因为它的不可控性太高，有点简单粗暴的感觉，可能带来一些不必要的"麻烦",最好还是使用Task，因为Task的可控性较好。

slock.Enter方法，解释如下：

![](https://images0.cnblogs.com/blog/622439/201411/252339147318360.png)



**3、多线程之间的数据同步**

　　多线程间的同步，在用thread的时候，我们常用的有lock和Monitor，上面刚刚介绍了.Net4.0中一个新的锁——SpinLock(自旋锁)，实际上，我们还可以将任务分成多块，由多个线程一起执行，最后合并多个线程的结果，如：求1到100的和，我们分10个线程，分别求1~10，......，90~100的和，然后合并十个线程的结果。还有就是使用线程安全集合，可参加第二天的文章。其实Task的同步机制做已经很好了，如果有特殊业务需求，有线程同步问题，大家可一起交流~~


#### ** Task和线程池之间的抉择**


　　我们要说的task的知识也说的差不多了，接下来我们开始站在理论上了解下“线程池”和“任务”之间的关系，我们要做到知其然，还要知其所以然。不管是说线程还是任务，我们都不可避免的要讨论下线程池，然而在.net 4.0以后，线程池引擎考虑了未来的扩展性，已经充分利用多核微处理器架构，只要在可能的情况下，我们应该尽量使用task，而不是线程池。

 　　这里简要的分析下CLR线程池，其实线程池中有一个叫做“全局队列”的概念，每一次我们使用QueueUserWorkItem的使用都会产生一个“工作项”，然后“工作项”进入“全局队列”进行排队，最后线程池中的的工作线程以FIFO(First Input First Output)的形式取出,这里值得一提的是在.net 4.0之后“全局队列”采用了无锁算法，相比以前版本锁定“全局队列”带来的性能瓶颈有了很大的改观。那么任务委托的线程池不光有“全局队列”，而且每一个工作线程都有”局部队列“。我们的第一反应肯定就是“局部队列“有什么好处呢？这里暂且不说，我们先来看一下线程池中的任务分配，如下图：

![](https://images0.cnblogs.com/blog/622439/201411/252218290745136.png)

线程池的工作方式大致如下，线程池的最小线程数是6，线程1~3正在执行任务1~3，当有新的任务时，就会向线程池请求新的线程，线程池会将空闲线程分配出去，当线程不足时，线程池就会创建新的线程来执行任务，直到线程池达到最大线程数(线程池满)。总的来说，只有有任务就会分配一个线程去执行,当FIFO十分频繁时，会造成很大的线程管理开销。

　　下面我们来看一下task中是怎么做的,当我们new一个task的时候“工作项”就会进去”全局队列”，如果我们的task执行的非常快，那么“全局队列“就会FIFO的非常频繁，那么有什么办法缓解呢？当我们的task在嵌套的场景下，“局部队列”就要产生效果了，比如我们一个task里面有3个task，那么这3个task就会存在于“局部队列”中，**如下图的任务一，里面有三个任务要执行，也就是产生了所谓的"局部队列"，当任务三的线程执行完成时，就会从任务一种的队列中以FIFO的形式"窃取"任务执行**，从而减少了线程管理的开销。这就相当于，有两个人，一个人干完了分配给自己的所有活，而另一个人却还有很多的活，闲的人应该接手点忙的人的活，一起快速完成。

![](https://images0.cnblogs.com/blog/622439/201411/260023400749950.png)

　　从上面种种情况我们看到，这些分流和负载都是普通ThreadPool.QueueUserWorkItem所不能办到的，所以说在.net 4.0之后，我们尽可能的使用TPL，抛弃ThreadPool。
原文地址：[http://www.cnblogs.com/yunfeifei/p/4122084.html](http://www.cnblogs.com/yunfeifei/p/4122084.html)



