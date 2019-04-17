# Java 8并发教程：原子变量和ConcurrentMap - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月25日 14:21:49[boonya](https://me.csdn.net/boonya)阅读数：1377








原文地址：[http://winterbe.com/posts/2015/05/22/java8-concurrency-tutorial-atomic-concurrent-map-examples/](http://winterbe.com/posts/2015/05/22/java8-concurrency-tutorial-atomic-concurrent-map-examples/)




欢迎阅读我的Java 8中多线程编程教程系列的第三部分。本教程介绍了并发API的两个重要部分：原子变量和并发映射。在最新的Java 8版本中引入了lambda表达式和功能编程，两者都得到了很大的改进。所有这些新功能都用一大堆易于理解的代码示例进行描述。请享用！
- 第1部分： [线程和执行器](http://winterbe.com/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/)
- 第2部分： [同步和锁定](http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/)
- 第3部分：原子变量和并发图 

为了简单起见，本教程的代码示例使用[这里](https://github.com/winterbe/java8-tutorial/blob/master/src/com/winterbe/java8/samples/concurrent/ConcurrentUtils.java)定义的两个辅助方法sleep(seconds)和stop(executor)。

## AtomicInteger

java.concurrent.atomic包包含许多有用的类来执行原子操作。当您可以安全地在多个线程上并行执行操作时，操作是原子的，而不使用我[以前的教程](http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/)中所示的synchronized关键字或锁。

在内部，原子类大量使用[比较和交换](http://en.wikipedia.org/wiki/Compare-and-swap)（CAS），这是大多数现代CPU直接支持的原子指令。那些指令通常比同步通过锁快得多。所以我的建议是更喜欢原子类超过锁，以防你只需要同时更改单个可变变量。

AtomicInteger现在让我们选择一个原子类的几个例子：AtomicInteger

```java
AtomicInteger atomicInt = new AtomicInteger(0);

ExecutorService executor = Executors.newFixedThreadPool(2);

IntStream.range(0, 1000)
    .forEach(i -> executor.submit(atomicInt::incrementAndGet));

stop(executor);

System.out.println(atomicInt.get());    // => 1000
```
通过使用AtomicInteger作为Integer的替代，我们可以在线程安全的庄园中同时增加数量，而不需要同步对变量的访问。方法incrementAndGet()是一个原子操作，所以我们可以从多个线程安全地调用这个方法。
 AtomicInteger支持各种原子操作。方法updateAndGet()接受一个lambda表达式，以便对整数执行任意的算术运算：

```java
AtomicInteger atomicInt = new AtomicInteger(0);

ExecutorService executor = Executors.newFixedThreadPool(2);

IntStream.range(0, 1000)
    .forEach(i -> {
        Runnable task = () ->
            atomicInt.updateAndGet(n -> n + 2);
        executor.submit(task);
    });

stop(executor);

System.out.println(atomicInt.get());    // => 2000
```
方法accumulateAndGet()接受另一种类型为IntBinaryOperator的lambda IntBinaryOperator。我们使用这种方法在下一个示例中将所有值从0到1000并发：




```java
AtomicInteger atomicInt = new AtomicInteger(0);

ExecutorService executor = Executors.newFixedThreadPool(2);

IntStream.range(0, 1000)
    .forEach(i -> {
        Runnable task = () ->
            atomicInt.accumulateAndGet(i, (n, m) -> n + m);
        executor.submit(task);
    });

stop(executor);

System.out.println(atomicInt.get());    // => 499500
```

其他有用的原子类是[AtomicBoolean](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/atomic/AtomicBoolean.html)，[AtomicLong](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/atomic/AtomicLong.html)和[AtomicReference](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/atomic/AtomicReference.html)。
## LongAdder

可以使用LongAdder类作为AtomicLong的替代方法来AtomicLong地向数字添加值。

```java
ExecutorService executor = Executors.newFixedThreadPool(2);

IntStream.range(0, 1000)
    .forEach(i -> executor.submit(adder::increment));

stop(executor);

System.out.println(adder.sumThenReset());   // => 1000
```
LongAdder提供了方法add()和increment()，就像原子序列类一样，也是线程安全的。但是，除了总结单个结果之外，这个类在内部维护一组变量以减少对线程的争用。实际结果可以通过调用sum()或sumThenReset()。
当多线程的更新比读取更常见时，此类通常优于原子序号。捕获统计数据时通常是这种情况，例如，您想要计算在Web服务器上提供的请求数。LongAdder的缺点是更高的内存消耗，因为一组变量被保存在内存中。

## LongAccumulator

 LongAccumulator是LongAdder的更广泛版本。代替执行简单的添加操作，类LongAccumulator构建了LongBinaryOperator类型的lambda表达式，如此代码示例所示：

```java
LongBinaryOperator op = (x, y) -> 2 * x + y;
LongAccumulator accumulator = new LongAccumulator(op, 1L);

ExecutorService executor = Executors.newFixedThreadPool(2);

IntStream.range(0, 10)
    .forEach(i -> executor.submit(() -> accumulator.accumulate(i)));

stop(executor);

System.out.println(accumulator.getThenReset());     // => 2539
```
我们创建一个具有函数2 * x + y和初始值为1的LongAccumulator。每次调用accumulate(i)当前结果和值i都作为参数传递给lambda表达式。
LongAccumulator就像LongAdder一样，在LongAdder维护一组变量以减少与线程的争用。

## ConcurrentMap

ConcurrentMap扩展了映射接口，并定义了最有用的并发收集类型之一。Java 8通过向此界面添加新方法来引入功能编程。

在下面的代码段中，我们使用以下示例映射来演示这些新方法：



```java
ConcurrentMap<String, String> map = new ConcurrentHashMap<>();
map.put("foo", "bar");
map.put("han", "solo");
map.put("r2", "d2");
map.put("c3", "p0");
```

**方法forEach()**接受一个类型为BiConsumer的lambda表达式，BiConsumer具有作为参数传递的映射的键和值。它可以用作替代每个循环来遍历并发映射的条目。迭代在当前线程上顺序执行。




```java
map.forEach((key, value) -> System.out.printf("%s = %s\n", key, value));
```

**方法putIfAbsent()**只有在给定键不存在任何值时，才会将新值放入映射。至少对于ConcurrentHashMap，该方法的实现是线程安全的，就像put()，所以你不必同步从不同的线程并发访问映射：
```java
String value = map.putIfAbsent("c3", "p1");
System.out.println(value);    // p0
```
**getOrDefault()**方法返回给定键的值。如果此键不存在，则返回传递的默认值：




```java
String value = map.getOrDefault("hi", "there");
System.out.println(value);    // there
```

**replaceAll()****方法**接受一个类型为BiFunction的lambda BiFunction。BiFunctions需要两个参数并返回一个值。在这种情况下，使用键和每个映射条目的值调用该函数，并返回要为当前密钥分配的新值：
```java
map.replaceAll((key, value) -> "r2".equals(key) ? "d3" : value);
System.out.println(map.get("r2"));    // d3
```
而不是替换map的所有值compute()让我们转换单个条目。该方法接受要计算的密钥和双功能以指定值的转换。




```java
map.compute("foo", (key, value) -> value + value);
System.out.println(map.get("foo"));   // barbar
```

除了compute()还有两个变量：computeIfAbsent()和computeIfPresent()。这些方法的功能参数只有在键不存在或分别存在的情况下才被调用。
最后，可以使用merge()方法merge()新值与映射中的现有值进行统一。合并接受一个密钥，要合并到现有条目中的新值和一个双功能来指定两个值的合并行为：



```java
map.merge("foo", "boo", (oldVal, newVal) -> newVal + " was " + oldVal);
System.out.println(map.get("foo"));   // boo was foo
```


## ConcurrentHashMap

以上所有这些方法都是ConcurrentMap一部分，从而可用于该接口的所有实现。此外，最重要的实现ConcurrentHashMap已经通过几种新方法进一步增强，以在地图上执行并行操作。

就像并行流一样，这些方法使用Java 8中的ForkJoinPool.commonPool()可以使用一个特殊的ForkJoinPool。该池使用一个取决于可用内核数量的预设并行度。我的机器上有四个CPU内核可以实现三个并行处理：

```java
System.out.println(ForkJoinPool.getCommonPoolParallelism());  // 3
```
可以通过设置以下JVM参数来减小或增加该值：


```java
-Djava.util.concurrent.ForkJoinPool.common.parallelism=5
```
我们使用相同的示例图来进行演示，但是这次我们通过具体实现ConcurrentHashMap来代替ConcurrentMap，所以我们可以从这个类访问所有的公共方法：


```java
ConcurrentHashMap<String, String> map = new ConcurrentHashMap<>();
map.put("foo", "bar");
map.put("han", "solo");
map.put("r2", "d2");
map.put("c3", "p0");
```
Java 8引入了三种并行操作：forEach，search和reduce。这些操作中的每一个都有四种形式接受具有键，值，条目和键值对参数的函数。
所有这些方法都使用一个共同的第一个参数，称为parallelismThreshold。该阈值表示并行执行操作时的最小收集大小。例如，如果通过阈值为500，并且地图的实际大小为499，则操作将在单个线程上顺序执行。在下面的例子中，我们使用一个阈值来总是强制执行并行执行来进行演示。

### ForEach

方法forEach()能够并行迭代地图的键值对。使用当前迭代步骤的键和值调用类型BiConsumer的lambda表达式。为了可视化并行执行，我们将当前线程名称打印到控制台。请记住，在我的情况下，底层的ForkJoinPool使用三个线程。



```java
map.forEach(1, (key, value) ->
    System.out.printf("key: %s; value: %s; thread: %s\n",
        key, value, Thread.currentThread().getName()));

// key: r2; value: d2; thread: main
// key: foo; value: bar; thread: ForkJoinPool.commonPool-worker-1
// key: han; value: solo; thread: ForkJoinPool.commonPool-worker-2
// key: c3; value: p0; thread: main
```



### Search

方法search()接受返回当前键值对的非空搜索结果的BiFunction，如果当前迭代不符合所需的搜索条件，则null。一旦返回非空结果，进一步处理被抑制。请记住，ConcurrentHashMap是无序的。搜索功能不应取决于地图的实际处理顺序。如果地图的多个条目与给定的搜索函数匹配，则结果可能是非确定性的。



```java
String result = map.search(1, (key, value) -> {
    System.out.println(Thread.currentThread().getName());
    if ("foo".equals(key)) {
        return value;
    }
    return null;
});
System.out.println("Result: " + result);

// ForkJoinPool.commonPool-worker-2
// main
// ForkJoinPool.commonPool-worker-3
// Result: bar
```

以下是另一个仅查看地图值的示例：


```java
String result = map.searchValues(1, value -> {
    System.out.println(Thread.currentThread().getName());
    if (value.length() > 3) {
        return value;
    }
    return null;
});

System.out.println("Result: " + result);

// ForkJoinPool.commonPool-worker-2
// main
// main
// ForkJoinPool.commonPool-worker-1
// Result: solo
```


### Reduce

Java 8 Streams中已知的方法reduce()接受两种类型为BiFunction lambda BiFunction。第一个函数将每个键值对转换为任何类型的单个值。第二个功能将所有这些变换的值组合成一个单独的结果，忽略任何可能的null值。

```java
String result = map.reduce(1,
    (key, value) -> {
        System.out.println("Transform: " + Thread.currentThread().getName());
        return key + "=" + value;
    },
    (s1, s2) -> {
        System.out.println("Reduce: " + Thread.currentThread().getName());
        return s1 + ", " + s2;
    });

System.out.println("Result: " + result);

// Transform: ForkJoinPool.commonPool-worker-2
// Transform: main
// Transform: ForkJoinPool.commonPool-worker-3
// Reduce: ForkJoinPool.commonPool-worker-3
// Transform: main
// Reduce: main
// Reduce: main
// Result: r2=d2, c3=p0, han=solo, foo=bar
```



我希望你喜欢阅读我的有关Java 8并发的教程系列的第三部分。本教程的代码示例与许多其他Java 8代码片段一起[托管在](https://github.com/winterbe/java8-tutorial)GitHub上。欢迎您分享回购并自行尝试。



我希望你喜欢这篇文章。如果您有任何其他问题，请在下面的评论中向我发送您的反馈。你也应该[跟随我在](https://twitter.com/winterbe_)Twitter上更多的开发相关的东西！







