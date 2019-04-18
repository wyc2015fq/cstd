# AutoResetEvent - weixin_33985507的博客 - CSDN博客
2015年11月06日 09:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
private static readonly AutoResetEvent autoResetEvent = new AutoResetEvent(false);
        private static void Main()
        {
            try
            {
                Console.WriteLine("{1}线程={0}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
                Thread thread = new Thread(Method)
                {
                    IsBackground = true
                };
                thread.Start();
                autoResetEvent.WaitOne();
                Console.WriteLine("Hello World");
            }
            catch (Exception ex)
            {
                while (ex != null)
                {
                    Console.WriteLine(ex.Message);
                    ex = ex.InnerException;
                }
            }
            Console.ReadLine();
        }
        private static void Method()
        {
            Thread.Sleep(2000);
            Console.WriteLine("{1}线程={0}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
            autoResetEvent.Set();
        }
```
```
private static readonly AutoResetEvent autoResetEvent = new AutoResetEvent(false);
        private static void Main()
        {
            try
            {
                Console.WriteLine("{1}线程={0}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
                Thread thread = new Thread(Method)
                {
                    IsBackground = true
                };
                thread.Start();
                bool flag = autoResetEvent.WaitOne(1000);//这里只等待1秒，但是线程里的Set需要2秒之后才执行
                if (flag)
                {
                    Console.WriteLine("Hello World");
                }
                autoResetEvent.WaitOne();//这里会受到上一次的Set的影响，可以直接执行
                Console.WriteLine("Hello Chuck");
                autoResetEvent.WaitOne();
                Console.WriteLine("To be continued");
            }
            catch (Exception ex)
            {
                while (ex != null)
                {
                    Console.WriteLine(ex.Message);
                    ex = ex.InnerException;
                }
            }
            Console.ReadLine();
        }
        private static void Method()
        {
            Thread.Sleep(2000);
            Console.WriteLine("{1}线程={0}", Thread.CurrentThread.ManagedThreadId, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
            autoResetEvent.Set();
        }
```
总结，AutoResetEvent在WaitOne之前需要先调用Reset，以防止之前某次超时之后，有其他地方调用了Set，那样会导致下一次的WaitOne可以直接进入
