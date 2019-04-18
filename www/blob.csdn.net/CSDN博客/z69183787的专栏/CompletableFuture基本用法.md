# CompletableFuture基本用法 - z69183787的专栏 - CSDN博客
2018年12月04日 16:54:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：88
[https://www.cnblogs.com/cjsblog/p/9267163.html](https://www.cnblogs.com/cjsblog/p/9267163.html)
## 异步计算
- 
所谓异步调用其实就是实现一个可无需等待被调用函数的返回值而让操作继续运行的方法。在 Java 语言中，简单的讲就是另启一个线程来完成调用中的部分计算，使调用继续运行或返回，而不需要等待计算结果。但调用者仍需要取线程的计算结果。
- 
JDK5新增了Future接口，用于描述一个异步计算的结果。虽然 Future 以及相关使用方法提供了异步执行任务的能力，但是对于结果的获取却是很不方便，只能通过阻塞或者轮询的方式得到任务的结果。阻塞的方式显然和我们的异步编程的初衷相违背，轮询的方式又会耗费无谓的 CPU 资源，而且也不能及时地得到计算结果。
- 以前我们获取一个异步任务的结果可能是这样写的
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705101315432-1126516064.png)
## Future 接口的局限性
Future接口可以构建异步应用，但依然有其局限性。它很难直接表述多个Future 结果之间的依赖性。实际开发中，我们经常需要达成以下目的：
- 
将多个异步计算的结果合并成一个
- 
等待Future集合中的所有任务都完成
- 
Future完成事件（即，任务完成以后触发执行动作）
- 。。。
## 函数式编程
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705101757529-501947925.png)
## CompletionStage
- 
CompletionStage代表异步计算过程中的某一个阶段，一个阶段完成以后可能会触发另外一个阶段
- 
一个阶段的计算执行可以是一个Function，Consumer或者Runnable。比如：stage.thenApply(x -> square(x)).thenAccept(x -> System.out.print(x)).thenRun(() -> System.out.println())
- 
一个阶段的执行可能是被单个阶段的完成触发，也可能是由多个阶段一起触发
## CompletableFuture
- 在Java8中，CompletableFuture提供了非常强大的Future的扩展功能，可以帮助我们简化异步编程的复杂性，并且提供了函数式编程的能力，可以通过回调的方式处理计算结果，也提供了转换和组合 CompletableFuture 的方法。
- 它可能代表一个明确完成的Future，也有可能代表一个完成阶段（ CompletionStage ），它支持在计算完成以后触发一些函数或执行某些动作。
- 它实现了Future和CompletionStage接口
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705101948324-1632732382.png)
## CompletableFuture基本用法
### 创建CompletableFuture
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705102457655-746001730.png)
### thenApply
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705102425547-1470669451.png)
当前阶段正常完成以后执行，而且当前阶段的执行的结果会作为下一阶段的输入参数。thenApplyAsync默认是异步执行的。这里所谓的异步指的是不在当前线程内执行。
### thenApply相当于回调函数（callback）
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705102626473-2144280533.png)
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705102634268-872166579.png)
### thenAccept与thenRun
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705102741301-1725796593.png)
- 可以看到，thenAccept和thenRun都是无返回值的。如果说thenApply是不停的输入输出的进行生产，那么thenAccept和thenRun就是在进行消耗。它们是整个计算的最后两个阶段。
- 
同样是执行指定的动作，同样是消耗，二者也有区别：
- 
thenAccept接收上一阶段的输出作为本阶段的输入 　　
- thenRun根本不关心前一阶段的输出，根本不不关心前一阶段的计算结果，因为它不需要输入参数
### thenCombine整合两个计算结果
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705103032777-345776723.png)
例如，此阶段与其它阶段一起完成，进而触发下一阶段：
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705103453917-1456567599.png)
### whenComplete
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705103422923-230222107.png)
## 最后，举个栗子：
> 
![](https://images2018.cnblogs.com/blog/874963/201807/874963-20180705103640694-1689901551.png)
事实上，如果每个操作都很简单的话（比如：上面的例子中按照id去查）没有必要用这种多线程异步的方式，因为创建线程还需要时间，还不如直接同步执行来得快。
事实证明，只有当每个操作很复杂需要花费相对很长的时间（比如，调用多个其它的系统的接口；比如，商品详情页面这种需要从多个系统中查数据显示的）的时候用CompletableFuture才合适，不然区别真的不大，还不如顺序同步执行。
