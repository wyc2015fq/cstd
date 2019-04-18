# Java里面CompletableFuture详解 - z69183787的专栏 - CSDN博客
2018年07月19日 19:53:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：125
[https://blog.csdn.net/cainiao_user/article/details/76423495](https://blog.csdn.net/cainiao_user/article/details/76423495)
Future是Java5添加的类，用来描述一个异步计算的结果。可以用isDone方法来检查计算是否完成，或者使用get阻塞住调用线程，直至计算完成返回结果，也可以用cancel方法来停止任务的执行。
- 
`public class BasicFuture {`
- 
`public static void main(String[] args) throws ExecutionException, InterruptedException {`
- 
`ExecutorService es = Executors.newFixedThreadPool(10);`
- 
`Future<Integer> f = es.submit(() -> {`
- 
`// 长时间的异步计算`
- 
`// ...`
- 
`// 然后返回结果`
- 
`return 100;`
- 
`});`
- 
`f.get();`
- 
`}`
- 
`}`
Future以及相关使用方法提供了异步执行任务的能力，但对于结果的获取却是不方便，只能通过阻塞或轮询的方式得到任务结果。阻塞的方式与我们理解的异步编程其实是相违背的，而轮询又会耗无谓的CPU资源。而且还不能及时得到计算结果，为什么不能用观察者设计模式当计算结果完成及时通知监听者呢？
很多语言像Node.js，采用回调的方式实现异步编程。Java的一些框架像Netty，自己扩展Java的Future接口，提供了addListener等多个扩展方法：
- 
`ChannelFuture future = bootstrap.connect(new InetSocketAddress(host, port));`
- 
`future.addListener(new ChannelFutureListener()`
- 
`{`
- 
`@Override`
- 
`public void operationComplete(ChannelFuture future) throws Exception`
- 
`{`
- 
`if (future.isSuccess()) {`
- 
`// SUCCESS`
- 
`}`
- 
`else {`
- 
`// FAILURE`
- 
`}`
- 
`}`
- 
`});`
guava里面也提供了通用的扩展Future: ListenableFuture\SettableFuture以及辅助类Futures等，方便异步编程。
作为正统Java类库，是不是应该加点什么特性，可以加强一下自身库的功能？
Java8里面新增加了一个包含50个方法左右的类：CompletableFuture. 提供了非常强大的Future的扩展功能，可以帮助简化异步编程的复杂性，提供了函数式编程能力，可以通过回调的方式计算处理结果，并且提供了转换和组织CompletableFuture的方法。
主动完成计算
CompletableFuture 类实现了CompletionStage和Future接口，所以还是可以像以前一样通过阻塞或轮询的方式获得结果。尽管这种方式不推荐使用。
- 
`public T get()`
- 
`public T get(long timeout, TimeUnit unit)`
- 
`public T getNow(T valueIfAbsent)`
- 
`public T join()`
其中的getNow有点特殊，如果结果已经计算完则返回结果或抛异常，否则返回给定的valueIfAbsent的值。
join返回计算的结果或抛出一个uncheckd异常。
尽管Future可以代表在另外的线程中执行一段异步代码，但你还是可以在本身线程中执行：
**创建CompletableFuture对象**
CompletableFuture.compleatedFuture是一个静态辅助方法，用来返回一个已经计算好的CompletableFuture.
以下四个静态方法用来为一段异步执行的代码创建CompletableFuture对象：
- 
`public static CompletableFuture<Void> runAsync(Runnable runnable)`
- 
`public static CompletableFuture<Void> runAsync(Runnable runnable, Executor executor)`
- 
`public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier)`
- 
`public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier, Executor executor)`
以Async结尾并且没有指定Executor的方法会使用ForkJoinPool.commonPool() 作为它的线程池执行异步代码。
runAsync方法：它以Runnabel函数式接口类型为参数，所以CompletableFuture的计算结果为空。
supplyAsync方法以Supplier<U>函数式接口类型为参数，CompletableFuture的计算结果类型为U。
方法的参数类型都是函数式接口，所以可以使用lambda表达式实现异步任务。如：
- 
`CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {`
- 
`//长时间的计算任务`
- 
`return "·00";`
- 
`});`
**计算结果完成时的处理**
当CompletableFuture的计算结果完成，或者抛出异常的时候，可以执行特定的Action。主要是下面的方法：
- 
`public CompletableFuture<T> whenComplete(BiConsumer<? super T,? super Throwable> action)`
- 
`public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action)`
- 
`public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action, Executor executor)`
- 
`public CompletableFuture<T> exceptionally(Function<Throwable,? extends T> fn)`
可以看到Action的类型是BiConsumer<? super T,? super Throwable>它可以处理正常的计算结果，或者异常情况。
方法不以Async结尾，意味着Action使用相同的线程执行，而Async可能会使用其他线程执行（如果是使用相同的线程池，也可能会被同一个线程选中执行）
- 
`public class BasicFuture {`
- 
- 
`private static Random rand = new Random();`
- 
`private static long t = System.currentTimeMillis();`
- 
- 
`static int getMoreData() {`
- 
`System.out.println("begin to start compute");`
- 
`try {`
- 
`TimeUnit.SECONDS.sleep(3);`
- 
`} catch (InterruptedException e) {`
- 
`e.printStackTrace();`
- 
`}`
- 
`System.out.println("end to compute,passed " + (System.currentTimeMillis()-t));`
- 
`return rand.nextInt(1000);`
- 
`}`
- 
- 
- 
`public static void main(String[] args) throws ExecutionException, InterruptedException {`
- 
`CompletableFuture<Integer> future = CompletableFuture.supplyAsync(BasicFuture::getMoreData);`
- 
`Future<Integer> f = future.whenComplete((v,e) -> {`
- 
`System.out.println(v);`
- 
`System.out.println(e);`
- 
`});`
- 
`System.out.println(f.get());`
- 
`}}`
下面一组方法虽然也返回CompletableFuture对象，但是对象的值和原来的CompletableFuture计算的值不同，当原先的CompletableFuture的值计算完成或抛异常的时候，会触发CompletableFuture对象的计算。
**转换**
CompletableFuture可以作为monad(单子)和functor. 由于回调风格的实现，我们不必因为等待一个计算完成而阻塞着调用线程，而是告诉CompletableFuture当计算完成的时候请执行某个Function. 还可以串联起来。
- 
`public <U> CompletableFuture<U> thenApply(Function<? super T,? extends U> fn)`
- 
`public <U> CompletableFuture<U> thenApplyAsync(Function<? super T,? extends U> fn)`
- 
`public <U> CompletableFuture<U> thenApplyAsync(Function<? super T,? extends U> fn, Executor executor)`
- 
参考：
http://colobu.com/2016/02/29/Java-CompletableFuture/
