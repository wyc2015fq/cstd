# 少年，你可能对RxJava的Zip操作有些误会 - weixin_33985507的博客 - CSDN博客
2019年02月18日 21:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
> 
阅读时间 ~ 5min
![699911-3d6ee2fd05f9318c.png](https://upload-images.jianshu.io/upload_images/699911-3d6ee2fd05f9318c.png)
image.png
项目中有这样一个场景，资讯详情页展示需要发送两个网络请求，一个获取资讯信息，一个获取评论信息，只有两部分内容都请求都完成，才能进行页面的展示，这是一个非常常见的业务场景，我们直接使用了RxJava的Zip操作符来实现，伪代码如下：
```
ApiService service; // Retrofit 产生的请求接口
Observable.zip(service.getNewsContent()
  ,service.getNewsComments()
  ,new Func() {})
  .subscribeOn(Schedulers.io())
  .observeOn(AndroidSchedulers.mainThread())
  .subscribe((data) -> {
      //展示资讯和评论
    })
```
然而我今天在调试另外一个问题时，当把网络延迟设置成5s时，明显发现`getNewsContent`和`getNewsComments`请求居然是串行的，这可和我们对Zip的主观印象不符呀～
![699911-76ea96de31997824.png](https://upload-images.jianshu.io/upload_images/699911-76ea96de31997824.png)
image.png
通常来说这种业务场景里面两个网络请求可以并发来达到更快的展示效果，那这里Zip操作符里面的两个请求为什么会串行呢？看了一眼注释恍然大悟，不是Zip有毛病，是之前使用这个操作符号的少年姿势有些不对,不知道聪明又伶俐的你发现问题了没？
是的，就是调度器的问题！我们来看看Zip操作符的代码：
```
public static <T1, T2, R> Observable<R> zip(Observable<? extends T1> o1, Observable<? extends T2> o2, final Func2<? super T1, ? super T2, ? extends R> zipFunction) {
    return just(new Observable<?>[] { o1, o2 }).lift(new OperatorZip<R>(zipFunction));
  }
```
很简单，直接把传入的两个Observable合成一个数组，通过just发出来，然后追加一个lift操作，所以重点就是这个lift操作中的OperatorZip怎么处理了。
如果你研究过lift操作，自然知道lift接受一个Operator接口，通过Operator接口的call方法，产生一个Subscriber去订阅lift左边的Observable，在这个例子里面就是`just(new Observable<?>[] { o1, o2 })`。
```
public Subscriber<? super Observable[]> call(final Subscriber<? super R> child) {
    //...
    final ZipSubscriber subscriber = new ZipSubscriber(child, zipper, producer);
    return subscriber;
  }
```
代码也很简单，直接new了一个ZipSubscriber：
```
private final class ZipSubscriber extends Subscriber<Observable[]> {
    final Zip<R> zipper;
    @Override
    public void onNext(Observable[] observables) {
            zipper.start(observables, producer);
    }
}
```
到此，你可以再捋一下了，前面zip传入的两个网络请求，getNewsContent和getNewsComments被just打包成一个新流，被ZipSubscriber接收，那么在它的onNext中它通过 zipper.start(observables, producer);来执行两个原始网络请求的，看它是怎么执行的吧：
```
public void start(Observable[] os, AtomicLong requested) {
        for (int i = 0; i < os.length; i++) {
            os[i].unsafeSubscribe((InnerSubscriber) subscribers[i]);
        }
    }
```
是的，你没看错，直接在一个for里面，通过unsafeSubscribe订阅来触发网络请求了，所以就会有上面的结论，两个网络请求串行了，要解决这个问题也很简单，你只需要给每个流追加一个调度器，让它们自己并发就可以啦～
```
ApiService service; // Retrofit 产生的请求接口
Observable.zip(service.getNewsContent().subscribeOn(Schedulers.io())
  ,service.getNewsComments().subscribeOn(Schedulers.io())
  ,new Func() {})
  .observeOn(AndroidSchedulers.mainThread())
  .subscribe((data) -> {
      //展示资讯和评论
    })
```
少年，这个误会解释清楚了吗？
