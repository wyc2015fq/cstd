# JAVA中Long与Integer比较容易犯的错误 - z69183787的专栏 - CSDN博客
2016年05月14日 20:40:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3354
今天使用findbugs扫描项目后发现很多高危漏洞，其中非常常见的一个是比较两个Long或Integer时直接使用的==来比较。 其实这样是错误的。
因为Long与Ineger都是包装类型，是对象。  而不是普通类型long与int ， 所以它们在比较时必须都应该用equals，或者先使用longValue()或intValue()方法来得到他们的基本类型的值然后使用==比较也是可以的。
但是有一种特殊情况， 其实Long与Integer都将 -128~127 这些对象缓存了。  可以看看Long类型源码里面有一个LongCache类，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- privatestaticclass LongCache {  
- private LongCache(){}  
- 
- staticfinal Long cache[] = new Long[-(-128) + 127 + 1];  
- 
- static {  
- for(int i = 0; i < cache.length; i++)  
-         cache[i] = new Long(i - 128);  
-     }  
-     }  
先看看这个例子：
**[java]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- publicclass Test05 {  
- 
- publicstaticvoid main(String[] args) {  
-         Long a = 5L;  
-         Long b = 5L;  
- 
-         System.out.println("a == b ? " + (a == b));  
- 
-         Long c = 129L;  
-         Long d = 129L;  
-         System.out.println("c == d ? " + (c == d));  
-     }  
- }  
打印的结果是：
**[css]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- a == b ? true  
- c == d ? false  
原因
首先来看看 Long a = 5L ; 它是如何将一个基本类型long包装成一个对象Long的 。
 可以写一个测试类，然后反编译一下，看看java它是如何解析Long a = 5L这样一条命令的 。
测试类如下：
**[java]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- publicclass Test06 {  
-     Long l = 3L;  
- }  
然后使用javap -verbose Test06 就能看到反编译的结果了， 下面是输出的部分：
**[css]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- {  
- java.lang.Long l;  
- 
- public com.spring.test.Test06();  
-   Code:  
-    Stack=3, Locals=1, Args_size=1
- 0:   aload_0
- 1:   invokespecial   #10; //Method java/lang/Object."<init>":()V  
- 4:   aload_0
- 5:   ldc2_w  #12; //long 3l  
- 8:   invokestatic    #14; //Method java/lang/Long.valueOf:(J)Ljava/lang/Long;  
- 11:  putfield        #20; //Field l:Ljava/lang/Long;  
- 14:  return  
-   LineNumberTable:  
-    line 3:0
-    line 5:4
-    line 3:14
- 
-   LocalVariableTable:  
-    Start  Length  Slot  Name   Signature  
- 0150    this       Lcom/spring/test/Test06;  
- 
- 
- }  
从Code中的8可以看出调用了Long的一个类方法Long.valueOf(Long) ， 所以可以得到的结论是Long a = 5L实际上等于 Long a = Long.valueOf(5) ;
然后再看看Long.valueOf()方法是如何定义的：
**[java]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- publicstatic Long valueOf(long l) {  
- finalint offset = 128;  
- if (l >= -128 && l <= 127) { // will cache
- return LongCache.cache[(int)l + offset];  
- }  
- returnnew Long(l);  
-    }  
一目了然，会先判断基本类型的值如果在-128~127之间，就会直接从LongCache里面取出缓存的对象返回，否则就new一个新的Long对象返回 。
现在就不难理解Test05程序执行得到的结果了，因为a与b等于5，在-127~128之内，所以都是直接从LongCache里面返回的一个Long对象，所以他们在使用==比较的时候，就是相等的（对于对象类型来说，==比较的是两个对象的引用指向堆中的地址） ，而c与d等于129，不在-127~128之间，所以他们他们是分别new出来的两个新的Long对象，使用==来比较自然是不相等的了。
Long重写了equals方法：
**[java]**[view
 plain](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)[copy](http://blog.csdn.net/is_zhoufeng/article/details/38443507#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/446106/fork)
- publicboolean equals(Object obj) {  
- if (obj instanceof Long) {  
- return value == ((Long)obj).longValue();  
- }  
- returnfalse;  
-    }  
它是先通过.longValue()方法获取Long对象的基本类型long的值之后再做比较的。
所以对于Integer与Long的比较，最好是使用equals来比较才能确保得到我们想要的结果。
Integer与Long一样，这里就不举例了。
今天，帮人看代码时，发现这个问题，回顾起来，记录下来。
先来看一个代码片段
**[java]**[view
 plain](http://blog.csdn.net/danielinbiti/article/details/39996065#)[copy](http://blog.csdn.net/danielinbiti/article/details/39996065#)
- publicstaticvoid main(String[] args) {  
-         Integer a1 = Integer.valueOf(60);  //danielinbiti
-         Integer b1 = 60;    
-         System.out.println("1:="+(a1 == b1));     
- 
- 
-         Integer a2 = 60;    
-         Integer b2 = 60;    
-         System.out.println("2:="+(a2 == b2));    
- 
- 
-         Integer a3 = new Integer(60);    
-         Integer b3 = 60;    
-         System.out.println("3:="+(a3 == b3));    
- 
-         Integer a4 = 129;    
-         Integer b4 = 129;    
-         System.out.println("4:="+(a4 == b4));    
-     }  
这段代码的比较结果，如果没有执行不知道各位心中的答案都是什么。
要知道这个答案，就涉及到[Java](http://lib.csdn.net/base/javase)缓冲区和堆的问题。
java中Integer类型对于-128-127之间的数是缓冲区取的，所以用等号比较是一致的。但对于不在这区间的数字是在堆中new出来的。所以地址空间不一样，也就不相等。
Integer b3=60,这是一个装箱过程也就是Integer b3=Integer.valueOf(60)
所以，以后碰到Integer比较值是否相等需要用intValue()
对于Double没有缓冲区。
答案
1:=true
2:=true
3:=false
4:=false
注：int 和 Integer 的转换比较可参考 [http://blog.csdn.net/Kinger0/article/details/47948025](http://blog.csdn.net/Kinger0/article/details/47948025)
其实Integer与int类型的赋值与比较最关键的一点就是：这两个变量的类型不同。Integer是引用类型，int是原生数据类型。
        我们分四种情况来讨论：
        1） Integer与int类型的赋值
                a.把Integer类型赋值给int类型。此时，int类型变量的值会自动装箱成Integer类型，然后赋给Integer类型的引用，这里底层就是通过调用valueOf()这个方法来实现所谓的装箱的。
                b.把int类型赋值给Integer类型。此时，Integer类型变量的值会自动拆箱成int类型，然后赋给int类型的变量，这里底层则是通过调用intValue()方法来实现所谓的拆箱的。
        2） Integer与int类型的比较
                这里就无所谓是谁与谁比较了，Integer == int与int == Integer的效果是一样的，都会把Integer类型变量拆箱成int类型，然后进行比较，相等则返回true，否则返回false。同样，拆箱调用的还是intValue()方法。
        3） Integer之间的比较
                这个就相对简单了，直接把两个引用的值（即是存储目标数据的那个地址）进行比较就行了，不用再拆箱、装箱什么的。
        4） int之间的比较
                这个也一样，直接把两个变量的值进行比较。
        值得注意的是：对Integer对象，JVM会自动缓存-128~127范围内的值，所以所有在这个范围内的值相等的Integer对象都会共用一块内存，而不会开辟多个；超出这个范围内的值对应的Integer对象有多少个就开辟多少个内存。
