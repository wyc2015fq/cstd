# C# Parallel并行遍历方法和执行任务 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月01日 17:38:48[boonya](https://me.csdn.net/boonya)阅读数：604








Parallel遍历分为For、ForEach，支持Lamda表达式，执行并行任务使用Invoke。

控制台示例程序：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MyParallel
{
    /// <summary>
    ///  Parallel.For()和Paraller.ForEach()方法在每次迭代中调用相同的代码，而Parallel.Invoke()方法允许同时调用不同的方法。Parallel.ForEach()用于数据并行性，Parallel.Invoke()用于任务并行性；
    /// </summary>
    class Program
    {
        /// <summary>
        /// 并行运行方法-Parallel.For-遍历顺序不定
        /// </summary>
        /// <param name="args"></param>
        static void Main0(string[] args)
        {

            ParallelLoopResult result = Parallel.For(0, 10, i =>
            {
                Console.WriteLine("迭代次数：{0},任务ID:{1},线程ID:{2}", i, Task.CurrentId, Thread.CurrentThread.ManagedThreadId);
                Thread.Sleep(10);
            });

            Console.WriteLine("是否完成:{0}", result.IsCompleted);
            Console.WriteLine("最低迭代:{0}", result.LowestBreakIteration);

            Console.Read();
        }

        /// <summary>
        /// 并行运行方法-Parallel.For-遍历顺序不定
        /// </summary>
        /// <param name="args"></param>
        static void Main1(string[] args)
        {

            ParallelLoopResult result = Parallel.For(0, 10, (i, state) =>
            {
                Console.WriteLine("迭代次数：{0},任务ID:{1},线程ID:{2}", i, Task.CurrentId, Thread.CurrentThread.ManagedThreadId);
                Thread.Sleep(10);
                if (i > 5)
                    state.Break();
            });

            Console.WriteLine("是否完成:{0}", result.IsCompleted);
            Console.WriteLine("最低迭代:{0}", result.LowestBreakIteration);

            Console.Read();
        }

        /// <summary>
        /// 并行运行方法-Parallel.ForEach-遍历顺序不定
        /// </summary>
        /// <param name="args"></param>
        static void Main2(string[] args)
        {

            string[] data = { "str1", "str2", "str3" };
            ParallelLoopResult result = Parallel.ForEach<string>(data, str =>
            {
                Console.WriteLine(str);
            });
            Console.WriteLine("是否完成:{0}", result.IsCompleted);
            Console.WriteLine("最低迭代:{0}", result.LowestBreakIteration);

            Console.Read();
        }

        /// <summary>
        /// 并行运行方法-Parallel.ForEach-遍历顺序不定
        /// </summary>
        /// <param name="args"></param>
        static void Main3(string[] args)
        {
            string[] data = { "str1", "str2", "str3", "str4", "str5" };
            ParallelLoopResult result = Parallel.ForEach<string>(data, (str, state, i) =>
            {
                Console.WriteLine("迭代次数：{0},{1}", i, str);
                if (i > 3)
                    state.Break();
            });
            Console.WriteLine("是否完成:{0}", result.IsCompleted);
            Console.WriteLine("最低迭代:{0}", result.LowestBreakIteration);

            Console.Read();
        }

        /// <summary>
        /// 并行运行方法-Parallel.Invoke-并行任务执行无先后顺序
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Parallel.Invoke(() =>
            {
                Thread.Sleep(100);
                Console.WriteLine("method1");
            }, () =>
            {
                Thread.Sleep(10);
                Console.WriteLine("method2");
            });

            Console.Read();
        }
    }
}
```

我的示例：

```
ParallelLoopResult result= Parallel.ForEach(sims, (sim) =>
            {
                m_list_sim_channel.Add(new SimChannel(sim, (byte)new Random().Next(37)));
            });

            while (true)
            {
                if (result.IsCompleted)
                {
                    Console.WriteLine("m_list_sim_channel count=" + m_list_sim_channel.Count);
                    break;
                }
            }
```


参考文章：[https://www.cnblogs.com/ricky-wang/p/7003162.html](https://www.cnblogs.com/ricky-wang/p/7003162.html)



