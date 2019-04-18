# ReactiveX 学习笔记（10）可连接的数据流 - zwvista - 博客园

## [ReactiveX 学习笔记（10）可连接的数据流](https://www.cnblogs.com/zwvista/p/9375399.html)

### Connectable Observable Operators

本文的主题为处理 Connectable Observable 的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（九）Connectable Observable Operators](http://mushuichuan.com/2016/01/12/rxjava-operator-9/)

### 冷数据流 / 热数据流 / 可连接的数据流
- 冷数据流只有在订阅后才开始发送数据。

订阅者可以保证观察到冷数据流最初所发送的数据。但是不同的订阅者所观察到的不是同一份数据，是冷数据流专为自己发送的数据。冷数据流不能共享数据。
- 热数据流从创建之初就开始发送数据，无论有没有订阅者都一直在发送数据。

订阅者只能观察到订阅之后热数据流所发送的数据。但是所有的订阅者所观察到的都是同一份数据。热数据流能够共享数据。
- 可连接的数据流在连接之后将转变为热数据流，无论有没有订阅者都开始发送数据。
- Interval 所创建的数据流是冷数据流
- RxNET

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
var observable = Observable.Interval(TimeSpan.FromSeconds(1));
using (observable.Subscribe(i => Console.WriteLine("first subscription : {0}", i)))
{
    Thread.Sleep(TimeSpan.FromSeconds(1.5));
    using (observable.Subscribe(i => Console.WriteLine("second subscription : {0}", i)))
        Console.ReadKey();
}
/*
first subscription : 0
first subscription : 1
second subscription : 0
first subscription : 2
second subscription : 1
first subscription : 3
...
*/
```
- RxJava

```
val cold = Observable
    .interval(200, TimeUnit.MILLISECONDS)
    .take(5)
cold.dump("First")
Thread.sleep(500)
cold.dump("Second")
/*
First: onNext: 0
First: onNext: 1
First: onNext: 2
Second: onNext: 0
First: onNext: 3
Second: onNext: 1
First: onNext: 4
First: onComplete
Second: onNext: 2
Second: onNext: 3
Second: onNext: 4
Second: onComplete
*/
```
- RxSwift

```
let interval = Observable<Int>.interval(1, scheduler: MainScheduler.instance)
_ = interval
    .subscribe(onNext: { print("Subscription: 1, Event: \($0)") })
delay(5) {
    _ = interval
        .subscribe(onNext: { print("Subscription: 2, Event: \($0)") })
}
/*
Subscription: 1, Event: 0
Subscription: 1, Event: 1
Subscription: 1, Event: 2
Subscription: 1, Event: 3
Subscription: 1, Event: 4
Subscription: 1, Event: 5
Subscription: 2, Event: 0
Subscription: 1, Event: 6
Subscription: 2, Event: 1
Subscription: 1, Event: 7
Subscription: 2, Event: 2
Subscription: 1, Event: 8
...
*/
```

### Connect / Publish / PublishLast / MultiCast

[ReactiveX - Connect operator](http://reactivex.io/documentation/operators/connect.html)
[ReactiveX - Publish operator](http://reactivex.io/documentation/operators/publish.html)
[Reactive Extensions再入門 その３６「ColdからHotへ！Publishメソッドと参照カウンタ？RefCountメソッド」](https://blog.okazuki.jp/entry/20120212/1329059831)
[Reactive Extensions再入門 その３７「ColdからHotへ！その他のPublish系メソッド」](https://blog.okazuki.jp/entry/20120213/1329059831)

Publish / PublishLast 将普通数据流发布（转变）为可连接的数据流。

可连接的数据流必须经 Connect 连接后才开始发送数据。

Connect 连接可连接的数据流，将可连接的数据流变为热数据流并开始发送数据。

经过 Publish 发布，Connect 连接后的热数据流在订阅之后可以观察到订阅之后热数据流发送的数据。

经过 PublishLast 发布，Connect 连接后的热数据流在订阅之后只能观察到订阅之后热数据流最后发送的数据。

Publish / PublishLast 的返回值代表数据流对象，销毁该对象将销毁源数据流。

Connect 的返回值代表连接对象，销毁该对象将断开连接，被断开连接的热数据流将变回可连接的数据流。

Multicast 的语义如下
- `.Publish() = .Multicast(new Subject<T>)`
- `.PublishLast() = .Multicast(new AsyncSubject<T>)`
- `.Replay() = .Multicast(new ReplaySubject<T>)`

![Publish](http://reactivex.io/documentation/operators/images/publishConnect.c.png)
![PublishLast](http://reactivex.io/documentation/operators/images/publishLast.png)
- RxNET

```
var observable = Observable.Interval(TimeSpan.FromSeconds(1)).Publish();
observable.Connect();
using (observable.Subscribe(i => Console.WriteLine("first subscription : {0}", i)))
{
    Thread.Sleep(TimeSpan.FromSeconds(2));
    using (observable.Subscribe(i => Console.WriteLine("second subscription : {0}", i)))
        Console.ReadKey();
}
/*
first subscription : 0
first subscription : 1
second subscription : 1
first subscription : 2
second subscription : 2
...
*/
```

```
var period = TimeSpan.FromSeconds(1);
var observable = Observable.Interval(TimeSpan.FromSeconds(1)).Publish();
using (observable.Subscribe(i => Console.WriteLine("first subscription : {0}", i)))
{
    Thread.Sleep(TimeSpan.FromSeconds(2));
    using (observable.Subscribe(i => Console.WriteLine("second subscription : {0}", i)))
    {
        observable.Connect();
        Console.ReadKey();
    }
}
/*
first subscription : 0
second subscription : 0
first subscription : 1
second subscription : 1
first subscription : 2
second subscription : 2
...
*/
```

```
var period = TimeSpan.FromSeconds(1);
var observable = Observable.Interval(period).Publish();
observable.Subscribe(i => Console.WriteLine("subscription : {0}", i));
var exit = false;
while (!exit)
{
    Console.WriteLine("Press enter to connect, esc to exit.");
    var key = Console.ReadKey(true);
    if (key.Key == ConsoleKey.Enter)
    {
        var connection = observable.Connect(); //--Connects here--
        Console.WriteLine("Press any key to dispose of connection.");
        Console.ReadKey();
        connection.Dispose(); //--Disconnects here--
    }
    if (key.Key == ConsoleKey.Escape)
    {
        exit = true;
    }
}
/*
Press enter to connect, esc to exit.
Press any key to dispose of connection.
subscription : 0
subscription : 1
subscription : 2
subscription : 3
Press enter to connect, esc to exit.
Press any key to dispose of connection.
subscription : 0
subscription : 1
subscription : 2
Press enter to connect, esc to exit.
...
*/
```

```
var period = TimeSpan.FromSeconds(1);
var observable = Observable.Interval(period)
.Do(l => Console.WriteLine("Publishing {0}", l)) //Side effect to show it is running
.Publish();
var subscription2 = observable.Connect();
Console.WriteLine("Press any key to subscribe");
Console.ReadKey();
var subscription = observable.Subscribe(i => Console.WriteLine("subscription : {0}", i));
Console.WriteLine("Press any key to unsubscribe.");
Console.ReadKey();
subscription.Dispose();
Console.WriteLine("Press any key to exit.");
Console.ReadKey();
subscription2.Dispose();
/*
Press any key to subscribe
Publishing 0
Publishing 1
Publishing 2
Press any key to unsubscribe.
Publishing 3
subscription : 3
Publishing 4
subscription : 4
Publishing 5
subscription : 5
Press any key to exit.
Publishing 6
Publishing 7
...
*/
```
- PublishLast

```
var period = TimeSpan.FromSeconds(1);
var observable = Observable.Interval(period)
.Take(5)
.Do(l => Console.WriteLine("Publishing {0}", l)) //side effect to show it is running
.PublishLast();
observable.Connect();
Console.WriteLine("Press any key to subscribe");
Console.ReadKey();
var subscription = observable.Subscribe(i => Console.WriteLine("subscription : {0}", i));
Console.WriteLine("Press any key to unsubscribe.");
Console.ReadKey();
subscription.Dispose();
Console.WriteLine("Press any key to exit.");
Console.ReadKey();
/*
Press any key to subscribe
Publishing 0
Publishing 1
Publishing 2
Press any key to unsubscribe.
Publishing 3
Publishing 4
subscription : 4
Press any key to exit.
*/
```
- RxJava

```
val cold = Observable.interval(200, TimeUnit.MILLISECONDS).publish()
cold.connect()
val s1 = cold.dump("First")
Thread.sleep(500)
val s2 = cold.dump("Second")
readLine()
s1.dispose()
s2.dispose()
/*
First: onNext: 0
First: onNext: 1
First: onNext: 2
Second: onNext: 2
First: onNext: 3
Second: onNext: 3
...
*/
```

```
val connectable = Observable.interval(200, TimeUnit.MILLISECONDS).publish()
var s = connectable.connect()
connectable.dump()
Thread.sleep(1000)
println("Closing connection")
s.dispose()
Thread.sleep(1000)
println("Reconnecting")
s = connectable.connect()
connectable.dump()
readLine()
s.dispose()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
Closing connection
onNext: 4
Reconnecting
onNext: 0
onNext: 1
onNext: 2
...
*/
```

```
val connectable = Observable.interval(200, TimeUnit.MILLISECONDS).publish()
connectable.connect()
val s1 = connectable.dump("First")
Thread.sleep(500)
val s2 = connectable.dump("Second")
Thread.sleep(500)
println("Disposing second")
s2.dispose()
readLine()
s1.dispose()
/*
First: onNext: 0
First: onNext: 1
First: onNext: 2
Second: onNext: 2
First: onNext: 3
Second: onNext: 3
Disposing second
First: onNext: 4
First: onNext: 5
First: onNext: 6
...
*/
```
- RxSwift

```
let intSequence = Observable<Int>.interval(1, scheduler: MainScheduler.instance)
    .publish()
_ = intSequence
    .subscribe(onNext: { print("Subscription 1:, Event: \($0)") })
delay(2) { _ = intSequence.connect() }
delay(4) {
    _ = intSequence
        .subscribe(onNext: { print("Subscription 2:, Event: \($0)") })
}
delay(6) {
    _ = intSequence
        .subscribe(onNext: { print("Subscription 3:, Event: \($0)") })
}
/*
Subscription 1:, Event: 0
Subscription 1:, Event: 1
Subscription 2:, Event: 1
Subscription 1:, Event: 2
Subscription 2:, Event: 2
Subscription 1:, Event: 3
Subscription 2:, Event: 3
Subscription 3:, Event: 3
Subscription 1:, Event: 4
Subscription 2:, Event: 4
Subscription 3:, Event: 4
...
*/
```

```
let subject = PublishSubject<Int>()
_ = subject
    .subscribe(onNext: { print("Subject: \($0)") })
let intSequence = Observable<Int>.interval(1, scheduler: MainScheduler.instance)
    .multicast(subject)
_ = intSequence
    .subscribe(onNext: { print("\tSubscription 1:, Event: \($0)") })
delay(2) { _ = intSequence.connect() }
delay(4) {
    _ = intSequence
        .subscribe(onNext: { print("\tSubscription 2:, Event: \($0)") })
}
delay(6) {
    _ = intSequence
        .subscribe(onNext: { print("\tSubscription 3:, Event: \($0)") })
}
/*
Subject: 0
    Subscription 1:, Event: 0
Subject: 1
    Subscription 1:, Event: 1
    Subscription 2:, Event: 1
Subject: 2
    Subscription 1:, Event: 2
    Subscription 2:, Event: 2
Subject: 3
    Subscription 1:, Event: 3
    Subscription 2:, Event: 3
    Subscription 3:, Event: 3
...
*/
```

### RefCount

[ReactiveX - RefCount operator](http://reactivex.io/documentation/operators/refcount.html)
[Reactive Extensions再入門 その３６「ColdからHotへ！Publishメソッドと参照カウンタ？RefCountメソッド」](https://blog.okazuki.jp/entry/20120212/1329059831)

RefCount 将可连接的数据流重新变回普通数据流。

RefCount 还给数据流加上引用计数语义，订阅时引用计数器加一，取消订阅时引用计数器减一，订阅者都取消订阅时数据流自动销毁。

![RefCount](http://reactivex.io/documentation/operators/images/publishRefCount.c.png)
- RxNET

```
var period = TimeSpan.FromSeconds(1);
var observable = Observable.Interval(period)
.Do(l => Console.WriteLine("Publishing {0}", l)) //side effect to show it is running
.Publish()
.RefCount();
//observable.Connect(); Use RefCount instead now 
Console.WriteLine("Press any key to subscribe");
Console.ReadKey();
var subscription = observable.Subscribe(i => Console.WriteLine("subscription : {0}", i));
Console.WriteLine("Press any key to unsubscribe.");
Console.ReadKey();
subscription.Dispose();
Console.WriteLine("Press any key to exit.");
Console.ReadKey();
/*
Press any key to subscribe
Press any key to unsubscribe.
Publishing 0
subscription : 0
Publishing 1
subscription : 1
Publishing 2
subscription : 2
Press any key to exit.
*/
```
- RxJava

```
val cold = Observable.interval(200, TimeUnit.MILLISECONDS).publish().refCount()
var s1 = cold.dump("First")
Thread.sleep(500)
val s2 = cold.dump("Second")
Thread.sleep(500)
println("Dispose Second")
s2.dispose()
Thread.sleep(500)
println("Dispose First")
s1.dispose()
println("First connection again")
Thread.sleep(500)
s1 = cold.dump("First")
readLine()
s1.dispose()
/*
First: onNext: 0
First: onNext: 1
First: onNext: 2
Second: onNext: 2
First: onNext: 3
Second: onNext: 3
Dispose Second
First: onNext: 4
First: onNext: 5
First: onNext: 6
Dispose First
First connection again
First: onNext: 0
First: onNext: 1
First: onNext: 2
...
*/
```

### Replay / Cache

[ReactiveX - Replay operator](http://reactivex.io/documentation/operators/replay.html)
[Reactive Extensions再入門 その３８「ColdからHotへ！その他のPublish系メソッド２」](https://blog.okazuki.jp/entry/20120214/1329059831)

Replay 给热数据流加上 Replay 语义，即缓存所有的数据，使后来的订阅者也能观察到订阅之前所发送的数据。

Replay 可以指定缓冲区的大小。

![Replay](http://reactivex.io/documentation/operators/images/replay.c.png)
- RxNET

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
var hot = Observable.Interval(TimeSpan.FromSeconds(1))
.Take(3)
.Publish();
hot.Connect();
Thread.Sleep(TimeSpan.FromSeconds(1.5)); //Run hot and ensure a value is lost.
var observable = hot.Replay();
observable.Connect();
observable.Subscribe(i => Console.WriteLine("first subscription : {0}", i));
Thread.Sleep(TimeSpan.FromSeconds(1.5));
observable.Subscribe(i => Console.WriteLine("second subscription : {0}", i));
Console.ReadKey();
observable.Subscribe(i => Console.WriteLine("third subscription : {0}", i));
Console.ReadKey();
/*
first subscription : 1
second subscription : 1
first subscription : 2
second subscription : 2
third subscription : 1
third subscription : 2
*/
```
- RxJava

```
val cold = Observable.interval(200, TimeUnit.MILLISECONDS).replay()
cold.connect()
println("Subscribe first")
val s1 = cold.dump("First")
Thread.sleep(700)
println("Subscribe second")
val s2 = cold.dump("Second")
Thread.sleep(500)
readLine()
s1.dispose()
s2.dispose()
/*
Subscribe first
First: onNext: 0
First: onNext: 1
First: onNext: 2
Subscribe second
Second: onNext: 0
Second: onNext: 1
Second: onNext: 2
First: onNext: 3
Second: onNext: 3
First: onNext: 4
Second: onNext: 4
...
*/
```

```
val source = Observable.interval(1000, TimeUnit.MILLISECONDS)
    .take(5)
    .replay(2)
source.connect()
Thread.sleep(4500)
source.dump()
/*
onNext: 2
onNext: 3
onNext: 4
onComplete
*/
```

```
val source = Observable.interval(1000, TimeUnit.MILLISECONDS)
    .take(5)
    .replay(2000, TimeUnit.MILLISECONDS)
source.connect()
Thread.sleep(4500)
source.dump()
/*
onNext: 2
onNext: 3
onNext: 4
onComplete
*/
```

```
val obs = Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(5)
    .cache()
Thread.sleep(500)
obs.dump("First")
Thread.sleep(300)
obs.dump("Second")
/*
First: onNext: 0
First: onNext: 1
First: onNext: 2
Second: onNext: 0
Second: onNext: 1
Second: onNext: 2
First: onNext: 3
Second: onNext: 3
First: onNext: 4
Second: onNext: 4
First: onComplete
Second: onComplete
*/
```

```
val obs = Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(5)
    .doOnNext { println(it) }
    .cache()
    .doOnSubscribe { println("Subscribed") }
    .doOnDispose { println("Disposed") }
val subscription = obs.subscribe()
Thread.sleep(150)
subscription.dispose()
/*
Subscribed
0
Disposed
1
2
3
4
*/
```
- RxSwift

[RxSwift: share vs replay vs shareReplay](https://medium.com/@_achou/rxswift-share-vs-replay-vs-sharereplay-bea99ac42168)

```
let intSequence = Observable<Int>.interval(1, scheduler: MainScheduler.instance)
    .replay(5)
_ = intSequence
    .subscribe(onNext: { print("Subscription 1:, Event: \($0)") })
delay(2) { _ = intSequence.connect() }
delay(4) {
    _ = intSequence
        .subscribe(onNext: { print("Subscription 2:, Event: \($0)") })
}
delay(8) {
    _ = intSequence
        .subscribe(onNext: { print("Subscription 3:, Event: \($0)") })
}
/*
Subscription 1:, Event: 0 (after 3 secs)
Subscription 2:, Event: 0 (after 4 secs)
Subscription 1:, Event: 1 (after 4 secs)
Subscription 2:, Event: 1 (after 4 secs)
Subscription 1:, Event: 2 (after 5 secs)
Subscription 2:, Event: 2 (after 5 secs)
Subscription 1:, Event: 3 (after 6 secs)
Subscription 2:, Event: 3 (after 6 secs)
Subscription 1:, Event: 4 (after 7 secs)
Subscription 2:, Event: 4 (after 7 secs)
Subscription 3:, Event: 0 (after 8 secs)
Subscription 3:, Event: 1 (after 8 secs)
Subscription 3:, Event: 2 (after 8 secs)
Subscription 3:, Event: 3 (after 8 secs)
Subscription 3:, Event: 4 (after 8 secs)
Subscription 1:, Event: 5 (after 9 secs)
Subscription 2:, Event: 5 (after 9 secs)
Subscription 3:, Event: 5 (after 9 secs)
...
*/
```


