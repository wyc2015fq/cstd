# RxJava学习笔记一 - weixin_33985507的博客 - CSDN博客
2018年12月26日 13:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：27
本学习笔记来自本人学习总结得出，学习参考内容大部分来自 [Carson_Ho](https://www.jianshu.com/users/383970bef0a0/latest_articles)的系列文章，仅做回忆使用。
#### 环境
![9009142-161f2315c4fcf901.png](https://upload-images.jianshu.io/upload_images/9009142-161f2315c4fcf901.png)
image.png
classpath 'com.android.tools.build:gradle:3.1.4'
distributionUrl=[https://services.gradle.org/distributions/gradle-4.4-all.zip](https://services.gradle.org/distributions/gradle-4.4-all.zip)
rxandroid2.1.0.jar
rxjava-2.2.3.jar
#### 基础用法
![9009142-5d04c9d0ad0efefd.png](https://upload-images.jianshu.io/upload_images/9009142-5d04c9d0ad0efefd.png)
image.png
**测试数据初始化：**
```
private void initData() {
        for (int i = 0; i < 15; i++) {
            TwiceBean bean2 = new TwiceBean(i);
            list.add(bean2);
        }
    }
```
**共同点→：**
这些东西好像都有delay延迟一定时间的方法
**create():**
```
private void RxTaskByCreate() {
        Observable.create(new ObservableOnSubscribe<TwiceBean>() {
            @Override
            public void subscribe(ObservableEmitter<TwiceBean> emitter) throws Exception {
                emitter.onNext(list.get(0));
                emitter.onNext(list.get(1));
                emitter.onNext(list.get(2));
                emitter.onComplete();
            }
        }).subscribe(new Observer<TwiceBean>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(TwiceBean twiceBean) {
            Log.i("mtg",twiceBean.getAge()+":onNext:"+Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 10:03:49.199 17624-17624/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 10:03:49.199 17624-17624/com.biabia.club.testrxandroid I/mtg: 0:onNext:main
2018-11-16 10:03:49.199 17624-17624/com.biabia.club.testrxandroid I/mtg: 1:onNext:main
2018-11-16 10:03:49.199 17624-17624/com.biabia.club.testrxandroid I/mtg: 2:onNext:main
2018-11-16 10:03:49.199 17624-17624/com.biabia.club.testrxandroid I/mtg: onComplete:main
```
**just():**
作用
1.快速创建1个被观察者对象（Observable）
2.发送事件的特点：直接发送 传入的事件
注：最多只能发送10个事件
```
private void RxTaskByJust(){
        Observable.just(list.get(0),list.get(1)).subscribe(new Observer<TwiceBean>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(TwiceBean twiceBean) {
              Log.i("mtg",twiceBean.getAge()+":onNext:"+Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 10:34:41.807 20564-20564/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 10:34:41.807 20564-20564/com.biabia.club.testrxandroid I/mtg: 0:onNext:main
2018-11-16 10:34:41.808 20564-20564/com.biabia.club.testrxandroid I/mtg: 1:onNext:main
2018-11-16 10:34:41.808 20564-20564/com.biabia.club.testrxandroid I/mtg: onComplete:main
```
**fromArray():**
作用:
快速创建1个被观察者对象（Observable）
发送事件的特点：直接发送 传入的整个数组数据
会将数组中的数据转换为Observable对象
注：可发送10个以上事件，可以看成对just的补充
```
private void RxTaskByFromArray(){
        Observable.fromArray(list).subscribe(new Observer<ArrayList<TwiceBean>>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(ArrayList<TwiceBean> twiceBeans) {
                for (int i = 0; i < twiceBeans.size(); i++) {
                    Log.i("mtg","  \t"+twiceBeans.get(i)+" onNext:"+Thread.currentThread().getName());
                }
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 10:38:57.937 21064-21064/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 10:38:57.937 21064-21064/com.biabia.club.testrxandroid I/mtg:    com.biabia.club.testrxandroid.bean.TwiceBean@4b2f595 onNext:main
2018-11-16 10:38:57.937 21064-21064/com.biabia.club.testrxandroid I/mtg:    com.biabia.club.testrxandroid.bean.TwiceBean@d4426aa onNext:main
2018-11-16 10:38:57.937 21064-21064/com.biabia.club.testrxandroid I/mtg:    com.biabia.club.testrxandroid.bean.TwiceBean@b48729b onNext:main
2018-11-16 10:38:57.937 21064-21064/com.biabia.club.testrxandroid I/mtg:    com.biabia.club.testrxandroid.bean.TwiceBean@fa63638 onNext:main...
onComplete:main
```
**fromIterable():**
作用：
快速创建1个被观察者对象（Observable）
发送事件的特点：直接发送 传入的集合List元素数据
会将数组中的元素数据转换为Observable对象
```
private void RxTaskByFromIterval(){
        Observable.fromIterable(list).subscribe(new Observer<TwiceBean>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(TwiceBean twiceBean) {
                Log.i("mtg",twiceBean.getAge()+" onNext:"+Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 10:52:41.634 22351-22351/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 10:52:41.634 22351-22351/com.biabia.club.testrxandroid I/mtg: 0 onNext:main
2018-11-16 10:52:41.634 22351-22351/com.biabia.club.testrxandroid I/mtg: 1 onNext:main
2018-11-16 10:52:41.634 22351-22351/com.biabia.club.testrxandroid I/mtg: 2 onNext:main...
2018-11-16 10:52:41.634 22351-22351/com.biabia.club.testrxandroid I/mtg: onComplete:main
```
**never**
被观察者对象什么都不会发送，观察者也什么都不接收，暂未知使用场景
**empty()**
注：该方法创建的被观察者对象仅发送Error事件,观察者接收后也只会直接调用onComplete（）
**error()**
注：该方法创建的被观察者对象仅发送Error事件,观察者接收后也只会直接调用onError（）
```
private void RxTaskByFromError(){
        Observable.error(new RuntimeException("有错误啦")).subscribe(new Observer<Object>() {
            @Override
            public void onSubscribe(Disposable disposable) {
            }
            @Override
            public void onNext(Object o) {
                Log.i("mtg","onNext:"+o.toString());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg","onError:"+throwable);
            }
            @Override
            public void onComplete() {
            }
        });
    }
```
```
2018-11-16 11:23:10.525 26052-26052/com.biabia.club.testrxandroid I/mtg: onError:java.lang.RuntimeException: 有错误啦
```
**defer()**
注：当有监察者订阅时再进行被监察者的创建,有点像懒汉模式
场景可用于变量传递过程中不停更改
```
private void RxTaskByDefer(){
        Observable<TwiceBean> defer = Observable.defer(new Callable<ObservableSource<TwiceBean>>() {
            @Override
            public ObservableSource<TwiceBean> call() throws Exception {
                //return的被监察者创建
                return Observable.create(new ObservableOnSubscribe<TwiceBean>() {
                    @Override
                    public void subscribe(ObservableEmitter<TwiceBean> observableEmitter) throws Exception {
                        //发射事件
                    }
                });
            }
        });
        defer.subscribe(new Observer<TwiceBean>() {
            @Override
            public void onSubscribe(Disposable disposable) {
               
            }
            @Override
            public void onNext(TwiceBean twiceBean) {
                
            }
            @Override
            public void onError(Throwable throwable) {
                
            }
            @Override
            public void onComplete() {
                
            }
        });
    }
```
**timer():**
延时操作，类型好像只能为Long
```
private void RxTaskByTimer(){
        Observable.timer(3,TimeUnit.SECONDS).subscribe(new Observer<Long>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(Long aLong) {
                Log.i("mtg",aLong+" onNext:"+Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
先调用了onSubscribe()，延时后分别顺序调用onNext()与onCpmplete()，每次都使用一个新线程
```
2018-11-16 14:34:20.665 25105-25105/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 14:34:23.667 25105-28494/com.biabia.club.testrxandroid I/mtg: 0 onNext:RxComputationThreadPool-2
2018-11-16 14:34:23.667 25105-28494/com.biabia.club.testrxandroid I/mtg: onComplete:RxComputationThreadPool-2
2018-11-16 14:34:26.281 25105-25105/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 14:34:29.282 25105-28498/com.biabia.club.testrxandroid I/mtg: 0 onNext:RxComputationThreadPool-3
2018-11-16 14:34:29.282 25105-28498/com.biabia.club.testrxandroid I/mtg: onComplete:RxComputationThreadPool-3
2018-11-16 14:34:33.312 25105-25105/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 14:34:36.313 25105-28501/com.biabia.club.testrxandroid I/mtg: 0 onNext:RxComputationThreadPool-4
2018-11-16 14:34:36.314 25105-28501/com.biabia.club.testrxandroid I/mtg: onComplete:RxComputationThreadPool-4
```
**interval():**
作用：
1.快速创建1个被观察者对象（Observable）
2.发送事件的特点：每隔指定时间 就发送 事件
3.发送的事件序列 = 从0开始、无限递增1的的整数序列
注：Observer好像只能接收Long类型，多个Observable可以共用一个Observer但好像无法区分,都位于子线程
```
private void RxTaskByInterval() {
        Observer<Long> beanObserver = new Observer<Long>() {
            @Override
            public void onSubscribe(Disposable d) {
                Log.i("mtg",d.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(Long l) {
                Log.i("mtg",l+":onNext:"+Thread.currentThread().getName());
                onComplete();
            }
            @Override
            public void onError(Throwable e) {
                Log.i("mtg",e+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        };
        Observable<Long> observable = Observable.interval(1, TimeUnit.SECONDS)
                .observeOn(AndroidSchedulers.mainThread()).retry(5000);
        Observable<Long> observable2 = Observable.interval(1, TimeUnit.SECONDS);
        observable.subscribe(beanObserver);
        observable2.subscribe(beanObserver);
    }
```
结果：
```
2018-11-15 16:33:38.777 21897-21972/com.biabia.club.testrxandroid I/mtg: 1447:onNext:RxComputationThreadPool-1
2018-11-15 16:33:38.778 21897-21973/com.biabia.club.testrxandroid I/mtg: 1447:onNext:RxComputationThreadPool-2
2018-11-15 16:33:39.778 21897-21972/com.biabia.club.testrxandroid I/mtg: 1448:onNext:RxComputationThreadPool-1
2018-11-15 16:33:39.778 21897-21973/com.biabia.club.testrxandroid I/mtg: 1448:onNext:RxComputationThreadPool-2
```
**IntervalRange():**
作用：
1.有限次数倒计时
2.Long类型
```
private void RxTaskByIntervalRange(){
        /* 参数1 = 事件序列起始点；
         参数2 = 事件数量；
         参数3 = 第1次事件延迟发送时间；
         参数4 = 间隔时间数字；
         参数5 = 时间单位*/
        //暂时 无法理解
        Observable.intervalRange(3,10,4, 1,TimeUnit.SECONDS)
                .subscribe(new Observer<Long>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg",disposable.isDisposed()+" onSubscribe:"+Thread.currentThread().getName());
            }
            @Override
            public void onNext(Long aLong) {
                Log.i("mtg",aLong+" onNext:"+Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg",throwable+":onError:"+Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg","onComplete:"+Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 16:51:20.431 17597-17597/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 16:51:24.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 3 onNext:RxComputationThreadPool-1
2018-11-16 16:51:25.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 4 onNext:RxComputationThreadPool-1
2018-11-16 16:51:26.433 17597-17656/com.biabia.club.testrxandroid I/mtg: 5 onNext:RxComputationThreadPool-1
2018-11-16 16:51:27.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 6 onNext:RxComputationThreadPool-1
2018-11-16 16:51:28.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 7 onNext:RxComputationThreadPool-1
2018-11-16 16:51:29.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 8 onNext:RxComputationThreadPool-1
2018-11-16 16:51:30.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 9 onNext:RxComputationThreadPool-1
2018-11-16 16:51:31.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 10 onNext:RxComputationThreadPool-1
2018-11-16 16:51:32.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 11 onNext:RxComputationThreadPool-1
2018-11-16 16:51:33.432 17597-17656/com.biabia.club.testrxandroid I/mtg: 12 onNext:RxComputationThreadPool-1
2018-11-16 16:51:33.433 17597-17656/com.biabia.club.testrxandroid I/mtg: onComplete:RxComputationThreadPool-1
```
**Range()**
作用:
1.立即倒数发送有限次数无延时事件
2.从0以上开始
3.只能Integer类型
```
private void RxRange() {
        Observable.range(3, 10).subscribe(new Observer<Integer>() {
            @Override
            public void onSubscribe(Disposable disposable) {
                Log.i("mtg", disposable.isDisposed() + " onSubscribe:" + Thread.currentThread().getName());
            }
            @Override
            public void onNext(Integer integer) {
                Log.i("mtg", integer + " onNext:" + Thread.currentThread().getName());
            }
            @Override
            public void onError(Throwable throwable) {
                Log.i("mtg", throwable + ":onError:" + Thread.currentThread().getName());
            }
            @Override
            public void onComplete() {
                Log.i("mtg", "onComplete:" + Thread.currentThread().getName());
            }
        });
    }
```
结果：
```
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: false onSubscribe:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 3 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 4 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 5 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 6 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 7 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 8 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 9 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 10 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 11 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: 12 onNext:main
2018-11-16 17:00:40.483 18077-18077/com.biabia.club.testrxandroid I/mtg: onComplete:main
```
**RangeLong()**
与Range相同只不过只能是Long类型
**创建效用总结：**
![9009142-110de141e31f7cba.png](https://upload-images.jianshu.io/upload_images/9009142-110de141e31f7cba.png)
image.png
##### 疑问：
1.RxJava能不能跨进程通信
2.RxJava线程最大能开多少是否有线程池管理
3.RxJava调用的几个方法是否运行在主线程
4.RxJava subscribe、onSubscribe、onNext、onError、onComplete各有什么特性特征，用法，含义
5.RxJava多个发送，其发送顺序与接收顺序是怎样的
先入先出，队列顺序
6.网络发送失败，则RxJava怎么处理
答：检测是否是IO异常，是IO异常则重新发送网络请求
Composable（可组装）
Lazy（延迟执行）
Reusable（可重用）
Asynchronous（异步）
Cacheable（可缓存）
Push or Pull（推还是拉）
Backpressure（反压）
Operator fusion（操作融合）
[https://yq.aliyun.com/articles/591627](https://yq.aliyun.com/articles/591627)
流控几个措施：
1.背压
2.节流
3.打包
4.同步阻塞
参考文章：[https://www.zhihu.com/question/49618581/answer/117107570](https://www.zhihu.com/question/49618581/answer/117107570)
###### 拓展：
《探索专为 Android而设计的 RxJava 2》
[https://www.jianshu.com/p/4b443f815111?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation](https://www.jianshu.com/p/4b443f815111?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation)
