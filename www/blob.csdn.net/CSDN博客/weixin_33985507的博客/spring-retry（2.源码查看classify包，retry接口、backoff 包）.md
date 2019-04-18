# spring-retry（2.源码查看classify包，retry接口、backoff 包） - weixin_33985507的博客 - CSDN博客
2018年01月24日 22:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
# classify
在spring-retry-××.jar的源码中，我们发现这个包里面还有一个和retry同级的classify包，显然它应该是retry需要用到，但是又不是包含的retry模型里面的东西。
![5384456-c55d95cb6a55ebc7.png](https://upload-images.jianshu.io/upload_images/5384456-c55d95cb6a55ebc7.png)
classify.png
classify包作为retry的辅助类，主要应用于RetryPolicy的canRetry()方法中，通过比较捕获的异常与定义的异常直接关系，决定是否符合重试条件，其包结构类图如下：
![5384456-dee964d7d59efeb7.png](https://upload-images.jianshu.io/upload_images/5384456-dee964d7d59efeb7.png)
Classifier.png
- Classifier<C, T> 接口：
是整个包的核心接口，定义了
```java
T classify(C classifiable);
```
把一个C类型对象，转换为T类型对象，其中T类型通常是枚举类型或者布尔类型这种可以直接比较结果的。
- ClassifierSupport<C, T> 类：
一个基础实现，引入默认值机制，无论要传入任何C类型对象，都返回默认的T类型对象。
- ClassifierAdapter<C, T> 类：
定义了两种方式的转换，一种直接Classifier，在需要转换时候调用，
一种传入通过识别一个目标类中@Classifier注解的方法，把它作为转换的实现类，在需要转换时候调用。
- SubclassClassifier<T, C>类：
首先要注意这里T和C对调写了，实现了能识别一个类和其子类都能被识别到，转换为目标类型对象的机制。这对于retry需要的异常类的转换十分重要，通常我们只需要定义某一类的异常重试策略，那么其子类异常也会同样应用到该策略，比如我们定义了数据库错误SQLException需要重试，实际运行可能抛出的是SQLTimeoutException，或者BatchUpdateException，它们就都会被捕获重试。
- BinaryExceptionClassifier类：
明确化了SubclassClassifier<T, C>的类型，其classify()方法把Throwable转换为Boolean。代码如下：
```java
@Override
    public Boolean classify(Throwable classifiable) {
        Boolean classified = super.classify(classifiable);
        if (!this.traverseCauses) {
            return classified;
        }
        /*
         * If the result is the default, we need to find out if it was by default
         * or so configured; if default, try the cause(es).
         */
        if (classified.equals(this.getDefault())) {
            Throwable cause = classifiable;
            do {
                if (this.getClassified().containsKey(cause.getClass())) {
                    return classified; // non-default classification
                }
                cause = cause.getCause();
                classified = super.classify(cause);
            }
            while (cause != null && classified.equals(this.getDefault()));
        }
        return classified;
    }
```
如果traverseCauses为false，就简单调用父类进行转换即可，如果为真，就必须一直找Throwable的Cause链条，直到找到匹配的转换。
P.S. Traverse: 穿过、横贯
- PatternMatcher和PatternMatchingClassifier<T> ：
能够把符合样式的字符串转换为T对象的转换器。
其核心方法为imatch()，对?和*进行了处理判断，判断输入的str是否符合某种样式pattern。
- BackToBackPatternClassifier<C, T> 类：
背对背映射组合，先吧C对象转换为string,然后再把string转换为T对象。
### retry顶级接口
```
retry顶级接口定义了核心的概念，其相互关系如下：
```
![5384456-bb1eddfa08e8f304.png](https://upload-images.jianshu.io/upload_images/5384456-bb1eddfa08e8f304.png)
retry.png
具体含义：请移步[spring-retry（1.概念和基本用法）](https://www.jianshu.com/p/58e753ca0151)，这里简要分析一些接口方法。
- RetryContext接口：
从图上可以看到，重试上下文处于核心位置，作为核心数据接口，储存了重试所需要的各类信息。
```java
RetryContext getParent();
```
```
从获取父上下文方法可知，它是一个链式结构。
```
- RetryPolicy接口：
```java
//判断一个上下文能否重试
    boolean canRetry(RetryContext context);
    //开启一个重试上下文环境
    RetryContext open(RetryContext parent);
    //关闭一个重试上下文环境
    void close(RetryContext context);
    //出现异常时候，把异常登记到上下文中
    void registerThrowable(RetryContext context, Throwable throwable);
```
从接口参数可以看出，策略都是根据上下文情况进行判断分析的。
- RetryOperations接口：
各种花式execute(),根据可配置的重试行为，进行方法的执行，其具体的实现就是核心类RetryTemplate
- RetryListener接口：
作为重试动作的监听器，给spring-retry加点料，用在统计机制上。监听3类动作：open()在开启操作之前，close()在关闭操作之后，onError()在出现错误时。
- RetryStatistics接口：
记录重试统计信息的接口。登记完成数、开始数、错误数、中止数、恢复数。
- RetryException及ExhaustedRetryException，TerminatedRetryException异常
定义了retry项目内部可能抛出的异常，RetryException是基类。
Exhausted：精疲力竭的
Terminated：终止的
### retry.backoff 包
```
该包定义了当出现异常，而又会重试的情况下的补偿机制（通常就是延时）。
```
![5384456-1b2d5f68a47c5755.png](https://upload-images.jianshu.io/upload_images/5384456-1b2d5f68a47c5755.png)
BackOff.png
- BackOffPolicy接口：
该包的核心接口，包含两个方法，意识生成一个当前补偿上下文环境，二是进行补偿动作。
```java
//根据重试上下文生成一个补偿上下文
    BackOffContext start(RetryContext context);
    //根据补偿上下文执行延迟操作，可能抛出中断异常
    void backOff(BackOffContext backOffContext) throws BackOffInterruptedException;
```
- Sleeper接口与ThreadWaitSleeper类：
真正的补偿动作具体执行器， ThreadWaitSleeper就是调用了Thread.sleep()方法进行延迟。
- StatelessBackOffPolicy抽象类：
其start方法返回null，也就是没有重试上下文，执行backOff时候调用的是无参数的doBackOff()。换句话说，代表具体补偿动作是固定的，并不依赖上下文环境。
- NoBackOffPolicy类：
最简单的默认策略，具体延迟为空操作，也就是不补偿，不延迟。
- SleepingBackOffPolicy接口：
有一个withSleeper()方法，传入一个Sleeper。
- UniformRandomBackOffPolicy类：
标准的随机延迟策略，给定最小值，最大值（默认为500L，1500L），会在这个区间里面随机进行补偿延迟。
7 FixedBackOffPolicy类：
标准的固定延迟策略，每次延迟固定时间（默认1000L）
- ExponentialBackOffPolicy类和ExponentialRandomBackOffPolicy：
这两个类都是SleepingBackOffPolicy的实现，内部用ThreadWaitSleeper延迟。实现的是延迟指数倍增的效果，区别是ExponentialBackOffPolicy是固定倍增，ExponentialRandomBackOffPolicy加入了随机性。
例如：
```java
* {@link ExponentialBackOffPolicy} 延迟序列为: [50, 100, 200, 400, 800]
 *
 * {@link ExponentialRandomBackOffPolicy} 延迟序列可能为： [76, 151, 304, 580, 901] 或者 [53, 190,
 * 267, 451, 815]
```
### retry.context 包
该包只有一个类RetryContextSupport，重试上下文的具体实现。
- 扩展AttributeAccessorSupport：内部有个linkedHashMap存储标准属性外的其他属性
- 有parent属性，在构造时候传入其父上下文，这样就维护了一个链表信息，方便后续查找。
- count和lastException是记录型属性，登记这个上下文的状态。
