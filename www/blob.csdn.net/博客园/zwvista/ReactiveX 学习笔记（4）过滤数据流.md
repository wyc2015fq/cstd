# ReactiveX 学习笔记（4）过滤数据流 - zwvista - 博客园

## [ReactiveX 学习笔记（4）过滤数据流](https://www.cnblogs.com/zwvista/p/9335540.html)

### Filtering Observables

本文主题为过滤 Observable 的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（三）Filtering](http://mushuichuan.com/2015/12/11/rxjava-operator-3/)

### Debounce / Throttle

[ReactiveX - Debounce operator](http://reactivex.io/documentation/operators/debounce.html)
[Reactive Extensions再入門 その２８「落ち着いたら流すThrottleメソッド」](https://blog.okazuki.jp/entry/20120202/1328107196)

Debounce / Throttle 只发送源数据流中满足如下条件的数据：源数据流在发送该数据之后在指定时间段内未发送任何数据。

![Debounce](http://reactivex.io/documentation/operators/images/debounce.c.png)
- RxNET

```
var source = new Subject<int>();
source
    .Throttle(TimeSpan.FromMilliseconds(500))
    .Subscribe(i =>
        Console.WriteLine("{0:HH:mm:ss.fff} {1}", DateTime.Now, i));
foreach (var i in Enumerable.Range(1, 10))
{
    Console.WriteLine("{0:HH:mm:ss.fff} OnNext({1})",DateTime.Now, i);
    source.OnNext(i);
    Thread.Sleep(100);
}
Console.WriteLine("{0:HH:mm:ss.fff} Sleep(2000)", DateTime.Now);
Thread.Sleep(2000);
foreach (var i in Enumerable.Range(1, 5))
{
    Console.WriteLine("{0:HH:mm:ss.fff} OnNext({1})", DateTime.Now, i);
    source.OnNext(i);
    Thread.Sleep(100);
}
Console.WriteLine("{0:HH:mm:ss.FFF} Sleep(2000)", DateTime.Now);
Thread.Sleep(2000);
/*
21:15:39.602 OnNext(1)
21:15:39.759 OnNext(2)
21:15:39.864 OnNext(3)
21:15:39.969 OnNext(4)
21:15:40.074 OnNext(5)
21:15:40.178 OnNext(6)
21:15:40.282 OnNext(7)
21:15:40.387 OnNext(8)
21:15:40.491 OnNext(9)
21:15:40.596 OnNext(10)
21:15:40.700 Sleep(2000)
21:15:41.105 10
21:15:42.706 OnNext(1)
21:15:42.809 OnNext(2)
21:15:42.911 OnNext(3)
21:15:43.011 OnNext(4)
21:15:43.113 OnNext(5)
21:15:43.217 Sleep(2000)
21:15:43.613 5
*/
```
- RxJava

![debounce](http://reactivex.io/documentation/operators/images/debounce.png)
![throttleWithTimeout](http://reactivex.io/documentation/operators/images/throttleWithTimeout.png)
![throttleFirst](http://reactivex.io/documentation/operators/images/throttleFirst.png)
![throttleLast](http://reactivex.io/documentation/operators/images/throttleLast.png)

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .debounce(150, TimeUnit.MILLISECONDS)
    .dump()
/*
onNext: 3
onNext: 4
onNext: 5
onNext: 9
onComplete
*/
```

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .debounce { i -> Observable.timer(i * 50L, TimeUnit.MILLISECONDS) }
    .dump()
/*
onNext: 0
onNext: 1
onNext: 3
onNext: 4
onNext: 5
onNext: 9
onComplete
*/
```

```
Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(500, TimeUnit.MILLISECONDS).take(3),
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3))
    .scan(0) { acc, v -> acc + 1 }
    .throttleWithTimeout(150, TimeUnit.MILLISECONDS)
    .dump()
/*
onNext: 3
onNext: 4
onNext: 5
onNext: 9
onComplete
*/
```

```
Observable.interval(150, TimeUnit.MILLISECONDS)
    .throttleFirst(1, TimeUnit.SECONDS)
    .take(3)
    .dump()
/*
onNext: 0
onNext: 7
onNext: 14
onComplete
*/
```

```
Observable.interval(150, TimeUnit.MILLISECONDS)
    .throttleLast(1, TimeUnit.SECONDS)
    .take(3)
    .dump()
/*
onNext: 5
onNext: 12
onNext: 18
onComplete
*/
```

### Distinct / DistinctUntilChanged

[ReactiveX - Distinct operator](http://reactivex.io/documentation/operators/distinct.html)
[Reactive Extensions再入門 その２３「重複を排除するメソッド」](https://blog.okazuki.jp/entry/20120104/1325679391)

Distinct 忽略源数据流的重复数据，只发送源数据流中其余各项。

DistinctUntilChanged 忽略源数据流中邻近的重复数据，只发送源数据流中其余各项。

![Distinct](http://reactivex.io/documentation/operators/images/distinct.c.png)
![DistinctUntilChanged](http://reactivex.io/documentation/operators/images/distinctUntilChanged.png)
- RxNET

```
var subject = new Subject<int>();
var distinct = subject.Distinct();
subject.Subscribe(
i => Console.WriteLine("{0}", i),
() => Console.WriteLine("subject.OnCompleted()"));
distinct.Subscribe(
i => Console.WriteLine("distinct.OnNext({0})", i),
() => Console.WriteLine("distinct.OnCompleted()"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnNext(1);
subject.OnNext(1);
subject.OnNext(4);
subject.OnCompleted();
/*
1
distinct.OnNext(1)
2
distinct.OnNext(2)
3
distinct.OnNext(3)
1
1
4
distinct.OnNext(4)
subject.OnCompleted()
distinct.OnCompleted()
*/
```

```
var subject = new Subject<int>();
var distinct = subject.DistinctUntilChanged();
subject.Subscribe(
i => Console.WriteLine("{0}", i),
() => Console.WriteLine("subject.OnCompleted()"));
distinct.Subscribe(
i => Console.WriteLine("distinct.OnNext({0})", i),
() => Console.WriteLine("distinct.OnCompleted()"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnNext(1);
subject.OnNext(1);
subject.OnNext(4);
subject.OnCompleted();
/*
1
distinct.OnNext(1)
2
distinct.OnNext(2)
3
distinct.OnNext(3)
1
distinct.OnNext(1)
1
4
distinct.OnNext(4)
subject.OnCompleted()
distinct.OnCompleted()
*/
```
- RxJava

![Distinct](http://reactivex.io/documentation/operators/images/distinct.key.png)
![DistinctUntilChanged](http://reactivex.io/documentation/operators/images/distinctUntilChanged.key.png)

```
val values = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(1)
    o.onNext(2)
    o.onNext(3)
    o.onNext(2)
    o.onComplete()
}
values
    .distinct()
    .dump()
/*
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```

```
val values = Observable.create<String> { o ->
    o.onNext("First")
    o.onNext("Second")
    o.onNext("Third")
    o.onNext("Fourth")
    o.onNext("Fifth")
    o.onComplete()
}
values
    .distinct { v -> v[0] }
    .dump()
/*
onNext: First
onNext: Second
onNext: Third
onComplete
*/
```

```
val values = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(1)
    o.onNext(2)
    o.onNext(3)
    o.onNext(2)
    o.onComplete()
}
values
    .distinctUntilChanged()
    .dump()
/*
onNext: 1
onNext: 2
onNext: 3
onNext: 2
onComplete
*/
```

```
val values = Observable.create<String> { o ->
    o.onNext("First")
    o.onNext("Second")
    o.onNext("Third")
    o.onNext("Fourth")
    o.onNext("Fifth")
    o.onComplete()
}
values
    .distinctUntilChanged { v -> v[0] }
    .dump()
/*
onNext: First
onNext: Second
onNext: Third
onNext: Fourth
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of("🐱", "🐷", "🐱", "🐱", "🐱", "🐵", "🐱")
    .distinctUntilChanged()
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐱
🐷
🐱
🐵
🐱
*/
```

### ElementAt / ElementAtOrDefault

[ReactiveX - ElementAt operator](http://reactivex.io/documentation/operators/elementat.html)
[Reactive Extensions再入門 その２２「単一の値を取得するメソッド」](https://blog.okazuki.jp/entry/20120103/1325587713)

ElementAt 只发送源数据流中的第 n 项。当源数据流中没有第 n 项时报错。

ElementAtOrDefault 当源数据流中没有第 n 项时不报错，发送给定的缺省值。

![ElementAt](http://reactivex.io/documentation/operators/images/elementAt.c.png)
![ElementAtOrDefault](http://reactivex.io/documentation/operators/images/elementAtOrDefault.png)
- RxNET

```
var subject = new Subject<int>();
subject.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Subject completed"));
var elementAt1 = subject.ElementAt(1);
elementAt1.Subscribe(
b => Console.WriteLine("elementAt1 value: {0}", b),
() => Console.WriteLine("elementAt1 completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnCompleted();
/*
1
2
elementAt1 value: 2
elementAt1 completed
3
Subject completed
*/
```
- RxJava

```
val values = Observable.range(100, 10)
values
    .elementAt(2)
    .dump()
/*
onSuccess: 102
*/
```

```
val values = Observable.range(100, 10)
values
    .elementAt(22, 0)
    .dump()
/*
onSuccess: 0
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of("🐱", "🐰", "🐶", "🐸", "🐷", "🐵")
    .elementAt(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐸
*/
```

### Filter / Where

[ReactiveX - Filter operator](http://reactivex.io/documentation/operators/filter.html)
[Reactive Extensions再入門 その７「LINQスタイルの拡張メソッド」](https://blog.okazuki.jp/entry/20111110/1320849106)

Filter / Where 过滤数据流：只发送源数据流中符合某个条件的数据。

![Filter](http://reactivex.io/documentation/operators/images/filter.c.png)
- RxNET

```
var oddNumbers = Observable.Range(0, 10)
    .Where(i => i % 2 == 0)
    .Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("Completed"));
/*
0
2
4
6
8
Completed
*/
```
- RxJava

```
val values = Observable.range(0, 10)
values
    .filter { v -> v % 2 == 0 }
    .dump()
/*
onNext: 0
onNext: 2
onNext: 4
onNext: 6
onNext: 8
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(
    "🐱", "🐰", "🐶",
    "🐸", "🐱", "🐰",
    "🐹", "🐸", "🐱")
    .filter {
        $0 == "🐱"
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐱
🐱
🐱
*/
```

### First / FirstOrDefault / Latest / MostRecent / Next / Single / SingleOrDefault

[ReactiveX - First operator](http://reactivex.io/documentation/operators/first.html)
[Reactive Extensions再入門 その２２「単一の値を取得するメソッド」](https://blog.okazuki.jp/entry/20120103/1325587713)
[Reactive Extensions再入門 その２４「単一の値を取得するメソッド その２」](https://blog.okazuki.jp/entry/20120109/1326122112)
[Reactive Extensions再入門 その１３「最後の値を取得するLatestとMostRecentメソッド」](https://blog.okazuki.jp/entry/20111205/1323056284)
[Reactive Extensions再入門 その１４「Nextメソッド」](https://blog.okazuki.jp/entry/20111205/1323099346)

First 只发送源数据流中的（满足条件的）第 1 项，如果数据不存在就报错。

Single 发送源数据流中的（满足条件的）唯一 1 项，如果数据不存在或者多于 1 项就报错。

FirstOrDefault / SingleOrDefault 当数据不存在时不报错，发送给定的缺省值。

Latest / Next 返回源数据流中最晚发送的数据。

MostRecent 返回源数据流中最近观察到的数据。数据不存在时返回缺省值。

![First](http://reactivex.io/documentation/operators/images/first.c.png)
![FirstOrDefault](http://reactivex.io/documentation/operators/images/firstOrDefault.png)
![Single](http://reactivex.io/documentation/operators/images/single.png)
![SingleOrDefault](http://reactivex.io/documentation/operators/images/singleOrDefault.png)
- RxNET
- First / FirstOrDefault / Single / SingleOrDefault 这些同步（阻塞）的操作符已经被废弃。
- 应该使用 FirstAsync / FirstOrDefaultAsync / SingleAsync / SingleOrDefaultAsync 这些异步的返回数据流的操作符。

```
Observable
    .Interval(TimeSpan.FromSeconds(2))
    .Select(i => "value is " + i)
    .FirstAsync().Subscribe(
        firstResult => {
            Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
            Console.WriteLine("firstResult: {0}", firstResult);
        },
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Timestamp 2018/07/27 11:20:28.57
Timestamp 2018/07/27 11:20:30.75
firstResult: value is 0
OnCompleted
*/
```

```
var noElementsSequence = new Subject<string>();
noElementsSequence.OnCompleted();
noElementsSequence.FirstAsync().Subscribe(
    i => Console.WriteLine("firstResult: {0}", i ?? "null"),
    ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
    () => Console.WriteLine("OnCompleted"));
/*
Exception: InvalidOperationException, Sequence contains no elements.
*/
```

```
Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Interval(TimeSpan.FromSeconds(2))
    .Select(i => "value is " + i)
    .FirstOrDefaultAsync().Subscribe(
        firstResult => {
            Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
            Console.WriteLine("firstResult: {0}", firstResult);
        },
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Timestamp 2018/07/27 11:20:34.736
Timestamp 2018/07/27 11:20:36.739
firstResult: value is 0
OnCompleted
*/
```

```
var noElementsSequence = new Subject<string>();
noElementsSequence.OnCompleted();
noElementsSequence.FirstOrDefaultAsync().Subscribe(
    i => Console.WriteLine("firstResult: {0}", i ?? "null"),
    ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
    () => Console.WriteLine("OnCompleted"));
/*
firstResult: null
OnCompleted
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(1, i => i == 1, i => ++i, i => i, _ => TimeSpan.FromSeconds(1))
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleAsync().Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:24:40.498
Dump 2018/07/27 12:24:41.804, Value = 1
singleResult: 1
OnCompleted
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(0, i => i < 2, i => ++i, i => i, i => TimeSpan.FromSeconds(1))
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleAsync().Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:24:44.284
Dump 2018/07/27 12:24:45.285, Value = 0
Dump 2018/07/27 12:24:46.285, Value = 1
Exception: InvalidOperationException, Sequence contains more than one element.
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(
        0, i => i < 5, i => ++i, i => i, i => TimeSpan.FromSeconds(1))
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleAsync(i => i == 3).Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:24:47.768
Dump 2018/07/27 12:24:48.772, Value = 0
Dump 2018/07/27 12:24:49.773, Value = 1
Dump 2018/07/27 12:24:50.773, Value = 2
Dump 2018/07/27 12:24:51.774, Value = 3
Dump 2018/07/27 12:24:52.774, Value = 4
singleResult: 3
OnCompleted
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(
        0, i => i < 5, i => ++i, i => i, i => TimeSpan.FromSeconds(1))
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleAsync(i => i > 10).Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:25:01.439
Dump 2018/07/27 12:25:02.441, Value = 0
Dump 2018/07/27 12:25:03.441, Value = 1
Dump 2018/07/27 12:25:04.442, Value = 2
Dump 2018/07/27 12:25:05.442, Value = 3
Dump 2018/07/27 12:25:06.442, Value = 4
Exception: InvalidOperationException, Sequence contains no matching element.
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(
        0, i => i < 5, i => ++i, i => i, i => TimeSpan.FromSeconds(1))
    .Select(i => i.ToString())
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleOrDefaultAsync(i => i == "3").Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult ?? "null"),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:25:08.681
Dump 2018/07/27 12:25:09.691, Value = 0
Dump 2018/07/27 12:25:10.692, Value = 1
Dump 2018/07/27 12:25:11.693, Value = 2
Dump 2018/07/27 12:25:12.694, Value = 3
Dump 2018/07/27 12:25:13.694, Value = 4
singleResult: 3
OnCompleted
*/
```

```
var s = new Subject<string>();
s.OnCompleted();
s.SingleOrDefaultAsync().Subscribe(
    singleResult => Console.WriteLine("singleResult: {0}", singleResult),
    ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
    () => Console.WriteLine("OnCompleted"));
/*
singleResult: null
OnCompleted
*/
```

```
Console.WriteLine("Start {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(
        0, i => i < 5, i => ++i, i => i, i => TimeSpan.FromSeconds(1))
    .Select(i => i.ToString())
    .Do(i => Console.WriteLine("Dump {0:yyyy/MM/dd HH:mm:ss.FFF}, Value = {1}", DateTime.Now, i))
    .SingleOrDefaultAsync().Subscribe(
        singleResult => Console.WriteLine("singleResult: {0}", singleResult),
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Start 2018/07/27 12:25:15.985
Dump 2018/07/27 12:25:16.986, Value = 0
Dump 2018/07/27 12:25:17.986, Value = 1
Exception: InvalidOperationException, Sequence contains more than one element.
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values
    .firstOrError()
    .dump()
values
    .filter { v -> v > 5 }
    .firstElement()
    .dump()
/*
onSuccess: 0
onSuccess: 6
*/
```

```
val values = Observable.empty<Long>()
values
    .filter { v -> v > 5 }
    .first(-1L)
    .dump()
/*
onSuccess: -1
*/
```

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values.take(10)
    .filter { v -> v == 5L } // Emits a result
    .singleOrError()
    .dump()
values
    .filter { v -> v == 5L } // Never emits
    .singleElement()
    .dump()
/*
onSuccess: 5
*/
```

```
val values = Observable.empty<Int>()
values
    .single(-1)
    .dump()
/*
onSuccess: -1
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of("🐱", "🐰", "🐶", "🐸", "🐷", "🐵")
    .single()
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐱
Unhandled error happened: Sequence contains more than one element.
*/
```

```
let disposeBag = DisposeBag()
Observable.of("🐱", "🐰", "🐶", "🐸", "🐷", "🐵")
    .single { $0 == "🐸" }
    .subscribe { print($0) }
    .disposed(by: disposeBag)
Observable.of("🐱", "🐰", "🐶", "🐱", "🐰", "🐶")
    .single { $0 == "🐰" }
    .subscribe { print($0) }
    .disposed(by: disposeBag)
Observable.of("🐱", "🐰", "🐶", "🐸", "🐷", "🐵")
    .single { $0 == "🔵" }
    .subscribe { print($0) }
    .disposed(by: disposeBag)
/*
next(🐸)
completed
next(🐰)
error(Sequence contains more than one element.)
error(Sequence doesn't contain any elements.)
*/
```

### IgnoreElements

[ReactiveX - IgnoreElements operator](http://reactivex.io/documentation/operators/ignoreelements.html)

IgnoreElements 忽略源数据流中的数据，仅仅发送源数据流的终止操作（结束或出错）。

![IgnoreElements](http://reactivex.io/documentation/operators/images/ignoreElements.c.png)
- RxNET

```
var subject = new Subject<int>();
var noElements = subject.IgnoreElements();
subject.Subscribe(
i => Console.WriteLine("subject.OnNext({0})", i),
() => Console.WriteLine("subject.OnCompleted()"));
noElements.Subscribe(
i => Console.WriteLine("noElements.OnNext({0})", i),
() => Console.WriteLine("noElements.OnCompleted()"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnCompleted();
/*
subject.OnNext(1)
subject.OnNext(2)
subject.OnNext(3)
subject.OnCompleted()
noElements.OnCompleted()
*/
```
- RxJava

```
val values = Observable.range(0, 10)
values
    .ignoreElements()
    .dump()
/*
onComplete
*/
```

### Last / LastOrDefault

[ReactiveX - Last operator](http://reactivex.io/documentation/operators/last.html)
[Reactive Extensions再入門 その２２「単一の値を取得するメソッド」](https://blog.okazuki.jp/entry/20120103/1325587713)

Last 只发送源数据流中的最后 1 项。源数据流中没有数据时报错。

LastOrDefault 当源数据流中没有数据时不报错，发送给定的缺省值。

![Last](http://reactivex.io/documentation/operators/images/last.c.png)
![LastOrDefault](http://reactivex.io/documentation/operators/images/lastOrDefault.png)
- RxNET
- Last / LastOrDefault 这些同步（阻塞）的操作符已经被废弃。
- 应该使用 LastAsync / LastOrDefaultAsync 这些异步的返回数据流的操作符。

```
Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(0, i => i < 5, i => ++i, i => "value is " + i, i => TimeSpan.FromSeconds(1))
    .LastAsync().Subscribe(
        lastResult => {
            Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
            Console.WriteLine("lastResult: {0}", lastResult);
        },
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Timestamp 2018/07/27 11:46:49.684
Timestamp 2018/07/27 11:46:54.807
lastResult: value is 4
OnCompleted
*/
```

```
var noElementsSequence = new Subject<string>();
noElementsSequence.OnCompleted();
noElementsSequence.LastAsync().Subscribe(
    i => Console.WriteLine("lastResult: {0}", i ?? "null"),
    ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
    () => Console.WriteLine("OnCompleted"));
/*
Exception: InvalidOperationException, Sequence contains no elements.
*/
```

```
Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
Observable
    .Generate(0, i => i < 5, i => ++i, i => "value is " + i, i => TimeSpan.FromSeconds(1))
    .LastOrDefaultAsync().Subscribe(
        lastResult => {
            Console.WriteLine("Timestamp {0:yyyy/MM/dd HH:mm:ss.FFF}", DateTime.Now);
            Console.WriteLine("lastResult: {0}", lastResult);
        },
        ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
        () => Console.WriteLine("OnCompleted"));
/*
Timestamp 2018/07/27 11:47:01.178
Timestamp 2018/07/27 11:47:06.181
lastResult: value is 4
OnCompleted
*/
```

```
var noElementsSequence = new Subject<string>();
noElementsSequence.OnCompleted();
noElementsSequence.LastOrDefaultAsync().Subscribe(
    i => Console.WriteLine("lastResult: {0}", i ?? "null"),
    ex => Console.WriteLine("Exception: {0}, {1}", ex.GetType().Name, ex.Message),
    () => Console.WriteLine("OnCompleted"));
/*
lastResult: null
OnCompleted
*/
```
- RxJava

```
val values = Observable.range(0, 10)
values
    .lastOrError()
    .dump()
values
    .filter { v -> v < 5 }
    .lastElement()
    .dump()
/*
onSuccess: 9
onSuccess: 4
*/
```

```
val values = Observable.empty<Int>()
values
    .filter { v -> v > 5 }
    .last(-1)
    .dump()
/*
onSuccess: -1
*/
```

### Sample

[ReactiveX - Sample operator](http://reactivex.io/documentation/operators/sample.html)
[Reactive Extensions再入門 その２７「時間でフィルタリング？Sampleメソッド」](https://blog.okazuki.jp/entry/20120201/1328107196)

Sample 过滤数据流：在每一个指定的时间段只取最后一个数据。

![Sample](http://reactivex.io/documentation/operators/images/sample.c.png)
- RxNET

```
var interval = Observable.Interval(TimeSpan.FromMilliseconds(150));
using (interval.Sample(TimeSpan.FromSeconds(1))
        .Subscribe(Console.WriteLine))
    Console.ReadLine();
/*
5
11
18
24
31
*/
```
- RxJava

```
Observable.interval(150, TimeUnit.MILLISECONDS)
    .sample(1, TimeUnit.SECONDS)
    .take(3)
    .dump()
/*
onNext: 5
onNext: 12
onNext: 18
onComplete
*/
```

### Skip

[ReactiveX - Skip operator](http://reactivex.io/documentation/operators/skip.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

Skip 跳过前 n 项 只发送源数据流中其余各项。

![Skip](http://reactivex.io/documentation/operators/images/skip.c.png)
- RxNET

```
Observable.Range(0, 10)
    .Skip(3)
    .Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
/*
3
4
...
8
9
Completed
*/
```
- RxJava

```
val values = Observable.range(0, 5)
values
    .skip(2)
    .dump()
/*
onNext: 2
onNext: 3
onNext: 4
onComplete
*/
```

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
val d = values
    .skip(250, TimeUnit.MILLISECONDS)
    .dump()
readLine()
d.dispose()
/*
onNext: 2
onNext: 3
onNext: 4
...
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(
    "🐱", "🐰", "🐶",
    "🐸", "🐱", "🐰",
    "🐹", "🐸", "🐱")
    .skip(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐸
🐱
🐰
🐹
🐸
🐱
*/
```

### SkipLast

[ReactiveX - SkipLast operator](http://reactivex.io/documentation/operators/skiplast.html)
[Reactive Extensions再入門 その３３「シーケンスの最後を起点にSkipとTake」](https://blog.okazuki.jp/entry/20120209/1328799859)

SkipLast 跳过最后 n 项 只发送源数据流中其余各项。

![SkipLast](http://reactivex.io/documentation/operators/images/skipLast.c.png)
- RxNET

```
var subject = new Subject<int>();
subject
.SkipLast(2)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
Console.WriteLine("Pushing 1");
subject.OnNext(1);
Console.WriteLine("Pushing 2");
subject.OnNext(2);
Console.WriteLine("Pushing 3");
subject.OnNext(3);
Console.WriteLine("Pushing 4");
subject.OnNext(4);
subject.OnCompleted();
/*
Pushing 1
Pushing 2
Pushing 3
1
Pushing 4
2
Completed
*/
```
- RxJava

```
val values = Observable.range(0, 5)
values
    .skipLast(2)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onComplete
*/
```

### Take

[ReactiveX - Take operator](http://reactivex.io/documentation/operators/take.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

Take 只发送源数据流中的前 n 项。

![Take](http://reactivex.io/documentation/operators/images/take.c.png)
- RxJava

```
Observable.range(0, 10)
    .take(3)
    .subscribe({ println("onNext: " + it) }, { throwable -> println("onError") }, { println("onComplete") })
/*
onNext: 0
onNext: 1
onNext: 2
onComplete
*/
```
- RxNET

```
Observable.Range(0, 10)
    .Take(3)
    .Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
/*
0
1
2
Completed
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(
    "🐱", "🐰", "🐶",
    "🐸", "🐱", "🐰",
    "🐹", "🐸", "🐱")
    .take(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐱
🐰
🐶
*/
```

### TakeLast

[ReactiveX - TakeLast operator](http://reactivex.io/documentation/operators/takelast.html)
[Reactive Extensions再入門 その３３「シーケンスの最後を起点にSkipとTake」](https://blog.okazuki.jp/entry/20120209/1328799859)

TakeLast 只发送源数据流中的最后 n 项。

![TakeLast](http://reactivex.io/documentation/operators/images/takeLast.c.png)
- RxNET

```
var subject = new Subject<int>();
subject
.TakeLast(2)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
Console.WriteLine("Pushing 1");
subject.OnNext(1);
Console.WriteLine("Pushing 2");
subject.OnNext(2);
Console.WriteLine("Pushing 3");
subject.OnNext(3);
Console.WriteLine("Pushing 4");
subject.OnNext(4);
subject.OnCompleted();
/*
Pushing 1
Pushing 2
Pushing 3
Pushing 4
3
4
Completed
*/
```
- RxJava

```
val values = Observable.range(0, 5)
values
    .takeLast(2)
    .dump()
/*
onNext: 3
onNext: 4
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(
    "🐱", "🐰", "🐶",
    "🐸", "🐱", "🐰",
    "🐹", "🐸", "🐱")
    .takeLast(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐹
🐸
🐱
*/
```


