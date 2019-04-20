# 用不用lambda，这是一个问题 - 纯洁的微笑 - CSDN博客
2019年01月23日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：312
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/dtrPPyvQFTyrOyV2yZaSF5VCmVlkxIJoQk956GNcYJPVtYziaXtmtMVmw6sibBIBA6gquYmt4pdicMxv0VXAgy93Q/640?wx_fmt=jpeg)
Sun在2009年开启了代号为“dolphin”的工程，计划在JDK1.7中加入lambda表达式、虚拟机模块化支持、动态语言支持等新特性。但是由于深陷商业竞争，市值严重缩水，Sun无法在预计的时间内完成JDK的迭代。
2010年，Oracle收购Sun，执行新计划，放缓了更新的节奏，将lambada等功能顺延至1.8版本。2013年，JDK1.8版本正式发布，这是Java语言历史上浓墨重彩的一笔，而lambda（λ）则是其中最重要的特性。
### 什么是lambda表达式？
众所周知，Java是一门强大的面向对象的语言，在Java中，除了8种基本的数据类型，其他一切皆为对象， 而数据和对数据的操作，是依赖于对象的属性和方法。面向对象的三大核心：封装、继承、多态都是对数据的抽象，而lambda提供了一种对行为抽象的编程模型。
Java中将方法作为参数进行传递的方式被称为lambda表达式。
lambda表达式是Java对于函数式编程的温和转变，面向对象编程和函数式编程不是互相对立的，结合使用能够更加有效地帮助我们管理程序的复杂性。
### 为什么需要lambada？
我认为有以下两点：将外部迭代转换为内部迭代使得效率更高、通过将函数作为参数使得编码更加优雅，更易读。
随着摩尔定律的失效，个人电脑与专业服务器中开始配置多核处理器，为了使得程序的运行效率更高，开发者需要能够将任务分发到多个核心去执行，而Java Collection Framework默认的外部迭代方式将程序绑定在单个核心上运行，极大地降低了运算速度。
有时候为了达到某一目的，我们需要重复很多样板代码，这些代码会侵入业务逻辑，降低了可读性。
### 一个例子
假设现在有这么一个例子：
> 
一个列表中拥有多个元素，元素类型为Integer，现在我要将其中每个元素通过转换为Point(java.awt.Point)，最后得出所有Point中距离原点最远的那个。
传统的写法是：
- 
`    public static void main(String[] args) {`
- 
`        List<Integer> intList = Arrays.asList(21, 82, 33, 54);`
- 
`        List<Point> pointList = new ArrayList<>();`
- 
`        for (Integer i : intList) {`
- 
`            pointList.add(new Point(i % 3, i)); // 转换一`
- 
`        }`
- 
`        double max = Double.MIN_VALUE;`
- 
`        for (Point p : pointList) {`
- 
`            max = Math.max(p.distance(0, 0), max); // 转换二`
- 
`        }`
这样写似乎没有什么不对，是的，它非常正确。但是依然存在着一些问题。
首先，我们考虑一下这两个转换之间的关系。点Integer类型的A需要通过转换一变为Point类型，再由转换二将其变为Double类型，这没有问题。但是代码中的逻辑是当转换一全部完成后，再开始转换二。这似乎没有必要，并且程序将任务绑定在了一个核心上。假如转换任务是一系列复杂的计算，那么串行会导致效率低下。
此外，我们的目的是intList -> Double(距离最大值)，但是为了这一目的，在堆上为中间载体pointList分配了空间，假如这个List非常大，无疑会增加初始化以及GC消耗。而且，这段代码显得有点臃肿。
### 匿名内部类 和 lambda
我们首先使用匿名内部类对其进行改造，内部类是一种语法糖，修改之后虽然比之前简短了一些，但是仍然十分丑陋冗长，并且难以快速理解。
- 
`        List<Integer> intList = Arrays.asList(21, 82, 33, 54);`
- 
`        final double[] max = {Double.MIN_VALUE};`
- 
`        intList.forEach(new Consumer<Integer>() {`
- 
`            @Override`
- 
`            public void accept(Integer i) {`
- 
`                max[0] = Math.max(new Point(i % 3, i).distance(0, 0), max[0]);`
- 
`            }`
- 
`        });`
那么，我们应该如何对其进行改造呢？首先我们来看看哪些东西其实是不需要的。因为List的泛型是Integer，所以编译器可以从上下文推断出来intList.forEach所要操作的元素类型是Integer，其次Consumer是一个interface，准确地来说他是一个函数式接口，我们重写了它的accept方法，它只是用来接收元素，所以我们可以摒弃这种啰嗦的写法，利用编译器自身根据上下文的类型推断，来书写更为精简的lambda表达式：
- 
`        List<Integer> intList = Arrays.asList(21, 82, 33, 54);`
- 
`        final double[] max = {Double.MIN_VALUE};`
- 
`        intList.forEach(i -> {`
- 
`            max[0] = Math.max(new Point(i % 3, i).distance(0, 0), max[0]);`
- 
`        });`
### Stream
Stream是JDK1.8中新增的重要API，它可以帮助开发者操作对集合进行复杂的操作，并且能够自动依赖Fork/Join框架来实现自动并行化。
在传统方式中，我们可以将数据想象成一种“小球”，而集合则是存放它的“瓶子”。那么上面的例子应该是这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dtrPPyvQFTyrOyV2yZaSF5VCmVlkxIJoHicX5vVkVAkiam2icxbibo0TxGlWuwXOt0QUvwEWxqJ7ynsqtgicHf2pMRw/640?wx_fmt=png)
如果我们使用流呢？流提供了一个可选的有序值序列，而且无需为这些值提供任何存储。我们将其想象成一条河道，其中是流动的数据，我们可以在河道上设立“滤网”，通过这些滤网在数据“流动”的时候就对其进行转化。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dtrPPyvQFTyrOyV2yZaSF5VCmVlkxIJojfCXly60U1nt0f6QnY5jTP4IhUffyrQrKvl8UteNpBsRERciaxscsxQ/640?wx_fmt=png)
此时转化一/二都是对流进行操作，简洁了许多，代码如下：
- 
`        List<Integer> intList = Arrays.asList(21, 82, 33, 54);`
- 
`        OptionalDouble max = intList.stream()`
- 
`                            .map(i -> new Point(i % 3, i)) //操作一`
- 
`                            .mapToDouble(p -> p.distance(0, 0)) //操作二`
- 
`                            .max();`
如果我们想要并行化处理呢？只要将上述代码中的 `stream`换成 `parallelStream`，便可将intList中的元素转换为并行流再进行操作。
### 写在最后
Java中的lambda表达式是对函数式编程的支持，在处理集合等问题上有着诸多的优势，而且能够可见地提高代码的质量，理想并且简洁地解决并发问题，但是这并不代表函数式编程（FP）就优于面向对象编程（OOP）。
可以说，早期的Java是纯粹的OOP语言，因此很多不需要使用对象来传递消息的地方也被强行要求使用对象，这是被人诟病的因素之一，但是随着发展，现在的Java程序大量使用反射、lambda等技术，已经不是那个单纯OOP语言了。
具体使用哪种编程模式，需要关注问题本身，正如《人月神话》中广为流传的真理——没有银弹。
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
