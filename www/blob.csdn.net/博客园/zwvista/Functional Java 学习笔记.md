# Functional Java 学习笔记 - zwvista - 博客园

## [Functional Java 学习笔记](https://www.cnblogs.com/zwvista/p/7496796.html)

### Functional Java

Functional Java是一个在Java语言中实现函数型编程范式的类库。

从接口上看，该类库似乎与Haskell语言关系密切，接口的方法名很多来自该语言。

```
<!-- https://mvnrepository.com/artifact/org.functionaljava/functionaljava -->
<dependency>
    <groupId>org.functionaljava</groupId>
    <artifactId>functionaljava</artifactId>
    <version>4.7</version>
</dependency>
```

### Functional Java 与 Java 8

标准的Java 8 也引入了很多函数型编程范式的元素，比如Stream，lambda以及函数型接口。

但在功能上Java 8 远不如Functional Java丰富，使用上也受到一定的限制。

### 函数接口类型

### F0, F, F2, ..., F8

有返回值的函数接口类型。使用该函数接口需要调用接口的 f 方法。

F0没有参数，F有1个参数，F2有2个参数，...，F8有8个参数

F0有1个类型参数，F有2个类型参数，F2有3个类型参数，...，F8有9个类型参数

最后那个类型参数代表返回值类型。

比如 F<Integer, Boolean>的参数类型为Integer,返回值类型为Boolean。

```
import fj.F;
import fj.F2;
import fj.F3;
F<Integer, Integer> twice = n -> n * 2;
F2<Integer, Integer, Integer> f = (a, b) -> a + b;
F3<Integer, Integer, Integer, Integer> g = (a, b, c) -> a + b + c;
System.out.println(twice.f(6), f.f(6, 7), g.f(3, 4, 5)); // 12 13 12
```

### Effect0, Effect1, Effect2, ..., Effect8

没有返回值的函数接口类型。使用该函数接口需要调用接口的 f 方法。

Effect0没有（类型）参数，Effect1有1个（类型）参数，Effect2有2个（类型）参数，...，Effect8有8个（类型）参数

比如 Effect1<Integer>的参数类型为Integer,返回值类型为void。

```
import fj.function.Effect1;
Effect1<Integer> abc = n -> System.out.println(n);
abc.f(333);  // 333
```

### Try0, Try1, Try2, ..., Try8

带异常有返回值的函数接口类型。使用该函数接口需要调用接口的 f 方法。

Try0没有参数，Try1有1个参数，Try2有2个参数，...，Try8有8个参数

Try0有2个类型参数，Try1有3个类型参数，Try2有4个类型参数，...，Try8有10个类型参数

最后两个类型参数代表返回值类型和异常类型。

比如 Try1<Integer, Boolean, Exception>的参数类型为Integer,返回值类型为Boolean,异常类型为Exception。

### TryEffect0, TryEffect1, TryEffect2, ..., TryEffect8

带异常没有返回值的函数接口类型。使用该函数接口需要调用接口的 f 方法。

TryEffect0没有参数，TryEffect1有1个参数，TryEffect2有2个参数，...，TryEffect8有8个参数

TryEffect0有1个类型参数，TryEffect1有2个类型参数，TryEffect2有3个类型参数，...，TryEffect8有9个类型参数

最后那个类型参数代表异常类型。

比如 TryEffect1<Integer, Exception>的参数类型为Integer,返回值类型为void,异常类型为Exception。

### Primitive

提供在基本类型之间进行转换的 F 函数接口的简写形式。

```
import static fj.data.Array.array;
import static fj.Primitive.Integer_Double;
double d = Integer_Double.f(1);
// d = (double)1;
Double[] ds = array(1, 2, 3).map(Integer_Double).array(Double[].class);
```

### 函数接口对照（Functional Java 与 Java 8）
|FJ 接口|FJ 方法|Java 8 接口|Java 8 方法|
|----|----|----|----|
|Effect2<T, U>|f|BiConsumer<T, U>|accept|
|F2<T, U, R>|f|BiFunction<T, U, R>|apply|
|F2<T, T, T>|f|BinaryOperator<T>|apply|
|F2<T, U, Boolean>|f|BiPredicate<T, U>|test|
|F0<Boolean>|f|BooleanSupplier|getAsBoolean|

### 元组类型

### P1, P2, ..., P8, P

积（Product）类型（成员类型各异的元组类型）。调用接口的 _1, _2, ..., _8 方法可以读取元组的各个成员。

P1只有1个成员, P2有2个成员, ...，P8内有8个成员。

构建元组需要调用 P 类型的 p 方法以及 p1, p2, ..., p8 方法。

```
import fj.P;
import fj.P2;
P2<Integer, String> kv = P.p(1, "a");
System.out.println(kv);  // (1,a)
kv = P.<Integer, String>p2().f(2).f("b");
System.out.println(kv);  // (2,b)
```

### V2, ..., V8, V

矢量（Vector）类型（成员类型同一的元组类型）。调用接口的 _1, _2, ..., _8 方法可以读取元组的各个成员。

V2有2个成员, ...，V8内有8个成员。

构建元组需要调用 V 类型的 v 方法以及 v2, ..., v8 方法。

```
import fj.data.vector.V;
import fj.data.vector.V2;
V2<Integer> kv2 = V.v(1, 3);
System.out.println(kv2.p());  // (1,3)
kv2 = V.<Integer>v2().f(2, 4);
System.out.println(kv2.p());  // (2,4)
```

### 数据结构

### List

```
package fp;

import fj.Equal;
import fj.Ord;
import fj.Ordering;
import fj.P;
import fj.data.List;
import fj.data.Option;

import static fj.Equal.intEqual;
import static fj.Ord.intOrd;
import static fj.data.List.list;

public class FJList {

    static void f(Object o) { System.out.println(o); }
    public static void main(String[] args) {
        f(list(1, 1, 1).allEqual(intEqual)); // true
        f(list(1, 2).append(list(3))); // List(1,2,3)
        f(list(1, 2, 3).apply(list(i -> i + 3, i -> i * 2))); // List(4,5,6,2,4,6)
        Integer[] arr = list(1, 2, 3).array(Integer[].class); // 123
        f(List.asString(list('a', 'b', 'c'))); // abc
        f(list(1, 2, 3).bind(i -> list(i, i * 2))); // List(1,2,2,4,3,6)
        f(list(1, 2, 3).bind(list(4, 5, 6), x -> y -> x * y)); // List(4,5,6,8,10,12,12,15,18)
        f(list(1, 2, 3).bind(list(4, 5, 6), (x, y) -> x * y)); // List(4,5,6,8,10,12,12,15,18)
        // takeUntil + dropUntil
        f(list(1, 2, 3).breakk(i -> i % 2 == 0)); // (List(1),List(2,3))
        f(List.cons(3, list(1, 2))); // List(3,1,2)
        f(list(1, 2).cons(3)); // List(3,1,2)
        f(list(1, 2, 3).delete(2, intEqual)); // List(1,3)
        f(list(1, 2, 3, 4, 5).drop(2)); // List(3,4,5)
        f(list(1, 2, 3, 4, 5).dropWhile(i -> i < 3)); // List(3,4,5)
        f(list(1, 2, 3, 4, 5).elementIndex(intEqual, 3)); // Some(2)
        f(list(1, 2, 3, 4, 5).exists(i -> i % 3 == 1)); // true
        f(list(1, 2, 3, 4, 5).filter(i -> i % 2 == 1)); // List(1,3,5)
        f(list(1, 2, 3, 4, 5).find(i -> i / 3 == 1)); // Some(3)
        f(list(1, 2, 3, 4, 5).foldLeft(acc -> i -> acc + i - 1, 0)); // 10
        f(list(1, 2, 3, 4, 5).foldLeft((acc, i) -> acc + i - 1, 0)); // 10
        f(list(1, 2, 3, 4, 5).foldLeft1(acc -> i -> acc + i - 1)); // 11
        f(list(1, 2, 3, 4, 5).foldLeft1((acc, i) -> acc + i - 1)); // 11
        f(list(1, 2, 3, 4, 5).foldRight(acc -> i -> acc + i - 1, 0)); // 10
        f(list(1, 2, 3, 4, 5).foldRight((acc, i) -> acc + i - 1 , 0)); // 10
        f(list(1, 2, 3, 4, 5).forall(i -> i % 2 == 0)); // false
        list(1, 2, 3, 4, 5).foreachDoEffect(i -> System.out.print(i));System.out.println(); // 12345
        f(List.fromString("abcde")); // List(a,b,c,d,e)
        f(list(2, 2, 3, 3).group(Equal.equal(a -> b -> a == b))); // List(List(2,2),List(3,3))
        f(list(1, 2, 3, 4, 5).groupBy(i -> i % 2, intOrd)); // TreeMap((0: List(4,2)),(1: List(5,3,1)))
        f(list(1, 2, 3, 4, 5).groupBy(i -> i % 2, i -> i * 2, Ord.ord(a -> b -> Ordering.fromInt(a - b)))); // TreeMap((0: List(8,4)),(1: List(10,6,2)))
        f(list(1, 2, 3, 4, 5).head()); // 1
        f(list(1, 2, 3, 4, 5).headOption()); // Some(1)
        f(list(1, 2, 3, 4, 5).index(2)); // 3
        f(list(1, 2, 3, 4, 5).init()); // List(1,2,3,4)
        f(list(1, 2, 3, 4, 5).inits()); // List(List(),List(1),List(1,2),List(1,2,3),List(1,2,3,4),List(1,2,3,4,5))
        f(list(1, 2, 3, 4, 5).insertBy(a -> b -> Ordering.fromInt(a - b), 3)); // List(1,2,3,3,4,5)
        f(list(1, 2, 3).intercalate(list(list(9, 9), list(10, 10), list(11, 11)))); // List(9,9,1,2,3,10,10,1,2,3,11,11)
        f(list(1, 2, 3).intersperse(9)); // List(1,9,2,9,3)
        f(list(1, 2, 3).isEmpty()); // false
        f(list(1, 2, 3).isNotEmpty()); // true
        f(list(1, 2, 3).isPrefixOf(intEqual, list(1, 2, 3, 4, 5))); // true
        f(list(1).isSingle()); // true
        f(list(1, 2, 3).isSuffixOf(intEqual, list(0, 1, 2, 3))); // true
        f(List.iterableList(java.util.Arrays.asList(1, 2, 3))); // List(1,2,3)
        f(List.iterateWhile(i -> i + 1, i -> i < 10, 1)); // List(1,2,3,4,5,6,7,8,9)
        f(List.join(list(list(1, 2), list(3, 4)))); // List(1,2,3,4)
        f(list(1, 2, 3).last()); // 3
        f(list(1, 2, 3).length()); // 3
        f(List.<Integer, Integer, Integer>liftM2(a -> b -> a * b).f(list(1, 2, 3)).f(list(1, 2, 3))); // List(1,2,3,2,4,6,3,6,9)
        f(List.lookup(intEqual, list(P.p(1, 2), P.p(3, 4)), 1)); // Some(2)
        f(list(1, 2, 3).map(i -> i * 2)); // List(2,4,6)
        f(list(1, 2, 3).<Integer, Integer>mapM(a -> b -> a * b).f(3)); // List(3,6,9)
        f(list(1, 2, 3).mapMOption(i -> i % 2 == 0 ? Option.some(i / 2) : Option.none())); // None
        f(list(1, 2, 3).mapMOption(i -> i > 0 ? Option.some(i / 2) : Option.none())); // Some(List(1,2,3))
        f(list(1, 2, 3).maximum(intOrd)); // 3
        f(list(1, 2, 3).minimumOption(intOrd)); // Some(1)
        f(list(1, 2, 3).minus(intEqual, list(3, 4, 5))); // List(1,2)
        f(list(1, 2, 2, 2, 3).mode(intOrd)); // 2
        f(List.nil()); // List()
        f(list(1, 2, 2, 3, 3).nub()); // List(1,2,3)
        f(list().orHead(() -> -1)); // -1
        f(list().orTail(() -> list(1, 2))); // List(1,2)
        f(list(1, 2, 3, 4, 5).partition(i -> i % 2 == 0)); // (List(2,4),List(1,3,5))
        f(list(1, 2, 3, 4, 5).partition(3)); // List(List(1,2,3),List(4,5))
        f(List.range(1, 10)); // List(1,2,3,4,5,6,7,8,9)
        f(list(1, 2, 3, 4, 5).removeAll(i -> i % 2 == 0)); // List(1,3,5)
        f(List.replicate(3, 3)); // List(3,3,3)
        f(list(1, 2, 3, 4, 5).reverse()); // List(5,4,3,2,1)
        f(list(1, 2, 3).sequence(list('a', 'b'))); // List(a,b,a,b,a,b)
        f(List.single(3)); // List(3)
        f(list(1, 2, 3).snoc(4)); // List(1,2,3,4)
        f(list(3, 2, 1).sort(intOrd)); // List(1,2,3)
        // takeWhile + dropWhile
        f(list(1, 2, 3, 4, 5).span(i -> i < 3)); // (List(1,2),List(3,4,5))
        f(list(1, 2, 3, 4, 5).splitAt(3)); // (List(1,2,3),List(4,5))
        f(list(1, 2, 3, 4, 5).tail()); // List(2,3,4,5)
        f(list(1, 2, 3, 4, 5).tailOption()); // Some(List(2,3,4,5))
        f(list(1, 2, 3, 4, 5).tails()); // List(List(1,2,3,4,5),List(2,3,4,5),List(3,4,5),List(4,5),List(5),List())
        f(list(1, 2, 3, 4, 5).take(3)); // List(1,2,3)
        f(list(1, 2, 3, 4, 5).takeWhile(i -> i < 3)); // List(1,2)
        f(list(1, 2, 3, 4, 5).toArray()); // Array(1,2,3,4,5)
        f(list(1, 2, 3, 4, 5).toJavaList()); // [1, 2, 3, 4, 5]
        // traverse (\i -> [i, i + 1]) [1, 2, 3]
        f(list(1, 2, 3).traverseList(i -> list(i, i + 1))); // List(List(1,2,3),List(1,2,4),List(1,3,3),List(1,3,4),List(2,2,3),List(2,2,4),List(2,3,3),List(2,3,4))
        f(list(1, 2, 3).traverseOption(i -> i % 2 == 0 ? Option.some(i / 2) : Option.none())); // None
        f(list(2, 4, 6).traverseOption(i -> i % 2 == 0 ? Option.some(i / 2) : Option.none())); // Some(List(1,2,3))
        f(list(1, 2, 3).uncons((a, b) -> b.head(), 0)); // 2
        // unfoldr (\b -> if b == 0 then Nothing else Just (b, b-1)) 10
        f(List.unfold(b -> b == 0 ? Option.none() : Option.some(P.p(b, b - 1)), 10)); // List(10,9,8,7,6,5,4,3,2,1)
        f(List.unzip(list(P.p(1, 2), P.p(3, 4)))); // (List(1,3),List(2,4))
        f(list(1, 2, 3).zip(list('a', 'b', 'c'))); // List((1,a),(2,b),(3,c))
        f(list(1, 2, 3).zipIndex()); // List((1,0),(2,1),(3,2))
        f(list(1, 2, 3).zipWith(list(1, 2, 3), a -> b -> a * b)); // List(1,4,9)
        f(list(1, 2, 3).zipWith(list(1, 2, 3), (a, b) -> a * b)); // List(1,4,9)
    }

}
```

### Array

```
package fp;

import fj.P;
import fj.data.Array;

import static fj.Unit.unit;
import static fj.data.Array.array;
import static fj.data.Array.iterableArray;

public class FJArray {

    static void f(Object o) { System.out.println(o); }
    public static void main(String[] args) {
        f(array(1, 2).append(array(3))); // Array(1,2,3)
        f(array(1, 2, 3).apply(array(i -> i + 3, i -> i * 2))); // Array(4,5,6,2,4,6)
        f(array(new Integer[]{1, 2, 3})); // Array(1,2,3)
        f(array(1, 2, 3).bind(i -> array(i, i * 2))); // Array(1,2,2,4,3,6)
        f(array(1, 2, 3).bind(array(4, 5, 6), x -> y -> x * y)); // Array(4,5,6,8,10,12,12,15,18)
        f(array(1, 2, 3).bind(array(4, 5, 6), (x, y) -> x * y)); // Array(4,5,6,8,10,12,12,15,18)
        f(Array.empty()); // Array()
        f(array(1, 2, 3, 4, 5).exists(i -> i % 3 == 1)); // true
        f(array(1, 2, 3, 4, 5).filter(i -> i % 2 == 1)); // Array(1,3,5)
        f(array(1, 2, 3, 4, 5).find(i -> i / 3 == 1)); // Some(3)
        f(array(1, 2, 3, 4, 5).foldLeft(acc -> i -> acc + i - 1, 0)); // 10
        f(array(1, 2, 3, 4, 5).foldLeft((acc, i) -> acc + i - 1, 0)); // 10
        f(array(1, 2, 3, 4, 5).foldRight(acc -> i -> acc + i - 1, 0)); // 10
        f(array(1, 2, 3, 4, 5).foldRight((acc, i) -> acc + i - 1, 0)); // 10
        f(array(1, 2, 3, 4, 5).forall(i -> i % 2 == 0)); // false
        array(1, 2, 3, 4, 5).foreach(i -> {System.out.print(i); return unit();});System.out.println(); // 12345
        array(1, 2, 3, 4, 5).foreachDoEffect(i -> System.out.print(i));System.out.println(); // 12345
        f(array(1, 2, 3, 4, 5).get(3)); // 4
        f(array(1, 2, 3).isEmpty()); // false
        f(array(1, 2, 3).isNotEmpty()); // true
        f(iterableArray(java.util.Arrays.asList(1, 2, 3))); // Array(1,2,3)
        f(Array.join(array(array(1, 2), array(3, 4)))); // Array(1,2,3,4)
        f(array(1, 2, 3).length()); // 3
        f(array(1, 2, 3).map(i -> i * 2)); // Array(2,4,6)
        f(Array.range(1, 10)); // Array(1,2,3,4,5,6,7,8,9)
        f(array(1, 2, 3, 4, 5).reverse()); // Array(5,4,3,2,1)
        f(array(1, 2, 3, 4, 5).scanLeft(acc -> i -> acc + i - 1, 0)); // Array(0,1,3,6,10)
        f(array(1, 2, 3, 4, 5).scanLeft((acc, i) -> acc + i - 1, 0)); // Array(0,1,3,6,10)
        f(array(1, 2, 3, 4, 5).scanLeft1(acc -> i -> acc + i - 1)); // Array(1,2,4,7,11)
        f(array(1, 2, 3, 4, 5).scanLeft1((acc, i) -> acc + i - 1)); // Array(1,2,4,7,11)
        f(array(1, 2, 3, 4, 5).scanRight(acc -> i -> acc + i - 1, 0)); // Array(10,10,9,7,4)
        f(array(1, 2, 3, 4, 5).scanRight((acc, i) -> acc + i - 1, 0)); // Array(10,10,9,7,4)
        f(array(1, 2, 3, 4, 5).scanRight1(acc -> i -> acc + i - 1)); // Array(11,11,10,8,5)
        f(array(1, 2, 3, 4, 5).scanRight1((acc, i) -> acc + i - 1)); // Array(11,11,10,8,5)
        f(array(1, 2, 3).sequence(array('a', 'b'))); // Array(a,b,a,b,a,b)
        f(Array.single(3)); // Array(3)
        f(Array.unzip(array(P.p(1, 2), P.p(3, 4)))); // (Array(1,3),Array(2,4))
        f(array(1, 2, 3).zip(array('a', 'b', 'c'))); // Array((1,a),(2,b),(3,c))
        f(array(1, 2, 3).zipIndex()); // Array((1,0),(2,1),(3,2))
        f(array(1, 2, 3).zipWith(array(1, 2, 3), a -> b -> a * b)); // Array(1,4,9)
        f(array(1, 2, 3).zipWith(array(1, 2, 3), (a, b) -> a * b)); // Array(1,4,9)
    }

}
```

### Option

```
package fp;

import fj.data.List;
import fj.data.Option;
import fj.function.Effect1;

import static fj.Unit.unit;
import static fj.data.Option.some;

public class FJOption {

    static void f(Object o) { System.out.println(o); }
    public static void main(String[] args) {
        Effect1<Object> f = o -> System.out.println(o);
        f(some(3).apply(some(i -> i * 2))); // Some(6)
        f(some(3).bind(i -> some(i * 2))); // Some(6)
        f(some(3).bind(some(2), a -> b -> a * b)); // Some(6)
        f(some(3).bindProduct(some('a'))); // Some((3,a))
        f(some(3).exists(i -> i % 2 == 1)); // true
        f(some(2).exists(i -> i % 2 == 1)); // false
        f(Option.<Integer>none().exists(i -> i % 2 == 1)); // false
        f(some(3).filter(i -> i % 2 == 1)); // Some(3)
        f(some(2).filter(i -> i % 2 == 1)); // None
        f(Option.<Integer>none().filter(i -> i % 2 == 1)); // None
        f(some(3).forall(i -> i % 2 == 1)); // true
        f(some(2).forall(i -> i % 2 == 1)); // false
        f(Option.<Integer>none().forall(i -> i % 2 == 1)); // true
        some(3).foreach(i -> {System.out.print(i); return unit();});System.out.println(); // 3
        some(3).foreachDoEffect(i -> System.out.print(i));System.out.println(); // 3
        f(Option.fromNull(1)); // Some(1)
        f(Option.fromString("abc")); // Some(abc)
        f(Option.iif(true, 3)); // Some(3)
        f(Option.iif(false, 3)); // None
        f(Option.iif(i -> i % 2 == 1, 3)); // Some(3)
        f(some(3).isNone()); // false
        f(some(3).isSome()); // true
        f(Option.join(some(some(3)))); // Some(3)
        f(some(3).length()); // 1
        f(Option.<Integer>none().length()); // 0
        f(some(3).liftM2(some(2), (x, y) -> x * y)); // Some(6)
        f(some(3).map(i -> i * 3)); // Some(9)
        f(some(3).orElse(some(0))); // Some(3)
        f(Option.<Integer>none().orElse(some(0))); // Some(0)
        f(Option.<Integer>none().orSome(3)); // 3
        f(Option.sequence(List.list(some(1), some(2), some(3)))); // Some(List(1,2,3))
        f(some(3).sequence(some('a'))); // Some(a)
        f(Option.somes(List.list(some(1), some(2), some(3)))); // List(1,2,3)
        f(some(3).traverseList(i -> List.list(1, 2, 3))); // List(Some(1),Some(2),Some(3))
        f(some(3).traverseOption(i -> i % 2 == 1 ? some(i) : Option.none())); // Some(Some(3))
    }

}
```

### Map

```
package fp;

import fj.P;
import fj.data.HashMap;
import fj.data.List;
import fj.data.TreeMap;

import static fj.Ord.charOrd;
import static fj.Ord.intOrd;
import static fj.data.HashMap.arrayHashMap;
import static fj.data.TreeMap.treeMap;

public class FJMap {

    static void f(Object o) { System.out.println(o); }
    static void f2(Object o) { System.out.print(o); }
    public static void main(String[] args) {
        {
            HashMap<Integer, Character> m = arrayHashMap(P.p(1, 'a'), P.p(2, 'b')); f(m.toMap()); // {1=a, 2=b}
            m.delete(1); f(m.toMap()); // {2=b}
            m.clear(); f(m.toMap()); // {}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).contains(1)); // true
            arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).foreachDoEffect(FJMap::f2); f(""); // (1,a)(2,b)
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).get(1)); // Some(a)
            m = arrayHashMap(P.p(1, 'a'), P.p(2, 'b')); f2(m.getDelete(1)); f(m.toMap()); // Some(a){2=b}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).isEmpty()); // false
            f(arrayHashMap().isEmpty()); // true
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).keys()); // List(1,2)
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).map(i -> i + 1, c -> (char)(c + 1)).toMap()); // {2=b, 3=c}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).map(kv -> P.p(kv._1() + 1, (char)(kv._2() + 1))).toMap()); // {2=b, 3=c}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).mapKeys(i -> i + 1).toMap()); // {2=a, 3=b}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).mapValues(c -> (char)(c + 1)).toMap()); // {1=b, 2=c}
            m = arrayHashMap(P.p(1, 'a'), P.p(2, 'b')); m.set(3, 'c'); f(m.toMap()); // {1=a, 2=b, 3=c}
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).size()); // 2
            f(arrayHashMap().size()); // 0
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).toArray()); // Array((1,a),(2,b))
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).toList()); // List((1,a),(2,b))
            f(arrayHashMap(P.p(1, 'a'), P.p(2, 'b')).values()); // List(a,b)
        }
        {
            TreeMap<Integer, Character> m = treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')); f(m.toMutableMap()); // {1=a, 2=b}
            m.delete(1); f(m.toMutableMap()); // {1=a, 2=b}
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).contains(1)); // true
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).get(1)); // Some(a)
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).isEmpty()); // false
            f(treeMap(intOrd).isEmpty()); // true
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).keys()); // List(1,2)
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).map(c -> (char)(c + 1))); // TreeMap((1: b),(2: c))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).max()); // Some((2,b))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).minKey()); // Some(1)
            m = treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')); m.set(3, 'c'); f(m.toMutableMap()); // {1=a, 2=b}
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b'), P.p(3, 'c')).split(charOrd, 2)); // (Set(a),Some(b),Set(c))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b'), P.p(3, 'c')).splitLookup(2)); // (TreeMap((1: a)),Some(b),TreeMap((3: c)))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).size()); // 2
            f(treeMap(intOrd).size()); // 0
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).toList()); // List((1,a),(2,b))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).toListReverse()); // List((2,b),(1,a))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).union(List.list(P.p(3, 'c')))); // TreeMap((1: a),(2: b),(3: c))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).union(treeMap(intOrd, P.p(3, 'c')))); // TreeMap((1: a),(2: b),(3: c))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).update(2, c -> (char)(c + 1))); // (true,TreeMap((1: a),(2: c)))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).update(3, c -> (char)(c + 1), 'z')); // TreeMap((1: a),(2: b),(3: z))
            f(treeMap(intOrd, P.p(1, 'a'), P.p(2, 'b')).values()); // List(a,b)
        }
    }

}
```

### Set

```
package fp;

import fj.Monoid;
import fj.Ord;
import fj.Ordering;
import fj.data.HashSet;
import fj.data.Set;

import static fj.Ord.intOrd;
import static fj.data.HashSet.hashSet;
import static fj.data.HashSet.iterableHashSet;
import static fj.data.Set.iterableSet;
import static fj.data.Set.set;

public class FJSet {

    static void f(Object o) { System.out.println(o); }
    static void f2(Object o) { System.out.println(o); }
    public static void main(String[] args) {
        {
            Set<Integer> s = set(intOrd, 1, 2); f(s.toJavaSet()); // [1, 2]
            f(s.bind(intOrd, a -> set(intOrd, a, a * 2))); // Set(1,2,4)
            f(s.delete(1)); // true
            f(s.size()); // 1
            f(Set.empty(intOrd).toJavaSet()); // []
            f(set(intOrd, 1, 2, 3, 4).filter(a -> a % 2 == 0)); // Set(2,4)
            f(set(intOrd, 1, 2, 3, 4).foldMap(a -> a, Monoid.intAdditionMonoid)); // 10
            f(set(intOrd, 1, 2, 3, 4).foldMapRight(a -> a, Monoid.intAdditionMonoid)); // 10
            f(set(intOrd, 1, 2).insert(3).toJavaHashSet()); // [1, 2, 3]
            f(set(intOrd, 1, 2, 3).intersect(set(intOrd, 3, 4, 5))); // Set(3)
            f(set(intOrd).isEmpty()); // true
            f(iterableSet(intOrd, java.util.Arrays.asList(1, 2, 3))); // Set(1,2,3)
            f(Set.join(intOrd, set(Ord.ord(a -> b -> Ordering.fromInt(a.size() - b.size())), set(intOrd, 1, 2, 3), set(intOrd, 3, 4, 5)))); // Set(3,4,5)
            f(set(intOrd, 1, 2, 3, 4).lookup(3)); // Some(3)
            f(set(intOrd, 1, 2, 3, 4).lookupGE(3)); // Some(3)
            f(set(intOrd, 1, 2, 3, 4).lookupGT(3)); // Some(4)
            f(set(intOrd, 1, 2, 3, 4).lookupLE(3)); // Some(3)
            f(set(intOrd, 1, 2, 3, 4).lookupLT(3)); // Some(2)
            f(set(intOrd, 1, 2, 3).map(intOrd, a -> a + 1)); // Set(2,3,4)
            f(set(intOrd, 1, 2, 3).max());
            f(set(intOrd, 1, 2, 3).member(3));
            f(set(intOrd, 1, 2, 3).minus(set(intOrd, 3, 4, 5)));
            f(set(intOrd, 1, 2).isEmpty()); // false
            f(Set.single(intOrd, 3)); // Set(3)
            f(set(intOrd, 1, 2, 3).size()); // 3
            f(set(intOrd, 1, 2, 3).split(2)); // (Set(1),Some(2),Set(3))
            f(set(intOrd, 1, 2).subsetOf(set(intOrd, 1, 2, 3))); // true
            f(set(intOrd, 1, 2).toList()); // List(1,2)
            f(set(intOrd, 1, 2).toListReverse()); // List(2,1)
            f(set(intOrd, 1, 2).toStreamReverse().toList()); // List(2,1)
            f(set(intOrd, 1, 2, 3).union(set(intOrd, 3, 4, 5))); // Set(1,2,3,4,5)
            f(set(intOrd, 1, 2, 3).update(2, a -> a + 1)); // (true,Set(1,3))
        }
        {
            HashSet<Integer> s = hashSet(1, 2); f(s.toJavaSet()); // [1, 2]
            f(s.contains(1)); // true
            f(s.delete(1)); // true
            s.clear(); f(s.toJavaSet()); // []
            f(iterableHashSet(java.util.Arrays.asList(1, 2, 3)).toJavaSet()); // [1, 2, 3]
            s.set(3); f(s.toJavaSet()); // [3]
            f(s.size()); // 1
            f(HashSet.empty().toJavaSet()); // []
            f(hashSet(1, 2).isEmpty()); // false
            f(hashSet().isEmpty()); // true
            f(hashSet(1, 2).toList()); // List(1,2)
        }
    }

}
```

### Seq

```
package fp;

import fj.data.Seq;

import static fj.data.Seq.seq;

public class FJSeq {

    static void f(Object o) { System.out.println(o); }
    public static void main(String[] args) {
        f(seq(1, 2).append(seq(3))); // Seq(1,2,3)
        f(seq(1, 2).cons(3)); // Seq(3,1,2)
        f(seq(1, 2, 3, 4, 5).delete(2)); // Seq(1,2,4,5)
        f(seq(1, 2, 3, 4, 5).drop(2)); // Seq(3,4,5)
        f(Seq.empty()); // Seq()
        f(seq(1, 2, 3, 4, 5).filter(i -> i % 2 == 1)); // Seq(1,3,5)
        f(seq(1, 2, 3, 4, 5).foldLeft((acc, i) -> acc + i - 1, 0)); // 10
        f(seq(1, 2, 3, 4, 5).foldRight((acc, i) -> acc + i - 1, 0)); // 10
        f(seq(1, 2, 3, 4, 5).head()); // 1
        f(seq(1, 2, 3, 4, 5).headOption()); // Some(1)
        f(seq(1, 2, 3, 4, 5).index(2)); // 3
        f(seq(1, 2, 3, 4, 5).init()); // Seq(1,2,3,4)
        f(seq(1, 2, 3, 4, 5).insert(3, 6)); // Seq(1,2,3,6,4,5)
        f(seq(1, 2, 3).isEmpty()); // false
        f(seq(1, 2, 3).isNotEmpty()); // true
        f(seq(1, 2, 3, 4, 5).last()); // 5
        f(seq(1, 2, 3, 4, 5).length()); // 5
        f(seq(1, 2, 3).map(i -> i * 2)); // Seq(2,4,6)
        f(Seq.single(3)); // Seq(3)
        f(seq(1, 2, 3).snoc(4)); // Seq(1,2,3,4)
        f(seq(1, 2, 3).split(1)); // (Seq(1),Seq(2,3))
        f(seq(1, 2, 3, 4, 5).tail()); // Seq(2,3,4,5)
        f(seq(1, 2, 3, 4, 5).take(3)); // Seq(1,2,3)
        f(seq(1, 2, 3, 4, 5).toList()); // List(1,2,3,4,5)
        f(seq(1, 2, 3, 4, 5).toJavaList()); // [1, 2, 3, 4, 5]
        f(seq(1, 2, 3, 4, 5).update(2, 6)); // Seq(1,2,6,4,5)
    }

}
```


