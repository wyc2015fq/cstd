# RxJava使用（一）基本使用 - z69183787的专栏 - CSDN博客
2018年08月09日 12:14:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：238
[https://blog.csdn.net/tangxl2008008/article/details/51334122](https://blog.csdn.net/tangxl2008008/article/details/51334122)
# 前言
        RxJava及RxAndroid比较详细的介绍可以参考该文档[《给 Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)
# 基本介绍
        ReactiveX 及 RxJava使用大部分来自和参考[《给 Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)和《[RxJava 入门](http://www.imooc.com/article/2298)》；进行整理及增加了部分自己的理解。
## 什么是 ReactiveX
        ReactiveX 是一个专注于异步编程与控制可观察数据（或者事件）流的API。它组合了观察者模式，迭代器模式和函数式编程的优秀思想。
        实时数据处理是一件普通的现象，有一个高效、干净和可扩展的方式来处理这些情景是重要的。使用 Observables 和 Operators 来熟练操作它们。ReactiveX 提供一个可组合又灵活的 API 来创建和处理数据流，同时简化了异步编程带来的一些担忧，如：线程创建和并发问题。
## RxJava 简介
        RxJava是 ReactiveX 在 Java 上的开源的实现。RxJava可以轻松处理不同运行环境下的后台线程或UI线程任务的框架。RxJava 的异步实现，是通过一种扩展的观察者模式来实现的。
        Observable（可观察者，即被观察者） 和 Subscriber（订阅者）是两个主要的类。在 RxJava 上，一个 Observable 是一个发出数据流或者事件的类，Subscriber 是一个对这些发出的 items （数据流或者事件）进行处理（采取行动）的类。
`   Observable` 和`Observer` 通过 `subscribe()` 方法实现订阅关系。一个 Observable 的标准流发出一个或多个item，然后成功完成或者出错。一个 Observable 可以有多个 Subscribers，并且通过 Observable 发出的每一个 item，该 item 将会被发送到 Subscriber.onNext() 方法来进行处理。一旦 Observable 不再发出 items，它将会调用 Subscriber.onCompleted() 方法，或如果有一个出错的话Observable 会调用 Subscriber.onError() 方法。
        onNext()： RxJava的事件回调方法，针对普通事件。
        onCompleted(): 事件队列完结。RxJava 不仅把每个事件单独处理，还会把它们看做一个队列。RxJava 规定，当不会再有新的 onNext() 发出时，需要触发 onCompleted() 方法作为标志。
        onError(): 事件队列异常。在事件处理过程中出异常时，onError() 会被触发，同时队列自动终止，不允许再有事件发出。
        在一个正确运行的事件序列中,onCompleted() 和 onError() 有且只有一个，并且是事件序列中的最后一个。需要注意的是，onCompleted() 和 onError() 二者也是互斥的，即在队列中调用了其中一个，就不应该再调用另一个。
![](https://img-blog.csdn.net/20160506195415729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图1-RxJava 的观察者模式（来自[《给 Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)）
    RxAndroid是RxJava针对android平台增加的移植版本；使用RxAndroid也需要引入RxJava包。
## GitHub地址及AS引入依赖
`GitHub` 链接：
[https://github.com/ReactiveX/RxJava](https://github.com/ReactiveX/RxJava)
[https://github.com/ReactiveX/RxAndroid](https://github.com/ReactiveX/RxAndroid)
引入依赖：
compile'io.reactivex:rxandroid:1.1.0'
compile'io.reactivex:rxjava:1.1.0'
# 基本使用
## 1.   创建Subscriber（Observer）
Subscriber是Observer接口的一个抽象实现；推荐使用Subscriber，实际上RxJava在subscibe过程中，会先将Observer转换为一个Subscriber。        
final Subscriber<String> tempSubsciber = new Subscriber<String>() {
			    @Override
			    public void onCompleted() {
			        Log.d(TAG, "onCompleted()");
			    }
			    @Override
			    public void onError(Throwable e) {
			        Log.d(TAG, "onError() e=" + e);
			    }
			    @Override
			    public void onNext(String s) {
			        Log.d(TAG, "onNext() data=" + s);
			    }
			}
## 2.   创建Observable
Observable<String> tempObservable = Observable.create(new Observable.OnSubscribe<String>() {
			    @Override
			    public void call(Subscriber<? super String> subscriber) {
			        subscriber.onNext("01");
			        subscriber.onNext("02");
			        subscriber.onNext("03");
			        subscriber.onCompleted();
			    }
			});
## 3.   subscribe (订阅)
tempObservable.subscribe(tempSubsciber);
        subscribe()主要做了一下工作：
       1)        调用subscriber.onStart()。
       2)        调用 Observable 中的 OnSubscribe.call(Subscriber) 。处理事件发送的逻辑，在 RxJava 中， Observable并不是在创建的时候就立即开始发送事件，而是在它被订阅的时候，即当 subscribe() 方法执行的时候。
       3)        将传入的 Subscriber 作为 Subscription 返回。这是为了方便 unsubscribe()。
## 4.   其他简洁使用方式
### 1)    快捷创建Observable
        just(T...):将传入的参数依次发送出来。
final Observable observable = Observable.just("info1", "info2", "info3");
        from(T[])/ from(Iterable<? extends T>) : 将传入的数组或 Iterable 拆分成具体对象后，依次发送出来。 
final String[] infos = {"info1", "info2", "info3"};
			final Observable observable = Observable.from(infos);
### 2)   Subscribe不完整定义的回调
final Action0 onCompletedAction = new Action0() {
			    @Override
			    public void call() {
			        //相当于Subscriber的onCompleted（）
			    }
			};
			final Action1<String> onNextAction = new Action1<String>() {
			    @Override
			    public void call(String s) {
			        //相当于Subscriber的onNext(Sring s)
			    }
			};
			final Action1<Throwable> onErrorAction = new Action1<Throwable>() {
			    @Override
			    public void call(Throwable throwable) {
			        //相当于Subscriber的onError(Throwable e)
			    }
			};
			// 自动创建 Subscriber ，并使用 onNextAction 来定义 onNext()
			observable.subscribe(onNextAction);
			// 自动创建 Subscriber ，并使用 onNextAction 和 onErrorAction 来定义 onNext() 和 onError()
			observable.subscribe(onNextAction, onErrorAction);
			// 自动创建 Subscriber ，并使用 onNextAction、 onErrorAction 和 onCompletedAction 来定义 onNext()、 onError() 和 onCompleted()
			observable.subscribe(onNextAction, onErrorAction, onCompletedAction);
         到此，已可以最简单方式使用RxJava。
## 5.   取消订阅-Unsubscribe()
        Observable.subscribe()的返回值是一个 Subscription 对象。Subscription 类只有两个方法，unsubscribe() 和 isUnsubscribed()。
        unsubscribe() 这个方法很重要，因为在subscribe() 之后， Observable 会持有 Subscriber 的引用，这个引用如果不能及时被释放，将有内存泄露的风险。
       为了防止可能的内存泄露，要在不再使用的时候尽快在合适的地方（例如 onPause() onStop() onDestroy等方法中），用Subscription.isUnsubscribed() 检查你的 Subscription 是否是 unsubscribed，若没有，可以通过Subscription.unsubscribe()取消订阅，Subscriber 将不再接收事件。
        如果调用了Subscription.unsubscribe() ，Unsubscribing将会对 items 停止通知给你的 Subscriber，并允许垃圾回收机制释放对象，防止任何 RxJava 造成内存泄露。
       如果你正在处理多个 Observables 和 Subscribers，所有的 Subscription 对象可以添加到 CompositeSubscription，然后可以使用 CompositeSubscription.unsubscribe() 方法在同一时间进行退订(unsubscribed)。
