# RxJava使用（三）Scheduler 线程控制 - z69183787的专栏 - CSDN博客
2018年08月09日 15:54:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：140
[https://blog.csdn.net/tangxl2008008/article/details/51334371](https://blog.csdn.net/tangxl2008008/article/details/51334371)
# 前言
        在默认情况下，即在不指定线程的情况下，RxJava 遵循的是线程不变的原则，即：在哪个线程调用 subscribe()，就在哪个线程生产事件；在哪个线程生产事件，就在哪个线程消费事件。
       如果需要切换线程，就需要用到 Scheduler （调度器）。
       Schedulers部分主要来自[《给Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)
# Schedulers介绍
        在RxJava 中，Scheduler ——调度器，相当于线程控制器，RxJava 通过它来指定每一段代码应该运行在什么样的线程。RxJava 已经内置了几个 Scheduler ，它们已经适合大多数的使用场景：
       Schedulers.immediate(): 直接在当前线程运行，相当于不指定线程。这是默认的 Scheduler。
       Schedulers.newThread(): 总是启用新线程，并在新线程执行操作。
       Schedulers.io(): I/O 操作（读写文件、读写数据库、网络信息交互等）所使用的 Scheduler。行为模式和 newThread() 差不多，区别在于 io() 的内部实现是是用一个无数量上限的线程池，可以重用空闲的线程，因此多数情况下 io() 比 newThread() 更有效率。不要把计算工作放在 io() 中，可以避免创建不必要的线程。
       Schedulers.computation(): 计算所使用的 Scheduler。这个计算指的是 CPU 密集型计算，即不会被I/O 等操作限制性能的操作，例如图形的计算。这个 Scheduler 使用的固定的线程池，大小为 CPU 核数。不要把 I/O 操作放在 computation() 中，否则 I/O 操作的等待时间会浪费 CPU。
       AndroidSchedulers.mainThread()，是RxAndroid 中一个对 RxJava 的轻量级扩展为了Android 的主线程提供 Scheduler，它指定的操作将在 Android 主线程运行。   
# 线程控制
       利用observeOn() 和 subscribeOn() 方法来进行线程控制，
        subscribeOn()：指定 subscribe() 所发生的线程，即 Observable.OnSubscribe 被激活时所处的线程。或者叫做事件产生的线程。
`observeOn()`：指定 Subscriber 所运行在的线程。或者叫做事件消费的线程。
`observeOn()` 指定的是 `Subscriber` 的线程，而这个 `Subscriber` 并不一定是`subscribe()` 参数中的 `Subscriber`（这块参考RxJava变换部分），而是 `observeOn()` 执行时的当前 `Observable` 所对应的 `Subscriber` ，即它的直接下级 `Subscriber` 。
       换句话说，`observeOn()` 指定的是它之后的操作所在的线程。因此如果有多次切换线程的需求，只要在每个想要切换线程的位置调用一次 `observeOn()` 即可。
` `
       下面的实例，在Observable.OnSubscribe的call()中模拟了长时间获取数据过程，在Subscriber的noNext()中显示数据到UI。
Observable.create(new Observable.OnSubscribe<String>() {
			     @Override
			     public void call(Subscriber<? super String> subscriber) {
			         subscriber.onNext("info1");
			         SystemClock.sleep(2000);
			         subscriber.onNext("info2-sleep 2s");
			         SystemClock.sleep(3000);
			         subscriber.onNext("info2-sleep 3s");
			         SystemClock.sleep(5000);
			         subscriber.onCompleted();
			     }
			 })
			.subscribeOn(Schedulers.io()) //指定 subscribe() 发生在 IO 线程
			.observeOn(AndroidSchedulers.mainThread()) //指定 Subscriber 的回调发生在主线程
			.subscribe(new Subscriber<String>() {
			    @Override
			    public void onCompleted() {
			        Log.v(TAG, "onCompleted()");
			    }
			    @Override
			    public void onError(Throwable e) {
			        Log.v(TAG, "onError() e=" + e);
			    }
			    @Override
			    public void onNext(String s) {
			        showInfo(s); //UI view显示数据
			    }
			});
       说明：subscribeOn () 与 observeOn()都会返回了一个新的Observable，因此若不是采用上面这种直接流方式，而是分步调用方式，需要将新返回的Observable赋给原来的Observable，否则线程调度将不会起作用。 
       1）使用下面方式，最后发现“OnSubscribe”还是在默认线程中运行；原因是subscribeOn这类操作后，返回的是一个新的Observable。
observable.subscribeOn(Schedulers.io());
			observable.observeOn(AndroidSchedulers.mainThread());
			observable .subscribe(subscribe);
       2）可以修改为下面两种方式：
observable = observable.subscribeOn(Schedulers.io());
			observable = observable.observeOn(AndroidSchedulers.mainThread());
			observable .subscribe(subscribe);
			////
			observable.subscribeOn(Schedulers.io())
			.observeOn(AndroidSchedulers.mainThread())
			.subscribe(subscribe);
