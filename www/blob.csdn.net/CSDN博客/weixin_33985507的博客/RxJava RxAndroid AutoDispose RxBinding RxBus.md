# RxJava/RxAndroid/AutoDispose/RxBinding/RxBus - weixin_33985507的博客 - CSDN博客
2018年05月30日 11:11:53[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
首先:
- RXJava建议先掌握Lambda, 否则要定义很多泛型. 并且代码量冗余.
- RxJava分为三部分, `Observable` 被观察者 和 `Observer` 观察者 以及操作符.
- Reactive流式编程是微软提出来的概念, 由Netfix公司实现的架构.
- RxJava是针对Java语言, Android和JavaEE都可以使用, 但是目前主要是Android在使用.
- 操作符(或者直接叫方法)有很多变形(或者说方法的重载)
优点:
- 对于多线程任务的异步处理非常强大和方便
- 某些自己实现很复杂的功能RxJava都提供好了
官方网站:
- 
[ReactiveX](https://link.juejin.im?target=http%3A%2F%2Freactivex.io%2F)
ReactiveX支持多种语言, RxJava只是其中一种而已(还例如RxPHP, RxSwift, RxKotlin).
- 
[RxJava](https://link.juejin.im?target=http%3A%2F%2Freactivex.io%2FRxJava%2F2.x%2Fjavadoc%2F)
- 
[RxKotlin](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FReactiveX%2FRxKotlin)
针对Android的扩展
- 
[RxAndroid](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FReactiveX%2FRxAndroid)
主要是增加了Android中的线程(AndroidSchedulers)
- 
[RxBinding](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FJakeWharton%2FRxBinding)
实现了Android View的事件被观察者
- 
[AutoDispose](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fuber%2FAutoDispose)
自动解绑观察者的扩展组件(跟随Activity生命周期)
- 
[RxBus](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FAndroidKnife%2FRxBus)
RxJava实现的事件总线
**文档**
[RxJavaDoc](https://link.juejin.im?target=http%3A%2F%2Freactivex.io%2FRxJava%2Fjavadoc%2F)
[RxJava中文翻译文档](https://link.juejin.im?target=https%3A%2F%2Fmcxiaoke.gitbooks.io%2Frxdocs%2Fcontent%2FIntro.html)
## 基本使用
### 调度器
```java
public final Observable<T> subscribeOn(Scheduler scheduler)
public final Observable<T> observeOn(Scheduler scheduler)
复制代码
```
AndroidScheduler是RxAndroid添加的调度器, 主要增加了主线程.
- 被观察者默认在创建实例的当前线程下, 观察者(操作符)跟随被观察者线程;
- subscribeOn决定被观察者的线程(只有第一次指定线程有效), 某些被观察者拥有默认线程就无法变更(例如interval默认在computation线程下)
- observeOn决定以下的观察者线程(包括操作符) (可以多次变更线程), 包括observerOn以下的无默认调度器的操作符回调函数的执行线程;
操作符
- 不能指定Scheduler的操作符都是跟随被观察者的线程, 可以通过`subscribeOn`来控制被观察者发射事件的线程
- 如果操作符拥有默认的线程调度器将无法通过subscribeOn来控制(但是一般有重载方法可以在参数中控制)
#### 调度器类型
Schedulers.computation( )
用于计算任务，如事件循环或和回调处理，不要用于IO操作(IO操作请使用Schedulers.io())；
默认线程数等于处理器的数量
Schedulers.from(executor)
使用指定的Executor作为调度器
Schedulers.immediate( )
在当前线程立即开始执行任务
Schedulers.io( )
用于IO密集型任务，如异步阻塞IO操作，这个调度器的线程池会根据需要增长；对于普通的计算任务，请使用Schedulers.computation()
Schedulers.newThread( )
为每个任务创建一个新线程
Schedulers.trampoline( )
​	当其它排队的任务完成后，在当前线程排队开始执行
Schedulers.single()
始终只有一个线程，任务以先进先出的顺序被执行...
### 批量解除订阅关系
#### CompositeDisposable
示例:
```java
CompositeDisposable compositeDisposable=new CompositeDisposable();
Observable.create(new ObservableOnSubscribe<Integer>() {
            @Override
            public void subscribe(ObservableEmitter<Integer> emitter) throws Exception {
                emitter.onNext(1);
                emitter.onComplete();或者 emitter.onError(new Throwable("O__O "));
            }
        }).subscribe(new Observer<Integer>() {
            private Disposable mDisposable;
            @Override
            public void onSubscribe(Disposable d) {
               // 订阅  
                mDisposable = d;
               // 添加到容器中 
                compositeDisposable.add(d);
            }
            @Override
            public void onNext(Integer value) {
               // 判断mDisposable.isDisposed() 如果解除了则不需要处理
            }
            @Override
            public void onError(Throwable e) {
            }
            @Override
            public void onComplete() {
            }
        });
        // 解除所有订阅者
        compositeDisposable.clear();
复制代码
```
需要强调的是Observable只有存在订阅者的时候才会发送事件, 如果取消了订阅者并不会发送任何事件, 不用担心内存泄漏等问题.
### 周期函数
`doOn**()`这一系列方法都是可以在观察者回调之前执行操作
```java
public final Flowable<T> doOnDispose(Action onCancel)
    public final Flowable<T> doOnComplete(Action onComplete)
    public final Flowable<T> doOnEach(Consumer<? super Notification<T>> onNotification)
    // 发送任何事件都会触发回调(包括onError onComplete)
    public final Flowable<T> doOnEach(org.reactivestreams.Subscriber<? super T> subscriber)
    public final Flowable<T> doOnError(Consumer<? super java.lang.Throwable> onError)
    public final Flowable<T> doOnLifecycle(Consumer<? super org.reactivestreams.Subscription> onSubscribe,
                                           LongConsumer onRequest,
                                           Action onCancel)
    public final Flowable<T> doOnNext(Consumer<? super T> onNext)
    public final Flowable<T> doOnRequest(LongConsumer onRequest)
    // 该方法用于跟踪背压, 常常用于调试, 故Observable没有该方法
    public final Flowable<T> doOnSubscribe(Consumer<? super org.reactivestreams.Subscription> onSubscribe)
    // 在订阅被观察者前回调
    public final Flowable<T> doOnTerminate(Action onTerminate)
    // 该回调会在onComplete和onError方法前回调, 无论是异常还是完成
复制代码
```
Notification
> 
因为doOnEach回调会在所有事件都触发, 所以Notification包含了所有事件的信息
```java
java.lang.Throwable	getError()
// 如果触发的事件是onError则会返回异常信息, 否则null
T	getValue()
// 如果触发的事件是onNext则会返回该值, 否则返回null
boolean	isOnComplete()
boolean	isOnError()
boolean	isOnNext()
// Notification提供静态方法直接构成出三种事件的实例对象
static <T> Notification<T>	createOnComplete()
static <T> Notification<T>	createOnError(java.lang.Throwable error)
static <T> Notification<T>	createOnNext(T value)
复制代码
```
## 创建操作符
创建操作符全部属于静态方法调用
### 创建被观察者
#### Create
```java
public static <T> Observable<T> create(ObservableOnSubscribe<T> source)
复制代码
```
**ObservableOnSubscribe**中只有一个方法
```java
void subscribe(ObservableEmitter<T> e)
               throws java.lang.Exception
复制代码
```
`ObservableEmitter` 译为发射器. 可以通过三种方法发送事件
```java
void	onComplete()
void	onError(java.lang.Throwable error)
void	onNext(T value)
复制代码
```
Tip:
- `onError()`和`onComplete()`不能同时使用
- `onError()`不能多次使用, `onComplete()`可以
其他方法:
```java
void setCancellable(Cancellable c)
// 设置一个取消事件监听器
void setDisposable(Disposable d)
boolean	isDisposed()
复制代码
```
#### Just
通过直接传入N个参数来批量发送事件(最多九个参数)
```java
static <T> Observable<T>	just(T item) 
复制代码
```
> 
全部事件发送完毕后会回调onComplete方法
#### FromArray
通过传入数组或者集合来发送事件
```java
static <T> Observable<T>	fromArray(T... items)
复制代码
```
#### FromIterable
Iterable是可遍历集合的根接口, 可以通过发送集合;
```java
List<String> list = new ArrayList<>();
list.add("a");
list.add("b");
list.add("c");
Flowable.fromIterable(list).subscribe(
        s -> Log.i("tag", s)
);
复制代码
```
更多的方法
> - Javadoc: from(array)
- Javadoc: from(Iterable)
- Javadoc: from(Future)
- Javadoc: from(Future,Scheduler)
- Javadoc: from(Future,timeout, timeUnit)
#### defer
只能够在回调函数中创建被观察者
```java
public static <T> Observable<T> defer(java.util.concurrent.Callable<? extends ObservableSource<? extends T>> supplier)
复制代码
```
### 追加事件
#### StartWith
在已经创建的事件前面再添加事件
```java
public final Observable<T> startWith(T item)
public final Observable<T> startWithArray(T... items)
复制代码
```
### 直接结束事件
#### Empty
不发送任何事件给观察者, 立即回调onComplete()
```java
Flowable.empty().subscribe(
        obj -> Log.i("tag", "next" + obj.toString()),
        e -> Log.i("tag", "error"),
        () -> Log.i("tag", "complete"));
复制代码
```
#### Never
不发送任何事件给观察者, 并且不执行任何方法(例如OnComplete)
#### Error
不发送任何事件, 但是会直接执行onError()
### 轮循器
#### Interval
定时控制间隔发送事件. 默认在计算线程(ComputationScheduler), 可以指定线程.
只能控制间隔时间
```java
public static Observable<java.lang.Long> interval(long period,
                                                  java.util.concurrent.TimeUnit unit)
public static Observable<java.lang.Long> interval(long initialDelay, // 第一次的延迟时间
                                                    long period, // 间隔时间
                                                    java.util.concurrent.TimeUnit unit) // 时间单位
复制代码
```
#### IntervalRange
更加精确的范围内发送计时器事件,  默认在计算线程(ComputationScheduler), 可以指定线程.
可以控制发送次数
```java
public static Observable<java.lang.Long> intervalRange(long start,  // 开始数字
                                                       long count, // 总共次数
                                                       long initialDelay, // 初始间隔时间
                                                       long period, // 后面每次间隔时间
                                                       java.util.concurrent.TimeUnit unit) // 时间单位
复制代码
```
#### Range
可以设置发送次数的定时器, 没有默认在特定的线程执行; 不会根据时间发送, 直接一次性按照按照顺序发送完毕.
```java
// 发送int事件类型
public static Observable<java.lang.Integer> range(int start, // 开始
                                                  int count) // 结束
// 发送long的事件类型
public static Observable<java.lang.Long> rangeLong(long start,
                                                     long count)
复制代码
```
#### Timer
定时器操作符. Timer默认在computationScheduler(计算线程)上运行, 可以指定线程.
```java
public static Observable<java.lang.Long> timer(long delay, // 间隔时间
                                               java.util.concurrent.TimeUnit unit) // 时间单位
复制代码
```
#### TimerInterval
记录轮循器的信息
- 时间间隔
- 时间单位
- 值
```java
public final Observable<Timed<T>> timeInterval()
// 时间间隔设置固定单位
public final Observable<Timed<T>> timeInterval(java.util.concurrent.TimeUnit unit)
复制代码
```
输出示例:
```bash
// 无参默认单位为毫秒
Timed[time=1003, unit=MILLISECONDS, value=12]
// 设置单位为秒
Timed[time=1, unit=SECONDS, value=40]
复制代码
```
#### Repeat
重复发送事件
- 无限循环
- 指定时间内循环
- 停止
```bash
public final Observable<T> repeat()
// 无限循环
public final Observable<T> repeat(long times)
// 设置循环次数
public final Observable<T> repeatUntil(BooleanSupplier stop)
// 设置循环结束条件
// 添加一个被观察者作为重新发送事件的条件
public final Observable<T> repeatWhen(Function<? super Observable<java.lang.Object>,? extends ObservableSource<?>> handler)
复制代码
```
示例
```bash
io.reactivex.Observable.just(1)
                .repeatUntil(
                        new BooleanSupplier() {
                            /**
                             * @return 返回true表示结束循环
                             * @throws Exception
                             */
                            @Override
                            public boolean getAsBoolean() throws Exception {
                                return true;
                            }
                        })
                .subscribe(System.out::println);
复制代码
```
#### RepeatWhen
如果回调函数中的被观察者发送onComplete和onError事件不会进入重复事件
但是如果发送onNext事件就会导致重复发送
```bash
Observable.intervalRange(0, 5, 1, 1, TimeUnit.SECONDS, TrampolineScheduler.instance())
    .repeatWhen(
    new Function<Observable<Object>, ObservableSource<?>>() {
        @Override
        public ObservableSource<?> apply(Observable<Object> objectObservable)
            throws Exception {
            // 源被观察者结束后(onComplete)等待五秒再次重新发送
            return Observable.interval(5, TimeUnit.SECONDS);
        }
    })
    .subscribe(
    new Consumer<Long>() {
        @Override
        public void accept(Long aLong) throws Exception {
            // do something
        }
    });
复制代码
```
### 多观察者创建
#### Concat
将多个被观察者连接起来按照顺序发送
`ConcatArray`和`Concat`操作符其实都一样, 只不过可以接受数组而已
#### Merge
将多个被观察者合并, 遵守时间顺序(不遵守参数添加顺序)
```java
public static <T> Observable<T> mergeArray(int maxConcurrency,
                                           int bufferSize,
                                           ObservableSource<? extends T>... sources)
复制代码
```
#### Zip
可以将多个发射器发送的事件**对应发送顺序**组合成一个然后统一一次接收事件, 遵守两两合并的原则.
如果存在异步情况, 将会等待需要合并的两个事件同时执行完毕后再发送给观察者;
- 遵守订阅顺序
```java
public static <T,R> Observable<R> zip(ObservableSource<? extends ObservableSource<? extends T>> sources,
                                      Function<? super java.lang.Object[],? extends R> zipper)
复制代码
```
示例:
```java
Observable.zip(getStringObservable(), getIntegerObservable(),
               new BiFunction<String, Integer, String>() {
                 @Override public String apply(@NonNull String s, @NonNull Integer integer)
                   throws Exception {
                   // 在这里将两个发射器的事件合并然后统一发送
                   return s + integer;
                 }
               }).subscribe(new Consumer<String>() {
  @Override public void accept(@NonNull String s) throws Exception {
    // 这里只会接受到apply的返回值
  }
});
复制代码
```
#### ZipWith
将传入的被观察者和源被观察者对应组合(Zip)该方法属于非静态方法
```bash
Observable.just(1).zipWith(Observable.just(2), new BiFunction<Integer, Integer, String>() {
        @Override
        public String apply(Integer integer, Integer integer2) throws Exception {
            System.out.println("integer = [" + integer + "], integer2 = [" + integer2 + "]");
            return integer + "" + integer2;
        }
    }).subscribe(new Consumer<String>() {
        @Override
        public void accept(String s) throws Exception {
            // 结果: s = [12]
            System.out.println("s = [" + s + "]");
        }
    });
复制代码
```
#### CombineLast
最后一个被观察者的所有事件依次和前面的被观察者的最后一个事件合并.
- 遵守订阅观察者顺序
```java
public static <T1,T2,T3,R> Observable<R> combineLatest(ObservableSource<? extends T1> source1,
                                                       ObservableSource<? extends T2> source2,
                                                       Function3<? super T1,? super T2,? super T3,? extends R> combiner)
复制代码
```
示例结果:
最后一个被观察者和之前的所有的被观察者的最后一个事件同时被接收到
```bash
s = [轮循器一10轮循器二10轮循器三1]
s = [轮循器一10轮循器二10轮循器三2]
s = [轮循器一10轮循器二10轮循器三3]
s = [轮循器一10轮循器二10轮循器三4]
s = [轮循器一10轮循器二10轮循器三5]
s = [轮循器一10轮循器二10轮循器三6]
s = [轮循器一10轮循器二10轮循器三7]
s = [轮循器一10轮循器二10轮循器三8]
s = [轮循器一10轮循器二10轮循器三9]
s = [轮循器一10轮循器二10轮循器三10]
复制代码
```
## 变化操作符
### 转换Observable
#### FlatMap
将事件拦截然后转成被观察者再次发送
> 
交错顺序, 即发射器发送事件的时候可能是异步或者延迟的.
```bash
public final <R> Observable<R> flatMap(Function<? super T,? extends ObservableSource<? extends R>> mapper)
// onNext/onError/onComplete 分别回调
public final <R> Observable<R> flatMap(Function<? super T,? extends ObservableSource<? extends R>> onNextMapper,
                                                               Function<? super java.lang.Throwable,? extends ObservableSource<? extends R>> onErrorMapper,
                                                               java.util.concurrent.Callable<? extends ObservableSource<? extends R>> onCompleteSupplier)
    
public final <U,R> Observable<R> flatMap(Function<? super T,? extends ObservableSource<? extends U>> mapper,
                                                                 BiFunction<? super T,? super U,? extends R> resultSelector)
复制代码
```
#### FlatMapIterable
```java
public final <U> Observable<U> flatMapIterable(Function<? super T,? extends java.lang.Iterable<? extends U>> mapper)
// 就是转成集合类型被观察者接收到
复制代码
```
重载方法将可迭代对象最终又转换成单个对象
```java
public final <U,V> Observable<V> flatMapIterable(
    Function<? super T,? extends java.lang.Iterable<? extends U>> mapper,
    BiFunction<? super T,? super U,? extends V> resultSelector)
复制代码
```
示例
```java
Observable.just(1, 2, 3, 4, 5).flatMapIterable(new Function<Integer, Iterable<String>>() {
      @Override public Iterable<String> apply(Integer integer) throws Exception {
        Log.d("日志",
            "(MainActivity.java:32) ___ " + "apply() called with: integer = [" + integer + "]");
        
        // iterable属于所有集合的根接口
        ArrayList<String> strings = new ArrayList<>();
        strings.add(integer.toString() + "集合中");
        return strings;
      }
    }, new BiFunction<Integer, Object, String>() {
      /**
       * 得到一个最终被观察者接受的事件
       * @param t1 发射器的事件
       * @param t2 被添加到集合中对象
       * @return 最终被观察者接受的事件
       * @throws Exception 如果返回null将抛出异常
       */
      @Override public String apply(Integer integer, Object o) throws Exception {
        Log.d("日志", "(MainActivity.java:39) ___ "
            + "apply() called with: integer = ["
            + integer
            + "], o = ["
            + o
            + "]");
        // 如果返回null则会抛出异常进入onError
        return "吴彦祖";
      }
    }).subscribe(new Observer<String>() {
      @Override public void onSubscribe(Disposable d) {
        Log.i("日志", "(MainActivity.java:49) ___ onSubscribe");
      }
      @Override public void onNext(String s) {
        Log.d("日志", "(MainActivity.java:53) ___ " + "onNext() called with: s = [" + s + "]");
      }
      @Override public void onError(Throwable e) {
        Log.i("日志", "(MainActivity.java:57) ___ onError");
      }
      @Override public void onComplete() {
        Log.i("日志", "(MainActivity.java:61) ___ onComplete");
      }
    });
复制代码
```
#### FatMapCompletable
只能接受到onComplete回调
```java
public final Completable flatMapCompletable(Function<? super T,? extends CompletableSource> mapper)
    public final Completable flatMapCompletable(Function<? super T,? extends CompletableSource> mapper,
                                                boolean delayErrors)
复制代码
```
#### FlatMapMaybe
```java
public final <R> Observable<R> flatMapMaybe(Function<? super T,? extends MaybeSource<? extends R>> mapper);
public final <R> Observable<R> flatMapMaybe(Function<? super T,? extends MaybeSource<? extends R>> mapper,
                                            boolean delayErrors)
复制代码
```
#### FlatMapSingle
单一接受者
```java
public final <R> Observable<R> flatMapSingle(Function<? super T,? extends SingleSource<? extends R>> mapper)
public final <R> Observable<R> flatMapSingle(Function<? super T,? extends SingleSource<? extends R>> mapper,
                                                 boolean delayErrors)
复制代码
```
#### ConcatMap
和FlatMap的区别是保证顺序发射(不存在交错顺序), 内部使用Concat实现;
例如: 两个异步的被观察者无论如何都会按照你参数添加的顺序发送事件
```java
public final <R> Flowable<R> concatMap(Function<? super T,? extends org.reactivestreams.Publisher<? extends R>> mapper)
public final <R> Flowable<R> concatMap(Function<? super T,? extends org.reactivestreams.Publisher<? extends R>> mapper,
                                           int prefetch)
复制代码
```
#### ConcatMapDelayError
延迟异常抛出到所有事件发送完毕后
```java
public final <R> Flowable<R> concatMapDelayError(Function<? super T,? extends org.reactivestreams.Publisher<? extends R>> mapper)
public final <R> Flowable<R> concatMapDelayError(Function<? super T,? extends org.reactivestreams.Publisher<? extends R>> mapper,
                                                     int prefetch,
                                                     boolean tillTheEnd)
复制代码
```
#### ConcatMapEager
将所有被观察者的事件全部添加到一个被观察者上, 然后一次性发送;
```bash
public final <R> Observable<R> concatMapEager(Function<? super T,? extends ObservableSource<? extends R>> mapper)
复制代码
```
### 事件变化
#### Map
将类型转换
```bash
public final <R> Flowable<R> map(Function<? super T,? extends R> mapper)
复制代码
```
#### SwitchMap
每次源被观察者发送数据的时候都会向观察者发送一个新的被观察者, 但是如果有延迟操作就只会发送最后一个源被观察者创建的新被观察者;
> 
在RxJava2之前SwitchMap叫做FlatMapLatest, 这样是否更加容易理解
```java
public final <R> Observable<R> switchMap(Function<? super T,? extends ObservableSource<? extends R>> mapper)
复制代码
```
示例
```java
Observable.just(1, 2, 3)
            .switchMap(new Function<Integer, ObservableSource<?>>() {
              @Override
              public ObservableSource<Long> apply(Integer integer) throws Exception {
                return interval1;
              }
            })
            .subscribe(ele -> Log.d("日志", "(MainActivity.java:39) ___ Result = " + ele));
复制代码
```
### 包装事件成容器
#### Buffer
创建一个List集合存储事件发送给观察者
- 数量
- 跨度
- 时间
```java
public final Observable<java.util.List<T>> buffer(int count) // 向List中添加事件数量
public final Observable<java.util.List<T>> buffer(int count,
                                                  int skip) // 分段事件跨度
复制代码
```
例如: {1.2.3.4.5} count = 3, skip = 2.
接收到的事件就为 {1,2,3} {3,4,5} {5}
```java
public final <B> Observable<java.util.List<T>> buffer(java.util.concurrent.Callable<? extends ObservableSource<B>> boundarySupplier)
// 在回调接口中boundarySupplier封装成集合Collection
// 缓存一定时间内的事件添加到集合中
public final Observable<java.util.List<T>> buffer(long timespan,
                                                  long timeskip,
                                      				java.util.concurrent.TimeUnit unit)
复制代码
```
#### Window
创建一个被观察者存储事件发送给观察者
类似Buffer但是分组的结果是被观察者, 类似GroupBy但是不是依靠于Key来分组;
分时间
```java
public final Observable<Observable<T>> window(long timespan,
                                              java.util.concurrent.TimeUnit unit)
// 按照时间跨度分组
public final Observable<Observable<T>> window(long timespan,
                                                  java.util.concurrent.TimeUnit unit,
                                                  long count)
    // 按照时间跨度内的数量分组 (规定事件内只能添加规定数量的事件到被观察者内)
public final Observable<Observable<T>> window(long timespan,
                                                  long timeskip,
                                                  java.util.concurrent.TimeUnit unit)
    // 按照时间跨度分组
public final Observable<Observable<T>> window(long timespan,
                                                  java.util.concurrent.TimeUnit unit,
                                                  long count,
                                                  boolean restart)
复制代码
```
分观察者发送
> 
类似Buffer, 不同的是Buffer是将事件封装成一个List集合, 而Window是将一定事件数量封装成一个新的被观察者.
```java
public final Observable<Observable<T>> window(long count)
    // 按照数量分组
public final Observable<Observable<T>> window(long count,
                                                long skip)
// 按照跨度分组
复制代码
```
示例
```java
Observable.just(1, 2, 3, 4).window(3).subscribe(new Consumer<Observable<Integer>>() {
      @Override public void accept(Observable<Integer> integerObservable) throws Exception {
        Log.i("日志", "(MainActivity.java:19) ___ 观察者");
        
        integerObservable.subscribe(new Consumer<Integer>() {
          @Override public void accept(Integer integer) throws Exception {
            Log.i("日志", "(MainActivity.java:23) ___ 接受到事件");
          }
        });
      }
    });
复制代码
```
```java
public final <B> Observable<Observable<T>> window(java.util.concurrent.Callable<? extends ObservableSource<B>> boundary)
    
public final <U,V> Observable<Observable<T>> window(ObservableSource<U> openingIndicator,
                                                                            Function<? super U,? extends ObservableSource<V>> closingIndicator)
public final <U,V> Observable<Observable<T>> window(ObservableSource<U> openingIndicator,
                                                                            Function<? super U,? extends ObservableSource<V>> closingIndicator,
                                                                            int bufferSize)
复制代码
```
#### GroupBy
根据key将Observable被观察者的事件分组变更为`GroupedObservable`, 该类继承自Observable, 但是新增一个方法
`getKey()`可以获取回调中返回对象key;
```java
// 在回调中通过key来分组
public final <K> Observable<GroupedObservable<K,T>> groupBy(Function<? super T,? extends K> keySelector)
// 分组然后再次发送事件
public final <K,V> Observable<GroupedObservable<K,V>> groupBy(Function<? super T,? extends K> keySelector,
                                                                                      Function<? super T,? extends V> valueSelector)
复制代码
```
#### Scan
```java
public final Flowable<T> scan(BiFunction<T,T,T> accumulator)
public final <R> Flowable<R> scan(R initialValue,
                                      BiFunction<R,? super T,R> accumulator)
复制代码
```
回调返回值将作为下次回调方法的第一个参数
```java
Observable.just(1, 2, 3, 4, 5).scan(110, new BiFunction<Integer, Integer, Integer>() {
      /**
       * 第一次发射器发射的值会由观察者接收到, 如果设置了initialValue则第一次接收到该初始值. 然后后面都会该方法的返回值
       * @return 返回的值最终被观察者接受到
       * @throws Exception on error
       */
      @Override
      public Integer apply(Integer integer, Integer integer2) throws Exception {
        return integer2 + 1;
      }
    }).subscribe(new Consumer<Integer>() {
      @Override
      public void accept(Integer integer) throws Exception {
        Log.d("日志",
              "(MainActivity.java:37) ___ " + "accept() called with: integer = [" + integer + "]");
      }
    });
复制代码
```
#### ScanWith
第一个回调函数中返回值是第二个回调函数中的第一个参数(即初始值)
```java
public final <R> Flowable<R> scanWith(java.util.concurrent.Callable<R> seedSupplier,
                                      BiFunction<R,? super T,R> accumulator)
复制代码
```
#### ToList
将事件转成集合一次接受; 但是主要要求被观察者的事件类型统一;
```java
public final Single<java.util.List<T>> toList()
public final Single<java.util.List<T>> toList(int capacityHint)
// 集合初始化空间
public final <U extends java.util.Collection<? super T>> Single<U> toList(java.util.concurrent.Callable<U> collectionSupplier)
复制代码
```
## 过滤操作符
### 延迟
#### Delay
`delay`操作符会延迟每个事件发送的时间(包括onComplete但包括onError)
```bash
public final Observable<T> delay(long delay,
                                                         java.util.concurrent.TimeUnit unit)
// 延迟时间单位
public final <U> Observable<T> delay(Function<? super T,? extends ObservableSource<U>> itemDelay)
// 在回调中通过观察者演出
复制代码
```
```bash
Observable.just(1, 2, 3)
        .delay(
            new Function<Integer, ObservableSource<Integer>>() {
              /**
               * 每次源被观察者发送事件都会执行该方法, 但是事件不会被观察者接收到(处于延迟中).
               *
               * @param integer 事件
               * @return 返回的被观察者, 当这个观察者发送事件的时候将终止延迟
               * @throws Exception
               */
              @Override
              public ObservableSource<Integer> apply(Integer integer) throws Exception {
                System.out.println("integer = [" + integer + "]");
                return Observable.just(1)
                    .delay(3, TimeUnit.SECONDS, TrampolineScheduler.instance());
              }
            })
        .subscribe(
            new Consumer<Integer>() {
              @Override
              public void accept(Integer integer) throws Exception {
                System.out.println("result = [" + integer + "]");
              }
            });
复制代码
```
### 去除重复事件
#### Distinct
相同事件去重
```java
public final Flowable<T> distinct()
// 去除所有重复的事件
public final <K> Observable<T> distinct(Function<? super T,K> keySelector)
// 在回调内返回一个泛型值, 然后比较该泛型值来判断是否属于重复
public final <K> Observable<T> distinct(Function<? super T,K> keySelector,
                                                                java.util.concurrent.Callable<? extends java.util.Collection<? super K>> collectionSupplier)
复制代码
```
示例:
```java
/*演示只取两个偶数*/
    Observable.just(1, 2, 3, 4, 5, 6).distinct(new Function<Integer, String>() {
      /**
       * 该方法每次发送事件都会回调
       * @throws Exception
       */
      @Override public String apply(Integer integer) throws Exception {
        return integer % 2 == 0 ? "偶数" : "奇数";
      }
    }).subscribe(new Consumer<Integer>() {
      @Override public void accept(Integer integer) throws Exception {
        Log.d("日志",
            "(MainActivity.java:34) ___ " + "accept() called with: integer = [" + integer + "]");
      }
    });
复制代码
```
#### distinctUntilChanged
> 
去除临近的重复事件
```java
public final Flowable<T> distinctUntilChanged()
// 只会去除邻近的重复事件
public final Observable<T> distinctUntilChanged(BiPredicate<? super T,? super T> comparer)
// 该回调会每次返回邻近的两个事件, 然后你自己在回调内比较两个值是否算重复, 返回布尔类型
public final <K> Observable<T> distinctUntilChanged(Function<? super T,K> keySelector)
// 在回调内返回一个泛型值, 然后比较该泛型值来判断是否属于重复
复制代码
```
示例:
```java
Observable.just(1, 2, 2, 4, 5, 6).distinctUntilChanged(new BiPredicate<Integer, Integer>() {
      @Override public boolean test(Integer integer, Integer integer2) throws Exception {
        
        return integer.equals(integer2);
      }
    }).subscribe(new Consumer<Integer>() {
      @Override public void accept(Integer integer) throws Exception {
        Log.d("日志",
            "(MainActivity.java:34) ___ " + "accept() called with: integer = [" + integer + "]");
      }
    });
复制代码
```
### 只发送指定数据
#### Element
只发送指定位置(索引)的事件
```java
public final Maybe<T> elementAt(long index) // 索引
public final Single<T> elementAt(long index,
                                 T defaultItem) // 索引越界后发送事件
public final Maybe<T> firstElement()
// 只发送第一个
public final Maybe<T> lastElement()
public final Completable ignoreElements()
// 忽略全部事件
public final Single<T> elementAtOrError(long index)
// 如果事件为空, 则会抛出异常
复制代码
```
#### Debounce
发送事件后的一定时间内再次发送的事件都会被舍弃, 并且重新开始计时.
这个场景常用语搜索输入框的自动提示: 你连续输入文字会不断地发送事件,即会导致时间不断地被重置始终无法成功发送事件. 只有你在停止输入后才会成功发送事件.
```java
public final <U> Observable<T> debounce(Function<? super T,? extends ObservableSource<U>> debounceSelector)
public final Observable<T> debounce(long timeout,
                                        java.util.concurrent.TimeUnit unit)
复制代码
```
同样还有一种实现方式
```java
public final Observable<T> throttleWithTimeout(long timeout,
public final Observable<T> throttleWithTimeout(long timeout,
                                               java.util.concurrent.TimeUnit unit,
                                               Scheduler scheduler)
复制代码
```
#### Filter
筛选事件,例如字符串为空则不发送事件
```java
Observable.just(1, 20, 65, -5, 7, 19)
                .filter(new Predicate<Integer>() {
                    @Override
                    public boolean test(@NonNull Integer integer) throws Exception {
                      // 根据返回结果布尔类型确定是否拦截事件
                        return integer >= 10;
                    }
                }).subscribe(new Consumer<Integer>() {
            @Override
            public void accept(@NonNull Integer integer) throws Exception {
                
            }
        });
复制代码
```
#### All
判断所有事件是否符合条件, 回调函数会执行多次但是观察者只能收到一个布尔类型值;
```java
public final Single<java.lang.Boolean> all(Predicate<? super T> predicate)
复制代码
```
```java
Flowable.just(1, 2, 3, 4).all(new Predicate<Integer>() {
    @Override public boolean test(Integer integer) throws Exception {
        return integer < 4;
    }
}).subscribe(new Consumer<Boolean>() {
    @Override public void accept(Boolean aBoolean) throws Exception {
    }
});
复制代码
```
#### First
只发射第一个事件, 否则就发射默认的事件
```java
public final Single<T> first(T defaultItem)
public final Single<T> firstOrError()
// 如果为空则抛出异常进入OnError()
复制代码
```
#### Last
只发射最后一个事件, 否则就发射默认的事件
```java
public final Single<T> last(T defaultItem)
复制代码
```
#### Amb
仅仅处理第一个发射事件的被观察者, 其他的观察者都会被抛弃.
要求添加的所有被观察者的事件类型需要统一;
```java
public static <T> Observable<T> amb(java.lang.Iterable<? extends ObservableSource<? extends T>> sources)
// 集合类型
复制代码
```
#### ambArray
```java
public static <T> Observable<T> ambArray(ObservableSource<? extends T>... sources)
// 可变参数类型
复制代码
```
#### ambWith
非静态函数, 结束对源被观察者的订阅, 然后对当前指定参数的被观察者订阅;
```java
public final Observable<T> ambWith(ObservableSource<? extends T> other)
复制代码
```
#### Blocking
该操作符会打断RxJava的链式调用;
只发送第一个或者最后一个事件
```bash
public final T blockingFirst()
public final T blockingFirst(T defaultItem)
public final T blockingLast()
public final T blockingLast(T defaultItem)
public final T blockingSingle()
复制代码
```
> - 如果没有设置默认值的话, 被观察者事件为空将抛出异常
- 使用single, 但是如果observable的事件不止一个, 将抛出异常
示例:
```bash
Long aLong = Observable.intervalRange(0, 5, 1, 1, TimeUnit.SECONDS)
.blockingFirst();
复制代码
```
循环
```bash
public final void blockingForEach(Consumer<? super T> onNext)
public final java.lang.Iterable<T> blockingIterable()
public final java.lang.Iterable<T> blockingIterable(int bufferSize)
public final java.lang.Iterable<T> blockingMostRecent(T initialValue)
public final void blockingSubscribe()
复制代码
```
### 限制接受事件
- 指定时间内 第一个
- 指定时间内 最后一个
#### throttleFirst
只发送一定时间内的第一个事件, 默认在ComputationScheduler上执行, 但是可以指定Scheduler
```java
public final Observable<T> throttleFirst(long windowDuration,
                                         java.util.concurrent.TimeUnit unit)
public final Observable<T> throttleFirst(long skipDuration,
                                             java.util.concurrent.TimeUnit unit,
                                             Scheduler scheduler)
复制代码
```
#### throttleLast
> 
只发送一定时间内的最后一个事件
```java
public final Observable<T> throttleLast(long  intervalDuration, 
                                        java.util.concurrent.TimeUnit unit)
public final Observable<T> throttleLast(long intervalDuration,
                                                                java.util.concurrent.TimeUnit unit,
                                                                Scheduler scheduler)
复制代码
```
#### throttleWithTimeout
发送事件A后将触发计时, 如果规定时间内有新的事件B发送, 将丢弃事件A; 功能和`debounce` 相同, 但是`debounce`可以添加一个Observable作为计时;
```java
public final Observable<T> throttleWithTimeout(long timeout,
                                               java.util.concurrent.TimeUnit unit)
public final Observable<T> throttleWithTimeout(long timeout,
                                                 java.util.concurrent.TimeUnit unit,
                                                 Scheduler scheduler)
复制代码
```
#### Sample
控制间隔, 在一定时间内只取最后发射的事件,  可以指定线程. 等同于`throttleLast`
```java
public final Observable<T> sample(long period,
                                                          java.util.concurrent.TimeUnit unit)
public final Observable<T> sample(long period,
                                                          java.util.concurrent.TimeUnit unit,
                                                          boolean emitLast)
复制代码
```
```java
public final <U> Observable<T> sample(ObservableSource<U> sampler)
public final <U> Observable<T> sample(ObservableSource<U> sampler,
                                                              boolean emitLast)
复制代码
```
#### TimeOut
```java
public final Observable<T> timeout(long timeout,
                                     java.util.concurrent.TimeUnit timeUnit)
  public final Observable<T> timeout(long timeout,
                                     java.util.concurrent.TimeUnit timeUnit,
                                     ObservableSource<? extends T> other)
复制代码
```
- 设置一个超时的时间间隔, 如果发送事件的间隔超过这个事件就会发出一个异常事件TimeoutException(进入onError).
- 如果你不想发出的是异常而是发送一个新的被观察者事件, 就可以使用该方法
和上面的操作符不同的是, 这个操作符是通过回调函数返回的Observable来控制超时时间; 如果返回的Observable发送了事件但是源被观察者还未发送事件, 将判断为超时; 进入onError抛出TimeOutException
```java
public final <V> Observable<T> timeout(Function<? super T,? extends ObservableSource<V>> itemTimeoutIndicator)
public final <V> Observable<T> timeout(Function<? super T,? extends ObservableSource<V>> itemTimeoutIndicator,
                                         ObservableSource<? extends T> other)
复制代码
```
以下重载可以给第一项被观察者发送的事件单独设置一个超时参数(Observable)
```java
public final <U,V> Observable<T> timeout(ObservableSource<U> firstTimeoutIndicator,
                                         Function<? super T,? extends ObservableSource<V>> itemTimeoutIndicator)
public final <U,V> Observable<T> timeout(ObservableSource<U> firstTimeoutIndicator,
                                           Function<? super T,? extends ObservableSource<V>> itemTimeoutIndicator,
                                           ObservableSource<? extends T> other)
复制代码
```
### 跳过事件
- 数量
- 指定时间
- 倒序跳过
- 指定被观察者发送完毕
- 回调函数连续跳过
#### Skip
跳过事件
```java
public final Observable<T> skip(long count)
// 跳过指定数量的事件
public final Observable<T> skip(long time,
                                  java.util.concurrent.TimeUnit unit)
// 跳过指定时间内的事件
复制代码
```
#### SkipLast
从后开始跳过发送事件;
```java
public final Observable<T> skipLast(int count)
public final Observable<T> skipLast(long time,
                                      java.util.concurrent.TimeUnit unit)
public final Flowable<T> skipLast(long time,
                                    java.util.concurrent.TimeUnit unit,
                                    boolean delayError)
复制代码
```
#### SkipUntil
在作为参数的被观察者没有发送完事件之前的所有源被观察者的事件都将被跳过;
```java
public final <U> Observable<T> skipUntil(ObservableSource<U> other)
复制代码
```
示例代码:
```java
Observable.intervalRange(0, 5, 1, 1, TimeUnit.SECONDS)
    // 如果下面的被观察者的事件没有发送完毕(不包括onComplete)源被观察者的事件都将被跳过
    .skipUntil(Observable.just(1)
        .delay(2, TimeUnit.SECONDS)) 
    .subscribe(new Consumer<Long>() {
      @Override
      public void accept(Long aLong) throws Exception {
        Log.d("日志",
            "(MainActivity.java:80) ___ " + "accept() called with: aLong = [" + aLong + "]");
      }
    });
复制代码
```
#### SkipWhile
在回调中判断是否抛弃事件;
和过滤操作符`filter`不同的是skipWhile只能从开头开始连续跳过事件, 即如果第一个事件你没有跳过, 那么该回调函数就不会再次执行, 也就是你之后都无法跳过事件了;
```bash
public final Observable<T> skipWhile(Predicate<? super T> predicate)
复制代码
```
### 停止订阅
- 可以接受的最多事件数量
- 可以接收到事件的时间
- 参数被观察者发送事件将停止源被观察者的订阅状态
- 倒序
- 回调函数判断是否停止
#### Take
阻止发射器继续发射事件, 结束以后会进入onComplete回调
```java
public final Observable<T> take(long count)
    // 控制最多接受到的事件数量
public final Observable<T> take(long time,
                                    java.util.concurrent.TimeUnit unit)
    // 控制只有规定时间内才能接受到事件
复制代码
```
#### TakeUntil
```java
public final <U> Observable<T> takeUntil(ObservableSource<U> other)
// 如果该参数指定的被观察者发送了事件就会结束之前的被观察者的事件发送
public final Observable<T> takeUntil(Predicate<? super T> stopPredicate)
// 通过回调来判断是否结束事件的发送, 返回true结束发射器发射事件
复制代码
```
#### TakeLast
```java
public final Observable<T> takeLast(int count)
  public final Observable<T> takeLast(long count,
                                      long time,
                                      java.util.concurrent.TimeUnit unit)
  public final Observable<T> takeLast(long time,
                                      java.util.concurrent.TimeUnit unit)
  public final Observable<T> takeLast(long time,
                                      java.util.concurrent.TimeUnit unit,
                                      boolean delayError)
复制代码
```
#### TakeWhile
再回调中判断是否结束发射器(同样进入onComplete), 但是和TakeUntil不同的是返回false为结束.
```java
public final Observable<T> takeWhile(Predicate<? super T> predicate)
复制代码
```
#### Join
添加一个被观察者(称为目标被观察者), 该被观察者发送的每个事件都将依次和源被观察者的所有事件结合(在同一个回调函数中传入)
```bash
public final <TRight,TLeftEnd,TRightEnd,R> Observable<R> join(ObservableSource<? extends TRight> other,
                                                                                      Function<? super T,? extends ObservableSource<TLeftEnd>> leftEnd,
                                                                                      Function<? super TRight,? extends ObservableSource<TRightEnd>> rightEnd,
                                                                                      BiFunction<? super T,? super TRight,? extends R> resultSelector)
复制代码
```
示例代码:
```java
Observable.just(1L, 2L, 3L, 4L)
        .join(Observable.just(5L, 6L, 7L, 8L), new Function<Long, ObservableSource<Long>>() {
          /**
           * 接受源被观察者事件
           * @param aLong
           * @return 返回的被观察者发送事件后将终止源被观察者的事件发送
           * @throws Exception
           */
          @Override
          public ObservableSource<Long> apply(Long aLong) throws Exception {
            Log.d("日志",
                "(MainActivity.java:65) ___ " + "源被观察者 aLong = [" + aLong + "]");
            return Observable.interval(3, TimeUnit.SECONDS);
          }
        }, new Function<Long, ObservableSource<Long>>() {
          /**
           * 接受添加的被观察者事件(join Observable)
           * @param aLong
           * @return 返回的被观察者发送事件后就将终止添加的被观察者的
           * @throws Exception
           */
          @Override
          public ObservableSource<Long> apply(Long aLong) throws Exception {
            Log.d("日志",
                "(MainActivity.java:75) ___ " + "被添加的被观察者 aLong = [" + aLong + "]");
            return Observable.interval(3, TimeUnit.SECONDS);
          }
        }, new BiFunction<Long, Long, String>() {
          /**
           * 同时接受被添加的被观察者和源被观察者的事件
           * @param aLong 源被观察者发送的事件
           * @param aLong2 被添加的被观察者发送的事件
           * @return 该返回值最终被观察者接收到
           * @throws Exception
           */
          @Override
          public String apply(Long aLong, Long aLong2) throws Exception {
            Log.d("日志", "(MainActivity.java:89) ___ " + "apply() called with: aLong = [" + aLong
                + "], aLong2 = [" + aLong2 + "]");
            return aLong + "" + aLong2;
          }
        })
        .subscribe(new Consumer<String>() {
          @Override
          public void accept(String s) throws Exception {
            Log.d("日志", "(MainActivity.java:83) ___ " + "accept() called with: s = [" + s + "]");
          }
        });
复制代码
```
#### GroupJoin
和Join类似
```java
public final <TRight,TLeftEnd,TRightEnd,R> Observable<R> groupJoin(ObservableSource<? extends TRight> other,
                                                                                           Function<? super T,? extends ObservableSource<TLeftEnd>> leftEnd,
                                                                                           Function<? super TRight,? extends ObservableSource<TRightEnd>> rightEnd,
                                                                                           BiFunction<? super T,? super Observable<TRight>,? extends R> resultSelector)
复制代码
```
示例:
```java
Observable.just(1L, 2L, 3L, 4L)
        .groupJoin(Observable.just(5L, 6L, 7L, 8L),
            new Function<Long, ObservableSource<Long>>() {
              @Override
              public ObservableSource<Long> apply(Long aLong) throws Exception {
                return null;
              }
            }, new Function<Long, ObservableSource<Long>>() {
              @Override
              public ObservableSource<Long> apply(Long aLong) throws Exception {
                return null;
              }
            }, new BiFunction<Long, Observable<Long>, String>() {
              @Override
              public String apply(Long aLong, Observable<Long> longObservable) throws Exception {
                return null;
              }
            })
        .subscribe(new Consumer<String>() {
          @Override
          public void accept(String s) throws Exception {
            Log.d("日志", "(MainActivity.java:78) ___ " + "accept() called with: s = [" + s + "]");
          }
        });
复制代码
```
#### ToMap
```java
public final <K> Single<java.util.Map<K,T>> toMap(Function<? super T,? extends K> keySelector)
public final <K,V> Single<java.util.Map<K,V>> toMap(Function<? super T,? extends K> keySelector,
                                                        Function<? super T,? extends V> valueSelector)
public final <K,V> Single<java.util.Map<K,V>> toMap(Function<? super T,? extends K> keySelector,
                                                        Function<? super T,? extends V> valueSelector,
                                                        java.util.concurrent.Callable<? extends java.util.Map<K,V>> mapSupplier)
复制代码
```
### 错误处理
#### onErrorResumeNext
如果发生异常将接收到一个回调返回另一个被观察者(执行OnNext不执行onError)
```bash
// 订阅另一个被观察者
public final Observable<T> onErrorResumeNext(ObservableSource<? extends T> next)
复制代码
```
#### onErrorReturn
如果发生异常将接收到一个回调返回事件(执行OnNext不执行onError)
```bash
public final Observable<T> onErrorReturn(Function<? super java.lang.Throwable,? extends T> valueSupplier)
复制代码
```
> 
两个操作符最后都会执行onComplete
#### Retry
当被观察者的发射器发出异常事件(onError)以后会执行**重试**操作
```bash
public final Observable<T> retry()
// 当发生错误将重新发送事件(被观察者的所有事件全部重新发送)
public final Observable<T> retry(long times)
// 重新发送的次数
public final Observable<T> retry(Predicate<? super java.lang.Throwable> predicate)
public final Observable<T> retry(BiPredicate<? super java.lang.Integer,? super java.lang.Throwable> predicate)
// 在回调函数中判断是否重试(和上面的操作符不同的是该回调函数里面会有一个重试次数的参数)
public final Observable<T> retry(long times,
                                                         Predicate<? super java.lang.Throwable> predicate)
// 判断函数 + 重试次数
复制代码
```
#### RetryUntil
该操作符其实Retry已经有相同实现`retry(predicate)`
不同的是返回true表示停止重试
```bash
public final Observable<T> retryUntil(BooleanSupplier stop)
复制代码
```
#### RetryWhen
在回调函数中返回一个被观察者, 该被观察者如果发出错误事件就会导致源被观察者重试. 如果没有发出错误事件就不会触发重试;
```bash
public final Observable<T> retryWhen(Function<? super Observable<java.lang.Throwable>,? extends ObservableSource<?>> handler)
复制代码
```
示例:
该示例不会触发重试;
```bash
Observable.create(
            new ObservableOnSubscribe<Integer>() {
              @Override
              public void subscribe(ObservableEmitter<Integer> emitter) throws Exception {
                emitter.onNext(1);
                emitter.onNext(2);
                emitter.onError(null);
                emitter.onNext(3);
              }
            })
        .retryWhen(new Function<Observable<Throwable>, ObservableSource<?>>() {
            @Override
            public ObservableSource<?> apply(Observable<Throwable> throwableObservable) throws Exception {
                return Observable.just(23);
            }
        })
        .subscribe(
            new DefaultObserver<Integer>() {
              @Override
              public void onNext(Integer integer) {
                System.out.println("integer = [" + integer + "]");
              }
              @Override
              public void onError(Throwable e) {
                System.out.println("Main.onError");
              }
              @Override
              public void onComplete() {
                System.out.println("Main.onComplete");
              }
            });
复制代码
```
## 判断操作
#### Contains
判断发射的事件是否包含指定的事件, 观察者得到一个布尔类型的值
```java
public final Single<java.lang.Boolean> contains(java.lang.Object element)
复制代码
```
#### Any
```bash
public final Single<java.lang.Boolean> any(Predicate<? super T> predicate)
复制代码
```
依次判断每个事件, 如果返回true则马上终止发射器
```java
Observable.just(1, 2, 3, 4).any(new Predicate<Integer>() {
      @Override public boolean test(Integer integer) throws Exception {
        Log.d("日志",
            "(MainActivity.java:27) ___ " + "test() called with: integer = [" + integer + "]");
        if (integer == 3) {
          return true;
        } else {
          return false;
        }
      }
    }).subscribe(new Consumer<Boolean>() {
      @Override public void accept(Boolean aBoolean) throws Exception {
        Log.d("日志",
            "(MainActivity.java:33) ___ " + "accept() called with: aBoolean = [" + aBoolean + "]");
      }
    });
复制代码
```
#### IsEmpty
```java
public final Single<java.lang.Boolean> isEmpty()
复制代码
```
判断是否有发射过事件, 观察者得到个布尔类型的值.
#### DefaultIfEmpty
```bash
public final Observable<T> defaultIfEmpty(T defaultItem)
复制代码
```
如果发射器没有发射任何事件, 就会发射一个指定的默认事件, 例如发射器的事件被拦截
#### SwitchIfEmpty
```bash
public final Observable<T> switchIfEmpty(ObservableSource<? extends T> other)
复制代码
```
如果没有发射事件就用另一个被观察者替代
```bash
Flowable.empty()
        .switchIfEmpty(Flowable.just(3, 4, 5))
        .subscribe(ele -> Log.i("tag", String.valueOf(ele)));
复制代码
```
#### SequenceEqual
```java
public static <T> Single<java.lang.Boolean> sequenceEqual(ObservableSource<? extends T> source1,
                                                          ObservableSource<? extends T> source2,
                                                          BiPredicate<? super T,? super T> isEqual)
// 最后的
public static <T> Single<java.lang.Boolean> sequenceEqual(ObservableSource<? extends T> source1,
                                                              ObservableSource<? extends T> source2)
复制代码
```
会比较两个被观察者是否相同, 然后观察者接受一个布尔类型的值, 发射的事件数量不相同, 类型不相同都会为false. 只要判断到不相同马上就会终止事件的发送.
```java
Observable.sequenceEqual(Observable.intervalRange(0, 3, 0, 1, TimeUnit.SECONDS),
        Observable.just(0l, 1l, 2l), new BiPredicate<Long, Long>() {
          @Override public boolean test(Long aLong, Long aLong2) throws Exception {
            // 在这里判断是否相等
            return false;
          }
        }).subscribe(new Consumer<Boolean>() {
      @Override public void accept(Boolean aBoolean) throws Exception {
        // 最终结果
      }
    });
复制代码
```
## 合并操作
#### Reduce
相加操作符, 每次都可以两个事件一起处理, 然后所有事件都被处理后就会被观察者接受到最终的事件
```java
public final Maybe<T> reduce(BiFunction<T,T,T> reducer)
复制代码
```
> 
第一次处理(apply)会接收到事件1和事件2, 然后第N次就是上次处理的结果(apply的返回值)和事件N
示例:
```java
Observable.just(1, 2, 3, 4).reduce(new BiFunction<Integer, Integer, Integer>() {
      /**
       *  该方法会回调多次直到所有事件都依次相加(或者说操作)以后才会被观察者接收到最终的结果
       * @throws Exception
       */
      @Override public Integer apply(Integer integer, Integer integer2) throws Exception {
        // 三个泛型分别对应 上次运行结果 当前事件 返回值
        return integer + integer2;
      }
    }).subscribe(new Consumer<Integer>() {
      @Override public void accept(Integer integer) throws Exception {
      }
    });
复制代码
```
待定操作符
```bash
public final <R> Single<R> reduce(R seed,
                                  BiFunction<R,? super T,R> reducer)
public final <R> Single<R> reduceWith(java.util.concurrent.Callable<R> seedSupplier,
                                      BiFunction<R,? super T,R> reducer)
复制代码
```
#### Scan
和Reduce类似, 但是每次观察者都能收到经过操作的事件.
```java
public final Observable<T> scan(BiFunction<T,T,T> accumulator)
复制代码
```
> 
第一次观察者就会接受到第一个事件, 后面才是被处理过后的事件.
后面介绍个重载方法, 可以单独传入一个初始化事件和待观察事件组合.
```java
public final <R> Observable<R> scan(R initialValue, // 加入一个初始化的值
                                      BiFunction<R,? super T,R> accumulator)
复制代码
```
这里我将`initialValue`设置为"吴彦祖"
```java
Observable.just(1, 2, 3, 4).scan("吴彦祖", new BiFunction<String, Integer, String>() {
  @Override public String apply(String s, Integer integer) throws Exception {
    Log.d("日志", "(MainActivity.java:19) ___ "
          + "apply() called with: s = ["
          + s
          + "], integer = ["
          + integer
          + "]");
    return s + integer;
  }
}).subscribe(new Consumer<String>() {
  @Override public void accept(String s) throws Exception {
  }
});
复制代码
```
#### ScanWith
```java
public final <R> Observable<R> scanWith(java.util.concurrent.Callable<R> seedSupplier,
                                        BiFunction<R,? super T,R> accumulator)
复制代码
```
#### Collect
可以创建容器来依次操作数据(观察者只会收到一次事件, 也就是容器)
```java
Flowable.just(1, 2, 3)
        .collect(
            new Callable<ArrayList<Integer>>() {//创建收集容器
              @Override
              public ArrayList<Integer> call() throws Exception {
                return new ArrayList<>();
              }
            }, new BiConsumer<ArrayList<Integer>, Integer>() {//创建收集
              @Override
              public void accept(ArrayList<Integer> list, Integer integer)
                  throws Exception {//前者容器，后者数据
                list.add(integer);
              }
            })
        .subscribe(ele -> Log.d("日志", "(MainActivity.java:33) ___ Result = " + String.valueOf(ele)));
复制代码
```
#### Compose
```bash
public final <R> Observable<R> compose(ObservableTransformer<? super T,? extends R> composer)
复制代码
```
## 辅助操作符
### 时间统计
#### TimeStamp
该操作符将事件和发送的时间都封装到一个对象`Timed`中
```bash
public final Observable<Timed<T>> timestamp()
public final Observable<Timed<T>> timestamp(Scheduler scheduler)
public final Observable<Timed<T>> timestamp(java.util.concurrent.TimeUnit unit)
public final Observable<Timed<T>> timestamp(java.util.concurrent.TimeUnit unit,
                                                                    Scheduler scheduler)
复制代码
```
示例:
```bash
Observable.intervalRange(0, 5, 2, 2, TimeUnit.SECONDS)
        .timestamp()
        .subscribe(new Consumer<Timed<Long>>() {
          @Override
          public void accept(Timed<Long> longTimed) throws Exception {
            Log.d("日志",
                "(MainActivity.java:62) ___ " + "accept() called with: longTimed = [" + longTimed
                    + "]");
          }
        });
复制代码
```
结果
```bash
longTimed = [Timed[time=1525735346216, unit=MILLISECONDS, value=2]]
复制代码
```
### 计数器
#### Count
观察者将接收到事件数量, 而无法收到事件本身.
```java
Flowable.just(1,2,3,4,5).count().subscribe(new BiConsumer<Long, Throwable>() {
    @Override public void accept(Long aLong, Throwable throwable) throws Exception {
        Log.d("日志", "(MainActivity.java:18) ___ Result = " + aLong);
    }
});
复制代码
```
#### To
转换操作符
```bash
public final <K,V> Single<java.util.Map<K,V>> toMap(Function<? super T,? extends K> keySelector,
                                                                            Function<? super T,? extends V> valueSelector)
复制代码
```
## BackPressure
意为"**背压**"
了解了Observable和Flowable的区别，我们还不知什么叫做背压，下面我们来简单了解下概念。所谓背压就是**生产者（被观察者）的生产速度大于消费者（观察者）消费速度**从而导致的问题。
举一个简单点的例子，如果被观察者快速发送消息，但是观察者处理消息的很缓慢，如果没有特定的流（Flow）控制，就会导致大量消息积压占用系统资源，最终导致十分缓慢。
> 
同步线程是不可能产生这种问题, 观察者没有处理完事件就不可能再次发送事件.
怎么优化和减少这种情况后面再探讨，不过可以注意到，Flowable创建的时候已经设置了BackpressureStrategy，而且Subscriber使用了request来控制最大的流量。
## 被观察者
### Observable
不支持背压的被观察者, 性能高;
### Flowable
支持背压的被观察者, 性能比Observable低;
### Publisher和ObservableSource
Publisher该类属于Flowable的根接口, ObservableSource属于Observable的根接口;
### Single
该被观察者只能发出一个事件, 重复发送不会受到(因为只能发送一条事件所以不存在背压).
```java
Single.create(new SingleOnSubscribe<Integer>() {
  @Override public void subscribe(SingleEmitter<Integer> emitter) throws Exception {
    emitter.onSuccess(1);
    emitter.onSuccess(2);
  }
}).subscribe(new Consumer<Integer>() {
  @Override public void accept(Integer integer) throws Exception {
    // 只会触发一次事件
    Log.d("日志",
          "(MainActivity.java:34) ___ " + "accept() called with: integer = [" + integer + "]");
  }
});
复制代码
```
DisposableSingleObserver
> 
可以手动断开观察者
```bash
public final void dispose()
    public final boolean isDisposed()
复制代码
```
ResourceSingleObserver
可以添加其他的Disposable, 然后一起取消观察者
```
public final void add(Disposable resource)
复制代码
```
> 
其他被观察者基本都遵守这几个规则有类似名称的观察者类
### Completable
如果你的观察者连onNext事件都不关心，你可以使用Completable，他只有onComplete和onError两个事件：
```bash
Completable.create(new CompletableOnSubscribe() {//被观察者
    @Override
    public void subscribe(CompletableEmitter e) throws Exception {
        e.onComplete();//单一onComplete或者onError
    }
}).subscribe(new CompletableObserver() {//观察者
    @Override
    public void onSubscribe(Disposable d) {
    }
    @Override
    public void onComplete() {
    }
    @Override
    public void onError(Throwable e) {
    }
});
复制代码
```
同样也可以使用Actions来简化Observer：
- completable.subscribe(onComplete)
- completable.subscribe(onComplete,onError)
要转换成其他类型的被观察者，也是可以使用`toFlowable()`、`toObservable()`等方法去转换。
### Maybe
如果你有一个需求是可能发送一个数据或者不会发送任何数据，这时候你就需要Maybe，它类似于Single和Completable的混合体。
Maybe可能会调用以下其中一种情况（也就是所谓的Maybe）：
- onSuccess或者onError
- onComplete或者onError
可以看到onSuccess和onComplete是互斥的存在，例子代码如下：
```java
//被观察者
Maybe<String> maybe = Maybe.create(new MaybeOnSubscribe<String>() {
    @Override
    public void subscribe(MaybeEmitter<String> e) throws Exception {
        e.onSuccess("test");//发送一个数据的情况，或者onError，不需要再调用onComplete(调用了也不会触发onComplete回调方法)
        //e.onComplete();//不需要发送数据的情况，或者onError
    }
});
//订阅观察者
maybe.subscribe(new MaybeObserver<String>() {
    @Override
    public void onSubscribe(Disposable d) {
    }
    @Override
    public void onSuccess(String s) {
        //发送一个数据时，相当于onNext和onComplete，但不会触发另一个方法onComplete
        Log.i("tag", s);
    }
    @Override
    public void onComplete() {
        //无数据发送时候的onComplete事件
        Log.i("tag", "onComplete");
    }
    @Override
    public void onError(Throwable e) {
    }
});
复制代码
```
### Subject
#### AsyncSubject
该被观察者只会发送最后一个事件(同时`onComplete`)
如果出现异常终止任何事件. 要求必须手动执行`onComplete()`才会接收到最后一个事件和`onComplete`
#### BehaviorSubject
该主题只会观察订阅前的最后一个发送事件`onNext()`以及订阅之后发送的所有事件;
#### PublishSubject
只观察订阅后发送的事件;
#### ReplaySubject
该主题无论订阅前还是订阅后发送的事件都将会被观察者接收到;
## 观察者
- 
**Action**:无参数类型
- 
**Consumer**:单一参数类型
- 
**BiConsumer<T1, T2>**:双参数类型
- 
**Consumer<Obejct[]>**:多参数类型
### Callable
## 测试
用于测试RxJava的事件类
### TestObserver
### TestScheduler
对于某些需要经过一定时间的轮循器等事件观察者
```bash
// 时间快速经过多少
public void advanceTimeBy(long delayTime,
                 java.util.concurrent.TimeUnit unit)
// 时间直接到某个点
public void advanceTimeTo(long delayTime,
                 java.util.concurrent.TimeUnit unit)
复制代码
```
### TestSubsriber
```java
@Test public void addition_isCorrect() throws Exception {
        TestSubscriber<String> testSubscriber = new TestSubscriber<>();
        //依次发射A，B，C
        Flowable.just("A", "B", "C").subscribe(testSubscriber);
        //断言值是否不存在
        testSubscriber.assertNever("D");
        //断言值是否相等
        testSubscriber.assertValues("A", "B", "C");
        //断言值的数量是否相等
        testSubscriber.assertValueCount(3);
        //断言是否结束
        testSubscriber.assertTerminated();
    }
复制代码
```
## RxBinding
依赖(根据需要来添加依赖即可)
```
compile 'com.jakewharton.rxbinding2:rxbinding:2.1.1'
compile 'com.jakewharton.rxbinding2:rxbinding-support-v4:2.1.1'
compile 'com.jakewharton.rxbinding2:rxbinding-appcompat-v7:2.1.1'
compile 'com.jakewharton.rxbinding2:rxbinding-design:2.1.1'
compile 'com.jakewharton.rxbinding2:rxbinding-recyclerview-v7:2.1.1'
复制代码
```
使用方式
## AutoDispose
会根据Activity的生命周期, 自动解除RxJava的观察者订阅. 由Uber开源. Rxlifecycler作者推荐使用.
依赖:
```bash
compile 'com.uber.autodispose:autodispose-android:x.y.z'
复制代码
```
使用:
在`OnDestroy`方法以后解除订阅
```java
Observable.interval(1, TimeUnit.SECONDS)
                .as(
                        AutoDispose.autoDisposable(
                                AndroidLifecycleScopeProvider.from(this)))
                .subscribe(
                        new Consumer<Long>() {
                            @Override
                            public void accept(Long aLong) throws Exception {
                                Log.d("日志", "(MainActivity.java:33) ___ Result = " + aLong);
                            }
                        });
复制代码
```
> 
as方法只能在subscribe之前
自定义生命周期
```java
Observable.interval(1, TimeUnit.SECONDS)
                .as(
                        AutoDispose.autoDisposable(
                                AndroidLifecycleScopeProvider.from(this, Lifecycle.Event.ON_STOP)))
                .subscribe(
                        new Consumer<Long>() {
                            @Override
                            public void accept(Long aLong) throws Exception {
                                Log.d("日志", "(MainActivity.java:33) ___ Result = " + aLong);
                            }
                        });
复制代码
```
## RxBus
RxBus是一种实现方式而非单指某库. 本文介绍的RxBus库是GitHub Star最高的. 主要特点是支持标记(tag)功能. 不支持粘性事件(粘性事件的缺点也比较明显不做介绍).
依赖
```bash
compile 'com.hwangjr.rxbus:rxbus:1.0.6'
复制代码
```
订阅事件
```java
@Subscribe(
    thread = EventThread.IMMEDIATE,
    tags = {@Tag}
)
public void heardFromMouseMam(String mouseWar) {
    Timber.e("Just heard from mouse mam: " + mouseWar + " from " + Thread.currentThread());
}
复制代码
```
多个标签接受事件
```java
@Subscribe(
        thread = EventThread.IMMEDIATE,
        tags = {@Tag(Constants.EventType.TAG_STORY)}
)
public void heardFromMouse(String mouseWar) {
    Timber.e("Just heard from mouse: " + mouseWar + " from " + Thread.currentThread());
}
复制代码
```
自动发送事件
```bash
@Produce(
        thread = EventThread.NEW_THREAD,
        tags = {@Tag}
)
public String tell() {
    return "执行Tell方法" + Thread.currentThread();
}
复制代码
```
