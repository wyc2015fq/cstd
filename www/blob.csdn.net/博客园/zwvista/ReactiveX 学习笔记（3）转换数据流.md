# ReactiveX 学习笔记（3）转换数据流 - zwvista - 博客园

## [ReactiveX 学习笔记（3）转换数据流](https://www.cnblogs.com/zwvista/p/9333314.html)

### Transforming Observables

本文的主题为转换 Observable 的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（二）Transforming Observables](http://mushuichuan.com/2015/12/11/rxjava-operator-2/)

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
- RxJava

```
fun <T> Observable<T>.dump() =
    this.subscribe({ println("onNext: $it") },
        { println("onError: $it: ${it.message}") },
        { println("onComplete") })
fun <T> Observable<T>.dump(name: String) =
    this.subscribe({ println("$name: onNext: $it") },
        { println("$name: onError: $it: ${it.message}") },
        { println("$name: onComplete") })
```

### Buffer

[ReactiveX - Buffer operator](http://reactivex.io/documentation/operators/buffer.html)
[Reactive Extensions再入門 その２５「値をまとめるBufferメソッド」](https://blog.okazuki.jp/entry/20120117/1326804922)

Buffer 转换数据流：间隔性地将数据流中邻近的数据打包，形成数据包的数据流。

这里的数据包即静态的数组。

![Buffer](http://reactivex.io/documentation/operators/images/Buffer.png)
- RxNET

```
var source = Observable.Interval(TimeSpan.FromSeconds(1)).Take(10);
source.Buffer(3, 1)
.Subscribe(
buffer =>
{
    Console.WriteLine("--Buffered values");
    foreach (var value in buffer)
        Console.WriteLine(value);
}, () => Console.WriteLine("Completed"));
/*
--Buffered values
0
1
2
--Buffered values
1
2
3
..
--Buffered values
7
8
9
--Buffered values
8
9
--Buffered values
9
Completed
*/
```
- RxJava

![Buffer](http://reactivex.io/documentation/operators/images/buffer1.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer2.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer3.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer4.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer5.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer6.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer7.png)
![Buffer](http://reactivex.io/documentation/operators/images/buffer8.png)

```
Observable.range(0, 10)
    .buffer(4)
    .dump()
/*
onNext: [0, 1, 2, 3]
onNext: [4, 5, 6, 7]
onNext: [8, 9]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS).take(10)
    .buffer(250, TimeUnit.MILLISECONDS)
    .dump()
/*
onNext: [0, 1]
onNext: [2, 3, 4]
onNext: [5, 6]
onNext: [7, 8, 9]
onNext: []
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(10)
    .buffer(250, TimeUnit.MILLISECONDS, 2)
    .dump()
/*
onNext: [0, 1]
onNext: []
onNext: [2, 3]
onNext: []
onNext: [4, 5]
onNext: [6]
onNext: [7, 8]
onNext: [9]
onNext: []
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS).take(10)
    .buffer(Observable.interval(250, TimeUnit.MILLISECONDS))
    .dump()
/*
onNext: [0, 1]
onNext: [2, 3]
onNext: [4, 5, 6]
onNext: [7, 8, 9]
onNext: []
onComplete
*/
```

```
Observable.range(0, 10)
    .buffer(4, 3)
    .dump()
/*
onNext: [0, 1, 2, 3]
onNext: [3, 4, 5, 6]
onNext: [6, 7, 8, 9]
onNext: [9]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS).take(10)
    .buffer(350, 200, TimeUnit.MILLISECONDS)
    .dump()
/*
onNext: [0, 1, 2]
onNext: [2, 3, 4]
onNext: [3, 4, 5, 6]
onNext: [6, 7, 8]
onNext: [7, 8, 9]
onNext: []
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS).take(10)
    .buffer<Long, Long>(
        Observable.interval(250, TimeUnit.MILLISECONDS),
        Function { i -> Observable.timer(200, TimeUnit.MILLISECONDS) } )
    .dump()
/*
onNext: [2, 3]
onNext: [5, 6]
onNext: [7, 8]
onNext: []
onComplete
*/
```

### FlatMap / SelectMany

[ReactiveX - FlatMap operator](http://reactivex.io/documentation/operators/flatmap.html)
[Reactive Extensions再入門 その４０「IObservableの合成はじめました」](https://blog.okazuki.jp/entry/20120218/1329581391)

FlatMap / SelectMany 转换数据流：将源数据流的每一项都转换成数据流，从而形成数据流的数据流，最后再平坦化将两维数据流合并成一个数据流。

![FlatMap](http://reactivex.io/documentation/operators/images/flatMap.c.png)
- RxNET

```
Observable.Range(1, 3)
.SelectMany(i => Observable.Range(1, i))
.Dump("SelectMany");
/*
SelectMany-->1
SelectMany-->1
SelectMany-->2
SelectMany-->1
SelectMany-->2
SelectMany-->3
SelectMany completed
*/
```
- RxJava

```
val values = Observable.just(2)
values
    .flatMap { i -> Observable.range(0, i) }
    .dump()
/*
onNext: 0
onNext: 1
onComplete
*/
```

```
val values = Observable.range(1, 3)
values
    .flatMap { i -> Observable.range(0, i) }
    .dump()
/*
onNext: 0
onNext: 0
onNext: 1
onNext: 0
onNext: 1
onNext: 2
onComplete
*/
```

```
val values = Observable.just(1)
values
    .flatMap { i ->
        Observable.just(
            Character.valueOf((i + 64).toChar())
        )
    }
    .dump()
/*
onNext: A
onComplete
*/
```

```
val values = Observable.range(0, 30)
values
    .flatMap<Char> { i ->
        if (i in 1..26)
            Observable.just(Character.valueOf((i + 64).toChar()))
        else
            Observable.empty()
    }
    .dump()
/*
onNext: A
onNext: B
...
onNext: Y
onNext: Z
onComplete
*/
```

```
Observable.just(100, 150)
    .flatMap { i ->
        Observable.interval(i.toLong(), TimeUnit.MILLISECONDS)
            .map { v -> i }
    }
    .take(10)
    .dump()
/*
onNext: 100
onNext: 150
onNext: 100
onNext: 150
onNext: 100
onNext: 100
onNext: 150
onNext: 100
onNext: 100
onNext: 150
onComplete
*/
```

```
Observable.just(100, 150)
    .concatMap { i ->
        Observable.interval(i.toLong(), TimeUnit.MILLISECONDS)
            .map { v -> i }
            .take(3)
    }
    .dump()
/*
onNext: 100
onNext: 100
onNext: 100
onNext: 150
onNext: 150
onNext: 150
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .switchMap { i ->
        Observable.interval(30, TimeUnit.MILLISECONDS)
            .map { l -> i }
    }
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
- concatMap / flatMap / switchMap
[RxJava: FlatMap, SwitchMap and ConcatMap differences & examples](https://medium.com/appunite-edu-collection/rxjava-flatmap-switchmap-and-concatmap-differences-examples-6d1f3ff88ee0)

![concatMap](http://reactivex.io/documentation/operators/images/concatMap.png)
![flatMap](http://reactivex.io/documentation/operators/images/flatMap.png)
![switchMap](http://reactivex.io/documentation/operators/images/switchMap.png)

```
val items = listOf("a", "b", "c", "d", "e", "f")
val scheduler = TestScheduler()
items.toObservable()
    .concatMap { s ->
        val delay = Random().nextInt(10)
        Observable.just(s + "x")
            .delay(delay.toLong(), TimeUnit.SECONDS, scheduler)
    }.toList()
    .doAfterSuccess { println(it) }
    .subscribe()
scheduler.advanceTimeBy(1, TimeUnit.MINUTES)
/*
[ax, bx, cx, dx, ex, fx]
*/
```

```
val items = listOf("a", "b", "c", "d", "e", "f")
val scheduler = TestScheduler()
items.toObservable()
    .flatMap { s ->
        val delay = Random().nextInt(10)
        Observable.just(s + "x")
            .delay(delay.toLong(), TimeUnit.SECONDS, scheduler)
    }.toList()
    .doAfterSuccess { println(it) }
    .subscribe()
scheduler.advanceTimeBy(1, TimeUnit.MINUTES)
/*
[dx, ex, bx, cx, fx, ax]
*/
```

```
val items = listOf("a", "b", "c", "d", "e", "f")
val scheduler = TestScheduler()
items.toObservable()
    .switchMap { s ->
        val delay = Random().nextInt(10)
        Observable.just(s + "x")
            .delay(delay.toLong(), TimeUnit.SECONDS, scheduler)
    }.toList()
    .doAfterSuccess { println(it) }
    .subscribe()
scheduler.advanceTimeBy(1, TimeUnit.MINUTES)
/*
[fx]
*/
```
- flatMapIterable

![flatMapIterable](http://reactivex.io/documentation/operators/images/mergeMapIterable.png)

```
Observable.range(1, 3)
    .flatMapIterable { i -> 1..i }
    .dump()
/*
onNext: 1
onNext: 1
onNext: 2
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```

```
Observable.range(1, 3)
    .flatMapIterable<Int, Int>(
        { i -> 1..i },
        { ori, rv -> ori * rv })
    .dump()
/*
onNext: 1
onNext: 2
onNext: 4
onNext: 3
onNext: 6
onNext: 9
onComplete
*/
```

```
Observable.range(1, 3)
    .flatMapIterable<Int, Int>(
        { i -> generateSequence(1) { (it + 1).takeIf { it <= i } }.asIterable() },
        { ori, rv -> ori * rv })
    .dump()
/*
onNext: 1
onNext: 2
onNext: 4
onNext: 3
onNext: 6
onNext: 9
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
struct Player {
    var score: Variable<Int>
}
let 👦🏻 = Player(score: Variable(80))
let 👧🏼 = Player(score: Variable(90))
let player = Variable(👦🏻)
player.asObservable()
    .flatMap { $0.score.asObservable() }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
👦🏻.score.value = 85
player.value = 👧🏼
👦🏻.score.value = 95
👧🏼.score.value = 100
/*
80
85
90
95
100
*/
```

```
let disposeBag = DisposeBag()
struct Player {
    var score: Variable<Int>
}
let 👦🏻 = Player(score: Variable(80))
let 👧🏼 = Player(score: Variable(90))
let player = Variable(👦🏻)
player.asObservable()
    .flatMapLatest { $0.score.asObservable() }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
👦🏻.score.value = 85
player.value = 👧🏼
👦🏻.score.value = 95
👧🏼.score.value = 100
/*
80
85
90
100
*/
```

### GroupBy / GroupByUntil

[ReactiveX - GroupBy operator](http://reactivex.io/documentation/operators/groupby.html)
[Reactive Extensions再入門 その２０「GroupByメソッドでグルーピングしてみよう」](https://blog.okazuki.jp/entry/20111228/1325043357)
[Reactive Extensions再入門 その２１「GroupByUntilメソッド」](https://blog.okazuki.jp/entry/20111229/1325172773)

GroupBy 转换数据流：将数据流分组，结果是被分组的数据流的数据流。

所谓被分组的数据流是指带分组键（Key）的数据流。

![GroupBy](http://reactivex.io/documentation/operators/images/groupBy.c.png)
![GroupByUntil](http://reactivex.io/documentation/operators/images/groupByUntil.png)
- RxNET

```
var source = Observable.Interval(TimeSpan.FromSeconds(0.1)).Take(10);
var group = source.GroupBy(i => i % 3);
group.Subscribe(
grp =>
grp.Min().Subscribe(
minValue =>
Console.WriteLine("{0} min value = {1}", grp.Key, minValue)),
() => Console.WriteLine("Completed"));
Console.ReadLine();
/*
0 min value = 0
1 min value = 1
2 min value = 2
Completed
*/
```

```
var source = Observable.Interval(TimeSpan.FromSeconds(0.1)).Take(10);
var group = source.GroupBy(i => i % 3);
group.SelectMany(
grp =>
grp.Max()
.Select(value => new { grp.Key, value }))
.Dump("group");
Console.ReadLine();
/*
group-->{ Key = 0, value = 9 }
group-->{ Key = 1, value = 7 }
group-->{ Key = 2, value = 8 }
group completed
*/
```
- RxJava

```
val values = Observable.just(
    "first",
    "second",
    "third",
    "forth",
    "fifth",
    "sixth"
)
values.groupBy { word -> word[0] }
    .flatMap<Any> { group -> group.lastOrError().toObservable().map { v -> "${group.key}: $v" } }
    .dump()
/*
onNext: s: sixth
onNext: t: third
onNext: f: fifth
onComplete
*/
```

### Map / Select / Cast / OfType

[ReactiveX - Map operator](http://reactivex.io/documentation/operators/map.html)
[Reactive Extensions再入門 その７「LINQスタイルの拡張メソッド」](https://blog.okazuki.jp/entry/20111110/1320849106)

Map / Select 转换数据流：针对源数据流的每一项都调用指定函数后将源数据流映射为目标数据流。

Cast 转换数据流：将源数据流的每一项都转变为指定类型，不能转型时出错。

OfType 过滤数据流：只发送源数据流中指定类型的数据。

![Map](http://reactivex.io/documentation/operators/images/map.c.png)
![Cast](http://reactivex.io/documentation/operators/images/cast.png)
![OfType](http://reactivex.io/documentation/operators/images/ofClass.png)
- RxNET

```
var source = Observable.Range(0, 3);
source.Select(i => i + 3).Dump("+3");
source.Select(i => (char)(i + 64)).Dump("char");
source.Select(i => new { Number = i, Character = (char)(i + 64) }).Dump("anon");
var query = from i in source
            select new { Number = i, Character = (char)(i + 64) };
query.Dump("anon");
/*
+3-->3
+3-->4
+3-->5
+3 completed
char-->@
char-->A
char-->B
char completed
anon-->{ Number = 0, Character = @ }
anon-->{ Number = 1, Character = A }
anon-->{ Number = 2, Character = B }
anon completed
anon-->{ Number = 0, Character = @ }
anon-->{ Number = 1, Character = A }
anon-->{ Number = 2, Character = B }
anon completed
*/
```

```
var objects = new Subject<object>();
objects.Cast<int>().Dump("cast");
objects.OnNext(1);
objects.OnNext(2);
objects.OnNext(3);
objects.OnCompleted();
/*
cast-->1
cast-->2
cast-->3
cast completed
*/
var objects = new Subject<object>();
objects.Cast<int>().Dump("cast");
objects.OnNext(1);
objects.OnNext(2);
objects.OnNext("3");//Fail
/*
cast-->1
cast-->2
cast failed-->Specified cast is not valid.
*/
```

```
var objects = new Subject<object>();
objects.OfType<int>().Dump("OfType");
objects.OnNext(1);
objects.OnNext(2);
objects.OnNext("3");//Ignored
objects.OnNext(4);
objects.OnCompleted();
/*
OfType-->1
OfType-->2
OfType-->4
OfType completed
*/
```
- RxJava

```
val values = Observable.range(0, 4)
values
    .map { i -> i + 3 }
    .dump()
/*
onNext: 3
onNext: 4
onNext: 5
onNext: 6
onComplete
*/
```

```
val values = Observable.just("0", "1", "2", "3")
    .map { Integer.parseInt(it) }
values.dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```

```
val values = Observable.just<Int>(0, 1, 2, 3)
values
    .cast(java.lang.Integer::class.java)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```

```
val values = Observable.just(0, 1, 2, "3")
values
    .cast(java.lang.Integer::class.java)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onError: java.lang.ClassCastException: Cannot cast java.lang.String to java.lang.Integer: Cannot cast java.lang.String to java.lang.Integer
*/
```

```
val values = Observable.just(0, 1, "2", 3)
values
    .ofType(java.lang.Integer::class.java)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 3
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(1, 2, 3)
    .map { $0 * $0 }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
1
4
9
*/
```

### Scan

[ReactiveX - Scan operator](http://reactivex.io/documentation/operators/scan.html)
[Reactive Extensions再入門 その１７「集計するメソッド」](https://blog.okazuki.jp/entry/20111212/1323698319)

Scan 转换数据流：
- 该操作带有一个有返回值的二元函数和一个可选的初值。
- 结果数据流的第 1 项没有初值时等于源数据流的第 1 项，有初值时等于将二元函数应用到初值和源数据流的第 1 项后所得到的结果。
- 结果数据流的第 n + 1 项等于将二元函数应用到结果数据流的第 n 项和源数据流的第 n + 1 项后所得到的结果。

![Scan](http://reactivex.io/documentation/operators/images/scan.c.png)
- RxNET

```
Console.WriteLine(MethodBase.GetCurrentMethod().Name);
var numbers = new Subject<int>();
var scan = numbers.Scan(0, (acc, current) => acc + current);
numbers.Dump("numbers");
scan.Dump("scan");
numbers.OnNext(1);
numbers.OnNext(2);
numbers.OnNext(3);
numbers.OnCompleted();
/*
Scan
numbers-->1
scan-->1
numbers-->2
scan-->3
numbers-->3
scan-->6
numbers completed
scan completed
*/
```
- RxJava

```
val values = Observable.range(0, 5)
values
    .scan { i1, i2 -> i1 + i2 }
    .dump()
/*
onNext: 0
onNext: 1
onNext: 3
onNext: 6
onNext: 10
onComplete
*/
```

```
val values = ReplaySubject.create<Int>()
values
    .dump("Values")
values
    .scan { i1, i2 -> if (i1 < i2) i1 else i2 }
    .distinctUntilChanged()
    .dump("Min")
values.onNext(2)
values.onNext(3)
values.onNext(1)
values.onNext(4)
values.onComplete()
/*
Values: onNext: 2
Min: onNext: 2
Values: onNext: 3
Values: onNext: 1
Min: onNext: 1
Values: onNext: 4
Values: onComplete
Min: onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(10, 100, 1000)
    .scan(1) { aggregateValue, newValue in
        aggregateValue + newValue
    }
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
11
111
1111
*/
```

### Window

[ReactiveX - Window operator](http://reactivex.io/documentation/operators/window.html)
[Reactive Extensions再入門 その２６「値をまとめるWindowメソッド」](https://blog.okazuki.jp/entry/20120121/1327119785)

Window 转换数据流：间隔性地将数据流分段，形成数据流的数据流，即两维数据流。

![Window](http://reactivex.io/documentation/operators/images/window.C.png)
- RxNET

```
var windowIdx = 0;
var source = Observable.Interval(TimeSpan.FromSeconds(1)).Take(10);
source.Window(3)
.Subscribe(window =>
{
    var id = windowIdx++;
    Console.WriteLine("--Starting new window");
    var windowName = "Window" + id;
    window.Subscribe(
    value => Console.WriteLine("{0} : {1}", windowName, value),
    ex => Console.WriteLine("{0} : {1}", windowName, ex),
    () => Console.WriteLine("{0} Completed", windowName));
},
() => Console.WriteLine("Completed"));
/*
--Starting new window
Window0 : 0
Window0 : 1
Window0 : 2
Window0 Completed
--Starting new window
Window1 : 3
Window1 : 4
Window1 : 5
Window1 Completed
--Starting new window
Window2 : 6
Window2 : 7
Window2 : 8
Window2 Completed
--Starting new window
Window3 : 9
Window3 Completed
Completed
*/
```
- RxJava

![Window](http://reactivex.io/documentation/operators/images/window1.png)
![Window](http://reactivex.io/documentation/operators/images/window2.png)
![Window](http://reactivex.io/documentation/operators/images/window3.png)
![Window](http://reactivex.io/documentation/operators/images/window4.png)
![Window](http://reactivex.io/documentation/operators/images/window5.png)
![Window](http://reactivex.io/documentation/operators/images/window6.png)
![Window](http://reactivex.io/documentation/operators/images/window7.png)
![Window](http://reactivex.io/documentation/operators/images/window8.png)

```
Observable
    .merge(
        Observable.range(0, 5)
            .window(3, 1))
    .dump()
/*
onNext: 0
onNext: 1
onNext: 1
onNext: 2
onNext: 2
onNext: 2
onNext: 3
onNext: 3
onNext: 3
onNext: 4
onNext: 4
onNext: 4
onComplete
*/
```

```
Observable.range(0, 5)
    .window(3, 1)
    .flatMap { o -> o.toList().toObservable() }
    .dump()
/*
onNext: [0, 1, 2]
onNext: [1, 2, 3]
onNext: [2, 3, 4]
onNext: [3, 4]
onNext: [4]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(5)
    .window(250, 100, TimeUnit.MILLISECONDS)
    .flatMap { o -> o.toList().toObservable() }
    .dump()
/*
onNext: [0, 1]
onNext: [1, 2]
onNext: [2, 3]
onNext: [3, 4]
onNext: [4]
onComplete
*/
```

```
Observable.interval(100, TimeUnit.MILLISECONDS)
    .take(5)
    .window<Long, Long>(
        Observable.interval(100, TimeUnit.MILLISECONDS),
        Function { _ -> Observable.timer(250, TimeUnit.MILLISECONDS) })
    .flatMap { o -> o.toList().toObservable() }
    .dump()
/*
onNext: [1, 2]
onNext: [2, 3]
onNext: [3, 4]
onNext: [4]
onNext: []
onComplete
*/
```


