# Java 8 peek() 方法的一些疑惑 - z69183787的专栏 - CSDN博客
2018年12月12日 19:15:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：271
`Stream.of("one", "two", "three", "four").peek(e -> System.out.println(e));`
这样不会有任何的输出；
`Stream.of("one", "two", "three", "four").peek(e -> System.out.println(e)).collect(Collectors.toList());`
这样输出:
one
two
three
four
```
Stream.of("one", "two", "three", "four")
    .peek(e -> System.out.println("Peeked value: " + e))
    .map(String::toUpperCase)
    .peek(e -> System.out.println("Mapped value: " + e))
    .collect(Collectors.toList());
```
这样输出：
Peeked value: one
Mapped value: ONE
Peeked value: two
Mapped value: TWO
Peeked value: three
Mapped value: THREE
Peeked value: four
Mapped value: FOUR
知道 peek() 方法接受一个实现了 consumer 接口的类，但是实在是搞不清楚到底它是如何运行的。
------------------------------------------------------------------------------------------------------------------------------------------------
peek不是终结操作，其返回值还是Stream，你这种情况应该用forEach
处理流的方法分为惰性求值和及早求值（这两个概念不在此展开了），对流的处理通常包括一系列惰性求知加上一个及早求值；流遇到及早求值的时候，才会真正去遍历和执行
java8流中所有的操作都是蓄而不发的，只有执行foreach，collect等终结操作时，流的操作才会执行。
而且流内部会自动进行优化，只要得到想要的解决就不会继续进行计算了。
peek是个中间操作，它提供了一种对流中所有元素操作的方法，而不会把这个流消费掉（foreach会把流消费掉），然后你可以继续对流进行其他操作。
stream 分 中间操作 和 终端操作, (foreach, collect count)属于终端操作;
peek: 生成一个包含原Stream的所有元素的新Stream，同时会提供一个消费函数（Consumer实例），新Stream每个元素被消费的时候都会执行给定的消费函数；
