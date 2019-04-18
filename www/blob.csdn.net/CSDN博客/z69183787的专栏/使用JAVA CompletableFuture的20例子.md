# 使用JAVA CompletableFuture的20例子 - z69183787的专栏 - CSDN博客
2018年04月17日 14:12:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：466
[https://segmentfault.com/a/1190000013452165?utm_source=index-hottest](https://segmentfault.com/a/1190000013452165?utm_source=index-hottest)
## 前言
Java 5 并发库主要关注于异步任务的处理，它采用了这样一种模式，producer 线程创建任务并且利用阻塞队列将其传递给任务的 consumer。这种模型在 Java 7 和 8 中进一步发展，并且开始支持另外一种风格的任务执行，那就是将任务的数据集分解为子集，每个子集都可以由独立且同质的子任务来负责处理。
这种风格的基础库也就是 fork/join 框架，它允许程序员规定数据集该如何进行分割，并且支持将子任务提交到默认的标准线程池中，也就是"通用的"ForkJoinPool。Java 8 中，fork/join 并行功能借助并行流的机制变得更加具有可用性。但是，不是所有的问题都适合这种风格的并行处理：所处理的元素必须是独立的，数据集要足够大，并且在并行加速方面，每个元素的处理成本要足够高，这样才能补偿建立 fork/join 框架所消耗的成本。CompletableFuture 类则是 Java 8 在并行流方面的创新。
## 准备知识
### 异步计算
所谓异步调用其实就是实现一个可无需等待被调用函数的返回值而让操作继续运行的方法。在 Java 语言中，简单的讲就是另启一个线程来完成调用中的部分计算，使调用继续运行或返回，而不需要等待计算结果。但调用者仍需要取线程的计算结果。
### 回调函数
回调函数比较通用的解释是，它是一个通过函数指针调用的函数。如果你把函数的指针（地址）作为参数传递给另一个函数，当这个指针被用为调用它所指向的函数时，我们就说这是回调函数。回调函数不是由该函数的实现方直接调用，而是在特定的事件或条件发生时由另外一方调用的，用于对该事件或条件进行响应。
回调函数的机制：
（1）定义一个回调函数；
（2）提供函数实现的一方在初始化时候，将回调函数的函数指针注册给调用者；
（3）当特定的事件或条件发生的时候，调用者使用函数指针调用回调函数对事件进行处理。
回调函数通常与原始调用者处于同一层次，如图 1 所示：
图 1 回调函数示例图
![](https://img-blog.csdn.net/20180501084636478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21vYWt1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Future 接口介绍
JDK5 新增了 Future 接口，用于描述一个异步计算的结果。虽然 Future 以及相关使用方法提供了异步执行任务的能力，但是对于结果的获取却是很不方便，只能通过阻塞或者轮询的方式得到任务的结果。阻塞的方式显然和我们的异步编程的初衷相违背，轮询的方式又会耗费无谓的 CPU 资源，而且也不能及时地得到计算结果，为什么不能用观察者设计模式呢？即当计算结果完成及时通知监听者。
有一些开源框架实现了我们的设想，例如 Netty 的 ChannelFuture 类扩展了 Future 接口，通过提供 addListener 方法实现支持回调方式的异步编程。Netty 中所有的 I/O 操作都是异步的,这意味着任何的 I/O 调用都将立即返回，而不保证这些被请求的 I/O 操作在调用结束的时候已经完成。取而代之地，你会得到一个返回的 ChannelFuture 实例，这个实例将给你一些关于 I/O 操作结果或者状态的信息。当一个 I/O 操作开始的时候，一个新的 Future 对象就会被创建。在开始的时候，新的 Future 是未完成的状态－－它既非成功、失败，也非被取消，因为 I/O 操作还没有结束。如果 I/O 操作以成功、失败或者被取消中的任何一种状态结束了，那么这个 Future 将会被标记为已完成，并包含更多详细的信息（例如：失败的原因）。请注意，即使是失败和被取消的状态，也是属于已完成的状态。阻塞方式的示例代码如清单 1 所示。
清单 1 阻塞方式示例代码
|123456|`// Start the connection attempt.``ChannelFuture Future = bootstrap.connect(new InetSocketAddress(host, port));``// Wait until the connection is closed or the connection attempt fails.``Future.getChannel().getCloseFuture().awaitUninterruptibly();``// Shut down thread pools to exit.``bootstrap.releaseExternalResources();`|
上面代码使用的是 awaitUninterruptibly 方法，源代码如清单 2 所示。
清单 2 awaitUninterruptibly 源代码
|12345678910111213141516171819202122|`publicChannelFutureawaitUninterruptibly() {``    ``boolean interrupted = false;``    ``synchronized (this) {``        ``//循环等待到完成``        ``while (!done) {``            ``checkDeadLock();``            ``waiters++;``        ``try {``            ``wait();``        ``} catch (InterruptedException e) {``            ``//不允许中断``            ``interrupted = true;``        ``} finally {``            ``waiters--;``        ``}``    ``}``}``    ``if (interrupted) {``    ``Thread.currentThread().interrupt();``}``return this;``}`|
清单 3 异步非阻塞方式示例代码
|12345678910|`// Start the connection attempt.``ChannelFuture Future = bootstrap.connect(new InetSocketAddress(host, port));``Future.addListener(new ChannelFutureListener(){``    ``public void operationComplete(final ChannelFuture Future)``        ``throws Exception``        ``{          ``    ``}``});``// Shut down thread pools to exit.``bootstrap.releaseExternalResources();`|
可以明显的看出，在异步模式下，上面这段代码没有阻塞，在执行 connect 操作后直接执行到 printTime("异步时间： ")，随后 connect 完成，Future 的监听函数输出 connect 操作完成。
非阻塞则是添加监听类 ChannelFutureListener，通过覆盖 ChannelFutureListener 的 operationComplete 执行业务逻辑。
清单 4 异步非阻塞方式示例代码
|12345678910111213141516171819202122232425|`public void addListener(final ChannelFutureListener listener) {``    ``if (listener == null) {``    ``throw new NullPointerException("listener");``}``    ``booleannotifyNow = false;``    ``synchronized (this) {``        ``if (done) {``        ``notifyNow = true;``    ``} else {``        ``if (firstListener == null) {``        ``//listener 链表头``        ``firstListener = listener;``    ``} else {``        ``if (otherListeners == null) {``        ``otherListeners = new ArrayList<``ChannelFutureListener``>(1);``        ``}``        ``//添加到 listener 链表中，以便操作完成后遍历操作``        ``otherListeners.add(listener);``    ``}``    ``......``    ``if (notifyNow) {``        ``//通知 listener 进行处理``        ``notifyListener(listener);``        ``}``}`|
这部分代码的逻辑很简单，就是注册回调函数，当操作完成后自动调用回调函数，就达到了异步的效果。
### CompletableFuture 类介绍 
Java 8 中, 新增加了一个包含 50 个方法左右的类--CompletableFuture，它提供了非常强大的 Future 的扩展功能，可以帮助我们简化异步编程的复杂性，并且提供了函数式编程的能力，可以通过回调的方式处理计算结果，也提供了转换和组合 CompletableFuture 的方法。
对于阻塞或者轮询方式，依然可以通过 CompletableFuture 类的 CompletionStage 和 Future 接口方式支持。
CompletableFuture 类声明了 CompletionStage 接口，CompletionStage 接口实际上提供了同步或异步运行计算的舞台，所以我们可以通过实现多个 CompletionStage 命令，并且将这些命令串联在一起的方式实现多个命令之间的触发。
我们可以通过 CompletableFuture.supplyAsync(this::sendMsg); 这么一行代码创建一个简单的异步计算。在这行代码中，supplyAsync 支持异步地执行我们指定的方法，这个例子中的异步执行方法是 sendMsg。当然，我们也可以使用 Executor 执行异步程序，默认是 ForkJoinPool.commonPool()。
我们也可以在异步计算结束之后指定回调函数，例如 CompletableFuture.supplyAsync(this::sendMsg) .thenAccept(this::notify);这行代码中的 thenAccept 被用于增加回调函数，在我们的示例中 notify 就成了异步计算的消费者，它会处理计算结果。
因为`CompletableFuture`是`CompletionInterface`接口的实现，所以我们首先要了解该接口的契约。它代表某个同步或异步计算的一个阶段。你可以把它理解为是一个为了产生有价值最终结果的计算的流水线上的一个单元。这意味着多个`ComletionStage`指令可以链接起来从而一个阶段的完成可以触发下一个阶段的执行。
除了实现了`CompletionStage`接口，`Completion`还继承了`Future`，这个接口用于实现一个未开始的异步事件。因为能够显式的完成`Future`，所以取名为`CompletableFuture`。
## 1.新建一个完成的CompletableFuture
这个简单的示例中创建了一个已经完成的预先设置好结果的`CompletableFuture`。通常作为计算的起点阶段。
```
static void completedFutureExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message");
    assertTrue(cf.isDone());
    assertEquals("message", cf.getNow(null));
}
```
`getNow`方法会返回完成后的结果（这里就是message），如果还未完成，则返回传入的默认值`null`。
## 2.运行一个简单的异步stage
下面的例子解释了如何创建一个异步运行`Runnable`的stage。
```
static void runAsyncExample() {
    CompletableFuture cf = CompletableFuture.runAsync(() -> {
        assertTrue(Thread.currentThread().isDaemon());
        randomSleep();
    });
    assertFalse(cf.isDone());
    sleepEnough();
    assertTrue(cf.isDone());
}
```
这个例子想要说明两个事情：
- `CompletableFuture`中以`Async`为结尾的方法将会异步执行
- 默认情况下（即指没有传入`Executor`的情况下），异步执行会使用`ForkJoinPool`实现，该线程池使用一个后台线程来执行`Runnable`任务。注意这只是特定于`CompletableFuture`实现，其它的`CompletableStage`实现可以重写该默认行为。
## 3.将方法作用于前一个Stage
下面的例子引用了第一个例子中已经完成的`CompletableFuture`，它将引用生成的字符串结果并将该字符串大写。
```
static void thenApplyExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message").thenApply(s -> {
        assertFalse(Thread.currentThread().isDaemon());
        return s.toUpperCase();
    });
    assertEquals("MESSAGE", cf.getNow(null));
}
```
这里的关键词是`thenApply`：
- `then`是指在当前阶段正常执行完成后（正常执行是指没有抛出异常）进行的操作。在本例中，当前阶段已经完成并得到值`message`。
- `Apply`是指将一个`Function`作用于之前阶段得出的结果
`Function`是阻塞的，这意味着只有当大写操作执行完成之后才会执行`getNow()`方法。
## 4.异步的的将方法作用于前一个Stage
通过在方法后面添加`Async`后缀，该`CompletableFuture`链将会异步执行（使用ForkJoinPool.commonPool()）
```
static void thenApplyAsyncExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message").thenApplyAsync(s -> {
        assertTrue(Thread.currentThread().isDaemon());
        randomSleep();
        return s.toUpperCase();
    });
    assertNull(cf.getNow(null));
    assertEquals("MESSAGE", cf.join());
}
```
## 使用一个自定义的Executor来异步执行该方法
异步方法的一个好处是可以提供一个`Executor`来执行`CompletableStage`。这个例子展示了如何使用一个固定大小的线程池来实现大写操作。
```
static ExecutorService executor = Executors.newFixedThreadPool(3, new ThreadFactory() {
    int count = 1;
    @Override
    public Thread newThread(Runnable runnable) {
        return new Thread(runnable, "custom-executor-" + count++);
    }
});
static void thenApplyAsyncWithExecutorExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message").thenApplyAsync(s -> {
        assertTrue(Thread.currentThread().getName().startsWith("custom-executor-"));
        assertFalse(Thread.currentThread().isDaemon());
        randomSleep();
        return s.toUpperCase();
    }, executor);
    assertNull(cf.getNow(null));
    assertEquals("MESSAGE", cf.join());
}
```
## 6.消费(Consume)前一个Stage的结果
如果下一个Stage接收了当前Stage的结果但是在计算中无需返回值（比如其返回值为void），那么它将使用方法`thenAccept`并传入一个`Consumer`接口。
```
static void thenAcceptExample() {
    StringBuilder result = new StringBuilder();
    CompletableFuture.completedFuture("thenAccept message")
            .thenAccept(s -> result.append(s));
    assertTrue("Result was empty", result.length() > 0);
}
```
`Consumer`将会同步执行，所以我们无需在返回的`CompletableFuture`上执行join操作。
## 7.异步执行Comsume
同样，使用Asyn后缀实现：
```
static void thenAcceptAsyncExample() {
    StringBuilder result = new StringBuilder();
    CompletableFuture cf = CompletableFuture.completedFuture("thenAcceptAsync message")
            .thenAcceptAsync(s -> result.append(s));
    cf.join();
    assertTrue("Result was empty", result.length() > 0);
}
```
## 8.计算出现异常时
我们现在来模拟一个出现异常的场景。为了简洁性，我们还是将一个字符串大写，但是我们会模拟延时进行该操作。我们会使用`thenApplyAsyn(Function, Executor)`，第一个参数是大写转化方法，第二个参数是一个延时executor，它会延时一秒钟再将操作提交给`ForkJoinPool`。
```
static void completeExceptionallyExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message").thenApplyAsync(String::toUpperCase,
            CompletableFuture.delayedExecutor(1, TimeUnit.SECONDS));
    CompletableFuture exceptionHandler = cf.handle((s, th) -> { return (th != null) ? "message upon cancel" : ""; });
    cf.completeExceptionally(new RuntimeException("completed exceptionally"));
    assertTrue("Was not completed exceptionally", cf.isCompletedExceptionally());
    try {
        cf.join();
        fail("Should have thrown an exception");
    } catch(CompletionException ex) { // just for testing
        assertEquals("completed exceptionally", ex.getCause().getMessage());
    }
    assertEquals("message upon cancel", exceptionHandler.join());
}
```
- 首先，我们新建了一个已经完成并带有返回值`message`的`CompletableFuture`对象。然后我们调用`thenApplyAsync`方法，该方法会返回一个新的`CompletableFuture`。这个方法用异步的方式执行大写操作。这里还展示了如何使用`delayedExecutor(timeout, timeUnit)`方法来延时异步操作。
- 然后我们创建了一个handler stage，`exceptionHandler`，这个阶段会处理一切异常并返回另一个消息`message upon cancel`。
- 最后，我们显式的完成第二个阶段并抛出异常，它会导致进行大写操作的阶段抛出`CompletionException`。它还会触发`handler`阶段。
> API补充：
`<U> CompletableFuture<U> handle(BiFunction<? super T,Throwable,? extends U> fn)`
返回一个新的CompletionStage，无论之前的Stage是否正常运行完毕。传入的参数包括上一个阶段的结果和抛出异常。
## 9.取消计算
和计算时异常处理很相似，我们可以通过`Future`接口中的`cancel(boolean mayInterruptIfRunning)`来取消计算。
```
static void cancelExample() {
    CompletableFuture cf = CompletableFuture.completedFuture("message").thenApplyAsync(String::toUpperCase,
            CompletableFuture.delayedExecutor(1, TimeUnit.SECONDS));
    CompletableFuture cf2 = cf.exceptionally(throwable -> "canceled message");
    assertTrue("Was not canceled", cf.cancel(true));
    assertTrue("Was not completed exceptionally", cf.isCompletedExceptionally());
    assertEquals("canceled message", cf2.join());
}
```
> API补充
`public CompletableFuture<T> exceptionally(Function<Throwable,? extends T> fn)`
返回一个新的CompletableFuture，如果出现异常，则为该方法中执行的结果，否则就是正常执行的结果。
## 10.将Function作用于两个已完成Stage的结果之一
下面的例子创建了一个`CompletableFuture`对象并将`Function`作用于已完成的两个Stage中的任意一个（没有保证哪一个将会传递给Function）。这两个阶段分别如下：一个将字符串大写，另一个小写。
```
static void applyToEitherExample() {
    String original = "Message";
    CompletableFuture cf1 = CompletableFuture.completedFuture(original)
            .thenApplyAsync(s -> delayedUpperCase(s));
    CompletableFuture cf2 = cf1.applyToEither(
            CompletableFuture.completedFuture(original).thenApplyAsync(s -> delayedLowerCase(s)),
            s -> s + " from applyToEither");
    assertTrue(cf2.join().endsWith(" from applyToEither"));
}
```
> `public <U> CompletableFuture<U> applyToEitherAsync(CompletionStage<? extends T> other,Function<? super T,U> fn)`
返回一个全新的CompletableFuture，包含着this或是other操作完成之后，在二者中的任意一个执行fn
## 11.消费两个阶段的任意一个结果
和前一个例子类似，将`Function`替换为`Consumer`
```
static void acceptEitherExample() {
    String original = "Message";
    StringBuilder result = new StringBuilder();
    CompletableFuture cf = CompletableFuture.completedFuture(original)
            .thenApplyAsync(s -> delayedUpperCase(s))
            .acceptEither(CompletableFuture.completedFuture(original).thenApplyAsync(s -> delayedLowerCase(s)),
                    s -> result.append(s).append("acceptEither"));
    cf.join();
    assertTrue("Result was empty", result.toString().endsWith("acceptEither"));
}
```
## 12.在两个阶段都完成后运行Runnable
注意这里的两个Stage都是同步运行的，第一个stage将字符串转化为大写之后，第二个stage将其转化为小写。
```
static void runAfterBothExample() {
    String original = "Message";
    StringBuilder result = new StringBuilder();
    CompletableFuture.completedFuture(original).thenApply(String::toUpperCase).runAfterBoth(
            CompletableFuture.completedFuture(original).thenApply(String::toLowerCase),
            () -> result.append("done"));
    assertTrue("Result was empty", result.length() > 0);
}
```
## 13.用Biconsumer接收两个stage的结果
Biconsumer支持同时对两个Stage的结果进行操作。
```
static void thenAcceptBothExample() {
    String original = "Message";
    StringBuilder result = new StringBuilder();
    CompletableFuture.completedFuture(original).thenApply(String::toUpperCase).thenAcceptBoth(
            CompletableFuture.completedFuture(original).thenApply(String::toLowerCase),
            (s1, s2) -> result.append(s1 + s2));
    assertEquals("MESSAGEmessage", result.toString());
}
```
## 14.将Bifunction同时作用于两个阶段的结果
如果`CompletableFuture`想要合并两个阶段的结果并且返回值，我们可以使用方法`thenCombine`。这里的计算流都是同步的，所以最后的`getNow()`方法会获得最终结果，即大写操作和小写操作的结果的拼接。
```
static void thenCombineExample() {
    String original = "Message";
    CompletableFuture cf = CompletableFuture.completedFuture(original).thenApply(s -> delayedUpperCase(s))
            .thenCombine(CompletableFuture.completedFuture(original).thenApply(s -> delayedLowerCase(s)),
                    (s1, s2) -> s1 + s2);
    assertEquals("MESSAGEmessage", cf.getNow(null));
}
```
## 15.异步将Bifunction同时作用于两个阶段的结果
和之前的例子类似，只是这里用了不同的方法：即两个阶段的操作都是异步的。那么`thenCombine`也会异步执行，及时它没有Async后缀。
```
static void thenCombineAsyncExample() {
    String original = "Message";
    CompletableFuture cf = CompletableFuture.completedFuture(original)
            .thenApplyAsync(s -> delayedUpperCase(s))
            .thenCombine(CompletableFuture.completedFuture(original).thenApplyAsync(s -> delayedLowerCase(s)),
                    (s1, s2) -> s1 + s2);
    assertEquals("MESSAGEmessage", cf.join());
}
```
## 16.Compose CompletableFuture
我们可以使用`thenCompose`来完成前两个例子中的操作。
```
static void thenComposeExample() {
    String original = "Message";
    CompletableFuture cf = CompletableFuture.completedFuture(original).thenApply(s -> delayedUpperCase(s))
            .thenCompose(upper -> CompletableFuture.completedFuture(original).thenApply(s -> delayedLowerCase(s))
                    .thenApply(s -> upper + s));
    assertEquals("MESSAGEmessage", cf.join());
}
```
## 17.当多个阶段中有有何一个完成，即新建一个完成阶段
```
static void anyOfExample() {
    StringBuilder result = new StringBuilder();
    List messages = Arrays.asList("a", "b", "c");
    List<CompletableFuture> futures = messages.stream()
            .map(msg -> CompletableFuture.completedFuture(msg).thenApply(s -> delayedUpperCase(s)))
            .collect(Collectors.toList());
    CompletableFuture.anyOf(futures.toArray(new CompletableFuture[futures.size()])).whenComplete((res, th) -> {
        if(th == null) {
            assertTrue(isUpperCase((String) res));
            result.append(res);
        }
    });
    assertTrue("Result was empty", result.length() > 0);
}
```
## 18.当所有的阶段完成，新建一个完成阶段
```
static void allOfExample() {
    StringBuilder result = new StringBuilder();
    List messages = Arrays.asList("a", "b", "c");
    List<CompletableFuture> futures = messages.stream()
            .map(msg -> CompletableFuture.completedFuture(msg).thenApply(s -> delayedUpperCase(s)))
            .collect(Collectors.toList());
    CompletableFuture.allOf(futures.toArray(new CompletableFuture[futures.size()]))
        .whenComplete((v, th) -> {
            futures.forEach(cf -> assertTrue(isUpperCase(cf.getNow(null))));
            result.append("done");
        });
    assertTrue("Result was empty", result.length() > 0);
}
```
## 19.当所有阶段完成以后，新建一个异步完成阶段
```
static void allOfAsyncExample() {
    StringBuilder result = new StringBuilder();
    List messages = Arrays.asList("a", "b", "c");
    List<CompletableFuture> futures = messages.stream()
            .map(msg -> CompletableFuture.completedFuture(msg).thenApplyAsync(s -> delayedUpperCase(s)))
            .collect(Collectors.toList());
    CompletableFuture allOf = CompletableFuture.allOf(futures.toArray(new CompletableFuture[futures.size()]))
            .whenComplete((v, th) -> {
                futures.forEach(cf -> assertTrue(isUpperCase(cf.getNow(null))));
                result.append("done");
            });
    allOf.join();
    assertTrue("Result was empty", result.length() > 0);
}
```
## 20.真实场景
下面展示了一个实践CompletableFuture的场景：
- 先通过调用`cars()`方法异步获得`Car`列表。它将会返回一个`CompletionStage<List<Car>>`。`cars()`方法应当使用一个远程的REST端点来实现。
- 我们将该Stage和另一个Stage组合，另一个Stage会通过调用`rating(manufactureId)`来异步获取每辆车的评分。
- 当所有的Car对象都填入评分后，我们调用`allOf()`来进入最终Stage，它将在这两个阶段完成后执行
- 在最终Stage上使用`whenComplete()`，打印出车辆的评分。
```
cars().thenCompose(cars -> {
    List<CompletionStage> updatedCars = cars.stream()
            .map(car -> rating(car.manufacturerId).thenApply(r -> {
                car.setRating(r);
                return car;
            })).collect(Collectors.toList());
    CompletableFuture done = CompletableFuture
            .allOf(updatedCars.toArray(new CompletableFuture[updatedCars.size()]));
    return done.thenApply(v -> updatedCars.stream().map(CompletionStage::toCompletableFuture)
            .map(CompletableFuture::join).collect(Collectors.toList()));
}).whenComplete((cars, th) -> {
    if (th == null) {
        cars.forEach(System.out::println);
    } else {
        throw new RuntimeException(th);
    }
}).toCompletableFuture().join();
```
