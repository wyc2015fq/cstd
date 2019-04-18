# ReactiveX 学习笔记（2）创建数据流 - zwvista - 博客园

## [ReactiveX 学习笔记（2）创建数据流](https://www.cnblogs.com/zwvista/p/9324298.html)

### 操作符（Operators）

Rx 的操作符能够操作（创建/转换/组合） Observable。

### Creating Observables

本文主题为创建/生成 Observable 的操作符。

这里的 Observable 实质上是可观察的数据流。

[RxJava操作符（一）Creating Observables](http://mushuichuan.com/2015/12/11/rxjava-operator-1/?utm_source=androidweekly.io&utm_medium=website)
[Creation Operators](https://github.com/btroncone/learn-rxjs/blob/master/operators/creation/README.md)

### 公共代码
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

### Create / Generate

[ReactiveX - Create operator](http://reactivex.io/documentation/operators/create.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)
[Reactive Extensions再入門 その４「Timer系のファクトリメソッド」](https://blog.okazuki.jp/entry/20111106/1320584830)

Create 通过调用 Observer 的 3 个方法 OnNext, OnError, OnComplete 来创建数据流。

Generate 通过模拟 for 循环来生成数据流。

Generate 带有如下参数
- 初始状态
- 终止条件
- 一个能根据当前状态产生下一个状态的函数
- 一个能把状态转换为输出结果的函数
- （可选）一个指定状态产生时间的函数

![Create](http://reactivex.io/documentation/operators/images/create.c.png)
![Generate](http://reactivex.io/documentation/operators/images/generate.png)
- RxNET

```
var ob = Observable.Create<string>(observer =>
{
    var timer = new Timer();
    timer.Interval = 1000;
    timer.Elapsed += (s, e) => observer.OnNext("tick");
    timer.Elapsed += (s, e) => Console.WriteLine(e.SignalTime);
    timer.Start();
    return timer;
});
using (ob.Subscribe(Console.WriteLine))
    Console.ReadLine();
/*
tick
2018/07/18 10:32:09
tick
2018/07/18 10:32:10
...
*/
```

```
var source = Observable.Generate(0, i => i < 10, i => ++i, i => i * i);
var subscription = source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
/*
OnNext(0)
OnNext(1)
OnNext(4)
OnNext(9)
OnNext(16)
OnNext(25)
OnNext(36)
OnNext(49)
OnNext(64)
OnNext(81)
Completed()
*/
```

```
var source = Observable.Generate(
    0,
    i => i < 10,
    i => ++i,
    i => i * i,
    i => TimeSpan.FromMilliseconds(i * 100));
var subscription = source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
Console.ReadLine();    
/*
OnNext(0)
OnNext(1)
OnNext(4)
OnNext(9)
OnNext(16)
OnNext(25)
OnNext(36)
OnNext(49)
OnNext(64)
OnNext(81)
Completed()
*/
```
- RxJava

```
val values = Observable.create<String> { o ->
    o.onNext("Hello")
    o.onComplete()
}
values.dump()
/*
onNext: Hello
onComplete
*/
```

```
val values = Observable.generate<Int, Int>( Callable { 0 }, BiFunction { i, o ->
    if (i < 10) {
        o.onNext(i * i); i + 1
    } else {
        o.onComplete(); i
    }
}, Consumer { i -> println(i) } )
values.dump()
/*
onNext: 0
onNext: 1
onNext: 4
onNext: 9
...
onNext: 81
onComplete
10
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let myJust = { (element: String) -> Observable<String> in
    return Observable.create { observer in
        observer.on(.next(element))
        observer.on(.completed)
        return Disposables.create()
    }
}
myJust("🔴")
    .subscribe { print($0) }
    .disposed(by: disposeBag)
/*
next(🔴)
completed
*/
```

```
let disposeBag = DisposeBag()
Observable.generate(
        initialState: 0,
        condition: { $0 < 3 },
        iterate: { $0 + 1 }
    )
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
0
1
2
*/
```
- RxJS

```
/*
  Create an observable that emits 'Hello' and 'World' on
  subscription.
*/
const hello = Observable.create(function(observer) {
  observer.next('Hello');
  observer.next('World');
});

// output: 'Hello'...'World'
const subscribe = hello.subscribe(val => console.log(val));
```

```
/*
  Increment value every 1s, emit even numbers.
*/
const evenNumbers = Observable.create(function(observer) {
  let value = 0;
  const interval = setInterval(() => {
    if (value % 2 === 0) {
      observer.next(value);
    }
    value++;
  }, 1000);

  return () => clearInterval(interval);
});
// output: 0...2...4...6...8
const subscribe = evenNumbers.subscribe(val => console.log(val));
// unsubscribe after 10 seconds
setTimeout(() => {
  subscribe.unsubscribe();
}, 10000);
```

### Defer

[ReactiveX - Defer operator](http://reactivex.io/documentation/operators/defer.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)

Defer 创建数据流，但只有在有观察者来订阅时才创建数据流，而且对于每个观察者来说都创建一个新的数据流。

![Defer](http://reactivex.io/documentation/operators/images/defer.c.png)
- RxNET

```
var source = Observable.Defer<int>(() =>
{
    Console.WriteLine("# Defer method called.");
    var s = new ReplaySubject<int>();
    s.OnNext(1);
    s.OnNext(2);
    s.OnNext(3);
    s.OnCompleted();
    return s.AsObservable();
});
var subscription1 = source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
var subscription2 = source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
/*
# Defer method called.
OnNext(1)
OnNext(2)
OnNext(3)
Completed()
# Defer method called.
OnNext(1)
OnNext(2)
OnNext(3)
Completed()
*/
```
- RxJava

```
val now = Observable.just(System.currentTimeMillis())
now.dump()
Thread.sleep(1000)
now.dump()
/*
onNext: 1532965990725
onComplete
onNext: 1532965990725
onComplete
*/
```

```
val now = Observable.defer { Observable.just(System.currentTimeMillis()) }
now.dump()
Thread.sleep(1000)
now.dump()
/*
onNext: 1532965991728
onComplete
onNext: 1532965992733
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
var count = 1
let deferredSequence = Observable<String>.deferred {
    print("Creating \(count)")
    count += 1
    return Observable.create { observer in
        print("Emitting...")
        observer.onNext("🐶")
        observer.onNext("🐱")
        observer.onNext("🐵")
        return Disposables.create()
    }
}
deferredSequence
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
deferredSequence
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
Creating 1
Emitting...
🐶
🐱
🐵
Creating 2
Emitting...
🐶
🐱
🐵
*/
```

### Empty / Never / Throw

[ReactiveX - Empty, Never, and Throw operators](http://reactivex.io/documentation/operators/empty-never-throw.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)

Empty 创建一个数据流：不发送值直接结束。

Never 创建一个数据流：不发送值，永远不结束。

Throw 创建一个数据流：抛出异常。

![Empty](http://reactivex.io/documentation/operators/images/empty.c.png)
![Never](http://reactivex.io/documentation/operators/images/never.c.png)
![Throw](http://reactivex.io/documentation/operators/images/throw.c.png)
- RxJava

```
val values = Observable.empty<String>()
values.dump()
/*
onComplete
*/
```

```
val values = Observable.never<String>()
values.dump()
/*
*/
```

```
val values = Observable.error<String>(Exception("Oops"))
values.dump()
/*
onError: java.lang.Exception: Oops: Oops
*/
```
- RxSwift

```
public enum TestError : Error {
    case test
}
let disposeBag = DisposeBag()    
Observable<Int>.empty()
    .subscribe { event in
        print(event)
    }
    .disposed(by: disposeBag)
let neverSequenceSubscription = neverSequence
    .subscribe { _ in
        print("This will never be printed")
}
neverSequenceSubscription.disposed(by: disposeBag)
Observable<Int>.error(TestError.test)
    .subscribe { print($0) }
    .disposed(by: disposeBag)
/*
error(test)
*/
```
- RxJS

```
// output: 'Complete!'
const subscribe = EMPTY.subscribe({
  next: () => console.log('Next'),
  complete: () => console.log('Complete!')
});
```

```
// emits an error with specified value on subscription
const source = throwError('This is an error!');
// output: 'Error: This is an error!'
const subscribe = source.subscribe({
  next: val => console.log(val),
  complete: () => console.log('Complete!'),
  error: val => console.log(`Error: ${val}`)
});
```

### From / ToObservable

[ReactiveX - From operator](http://reactivex.io/documentation/operators/from.html)
[Reactive Extensions再入門 その６「HotなIObservableを作成するファクトリ」](https://blog.okazuki.jp/entry/20111109/1320849106)

From / ToObservable 把其他对象和数据类型转换成数据流。

![From](http://reactivex.io/documentation/operators/images/from.c.png)
![toObservable](http://reactivex.io/documentation/operators/images/toObservable.png)
- RxNET

```
var values = new List<string> { "Rx", "is", "easy" };
values.ToObservable().Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("Completed"));
/*
Rx
is
easy
Completed
*/
```

```
public class LetterRepo
{
    private readonly ReplaySubject<string> _letters;
    public LetterRepo()
    {
        _letters = new ReplaySubject<string>();
        _letters.OnNext("A");
        _letters.OnNext("B");
        _letters.OnNext("C");
    }
    public IObservable<string> GetLetters()
    {
        return _letters.AsObservable();
    }
}
var repo = new LetterRepo();
var good = repo.GetLetters();
var evil = repo.GetLetters();
good.Subscribe(
Console.WriteLine);
//Be naughty
var asSubject = evil as ISubject<string>;
if (asSubject != null)
{
    //So naughty, 1 is not a letter!
    asSubject.OnNext("1");
}
else
{
    Console.WriteLine("could not sabotage");
}
/*
A
B
C
could not sabotage
*/
```

![fromEvent](http://reactivex.io/documentation/operators/images/fromEvent.png)

```
var eventSource = new EventSource();
var source = Observable.FromEvent<EventHandler, EventArgs>(
    h => (s, e) => h(e),
    h => 
    {
        Console.WriteLine("add handler");
        eventSource.Raised += h;
    },
    h => 
    {
        Console.WriteLine("remove handler");
        eventSource.Raised -= h;
    });
var subscription1 = source.Subscribe(
    i => Console.WriteLine("1##OnNext({0})", i),
    ex => Console.WriteLine("1##OnError({0})", ex.Message),
    () => Console.WriteLine("1##Completed()"));
var subscription2 = source.Subscribe(
    i => Console.WriteLine("2##OnNext({0})", i),
    ex => Console.WriteLine("2##OnError({0})", ex.Message),
    () => Console.WriteLine("2##Completed()"));
eventSource.OnRaised();
eventSource.OnRaised();
Console.WriteLine("dispose method call.");
subscription1.Dispose();
subscription2.Dispose();
/*
add handler
1##OnNext(System.EventArgs)
2##OnNext(System.EventArgs)
1##OnNext(System.EventArgs)
2##OnNext(System.EventArgs)
dispose method call.
remove handler
*/
```

```
class EventSource
{
    public event EventHandler Raised;
    public void OnRaised()
    {
        var h = this.Raised;
        if (h != null)
        {
            h(this, EventArgs.Empty);
        }
    }
}
Func<int, int, int> asyncProcess = (x, y) =>
{
    Console.WriteLine("process start.");
    Thread.Sleep(2000);
    Console.WriteLine("process end.");
    return x + y;
};
var source = Observable.FromAsync(() => Task.FromResult(asyncProcess(10, 2)));
Console.WriteLine("subscribe1");
var subscription1 = source.Subscribe(
    i => Console.WriteLine("1##OnNext({0})", i),
    ex => Console.WriteLine("1##OnError({0})", ex.Message),
    () => Console.WriteLine("1##Completed()"));
Console.WriteLine("sleep 5sec");
Thread.Sleep(5000);
Console.WriteLine("dispose method call.");
subscription1.Dispose();
Console.WriteLine("subscribe2");
var subscription2 = source.Subscribe(
    i => Console.WriteLine("2##OnNext({0})", i),
    ex => Console.WriteLine("2##OnError({0})", ex.Message),
    () => Console.WriteLine("2##Completed()"));
Console.WriteLine("dispose method call.");
subscription2.Dispose();
/*
subscribe1
process start.
process end.
1##OnNext(12)
1##Completed()
sleep 5sec
dispose method call.
subscribe2
process start.
process end.
2##OnNext(12)
2##Completed()
dispose method call.
*/
```
- RxJava

![fromFuture](http://reactivex.io/documentation/operators/images/fromFuture.png)

```
val f = FutureTask {
    Thread.sleep(2000)
    21
}
Thread(f).start()
val values = Observable.fromFuture(f)
values.dump()
/*
onNext: 21
onComplete
*/
```

```
val f = FutureTask {
    Thread.sleep(2000)
    21
}
Thread(f).start()
val values = Observable.fromFuture(f, 1000, TimeUnit.MILLISECONDS)
values.dump()
/*
onError: java.util.concurrent.TimeoutException: null
*/
```

```
val `is` = arrayOf(1, 2, 3)
val values = Observable.fromArray(*`is`)
// val values = `is`.toObservable()
values.dump()
/*
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```

```
val input = Arrays.asList(1, 2, 3)
val values = Observable.fromIterable(input)
// val values = input.toObservable()
values.dump()
/*
onNext: 1
onNext: 2
onNext: 3
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.from(["🐶", "🐱", "🐭", "🐹"])
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🐶
🐱
🐭
🐹
*/
```

### Interval

[ReactiveX - Interval operator](http://reactivex.io/documentation/operators/interval.html)
[Reactive Extensions再入門 その４「Timer系のファクトリメソッド」](https://blog.okazuki.jp/entry/20111106/1320584830)

Interval 创建一个数据流：每隔指定时间就发送一个值，只有在取消订阅时才会结束。

![Interval](http://reactivex.io/documentation/operators/images/interval.c.png)
- RxNET

```
var interval = Observable.Interval(TimeSpan.FromMilliseconds(250));
using (interval.Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("completed")))
    Console.ReadLine();
/*
0
1
2
...
*/
```
- RxJava

```
val values = Observable.interval(1000, TimeUnit.MILLISECONDS)
val s = values.dump()
readLine()
s.dispose()
/*
onNext: 0
onNext: 1
onNext: 2
...
*/
```
- RxSwift

```
Observable<Int>.interval(1, scheduler: MainScheduler.instance)
.subscribe(onNext: { print($0) })
/*
0
1
2
...
*/
```
- RxJS

```
Rx.Observable.interval(1000).take(12).subscribe(s => console.log(`⏰ ${s} sec`));
/*
⏰ 0 sec
⏰ 1 sec
...
⏰ 10 sec
⏰ 11 sec
*/
```

### Just / Return

[ReactiveX - Just operator](http://reactivex.io/documentation/operators/just.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)

Just 创建一个数据流：发送一个或多个值然后结束。

Return 创建一个数据流：发送一个值然后结束。

![Just](http://reactivex.io/documentation/operators/images/just.c.png)
- RxNET

```
var singleValue = Observable.Return<string>("Value");
singleValue.Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("completed"));
/*
Value
completed
*/
```
- RxJava

```
val values = Observable.just("one", "two", "three")
values.dump()
/*
onNext: one
onNext: two
onNext: three
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.just("🔴")
    .subscribe { event in
        print(event)
    }
    .disposed(by: disposeBag)
/*
next(🔴)
completed
*/
```

```
let disposeBag = DisposeBag()
Observable.of("🐶", "🐱", "🐭", "🐹")
    .subscribe(onNext: { element in
        print(element)
    })
    .disposed(by: disposeBag)
/*
🐶
🐱
🐭
🐹
*/
```
- RxJS

```
// emits any number of provided values in sequence
const source = of(1, 2, 3, 4, 5);
// output: 1,2,3,4,5
const subscribe = source.subscribe(val => console.log(val));
```

```
// emits values of any type
const source = of({ name: 'Brian' }, [1, 2, 3], function hello() {
  return 'Hello';
});
// output: {name: 'Brian}, [1,2,3], function hello() { return 'Hello' }
const subscribe = source.subscribe(val => console.log(val));
```

### Range

[ReactiveX - Range operator](http://reactivex.io/documentation/operators/range.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)

Range 创建一个数据流：发送某个特定区间内连续的整数序列然后结束。

![Range](http://reactivex.io/documentation/operators/images/range.c.png)
- RxNET

```
var source = Observable.Range(1, 10);
source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
/*
OnNext(1)
OnNext(2)
...
OnNext(9)
OnNext(10)
Completed()
*/
```
- RxJava

```
val values = Observable.range(10, 15)
values.dump()
/*
onNext: 10
onNext: 11
...
onNext: 23
onNext: 24
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.range(start: 1, count: 10)
    .subscribe { print($0) }
    .disposed(by: disposeBag)
/*
next(1)
next(2)
...
next(9)
next(10)
completed
*/
```
- RxJS

```
// emit 1-10 in sequence
const source = range(1, 10);
// output: 1,2,3,4,5,6,7,8,9,10
const example = source.subscribe(val => console.log(val));
```

### Repeat

[ReactiveX - Repeat operator](http://reactivex.io/documentation/operators/repeat.html)
[Reactive Extensions再入門 その３「IObservableのファクトリメソッド」](https://blog.okazuki.jp/entry/20111104/1320409976)

Repeat 创建一个数据流：重复多次发送一个或多个值然后结束。

![Repeat](http://reactivex.io/documentation/operators/images/repeat.c.png)
- RxNET

```
var source = Observable.Repeat(2, 5);
source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
/*
OnNext(2)
OnNext(2)
OnNext(2)
OnNext(2)
OnNext(2)
Completed()
*/
```

```
var source = Observable.Range(1, 3);
source = source.Repeat(3);
source.Subscribe(
    i => Console.WriteLine("OnNext({0})", i),
    ex => Console.WriteLine("OnError({0})", ex.Message),
    () => Console.WriteLine("Completed()"));
/*
OnNext(1)
OnNext(2)
OnNext(3)
OnNext(1)
OnNext(2)
OnNext(3)
OnNext(1)
OnNext(2)
OnNext(3)
Completed()
*/
```
- RxJava

```
val words = Observable.range(0, 2)
words.repeat()
    .take(4)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 0
onNext: 1
onComplete
*/
```

```
val words = Observable.range(0, 2)
words.repeat(2)
    .dump()
/*
onNext: 0
onNext: 1
onNext: 0
onNext: 1
onComplete
*/
```

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values
    .take(2)
    .repeatWhen { ob -> ob.take(2) }
    .dump()
/*
onNext: 0
onNext: 1
onNext: 0
onNext: 1
onComplete
*/
```

```
val values = Observable.interval(100, TimeUnit.MILLISECONDS)
values
    .take(5) // Numbers 0 to 4
    .repeatWhen { ob ->
        ob.subscribe()
        Observable.interval(2, TimeUnit.SECONDS)
    } // Repeat 0 to 4 every 2s, forever
    .take(2) // Stop after second repetition
    .dump()
/*
onNext: 0
onNext: 1
onNext: 0
onNext: 1
onComplete
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
Observable.repeatElement("🔴")
    .take(3)
    .subscribe(onNext: { print($0) })
    .disposed(by: disposeBag)
/*
🔴
🔴
🔴
*/
```
- RxJS
``
### Start / FromCallable / ToAsync

[ReactiveX - Start operator](http://reactivex.io/documentation/operators/start.html)
[Intro to Rx - Start](http://www.introtorx.com/Content/v1.0.10621.0/04_CreatingObservableSequences.html#ObservableStart)
[Reactive Extensions再入門 その６「HotなIObservableを作成するファクトリ」](https://blog.okazuki.jp/entry/20111109/1320849106)

Start / FromCallable 创建一个数据流：执行指定函数并发送函数的返回值，然后结束。

Start / FromCallable 缺省情况下会在新的线程中执行指定函数。

ToAsync 可以将一个普通函数转化为创建数据流的函数。

通过调用 ToAsync 的返回值函数，可以创建一个数据流：执行指定函数并发送函数的返回值，然后结束。

可以说 ToAsync 是 Start 的带参数版本。

![Start](http://reactivex.io/documentation/operators/images/start.c.png)
![ToAsync](http://reactivex.io/documentation/operators/images/toAsync.png)
- RxNET

```
static void StartAction()
{
    var start = Observable.Start(() =>
    {
        Console.Write("Working away");
        for (int i = 0; i < 10; i++)
        {
            Thread.Sleep(100);
            Console.Write(".");
        }
    });
    start.Subscribe(
    unit => Console.WriteLine("Unit published"),
    () => Console.WriteLine("Action completed"));
}
static void StartFunc()
{
    var start = Observable.Start(() =>
    {
        Console.Write("Working away");
        for (int i = 0; i < 10; i++)
        {
            Thread.Sleep(100);
            Console.Write(".");
        }
        return "Published value";
    });
    start.Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("Action completed"));
}
StartAction();
Console.ReadLine();
StartFunc();
Console.ReadLine();
/*
Working away..........Unit published
Action completed

Working away..........Published value
Action completed
*/
```

```
var source = new Func<int,int>(i =>
{
    Console.WriteLine("background task start.");
    Thread.Sleep(2000);
    Console.WriteLine("background task end.");
    return i;
}).ToAsync();
Console.WriteLine("source(1) call.");
var subscription1 = source(1).Subscribe(
    i => Console.WriteLine("1##OnNext({0})", i),
    ex => Console.WriteLine("1##OnError({0})", ex.Message),
    () => Console.WriteLine("1##Completed()"));
Console.WriteLine("sleep 3sec.");
Thread.Sleep(3000);
Console.WriteLine("dispose method call.");
subscription1.Dispose();
Console.WriteLine("source(2) call.");
var subscription2 = source(2).Subscribe(
    i => Console.WriteLine("2##OnNext({0})", i),
    ex => Console.WriteLine("2##OnError({0})", ex.Message),
    () => Console.WriteLine("2##Completed()"));
Console.WriteLine("sleep 3sec.");
Thread.Sleep(3000);
Console.WriteLine("dispose method call.");
subscription2.Dispose();
/*
source(1) call.
background task start.
sleep 3sec.
background task end.
1##OnNext(1)
1##Completed()
dispose method call.
source(2) call.
sleep 3sec.
background task start.
background task end.
2##OnNext(2)
2##Completed()
dispose method call.
*/
```
- RxJava

![fromCallable](http://reactivex.io/documentation/operators/images/fromCallable.png)

```
val now = Observable.fromCallable { System.currentTimeMillis() }
now.dump()
Thread.sleep(1000)
now.dump()
/*
onNext: 1533301081174
onComplete
onNext: 1533301082178
onComplete
*/
```
- RxJS
``
### Timer

[ReactiveX - Timer operator](http://reactivex.io/documentation/operators/timer.html)
[Reactive Extensions再入門 その４「Timer系のファクトリメソッド」](https://blog.okazuki.jp/entry/20111106/1320584830)

Timer 创建一个数据流：延迟指定时间发送一个值然后结束。

![Timer](http://reactivex.io/documentation/operators/images/timer.c.png)
- RxNET

```
var timer = Observable.Timer(TimeSpan.FromSeconds(1));
timer.Subscribe(
    Console.WriteLine,
    () => Console.WriteLine("completed"));
/*
0
completed
*/
```
- RxJava

```
val values = Observable.timer(1, TimeUnit.SECONDS)
values.dump()
/*
onNext: 0
onComplete
*/
```
- RxSwift

```
Observable<Int>.timer(1, period: 1, scheduler: MainScheduler.instance)
    .subscribe(onNext: { print($0) })
/*
0
1
2
...
*/
```
- RxJS

```
// emit 0 after 1 second then complete, since no second argument is supplied
const source = timer(1000);
// output: 0
const subscribe = source.subscribe(val => console.log(val));
```

```
/*
  timer takes a second argument, how often to emit subsequent values
  in this case we will emit first value after 1 second and subsequent
  values every 2 seconds after
*/
const source = timer(1000, 2000);
// output: 0,1,2,3,4,5......
const subscribe = source.subscribe(val => console.log(val));
```


