# String的hashcode(java) - z69183787的专栏 - CSDN博客
2017年09月11日 10:33:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：360
hashCode就是我们所说的散列码，使用hashCode[算法](http://lib.csdn.net/base/31)可以帮助我们进行高效率的查找，例如HashMap，说hashCode之前，先来看看Object类。
Java程序中所有类的直接或间接父类，处于类层次的最高点。在Object类里定义了很多我们常见的方法，包括我们要讲的hashCode方法，如下
Java代码  [](http://crd1991.iteye.com/blog/1473108)
- public final native Class<?> getClass();  
- public native int hashCode();  
- public boolean equals(Object obj) {  
- return (this == obj);  
- }   
- public String toString() {  
- return getClass().getName() + "@" +  Integer.toHexString(hashCode());  
- }  
在 java的很多类中都会重写equals和hashCode方法，这是为什么呢？最常见的String类，比如我定义两个字符相同的字符串，那么对它们进 行比较时，我想要的结果应该是相等的，如果你不重写equals和hashCode方法，他们肯定是不会相等的，因为两个对象的内存地址不一样。
String类的重写的hashCode方法
Java代码  [](http://crd1991.iteye.com/blog/1473108)
- public int hashCode() {  
- int h = hash;  
- if (h == 0) {  
- int off = offset;  
- char val[] = value;  
- int len = count;  
- 
- for (int i = 0; i < len; i++) {  
-                 h = 31*h + val[off++];  
-             }  
-             hash = h;  
-         }  
- return h;  
-     }  
1、这段代码究竟是什么意思？
其实这段代码是这个数学表达式的实现
Java代码  [](http://crd1991.iteye.com/blog/1473108)
- s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]  
s[i] 是string的第i个字符，n是String的长度。那为什么这里用31，而不是其它数呢?《Effective Java》是这样说的：之所以选择31，是因为它是个奇素数，如果乘数是偶数，并且乘法溢出的话，信息就会丢失，因为与2相乘等价于移位运算。使用素数的 好处并不是很明显，但是习惯上都使用素数来计算散列结果。31有个很好的特性，就是用移位和减法来代替乘法，可以得到更好的性能：31*i==(i<<5)-i。现在的VM可以自动完成这种优化。
2、它返回的hashCode有什么特点呢？
可 以看到，String类是用它的value值作为参数来计算hashCode的，也就是说，相同的value就一定会有相同的hashCode值。这点也 很容易理解，因为value值相同，那么用equals比较也是相等的，equals方法比较相等，则hashCode一定相等。反过来不一定成立。它不
 保证相同的hashCode一定有相同的对象。
一个好的hash函数应该是这样的：为不相同的对象产生不相等的hashCode。
在 理想情况下，hash函数应该把集合中不相等的实例均匀分布到所有可能的hashCode上，要想达到这种理想情形是非常困难的，至少java没有达到。 因为我们可以看到，hashCode是非随机生成的，它有一定的规律，就是上面的数学等式，我们可以构造一些具有相同hashCode但value值不一
 样的，比如说：Aa和BB的hashCode是一样的。
说 到这里，你可能会想，原来构造hash冲突那么简单啊，那我是不是可以对HashMap函数构造很多<key,value>不都一样，但具有 相同的hashCode，这样的话可以把HashMap函数变成一条单向链表，运行时间由线性变为平方级呢？虽然HashMap重写的hashCode方
 法比String类的要复杂些，但理论上说是可以这么做的。这也是最近比较热门的Hash Collision DoS事件。
Java代码  [](http://crd1991.iteye.com/blog/1473108)
- public final int hashCode() {  
- return (key==null   ? 0 : key.hashCode()) ^  
-             (value==null ? 0 : value.hashCode());  
-  } 
（转载请注明出处：[url=http://www.k8764.com]博彩通[/url]
总结：字符串hash函数，不仅要减少冲突，而且要注意相同前缀的字符串生成的hash值要相邻。
