# JDK8 Stream API中Collectors中toMap方法的问题以及解决方案 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [theboboy](http://www.jobbole.com/members/theboboy) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
使用Collectors.toMap方法时的两个问题：
1、当key重复时，会抛出异常：java.lang.IllegalStateException: Duplicate key **
2、当value为null时，会抛出异常：java.lang.NullPointerException
首先这个不是JDK的bug，只是不符合我们的预期。
大家通常希望这个方法是可以容错的，遇到重复的key就使用后者替换，而且HashMap的value可以是null。
下面举个小例子。
首先是元素类Emp：

Java
```
public class Emp {
	private Integer id;
	private String name;
	public Emp(Integer id, String name) {
		super();
		this.id = id;
		this.name = name;
		System.out.println(this);
	}
	public Integer getId() {
		return id;
	}
	public String getName() {
		return name;
	}
	@Override
	public String toString() {
		return "id: " + id + ", name: " + name;
	}
}
```
然后是测试类：

Java
```
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;
public class Test {
	public static void main(String[] args) {
		Emp[] emps = IntStream.range(0, 10).mapToObj(x -> new Emp(x % 3, "name" + x))
				.toArray(Emp[]::new);
		Map<Integer, String> map = Stream.of(emps).collect(Collectors.toMap(Emp::getId, Emp::getName));
		System.out.println(map);
	}
}
```
生成10个对象，其中id只能为0，1，2，用id做key，name做value，运行结果如下：


```
id: 0, name: name0
id: 1, name: name1
id: 2, name: name2
id: 0, name: name3
id: 1, name: name4
id: 2, name: name5
id: 0, name: name6
id: 1, name: name7
id: 2, name: name8
id: 0, name: name9
Exception in thread "main" java.lang.IllegalStateException: Duplicate key name0
	at java.util.stream.Collectors.lambda$throwingMerger$0(Collectors.java:133)
	at java.util.HashMap.merge(HashMap.java:1253)
	at java.util.stream.Collectors.lambda$toMap$58(Collectors.java:1320)
	at java.util.stream.ReduceOps$3ReducingSink.accept(ReduceOps.java:169)
	at java.util.Spliterators$ArraySpliterator.forEachRemaining(Spliterators.java:948)
	at java.util.stream.AbstractPipeline.copyInto(AbstractPipeline.java:481)
	at java.util.stream.AbstractPipeline.wrapAndCopyInto(AbstractPipeline.java:471)
	at java.util.stream.ReduceOps$ReduceOp.evaluateSequential(ReduceOps.java:708)
	at java.util.stream.AbstractPipeline.evaluate(AbstractPipeline.java:234)
	at java.util.stream.ReferencePipeline.collect(ReferencePipeline.java:499)
	at Test.main(Test.java:11)
```
提示key重复了。
接下来看另一种情况：


```
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;
public class Test {
	public static void main(String[] args) {
		Emp[] emps = IntStream.range(0, 10).mapToObj(x -> new Emp(x, x % 5 == 0 ? null : "name" + x))
				.toArray(Emp[]::new);
		Map<Integer, String> map = Stream.of(emps).collect(Collectors.toMap(Emp::getId, Emp::getName));
		System.out.println(map);
	}
}
```
这个是让emps数组中出现两个value为null的元素，运行结果如下：


```
id: 0, name: null
id: 1, name: name1
id: 2, name: name2
id: 3, name: name3
id: 4, name: name4
id: 5, name: null
id: 6, name: name6
id: 7, name: name7
id: 8, name: name8
id: 9, name: name9
Exception in thread "main" java.lang.NullPointerException
	at java.util.HashMap.merge(HashMap.java:1224)
	at java.util.stream.Collectors.lambda$toMap$58(Collectors.java:1320)
	at java.util.stream.ReduceOps$3ReducingSink.accept(ReduceOps.java:169)
	at java.util.Spliterators$ArraySpliterator.forEachRemaining(Spliterators.java:948)
	at java.util.stream.AbstractPipeline.copyInto(AbstractPipeline.java:481)
	at java.util.stream.AbstractPipeline.wrapAndCopyInto(AbstractPipeline.java:471)
	at java.util.stream.ReduceOps$ReduceOp.evaluateSequential(ReduceOps.java:708)
	at java.util.stream.AbstractPipeline.evaluate(AbstractPipeline.java:234)
	at java.util.stream.ReferencePipeline.collect(ReferencePipeline.java:499)
	at Test.main(Test.java:11)
```
提示value是null。
这个是因为调用了HashMap的merge方法，方法不接受这种数据。因此需要手动实现，实现方式也很简单，只要调用map的put方法就可以了。

Java
```
import java.util.Collections;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.function.BinaryOperator;
import java.util.function.Function;
import java.util.function.Supplier;
import java.util.stream.Collector;
public class ForceToMapCollector<T, K, V> implements Collector<T, Map<K, V>, Map<K, V>> {
	private Function<? super T, ? extends K> keyMapper;
	private Function<? super T, ? extends V> valueMapper;
	public ForceToMapCollector(Function<? super T, ? extends K> keyMapper,
			Function<? super T, ? extends V> valueMapper) {
		super();
		this.keyMapper = keyMapper;
		this.valueMapper = valueMapper;
	}
	@Override
	public BiConsumer<Map<K, V>, T> accumulator() {
		return (map, element) -> map.put(keyMapper.apply(element), valueMapper.apply(element));
	}
	@Override
	public Supplier<Map<K, V>> supplier() {
		return HashMap::new;
	}
	@Override
	public BinaryOperator<Map<K, V>> combiner() {
		return null;
	}
	@Override
	public Function<Map<K, V>, Map<K, V>> finisher() {
		return null;
	}
	@Override
	public Set<Characteristics> characteristics() {
		return Collections.unmodifiableSet(EnumSet.of(Collector.Characteristics.IDENTITY_FINISH));
	}
}
```


Java
```
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collector;
public final class MyCollectors {
	public static <T, K, V> Collector<T, ?, Map<K, V>> toMap(Function<T, K> f1, Function<T, V> f2) {
		return new ForceToMapCollector<T, K, V>(f1, f2);
	}
}
```
测试类如下：

Java
```
import java.util.Map;
import java.util.stream.IntStream;
import java.util.stream.Stream;
public class Test {
	public static void main(String[] args) {
		Emp[] emps = IntStream.range(0, 10).mapToObj(x -> new Emp(x % 3, x % 5 == 0 ? null : "name" + x))
				.toArray(Emp[]::new);
		Map<Integer, String> map = Stream.of(emps).collect(MyCollectors.toMap(Emp::getId, Emp::getName));
		System.out.println(map);
	}
}
```
包含了上面两种可能产生异常的情况，运行结果如下：


```
id: 0, name: null
id: 1, name: name1
id: 2, name: name2
id: 0, name: name3
id: 1, name: name4
id: 2, name: null
id: 0, name: name6
id: 1, name: name7
id: 2, name: name8
id: 0, name: name9
{0=name9, 1=name7, 2=name8}
```
解决了上面的问题，满足了容错需求。
由于Stream API用的不是很广泛，如果网友们有更好的实现方案，希望告知，多谢！
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/eac008a5ad6c8763c7b5d84ac3560bae.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/d1883dd90740e55db921de7eb1e42048.jpg)
