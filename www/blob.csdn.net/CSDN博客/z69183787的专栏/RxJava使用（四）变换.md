# RxJava使用（四）变换 - z69183787的专栏 - CSDN博客
2018年08月09日 17:08:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：43
个人分类：[Java-RxJava异步框架](https://blog.csdn.net/z69183787/article/category/7921321)
[https://blog.csdn.net/tangxl2008008/article/details/51334438](https://blog.csdn.net/tangxl2008008/article/details/51334438)
 RxJava 提供了对事件序列进行变换的支持；**所谓变换，就是将事件序列中的对象或整个序列进行加工处理，转换成不同的事件或事件序列。**
**       不仅可以针对事件对象，还可以针对整个事件队列。**
变换部分主要来自[《给Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)
## 1.   **事件对象变换-map()**
`  map()`: 事件对象的直接变换；它是 RxJava 最常用的变换；可以将Observable深入的对象1转换为对象2发送给Subscriber。
       基本用法如下：
Observable.just(R.drawable.t)
			        .map(new Func1<Integer, Drawable>() {
			            @Override
			            public Drawable call(Integer integer) {
			                return getResources().getDrawable(integer);
			            }
			        })
			        .subscribe(new Action1<Drawable>() {
			            @Override
			            public void call(Drawable drawable) {
			                showImg(drawable);
			            }
			        });
## **2.    事件序列变换-flatMap()**
**flatMap() 也和 map() 相同，也是把传入的参数转化之后返回另一个对象；和 map() 不同的是， flatMap() 中返回的是个 Observable 对象，并且这个 Observable 对象并不是被直接发送到了 Subscriber 的回调方法中。**
**         flatMap() 的原理：**
**       1. 使用传入的事件对象创建一个 Observable 对象；**
**       2. 并不发送这个 Observable, 而是将它激活，于是它开始发送事件；**
**       3. 每一个创建出来的 Observable 发送的事件，都被汇入同一个 Observable ，而这个 Observable 负责将这些事件统一交给 Subscriber 的回调方法。**
**       这三个步骤，把事件拆成了两级，通过一组新创建的 Observable 将初始的对象『铺平』之后通过统一路径分发了下去。**
         //
         扩展：由于可以在嵌套的Observable 中添加异步代码， flatMap() 也常用于嵌套的异步操作，例如嵌套的网络请求。
         flatmap()基本用法如下，定义了一个Book类型，包含名称和章节列表；通过flatmap()打印出全部的章节列表。
       Ø  Book.java
public class Book {
			    public String name;
			    public List<String> chapterList = new ArrayList<String>();
			    public void addChapter(String chapter) {
			        chapterList.add(chapter);
			    }
			}
       Ø  flagmap()使用
Book[] books = getBookList(5);
			Observable.from(books)
			        .flatMap(new Func1<Book, Observable<String>>() {
			            @Override
			            public Observable<String> call(Book book) {
			                return Observable.from(book.chapterList);
			            }
			        })
			        .subscribe(new Action1<String>() {
			            @Override
			            public void call(String s) {
			                Log.v(TAG, s);
			            }
			        });
### 3.     变换的原理：lift()
         所有的变换功能可能有所不同，实质上都是针对事件序列的处理和再发送。而在 RxJava 的内部，它们是基于同一个基础的变换方法： lift(Operator)。
        lift()实现的源码：
/**
			 * Lifts a function to the current Observable and returns a new Observable that when subscribed to will pass
			 * the values of the current Observable through the Operator function.
			 * <p>
			 * In other words, this allows chaining Observers together on an Observable for acting on the values within
			 * the Observable.
			 * <p> {@code
			 * observable.map(...).filter(...).take(5).lift(new OperatorA()).lift(new OperatorB(...)).subscribe()
			 * }
			 * <p>
			 * If the operator you are creating is designed to act on the individual items emitted by a source
			 * Observable, use {@code lift}. If your operator is designed to transform the source Observable as a whole
			 * (for instance, by applying a particular set of existing RxJava operators to it) use {@link #compose}.
			 * <dl>
			 *  <dt><b>Scheduler:</b></dt>
			 *  <dd>{@code lift} does not operate by default on a particular {@link Scheduler}.</dd>
			 * </dl>
			 * 
			 * @param operator the Operator that implements the Observable-operating function to be applied to the source
			 *             Observable
			 * @return an Observable that is the result of applying the lifted Operator to the source Observable
			 * @see <a href="https://github.com/ReactiveX/RxJava/wiki/Implementing-Your-Own-Operators">RxJava wiki: Implementing Your Own Operators</a>
			 */
			  public final <R> Observable<R> lift(final Operator<? extends R, ? super T> operator) {
			    return new Observable<R>(new OnSubscribe<R>() {
			        @Override
			        public void call(Subscriber<? super R> o) {
			            try {
			                Subscriber<? super T> st = hook.onLift(operator).call(o);
			                try {
			                    // new Subscriber created and being subscribed with so 'onStart' it
			                    st.onStart();
			                    onSubscribe.call(st);
			                } catch (Throwable e) {
			                    // localized capture of errors rather than it skipping all operators 
			                    // and ending up in the try/catch of the subscribe method which then
			                    // prevents onErrorResumeNext and other similar approaches to error handling
			                    Exceptions.throwIfFatal(e);
			                    st.onError(e);
			                }
			            } catch (Throwable e) {
			                Exceptions.throwIfFatal(e);
			                // if the lift function failed all we can do is pass the error to the final Subscriber
			                // as we don't have the operator available to us
			                o.onError(e);
			            }
			        }
			    });
			  }
         通过lift()源码可以发现，生成了一个新的Observable（new）；并且在新的Observable（new）的OnSubscribe（new）的call()回调方法中，创建了一个新的Subscriber（new），该新的Subscriber（最终目标）使用了最终调用的Subscriber的代理；再使用原来的Observable（old）的onSubscribe（old）来调用新的Subscriber（new）。
![](https://img-blog.csdn.net/20160506204742424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图示lift()变换过程-（图片来自[《给Android 开发者的 RxJava 详解》](http://gank.io/post/560e15be2dca930e00da1083)）        
`lift()` 实现过程
       1)  lift() 创建一个新Observable ，此时加上之前的原始 Observable，已经有两个 Observable 了；
       2) 而同样地，新 Observable 里的新 OnSubscribe 加上之前的原始 Observable 中的原始 OnSubscribe，也就有了两个 OnSubscribe；
       3)  当用户调用经过 lift() 后的 Observable 的 subscribe() 的时候，使用的是 lift() 所返回的新的 Observable ，于是它所触发的 onSubscribe.call(subscriber)，也是用的新 Observable 中的新OnSubscribe，即在 lift() 中生成的那个 OnSubscribe；
        4) 而这个新 OnSubscribe 的 call() 方法中的 onSubscribe ，就是指的原始 Observable 中的原始 OnSubscribe ，在这个 call() 方法里，新 OnSubscribe 利用 operator.call(subscriber) 生成了一个新的 Subscriber（Operator 就是在这里，通过自己的call() 方法将新 Subscriber 和原始 Subscriber 进行关联，并插入自己的『变换』代码以实现变换），然后利用这个新 Subscriber 向原始 Observable 进行订阅。
       lift() 过程，有点像一种代理机制，通过事件拦截和处理实现事件序列的变换。
       在 Observable 执行了lift(Operator) 方法之后，会返回一个新的 Observable，这个新的 Observable 会像一个代理一样，负责接收原始的 Observable 发出的事件，并在处理后发送给 Subscriber。
4.   自定义Observable.Operator，直接调用lift()
         下面实例实现的功能跟前面**flatMap()**实现的功能一致：
Observable.from(getBookList(4))
			        .lift(new Observable.Operator<String, Book>() {
			            @Override
			            public Subscriber<? super Book> call(final Subscriber<? super String> subscriber) {
			                return new Subscriber<Book>() {
			                    @Override
			                    public void onCompleted() {
			                        subscriber.onCompleted();
			                    }
			                    @Override
			                    public void onError(Throwable e) {
			                        subscriber.onError(e);
			                    }
			                    @Override
			                    public void onNext(Book book) {
			                        for(String chapter : book.chapterList) {
			                            subscriber.onNext(chapter);
			                        }
			                    }
			                };
			            }
			        })
			        .subscribe(new Action1<String>() {
			            @Override
			            public void call(String s) {
			                Log.v(TAG, s);
			            }
			        });
5.   compose(Transformer)变换
        compose(Transformer)变换，针对Observable 自身进行变换。假设在程序中有多个 Observable ，并且他们都需要应用一组相同的 lift() 变换，可以使用compose方式处理，而不是每个都调用一次相同的变换过程
        实例代码，如需要将Book转换得到对应的全部章节，两个Observable对章节数据有不同的处理方式：
final Observable.Transformer bookTtransformer = new Observable.Transformer<Book, String>() {
			    @Override
			    public Observable<String> call(Observable<Book> bookObservable) {
			        return bookObservable.flatMap(new Func1<Book, Observable<String>>() {
			            @Override
			            public Observable<String> call(Book book) {
			                return Observable.from(book.chapterList);
			            }
			        });
			    }
			};
			Observable.from(getBookList(4))
			      .compose(bookTtransformer)
			      .subscribe(new Action1<String>() {
			          @Override
			          public void call(String s) {
			              //处理方式1
			              Log.v(TAG, s);
			          }
			      });
			////
			Observable.from(getBookList(2))
			      .compose(bookTtransformer)
			      .subscribe(new Action1<String>() {
			          @Override
			          public void call(String s) {
			              //处理方式2
			              Log.v(TAG, s);
			          }
			      });
