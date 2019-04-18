# java8 Stream流操作介绍 - z69183787的专栏 - CSDN博客
2019年02月11日 11:47:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：68
[https://blog.csdn.net/sf_cyl/article/details/51900701](https://blog.csdn.net/sf_cyl/article/details/51900701)
流操作在本人接触到的部分，都是处理集合容器Collection，加入流操作主要是为了函数式编程，在很大程度上可以简化代码
简洁的代码处理复杂逻辑这是每个程序猿的追求，废话不多说，开始介绍
使用Stream基本步骤
1. 创建Stream；
2. 转换Stream，每次转换原有Stream对象不改变，返回一个新的Stream对象（**可以有多次转换**）；
3. 对Stream进行聚合（Reduce）操作，获取想要的结果；
Stream 创建
```java
//Lists是Guava中的一个工具类
List<Integer> nums = Lists.newArrayList(1,null,3,4,null,6);
nums.stream().filter(num -> num != null).count();
/**
of方法：有两个overload方法，一个接受变长参数，一个接口单一值
*/
Stream<Integer> integerStream = Stream.of(1, 2, 3, 5);
Stream<String> stringStream = Stream.of("taobao");
/**
generator方法：生成一个无限长度的Stream，其元素的生成是通过给定的Supplier
*/
Stream.generate(new Supplier<Double>() {
       @Override
       public Double get() {
        return Math.random();
    }
});
Stream.generate(() -> Math.random());
Stream.generate(Math::random);
   
/**
iterate方法：也是生成无限长度的Stream，和generator不同的是，其元素的生成是重复对给定的种子值(seed)
调用用户指定函数来生成的。其中包含的元素可以认为是：seed
*/
Stream.iterate(1, item -> item + 1).limit(10).forEach(System.out::println);
```
所有集合类都可以直接调用stream()方法返回一个Stream对象
这个在本文的第一个例子中就展示了从List对象获取其对应的Stream对象，如果查看Java doc就可以发现Collection接口有一个stream方法，所以其所有子类都都可以获取对应的Stream对象。
```java
public interface Collection<E> extends Iterable<E> {
    //其他方法省略
    default Stream<E> stream() {
        return StreamSupport.stream(spliterator(), false);
    }
}
```
转换Stream
转化Stream方法有很处理方法，此处全部介绍，介绍几种，其余方法的使用都差不多
1. distinct: 对于Stream中包含的元素进行去重操作（去重逻辑依赖元素的equals方法），新生成的Stream中没有重复的元素；
```java
List<String> list = Arrays.asList("aa", "bb", "cc", "a", "b", "c", "aa", "ab", "cc", "bb", "bc");
list = list.stream().distinct().collect(Collectors.toList()); // [aa, bb, cc, a, b, c, ab, bc]
```
2. filter: 对于Stream中包含的元素使用给定的过滤函数进行过滤操作，新生成的Stream只包含符合条件的元素
```java
List<String> list = Arrays.asList("aa", "bb", "cc", "a", "b", "c","a", "aa", "ab", "cc", "bb", "bc");
list.stream().filter(e -> e.length()>=2).forEach(e -> System.out.print(e + ","));
// aa,bb,cc,aa,ab,cc,bb,bc,
```
3. map: 对于Stream中包含的元素使用给定的转换函数进行转换操作，新生成的Stream只包含转换生成的元素。这个方法有三个对于原始类型的变种方法，分别是：mapToInt，mapToLong和mapToDouble。这三个方法也比较好理解，比如mapToInt就是把原始Stream转换成一个新的Stream，这个新生成的Stream中的元素都是int类型。之所以会有这样三个变种方法，可以免除自动装箱/拆箱的额外消耗；
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 8, 9, 6, 4, 2, 3, 7, 6);
integerList.stream().map(var -> { var ++; var += 2; return var; }).forEach(System.out::print);
DoubleStream doubleStream = integerList.stream().mapToDouble((value) -> value * 1.0);
```
4. flatMap：和map类似，不同的是其每个元素转换得到的是Stream对象，会把子Stream中的元素压缩到父集合中；
```java
List<Integer> together = Stream.of(asList(1, 2),asList(3, 4)).flatMap(numbers -> numbers.stream()).collect(toList());
```
将两个队列一起处理流操作的时候，可以利用此方法将其转化为一个流
上部分的代码可以简化为
```java
List<Integer> together = Stream.of(Arrays.asList(1, 2),Arrays.asList(3, 4))
                .flatMap(Collection::stream).collect(Collectors.toList());
```
5. peek: 生成一个包含原Stream的所有元素的新Stream，同时会提供一个消费函数（Consumer实例），新Stream每个元素被消费的时候都会执行给定的消费函数；
此方法不会更改流中元素形式，主要用于调试或输出。
```java
list.stream().distinct().peek(e -> System.out.println(e.length())).count();
```
在peek()方法中使用lambda 表达式的时候，返回值只能为void类型 
方法暂时只介绍这么几个其余还有 limit()  skip()   forEach()等
聚合（Reduce）操作
下面会分两部分来介绍汇聚操作：
1. 可变汇聚：把输入的元素们累积到一个可变的容器中，比如Collection或者StringBuilder；
```java
List<Integer> nums = Lists.newArrayList(1,1,null,2,3,4,null,5,6,7,8,9,10);
       List<Integer> numsWithoutNull = nums.stream().filter(num -> num != null).
               collect(() -> new ArrayList<Integer>(),
                       (list, item) -> list.add(item),
                       (list1, list2) -> list1.addAll(list2));
```
上面这段代码就是对一个元素是Integer类型的List，先过滤掉全部的null，然后把剩下的元素收集到一个新的List中。进一步看一下collect方法的三个参数，都是lambda形式的函数（*上面的代码可以使用方法引用来简化，留给读者自己去思考*）。
第一个函数生成一个新的ArrayList实例；
第二个函数接受两个参数，第一个是前面生成的ArrayList对象，二个是stream中包含的元素，函数体就是把stream中的元素加入ArrayList对象中。第二个函数被反复调用直到原stream的元素被消费完毕；
第三个函数也是接受两个参数，这两个都是ArrayList类型的，函数体就是把第二个ArrayList全部加入到第一个中；
但是上面的collect方法调用也有点太复杂了，没关系！我们来看一下collect方法另外一个override的版本，其依赖
<R, A> R collect(Collector<? super T, A, R> collector);
Java8还给我们提供了Collector的工具类–[Collectors]其中已经定义了一些静态工厂方法，比如：Collectors.toCollection()收集到Collection中, Collectors.toList()收集到List中和Collectors.toSet()收集到Set中。这样的静态方法还有很多，这里就不一一介绍了，大家可以直接去看JavaDoc。下面看看使用Collectors对于代码的简化：
```java
List<Integer> numsWithoutNull = nums.stream().filter(num -> num != null).
                collect(Collectors.toList());
```
2. 其他汇聚：除去可变汇聚剩下的，一般都不是通过反复修改某个可变对象，而是通过把前一次的汇聚结果当成下一次的入参，反复如此。比如reduce，count，allMatch；
reduce方法：reduce方法非常的通用，后面介绍的count，sum等都可以使用其实现。reduce方法有三个override的方法，本文介绍两个最常用的，最后一个留给读者自己学习。先来看reduce方法的第一种形式，其方法定义如下：
```java
Optional<T> reduce(BinaryOperator<T> accumulator);
```
接受一个BinaryOperator类型的参数，在使用的时候我们可以用lambda表达式来。
```java
List<Integer> ints = Lists.newArrayList(1,2,3,4,5,6,7,8,9,10);
System.out.println("ints sum is:" + ints.stream().reduce((sum, item) -> sum + item).get());
```
可以看到reduce方法接受一个函数，这个函数有两个参数，第一个参数是上次函数执行的返回值（也称为中间结果），第二个参数是stream中的元素，这个函数把这两个值相加，得到的和会被赋值给下次执行这个函数的第一个参数。要注意的是：**第一次执行的时候第一个参数的值是Stream的第一个元素，第二个参数是Stream的第二个元素**。这个方法返回值类型是Optional，这是Java8防止出现NPE的一种可行方法，后面的文章会详细介绍，这里就简单的认为是一个容器，其中可能会包含0个或者1个对象。
reduce方法还有一个很常用的变种：
```java
T reduce(T identity, BinaryOperator<T> accumulator);
```
这个定义上上面已经介绍过的基本一致，不同的是：它允许用户提供一个循环计算的初始值，如果Stream为空，就直接返回该值。而且这个方法不会返回Optional，因为其不会出现null值。下面直接给出例子，就不再做说明了。
```java
List<Integer> ints = Lists.newArrayList(1,2,3,4,5,6,7,8,9,10);
System.out.println("ints sum is:" + ints.stream().reduce(0, (sum, item) -> sum + item));
```
count方法：获取Stream中元素的个数。比较简单，这里就直接给出例子，不做解释了。
```java
List<Integer> ints = Lists.newArrayList(1,2,3,4,5,6,7,8,9,10);
System.out.println("ints sum is:" + ints.stream().count());
```
– 搜索相关
– allMatch：是不是Stream中的所有元素都满足给定的匹配条件
– anyMatch：Stream中是否存在任何一个元素满足匹配条件
– findFirst: 返回Stream中的第一个元素，如果Stream为空，返回空Optional
– noneMatch：是不是Stream中的所有元素都不满足给定的匹配条件
– max和min：使用给定的比较器（Operator），返回Stream中的最大|最小值
下面给出allMatch和max的例子，剩下的方法读者当成练习。
```java
List<Integer> ints = Lists.newArrayList(1,2,3,4,5,6,7,8,9,10);
System.out.println(ints.stream().allMatch(item -> item < 100));
ints.stream().max((o1, o2) -> o1.compareTo(o2)).ifPresent(System.out::println);
```
