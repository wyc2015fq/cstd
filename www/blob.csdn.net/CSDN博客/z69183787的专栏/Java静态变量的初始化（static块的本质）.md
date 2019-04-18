# Java静态变量的初始化（static块的本质） - z69183787的专栏 - CSDN博客
2013年03月27日 10:38:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2148
在网上看到了下面的一段代码：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- static {  
-         _i = 20;  
-     }  
- publicstaticint _i = 10;  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(_i);  
-     }  
- }  
上述代码会打印出什么结果来呢？10还是20？本文将以此代码为引子，着重讨论一下静态变量的初始化问题。
## 问题1：静态变量如何初始化
Java类中可以定义一个static块，用于静态变量的初始化。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- static {  
-         _i = 10;  
-     }  
- }  
当然最常用的初始化静态变量的操作是在声明变量时直接进行赋值操作。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i = 10;  
- }  
那么上述两例在本质上有什么区别吗？回答是没有区别。两例代码编译之后的字节码完全一致，通过 “javap -c”查看到的字节码如下：
> 
public class Test extends java.lang.Object{
public static int _i;
public Test();
  Code:
   0: aload_0
   1: invokespecial #1; //Method java/lang/Object."<init>":()V
   4: return
static {};
  Code:
   0: bipush 10
   2: putstatic #2; //Field _i:I
   5: return
}
通过字节码还可以看出，当类的定义中不含有static块时，编译器会为该类提供一个默认的static块。当然这是在含有静态变量初始化操作的前提下。如果静态变量没有初始化操作，则编译器不会为之提供默认的static块。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- }  
其字节码的表现形式为：
> 
public class Test extends java.lang.Object{
public static int _i;
public Test();
  Code:
   0: aload_0
   1: invokespecial #1; //Method java/lang/Object."<init>":()V
   4: return
}
由于静态变量是通过赋值操作进行初始化的，因此可以通过静态函数返回值的方式为其初始化。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i = init();  
- 
- privatestaticint init() {  
- return10;  
-     }  
- }  
其本质与下面的代码相同：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- static {  
-         _i = init();  
-     }  
- 
- privatestaticint init() {  
- return10;  
-     }  
- }  
## 问题2：JDK如何处理static块
类定义中可以存在多个static块吗？回答是可以。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- static {  
-         _i = 10;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-     }  
- 
- static {  
-         _i = 20;  
-     }  
- }  
此类编译之后的字节码为：
> 
public class Test extends java.lang.Object{
public static int _i;
public Test();
  Code:
   0: aload_0
   1: invokespecial #1; //Method java/lang/Object."<init>":()V
   4: return
public static void main(java.lang.String[]);
  Code:
   0: return
static {};
  Code:
   0: bipush 10
   2: putstatic #2; //Field _i:I
   5: bipush 20
   7: putstatic #2; //Field _i:I
   10: return
}
观察static{}部分可以看出，上例的代码与下面的代码效果一致：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- 
- publicstaticvoid main(String[] args) {  
-     }  
- 
- static {  
-         _i = 10;  
-         _i = 20;  
-     }  
- }  
此例可以证明，不仅类定义中可以有多个static块，而且在编译时编译器会将多个static块按照代码的前后位置重新组合成一个static块。
## 问题3：如何看待静态变量的声明
**静态变量存放在常量池之中。**如何证明呢？如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i = 10;  
- }  
使用“javap -c -verbose”查看其字节码的内容如下：
> 
public class Test extends java.lang.Object
  SourceFile: "Test.java"
  minor version: 0
  major version: 49
  Constant pool:
const #1 = Method #4.#14; //  java/lang/Object."<init>":()V
const #2 = Field #3.#15; //  Test._i:I
const #3 = class #16; //  Test
const #4 = class #17; //  java/lang/Object
const #5 = Asciz _i;
const #6 = Asciz I;
const #7 = Asciz <init>;
const #8 = Asciz ()V;
const #9 = Asciz Code;
const #10 = Asciz LineNumberTable;
const #11 = Asciz <clinit>;
const #12 = Asciz SourceFile;
const #13 = Asciz Test.java;
const #14 = NameAndType #7:#8;//  "<init>":()V
const #15 = NameAndType #5:#6;//  _i:I
const #16 = Asciz Test;
const #17 = Asciz java/lang/Object;
{
public static int _i;
public Test();
  Code:
   Stack=1, Locals=1, Args_size=1
   0: aload_0
   1: invokespecial #1; //Method java/lang/Object."<init>":()V
   4: return
  LineNumberTable: 
   line 2: 0
static {};
  Code:
   Stack=1, Locals=0, Args_size=0
   0: bipush 10
   2: putstatic #2; //Field _i:I
   5: return
  LineNumberTable: 
   line 3: 0
}
我们看到，常量池中const #2指向的就是Test._i，也就是静态变量。静态变量被保存到常量池中的工作原理这里不深入讨论。在此需要注意的是：
- 静态变量的声明与初始化是两个不同的操作；
- 静态变量的声明在编译时已经明确了内存的位置。
如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i = 10;  
- }  
上述代码的本质可以视为：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- // 静态变量的声明
- publicstaticint _i;  
- 
- // 静态变量的初始化
- static {  
-         _i = 10;  
-     }  
- }  
由于静态变量的声明在编译时已经明确，所以静态变量的声明与初始化在编码顺序上可以颠倒。也就是说可以先编写初始化的代码，再编写声明代码。如：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- // 静态变量的初始化
- static {  
-         _i = 10;  
-     }  
- 
- // 静态变量的声明
- publicstaticint _i;  
- }  
## 对初始问题的解答
解答了上述三个问题，让我们再来看看开篇提到的问题。代码如下：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- static {  
-         _i = 20;  
-     }  
- publicstaticint _i = 10;  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(_i);  
-     }  
- }  
其本质可以用下面的代码表示：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- static {  
-         _i = 20;  
-     }  
- publicstaticint _i;  
- static {  
-         _i = 10;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(_i);  
-     }  
- }  
再简化一下，可以表示为：
**[java]**[view
 plain](http://blog.csdn.net/darxin/article/details/5293427#)[copy](http://blog.csdn.net/darxin/article/details/5293427#)
- publicclass Test {  
- publicstaticint _i;  
- 
- static {  
-         _i = 20;  
-         _i = 10;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(_i);  
-     }  
- }  
至此，代码已经明确告诉我们打印结果是什么了！
