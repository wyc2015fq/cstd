# ReactiveX 学习笔记（5）合并数据流 - zwvista - 博客园

## [ReactiveX 学习笔记（5）合并数据流](https://www.cnblogs.com/zwvista/p/9335553.html)

### Combining Observables

本文的主题为合并 Observable 的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（四）Combining](http://mushuichuan.com/2015/12/11/rxjava-operator-4/)

### And/Then/When

[ReactiveX - And/Then/When operator](http://reactivex.io/documentation/operators/and-then-when.html)
[Reactive Extensions再入門 その４４「And, Then, Whenメソッド」](https://blog.okazuki.jp/entry/20120222/1329923070)

And/Then/When 将两个或多个数据流的数据通过 Pattern 和 Plan 中间量合为一体。

And/Then/When 操作符中数据结合的方法为将所有源数据流的第 1 个数据结合，然后再将所有源数据流的第 2 个数据结合，以此类推，直到某一个源数据流没有数据为止。

![And/Then/When](http://reactivex.io/documentation/operators/images/and_then_when.C.png)
- RxNET

```
var one = Observable.Interval(TimeSpan.FromSeconds(1)).Take(5);
var two = Observable.Interval(TimeSpan.FromMilliseconds(250)).Take(10);
var three = Observable.Interval(TimeSpan.FromMilliseconds(150)).Take(14);
var zippedSequence = Observable.When(
    one.And(two)
    .And(three)
    .Then((first, second, third) =>
    new {
        One = first,
        Two = second,
        Three = third
    })
);
zippedSequence.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Completed"));
Console.ReadLine();
/*
{ One = 0, Two = 0, Three = 0 }
{ One = 1, Two = 1, Three = 1 }
{ One = 2, Two = 2, Three = 2 }
{ One = 3, Two = 3, Three = 3 }
{ One = 4, Two = 4, Three = 4 }
Completed
*/
```

### CombineLatest

[ReactiveX - CombineLatest operator](http://reactivex.io/documentation/operators/combinelatest.html)
[Reactive Extensions再入門 その４１「どんどん合成するよ」](http://reactivex.io/documentation/operators/replay.html)

CombineLatest 将两个数据流中的最新数据通过调用指定函数合为一体。

![CombineLatest](http://reactivex.io/documentation/operators/images/combineLatest.c.png)
- RxNET

```
var source1 = new Subject<string>();
var source2 = new Subject<int>();
source1
    .CombineLatest(
        source2,
        (l, r) => string.Format("{0}-{1}", l, r))
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        () => Console.WriteLine("OnCompleted"));
Console.WriteLine("source1.OnNext(foo)");
source1.OnNext("foo");
Console.WriteLine("source2.OnNext(100)");
source2.OnNext(100);
Console.WriteLine("source2.OnNext(200)");
source2.OnNext(200);
Console.WriteLine("source1.OnNext(bar)");
source1.OnNext("bar");
Console.WriteLine("source1.OnNext(boo)");
source1.OnNext("boo");
Console.WriteLine("source1.OnCompleted()");
source1.OnCompleted();
Console.WriteLine("source2.OnNext(999)");
source2.OnNext(999);
Console.WriteLine("source2.OnCompleted()");
source2.OnCompleted();
/*
source1.OnNext(foo)
source2.OnNext(100)
OnNext: foo-100
source2.OnNext(200)
OnNext: foo-200
source1.OnNext(bar)
OnNext: bar-200
source1.OnNext(boo)
OnNext: boo-200
source1.OnCompleted()
source2.OnNext(999)
OnNext: boo-999
source2.OnCompleted()
OnCompleted
*/
```
- RxJava

```
Observables.combineLatest(
    Observable.interval(100, TimeUnit.MILLISECONDS)
        .doOnNext { i -> println("Left emits") },
    Observable.interval(150, TimeUnit.MILLISECONDS)
        .doOnNext { i -> println("Right emits") })
    { i1, i2 -> i1.toString() + " - " + i2 }
    .take(6)
    .dump()
/*
Left emits
Right emits
onNext: 0 - 0
Left emits
onNext: 1 - 0
Left emits
Right emits
onNext: 2 - 0
onNext: 2 - 1
Left emits
onNext: 3 - 1
Right emits
onNext: 3 - 2
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let stringSubject = PublishSubject<String>()
let intSubject = PublishSubject<Int>()
Observable.combineLatest(stringSubject, intSubject) { stringElement, intElement in
        "\(stringElement) \(intElement)"
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
stringSubject.onNext("🅰️")
stringSubject.onNext("🅱️")
intSubject.onNext(1)
intSubject.onNext(2)
stringSubject.onNext("🆎")
/*
🅱️ 1
🅱️ 2
🆎 2
*/
```

```
let disposeBag = DisposeBag()
let stringObservable = Observable.just("❤️")
let fruitObservable = Observable.from(["🍎", "🍐", "🍊"])
let animalObservable = Observable.of("🐶", "🐱", "🐭", "🐹")
Observable.combineLatest([stringObservable, fruitObservable, animalObservable]) {
        "\($0[0]) \($0[1]) \($0[2])"
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
❤️ 🍎 🐶
❤️ 🍐 🐶
❤️ 🍐 🐱
❤️ 🍊 🐱
❤️ 🍊 🐭
❤️ 🍊 🐹
*/
```

### Join / GroupJoin

[ReactiveX - Join operator](http://reactivex.io/documentation/operators/join.html)
[Reactive Extensions再入門 その４２「StartWithメソッドとJoinメソッド」](https://blog.okazuki.jp/entry/20120220/1329742486)
[Reactive Extensions再入門 その４３「GroupJoinメソッド」](https://blog.okazuki.jp/entry/20120220/1329748492)

![Join](http://reactivex.io/documentation/operators/images/join.c.png)
![GroupJoin](http://reactivex.io/documentation/operators/images/groupJoin.png)
- RxNET

```
{
    var left = new Subject<int>();
    var right = new Subject<int>();
    left.Join(
        right,
        _ => Observable.Never<Unit>(),
        _ => Observable.Never<Unit>(),
        Tuple.Create)
        .Subscribe(
            tuple => Console.WriteLine("Left: {0}, Right: {1}", tuple.Item1, tuple.Item2),
            () => Console.WriteLine("OnCompleted"));
    Console.WriteLine("left.OnNext(1)");
    left.OnNext(1);
    Console.WriteLine("right.OnNext(10)");
    right.OnNext(10);
    Console.WriteLine("right.OnNext(100)");
    right.OnNext(100);
    Console.WriteLine("left.OnNext(2)");
    left.OnNext(2);
    Console.WriteLine("left.OnCompleted()");
    left.OnCompleted();
    Console.WriteLine("right.OnCompleted()");
    right.OnCompleted();
}
Console.WriteLine("------------------------------");
{
    var left = new Subject<int>();
    var right = new Subject<int>();
    left.Join(
        right,
        _ => Observable.Never<Unit>(),
        _ => Observable.Empty<Unit>(),
        Tuple.Create)
        .Subscribe(
            tuple => Console.WriteLine("Left: {0}, Right: {1}", tuple.Item1, tuple.Item2),
            () => Console.WriteLine("OnCompleted"));
    Console.WriteLine("left.OnNext(1)");
    left.OnNext(1);
    Console.WriteLine("right.OnNext(10)");
    right.OnNext(10);
    Console.WriteLine("right.OnNext(100)");
    right.OnNext(100);
    Console.WriteLine("left.OnNext(2)");
    left.OnNext(2);
    Console.WriteLine("right.OnNext(1000)");
    right.OnNext(1000);
    Console.WriteLine("left.OnCompleted()");
    left.OnCompleted();
    Console.WriteLine("right.OnCompleted()");
    right.OnCompleted();
}
Console.WriteLine("------------------------------");
/*
left.OnNext(1)
right.OnNext(10)
Left: 1, Right: 10
right.OnNext(100)
Left: 1, Right: 100
left.OnNext(2)
Left: 2, Right: 10
Left: 2, Right: 100
left.OnCompleted()
right.OnCompleted()
OnCompleted
------------------------------
left.OnNext(1)
right.OnNext(10)
Left: 1, Right: 10
right.OnNext(100)
Left: 1, Right: 100
left.OnNext(2)
right.OnNext(1000)
Left: 1, Right: 1000
Left: 2, Right: 1000
left.OnCompleted()
right.OnCompleted()
OnCompleted
------------------------------
*/
```

```
var sensors = new Subject<string>();
var values = new Subject<int>();
var valueReset = new Subject<Unit>();
sensors.GroupJoin(
    values,
    _ => Observable.Never<Unit>(),
    _ => valueReset,
    (l, r) => new { Name = l, Log = r.Scan((x, y) => x + y) })
    .Subscribe(
        sensor =>
        {
            sensor
                .Log
                .Subscribe(i => Console.WriteLine("{0}: {1}", sensor.Name, i));
        },
        () => Console.WriteLine("OnCompleted"));
Console.WriteLine("sensors.OnNext(sensor1)");
sensors.OnNext("sensor1");
Console.WriteLine("sensors.OnNext(sensor2)");
sensors.OnNext("sensor2");
Console.WriteLine("values.OnNext(100)");
values.OnNext(100);
Console.WriteLine("values.OnNext(10)");
values.OnNext(10);
Console.WriteLine("values.OnNext(1)");
values.OnNext(1);
Console.WriteLine("valueReset.OnNext()");
valueReset.OnNext(Unit.Default);
Console.WriteLine("sensors.OnNext(sensor3)");
sensors.OnNext("sensor3");
Console.WriteLine("values.OnNext(1)");
values.OnNext(1);
Console.WriteLine("values.OnNext(2)");
values.OnNext(2);
Console.WriteLine("values.OnNext(3)");
values.OnNext(3);
Console.WriteLine("values.OnCompleted()");
values.OnCompleted();
Console.WriteLine("sensors.OnCompleted()");
sensors.OnCompleted();
/*
sensors.OnNext(sensor1)
sensors.OnNext(sensor2)
values.OnNext(100)
sensor1: 100
sensor2: 100
values.OnNext(10)
sensor1: 110
sensor2: 110
values.OnNext(1)
sensor1: 111
sensor2: 111
valueReset.OnNext()
sensors.OnNext(sensor3)
values.OnNext(1)
sensor1: 112
sensor2: 112
sensor3: 1
values.OnNext(2)
sensor1: 114
sensor2: 114
sensor3: 3
values.OnNext(3)
sensor1: 117
sensor2: 117
sensor3: 6
values.OnCompleted()
sensors.OnCompleted()
OnCompleted
*/
```
- RxJava

```
val left = Observable.interval(100, TimeUnit.MILLISECONDS)
    .map { i -> "L$i" }
val right = Observable.interval(200, TimeUnit.MILLISECONDS)
    .map { i -> "R$i" }
left
    .join<String, Any, Long, String>(
        right,
        Function { i -> Observable.never() },
        Function { i -> Observable.timer(0, TimeUnit.MILLISECONDS) },
        BiFunction { l, r -> "$l - $r" }
    )
    .take(10)
    .dump()
/*
onNext: L0 - R0
onNext: L1 - R0
onNext: L0 - R1
onNext: L1 - R1
onNext: L2 - R1
onNext: L3 - R1
onNext: L0 - R2
onNext: L1 - R2
onNext: L2 - R2
onNext: L3 - R2
onComplete
*/
```

```
val left = Observable.interval(100, TimeUnit.MILLISECONDS)
    .map { i -> "L$i" }
val right = Observable.interval(100, TimeUnit.MILLISECONDS)
    .map { i -> "R$i" }
left
    .join<String, Long, Long, String>(
        right,
        Function {  i -> Observable.timer(150, TimeUnit.MILLISECONDS) },
        Function { i -> Observable.timer(0, TimeUnit.MILLISECONDS) },
        BiFunction { l, r -> "$l - $r" }
    )
    .take(10)
    .dump()
/*
onNext: L0 - R0
onNext: L0 - R1
onNext: L1 - R1
onNext: L1 - R2
onNext: L2 - R2
onNext: L2 - R3
onNext: L3 - R3
onNext: L3 - R4
onNext: L4 - R4
onNext: L4 - R5
onComplete
*/
```

```
val left = Observable.interval(100, TimeUnit.MILLISECONDS)
    .map { i -> "L$i" }
    .take(6)
val right = Observable.interval(200, TimeUnit.MILLISECONDS)
    .map { i -> "R$i" }
    .take(3)
left
    .groupJoin<String, Any, Long, Any>(
        right,
        Function { i -> Observable.never<Any>() },
        Function { i -> Observable.timer(0, TimeUnit.MILLISECONDS) },
        BiFunction { l, rs -> rs.toList().subscribe { list -> println("$l: $list") } }
    )
    .subscribe()
/*
L0: [R0, R1, R2]
L1: [R0, R1, R2]
L2: [R1, R2]
L3: [R1, R2]
L4: [R2]
L5: [R2]
*/
```

### Merge

[ReactiveX - Merge operator](http://reactivex.io/documentation/operators/merge.html)
[Reactive Extensions再入門 その４０「IObservableの合成はじめました」](https://blog.okazuki.jp/entry/20120218/1329581391)

Merge 将多个数据流的数据按照数据的先后顺序合并成一个数据流。

![Merge](http://reactivex.io/documentation/operators/images/merge.C.png)
- RxNET

```
/*
    s1 ----0----0----0| 
    s2 --0--0--0--0--0|
    sR --0-00--00-0--00|
*/
//Generate values 0,1,2 
var s1 = Observable.Interval(TimeSpan.FromMilliseconds(250))
.Take(3);
//Generate values 100,101,102,103,104 
var s2 = Observable.Interval(TimeSpan.FromMilliseconds(150))
.Take(5)
.Select(i => i + 100);
s1.Merge(s2)
.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Completed"));
Console.ReadLine();
/*
100
0
101
102
1
103
2
104
Completed
*/
```
- RxJava

```
Observable.merge(
    Observable.interval(250, TimeUnit.MILLISECONDS).map { i -> "First" },
    Observable.interval(150, TimeUnit.MILLISECONDS).map { i -> "Second" })
    .take(10)
    .dump()
/*
onNext: Second
onNext: First
onNext: Second
onNext: Second
onNext: First
onNext: Second
onNext: Second
onNext: First
onNext: Second
onNext: First
onComplete
*/
```

```
Observable.interval(250, TimeUnit.MILLISECONDS).map { i -> "First" }
    .mergeWith(Observable.interval(150, TimeUnit.MILLISECONDS).map { i -> "Second" })
    .take(10)
    .dump()
/*
onNext: Second
onNext: First
onNext: Second
onNext: Second
onNext: First
onNext: Second
onNext: Second
onNext: First
onNext: Second
onNext: First
onComplete
*/
```
- mergeDelayError

![mergeDelayError](http://reactivex.io/documentation/operators/images/mergeDelayError.C.png)

```
val failAt200 = Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(2),
    Observable.error(Exception("Failed")))
val completeAt400 = Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(4)
Observable.mergeDelayError(failAt200, completeAt400)
    .dump()
/*
onNext: 0
onNext: 0
onNext: 1
onNext: 1
onNext: 2
onNext: 3
onError: java.lang.Exception: Failed: Failed
*/
```

```
val failAt200 = Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(2),
    Observable.error(Exception("Failed")))
val failAt300 = Observable.concat(
    Observable.interval(100, TimeUnit.MILLISECONDS).take(3),
    Observable.error(Exception("Failed")))
val completeAt400 = Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(4)
Observable.mergeDelayError(failAt200, failAt300, completeAt400)
    .dump()
/*
onNext: 0
onNext: 0
onNext: 0
onNext: 1
onNext: 1
onNext: 1
onNext: 2
onNext: 2
onNext: 3
onError: io.reactivex.exceptions.CompositeException: 2 exceptions occurred. : 2 exceptions occurred. 
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject1 = PublishSubject<String>()
let subject2 = PublishSubject<String>()
Observable.of(subject1, subject2)
    .merge()
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
subject1.onNext("🅰️")
subject1.onNext("🅱️")
subject2.onNext("①")
subject2.onNext("②")
subject1.onNext("🆎")
subject2.onNext("③")
/*
🅰️
🅱️
①
②
🆎
③
*/
```

### StartWith

[ReactiveX - StartWith operator](http://reactivex.io/documentation/operators/startwith.html)
[Reactive Extensions再入門 その４２「StartWithメソッドとJoinメソッド」](https://blog.okazuki.jp/entry/20120220/1329742486)

StartWith 在发送源数据流的数据之前先发送指定的数据序列。

![StartWith](http://reactivex.io/documentation/operators/images/startWith.C.png)
- RxNET

```
var source = Observable.Range(0, 3);
var result = source.StartWith(-3, -2, -1);
result.Subscribe(
Console.WriteLine,
() => Console.WriteLine("Completed"));
/*
-3
-2
-1
0
1
2
*/
```
- RxJava

```
val values = Observable.range(0, 3)
values.startWith(listOf(-1, -2))
    .dump()
/*
onNext: -1
onNext: -2
onNext: 0
onNext: 1
onNext: 2
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of("🐶", "🐱", "🐭", "🐹")
    .startWith("1️⃣")
    .startWith("2️⃣")
    .startWith("3️⃣", "🅰️", "🅱️")
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
3️⃣
🅰️
🅱️
2️⃣
1️⃣
🐶
🐱
🐭
🐹
*/
```

### Switch

[ReactiveX - Switch operator](http://reactivex.io/documentation/operators/switch.html)
[Reactive Extensions再入門 その４０「IObservableの合成はじめました」](https://blog.okazuki.jp/entry/20120218/1329581391)

Switch 合并数据流，该操作符只发送最近开始发送数据的数据流的数据。

给定开始发送数据时间不同的两个数据流 A 和 B，A 先开始发送，B 后开始发送。则 Switch 会先发送 A 中的数据，等到 B 开始发送数据后 Switch 会转而发送 B 中的数据。多个数据流也以此类推。

![Switch](http://reactivex.io/documentation/operators/images/switch.c.png)
- RxNET

```
var source = new Subject<int>();
source
    .Select(i => Observable
        .Interval(TimeSpan.FromSeconds(1)).Take(3)
        .Select(l => (l + 1) * i))
    .Switch()
    .Subscribe(
        i => Console.WriteLine("OnNext: {0}", i),
        () => Console.WriteLine("OnCompleted"));
Console.WriteLine("# OnNext(10)");
source.OnNext(10);
Console.WriteLine("Sleep 2000ms...");
Thread.Sleep(2000);
Console.WriteLine("# OnNext(100)");
source.OnNext(100);
Console.WriteLine("Sleep 2000ms...");
Thread.Sleep(2000);
Console.WriteLine("# OnNext(1000)");
source.OnNext(1000);
Console.WriteLine("Sleep 2000ms...");
Thread.Sleep(2000);
Console.WriteLine("# OnCompleted");
source.OnCompleted();
/*
# OnNext(10)
Sleep 2000ms...
OnNext: 10
OnNext: 20
# OnNext(100)
Sleep 2000ms...
OnNext: 100
OnNext: 200
# OnNext(1000)
Sleep 2000ms...
OnNext: 1000
OnNext: 2000
# OnCompleted
OnNext: 3000
OnCompleted
*/
```
- RxJava

![switchOnNext](http://reactivex.io/documentation/operators/images/switchDo.png)

```
Observable.switchOnNext(
    Observable.interval(100, TimeUnit.MILLISECONDS)
        .map { i ->
            Observable.interval(30, TimeUnit.MILLISECONDS)
                .map { i2 -> i }
        }
)
    .take(9)
    .dump()
/*
onNext: 0
onNext: 0
onNext: 0
onNext: 1
onNext: 1
onNext: 1
onNext: 2
onNext: 2
onNext: 2
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject1 = BehaviorSubject(value: "⚽️")
let subject2 = BehaviorSubject(value: "🍎")
let variable = Variable(subject1)
variable.asObservable()
    .switchLatest()
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
subject1.onNext("🏈")
subject1.onNext("🏀")
variable.value = subject2
subject1.onNext("⚾️")
subject2.onNext("🍐")
/*
⚽️
🏈
🏀
🍎
🍐
*/
```

### Zip

[ReactiveX - Zip operator](http://reactivex.io/documentation/operators/zip.html)
[Reactive Extensions再入門 その４１「どんどん合成するよ」](http://reactivex.io/documentation/operators/replay.html)

Zip 将两个或多个数据流的数据通过调用指定函数合为一体。

Zip 操作符中数据结合的方法为将所有源数据流的第 1 个数据结合，然后再将所有源数据流的第 2 个数据结合，以此类推，直到某一个源数据流没有数据为止。

![Zip](http://reactivex.io/documentation/operators/images/zip.c.png)
- RxNET

```
/*
    nums  ----0----1----2| 
    chars --a--b--c--d--e--f| 
    result----0----1----2|
                a    b    c|
*/
//Generate values 0,1,2 
var nums = Observable.Interval(TimeSpan.FromMilliseconds(250))
.Take(3);
//Generate values a,b,c,d,e,f 
var chars = Observable.Interval(TimeSpan.FromMilliseconds(150))
.Take(6)
.Select(i => Char.ConvertFromUtf32((int)i + 97));
//Zip values together
nums.Zip(chars, (lhs, rhs) => new { Left = lhs, Right = rhs })
.Dump("Zip");
/*
Zip-->{ Left = 0, Right = a }
Zip-->{ Left = 1, Right = b }
Zip-->{ Left = 2, Right = c }
Zip completed
*/
```
- RxJava

![Zip](http://reactivex.io/documentation/operators/images/zip.o.png)

```
Observables.zip(
    Observable.interval(100, TimeUnit.MILLISECONDS)
        .doOnNext { i -> println("Left emits $i") },
    Observable.interval(150, TimeUnit.MILLISECONDS)
        .doOnNext { i -> println("Right emits $i") })
    { i1, i2 -> "$i1 - $i2" }
    .take(6)
    .dump()
/*
Left emits 0
Right emits 0
onNext: 0 - 0
Left emits 1
Left emits 2
Right emits 1
onNext: 1 - 1
Left emits 3
Right emits 2
onNext: 2 - 2
Left emits 4
Right emits 3
Left emits 5
onNext: 3 - 3
Left emits 6
Right emits 4
onNext: 4 - 4
Left emits 7
Left emits 8
Right emits 5
onNext: 5 - 5
onComplete
*/
```

```
Observables.zip(
    Observable.interval(100, TimeUnit.MILLISECONDS),
    Observable.interval(150, TimeUnit.MILLISECONDS),
    Observable.interval(40, TimeUnit.MILLISECONDS))
    { i1, i2, i3 -> "$i1 - $i2 - $i3" }
    .take(6)
    .dump()
/*
onNext: 0 - 0 - 0
onNext: 1 - 1 - 1
onNext: 2 - 2 - 2
onNext: 3 - 3 - 3
onNext: 4 - 4 - 4
onNext: 5 - 5 - 5
onComplete
*/
```

```
Observables.zip(
    Observable.range(0, 5),
    Observable.range(0, 3),
    Observable.range(0, 8))
    { i1, i2, i3 -> "$i1 - $i2 - $i3" }
    .count()
    .dump()
/*
onSuccess: 3
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .zipWith(Observable.interval(150, TimeUnit.MILLISECONDS))
        { i1, i2 -> "$i1 - $i2" }
    .take(6)
    .dump()
/*
onNext: 0 - 0
onNext: 1 - 1
onNext: 2 - 2
onNext: 3 - 3
onNext: 4 - 4
onNext: 5 - 5
onComplete
*/
```

```
Observable.range(0, 5)
    .zipWith(Arrays.asList(0, 2, 4, 6, 8))
        { i1, i2 -> "$i1 - $i2" }
    .dump()
/*
onNext: 0 - 0
onNext: 1 - 2
onNext: 2 - 4
onNext: 3 - 6
onNext: 4 - 8
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let stringSubject = PublishSubject<String>()
let intSubject = PublishSubject<Int>()
Observable.zip(stringSubject, intSubject) { stringElement, intElement in
    "\(stringElement) \(intElement)"
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
stringSubject.onNext("🅰️")
stringSubject.onNext("🅱️")
intSubject.onNext(1)
intSubject.onNext(2)
stringSubject.onNext("🆎")
intSubject.onNext(3)
/*
🅰️ 1
🅱️ 2
🆎 3
*/
```


