# JUnit中assertEquals和assertSame方法的不同 - Soul Joy Hub - CSDN博客

2016年11月01日 20:17:33[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1502



# [JUnit中assertEquals和assertSame方法的不同](http://www.cnblogs.com/ITEagle/archive/2010/04/05/1704709.html)

1)提供的接口数量不完全相同。
assertEquals支持boolean,long,int等等java primitiveType变量。
assertSame只支持Object。

2)比较的逻辑不同，结果可能不同。
assertSame是对象直接比较。assertEquals能利用被比较对象提供的比较逻辑来进行比较。
使得同样的条件下，两者的运行结果不一定相同。
简单解释如下：
assertEquals(Object A,  Object B) 的比较逻辑：
如果 A,B都是Null,返回true。否则调用 A.equals(B)来判断。

assertSame(Object A, Object B)的比较逻辑：
以A == B运算的结果来判断。

A.equals(B) 和 A==B 的差别在于。
如果A没有重写java.lang.Object的equals方法，
那么就是两个java对象的内存地址比较，比较结果和 A==B的结果相同。
如果A重写了equals方法(比如GregorianCalendar，BigDecimal类)，
那么比较的结果不一定和A==B的结果相同。







