# C#基础系列：异步编程初探async和await - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4734252.html)
前言：前面有篇从应用层面上面介绍了下多线程的几种用法，有博友就说到了async, await等新语法。确实，没有异步的多线程是单调的、乏味的，async和await是出现在C#5.0之后，它的出现给了异步并行变成带来了很大的方便。异步编程涉及到的东西还是比较多，本篇还是先介绍下async和await的原理及简单实现。
了解异步之前，我们先来看看Thread对象的升级版本Task对象：
1、Task对象的前世今生：Task对象是.Net Framework 4.0之后出现的异步编程的一个重要对象。在一定程度上来说，Task对象可以理解Thread对象的一个升级产品。既然是升级产品，那它肯定有他的优势，比如我们上面Thread对象不能解决的问题：对于有返回值类型的委托。Task对象就能简单的解决。

C#
```
static void Main(string[] args)
        {
            Console.WriteLine("执行GetReturnResult方法前的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            var strRes = Task.Run<string>(() => { return GetReturnResult(); });//启动Task执行方法
            Console.WriteLine("执行GetReturnResult方法后的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            Console.WriteLine(strRes.Result);//得到方法的返回值
            Console.WriteLine("得到结果后的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            Console.ReadLine();
        }
        static string GetReturnResult()
        {
            Thread.Sleep(2000);
            return "我是返回值";
        }
```
先来看结果：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/03/8691dc0a8aa997a92feb119e584ab6d2.png)
从结果分析可知在执行var strRes = Task.Run<string>(() => { return GetReturnResult(); })这一句后，主线程并没有阻塞去执行GetReturnResult()方法，而是开启了另一个线程去执行GetReturnResult()方法。直到执行strRes.Result这一句的时候主线程才会等待GetReturnResult()方法执行完毕。为什么说是开启了另一个线程，我们通过线程ID可以看得更清楚：

C#
```
static void Main(string[] args)
        {
            Console.WriteLine("执行GetReturnResult方法前的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            var strRes = Task.Run<string>(() => { return GetReturnResult(); });
            Console.WriteLine("执行GetReturnResult方法后的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            Console.WriteLine("我是主线程，线程ID：" + Thread.CurrentThread.ManagedThreadId);
            Console.WriteLine(strRes.Result);
            Console.WriteLine("得到结果后的时间：" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            Console.ReadLine();
        }
        static string GetReturnResult()
        {
            Console.WriteLine("我是GetReturnResult里面的线程，线程ID：" + Thread.CurrentThread.ManagedThreadId);
            Thread.Sleep(2000);
            return "我是返回值";
        }
```
结果：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/03/7e0ef05ee4a5a6749aed7959143f0c3a.png)
由此可以得知，Task.Run<string>(()=>{}).Reslut是阻塞主线程的，因为主线程要得到返回值，必须要等方法执行完成。
Task对象的用法如下：

C#
```
//用法一
            Task task1 = new Task(new Action(MyAction));
        　　//用法二
            Task task2 = new Task(delegate
            {
                MyAction();
            });
　　　　　　  //用法三
            Task task3 = new Task(() => MyAction());
            Task task4 = new Task(() =>
            {
                MyAction();
            });
            task1.Start();
            task2.Start();
            task3.Start();
            task4.Start();
```
由上可知，Task对象的构造函数传入的是一个委托，既然能传入Action类型的委托，可想而知Action的16中类型的参数又可以派上用场了。于是乎Task对象参数的传递就不用多说了吧。
2、初识 async & await。

C#
```
static void Main(string[] args)
        {
            Console.WriteLine("我是主线程，线程ID：{0}", Thread.CurrentThread.ManagedThreadId);
            TestAsync();
            Console.ReadLine();
        }
        static async Task TestAsync()
        {
            Console.WriteLine("调用GetReturnResult()之前，线程ID：{0}。当前时间：{1}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
            var name = GetReturnResult();
            Console.WriteLine("调用GetReturnResult()之后，线程ID：{0}。当前时间：{1}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
            Console.WriteLine("得到GetReturnResult()方法的结果：{0}。当前时间：{1}", await name, DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
        }
        static async Task<string> GetReturnResult()
        {
            Console.WriteLine("执行Task.Run之前, 线程ID：{0}", Thread.CurrentThread.ManagedThreadId);
            return await Task.Run(() =>
            {
                Thread.Sleep(3000);
                Console.WriteLine("GetReturnResult()方法里面线程ID: {0}", Thread.CurrentThread.ManagedThreadId);
                return "我是返回值";
            });
        }
```
结果：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/03/b91c3274d8e37caca38b05c71f6c3202.png)
我们来看看程序的执行过程：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/03/13229c34b6c09ac1ed8e2b32fb7a592d.png)
由上面的结果可以得到如下结论：
（1）在async标识的方法体里面，如果没有await关键字的出现，那么这种方法和调用普通的方法没什么区别。
（2）在async标识的方法体里面，在await关键字出现之前，还是主线程顺序调用的，直到await关键字的出现才会出现线程阻塞。
（3）await关键字可以理解为等待方法执行完毕，除了可以标记有async关键字的方法外，还能标记Task对象，表示等待该线程执行完毕。所以await关键字并不是针对于async的方法，而是针对async方法所返回给我们的Task。
（4）是否async关键字只能标识返回Task对象的方法呢。我们来试试：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/03/a04f1afed5741e0da470b6a02a2119d7.png)
异步方法的返回类型必须为void、Task或者Task<T>类型。也就是说async要么是void，要么和Task关联。
3、除了await关键字，Task对象还有另外一种方式等待执行结果。

C#
```
static async Task TestAsync()
        {
            Console.WriteLine("调用GetReturnResult()之前，线程ID：{0}。当前时间：{1}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
            var name = GetReturnResult();
            Console.WriteLine("调用GetReturnResult()之后，线程ID：{0}。当前时间：{1}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
            Console.WriteLine("得到GetReturnResult()方法的结果：{0}。当前时间：{1}", name.GetAwaiter().GetResult(), DateTime.Now.ToString("yyyy-MM-dd hh:MM:ss"));
        }
```
这样可以得到相同的结果。
name.GetAwaiter()这个方法得到的是一个TaskAwaiter对象，这个对象表示等待完成的异步任务的对象，并提供结果的参数。所以除了能完成await关键字的等待之外，它还能做一些其他的操作。我们将TaskAwaiter转到定义

C#
```
public struct TaskAwaiter<TResult> : ICriticalNotifyCompletion, INotifyCompletion
{
        public bool IsCompleted { get; }
        public TResult GetResult();
        public void OnCompleted(Action continuation);
        public void UnsafeOnCompleted(Action continuation);
}
```
IsCompleted：获取一个值，该值指示异步任务是否已完成。
GetResult()：得到执行的结果。这个方法和await关键字效果相同。
OnCompleted()：传入一个委托，在任务执行完成之后执行。
UnsafeOnCompleted()：计划与此 awaiter 相关异步任务的延续操作。
由此可以看出，await关键字实际上就是调用了TaskAwaiter对象的GetResult()方法。
### 本系列：
- [C#基础系列：再也不用担心面试官问我“事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98370/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98378/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
