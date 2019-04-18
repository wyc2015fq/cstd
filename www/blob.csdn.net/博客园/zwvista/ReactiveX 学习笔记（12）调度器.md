# ReactiveX 学习笔记（12）调度器 - zwvista - 博客园

## [ReactiveX 学习笔记（12）调度器](https://www.cnblogs.com/zwvista/p/9394105.html)

### Schedulers, threading and testing

本文的主题为调度器，多线程以及测试。

[RxJava操作符（六）Utility](http://mushuichuan.com/2015/12/11/rxjava-operator-6/)

### SubscribeOn / ObserveOn
- SubscribeOn / ObserveOn 用来指定数据流和观察者所运行的线程。

这里数据流的运行是指数据流创建，转化，发送数据以及最后销毁的全过程。

这里观察者的运行是指数据流发送数据和观察者观察数据的过程。

SubscribeOn / ObserveOn 指定线程是通过指定调度器来完成的。
- SubscribeOn 同时指定数据流和观察者所运行的线程。

SubscribeOn 截获对 Subscribe 及其之后的 Dispose 方法的调用，将其转化为异步操作。
- ObserveOn 仅限于指定观察者运行的线程。

ObserveOn 截获对 OnNext OnCompleted OnError 方法的调用，将其转化为异步操作。

也就是说 ObserveOn 指定 Subscribe 方法内 3 个回调函数在哪个线程上运行。
- SubscribeOn 指定的范围大，同时使用的情况下 ObserveOn 指定的优先度高。

[ReactiveX - SubscribeOn operator](http://reactivex.io/documentation/operators/subscribeon.html)
[ReactiveX - ObserveOn operator](http://reactivex.io/documentation/operators/observeon.html)
[ObserveOn and SubscribeOn - where the work is being done](https://stackoverflow.com/questions/20451939/observeon-and-subscribeon-where-the-work-is-being-done)

![SubscribeOn](http://reactivex.io/documentation/operators/images/subscribeOn.c.png)
![ObserveOn](http://reactivex.io/documentation/operators/images/observeOn.c.png)
![Schedulers](http://reactivex.io/documentation/operators/images/schedulers.png)
- RxNET
- 缺省情况下，数据流以及观察者运行的线程均与 Subscribe 的调用者线程相同。

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
Console.WriteLine("Starting on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
var source = Observable.Create<int>(
o =>
{
    Console.WriteLine("Invoked on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    o.OnNext(1);
    o.OnNext(2);
    o.OnNext(3);
    o.OnCompleted();
    Console.WriteLine("Finished on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
});
source
.Subscribe(
o => Console.WriteLine("Received {1} on threadId:{0}", Thread.CurrentThread.ManagedThreadId,
o),
() => Console.WriteLine("OnCompleted on threadId:{0}", Thread.CurrentThread.ManagedThreadId));
Console.WriteLine("Subscribed on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
/*
Starting on threadId:1
Invoked on threadId:1
Received 1 on threadId:1
Received 2 on threadId:1
Received 3 on threadId:1
OnCompleted on threadId:1
Finished on threadId:1
Subscribed on threadId:1
*/
```
- SubscribeOn

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
Console.WriteLine("Starting on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
var source = Observable.Create<int>(
o =>
{
    Console.WriteLine("Invoked on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    o.OnNext(1);
    o.OnNext(2);
    o.OnNext(3);
    o.OnCompleted();
    Console.WriteLine("Finished on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
});
source
.SubscribeOn(Scheduler.Default)
.Subscribe(
o => Console.WriteLine("Received {1} on threadId:{0}", Thread.CurrentThread.ManagedThreadId,
o),
() => Console.WriteLine("OnCompleted on threadId:{0}", Thread.CurrentThread.ManagedThreadId));
Console.WriteLine("Subscribed on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
/*
Starting on threadId:1
Subscribed on threadId:1
Invoked on threadId:4
Received 1 on threadId:4
Received 2 on threadId:4
Received 3 on threadId:4
OnCompleted on threadId:4
Finished on threadId:4
*/
```
- ObserveOn

```
Console.WriteLine("Starting on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
var source = Observable.Create<int>(
o =>
{
    Console.WriteLine("Invoked on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    o.OnNext(1);
    o.OnNext(2);
    o.OnNext(3);
    o.OnCompleted();
    Console.WriteLine("Finished on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
});
source
.ObserveOn(Scheduler.Default)
.Subscribe(
o => Console.WriteLine("Received {1} on threadId:{0}", Thread.CurrentThread.ManagedThreadId,
o),
() => Console.WriteLine("OnCompleted on threadId:{0}", Thread.CurrentThread.ManagedThreadId));
Console.WriteLine("Subscribed on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
/*
Starting on threadId:1
Invoked on threadId:1
Finished on threadId:1
Subscribed on threadId:1
Received 1 on threadId:6
Received 2 on threadId:6
Received 3 on threadId:6
OnCompleted on threadId:6
*/
```
- SubscribeOn + ObserveOn

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
Console.WriteLine("Starting on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
var source = Observable.Create<int>(
o =>
{
    Console.WriteLine("Invoked on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    o.OnNext(1);
    o.OnNext(2);
    o.OnNext(3);
    o.OnCompleted();
    Console.WriteLine("Finished on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
});
source
.SubscribeOn(Scheduler.Default)
.ObserveOn(Scheduler.Default)
.Subscribe(
o => Console.WriteLine("Received {1} on threadId:{0}", Thread.CurrentThread.ManagedThreadId,
o),
() => Console.WriteLine("OnCompleted on threadId:{0}", Thread.CurrentThread.ManagedThreadId));
Console.WriteLine("Subscribed on threadId:{0}", Thread.CurrentThread.ManagedThreadId);
/*
Starting on threadId:1
Subscribed on threadId:1
Invoked on threadId:4
Finished on threadId:4
Received 1 on threadId:7
Received 2 on threadId:7
Received 3 on threadId:7
OnCompleted on threadId:7
*/
```
- RxJava

```
val subject = BehaviorSubject.create<Int>()
subject.subscribe { i -> println("Received $i on ${Thread.currentThread().id}") }
val i = intArrayOf(1) // naughty side-effects for examples only ;)
val r = Runnable {
    synchronized(i) {
        println("onNext(${i[0]}) on ${Thread.currentThread().id}")
        subject.onNext(i[0]++)
    }
}
r.run() // Execute on main thread
Thread(r).start()
Thread(r).start()
/*
onNext(1) on 1
Received 1 on 1
onNext(2) on 11
Received 2 on 11
onNext(3) on 12
Received 3 on 12
*/
```

```
println("Main: ${Thread.currentThread().id}")
Observable.create<Int> { o ->
    println("Created on ${Thread.currentThread().id}")
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
}
    .subscribe { i -> println("Received $i on ${Thread.currentThread().id}") }
println("Finished main: ${Thread.currentThread().id}")
/*
Main: 1
Created on 1
Received 1 on 1
Received 2 on 1
Finished main: 1
*/
```

```
println("Main: ${Thread.currentThread().id}")
Observable.create<Int> { o ->
    println("Created on ${Thread.currentThread().id}")
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
}
    .subscribeOn(Schedulers.newThread())
    .subscribe { i -> println("Received $i on ${Thread.currentThread().id}") }
println("Finished main: ${Thread.currentThread().id}")
/*
Main: 1
Finished main: 1
Created on 15
Received 1 on 15
Received 2 on 15
*/
```

```
println("Main: ${Thread.currentThread().id}")
val s = Observable.interval(100, TimeUnit.MILLISECONDS)
    .subscribe { i -> println("Received $i on ${Thread.currentThread().id}") }
println("Finished main: ${Thread.currentThread().id}")
readLine()
s.dispose()
/*
Main: 1
Finished main: 1
Received 0 on 16
Received 1 on 16
Received 2 on 16
Received 3 on 16
...
*/
```

```
Observable.create<Int> { o ->
    println("Created on ${Thread.currentThread().id}")
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
}
    .observeOn(Schedulers.newThread())
    .subscribe { i -> println("Received $i on ${Thread.currentThread().id}") }
/*
Created on 1
Received 1 on 17
Received 2 on 17
*/
```

```
Observable.create<Int> { o ->
    println("Created on " + Thread.currentThread().id)
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
}
    .doOnNext { i -> println("Before $i on ${Thread.currentThread().id}") }
    .observeOn(Schedulers.newThread())
    .doOnNext { i -> println("After $i on ${Thread.currentThread().id}") }
    .subscribe()
/*
Created on 1
Before 1 on 1
Before 2 on 1
After 1 on 18
After 2 on 18
*/
```

### 调度器的使用
- RxNET

状态

```
var scheduler = NewThreadScheduler.Default;
var myName = "Lee";
scheduler.Schedule(
() => Console.WriteLine("myName = {0}", myName));
myName = "John";
/*
myName = John
*/
```

```
var scheduler = ImmediateScheduler.Instance;
var myName = "Lee";
scheduler.Schedule(
() => Console.WriteLine("myName = {0}", myName));
myName = "John";
/*
myName = Lee
*/
```

```
var scheduler = NewThreadScheduler.Default;
var myName = "Lee";
scheduler.Schedule(myName,
(_, state) =>
{
    Console.WriteLine("myName = {0}", state);
    return Disposable.Empty;
});
myName = "John";
/*
myName = Lee
*/
```

```
var scheduler = ImmediateScheduler.Instance;
var myName = "Lee";
scheduler.Schedule(myName,
(_, state) =>
{
    Console.WriteLine("myName = {0}", state);
    return Disposable.Empty;
});
myName = "John";
/*
myName = Lee
*/
```

```
var scheduler = NewThreadScheduler.Default;
var list = new List<int>();
scheduler.Schedule(list,
(innerScheduler, state) =>
{
    Console.WriteLine(state.Count);
    return Disposable.Empty;
});
list.Add(1);
/*
1
*/
```

```
var scheduler = ImmediateScheduler.Instance;
var list = new List<int>();
scheduler.Schedule(list,
(innerScheduler, state) =>
{
    Console.WriteLine(state.Count);
    return Disposable.Empty;
});
list.Add(1);
/*
0
*/
```

时间

```
var scheduler = ImmediateScheduler.Instance;
var delay = TimeSpan.FromSeconds(1);
Console.WriteLine("Before schedule at {0:o}", DateTime.Now);
scheduler.Schedule(delay,
() => Console.WriteLine("Inside schedule at {0:o}", DateTime.Now));
Console.WriteLine("After schedule at  {0:o}", DateTime.Now);
Console.ReadKey();
/*
Before schedule at 2018-08-01T11:20:26.0042252+09:00
Inside schedule at 2018-08-01T11:20:27.0232835+09:00
After schedule at  2018-08-01T11:20:27.0232835+09:00
*/
```

取消任务

```
var scheduler = NewThreadScheduler.Default;
var delay = TimeSpan.FromSeconds(1);
Console.WriteLine("Before schedule at {0:o}", DateTime.Now);
var token = scheduler.Schedule(delay,
() => Console.WriteLine("Inside schedule at {0:o}", DateTime.Now));
Console.WriteLine("After schedule at  {0:o}", DateTime.Now);
token.Dispose();
Console.ReadKey();
/*
Before schedule at 2018-08-01T11:20:27.9713377+09:00
After schedule at  2018-08-01T11:20:27.9743379+09:00
*/
```

```
public static IDisposable Work(IScheduler scheduler, List<int> list)
{
    var tokenSource = new CancellationTokenSource();
    var cancelToken = tokenSource.Token;
    var task = new Task(() =>
    {
        Console.WriteLine();
        for (int i = 0; i < 1000; i++)
        {
            var sw = new SpinWait();
            for (int j = 0; j < 3000; j++) sw.SpinOnce();
            Console.Write(".");
            list.Add(i);
            if (cancelToken.IsCancellationRequested)
            {
                Console.WriteLine("Cancelation requested");
                //cancelToken.ThrowIfCancellationRequested();
                return;
            }
        }
    }, cancelToken);
    task.Start();
    return Disposable.Create(tokenSource.Cancel);
}
var scheduler = NewThreadScheduler.Default;
var list = new List<int>();
Console.WriteLine("Enter to quit:");
var token = scheduler.Schedule(list, Work);
Console.ReadLine();
Console.WriteLine("Cancelling...");
token.Dispose();
Console.WriteLine("Cancelled");
/*
Enter to quit:

...........
Cancelling...
Cancelled
.Cancelation requested
*/
```

递归

```
var scheduler = NewThreadScheduler.Default;
Action<Action> work = (Action self)
=>
{
    Console.WriteLine("Running");
    self();
};
Console.WriteLine("Enter to quit:");
var token = scheduler.Schedule(work);
Console.ReadLine();
Console.WriteLine("Cancelling");
token.Dispose();
Console.WriteLine("Cancelled");
/*
Enter to quit:
Running
Running
Running
Running
Cancelling
Running
Running
Cancelled
*/
```
- RxJava

```
val scheduler = Schedulers.trampoline()
val worker = scheduler.createWorker()
worker.schedule(
    { println("Action") })
/*
Action
*/
```

```
val scheduler = Schedulers.newThread()
val start = System.currentTimeMillis()
val worker = scheduler.createWorker()
worker.schedule(
    { println(System.currentTimeMillis() - start) },
    5, TimeUnit.SECONDS)
worker.schedule(
    { println(System.currentTimeMillis() - start) },
    5, TimeUnit.SECONDS)
/*
5005
5005
*/
```

```
val scheduler = Schedulers.newThread()
val start = System.currentTimeMillis()
val worker = scheduler.createWorker()
worker.schedule(
    {
        println(System.currentTimeMillis() - start)
        worker.dispose()
    },
    5, TimeUnit.SECONDS)
worker.schedule(
    { println(System.currentTimeMillis() - start) },
    5, TimeUnit.SECONDS)
/*
5001
*/
```

```
val scheduler = Schedulers.newThread()
val worker = scheduler.createWorker()
worker.schedule {
    try {
        Thread.sleep(2000)
        println("Action completed")
    } catch (e: InterruptedException) {
        println("Action interrupted")
    }
}
Thread.sleep(500)
worker.dispose()
/*
Action interrupted
*/
```

### 调度器的种类
- ImmediateScheduler
- CurrentThreadScheduler
- RxNET

```
private static void ScheduleTasks(IScheduler scheduler)
{
    Action leafAction = () => Console.WriteLine("----leafAction.");
    Action innerAction = () =>
    {
        Console.WriteLine("--innerAction start.");
        scheduler.Schedule(leafAction);
        Console.WriteLine("--innerAction end.");
    };
    Action outerAction = () =>
    {
        Console.WriteLine("outer start.");
        scheduler.Schedule(innerAction);
        Console.WriteLine("outer end.");
    };
    scheduler.Schedule(outerAction);
}
ScheduleTasks(Scheduler.CurrentThread);
/*
outer start.
outer end.
--innerAction start.
--innerAction end.
----leafAction.
*/
ScheduleTasks(Scheduler.Immediate);
/*
outer start.
--innerAction start.
----leafAction.
--innerAction end.
outer end.
*/
```
- RxJava

Schedulers.immediate() 被废除了。

```
val scheduler = Schedulers.trampoline()
val worker = scheduler.createWorker()
worker.schedule {
    println("Start")
    worker.schedule { println("Inner") }
    println("End")
}
/*
Start
End
Inner
*/
```
- DispatcherScheduler
- EventLoopScheduler
- NewThreadScheduler
- ThreadPoolScheduler
- TaskPoolScheduler
- RxNET

```
private static IDisposable OuterAction(IScheduler scheduler, string state)
{
    Console.WriteLine("{0} start. ThreadId:{1}",
    state,
    Thread.CurrentThread.ManagedThreadId);
    scheduler.Schedule(state + ".inner", InnerAction);
    Console.WriteLine("{0} end. ThreadId:{1}",
    state,
    Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
}
private static IDisposable InnerAction(IScheduler scheduler, string state)
{
    Console.WriteLine("{0} start. ThreadId:{1}",
    state,
    Thread.CurrentThread.ManagedThreadId);
    scheduler.Schedule(state + ".Leaf", LeafAction);
    Console.WriteLine("{0} end. ThreadId:{1}",
    state,
    Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
}
private static IDisposable LeafAction(IScheduler scheduler, string state)
{
    Console.WriteLine("{0}. ThreadId:{1}",
    state,
    Thread.CurrentThread.ManagedThreadId);
    return Disposable.Empty;
}
Console.WriteLine("Starting on thread :{0}", Thread.CurrentThread.ManagedThreadId);
NewThreadScheduler.Default.Schedule("A", OuterAction);
NewThreadScheduler.Default.Schedule("B", OuterAction);
/*
Starting on thread :1
A start. ThreadId:13
A end. ThreadId:13
A.inner start. ThreadId:13
A.inner end. ThreadId:13
B start. ThreadId:14
B end. ThreadId:14
B.inner start. ThreadId:14
B.inner end. ThreadId:14
B.inner.Leaf. ThreadId:14
A.inner.Leaf. ThreadId:13
*/
Console.WriteLine("Starting on thread :{0}", Thread.CurrentThread.ManagedThreadId);
ThreadPoolScheduler.Instance.Schedule("A", OuterAction);
ThreadPoolScheduler.Instance.Schedule("B", OuterAction);
/*
Starting on thread :1
A start. ThreadId:5
A end. ThreadId:5
A.inner start. ThreadId:5
A.inner end. ThreadId:5
A.inner.Leaf. ThreadId:5
B start. ThreadId:4
B end. ThreadId:4
B.inner start. ThreadId:5
B.inner end. ThreadId:5
B.inner.Leaf. ThreadId:4
*/
Console.WriteLine("Starting on thread :{0}", Thread.CurrentThread.ManagedThreadId);
TaskPoolScheduler.Default.Schedule("A", OuterAction);
TaskPoolScheduler.Default.Schedule("B", OuterAction);
/*
Starting on thread :1
A start. ThreadId:4
A end. ThreadId:4
A.inner start. ThreadId:15
A.inner end. ThreadId:15
A.inner.Leaf. ThreadId:4
B start. ThreadId:5
B end. ThreadId:5
B.inner start. ThreadId:15
B.inner end. ThreadId:15
B.inner.Leaf. ThreadId:4
*/
```
- RxJava

```
private fun printThread(message: String) {
    println("$message on ${Thread.currentThread().id}")
}
printThread("Main")
val scheduler = Schedulers.newThread()
val worker = scheduler.createWorker()
worker.schedule {
    printThread("Start")
    worker.schedule { printThread("Inner") }
    printThread("End")
}
Thread.sleep(500)
worker.schedule { printThread("Again") }
/*
Main on 1
Start on 13
End on 13
Inner on 13
Again on 13
*/
```
- TestScheduler

### 测试调度器
- RxNET

```
var scheduler = new TestScheduler();
var wasExecuted = false;
scheduler.Schedule(() => wasExecuted = true);
Console.WriteLine(wasExecuted); // False
scheduler.AdvanceBy(1); //execute 1 tick of queued actions
Console.WriteLine(wasExecuted); // True
/*
False
True
*/
```

```
var scheduler = new TestScheduler();
scheduler.Schedule(() => Console.WriteLine("A")); //Schedule immediately
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(20), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.AdvanceTo(1);");
scheduler.AdvanceTo(1);
Console.WriteLine("scheduler.AdvanceTo(10);");
scheduler.AdvanceTo(10);
Console.WriteLine("scheduler.AdvanceTo(15);");
scheduler.AdvanceTo(15);
Console.WriteLine("scheduler.AdvanceTo(20);");
scheduler.AdvanceTo(20);
/*
scheduler.AdvanceTo(1);
A
scheduler.AdvanceTo(10);
B
scheduler.AdvanceTo(15);
scheduler.AdvanceTo(20);
C
*/
```

```
var scheduler = new TestScheduler();
scheduler.Schedule(() => Console.WriteLine("A")); //Schedule immediately
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(20), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.AdvanceBy(1);");
scheduler.AdvanceBy(1);
Console.WriteLine("scheduler.AdvanceBy(9);");
scheduler.AdvanceBy(9);
Console.WriteLine("scheduler.AdvanceBy(5);");
scheduler.AdvanceBy(5);
Console.WriteLine("scheduler.AdvanceBy(5);");
scheduler.AdvanceBy(5);
/*
scheduler.AdvanceBy(1);
A
scheduler.AdvanceBy(9);
B
scheduler.AdvanceBy(5);
scheduler.AdvanceBy(5);
C
*/
```

```
var scheduler = new TestScheduler();
scheduler.Schedule(() => Console.WriteLine("A")); //Schedule immediately
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(20), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.Start();");
scheduler.Start();
Console.WriteLine("scheduler.Clock:{0}", scheduler.Clock);
/*
scheduler.Start();
A
B
C
scheduler.Clock:20
*/
```

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
var scheduler = new TestScheduler();
scheduler.Schedule(() => Console.WriteLine("A"));
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(20), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.Start();");
scheduler.Start();
Console.WriteLine("scheduler.Clock:{0}", scheduler.Clock);
scheduler.Schedule(() => Console.WriteLine("D"));
scheduler.Start();
Console.WriteLine("scheduler.Clock:{0}", scheduler.Clock);
/*
scheduler.Start();
A
B
C
scheduler.Clock:20
D
scheduler.Clock:21
*/
```

```
var scheduler = new TestScheduler();
scheduler.Schedule(() => Console.WriteLine("A"));
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(15), scheduler.Stop);
scheduler.Schedule(TimeSpan.FromTicks(20), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.Start();");
scheduler.Start();
Console.WriteLine("scheduler.Clock:{0}", scheduler.Clock);
/*
scheduler.Start();
A
B
scheduler.Clock:15
*/
```

```
var scheduler = new TestScheduler();
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("A"));
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("B"));
scheduler.Schedule(TimeSpan.FromTicks(10), () => Console.WriteLine("C"));
Console.WriteLine("scheduler.Start();");
scheduler.Start();
Console.WriteLine("scheduler.Clock:{0}", scheduler.Clock);
/*
scheduler.Start();
A
B
C
scheduler.Clock:10
*/
```

```
var expectedValues = new long[] { 0, 1, 2, 3, 4 };
var actualValues = new List<long>();
var scheduler = new TestScheduler();
var interval = Observable
.Interval(TimeSpan.FromSeconds(1), scheduler)
.Take(5);
interval.Subscribe(actualValues.Add);
scheduler.Start();
Console.WriteLine(Enumerable.SequenceEqual(expectedValues, actualValues));
/*
True
*/
```

```
var scheduler = new TestScheduler();
var never = Observable.Never<int>();
var exceptionThrown = false;
never.Timeout(TimeSpan.FromMinutes(1), scheduler)
.Subscribe(
i => Console.WriteLine("This will never run."),
ex => exceptionThrown = true);
scheduler.Start();
Console.WriteLine(exceptionThrown);
/*
True
*/
```

```
var scheduler = new TestScheduler();
var source = Observable.Interval(TimeSpan.FromSeconds(1), scheduler)
.Take(4);
var testObserver = scheduler.Start(
() => source,
0,
0,
TimeSpan.FromSeconds(5).Ticks);
Console.WriteLine("Time is {0} ticks", scheduler.Clock);
Console.WriteLine("Received {0} notifications", testObserver.Messages.Count);
foreach (Recorded<Notification<long>> message in testObserver.Messages)
{
    Console.WriteLine("{0} @ {1}", message.Value, message.Time);
}
/*
Time is 50000000 ticks
Received 5 notifications
OnNext(0) @ 10000001
OnNext(1) @ 20000001
OnNext(2) @ 30000001
OnNext(3) @ 40000001
OnCompleted() @ 40000001
*/
```

```
var scheduler = new TestScheduler();
var source = Observable.Interval(TimeSpan.FromSeconds(1), scheduler)
.Take(4);
var testObserver = scheduler.Start(
() => Observable.Interval(TimeSpan.FromSeconds(1), scheduler).Take(4),
0,
TimeSpan.FromSeconds(2).Ticks,
TimeSpan.FromSeconds(5).Ticks);
Console.WriteLine("Time is {0} ticks", scheduler.Clock);
Console.WriteLine("Received {0} notifications", testObserver.Messages.Count);
foreach (Recorded<Notification<long>> message in testObserver.Messages)
{
    Console.WriteLine("{0} @ {1}", message.Value, message.Time);
}
/*
Time is 50000000 ticks
Received 2 notifications
OnNext(0) @ 30000000
OnNext(1) @ 40000000
*/
```

```
var scheduler = new TestScheduler();
var source = scheduler.CreateColdObservable(
new Recorded<Notification<long>>(10000000, Notification.CreateOnNext(0L)),
new Recorded<Notification<long>>(20000000, Notification.CreateOnNext(1L)),
new Recorded<Notification<long>>(30000000, Notification.CreateOnNext(2L)),
new Recorded<Notification<long>>(40000000, Notification.CreateOnNext(3L)),
new Recorded<Notification<long>>(40000000, Notification.CreateOnCompleted<long>())
);
var testObserver = scheduler.Start(
() => source,
0,
0,
TimeSpan.FromSeconds(5).Ticks);
Console.WriteLine("Time is {0} ticks", scheduler.Clock);
Console.WriteLine("Received {0} notifications", testObserver.Messages.Count);
foreach (Recorded<Notification<long>> message in testObserver.Messages)
{
    Console.WriteLine("  {0} @ {1}", message.Value, message.Time);
}
/*
Time is 50000000 ticks
Received 5 notifications
  OnNext(0) @ 10000001
  OnNext(1) @ 20000001
  OnNext(2) @ 30000001
  OnNext(3) @ 40000001
  OnCompleted() @ 40000001
*/
```

```
var scheduler = new TestScheduler();
var source = scheduler.CreateHotObservable(
new Recorded<Notification<long>>(10000000, Notification.CreateOnNext(0L)),
new Recorded<Notification<long>>(20000000, Notification.CreateOnNext(1L)),
new Recorded<Notification<long>>(30000000, Notification.CreateOnNext(2L)),
new Recorded<Notification<long>>(40000000, Notification.CreateOnNext(3L)),
new Recorded<Notification<long>>(40000000, Notification.CreateOnCompleted<long>())
);
var testObserver = scheduler.Start(
() => source,
0,
TimeSpan.FromSeconds(1).Ticks,
TimeSpan.FromSeconds(5).Ticks);
Console.WriteLine("Time is {0} ticks", scheduler.Clock);
Console.WriteLine("Received {0} notifications", testObserver.Messages.Count);
foreach (Recorded<Notification<long>> message in testObserver.Messages)
{
    Console.WriteLine("  {0} @ {1}", message.Value, message.Time);
}
/*
Time is 50000000 ticks
Received 4 notifications
  OnNext(1) @ 20000000
  OnNext(2) @ 30000000
  OnNext(3) @ 40000000
  OnCompleted() @ 40000000
*/
```
- RxJava

```
val scheduler = TestScheduler()
val expected = Arrays.asList(0L, 1L, 2L, 3L, 4L)
val result = ArrayList<Long>()
Observable
    .interval(1, TimeUnit.SECONDS, scheduler)
    .take(5)
    .subscribe { i -> result.add(i) }
println(result.isEmpty())
scheduler.advanceTimeBy(5, TimeUnit.SECONDS)
println(result == expected)
/*
true
true
*/
```

```
val s = TestScheduler()
s.createWorker().schedule(
    { println("Immediate") })
s.createWorker().schedule(
    { println("20s") },
    20, TimeUnit.SECONDS)
s.createWorker().schedule(
    { println("40s") },
    40, TimeUnit.SECONDS)
println("Advancing to 1ms")
s.advanceTimeTo(1, TimeUnit.MILLISECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
println("Advancing to 10s")
s.advanceTimeTo(10, TimeUnit.SECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
println("Advancing to 40s")
s.advanceTimeTo(40, TimeUnit.SECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
/*
Advancing to 1ms
Immediate
Virtual time: 0
Advancing to 10s
Virtual time: 10
Advancing to 40s
20s
40s
Virtual time: 40
*/
```

```
val s = TestScheduler()
s.createWorker().schedule { println("Immediate") }
s.createWorker().schedule(
    { println("20s") },
    20, TimeUnit.SECONDS)
s.createWorker().schedule(
    { println("40s") },
    40, TimeUnit.SECONDS)
println("Advancing by 1ms")
s.advanceTimeBy(1, TimeUnit.MILLISECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
println("Advancing by 10s")
s.advanceTimeBy(10, TimeUnit.SECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
println("Advancing by 40s")
s.advanceTimeBy(40, TimeUnit.SECONDS)
println("Virtual time: " + s.now(TimeUnit.SECONDS))
/*
Advancing by 1ms
Immediate
Virtual time: 0
Advancing by 10s
Virtual time: 10
Advancing by 40s
20s
40s
Virtual time: 50
*/
```

```
val s = TestScheduler()
s.createWorker().schedule { println("Immediate") }
s.createWorker().schedule(
    { println("20s") },
    20, TimeUnit.SECONDS)
s.triggerActions()
println("Virtual time: " + s.now(TimeUnit.SECONDS))
/*
Immediate
Virtual time: 0
*/
```

```
val s = TestScheduler()
s.createWorker().schedule(
    { println("First") },
    20, TimeUnit.SECONDS)
s.createWorker().schedule(
    { println("Second") },
    20, TimeUnit.SECONDS)
s.createWorker().schedule(
    { println("Third") },
    20, TimeUnit.SECONDS)
s.advanceTimeTo(20, TimeUnit.SECONDS)
/*
First
Second
Third
*/
```


