# Java 8并发教程：线程和执行程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月25日 12:39:52[boonya](https://me.csdn.net/boonya)阅读数：1667








 原文地址：[http://winterbe.com/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/](http://winterbe.com/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/)



欢迎来到我的Java 8并发教程的第一部分。 本指南教你用Java 8进行[并发编程](http://en.wikipedia.org/wiki/Concurrent_computing) ，并具有易于理解的代码示例。 这是涵盖Java并发API的一系列教程的第一部分。在接下来的15分钟内，您将学习如何通过线程，任务和执行器服务并行执行代码。 
- 第1部分：线程和执行者 
- 第2部分： [同步和锁定](http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/)
- 第3部分： [原子变量和并发图](http://winterbe.com/posts/2015/05/22/java8-concurrency-tutorial-atomic-concurrent-map-examples/)

[并发API](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/package-summary.html)首先引入了Java 5的发行版，然后逐渐增强了每个新的Java版本。 本文中显示的大多数概念也适用于较旧版本的Java。然而我的代码示例着重于Java 8，并大量使用lambda表达式和其他新功能。如果你还不熟悉lambdas，我建议先阅读我的[Java 8 Tutorial](http://winterbe.com/posts/2014/03/16/java-8-tutorial/) 。 

## Threads and Runnables



所有现代操作系统通过[进程](http://en.wikipedia.org/wiki/Process_(computing))和[线程](http://en.wikipedia.org/wiki/Thread_%2528computing%2529)来支持[并发](http://en.wikipedia.org/wiki/Thread_%2528computing%2529) 。进程是通常彼此独立运行的程序的实例，例如，如果您启动一个Java程序，则操作系统会产生一个与其他程序并行运行的新进程。 在这些进程中，我们可以利用线程同时执行代码，因此我们可以充分利用CPU的可用核心。 

Java支持从JDK 1.0开始的[线程](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.html) 。在开始一个新线程之前，你必须指定要由此线程执行的代码，通常称为*任务* 。 这通过实现Runnable - 定义单个void no-args方法run()的功能接口来完成，如以下示例所示： 

```java
Runnable task = () -> {
    String threadName = Thread.currentThread().getName();
    System.out.println("Hello " + threadName);
};

task.run();

Thread thread = new Thread(task);
thread.start();

System.out.println("Done!");
```
由于Runnable是一个功能界面，我们可以使用Java 8 lambda表达式将当前线程名称打印到控制台。 首先，我们在开始一个新线程之前直接在主线程上执行runnable。 
控制台上的结果可能如下所示： 

```
Hello main
Hello Thread-0
Done!
```

或者说：




```java
Hello main
Done!
Hello Thread-0
```

由于并发执行，我们无法预测在打印完成之前或之后是否可以调用runnable。该顺序是非确定性的，因此在大型应用中使并发编程成为一项复杂的任务。 
线程可以进入睡眠一段时间。 这在本文的后续代码示例中模拟长时间运行的任务非常方便： 



```java
unnable runnable = () -> {
    try {
        String name = Thread.currentThread().getName();
        System.out.println("Foo " + name);
        TimeUnit.SECONDS.sleep(1);
        System.out.println("Bar " + name);
    }
    catch (InterruptedException e) {
        e.printStackTrace();
    }
};

Thread thread = new Thread(runnable);
thread.start();
```

当您运行上述代码时，您将注意到第一个和第二个打印语句之间的延迟一秒钟。 TimeUnit是用于处理单位时间的一个有用的枚举。 或者，您可以通过调用Thread.sleep(1000)来实现相同的Thread.sleep(1000)。 
使用Thread类可能非常繁琐，容易出错。由于这个原因， **Concurrency API**已经在2004年被推出了Java 5的发行版。该API位于包java.util.concurrent包java.util.concurrent，并且包含许多有用的类来处理并发编程。从那时起，并发API已经在每个新的Java版本中得到增强，甚至Java 8提供了处理并发性的新类和方法。 

现在让我们深入了解并发API的最重要的部分 - 执行程序服务。 

## Executors

Concurrency API将ExecutorService的概念引入直接处理线程的更高级替换。 执行器能够运行异步任务，通常管理一个线程池，所以我们不必手动创建新的线程。内部池的所有线程将被重新使用在引擎框架下执行任务，因此我们可以在单个执行程序服务的整个生命周期中运行尽可能多的并行任务。 

这是第一个线程示例如何使用执行器： 

```java
ExecutorService executor = Executors.newSingleThreadExecutor();
executor.submit(() -> {
    String threadName = Thread.currentThread().getName();
    System.out.println("Hello " + threadName);
});

// => Hello pool-1-thread-1
```
Executors类提供了方便的工厂方法来创建不同类型的执行器服务。在这个示例中，我们使用一个大小为1的线程池的执行器。 
结果看起来类似于上面的例子，但是运行代码时会发现一个重要的区别：java进程永远不会停止！执行者必须明确停止 - 否则他们会继续监听新任务。 

ExecutorService提供了两种方法： shutdown()等待当前运行的任务完成，而shutdownNow()中断所有运行的任务，并立即关闭执行程序。 

这是我通常关闭执行程序的首选方式： 

```java
try {
    System.out.println("attempt to shutdown executor");
    executor.shutdown();
    executor.awaitTermination(5, TimeUnit.SECONDS);
}
catch (InterruptedException e) {
    System.err.println("tasks interrupted");
}
finally {
    if (!executor.isTerminated()) {
        System.err.println("cancel non-finished tasks");
    }
    executor.shutdownNow();
    System.out.println("shutdown finished");
}
```
执行者通过等待一段时间来终止当前运行的任务，轻轻地关闭。最长五秒钟后，执行程序最终会关闭所有正在运行的任务。 
### Callables and Futures

除了Runnable执行器之外，还支持另一种名为Callable的任务。 可调用函数是像runnable这样的功能接口，但不是void它们返回一个值。 

这个lambda表达式定义了一个可以在休眠1秒后返回整数的可调用函数： 

```java
Callable<Integer> task = () -> {
    try {
        TimeUnit.SECONDS.sleep(1);
        return 123;
    }
    catch (InterruptedException e) {
        throw new IllegalStateException("task interrupted", e);
    }
};
```

 Callable可以像runnable那样提交给执行者服务。但是可调用的结果如何？由于submit()不等待任务完成，所以执行程序服务无法直接返回可调用的结果。相反，执行者返回一个类型为Future的特殊结果，可以用于在稍后的时间点检索实际结果。




```java
ExecutorService executor = Executors.newFixedThreadPool(1);
Future<Integer> future = executor.submit(task);

System.out.println("future done? " + future.isDone());

Integer result = future.get();

System.out.println("future done? " + future.isDone());
System.out.print("result: " + result);
```

在提交可执行程序后，我们首先检查未来是否已经通过isDone()完成执行。我很确定不是这样，因为上面的可调用休眠一秒钟后返回整数。 
调用方法get()阻止当前线程，并等待直到可执行完成，然后返回实际结果123。 现在未来终于完成了，我们在控制台上看到如下结果： 

```
future done? false
future done? true
result: 123
```

期货与基础执行者服务紧密相连。请记住，如果关闭执行程序，每个未终止的未来将抛出异常： 




```java
executor.shutdownNow();
future.get();
```

您可能已经注意到执行器的创建与前面的示例略有不同。 我们使用newFixedThreadPool(1)创建由大小为1的线程池支持的执行器服务。 这相当于newSingleThreadExecutor()但是稍后可以通过传递大于1的值来增加池大小。 
### Timeouts

对future.get()任何调用将阻止并等待底层可调用终止。在最坏的情况下，可以永久运行 - 从而使您的应用程序无响应。 您可以通过传递超时来简单地抵制这些场景： 

```java
ExecutorService executor = Executors.newFixedThreadPool(1);

Future<Integer> future = executor.submit(() -> {
    try {
        TimeUnit.SECONDS.sleep(2);
        return 123;
    }
    catch (InterruptedException e) {
        throw new IllegalStateException("task interrupted", e);
    }
});

future.get(1, TimeUnit.SECONDS);
```

执行上述代码会导致TimeoutException：




```java
Exception in thread "main" java.util.concurrent.TimeoutException
    at java.util.concurrent.FutureTask.get(FutureTask.java:205)
```

您可能已经猜到为什么会抛出此异常：我们指定了一秒钟的最大等待时间，但是可调用实际上需要两秒钟才能返回结果。 
###  invokeAll

执行者支持通过invokeAll()一次性批量提交多个可转换。 此方法接受可收回的集合并返回期货列表。 

```java
ExecutorService executor = Executors.newWorkStealingPool();

List<Callable<String>> callables = Arrays.asList(
        () -> "task1",
        () -> "task2",
        () -> "task3");

executor.invokeAll(callables)
    .stream()
    .map(future -> {
        try {
            return future.get();
        }
        catch (Exception e) {
            throw new IllegalStateException(e);
        }
    })
    .forEach(System.out::println);
```
在这个例子中，我们使用Java 8函数流来处理调用invokeAll返回的所有期货。我们首先将每个未来映射到其返回值，然后将每个值打印到控制台。 If you're not yet familiar with streams read my [Java 8 Stream Tutorial](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://winterbe.com/posts/2014/07/31/java8-stream-tutorial-examples/&usg=ALkJrhjS8818FZshXFtMLHLh1D3RD4h9iA) .如果您还不熟悉流，请阅读我的[Java 8 Stream Tutorial](http://winterbe.com/posts/2014/07/31/java8-stream-tutorial-examples/) 。 
### InvokeAny 

批量提交可调用的另一种方法是invokeAny()与invokeAny()的方法略有不同。 而不是返回将来的对象，此方法阻止直到第一个可调用终止并返回该可调用的结果。 

为了测试这种行为，我们使用这个帮助方法来模拟不同持续时间的可调用。 该方法返回一个可以调用一段时间的可调用函数，直到返回给定的结果： 

```java
Callable<String> callable(String result, long sleepSeconds) {
    return () -> {
        TimeUnit.SECONDS.sleep(sleepSeconds);
        return result;
    };
}
```

我们使用这种方法来创建一堆不等于1秒到3秒的可调用。通过invokeAny()将执行者提交给执行者返回最快的可调用的字符串结果 - 在这种情况下task2：



```java
ExecutorService executor = Executors.newWorkStealingPool();

List<Callable<String>> callables = Arrays.asList(
    callable("task1", 2),
    callable("task2", 1),
    callable("task3", 3));

String result = executor.invokeAny(callables);
System.out.println(result);

// => task2
```

上面的例子使用了另一种类型的通过newWorkStealingPool()创建的执行器。 此工厂方法是Java 8的一部分，并返回ForkJoinPool的执行程序，该执行程序与正常执行程序略有不同。而不是使用固定大小的线程池[ForkJoinPools](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/ForkJoinPool.html)是为给定并行度大小创建的，默认值是主机CPU可用核心数。 
 ForkJoinPools存在于Java 7之后，将在本系列的后续教程中详细介绍。通过深入了解计划的执行人员，完成本教程。 

## Scheduled Executors

我们已经学会了如何在执行者身上提交和运行一次任务。为了多次定期运行通用任务，我们可以利用预定的线程池。 

ScheduledExecutorService能够调度任务在经过一段时间后定期运行或一次运行。 

此代码示例在通过三秒钟的初始延迟后安排一个任务运行： 

```java
ScheduledExecutorService executor = Executors.newScheduledThreadPool(1);

Runnable task = () -> System.out.println("Scheduling: " + System.nanoTime());
ScheduledFuture<?> future = executor.schedule(task, 3, TimeUnit.SECONDS);

TimeUnit.MILLISECONDS.sleep(1337);

long remainingDelay = future.getDelay(TimeUnit.MILLISECONDS);
System.out.printf("Remaining Delay: %sms", remainingDelay);
```
计划一个任务会产生一个ScheduledFuture类型的专门的未来，除了Future，还提供getDelay()方法来检索剩余的延迟。 在此延迟过后，任务将同时执行。 
为了定期执行任务执行，执行器提供了scheduleAtFixedRate()和scheduleWithFixedDelay()两种方法。 第一种方法能够以固定的时间速率执行任务，例如每秒一次，如本例所示： 



```java
ScheduledExecutorService executor = Executors.newScheduledThreadPool(1);

Runnable task = () -> System.out.println("Scheduling: " + System.nanoTime());

int initialDelay = 0;
int period = 1;
executor.scheduleAtFixedRate(task, initialDelay, period, TimeUnit.SECONDS);
```

此外，该方法接受一个初始延迟，它描述了第一次执行任务之前的前导等待时间。 
请记住， scheduleAtFixedRate()不考虑任务的实际持续时间。 因此，如果您指定一秒钟的时间段，但是任务需要2秒钟才能执行，那么线程池将很快工作。 

在这种情况下，您应考虑使用scheduleWithFixedDelay()。 该方法的工作方式与上述对应方法一样。不同的是，等待时间段适用于任务的结束和下一个任务的开始之间。 For example:例如： 

```java
ScheduledExecutorService executor = Executors.newScheduledThreadPool(1);

Runnable task = () -> {
    try {
        TimeUnit.SECONDS.sleep(2);
        System.out.println("Scheduling: " + System.nanoTime());
    }
    catch (InterruptedException e) {
        System.err.println("task interrupted");
    }
};

executor.scheduleWithFixedDelay(task, 0, 1, TimeUnit.SECONDS);
```
该示例在执行结束和下一次执行的开始之间以固定的延迟时间安排一秒。 初始延迟为零，任务持续时间为2秒。 所以我们的执行时间间隔为0s，3s，6s，9s等等。正如你可以看到scheduleWithFixedDelay()如果您无法预测计划的任务的持续时间，这是方便的。 
这是一系列并发教程中的第一部分。我建议您自己修理显示的代码示例。 您可以在[GitHub上](https://github.com/winterbe/java8-tutorial)找到本文中的所有代码示例，因此请随意分配repo并[给我一个星](https://github.com/winterbe/java8-tutorial/stargazers) 。 

我希望你喜欢这篇文章。 如果您有任何其他问题，请通过以下评论或通过[Twitter](https://twitter.com/winterbe_)向我发送反馈意见。 





