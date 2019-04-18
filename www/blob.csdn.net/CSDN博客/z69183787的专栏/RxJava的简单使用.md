# RxJava的简单使用 - z69183787的专栏 - CSDN博客
2018年08月09日 12:08:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：122
[https://blog.csdn.net/Jannolan/article/details/52473874](https://blog.csdn.net/Jannolan/article/details/52473874)
## 为什么要用RxJava
最近发现越来越多的项目开始使用RxJava，那么他到底有什么优势呢。总结来说有两点： 
1.代码简洁、逻辑清晰，避免了处理复杂逻辑时的大量代码嵌套 
2.线程自由控制，能够多次自由地切换线程，满足复杂异步操作的需求
## RxJava的使用
RxJava的主要组成是两个核心类Observables(被观察者、事件源)，Subscribers(观察者),实现完核心类后用subscribe()将两着订阅起来 
要使用RxJava首先引入两个依赖 
compile ‘io.reactivex:rxandroid:1.2.1’ 
compile ‘io.reactivex:rxjava:1.1.6’ 
接下来贴出几个小例子来简单演示RxJava的使用
**最基础的使用**
```
//创建事件源
        Observable<String> myObservable = Observable.create(
                new Observable.OnSubscribe<String>() {
                    @Override
                    public void call(Subscriber<? super String> sub) {
                        sub.onNext("Hello, world!");
                        sub.onCompleted();
                    }
                }
        );
    //创建观察者
        Subscriber<String> mySubscriber = new Subscriber<String>() {
            @Override
            public void onNext(String s) {
                Toast.makeText(SecondActivity.this, s ,Toast.LENGTH_SHORT).show();
            }
            @Override
            public void onCompleted() {
            }
            @Override
            public void onError(Throwable e) {
            }
        };
    //订阅
        myObservable.subscribe(mySubscriber);
```
**简化写法**
```
Observable<String> myObservable = Observable.just("hellow simple");
        Action1<String> onNextAction=new Action1<String>() {
            @Override
            public void call(String s) {
                Toast.makeText(SecondActivity.this, s, Toast.LENGTH_SHORT).show();
                Log.i("tag",s);
            }
        };
        Action1<Throwable> onError=new Action1<Throwable>() {
            @Override
            public void call(Throwable throwable) {
                Toast.makeText(SecondActivity.this, "error", Toast.LENGTH_SHORT).show();
                Log.i("tag","error");
            }
        };
        Action0 onCompleted=new Action0() {
            @Override
            public void call() {
                Toast.makeText(SecondActivity.this, "completed", Toast.LENGTH_SHORT).show();
                Log.i("tag","completed");
            }
        };
        myObservable.subscribe(onNextAction,onError,onCompleted);
```
RxJava还提供了简化写法，Observable.just()可以有多个Object参数，相当于实现了多次onNext(Object o)和一次onCompleted()。
**操作符的使用**
大量的操作符是RxJava实现逻辑清晰的关键，他能将复杂的逻辑操作分散开来，使每一步的结果都清晰可见。 
接下来介绍几个常用的操作符
*map*
对于事件作进一步处理,用于一对一的转化
```
Observable<String> myObservable = Observable.just("hellow simple");
        Observable<String> myObservableMap=myObservable.map(new Func1<String, String>() {
            @Override
            public String call(String s) {
                return s+"-xu";
            }
        });
        Action1<String> onNextAction=new Action1<String>() {
            @Override
            public void call(String s) {
                Toast.makeText(SecondActivity.this, s, Toast.LENGTH_SHORT).show();
                Log.i("tag",s);
            }
        };
        myObservableMap.subscribe(onNextAction);
```
*from*
from标签相当于调用了多次onNext(Object o)一次onCompleted()
```
from标签相当于调用了多次onNext(Object o)一次onCompleted()
        String[] words = {"a", "b", "c"};
        Observable<String> observable = Observable.from(words);
        observable.subscribe(new Action1<String>() {
            @Override
            public void call(String s) {
                Log.i("form", s);
            }
        }, new Action1<Throwable>() {
            @Override
            public void call(Throwable throwable) {
                Log.i("from", "error");
            }
        }, new Action0() {
            @Override
            public void call() {
                Log.i("from","complete");
            }
        });
```
*flatMap*
与map不同的flatMap是用于一对多的转化
```
List<String> list=new ArrayList<>();
        list.add("a");
        list.add("b");
        list.add("c");
        Observable<List<String>> observable=Observable.just(list);
        observable.flatMap(new Func1<List<String>, Observable<String>>() {
            @Override
            public Observable<String> call(List<String> strings) {
                return Observable.from(strings);
            }
        }).subscribe(new Action1<String>() {
            @Override
            public void call(String s) {
                Log.i("tag",s);
            }
        });
```
**线程控制**
几种线程的位置:
- Schedulers.immediate(): 直接在当前线程运行，相当于不指定线程。这是默认的 Scheduler。
- Schedulers.newThread(): 总是启用新线程，并在新线程执行操作。
- Schedulers.io(): I/O 操作（读写文件、读写数据库、网络信息交互等）所使用的 Scheduler。行为模式和 newThread() 差不多，区别在于 io() 的内部实现是是用一个无数量上限的线程池，可以重用空闲的线程，因此多数情况下 io() 比 newThread() 更有效率。不要把计算工作放在 io() 中，可以避免创建不必要的线程。
- Schedulers.computation(): 计算所使用的 Scheduler。这个计算指的是 CPU 密集型计算，即不会被 I/O 等操作限制性能的操作，例如图形的计算。这个 Scheduler 使用的固定的线程池，大小为 CPU 核数。不要把 I/O 操作放在 computation() 中，否则 I/O 操作的等待时间会浪费 CPU。
- AndroidSchedulers.mainThread()，它指定的操作将在 Android 主线程运行
变化线程操纵 
subscribeOn(Scheduler.io());事件产生的线程,作用在Observable.just(1, 2, 3, 4)，即Observable生成时，位置放在哪都可以，但只能调用一次 
observeOn(AndroidSchedulers.mainThread());事件消费的线程,用于Observable生成后的操作，可以多次调用实现线程的多次切换
```
Observable<String> observable=Observable.just("thread");
        observable.subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread()).map(new Func1<String, String>() {
            @Override
            public String call(String s) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.i("thread",Thread.currentThread().getId()+"map1___"+System.currentTimeMillis());
                return s;
            }
        }).observeOn(Schedulers.io()).map(new Func1<String,String>() {
            @Override
            public String call(String s) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.i("thread",Thread.currentThread().getId()+"map2___"+System.currentTimeMillis());
                return s;
            }
        }).map(new Func1<String,String>() {
            @Override
            public String call(String s) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.i("thread",Thread.currentThread().getId()+"map3___"+System.currentTimeMillis());
                return s;
            }
        }).observeOn(AndroidSchedulers.mainThread()).subscribe(new Action1<String>() {
            @Override
            public void call(String s) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.i("thread", Thread.currentThread().getId() + "main___"+System.currentTimeMillis());
            }
        }, new Action1<Throwable>() {
            @Override
            public void call(Throwable throwable) {
                Log.i("thread","wrong0");
            }
        });
```
关于RxJava的功能还有很多我还未使用到，如果想要对RxJava有更多的了解可以参考以下几篇文章 
[给 Android 开发者的 RxJava 详解](http://gank.io/post/560e15be2dca930e00da1083)
[深入浅出RxJava](http://blog.csdn.net/lzyzsd/article/details/41833541/)
[Awesome-RxJava](https://github.com/lzyzsd/awesome-rxjava)
