
# Parallel Stop与Break的深入解析 - 追求卓越,做到专业 - CSDN博客


2018年08月27日 11:21:11[Waldenz](https://me.csdn.net/enter89)阅读数：115


普通for循环、foreach循环中有break/continue用于流程控制，在Parallel中也有用于流程控制的方法，即ParallelLoopState类的Stop/Break方法。但下问要证明的是Parallel中的Stop/Break方法和普通循环中的break/continue的区别。
首先给大家看一个错误的说法：
来源[https://www.cnblogs.com/huatao/p/4702049.html](https://www.cnblogs.com/huatao/p/4702049.html)
![](https://img-blog.csdn.net/20180827110231305?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
首先给出正确的解释：
**ParallelLoopState.Break()：在完成当前的这轮工作之后，不再执行后继的工作，但在当前这轮工作开始之前“已经在执行”的工作，则必须完成。但并不能执行完所有的循环。**
**ParallelLoopState.Stop：不但不会再创建新的线程执行并行循环，而且当前“已经在执行”的工作也应该被中止。**
**两种方法都不会完全执行完循环，所以Break()方法不能等同于普通循环中的continue.**
详细代码解析如下：
break方法
```python
Parallel.For(1, 10,
                   new ParallelOptions { MaxDegreeOfParallelism = 4 },
                   (i, loopState) =>
                   {
                       Console.WriteLine(i);
                       if (i % 3 == 0)
                       {
                           loopState.Break();
                           Console.WriteLine($"Selected Number:{i}");
                       }
                   });
```
结果
![](https://img-blog.csdn.net/20180827110752847?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Stop方法：
```python
Parallel.For(1, 10,
                   new ParallelOptions { MaxDegreeOfParallelism = 4 },
                   (i, loopState) =>
                   {
                       Console.WriteLine(i);
                       if (i % 3 == 0)
                       {
                           loopState.Stop();
                           Console.WriteLine($"Selected Number:{i}");
                       }
                   });
```
![](https://img-blog.csdn.net/20180827111023600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
或者另外一种可以正面的方法，让将MaxDegreeofparallelism设置为1，即最大并行度1，也就相当于普通循环
将下列代码中的Stop方法替换为Break，运行结果完全相同。由此证明Parallel中的Break和Stop都不会继续代码循环。
`Parallel.For(1, 10,
                   new ParallelOptions { MaxDegreeOfParallelism = 1 },
                   (i, loopState) =>
                   {
                       Console.WriteLine(i);
                       if (i % 3 == 0)
                       {
                           loopState.Stop();
                           Console.WriteLine($"Selected Number:{i}");
                       }
                   });`如果非要找一个和普通循环中continue同义的用法，也只能是return
return: 退出当前执行的循环项，继续执行下一项
`Parallel.For(1, 10,
                   new ParallelOptions { MaxDegreeOfParallelism = 3 },
                   (i, loopState) =>
                   {
                       Console.WriteLine(i);
                       if (i % 3 == 0)
                       {
                           return;
                           Console.WriteLine($"Selected Number:{i}");
                       }
                   });`![](https://img-blog.csdn.net/20180827112012411?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


