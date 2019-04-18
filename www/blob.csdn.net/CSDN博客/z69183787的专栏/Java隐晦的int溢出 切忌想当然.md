# Java隐晦的int溢出 切忌想当然 - z69183787的专栏 - CSDN博客
2017年01月23日 12:02:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2715

```java
public static void main(String[] args) {
        int u1 = 1475143069;
        Date d = new Date(u1 +1000);
        long u2 = u1 *1000;
        long u3 =(long)u1 * 1000;
        System.out.println(u1);
        System.out.println(d.getTime());
        System.out.println(u2);
        System.out.println(u3);
    }
```
1475143069
1475144069
1969286472
1475143069000
上面3个 都因为计算过程中 int 溢出了 所以没有得到正确的值，最后一种方式才能得到正确的
所有int或者Integer 的计算 最好吧每个计算数 或者左侧第一个 转成long 或者 Long.valueOf 保证计算的正确性
“任意一个常量上加L”这样总结是有问题的！
要培养这样的习惯,如果我写的常量是long的,我必定没有一个都写后缀.
long a = 10L * 100L ...
Java代码  ![收藏代码](http://linliangyi2007.iteye.com/images/icon_star.png)
- 60 * 24 * 3600 * 1000=889032704
- 60 * 24 * 3600L* 1000=5184000000
- 60 * 24 * 3600 * 1000 * 1L=889032704
因为java中乘法的计算是左结合的！

下面转别人的：
先上一段代码 简单介绍一下问题
**[java]**[view
 plain](http://blog.csdn.net/a597926661/article/details/8554878#)[copy](http://blog.csdn.net/a597926661/article/details/8554878#)
- long now = System.currentTimeMillis();  
- long m1 = now-1000*60*60*24*30*3;   
- long test = (long)1000*60*60*24*30*3;  
- long nocast = 1000L*60*60*24*30*3;  
- long m2 = now-1000L*60*60*24*30*3;  
- 
-             System.out.println("我是test:"+test);  
-             System.out.println(nocast);  
-             System.out.println("now:"+now);  
-             System.out.println("m1:"+m1);   
-             System.out.println("m2:"+m2);  
输出结果：
**[java]**[view
 plain](http://blog.csdn.net/a597926661/article/details/8554878#)[copy](http://blog.csdn.net/a597926661/article/details/8554878#)
- 我是test:7776000000
- 7776000000
- now:1359510417546
- m1:1360324352138
- m2:1351734417546
有点奇怪了吧 1000*60*60*24*30*3的计算值与1000L*60*60*24*30*3 的计算值是不一样的 1000*60*60*24*30*3 的计算结果是-813934592
而 (long)1000*60*60*24*30*3;  1000L*60*60*24*30*3;的计算结果都是7776000000 这个结果才是需要的东西
为什么会出现这种情况呢 [Java](http://lib.csdn.net/base/javase)在做乘法时，如果没有显示确定的数据类型，那么他会默认按照int数据类型进行计算就好比1000*60*60*24*30*3，这个数值在计算时导致int数据溢出，所以出现了负数的情况。
(long)1000*60*60*24*30*3;  1000L*60*60*24*30*3;这两者，前者是将首个计算数强制造型为long 然后按照long类型运算得出了正确的计算结果，后者在计算时将数据类型修改为long 能直接获得正确的计算结果，异曲同工。
这一点还是很隐晦的，要注意，防止以后出错。这错误估计不好排查。
还有一点就是 L所在的位置，有点奇怪 1000L*60*60*24*30*3; 1000*60L*60*24*30*3;1000*60*60L*24*30*3;1000*60*60*24*30L*3;这几种写法都可以正确转为long型，但是1000*60*60*24*30*3L;就不对，而且3*60*60*24*30*1000这一种写法的时候 L跟在任何一个数字后面都没问题。有点莫名其妙，用的时候debug[测试](http://lib.csdn.net/base/softwaretest)要优先确定这个串的计算结果。
