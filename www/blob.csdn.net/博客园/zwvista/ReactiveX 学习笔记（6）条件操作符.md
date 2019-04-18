# ReactiveX 学习笔记（6）条件操作符 - zwvista - 博客园

## [ReactiveX 学习笔记（6）条件操作符](https://www.cnblogs.com/zwvista/p/9360298.html)

### Conditional and Boolean Operators

本文的主题为处理 Observable 的条件和布尔操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（七）Conditional and Boolean](http://mushuichuan.com/2015/12/14/rxjava-operator-7/)

### 公共代码

```
fun <T> Observable<T>.dump() =
    this.subscribe({ println("onNext: $it") },
        { println("onError: $it: ${it.message}") },
        { println("onComplete") })
fun <T> Single<T>.dump() =
    this.subscribe({ println("onNext: $it") },
        { println("onError: $it: ${it.message}") })
```

### All

[ReactiveX - All operator](http://reactivex.io/documentation/operators/all.html)
[Reactive Extensions再入門 その１９「AnyメソッドとAllメソッド」](https://blog.okazuki.jp/entry/20111215/1323959456)

All 检查一个数据流的所有数据是否都满足某个条件。

![All](http://reactivex.io/documentation/operators/images/all.C.png)
- RxNET

```
var subject = new Subject<int>();
subject.Subscribe(Console.WriteLine, () => Console.WriteLine("Subject completed"));
var all = subject.All(i => i < 5);
all.Subscribe(b => Console.WriteLine("All values less than 5? {0}", b));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(6);
subject.OnNext(2);
subject.OnNext(1);
subject.OnCompleted();
/*
1
2
6
All values less than 5? False
2
1
Subject completed
*/
```
- RxJava

```
val values = Observable.create<Int> { o ->
    o.onNext(0)
    o.onNext(10)
    o.onNext(10)
    o.onNext(2)
    o.onComplete()
}
values
    .all { i -> i % 2 == 0 }
    .dump()
/*
onSuccess: true
*/
```

```
val values = Observable.interval(150, TimeUnit.MILLISECONDS).take(5)
val subscription = values
    .all { i -> i < 3 }
    .dump()
val subscription2 = values
    .dump()
readLine()
subscription.dispose()
subscription2.dispose()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onSuccess: false
onNext: 4
onComplete
*/
```

```
val values = Observable.create<Int> { o ->
    o.onNext(0)
    o.onNext(2)
    o.onError(Exception())
}
values
    .all { i -> i % 2 == 0 }
    .dump()
/*
onError: java.lang.Exception: null
*/
```
- RxJS

[RxJs - every](https://rxjs-dev.firebaseapp.com/api/operators/every)
[learn-rxjs - every](https://www.learnrxjs.io/operators/conditional/every.html)

### Amb

[ReactiveX - Amb operator](http://reactivex.io/documentation/operators/amb.html)
[Reactive Extensions再入門 その４１「どんどん合成するよ」](http://reactivex.io/documentation/operators/replay.html)

给定两个或多个数据流，Amb 只发送最早发送数据的数据流的所有数据。

![Amb](http://reactivex.io/documentation/operators/images/amb.C.png)
- RxNET

```
/*
    s1 -1--1--|
    s2 --2--2--|
    s3 ---3--3--|
    r  -1--1--|
*/
var s1 = new Subject<int>();
var s2 = new Subject<int>();
var s3 = new Subject<int>();
var result = Observable.Amb(s1, s2, s3);
result.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Completed"));
s1.OnNext(1);
s2.OnNext(2);
s3.OnNext(3);
s1.OnNext(1);
s2.OnNext(2);
s3.OnNext(3);
s1.OnCompleted();
s2.OnCompleted();
s3.OnCompleted();
/*
1
1
Completed
*/
```

```
/*
    s1 ---1--|
    s2 -2--2--|
    s3 --3--3--|
    r  -2--2--|
*/
var s1 = new Subject<int>();
var s2 = new Subject<int>();
var s3 = new Subject<int>();
var result = Observable.Amb(s1, s2, s3);
result.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Completed"));
// s1.OnNext(1);
s2.OnNext(2);
s3.OnNext(3);
s1.OnNext(1);
s2.OnNext(2);
s3.OnNext(3);
s1.OnCompleted();
s2.OnCompleted();
s3.OnCompleted();
/*
2
2
Completed
*/
```
- RxJava

![amb](http://reactivex.io/documentation/operators/images/amb.png)
![ambArray](http://reactivex.io/documentation/operators/images/ambArray.png)
![ambWith](http://reactivex.io/documentation/operators/images/ambWith.png)

```
Observable.amb(listOf(
    Observable.timer(100, TimeUnit.MILLISECONDS).map { i -> "First" },
    Observable.timer(50, TimeUnit.MILLISECONDS).map { i -> "Second" },
    Observable.timer(70, TimeUnit.MILLISECONDS).map { i -> "Third" }))
    .dump()
/*
onNext: Second
onComplete
*/
```

```
Observable.ambArray(
    Observable.timer(100, TimeUnit.MILLISECONDS).map { i -> "First" },
    Observable.timer(50, TimeUnit.MILLISECONDS).map { i -> "Second" },
    Observable.timer(70, TimeUnit.MILLISECONDS).map { i -> "Third" })
    .dump()
/*
onNext: Second
onComplete
*/
```

```
Observable.timer(100, TimeUnit.MILLISECONDS).map { i -> "First" }
    .ambWith(Observable.timer(50, TimeUnit.MILLISECONDS).map { i -> "Second" })
    .ambWith(Observable.timer(70, TimeUnit.MILLISECONDS).map { i -> "Third" })
    .dump()
/*
onNext: Second
onComplete
*/
```
- RxJS

[RxJs - race](https://rxjs-dev.firebaseapp.com/api/operators/race)
[learn-rxjs - race](https://www.learnrxjs.io/operators/combination/race.html)

### Contains / Any / Exists / IsEmpty

[ReactiveX - Contains operator](http://reactivex.io/documentation/operators/contains.html)
[Reactive Extensions再入門 その１９「AnyメソッドとAllメソッド」](https://blog.okazuki.jp/entry/20111215/1323959456)

Contains 检查一个数据流是否发送了某个特定数据。

Any / Exists 检查一个数据流是否发送了（满足特定条件的）数据。

IsEmpty 检查一个数据流是否没有发送任何数据。

![Contains](http://reactivex.io/documentation/operators/images/contains.c.png)
![IsEmpty](http://reactivex.io/documentation/operators/images/isEmpty.png)
- RxNET

```
var subject = new Subject<int>();
subject.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Subject completed"));
var contains = subject.Contains(2);
contains.Subscribe(
b => Console.WriteLine("Contains the value 2? {0}", b),
() => Console.WriteLine("contains completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnCompleted();
/*
1
2
Contains the value 2? True
contains completed
3
Subject completed
*/
```

```
var subject = new Subject<int>();
subject.Subscribe(Console.WriteLine, () => Console.WriteLine("Subject completed"));
var any = subject.Any();
any.Subscribe(b => Console.WriteLine("The subject has any values? {0}", b));
subject.OnNext(1);
subject.OnCompleted();
/*
1
The subject has any values? True
Subject completed
*/
```
- RxJava
- exists 已经改名为 any

![exists](http://reactivex.io/documentation/operators/images/exists.png)

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values
    .contains(4L)
    .dump()
/*
onSuccess: true
*/
```

```
val values = Observable.range(0, 2)
values
    .any { i -> i > 2 }
    .dump()
/*
onSuccess: false
*/
```

```
val values = Observable.range(0, 2)
values
    .any { i -> i > 2 }
    .dump()
/*
onSuccess: false
*/
```

```
val values = Observable.timer(1000, TimeUnit.MILLISECONDS)
values
    .isEmpty
    .dump()
/*
onSuccess: false
*/
```

### DefaultIfEmpty

[ReactiveX - DefaultIfEmpty operator](http://reactivex.io/documentation/operators/defaultifempty.html)

DefaultIfEmpty 发送源数据流的所有数据，当源数据流没有数据时发送缺省的某个数据。

![DefaultIfEmpty](http://reactivex.io/documentation/operators/images/defaultIfEmpty.c.png)
- RxNET

```
var subject = new Subject<int>();
subject.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Subject completed"));
var defaultIfEmpty = subject.DefaultIfEmpty();
defaultIfEmpty.Subscribe(
b => Console.WriteLine("defaultIfEmpty value: {0}", b),
() => Console.WriteLine("defaultIfEmpty completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnCompleted();
/*
1
defaultIfEmpty value: 1
2
defaultIfEmpty value: 2
3
defaultIfEmpty value: 3
Subject completed
defaultIfEmpty completed
*/
```

```
var subject = new Subject<int>();
subject.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Subject completed"));
var defaultIfEmpty = subject.DefaultIfEmpty();
defaultIfEmpty.Subscribe(
b => Console.WriteLine("defaultIfEmpty value: {0}", b),
() => Console.WriteLine("defaultIfEmpty completed"));
var default42IfEmpty = subject.DefaultIfEmpty(42);
default42IfEmpty.Subscribe(
b => Console.WriteLine("default42IfEmpty value: {0}", b),
() => Console.WriteLine("default42IfEmpty completed"));
subject.OnCompleted();
/*
Subject completed
defaultIfEmpty value: 0
defaultIfEmpty completed
default42IfEmpty value: 42
default42IfEmpty completed
*/
```
- RxJava

```
val values = Observable.empty<Int>()
values
    .defaultIfEmpty(2)
    .dump()
/*
onNext: 2
onComplete
*/
```

```
val values = Observable.error<Int>(Exception())
values
    .defaultIfEmpty(2)
    .dump()
/*
onError: java.lang.Exception: null
*/
```

### SequenceEqual

[ReactiveX - SequenceEqual operator](http://reactivex.io/documentation/operators/sequenceequal.html)

SequenceEqual 判断两个数据流是否发送完全相同的数据。

![SequenceEqual](http://reactivex.io/documentation/operators/images/sequenceEqual.c.png)
- RxNET

```
var subject1 = new Subject<int>();
subject1.Subscribe(
i => Console.WriteLine("subject1.OnNext({0})", i),
() => Console.WriteLine("subject1 completed"));
var subject2 = new Subject<int>();
subject2.Subscribe(
i => Console.WriteLine("subject2.OnNext({0})", i),
() => Console.WriteLine("subject2 completed"));
var areEqual = subject1.SequenceEqual(subject2);
areEqual.Subscribe(
i => Console.WriteLine("areEqual.OnNext({0})", i),
() => Console.WriteLine("areEqual completed"));
subject1.OnNext(1);
subject1.OnNext(2);
subject2.OnNext(1);
subject2.OnNext(2);
subject2.OnNext(3);
subject1.OnNext(3);
subject1.OnCompleted();
subject2.OnCompleted();
/*
subject1.OnNext(1)
subject1.OnNext(2)
subject2.OnNext(1)
subject2.OnNext(2)
subject2.OnNext(3)
subject1.OnNext(3)
subject1 completed
subject2 completed
areEqual.OnNext(True)
areEqual completed
*/
```
- RxJava

```
val strings = Observable.just("1", "2", "3")
val ints = Observable.just(1, 2, 3)
Observable.sequenceEqual(strings, ints, BiPredicate { s, i -> s == i.toString() } )
    .dump()
/*
onSuccess: true
*/
```

```
val strings = Observable.just("1", "2", "3")
val ints = Observable.just(1, 2, 3)
Observable.sequenceEqual(strings, ints)
    .dump()
/*
onSuccess: false
*/
```

```
val values = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onError(Exception())
}
Observable.sequenceEqual(values, values)
    .dump()
/*
onError: java.lang.Exception: null
*/
```

### SkipUntil

[ReactiveX - SkipUntil operator](http://reactivex.io/documentation/operators/skipuntil.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

SkipUntil 忽略源数据流的数据，直到另一个数据流开始发送数据后才开始发送源数据流的数据。

![SkipUntil](http://reactivex.io/documentation/operators/images/skipUntil.c.png)
- RxNET

```
var subject = new Subject<int>();
var otherSubject = new Subject<Unit>();
subject
.SkipUntil(otherSubject)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
otherSubject.OnNext(Unit.Default);
subject.OnNext(4);
subject.OnNext(5);
subject.OnNext(6);
subject.OnNext(7);
subject.OnNext(8);
subject.OnCompleted();
/*
4
5
6
7
8
Completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
val cutoff = Observable.timer(250, TimeUnit.MILLISECONDS)
val subscription = values
    .skipUntil(cutoff)
    .dump()
readLine()
subscription.dispose()
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
Observable.of(1, 2, 3, 4, 5, 6)
    .takeWhile { $0 < 4 }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
1
2
3
*/
```

### SkipWhile

[ReactiveX - SkipWhile operator](http://reactivex.io/documentation/operators/skipwhile.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

SkipWhile 忽略源数据流的数据，直到某个条件不再满足时才开始发送源数据流的数据。

![SkipWhile](http://reactivex.io/documentation/operators/images/skipWhile.c.png)
- RxNET

```
var subject = new Subject<int>();
subject
.SkipWhile(i => i < 4)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnNext(4);
subject.OnNext(3);
subject.OnNext(2);
subject.OnNext(1);
subject.OnNext(0);
subject.OnCompleted();
/*
4
3
2
1
0
Completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
val subscription = values
    .skipWhile { v -> v < 2 }
    .dump()
readLine()
subscription.dispose()
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
Observable.of(1, 2, 3, 4, 5, 6)
    .skipWhile { $0 < 4 }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
4
5
6
*/
```

```
let disposeBag = DisposeBag()
Observable.of("🐱", "🐰", "🐶", "🐸", "🐷", "🐵")
    .skipWhileWithIndex { element, index in
        index < 3
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐸
🐷
🐵
*/
```

### TakeUntil

[ReactiveX - TakeUntil operator](http://reactivex.io/documentation/operators/takeuntil.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

TakeUntil 发送源数据流的数据，直到另一个数据流开始发送数据之后停止发送。

![TakeUntil](http://reactivex.io/documentation/operators/images/takeUntil.c.png)
- RxNET

```
var subject = new Subject<int>();
var otherSubject = new Subject<Unit>();
subject
.TakeUntil(otherSubject)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
otherSubject.OnNext(Unit.Default);
subject.OnNext(4);
subject.OnNext(5);
subject.OnNext(6);
subject.OnNext(7);
subject.OnNext(8);
subject.OnCompleted();
/*
1
2
3
Completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
val cutoff = Observable.timer(250, TimeUnit.MILLISECONDS)
val subscription = values
    .takeUntil(cutoff)
    .dump()
readLine()
subscription.dispose()
/*
onNext: 0
onNext: 1
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let sourceSequence = PublishSubject<String>()
let referenceSequence = PublishSubject<String>()
sourceSequence
    .takeUntil(referenceSequence)
    .subscribe { print($0) }
    .disposed(by: disposeBag)
sourceSequence.onNext("🐱")
sourceSequence.onNext("🐰")
sourceSequence.onNext("🐶")
referenceSequence.onNext("🔴")
sourceSequence.onNext("🐸")
sourceSequence.onNext("🐷")
sourceSequence.onNext("🐵")
/*
next(🐱)
next(🐰)
next(🐶)
completed
*/
```

### TakeWhile

[ReactiveX - TakeWhile operator](http://reactivex.io/documentation/operators/takewhile.html)
[Reactive Extensions再入門 その８「SkipとTakeメソッド」](https://blog.okazuki.jp/entry/20111113/1321191314)

TakeWhile 发送源数据流的数据，直到某个条件不再满足后停止发送。

![TakeWhile](http://reactivex.io/documentation/operators/images/takeWhile.c.png)
- RxNET

```
var subject = new Subject<int>();
subject
.TakeWhile(i => i < 4)
.Subscribe(Console.WriteLine, () => Console.WriteLine("Completed"));
subject.OnNext(1);
subject.OnNext(2);
subject.OnNext(3);
subject.OnNext(4);
subject.OnNext(3);
subject.OnNext(2);
subject.OnNext(1);
subject.OnNext(0);
subject.OnCompleted();
/*
1
2
3
Completed
*/
```
- RxJava

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
val subscription = values
    .takeWhile { v -> v < 2 }
    .dump()
readLine()
subscription.dispose()
/*
onNext: 0
onNext: 1
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(1, 2, 3, 4, 5, 6)
    .takeWhile { $0 < 4 }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
1
2
3
*/
```


