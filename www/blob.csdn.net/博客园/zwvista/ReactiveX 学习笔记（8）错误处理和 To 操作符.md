# ReactiveX 学习笔记（8）错误处理和 To 操作符 - zwvista - 博客园

## [ReactiveX 学习笔记（8）错误处理和 To 操作符](https://www.cnblogs.com/zwvista/p/9369186.html)

### Error Handling Operators

### Operators to Convert Observables

本文的主题为对 Observable 进行错误处理的操作符以及转换 Observable 类型的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（五）Error Handling](http://mushuichuan.com/2015/12/11/rxjava-operator-5/)

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
        { e -> println("onError: ${e.javaClass.name}: ${e.message}") },
        { println("onComplete") })
```

### Catch / OnErrorResumeNext

[ReactiveX - Catch operator](http://reactivex.io/documentation/operators/catch.html)
[Reactive Extensions再入門 その１１「Catchメソッド」](https://blog.okazuki.jp/entry/20111129/1322491648)
[Reactive Extensions再入門 その３４「ダメなら次の人！を実現するOnErrorResumeNextメソッド」](https://blog.okazuki.jp/entry/20120211/1328973285)

当源数据流出错抛出异常时，Catch 能捕捉该异常并从中恢复。相当于 catch 语句。

不指定异常类型时，Catch 能捕捉所有类型的异常。

指定异常类型时，Catch 只能捕捉指定类型的异常。

OnErrorResumeNext 与 VB 的同名语句的作用相同。使用该操作符的话，一个数据流无论出错还是正常结束都会转到后一个指定数据流。

![Catch](http://reactivex.io/documentation/operators/images/Catch.png)
![OnErrorResumeNext](http://reactivex.io/documentation/operators/images/onErrorResumeNext.png)
- RxNET

```
var source = new Subject<int>();
var result = source.Catch(Observable.Empty<int>());
result.Dump("Catch");
source.OnNext(1);
source.OnNext(2);
source.OnError(new Exception("Fail!"));
/*
Catch-->1
Catch-->2
Catch completed
*/
```

```
var source = new Subject<int>();
var result = source.Catch<int, TimeoutException>(tx => Observable.Return(-1));
result.Dump("Catch");
source.OnNext(1);
source.OnNext(2);
source.OnError(new TimeoutException());
/*
Catch-->1
Catch-->2
Catch-->-1
Catch completed
*/
```

```
var source = new Subject<int>();
var result = source.Catch<int, TimeoutException>(tx => Observable.Return(-1));
result.Dump("Catch");
source.OnNext(1);
source.OnNext(2);
source.OnError(new ArgumentException("Fail!"));
/*
Catch-->1
Catch-->2
Catch failed-->Fail!
*/
```

```
Observable
    .Throw<string>(new Exception())
    .OnErrorResumeNext(Observable.Return("OK"))
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        ex => Console.WriteLine("OnError: {0}", ex),
        () => Console.WriteLine("OnCompleted"));
new[] { "NG", "Error", "Abort", "OK" }
    .Select((s, i) => new { index = i, value = s })
    .Select(s => s.value != "OK" ?
        Observable.Throw<string>(new Exception(s.ToString())) :
        Observable.Return(s.ToString()))
    .OnErrorResumeNext()
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        ex => Console.WriteLine("OnError: {0}", ex),
        () => Console.WriteLine("OnCompleted"));
new[] { "NG", "OK", "Abort", "Error" }
    .Select((s, i) => new { index = i, value = s })
    .Select(s => s.value != "OK" ?
        Observable.Throw<string>(new Exception(s.ToString())) :
        Observable.Return(s.ToString()))
    .OnErrorResumeNext()
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        ex => Console.WriteLine("OnError: {0}", ex),
        () => Console.WriteLine("OnCompleted"));
new[] { "NG", "Exception", "Abort", "Error" }
    .Select((s, i) => new { index = i, value = s })
    .Select(s => s.value != "OK" ?
        Observable.Throw<string>(new Exception(s.ToString())) :
        Observable.Return(s.ToString()))
    .OnErrorResumeNext()
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        ex => Console.WriteLine("OnError: {0}", ex),
        () => Console.WriteLine("OnCompleted"));
/*
OnNext: OK
OnCompleted
OnNext: { index = 3, value = OK }
OnCompleted
OnNext: { index = 1, value = OK }
OnCompleted
OnCompleted
*/
```
- RxJava

![onErrorReturn](http://reactivex.io/documentation/operators/images/onErrorReturn.png)
![OnErrorResumeNext](http://reactivex.io/documentation/operators/images/onErrorResumeNext.png)
![onExceptionResumeNext](http://reactivex.io/documentation/operators/images/onExceptionResumeNextViaObservable.png)

```
val values = Observable.create<String> { o ->
    o.onNext("Rx")
    o.onNext("is")
    o.onError(Exception("adjective unknown"))
}
values
    .onErrorReturn { e -> "Error: " + e.message }
    .dump()
/*
onNext: Rx
onNext: is
onNext: Error: adjective unknown
onComplete
*/
```

```
val values = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onError(Exception("Oops"))
}
values
    .onErrorResumeNext(Observable.just(Integer.MAX_VALUE))
    .dump()
/*
onNext: 1
onNext: 2
onNext: 2147483647
onComplete
*/
```

```
val values = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onError(Exception("Oops"))
}
values
    .onErrorResumeNext( Function { e -> Observable.error(UnsupportedOperationException(e)) } )
    .dump()
/*
onNext: 1
onNext: 2
onError: java.lang.UnsupportedOperationException: java.lang.Exception: Oops: java.lang.Exception: Oops
*/
```

```
val values = Observable.create<String> { o ->
    o.onNext("Rx")
    o.onNext("is")
// o.onError(Throwable()) // this won't be caught
    o.onError(Exception()) // this will be caught
}
values
    .onExceptionResumeNext(Observable.just("hard"))
    .dump()
/*
onNext: Rx
onNext: is
onNext: hard
onComplete
*/
```

```
val values = Observable.create<String> { o ->
    o.onNext("Rx")
    o.onNext("is")
    o.onError(object : Throwable() {
    }) // this won't be caught
}
values
    .onExceptionResumeNext(Observable.just("hard"))
    .dump()
/*
onNext: Rx
onNext: is
onNext: hard
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let sequenceThatFails = PublishSubject<String>()
sequenceThatFails
    .catchErrorJustReturn("😊")
    .subscribe { print($0) }
    .disposed(by: disposeBag)
sequenceThatFails.onNext("😬")
sequenceThatFails.onNext("😨")
sequenceThatFails.onNext("😡")
sequenceThatFails.onNext("🔴")
sequenceThatFails.onError(TestError.test)
/*
next(😬)
next(😨)
next(😡)
next(🔴)
next(😊)
completed
*/
```

```
let disposeBag = DisposeBag()
let sequenceThatFails = PublishSubject<String>()
let recoverySequence = PublishSubject<String>()
sequenceThatFails
    .catchError {
        print("Error:", $0)
        return recoverySequence
    }
    .subscribe { print($0) }
    .disposed(by: disposeBag)
sequenceThatFails.onNext("😬")
sequenceThatFails.onNext("😨")
sequenceThatFails.onNext("😡")
sequenceThatFails.onNext("🔴")
sequenceThatFails.onError(TestError.test)
recoverySequence.onNext("😊")
/*
next(😬)
next(😨)
next(😡)
next(🔴)
Error: test
next(😊)
*/
```

### Retry

[ReactiveX - Retry operator](http://reactivex.io/documentation/operators/retry.html)
[Reactive Extensions再入門 その３５「駄目ならやり直す！を実現するRetryメソッド」](https://blog.okazuki.jp/entry/20120211/1328977227)

当源数据流出错抛出异常时，Retry 会重新尝试。

可以指定重新尝试的次数。不指定次数时，Retry 会进行无限次重新尝试。

![Retry](http://reactivex.io/documentation/operators/images/retry.png)
- RxNET

```
var retryCount = 0;
Observable
    .Create<string>(o =>
    {
        Console.WriteLine("Create method called: {0}", retryCount);
        if (retryCount == 3)
        {
            o.OnNext(retryCount.ToString());
            o.OnCompleted();
            return Disposable.Empty;
        }
        retryCount++;
        o.OnError(new InvalidOperationException(retryCount.ToString()));
        return Disposable.Empty;
    })
    .Retry()
    .Subscribe(
        s => Console.WriteLine("OnNext: {0}", s),
        ex => Console.WriteLine("OnError: {0}", ex),
        () => Console.WriteLine("OnCompleted"));
/*
Create method called: 0
Create method called: 1
Create method called: 2
Create method called: 3
OnNext: 3
OnCompleted
*/
```

```
var retryCount = 0;
Observable
.Create<string>(o =>
{
    Console.WriteLine("Create method called: {0}", retryCount);
    if (retryCount == 3)
    {
        o.OnNext(retryCount.ToString());
        o.OnCompleted();
        return Disposable.Empty;
    }
    retryCount++;
    o.OnError(new InvalidOperationException(retryCount.ToString()));
    return Disposable.Empty;
})
.Retry(2)
.Subscribe(
    s => Console.WriteLine("OnNext: {0}", s),
    ex => Console.WriteLine("OnError: {0}", ex),
    () => Console.WriteLine("OnCompleted"));
/*
Create method called: 0
Create method called: 1
OnError: System.InvalidOperationException: 2
*/
```
- RxJava

![Retry](http://reactivex.io/documentation/operators/images/retry.png)
![retryWhen](http://reactivex.io/documentation/operators/images/retryWhen.f.png)

```
val random = Random()
val values = Observable.create<Int> { o ->
    o.onNext(random.nextInt() % 20)
    o.onNext(random.nextInt() % 20)
    o.onError(Exception())
}
values
    .retry(1)
    .dump()
/*
onNext: 8
onNext: -9
onNext: -6
onNext: -10
onError: java.lang.Exception: null
*/
```

```
val source = Observable.create<Int> { o ->
    o.onNext(1)
    o.onNext(2)
    o.onError(Exception("Failed"))
}
source.retryWhen { o ->
    o.take(2).delay(100, TimeUnit.MILLISECONDS)
}
    .timeInterval()
    .dump()
/*
onNext: Timed[time=2, unit=MILLISECONDS, value=1]
onNext: Timed[time=0, unit=MILLISECONDS, value=2]
onNext: Timed[time=105, unit=MILLISECONDS, value=1]
onNext: Timed[time=0, unit=MILLISECONDS, value=2]
onNext: Timed[time=102, unit=MILLISECONDS, value=1]
onNext: Timed[time=1, unit=MILLISECONDS, value=2]
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
var count = 1
let sequenceThatErrors = Observable<String>.create { observer in
    observer.onNext("🍎")
    observer.onNext("🍐")
    observer.onNext("🍊")
    if count == 1 {
        observer.onError(TestError.test)
        print("Error encountered")
        count += 1
    }
    observer.onNext("🐶")
    observer.onNext("🐱")
    observer.onNext("🐭")
    observer.onCompleted()
    return Disposables.create()
}
sequenceThatErrors
    .retry()
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🍎
🍐
🍊
Error encountered
🍎
🍐
🍊
🐶
🐱
🐭
*/
```

```
let disposeBag = DisposeBag()
var count = 1
let sequenceThatErrors = Observable<String>.create { observer in
    observer.onNext("🍎")
    observer.onNext("🍐")
    observer.onNext("🍊")
    if count < 5 {
        observer.onError(TestError.test)
        print("Error encountered")
        count += 1
    }
    observer.onNext("🐶")
    observer.onNext("🐱")
    observer.onNext("🐭")
    observer.onCompleted()
    return Disposables.create()
}
sequenceThatErrors
    .retry(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🍎
🍐
🍊
Error encountered
🍎
🍐
🍊
Error encountered
🍎
🍐
🍊
Error encountered
Unhandled error happened: test
 subscription called from:
*/
```

### To / Wait

[ReactiveX - To operator](http://reactivex.io/documentation/operators/to.html)
[Reactive Extensions再入門 その１５「To*****系メソッド」](https://blog.okazuki.jp/entry/20111208/1323357358)

ToArray / ToList / ToDictionary(ToMap) / ToLookup(ToMultiMap) 能将源数据流中的所有数据打包装进一个集合，发送这个集合并结束。

Wait 会等待源数据流发送完毕，源数据流正常结束时返回最后一个数据，源数据流出错时报错抛出异常。

![To](http://reactivex.io/documentation/operators/images/to.c.png)
![ToArray](http://reactivex.io/documentation/operators/images/toArray.png)
![ToList](http://reactivex.io/documentation/operators/images/toList.png)
![ToMap](http://reactivex.io/documentation/operators/images/toMap.png)
![ToMultiMap](http://reactivex.io/documentation/operators/images/toMultiMap.png)
- RxNET

```
var s = new Subject<int>();
s.ToArray().Subscribe(array =>
{
    Console.WriteLine("start array dump");
    foreach (var i in array)
    {
        Console.WriteLine("  array value : {0}", i);
    }
});
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(2)");
s.OnNext(2);
Console.WriteLine("OnNext(3)");
s.OnNext(3);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
OnNext(2)
OnNext(3)
OnCompleted()
start array dump
  array value : 1
  array value : 2
  array value : 3
*/
```

```
var s = new Subject<Tuple<string, int>>();
s.ToDictionary(t => t.Item1).Subscribe(dict =>
{
    Console.WriteLine("one : {0}", dict["one"]);
    Console.WriteLine("two : {0}", dict["two"]);
    Console.WriteLine("three : {0}", dict["three"]);
});
Console.WriteLine("OnNext(one)");
s.OnNext(Tuple.Create("one", 1));
Console.WriteLine("OnNext(two)");
s.OnNext(Tuple.Create("two", 2));
Console.WriteLine("OnNext(three)");
s.OnNext(Tuple.Create("three", 3));
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(one)
OnNext(two)
OnNext(three)
OnCompleted()
one : (one, 1)
two : (two, 2)
three : (three, 3)
*/
```

```
var s = new Subject<int>();
s.ToList().Subscribe(list =>
{
    foreach (var i in list)
    {
        Console.WriteLine("value : {0}", i);
    }
});
Console.WriteLine("OnNext(1)");
s.OnNext(1);
Console.WriteLine("OnNext(2)");
s.OnNext(2);
Console.WriteLine("OnNext(3)");
s.OnNext(3);
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(1)
OnNext(2)
OnNext(3)
OnCompleted()
value : 1
value : 2
value : 3
*/
```

```
var s = new Subject<Tuple<string, string>>();
s.ToLookup(t => t.Item1).Subscribe(l =>
{
    foreach (var g in l)
    {
        Console.WriteLine("Key : {0}", g.Key);
        foreach (var i in g)
        {
            Console.WriteLine("  item : {0}", i);
        }
    }
});
Console.WriteLine("OnNext(group A)");
s.OnNext(Tuple.Create("group A", "taro"));
s.OnNext(Tuple.Create("group A", "jiro"));
Console.WriteLine("OnNext(group B)");
s.OnNext(Tuple.Create("group B", "foo"));
s.OnNext(Tuple.Create("group B", "hoge"));
s.OnNext(Tuple.Create("group B", "bar"));
Console.WriteLine("OnCompleted()");
s.OnCompleted();
/*
OnNext(group A)
OnNext(group B)
OnCompleted()
Key : group A
  item : (group A, taro)
  item : (group A, jiro)
Key : group B
  item : (group B, foo)
  item : (group B, hoge)
  item : (group B, bar)
*/
```
- ToEnumerable 将 IObservable 类型的数据流转变成 IEnumerable 类型的序列。

转变之后源数据流发送的数据将变成序列中的一个数据。
[Intro to Rx - ToEnumerable](http://www.introtorx.com/Content/v1.0.10621.0/10_LeavingTheMonad.html#ToEnumerable)

```
var period = TimeSpan.FromMilliseconds(200);
var source = Observable.Timer(TimeSpan.Zero, period)
.Take(5);
var result = source.ToEnumerable();
foreach (var value in result)
{
    Console.WriteLine(value);
}
Console.WriteLine("done");
/*
0
1
2
3
4
done
*/
```
- ToTask 已经被 Wait 所取代。
[Intro to Rx - ToTask](http://www.introtorx.com/Content/v1.0.10621.0/10_LeavingTheMonad.html#ToTask)

```
var source = Observable.Interval(TimeSpan.FromSeconds(1)).Take(5);
var result = source.Wait(); //Will arrive in 5 seconds. 
Console.WriteLine(result);
/*
4
*/
```

```
var source = Observable.Throw<long>(new Exception("Fail!"));
try
{
    source.Wait();
}
catch (Exception e)
{
    Console.WriteLine(e.Message);
}
/*
Fail!
*/
```
- ToEvent 将数据流转变为具有 OnNext 事件成员的对象，转变之后数据流发送数据时 OnNext 事件将被触发。
[Intro to Rx - ToEvent](http://www.introtorx.com/Content/v1.0.10621.0/10_LeavingTheMonad.html#ToEventT)
- ToEvent 所生成对象的 OnNext 事件不是标准的 .NET 事件，没有 sender 参数。

```
var source = Observable.Interval(TimeSpan.FromSeconds(1)).Take(5);
var result = source.ToEvent();
result.OnNext += val => Console.WriteLine(val);
/*
0
1
2
3
...
*/
```
- ToEventPattern 将数据流转变为具有 OnNext 事件成员的对象，转变之后数据流发送数据时 OnNext 事件将被触发。
[Intro to Rx - ToEventPattern](http://www.introtorx.com/Content/v1.0.10621.0/10_LeavingTheMonad.html#ToEventPattern)
- 要使用 ToEventPattern，数据流中的数据必须事先转变成 EventPattern 类型。
- ToEventPattern 所生成对象的 OnNext 事件是标准的 .NET 事件，有 sender 参数。

```
public class MyEventArgs : EventArgs 
{ 
    private readonly long _value; 
    public MyEventArgs(long value) 
    { 
        _value = value; 
    } 
    public long Value 
    { 
        get { return _value; } 
    }
} 
var source = Observable.Interval(TimeSpan.FromSeconds(1))
.Select(i => new EventPattern<MyEventArgs>(null, new MyEventArgs(i)));
var result = source.ToEventPattern();
result.OnNext += (e, val) => Console.WriteLine(val.Value);
/*
0
1
2
3
...
*/
```
- RxJava

```
val values = Observable.range(10, 5)
values
    .reduce(
        ArrayList<Int>()
    ) { acc, value ->
        acc.add(value)
        acc
    }
    .dump()
/*
onSuccess: [10, 11, 12, 13, 14]
*/
```

```
val values = Observable.range(10, 5)
values
    .toList()
    .dump()
/*
onSuccess: [10, 11, 12, 13, 14]
*/
```

```
val values = Observable.range(10, 5)
values
    .toSortedList { i1, i2 -> i2 - i1 }
    .dump()
/*
onSuccess: [14, 13, 12, 11, 10]
*/
```

```
private data class Person(val name: String, val age: Int)
val values = Observable.just(
    Person("Will", 25),
    Person("Nick", 40),
    Person("Saul", 35)
)
values
    .toMap { person -> person.name }
    .dump()
/*
onSuccess: {Saul=Person(name=Saul, age=35), Nick=Person(name=Nick, age=40), Will=Person(name=Will, age=25)}
*/
```

```
val values = Observable.just(
    Person("Will", 25),
    Person("Nick", 40),
    Person("Saul", 35)
)
values
    .toMap<String, Int>(
        { person -> person.name },
        { person -> person.age })
    .dump()
/*
onSuccess: {Saul=35, Nick=40, Will=25}
*/
```

```
val values = Observable.just(
    Person("Will", 25),
    Person("Nick", 40),
    Person("Saul", 35)
)
values
    .toMap<String, Int>(
        { person -> person.name },
        { person -> person.age },
        { HashMap() })
    .dump()
/*
onSuccess: {Saul=35, Nick=40, Will=25}
*/
```

```
val values = Observable.just(
    Person("Will", 35),
    Person("Nick", 40),
    Person("Saul", 35)
)
values
    .toMultimap<Int, String>(
        { person -> person.age },
        { person -> person.name })
    .dump()
/*
onSuccess: {35=[Will, Saul], 40=[Nick]}
*/
```

```
val values = Observable.just(
    Person("Will", 35),
    Person("Nick", 40),
    Person("Saul", 35)
)
values
    .toMultimap(
        { person -> person.age },
        { person -> person.name },
        { HashMap<Int, Collection<String>>() },
        { key -> ArrayList() })
    .dump()
/*
onSuccess: {35=[Will, Saul], 40=[Nick]}
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.range(start: 1, count: 10)
    .toArray()
    .subscribe { print($0) }
    .disposed(by: disposeBag)
/*
next([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
completed
*/
```


