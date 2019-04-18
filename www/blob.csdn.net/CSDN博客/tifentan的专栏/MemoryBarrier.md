# MemoryBarrier - tifentan的专栏 - CSDN博客

2018年03月09日 17:37:14[露蛇](https://me.csdn.net/tifentan)阅读数：86标签：[MemoryBarrier](https://so.csdn.net/so/search/s.do?q=MemoryBarrier&t=blog)


查了下MSDN的解释：

```
MemoryBarrier is required only on multiprocessor systems with weak memory ordering (for example, a system employing multiple Intel Itanium processors).

         Synchronizes memory access as follows: The processor executing the current thread cannot reorder instructions in such a way that memory accesses prior to the call to MemoryBarrier execute after memory accesses that follow the call to MemoryBarrier.

        就是说多核处理器会对运行CPU指令顺序重排优化，MemoryBarrier可以阻止指令重排，调用Thread.MemoryBarrier()之后的代码中内存访问不能再这之前就完成了。也就是它可以限制指令重排和内存读写的缓存。

     下面有段代码，在release编译情况下会死循环(debug应该是没做instructions reorder而不会有问题)

        bool complete = false;
        var t = new Thread(() =>
        {
            bool toggle = false;
            while (!complete)
                toggle = !toggle;
        });
        t.Start();
        Thread.Sleep(1000);
        complete = true;
        t.Join();
```

问题原因：

1.编译器、CLR或者CPU可能重新排序了程序指令，以此提高效率。

2.编译器、CLR或者CPU引入缓存优化导致其他的线程不能马上看到变量值的更改。

尝试了下将Thread.MomoryBarrier加到while中，保证complete读到的最新的：

此例子中MomoryBarrier 应该时解决jit时过度优化，虽然两个核心cache line 不会及时同步，complete 值修改通过MSIE协议也很快的通知到其他核心。

出现死循环在于jit 任魏complete值一直是false，并没有执行load操作，而是直接判断true跳转。

```
bool complete = false;
        var t = new Thread(() =>
        {
            bool toggle = false;
            while (!complete)
            {
               Thread.MemoryBarrier();
                toggle = !toggle;
            }
        });
        t.Start();
        Thread.Sleep(1000);
        complete = true;
        t.Join();
```

在看一个例子：（来自《window 并发编程指南》内存模型部分） 

MyObject mo= ..; 

int f= mo.field; 

if(f==0) 

{ 

　　//same operation 

　　Console.WriteLine(f); 

} 

如果mo.field 在读取和Console.WriteLine相隔足够远，那么编译器可能会认为读mo.field 读取两遍会更有效,被编译成如下代码： 

MyObject mo= ..; 

if(mo.filed==0) 

{ 

　　//same operation 

　　Console.WriteLine(mo.field); 

} 

编译器可能判断，保留这个值是否给寄存器带来压力并导致栈空间的低效使用，并且之个分支是否很少被使用（因此f值就不需要多次） 

这样带来多线程问题，将f值使用volatitle修饰，可禁止这种优化

**我看：多线程才会出现吧！！**

