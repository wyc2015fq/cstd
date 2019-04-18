# ReactiveX 学习笔记（13）基础类型 - zwvista - 博客园

## [ReactiveX 学习笔记（13）基础类型](https://www.cnblogs.com/zwvista/p/9411522.html)

### Key Types and Life Management

本文主题是 Rx 中的基础类型 Subject 类及其生命周期的管理。

### 公共代码
- RxNET

```
private static void WriteSequenceToConsole(IObservable<string> sequence)
{
    sequence.Subscribe(Console.WriteLine);
}
```
- RxSwift

```
extension ObservableType {
    func addObserver(_ id: String) -> Disposable {
        return subscribe { print("Subscription:", id, "Event:", $0) }
    }
}
func writeSequenceToConsole<O: ObservableType>(name: String, sequence: O) -> Disposable {
    return sequence.subscribe { event in
        print("Subscription: \(name), event: \(event)")
    }
}
```

### Subject<T>
- RxNET

```
var subject = new Subject<string>();
WriteSequenceToConsole(subject);
subject.OnNext("a");
subject.OnNext("b");
subject.OnNext("c");
/*
a
b
c
*/
```

```
var subject = new Subject<string>();
subject.OnNext("a");
WriteSequenceToConsole(subject);
subject.OnNext("b");
subject.OnNext("c");
/*
b
c
*/
```
- RxJava

```
val subject = PublishSubject.create<Int>()
subject.onNext(1)
subject.subscribe { println(it) }
subject.onNext(2)
subject.onNext(3)
subject.onNext(4)
/*
2
3
4
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject = PublishSubject<String>()
subject.addObserver("1").disposed(by: disposeBag)
subject.onNext("🐶")
subject.onNext("🐱")
subject.addObserver("2").disposed(by: disposeBag)
subject.onNext("🅰️")
subject.onNext("🅱️")
/*
Subscription: 1 Event: next(🐶)
Subscription: 1 Event: next(🐱)
Subscription: 1 Event: next(🅰️)
Subscription: 2 Event: next(🅰️)
Subscription: 1 Event: next(🅱️)
Subscription: 2 Event: next(🅱️)
*/
```

### ReplaySubject<T>
- RxNET

```
var subject = new ReplaySubject<string>();
subject.OnNext("a");
WriteSequenceToConsole(subject);
subject.OnNext("b");
subject.OnNext("c");
/*
a
b
c
*/
```

```
var bufferSize = 2;
var subject = new ReplaySubject<string>(bufferSize);
subject.OnNext("a");
subject.OnNext("b");
subject.OnNext("c");
subject.Subscribe(Console.WriteLine);
subject.OnNext("d");
/*
b
c
d
*/
```

```
var window = TimeSpan.FromMilliseconds(150);
var subject = new ReplaySubject<string>(window);
subject.OnNext("w");
Thread.Sleep(TimeSpan.FromMilliseconds(100));
subject.OnNext("x");
Thread.Sleep(TimeSpan.FromMilliseconds(100));
subject.OnNext("y");
subject.Subscribe(Console.WriteLine);
subject.OnNext("z");
/*
x
y
z
*/
```
- RxJava

```
val s = ReplaySubject.create<Int>()
s.subscribe { v -> println("Early:$v") }
s.onNext(0)
s.onNext(1)
s.subscribe { v -> println("Late: $v") }
s.onNext(2)
/*
Early:0
Early:1
Late: 0
Late: 1
Early:2
Late: 2
*/
```

```
val s = ReplaySubject.createWithSize<Int>(2)
s.onNext(0)
s.onNext(1)
s.onNext(2)
s.subscribe { v -> println("Late: $v") }
s.onNext(3)
/*
Late: 1
Late: 2
Late: 3
*/
```

```
val s = ReplaySubject.createWithTime<Int>(150, TimeUnit.MILLISECONDS, Schedulers.trampoline())
s.onNext(0)
Thread.sleep(100)
s.onNext(1)
Thread.sleep(100)
s.onNext(2)
s.subscribe { v -> println("Late: $v") }
s.onNext(3)
/*
Late: 1
Late: 2
Late: 3
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject = ReplaySubject<String>.create(bufferSize: 1)
subject.addObserver("1").disposed(by: disposeBag)
subject.onNext("🐶")
subject.onNext("🐱")
subject.addObserver("2").disposed(by: disposeBag)
subject.onNext("🅰️")
subject.onNext("🅱️")
/*
Subscription: 1 Event: next(🐶)
Subscription: 1 Event: next(🐱)
Subscription: 2 Event: next(🐱)
Subscription: 1 Event: next(🅰️)
Subscription: 2 Event: next(🅰️)
Subscription: 1 Event: next(🅱️)
Subscription: 2 Event: next(🅱️)
*/
```

### BehaviorSubject<T>
- RxNET

```
var subject = new BehaviorSubject<string>("a");
subject.Subscribe(Console.WriteLine);
/*
a
*/
```

```
var subject = new BehaviorSubject<string>("a");
subject.OnNext("b");
subject.Subscribe(Console.WriteLine);
/*
b
*/
```

```
var subject = new BehaviorSubject<string>("a");
subject.OnNext("b");
subject.Subscribe(Console.WriteLine);
subject.OnNext("c");
subject.OnNext("d");
/*
b
c
d
*/
```

```
var subject = new BehaviorSubject<string>("a");
subject.OnNext("b");
subject.OnNext("c");
subject.OnCompleted();
subject.Subscribe(Console.WriteLine);
/*
*/
```
- RxJava

```
val s = BehaviorSubject.create<Int>()
s.onNext(0)
s.onNext(1)
s.onNext(2)
s.subscribe { v -> println("Late: $v") }
s.onNext(3)
/*
Late: 2
Late: 3
*/
```

```
val s = BehaviorSubject.create<Int>()
s.onNext(0)
s.onNext(1)
s.onNext(2)
s.onComplete()
s.subscribe(
    { v -> println("Late: $v") },
    { e -> println("Error") },
    { println("Completed") }
/*
Completed
*/
```

```
val s = BehaviorSubject.createDefault(0)
s.subscribe { v -> println(v) }
s.onNext(1)
/*
0
1
*/
```
- RxSwift

```
let disposeBag = DisposeBag()
let subject = BehaviorSubject(value: "🔴")

subject.addObserver("1").disposed(by: disposeBag)
subject.onNext("🐶")
subject.onNext("🐱")
subject.addObserver("2").disposed(by: disposeBag)
subject.onNext("🅰️")
subject.onNext("🅱️")
subject.addObserver("3").disposed(by: disposeBag)
subject.onNext("🍐")
subject.onNext("🍊")
/*
Subscription: 1 Event: next(🔴)
Subscription: 1 Event: next(🐶)
Subscription: 1 Event: next(🐱)
Subscription: 2 Event: next(🐱)
Subscription: 1 Event: next(🅰️)
Subscription: 2 Event: next(🅰️)
Subscription: 1 Event: next(🅱️)
Subscription: 2 Event: next(🅱️)
Subscription: 3 Event: next(🅱️)
Subscription: 1 Event: next(🍐)
Subscription: 2 Event: next(🍐)
Subscription: 3 Event: next(🍐)
Subscription: 1 Event: next(🍊)
Subscription: 2 Event: next(🍊)
Subscription: 3 Event: next(🍊)
*/
```

```
let disposeBag = DisposeBag()
let variable = Variable("🔴")
variable.asObservable().addObserver("1").disposed(by: disposeBag)
variable.value = "🐶"
variable.value = "🐱"
variable.asObservable().addObserver("2").disposed(by: disposeBag)
variable.value = "🅰️"
variable.value = "🅱️"
/*
Subscription: 1 Event: next(🔴)
Subscription: 1 Event: next(🐶)
Subscription: 1 Event: next(🐱)
Subscription: 2 Event: next(🐱)
Subscription: 1 Event: next(🅰️)
Subscription: 2 Event: next(🅰️)
Subscription: 1 Event: next(🅱️)
Subscription: 2 Event: next(🅱️)
Subscription: 1 Event: completed
Subscription: 2 Event: completed
*/
```

### AsyncSubject<T>
- RxNET

```
var subject = new AsyncSubject<string>();
subject.OnNext("a");
WriteSequenceToConsole(subject);
subject.OnNext("b");
subject.OnNext("c");
/*
*/
```

```
var subject = new AsyncSubject<string>();
subject.OnNext("a");
WriteSequenceToConsole(subject);
subject.OnNext("b");
subject.OnNext("c");
subject.OnCompleted();
/*
c
*/
```
- RxJava

```
val s = AsyncSubject.create<Int>()
s.subscribe { v -> println(v) }
s.onNext(0)
s.onNext(1)
s.onNext(2)
s.onComplete()
/*
2
*/
```

```
val s = AsyncSubject.create<Int>()
s.subscribe { v -> println(v) }
s.onNext(0)
s.onNext(1)
s.onNext(2)
/*
*/
```

### 调用约定
- RxNET

```
var subject = new Subject<string>();
subject.Subscribe(Console.WriteLine);
subject.OnNext("a");
subject.OnNext("b");
subject.OnCompleted();
subject.OnNext("c");
/*
a
b
*/
```
- RxJava

```
val s = ReplaySubject.create<Int>()
s.subscribe { v -> println(v) }
s.onNext(0)
s.onComplete()
s.onNext(1)
s.onNext(2)
/*
0
*/
```

```
val values = ReplaySubject.create<Int>()
values.subscribe(
    { v -> println(v) },
    { e -> println(e) },
    { println("Completed") }
)
values.onNext(0)
values.onNext(1)
values.onComplete()
values.onNext(2)
/*
0
1
*/
```

### Disposable
- RxJava

```
val values = ReplaySubject.create<Int>()
val subscription = values.subscribe(
    { v -> println(v) },
    { e -> println(e) },
    { println("Done") })
values.onNext(0)
values.onNext(1)
subscription.dispose()
values.onNext(2)
/*
0
1
*/
```

```
val values = ReplaySubject.create<Int>()
val subscription1 = values.subscribe { v -> println("First: $v") }
values.subscribe { v -> println("Second: $v") }
values.onNext(0)
values.onNext(1)
subscription1.dispose()
println("Unsubscribed first")
values.onNext(2)
/*
First: 0
Second: 0
First: 1
Second: 1
Unsubscribed first
Second: 2
*/
```


