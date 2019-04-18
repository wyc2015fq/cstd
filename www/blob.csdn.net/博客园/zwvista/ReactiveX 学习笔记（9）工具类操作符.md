# ReactiveX 学习笔记（9）工具类操作符 - zwvista - 博客园

## [ReactiveX 学习笔记（9）工具类操作符](https://www.cnblogs.com/zwvista/p/9371686.html)

### Observable Utility Operators

本文的主题为处理 Observable 的实用工具类操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（六）Utility](http://mushuichuan.com/2015/12/11/rxjava-operator-6/)

### 公共代码
- RxNET

```
public static void Dump<T>(this IObservable<T> source, string name)
{
    source.Subscribe(
    i => Console.WriteLine("{0}-->{1}", name, i),
    ex => Console.WriteLine("{0} failed-->{1}", name, ex.Message),
    () => Console.WriteLine("{0} completed", name));
}
```

### Delay / DelaySubscription

[ReactiveX - Delay operator](http://reactivex.io/documentation/operators/delay.html)
[Reactive Extensions再入門 その２９「値を指定した時間だけ遅延させるDelayメソッド」](https://blog.okazuki.jp/entry/20120203/1328274110)

Delay 发送源数据流的数据，但是发送时间要延迟一个指定的时间段。

DelaySubscription 发送源数据流的数据，但是要订阅时间要延迟一个指定的时间段。

![Delay](http://reactivex.io/documentation/operators/images/delay.c.png)
![DelaySubscription](http://reactivex.io/documentation/operators/images/delaySubscription.o.png)
- RxNET

```
var source = Observable.Interval(TimeSpan.FromSeconds(1))
.Take(5)
.Timestamp();
var delay = source.Delay(TimeSpan.FromSeconds(2));
source.Subscribe(
value => Console.WriteLine("source : {0}", value),
() => Console.WriteLine("source Completed"));
delay.Subscribe(
value => Console.WriteLine("delay : {0}", value),
() => Console.WriteLine("delay Completed"));
/*
source : 0@2018/07/26 9:34:09 +00:00
source : 1@2018/07/26 9:34:10 +00:00
source : 2@2018/07/26 9:34:11 +00:00
delay : 0@2018/07/26 9:34:09 +00:00
source : 3@2018/07/26 9:34:12 +00:00
delay : 1@2018/07/26 9:34:10 +00:00
source : 4@2018/07/26 9:34:13 +00:00
source Completed
delay : 2@2018/07/26 9:34:11 +00:00
delay : 3@2018/07/26 9:34:12 +00:00
delay : 4@2018/07/26 9:34:13 +00:00
delay Completed
*/
```

```
var source = Observable.Interval(TimeSpan.FromSeconds(1))
.Take(5)
.Timestamp();
var delay = source.DelaySubscription(TimeSpan.FromSeconds(2));
source.Subscribe(
value => Console.WriteLine("source : {0}", value),
() => Console.WriteLine("source Completed"));
delay.Subscribe(
value => Console.WriteLine("delay : {0}", value),
() => Console.WriteLine("delay Completed"));
/*
source : 0@2018/07/26 11:03:15 +00:00
source : 1@2018/07/26 11:03:16 +00:00
source : 2@2018/07/26 11:03:17 +00:00
delay : 0@2018/07/26 11:03:17 +00:00
source : 3@2018/07/26 11:03:18 +00:00
delay : 1@2018/07/26 11:03:18 +00:00
source : 4@2018/07/26 11:03:19 +00:00
source Completed
delay : 2@2018/07/26 11:03:19 +00:00
delay : 3@2018/07/26 11:03:20 +00:00
delay : 4@2018/07/26 11:03:21 +00:00
delay Completed
*/
```
- RxJava

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .delay({ i -> Observable.timer(i * 100, TimeUnit.MILLISECONDS) })
    .timeInterval()
    .take(5)
    .dump()
/*
onNext: Timed[time=105, unit=MILLISECONDS, value=0]
onNext: Timed[time=196, unit=MILLISECONDS, value=1]
onNext: Timed[time=201, unit=MILLISECONDS, value=2]
onNext: Timed[time=207, unit=MILLISECONDS, value=3]
onNext: Timed[time=196, unit=MILLISECONDS, value=4]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .delaySubscription(1000, TimeUnit.MILLISECONDS)
    .timeInterval()
    .take(5)
    .dump()
/*
onNext: Timed[time=1105, unit=MILLISECONDS, value=0]
onNext: Timed[time=102, unit=MILLISECONDS, value=1]
onNext: Timed[time=98, unit=MILLISECONDS, value=2]
onNext: Timed[time=99, unit=MILLISECONDS, value=3]
onNext: Timed[time=99, unit=MILLISECONDS, value=4]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .delaySubscription(Observable.timer(1000, TimeUnit.MILLISECONDS))
    .timeInterval()
    .take(5)
    .dump()
/*
onNext: Timed[time=1110, unit=MILLISECONDS, value=0]
onNext: Timed[time=96, unit=MILLISECONDS, value=1]
onNext: Timed[time=99, unit=MILLISECONDS, value=2]
onNext: Timed[time=100, unit=MILLISECONDS, value=3]
onNext: Timed[time=103, unit=MILLISECONDS, value=4]
onComplete
*/
```
- RxJS

```
// emit one item
const example = of(null);
// delay output of each by an extra second
const message = merge(
  example.pipe(mapTo('Hello')),
  example.pipe(
    mapTo('World!'),
    delay(1000)
  ),
  example.pipe(
    mapTo('Goodbye'),
    delay(2000)
  ),
  example.pipe(
    mapTo('World!'),
    delay(3000)
  )
);
// output: 'Hello'...'World!'...'Goodbye'...'World!'
const subscribe = message.subscribe(val => console.log(val));
```

```
// emit value every second
const message = interval(1000);
// emit value after five seconds
const delayForFiveSeconds = () => timer(5000);
// after 5 seconds, start emitting delayed interval values
const delayWhenExample = message.pipe(delayWhen(delayForFiveSeconds));
// log values, delayed for 5 seconds
// ex. output: 5s....1...2...3
const subscribe = delayWhenExample.subscribe(val => console.log(val));
```

### Do / Finally

[ReactiveX - Do operator](http://reactivex.io/documentation/operators/do.html)
[Reactive Extensions再入門 その１０「Doメソッド」](https://blog.okazuki.jp/entry/20111128/1322491648)
[Reactive Extensions再入門 その１２「Finallyメソッドとリソース解放」](https://blog.okazuki.jp/entry/20111129/1322575568)

Do / Finally 在源数据流的生命周期内的指定时间点注册一个需要被执行的回调函数。

Do 所注册的回调函数在源数据流调用 OnNext, OnError 以及 OnComplete 之前被调用。Do 最多可以注册 3 个回调函数。

Finally 所注册的回调函数在源数据流调用 OnError 或 OnComplete 之后被调用。Finally 只能注册 1 个回调函数。

![Do](http://reactivex.io/documentation/operators/images/do.c.png)
![Do](http://reactivex.io/documentation/operators/images/finally.png)
- RxNET

```
private static void Log(object onNextValue)
{
    Console.WriteLine("Logging OnNext({0}) @ {1}", onNextValue, DateTime.Now);
}
private static void Log(Exception onErrorValue)
{
    Console.WriteLine("Logging OnError({0}) @ {1}", onErrorValue, DateTime.Now);
}
private static void Log()
{
    Console.WriteLine("Logging OnCompleted()@ {0}", DateTime.Now);
}
var source = Observable
.Interval(TimeSpan.FromSeconds(1))
.Take(3);
var result = source.Do(
i => Log(i),
ex => Log(ex),
() => Log());
result.Subscribe(
Console.WriteLine,
() => Console.WriteLine("completed"));
/*
Logging OnNext(0) @ 2018/07/26 18:58:32
0
Logging OnNext(1) @ 2018/07/26 18:58:33
1
Logging OnNext(2) @ 2018/07/26 18:58:34
2
Logging OnCompleted()@ 2018/07/26 18:58:34
completed
*/
```

```
private static IObservable<long> GetNumbers()
{
    return Observable.Interval(TimeSpan.FromMilliseconds(250))
    .Do(i => Console.WriteLine("pushing {0} from GetNumbers", i));
}
var source = GetNumbers();
var result = source.Where(i => i % 3 == 0)
.Take(3)
.Select(i => (char)(i + 65));
result.Subscribe(
Console.WriteLine,
() => Console.WriteLine("completed"));
/*
pushing 0 from GetNumbers
A
pushing 1 from GetNumbers
pushing 2 from GetNumbers
pushing 3 from GetNumbers
D
pushing 4 from GetNumbers
pushing 5 from GetNumbers
pushing 6 from GetNumbers
G
completed
*/
```

```
var source = new Subject<int>();
var result = source.Finally(() => Console.WriteLine("Finally action ran"));
result.Dump("Finally");
source.OnNext(1);
source.OnNext(2);
source.OnNext(3);
source.OnCompleted();
/*
Finally-->1
Finally-->2
Finally-->3
Finally completed
Finally action ran
*/
```

```
var source = new Subject<int>();
var result = source.Finally(() => Console.WriteLine("Finally"));
var subscription = result.Subscribe(
Console.WriteLine,
Console.WriteLine,
() => Console.WriteLine("Completed"));
source.OnNext(1);
source.OnNext(2);
source.OnNext(3);
subscription.Dispose();
/*
1
2
3
Finally
*/
```
- RxJava

```
val values = Observable.just("side", "effects")
values
    .doOnEach(PrintSubscriber("Log"))
    .map { s -> s.toUpperCase() }
    .dump("Process")
/*
Log: onNext: side
Process: onNext: SIDE
Log: onNext: effects
Process: onNext: EFFECTS
Log: onComplete
Process: onComplete
*/
```

```
val service = {
    Observable
        .just("First", "Second", "Third")
        .doOnEach(PrintSubscriber("Log"))
}
service.invoke()
    .map { s -> s.toUpperCase() }
    .filter { s -> s.length > 5 }
    .dump("Process")
/*
Log: onNext: First
Log: onNext: Second
Process: onNext: SECOND
Log: onNext: Third
Log: onComplete
Process: onComplete
*/
```

```
val subject = ReplaySubject.create<Int>()
val values = subject
    .doOnSubscribe { println("New subscription") }
    .doOnDispose() { println("Subscription over") }
val s1 = values.dump("1st")
subject.onNext(0)
values.dump("2st")
subject.onNext(1)
s1.dispose()
subject.onNext(2)
subject.onNext(3)
subject.onComplete()
/*
New subscription
1st: onNext: 0
New subscription
2st: onNext: 0
1st: onNext: 1
2st: onNext: 1
Subscription over
2st: onNext: 2
2st: onNext: 3
2st: onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of("🍎", "🍐", "🍊", "🍋")
    .do(onNext: { print("Intercepted:", $0) }, onError: { print("Intercepted error:", $0) }, onCompleted: { print("Completed")  })
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
Intercepted: 🍎
🍎
Intercepted: 🍐
🍐
Intercepted: 🍊
🍊
Intercepted: 🍋
🍋
Completed
*/
```
- RxJS

```
const source = of(1, 2, 3, 4, 5);
// transparently log values from source with 'do'
const example = source.pipe(
  tap(val => console.log(`BEFORE MAP: ${val}`)),
  map(val => val + 10),
  tap(val => console.log(`AFTER MAP: ${val}`))
);

// 'do' does not transform values
// output: 11...12...13...14...15
const subscribe = example.subscribe(val => console.log(val));
```

### Materialize / Dematerialize

[ReactiveX - Materialize & Dematerialize operators](http://reactivex.io/documentation/operators/materialize-dematerialize.html)

![Materialize](http://reactivex.io/documentation/operators/images/materialize.c.png)
![Materialize](http://reactivex.io/documentation/operators/images/dematerialize.c.png)
- RxNET

```
Observable.Range(1, 3)
.Materialize()
.Dump("Materialize");
/*
Materialize-->OnNext(1)
Materialize-->OnNext(2)
Materialize-->OnNext(3)
Materialize-->OnCompleted()
Materialize completed
*/
var source = new Subject<int>();
source.Materialize()
.Dump("Materialize");
source.OnNext(1);
source.OnNext(2);
source.OnNext(3);
source.OnError(new Exception("Fail?"));
/*
Materialize-->OnNext(1)
Materialize-->OnNext(2)
Materialize-->OnNext(3)
Materialize-->OnError(System.Exception)
Materialize completed
*/
```

```
Observable.Range(1, 3)
.Materialize()
.Dematerialize()
.Dump("Dematerialize");
/*
Dematerialize-->1
Dematerialize-->2
Dematerialize-->3
Dematerialize completed
*/
```
- RxJava

```
val values = Observable.range(0, 3)
values.take(3)
    .materialize()
    .dump()
/*
onNext: OnNextNotification[0]
onNext: OnNextNotification[1]
onNext: OnNextNotification[2]
onNext: OnCompleteNotification
onComplete
*/
```
- RxJS

```
// emit next and error notifications
const source = from([
  Notification.createNext('SUCCESS!'),
  Notification.createError('ERROR!')
]).pipe(
  // turn notification objects into notification values
  dematerialize()
);

// output: 'NEXT VALUE: SUCCESS' 'ERROR VALUE: 'ERROR!'
const subscription = source.subscribe({
  next: val => console.log(`NEXT VALUE: ${val}`),
  error: val => console.log(`ERROR VALUE: ${val}`)
});
```

### Subscribe

[ReactiveX - Subscribe operator](http://reactivex.io/documentation/operators/subscribe.html)

Subscribe（订阅） 将数据流与观察者联系起来，通过订阅观察者开始接收数据流所发送的数据以及完成和出错消息。

### Serialize

[ReactiveX - Serialize operator](http://reactivex.io/documentation/operators/serialize.html)

![Serialize](http://reactivex.io/documentation/operators/images/serialize.c.png)
- RxNET

```
/*
*/
```
- RxJava

```
val source = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
    o.onNext(3)
    o.onComplete()
}
source
    .dump()
/*
onNext: 1
onNext: 2
onComplete
*/
```

```
val source = Observable.create<Any> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onComplete()
    o.onNext(3)
    o.onComplete()
}
    .cast(java.lang.Integer::class.java)
    .serialize()
source
    .dump()
/*
onNext: 1
onNext: 2
onComplete
*/
```

### TimeInterval

[ReactiveX - TimeInterval operator](http://reactivex.io/documentation/operators/timeinterval.html)
[Reactive Extensions再入門 その３１「時間に関する情報を付与するTimestampとTimeIntervalメソッド」](https://blog.okazuki.jp/entry/20120205/1328450809)

TimeInterval 发送源数据流的数据，但在数据上附上与上一个数据间的时间间隔。

![TimeInterval](http://reactivex.io/documentation/operators/images/timeInterval.c.png)
- RxNET

```
Observable.Interval(TimeSpan.FromSeconds(1))
.Take(3)
.TimeInterval()
.Dump("TimeInterval");
/*
TimeInterval-->0@00:00:01.0147480
TimeInterval-->1@00:00:01.0130265
TimeInterval-->2@00:00:01.0138236
TimeInterval completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values.take(3)
    .timeInterval()
    .dump()
/*
onNext: Timed[time=105, unit=MILLISECONDS, value=0]
onNext: Timed[time=100, unit=MILLISECONDS, value=1]
onNext: Timed[time=96, unit=MILLISECONDS, value=2]
onComplete
*/
```

### Timeout

[ReactiveX - Timeout operator](http://reactivex.io/documentation/operators/timeout.html)
[Reactive Extensions再入門 その３０「もう待ちきれない！を表現するTimeoutメソッド」](https://blog.okazuki.jp/entry/20120205/1328274110)

Timeout 发送源数据流的数据，但是当指定时间段内没有发送任何数据时会报错抛出 TimeoutException 异常。

![Timeout](http://reactivex.io/documentation/operators/images/timeout.c.png)
- RxNET

```
var source = Observable.Interval(TimeSpan.FromMilliseconds(100)).Take(10)
.Concat(Observable.Interval(TimeSpan.FromSeconds(2)));
var timeout = source.Timeout(TimeSpan.FromSeconds(1));
timeout.Subscribe(
Console.WriteLine,
Console.WriteLine,
() => Console.WriteLine("Completed"));
/*
0
1
2
3
4
5
6
7
8
9
*/
System.TimeoutException:
```

```
var dueDate = DateTimeOffset.UtcNow.AddSeconds(4);
var source = Observable.Interval(TimeSpan.FromSeconds(1));
var timeout = source.Timeout(dueDate);
timeout.Subscribe(
Console.WriteLine,
Console.WriteLine,
() => Console.WriteLine("Completed"));
/*
0
1
2
System.TimeoutException: 
*/
```
- RxJava

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .timeout(200, TimeUnit.MILLISECONDS)
    .dump()
readLine()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onError: java.util.concurrent.TimeoutException: null
*/
```

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .timeout(200, TimeUnit.MILLISECONDS, Observable.just(-1))
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onNext: -1
onComplete
*/
```

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .timeout { i -> Observable.timer(200, TimeUnit.MILLISECONDS) }
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onError: java.util.concurrent.TimeoutException: null
*/
```

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .timeout<Long>(Function { i -> Observable.timer(200, TimeUnit.MILLISECONDS) }, Observable.just(-1))
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onNext: -1
onComplete
*/
```
- RxJS

```
//  simulate request
function makeRequest(timeToDelay) {
  return of('Request Complete!').pipe(delay(timeToDelay));
}

of(4000, 3000, 2000)
  .pipe(
    concatMap(duration =>
      makeRequest(duration).pipe(
        timeout(2500),
        catchError(error => of(`Request timed out after: ${duration}`))
      )
    )
  )
  /*
  *  "Request timed out after: 4000"
  *  "Request timed out after: 3000"
  *  "Request Complete!"
  */
  .subscribe(val => console.log(val));
```

### Timestamp

[ReactiveX - Timestamp operator](http://reactivex.io/documentation/operators/timestamp.html)
[Reactive Extensions再入門 その３１「時間に関する情報を付与するTimestampとTimeIntervalメソッド」](https://blog.okazuki.jp/entry/20120205/1328450809)

Timestamp 发送源数据流的数据，但在数据上附上时间戳。

![Timestamp](http://reactivex.io/documentation/operators/images/timestamp.c.png)
- RxNET

```
Observable.Interval(TimeSpan.FromSeconds(1))
.Take(3)
.Timestamp()
.Dump("TimeStamp");
/*
TimeStamp-->0@2018/07/26 9:29:47 +00:00
TimeStamp-->1@2018/07/26 9:29:48 +00:00
TimeStamp-->2@2018/07/26 9:29:49 +00:00
TimeStamp completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values.take(3)
    .timestamp()
    .dump()
/*
onNext: Timed[time=1533111209618, unit=MILLISECONDS, value=0]
onNext: Timed[time=1533111209717, unit=MILLISECONDS, value=1]
onNext: Timed[time=1533111209819, unit=MILLISECONDS, value=2]
onComplete
*/
```

### Using

[ReactiveX - Using operator](http://reactivex.io/documentation/operators/using.html)
[Reactive Extensions再入門 その１２「Finallyメソッドとリソース解放」](https://blog.okazuki.jp/entry/20111129/1322575568)

Using 可以创建一个和源数据流具有同样生命周期的可销毁的资源。

当源数据流终止之后，Using 所创建的资源也将被销毁。

![Using](http://reactivex.io/documentation/operators/images/using.c.png)
- RxNET

```
public class TimeIt : IDisposable
{
    private readonly string _name;
    private readonly Stopwatch _watch;
    public TimeIt(string name)
    {
        _name = name;
        _watch = Stopwatch.StartNew();
    }
    public void Dispose()
    {
        _watch.Stop();
        Console.WriteLine("{0} took {1}", _name, _watch.Elapsed);
    }
}
var source = Observable.Interval(TimeSpan.FromSeconds(1));
var result = Observable.Using(
() => new TimeIt("Subscription Timer"),
timeIt => source);
result.Take(5).Dump("Using");
Console.ReadLine();
/*
Using-->0
Using-->1
Using-->2
Using-->3
Using-->4
Using completed
Subscription Timer took 00:00:05.0705125
*/
```
- RxJava

```
val values = Observable.using(
    {
        val resource = "MyResource"
        println("Leased: $resource")
        resource
    },
    { resource ->
        Observable.create<Char> { o ->
            for (c in resource.toCharArray())
                o.onNext(c)
            o.onComplete()
        }
    },
    { resource -> println("Disposed: $resource") })
values
    .dump()
/*
Leased: MyResource
onNext: M
onNext: y
onNext: R
onNext: e
onNext: s
onNext: o
onNext: u
onNext: r
onNext: c
onNext: e
Disposed: MyResource
onComplete
*/
```


