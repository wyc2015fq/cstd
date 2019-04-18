# ReactiveX 学习笔记（7）聚合操作符 - zwvista - 博客园

## [ReactiveX 学习笔记（7）聚合操作符](https://www.cnblogs.com/zwvista/p/9360306.html)

### Mathematical and Aggregate Operators

本文的主题为处理 Observable 的聚合操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（八）Aggregate](http://mushuichuan.com/2015/12/14/rxjava-operator-8/)

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

### Average

[ReactiveX - Average operator](http://reactivex.io/documentation/operators/average.html)
[Reactive Extensions再入門 その１６「最大、最少、平均を求めるメソッド」](https://blog.okazuki.jp/entry/20111210/1323540094)

Average 计算源数据流所有数据的平均值，然后发送这个平均值。

![Average](http://reactivex.io/documentation/operators/images/average.c.png)
- RxNET

```
var s = new Subject<int>();
s.Average().Subscribe(avg =>
{
    Console.WriteLine("Average {0}", avg);
},
() => Console.WriteLine("Average Completed"));
Console.WriteLine("OnNext(1-3)");
s.OnNext(1);
s.OnNext(2);
s.OnNext(3);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1-3)
OnCompleted()
Average 2
Average Completed
*/
```

### Concat

[ReactiveX - Concat operator](http://reactivex.io/documentation/operators/concat.html)
[Reactive Extensions再入門 その４１「どんどん合成するよ」](http://reactivex.io/documentation/operators/replay.html)

Concat 合并数据流，按照数据流的连接顺序依次发送各个数据流的数据。

给定两个数据流 A 和 B，Concat(A, B) 会先发送 A 中的数据，等到 A 中的数据全部发送完毕后再发送 B 中的数据。多个数据流也以此类推。

![Concat](http://reactivex.io/documentation/operators/images/concat.C.png)
- RxNET

```
var s1 = Observable.Range(0, 3);
var s2 = Observable.Range(5, 5);
s1.Concat(s2)
.Subscribe(Console.WriteLine);
/*
0
1
2
5
6
7
8
9
*/
```

```
private static IEnumerable<IObservable<long>> GetSequences()
{
    Console.WriteLine("GetSequences() called");
    Console.WriteLine("Yield 1st sequence");
    yield return Observable.Create<long>(o =>
    {
        Console.WriteLine("1st subscribed to");
        return Observable.Timer(TimeSpan.FromMilliseconds(500))
        .Select(i => 1L)
        .Subscribe(o);
    });
    Console.WriteLine("Yield 2nd sequence");
    yield return Observable.Create<long>(o =>
    {
        Console.WriteLine("2nd subscribed to");
        return Observable.Timer(TimeSpan.FromMilliseconds(300))
        .Select(i => 2L)
        .Subscribe(o);
    });
    Thread.Sleep(1000);     //Force a delay
    Console.WriteLine("Yield 3rd sequence");
    yield return Observable.Create<long>(o =>
    {
        Console.WriteLine("3rd subscribed to");
        return Observable.Timer(TimeSpan.FromMilliseconds(100))
        .Select(i => 3L)
        .Subscribe(o);
    });
    Console.WriteLine("GetSequences() complete");
}
GetSequences().Concat().Dump("Concat");
/*
GetSequences() called
Yield 1st sequence
1st subscribed to
Concat-->1
Yield 2nd sequence
2nd subscribed to
Concat-->2
Yield 3rd sequence
3rd subscribed to
Concat-->3
GetSequences() complete
Concat completed
*/
```
- RxJava

```
val seq1 = Observable.range(0, 3)
val seq2 = Observable.range(10, 3)
Observable.concat(seq1, seq2)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 10
onNext: 11
onNext: 12
onComplete
*/
```

```
val words = Observable.just(
    "First",
    "Second",
    "Third",
    "Fourth",
    "Fifth",
    "Sixth"
)
Observable.concat(words.groupBy { v -> v[0] })
    .dump()
/*
onNext: First
onNext: Fourth
onNext: Fifth
onNext: Second
onNext: Sixth
onNext: Third
onComplete
*/
```

```
val seq1 = Observable.range(0, 3)
val seq2 = Observable.range(10, 3)
val seq3 = Observable.just(20)
seq1.concatWith(seq2)
    .concatWith(seq3)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 2
onNext: 10
onNext: 11
onNext: 12
onNext: 20
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject1 = BehaviorSubject(value: "🍎")
let subject2 = BehaviorSubject(value: "🐶")
let variable = Variable(subject1)
variable.asObservable()
    .concat()
    .subscribe { print($0) }
    .disposed(by: disposeBag)
subject1.onNext("🍐")
subject1.onNext("🍊")
variable.value = subject2
subject2.onNext("I would be ignored")
subject2.onNext("🐱")
subject1.onCompleted()
subject2.onNext("🐭")
/*
next(🍎)
next(🍐)
next(🍊)
next(🐱)
next(🐭)
*/
```

### Count

[ReactiveX - Count operator](http://reactivex.io/documentation/operators/count.html)
[Reactive Extensions再入門 その１８「CountメソッドとLongCountメソッド」](https://blog.okazuki.jp/entry/20111212/1323699405)

Count 计算源数据流的个数，然后发送这个值。

![Count](http://reactivex.io/documentation/operators/images/Count.png)
- RxNET

```
var s = new Subject<int>();
s.Count()
    .Subscribe(
        i => Console.WriteLine("Count OnNext({0})", i),
        () => Console.WriteLine("Count OnCompleted()"));
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(10)");
s.OnNext(10);
Console.WriteLine("OnNext(100)");
s.OnNext(100);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
OnNext(10)
OnNext(100)
OnCompleted()
Count OnNext(3)
Count OnCompleted()
*/
```
- RxJava

```
val values = Observable.range(0, 3)
values
    .dump("Values")
values
    .count()
    .dump("Count")
/*
Values: onNext: 0
Values: onNext: 1
Values: onNext: 2
Values: onComplete
Count: onSuccess: 3
*/
```

### Max

[ReactiveX - Max operator](http://reactivex.io/documentation/operators/max.html)
[Reactive Extensions再入門 その１６「最大、最少、平均を求めるメソッド」](https://blog.okazuki.jp/entry/20111210/1323540094)

Max 计算源数据流所有数据的最大值，然后发送这个最大值。

![Max](http://reactivex.io/documentation/operators/images/max.c.png)
- RxNET

```
var s = new Subject<int>();
s.Max().Subscribe(max =>
{
    Console.WriteLine("Max {0}", max);
},
() => Console.WriteLine("Max Completed"));
Console.WriteLine("OnNext(1-3)");
s.OnNext(1);
s.OnNext(2);
s.OnNext(3);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1-3)
OnCompleted()
Max 3
Max Completed
*/
```

```
var s = new Subject<Tuple<int, int>>();
s.MaxBy(t => t.Item1).Subscribe(max =>
{
    foreach (var i in max)
    {
        Console.WriteLine("MaxBy {0}", i);
    }
},
() => Console.WriteLine("MaxBy Completed"));
Console.WriteLine("OnNext");
s.OnNext(Tuple.Create(1, 1));
s.OnNext(Tuple.Create(1, 2));
s.OnNext(Tuple.Create(3, 1));
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext
OnCompleted()
MaxBy (3, 1)
MaxBy Completed
*/
```

### Min

[ReactiveX - Min operator](http://reactivex.io/documentation/operators/min.html)
[Reactive Extensions再入門 その１６「最大、最少、平均を求めるメソッド」](https://blog.okazuki.jp/entry/20111210/1323540094)

Min 计算源数据流所有数据的最小值，然后发送这个最小值。

![Min](http://reactivex.io/documentation/operators/images/min.c.png)
- RxNET

```
var s = new Subject<int>();
s.Min().Subscribe(min =>
{
    Console.WriteLine("Min {0}", min);
},
() => Console.WriteLine("Min Completed"));
Console.WriteLine("OnNext(1-3)");
s.OnNext(1);
s.OnNext(2);
s.OnNext(3);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1-3)
OnCompleted()
Min 1
Min Completed
*/
```

```
var s = new Subject<Tuple<int, int>>();
s.MinBy(t => t.Item1).Subscribe(min =>
{
    foreach (var i in min)
    {
        Console.WriteLine("MinBy {0}", i);
    }
},
() => Console.WriteLine("MinBy Completed"));
Console.WriteLine("OnNext");
s.OnNext(Tuple.Create(1, 1));
s.OnNext(Tuple.Create(1, 2));
s.OnNext(Tuple.Create(3, 1));
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext
OnCompleted()
MinBy (1, 1)
MinBy (1, 2)
MinBy Completed
*/
```

### Reduce / Aggregate / Collect

[ReactiveX - Reduce operator](http://reactivex.io/documentation/operators/reduce.html)
[Reactive Extensions再入門 その１７「集計するメソッド」](https://blog.okazuki.jp/entry/20111212/1323698319)

Reduce 带有一个有返回值的二元函数和可选的初值。

有初值时 Reduce 将二元函数应用到初值和源数据流第 1 项数据上得到中间值，然后再将二元函数应用到中间值和源数据流第 2 项数据上得到新的中间值，以此类推，直到得出最终结果后发送这个最终结果。

没有初值时 Reduce 将二元函数应用到源数据流第 1 项和第 2 项数据上得到中间值，然后再将二元函数应用到中间值和源数据流第 3 项数据上得到新的中间值，以此类推，直到得出最终结果后发送这个最终结果。

Collect 的处理流程与 Reduce 近似，只是 Collect 的初值不是一个固定的值，而是由回调函数生成的。

![Reduce](http://reactivex.io/documentation/operators/images/reduce.c.png)
- RxNET

```
var s = new Subject<int>();
s.Aggregate((x, y) =>
{
    Console.WriteLine("log({0}, {1})", x, y);
    return x + y;
})
.Subscribe(
    i => Console.WriteLine("Aggregate OnNext({0})", i),
    () => Console.WriteLine("Aggregate OnCompleted()"));
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(10)");
s.OnNext(10);
Console.WriteLine("OnNext(100)");
s.OnNext(100);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
OnNext(10)
log(1, 10)
OnNext(100)
log(11, 100)
OnCompleted()
Aggregate OnNext(111)
Aggregate OnCompleted()
*/
```

```
var s = new Subject<int>();
s.Aggregate(5, (x, y) =>
{
    Console.WriteLine("log({0}, {1})", x, y);
    return x + y;
})
.Subscribe(
    i => Console.WriteLine("Aggregate OnNext({0})", i),
    () => Console.WriteLine("Aggregate OnCompleted()"));
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(10)");
s.OnNext(10);
Console.WriteLine("OnNext(100)");
s.OnNext(100);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
log(5, 1)
OnNext(10)
log(6, 10)
OnNext(100)
log(16, 100)
OnCompleted()
Aggregate OnNext(116)
Aggregate OnCompleted()
*/
```
- RxJava

![Collect](http://reactivex.io/documentation/operators/images/collect.png)

```
val values = Observable.range(0, 5)
values
    .reduce { i1, i2 -> i1 + i2 }
    .dump("Sum")
values
    .reduce { i1, i2 -> if (i1 > i2) i2 else i1 }
    .dump("Min")
/*
Sum: onSuccess: 10
Min: onSuccess: 0
*/
```

```
val values = Observable.just("Rx", "is", "easy")
values
    .reduce(0) { acc, next -> acc + 1 }
    .dump("Count")
/*
Count: onSuccess: 3
*/
```

```
val values = Observable.range(10, 5)
values
    .collect(
        { ArrayList<Int>() },
        { acc, value -> acc.add(value) })
    .dump()
/*
onSuccess: [10, 11, 12, 13, 14]
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.of(10, 100, 1000)
    .reduce(1, accumulator: +)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
1111
*/
```

### Sum

[ReactiveX - Sum operator](http://reactivex.io/documentation/operators/sum.html)

Sum 计算源数据流发送的所有数值的和，然后发送这个值。

![Sum](http://reactivex.io/documentation/operators/images/sum.c.png)
- RxNET

```
var s = new Subject<int>();
s.Sum()
    .Subscribe(
        i => Console.WriteLine("Sum OnNext({0})", i),
        () => Console.WriteLine("Sum OnCompleted()"));
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(10)");
s.OnNext(10);
Console.WriteLine("OnNext(100)");
s.OnNext(100);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
OnNext(10)
OnNext(100)
OnCompleted()
Sum OnNext(111)
Sum OnCompleted()
*/
```


