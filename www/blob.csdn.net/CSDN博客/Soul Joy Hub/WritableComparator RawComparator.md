# WritableComparator RawComparator - Soul Joy Hub - CSDN博客

2016年10月11日 19:44:48[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：333



转自：http://blog.csdn.net/maixia24/article/details/16964655

IntWritable 实现了WritableComparable 接口, 它是Writable 和 java.lang.Comparable 接口的子类

package org.apache.hadoop.io;

public interface WritableComparable<T> extends Writable, Comparable<T> {

}

数据类型的比较在MapReduce中式及其重要的, Mapreduce中有一个排序阶段，key和其他的key相比较。 针对此，Hadoop 提供的一个优化是 RawComparator，它继承了Java’s Comparator:

package org.apache.hadoop.io;

import java.util.Comparator;

public interface RawComparator<T> extends Comparator<T> {

public int compare(byte[] b1, int s1, int l1, byte[] b2, int s2, int l2);

}

这个接口允许实现它来在字节流的层面比较，而不用反序列化为对象，因此，可以避免过多的对象的创建。

例如，IntWritables的comparator 类实现了compare()方法，这是通过从每一个byte数组b1,b2直接读取整数，并且直接比较，比较的时候，从开始的位置(s1和s2)，并且长度是(l1和l2)

WritableComparator 是RawComparator 的一个通用实现。它提供了两个主要的功能。第一，它提供了一个默认的compare()方法的实现，从字节流反序列化为对象，使用对象的compare()方法进行比较。第二，作为RawComparator 的一个工厂类，例如，获取一个IntWritable的comparator ，如下：

RawComparator<IntWritable> comparator = WritableComparator.get(IntWritable.class);

comparator 可以被用来比较两个IntWritable 对象：

IntWritable w1 = new IntWritable(163);

IntWritable w2 = new IntWritable(67);

assertThat(comparator.compare(w1, w2), greaterThan(0));

或者它的序列化描述是：

byte[] b1 = serialize(w1);

byte[] b2 = serialize(w2);

assertThat(comparator.compare(b1, 0, b1.length, b2, 0, b2.length),

greaterThan(0));

Hadoop自身提供的IntWritable、LongWritabe等类已经实现了这种优化，使这些Writable类作为键进行比较时，直接使用序列化的字节数组进行比较大小，而不用进行反序列化。


