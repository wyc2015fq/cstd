# Java int/Integer 区别 - Simple 专栏 - CSDN博客
2012年01月31日 13:49:22[Simple_Zz](https://me.csdn.net/love284969214)阅读数：1484
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
首先Java是一个近乎纯洁的面向对象编程语言，但是为了编程的方便还是引入不是对象的基本数据类型，但是为了能够将这些基本数据类型当成对象操作，Java为每一个基本数据类型都引入了对应的包装类型（wrapper class），int的包装类就是Integer，从JDK 1.5开始引入了自动装箱/拆箱机制，使得二者可以相互转换。
|原始类型|boolean|char|byte|short|int|long|float|double|
|----|----|----|----|----|----|----|----|----|
|包装类型|Boolean|Character|Byte|Short|Integer|Long|Float|Double|
```java
public static void main(String[] args) {
    Integer a = new Integer(3);
    Integer b = 3; // 将3自动装箱成Integer类型
    int c = 3;
    System.out.println(a == b); // false 两个引用没有引用同一对象
    System.out.println(a == c); // true a自动拆箱成int类型再和c比较
}
```
补充：自动装箱和拆箱相关的还有一个问题，代码如下所示：
```
public static void main(String[] args) {
    Integer f1 = 100, f2 = 100, f3 = 150, f4 = 150;  
    System.out.println(f1 == f2);  // true
    System.out.println(f3 == f4);  // false
}
```
这个很容易理解为两个都是true或者false。
首先需要注意的是f1、f2、f3、f4四个变量都是Integer对象，所以下面的 == 运算比较的不是值而是引用。
装箱的本质是什么呢？当我们给一个Integer对象赋一个int值的时候，会调用Integer类的静态方法valueOf，如果看看valueOf的源代码就知道发生了什么。
```java
public static Integer valueOf(int i) {  
    if (i >= IntegerCache.low && i <= IntegerCache.high)  
        return IntegerCache.cache[i + (-IntegerCache.low)];  
    return new Integer(i);  
}
```
IntegerCache是Integer的内部类，其代码如下所示：
```java
/**
 * Cache to support the object identity semantics of autoboxing for values
 * between -128 and 127 (inclusive) as required by JLS.
 * 
 * The cache is initialized on first usage. The size of the cache may be
 * controlled by the {@code -XX:AutoBoxCacheMax=<size>} option. During VM
 * initialization, java.lang.Integer.IntegerCache.high property may be set and
 * saved in the private system properties in the sun.misc.VM class.
 */
/** 
 * 该类可以简单的理解为。
 * 如果Integer数值在-128到127之间，那么不会new新的Integer对象。
 * 而是直接引用常量池中的Integer对象。
 * 
 * 所以上面的代码中 f1==f2的结果是true，而f3==f4的结果是false。
 */
private static class IntegerCache {
    static final int low = -128;
    static final int high;
    static final Integer cache[];
    static {
        // high value may be configured by property
        int h = 127;
        String integerCacheHighPropValue = sun.misc.VM.getSavedProperty("java.lang.Integer.IntegerCache.high");
        if (integerCacheHighPropValue != null) {
            try {
                int i = parseInt(integerCacheHighPropValue);
                i = Math.max(i, 127);
                // Maximum array size is Integer.MAX_VALUE
                h = Math.min(i, Integer.MAX_VALUE - (-low) - 1);
            } catch (NumberFormatException nfe) {
                // If the property cannot be parsed into an int, ignore it.
            }
        }
        high = h;
        cache = new Integer[(high - low) + 1];
        int j = low;
        for (int k = 0; k < cache.length; k++)
            cache[k] = new Integer(j++);
        // range [-128, 127] must be interned (JLS7 5.1.7)
        assert IntegerCache.high >= 127;
    }
    private IntegerCache() {
    }
}
```
