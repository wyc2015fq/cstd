# 浅谈 Java 中的 StringBuilder - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 连接符号 “+” 本质
在《[浅谈Java String内幕（1）](http://blog.jobbole.com/105169/)》 中，字符串变量（非final修饰）通过 “+” 进行拼接，在编译过程中会转化为StringBuilder对象的append操作，注意是编译过程，而不是在JVM中。


```
public class StringTest {
    public static void main(String[] args) {
        String str1 = "hello ";
        String str2 = "java";
        String str3 = str1 + str2 + "!";
        String str4 = new StringBuilder().append(str1).append(str2).append("!").toString();
    }
}
```
上述 str3 和 str4 的执行效果其实是一样的，不过在for循环中，千万不要使用 “+” 进行字符串拼接。


```
public class test {
    public static void main(String[] args) {
        run1();
        run2();
    }   
    public static void run1() {
        long start = System.currentTimeMillis();
        String result = "";
        for (int i = 0; i
```
在for循环中使用 “+” 和StringBuilder进行1万次字符串拼接，耗时情况如下：
1、使用 “+” 拼接，平均耗时 250ms；
2、使用StringBuilder拼接，平均耗时 1ms；
for循环中使用 “+” 拼接为什么这么慢？下面是run1方法的字节码指令
![2184951-aa3532a823ca11b5](http://jbcdn2.b0.upaiyun.com/2016/09/7ba59d5b503de86c97e18d6fe33e262b.png)
5 ~ 34 行对应for循环的代码，可以发现，每次循环都会重新初始化StringBuilder对象，导致性能问题的出现。
### 性能问题
StringBuilder内部维护了一个char[]类型的value，用来保存通过append方法添加的内容，通过 `new StringBuilder()` 初始化时，char[]的默认长度为16，如果append第17个字符，会发生什么？


```
void expandCapacity(int minimumCapacity) {
    int newCapacity = value.length * 2 + 2;
    if (newCapacity - minimumCapacity
```
如果value的剩余容量，无法添加全部内容，则通过`expandCapacity(int minimumCapacity)`方法对value进行扩容，其中minimumCapacity = 原value长度 + append添加的内容长度。
1、扩大容量为原来的两倍 + 2，为什么要 + 2，而不是刚好两倍？
2、如果扩容之后，还是无法添加全部内容，则将 minimumCapacity 作为最终的容量大小；
3、利用 `System.arraycopy` 方法对原value数据进行复制；
在使用StringBuilder时，如果给定一个合适的初始值，可以避免由于char[]数组多次复制而导致的性能问题。
**不同初始容量的性能测试：**


```
public class StringBuilderTest {
    public static void main(String[] args) {
        int sum = 0;
        final int capacity = 40000000;
        for (int i = 0; i
```
执行一千万次append操作，不同初始容量的耗时情况如下：
1、容量为默认16时，平均耗时110ms；
2、容量为40000000时，不会发生复制操作，平均耗时85ms；
通过以上数据可以发现，性能损耗不是很严重。
### 内存问题
1、StringBuilder内部进行扩容时，会新建一个大小为原来两倍+2的char数组，并复制原char数组到新数组，导致内存的消耗，增加GC的压力。
2、StringBuilder的toString方法，也会造成char数组的浪费。


```
public String toString() {
    // Create a copy, don't share the array
    return new String(value, 0, count);
}
```
String的构造方法中，会新建一个大小相等的char数组，并使用 `System.arraycopy()` 复制StringBuilder中char数组的数据，这样StringBuilder的char数组就白白浪费了。
**重用StringBuilder**


```
public class StringBuilderHolder {
    private final StringBuilder sb;
    public StringBuilderHolder(int capacity) {
        sb = new StringBuilder(capacity);
    }
    public StringBuilder resetAndGet() {
        sb.setLength(0);
        return sb;
    }
}
```
通过 `sb.setLength(0)` 方法可以把char数组的内存区域设置为0，这样char数组重复使用，为了避免并发访问，可以在ThreadLocal中使用StringBuilderHolder，使用方式如下：


```
private static final ThreadLocal stringBuilder= new ThreadLocal() {
    @Override
    protected StringBuilderHolder initialValue() {
        return new StringBuilderHolder(256);
    }
};
StringBuilder sb = stringBuilder.get().resetAndGet();
```
不过这种方式也存在一个问题，该StringBuilder实例的内存空间一直不会被GC回收，如果char数组在某次操作中被扩容到一个很大的值，可能之后很长一段时间都不会用到如此大的空间，就会造成内存的浪费。
### 总结
虽然使用默认的StringBuilder进行字符串拼接操作，性能消耗不是很严重，但在高性能场景下，还是推荐使用ThreadLocal下可重用的StringBuilder方案。
参考资料：
[StringBuilder在高性能场景下的正确用法](http://calvin1978.blogcn.com/articles/stringbuilder.html)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
