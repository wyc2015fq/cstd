
# 测量运行时间StopWatch类 - 追求卓越,做到专业 - CSDN博客


2012年03月07日 16:12:02[Waldenz](https://me.csdn.net/enter89)阅读数：386


```python
using System;
using System.Diagnostics;
using System.Threading;
class Program
{
    static void Main(string[] args)
    {
                Stopwatch stopWatch = new Stopwatch();//测量运行时间
                stopWatch.Start();//启动
                Thread.Sleep(10000);//运行10秒
                stopWatch.Stop();//计时器停止
                // Get the elapsed time as a TimeSpan value.
                TimeSpan ts = stopWatch.Elapsed;

                // Format and display the TimeSpan value.
                string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                    ts.Hours, ts.Minutes, ts.Seconds,
                    ts.Milliseconds / 10);
                Console.WriteLine(elapsedTime, "RunTime");//00:00:09.99

                Stopwatch sw = Stopwatch.StartNew();//创建类并启动
                Thread.Sleep(5000);
                Console.WriteLine(sw.Elapsed);//00:00:04.9997583
                Console.WriteLine(sw.IsRunning);//查看stopwatch是否在运行.不调用sw.Stop()的话,一直是true.

                Console.WriteLine("Success");
```
```python
}
}
```


