
# Java包装类型的大小比较需谨慎 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:36:09[seven-soft](https://me.csdn.net/softn)阅读数：1023


基本类型是可以比较大小的，其所对应的包装类型都实现了Comparable 接口也说明了此问题，那我们来比较一下两个包装类型的大小，代码如下：
public class Client {
public static void main(String[] args) {
Integer i = new Integer(100);
Integer j = new Integer(100);
compare(i,j);
}
// 比较两个包装对象大小
public static void compare(Integer i , Integer j) {
System.out.println(i == j);
System.out.println(i > j);
System.out.println(i < j);
}
}
代码很简单，产生了两个Integer 对象，然后比较两者的大小关系，既然基本类型和包装类型是可以自由转换的，那上面的代码是不是就可打印出两个相等的值呢？让事实说话，运行结果如下：
false
false
false
竟然是3 个false，也就是说两个值之间不等，也没大小关系，这也太奇怪了吧。不奇怪，我们来一一解释。i == j
在Java 中“==”是用来判断两个操作数是否有相等关系的，如果是基本类型则判断值是否相等，如果是对象则判断是否是一个对象的两个引用，也就是地址是否相等，这里很明显是两个对象，两个地址，不可能相等。
i > j 和 i < j
在Java 中，“>”和“<”用来判断两个数字类型的大小关系，注意只能是数字型的判断，对于Integer 包装类型，是根据其intValue() 方法的返回值（也就是其相应的基本类型）进行比较的（其他包装类型是根据相应的value 值来比较的，如doubleValue、floatValue 等），那很显然，两者不可能有大小关系的。
问题清楚了，修改总是比较容易的，直接使用Integer 实例的compareTo 方法即可。但是这类问题的产生更应该说是习惯问题，只要是两个对象之间的比较就应该采用相应的方法，而不是通过Java 的默认机制来处理，除非你确定对此非常了解。

