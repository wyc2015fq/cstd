# Java虚拟机解析篇之---内存模型 - 零度的博客专栏 - CSDN博客
2017年03月07日 11:14:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：239标签：[虚拟机																[内存](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)
个人分类：[JVM调优](https://blog.csdn.net/zmx729618/article/category/6129208)
今天闲来无事来，看一下[Java](http://lib.csdn.net/base/javase)中的内存模型和垃圾回收机制的原理，关于这个方面的知识，网上已经有很多现成的资料可以供我们参考，但是知识还是比较杂的，在这部分知识点中有一本书不得不推荐：**《深入理解Java虚拟机》**，现在已经是第二版了。这本书就从头开始详细介绍了Java整个虚拟机的模型以及Java的类文件结构，加载机制等。这里大部分的知识点都是可以在这本书中找到的，当然我是主要还是借鉴这本书中的很多内容的。下面就不多说了，进入主题吧。
首先来看一下Java中的内存模型图：
![](https://img-blog.csdn.net/20141102100112703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**第一、程序计数器(PC)**
程序计数器(Program Counter Register)是一块较小的内存空间，它可以看做当前线程所执行的字节码的行号指示器，字节码解释器工作时就是通过改变这个计数器的值来取下一条需要执行的字节码指令，分支、跳转、循环、异常处理、线程恢复等基础功能都需要这个计数器来完成
**注：程序计数器是线程私有的，每条线程都会有一个独立的程序计数器**
**第二、Java栈(虚拟机栈)**
Java栈就是Java中的方法执行的内存模型，每个方法在执行的同时都会创建一个**栈帧**(关于栈帧后面介绍)，这个栈帧用于存储局部变量表、操作数栈、动态链接、方法出口等信息，每个方法从调用直至执行完成的过程，就对应着一个栈帧在虚拟机栈中入栈到出栈的过程。
**注：Java栈也是线程私有的。**
**异常可能性：对于栈有两种异常情况：如果线程请求的栈深度大于栈所允许的深度，将抛出StackOverflowError异常，如果虚拟****机栈****可以动态拓展，在拓展的时无法申请到足够的内存，将会抛出OutOfMemoryError异常**
**栈帧的概念：**
栈帧用于支持虚拟机进行方法调用和执行的[数据结构](http://lib.csdn.net/base/datastructure)。
![](https://img-blog.csdn.net/20141102101427234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**1) 局部变量表**
局部变量表（Local Variable Table）是一组 变量值存储空间，用于存放 方法参数和方法内部定义的局部变量.局部变量表的容量以变量槽（Variable Slot，下称Slot）为最小单位. 一个Slot可以存放一个32位以内的数据类型，Java中占用32位以内的数据类型有boolean、byte、char、short、int、float、reference[3]和returnAddress 8种类型,对于 64位的数据类型，虚拟机会以高位对齐的方式为其
 分配两个连续的Slot空间(long double).
**2) 操作数栈**
操作数栈（Operand Stack）也常称为操作栈，它是一个后入先出（Last In First Out,LIFO）栈，当一个方法刚刚执行的时候，这个方法的操作数栈是空的，在方法的执行过程中，会有各种字节码指令往操作数栈中写入和提取内容，也就是出栈/入栈操作，例如，在做算术运算的时候通过操作数栈来进行的，又或者在调用其他方法的时候是通过操作数栈来进行参数传递的。
举个例子：整数假发的字节码指令iadd在运行的时候操作数栈中最接近栈顶的两个元素已经存入了两个int类型的数值，当执行这个指令时，会将这两个int值出栈并相加，然后将相加的结果入栈。
**3) 方法返回地址**
一个方法开始执行后，只有 两种方式可以退出这个方法。第一种方式是执行引擎遇到任意一个方法返回的字节码指令，这时候可能会有返回值传递给上层的方法调用者（调用当前方法的方法称为调用者），是否有返回值和返回值的类型将根据遇到何种方法返回指令来决定，这种退出方法的方式称为 正常完成出口（Normal Method Invocation Completion）。另外一种退出方式是，在方法执行过程中 遇到了异常，并且这个异常没有在方法体内得到处理，无论是Java虚拟机内部产生的异常，还是代码中使用athrow字节码指令产生的异常，只要在本方法的异常表中没有搜索到匹配的异常处理器，就会导致方法退出，这种退出方法的方式称为
 异常完成出口（Abrupt Method Invocation Completion）。 一个方法使用异常完成出口的方式退出，是不会给它的上层调用者产生任何返回值的  
**4) 附加信息**
虚拟机规范允许具体的虚拟机实现增加一些规范里没有描述的信息到栈帧之中，例如与调试相关的信息，这部分信息完全取决于具体的虚拟机实现，这里不再详述。在实际开发中，一般会把动态连接、方法返回地址与其他附加信息全部归为一类，称为栈帧信息。
**第三、本地方法栈**
本地方法栈与Java栈所发挥的作用是非常相似的，它们之间的区别不过是Java栈执行Java方法，本地方法栈执行的是本地方法。
**注：本地方法栈也是线程私有的**
**异常可能性：和Java栈一样，可能抛出StackOverflowError和OutOfMemeryError异常**
**第四、Java堆**
对于大多数应用来说，Java堆是Java虚拟机所管理的内存中最大的一块，在虚拟机启动时创建。此内存区域的唯一目的就是存放对象实例，几乎所有的对象实例都在这里分配内存，当然我们后面说到的垃圾回收器的内容的时候，其实Java堆就是垃圾回收器管理的主要区域。
**注：堆是线程共享的**
**异常可能性：如果堆中没有内存完成实例分配，并且堆也无法再拓展时，将会抛出OutOfMemeryError异常**
**第五、方法区**
方法区它用于存储已被虚拟机加载的类信息、常量、静态常量、即时编译器编译后的代码等数据。
**注：方法区和堆一样是线程共享的**
**异常可能性：当方法区无法满足内存分配需求时，将抛出OutOfMemeryError异常**
**1)运行时常量池**
运行时常量池是方法区的一部分，Class文件中除了有类的版本、字段、方法、接口等描述信息外，还有一项信息是常量池，用于存放编译器生成的各种字面量和符号引用，这部分内容将在类加载器后进入方法区的运行时异常常量池存放。
上面就介绍了Java的内存的几个模块的相关概念，其实我们需要知道这些知识，最主要的目的是不要在项目中写那些OOM的代码，因为我们如果知道了内存模型之后，即使代码中出现了OOM的问题，我们可以定位到哪里出了问题。
下面也来看一下上面说到的几个内存模块导致的内存溢出异常问题：
**(这个也是面试的时候经常会被问到：比如叫你写一段让堆内存溢出的代码，或者是问你如果如果修改堆大小)**
**第一、堆溢出**
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505061/fork)
- publicclass HeapOOM {  
- 
- staticclass OOMObject{}  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         List<OOMObject> list = new ArrayList<OOMObject>();  
- 
- while(true){  
-             list.add(new OOMObject());  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们上面看到堆主要是存放对象的，所以我们如果想让堆出现OOM的话，可以开一个死循环，然后产生新的对象就可以了。然后在将堆的大小调小点。
加上JVM参数
-verbose:gc **-Xms10M -Xmx10M** -XX:+PrintGCDetails -XX:SurvivorRatio=8 -XX:+HeapDumpOnOutOfMemoryError，
就能很快报出OOM：
**Exception in thread "main" java.lang.OutOfMemoryError: [Java](http://lib.csdn.net/base/java)heap space**
**第二、栈溢出**
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505061/fork)
- package com.cutesource;  
- 
- publicclass StackOOM {  
- 
- /**
-      * @param args
-      */
- 
- privateint stackLength = 1;  
- 
- publicvoid stackLeak(){  
-         stackLength++;  
-         stackLeak();  
-     }  
- 
- publicstaticvoid main(String[] args) throws Throwable{  
- // TODO Auto-generated method stub
-         StackOOM oom = new StackOOM();  
- try{  
-             oom.stackLeak();  
-         }catch(Throwable err){  
-             System.out.println("Stack length:" + oom.stackLength);  
- throw err;  
-         }  
- 
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们知道栈中存放的方法执行的过程中需要的空间，所以我们可以下一个循环递归，这样方法栈就会出现OOM的异常了。
设置JVM参数：**-Xss128k**，报出异常：
**Exception in thread "main" java.lang.StackOverflowError**
打印出Stack length:1007，这里可以看出，在我的机器上128k的栈容量能承载深度为1007的方法调用。当然报这样的错很少见，一般只会出现无限循环的递归中，另外，线程太多也会占满栈区域：
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505061/fork)
- package com.cutesource;  
- 
- publicclass StackOOM {  
- 
- /**
-      * @param args
-      */
- 
- privateint stackLength = 1;  
- 
- privatevoid dontStop(){  
- while(true){  
- try{Thread.sleep(1000);}catch(Exception err){}  
-         }  
-     }  
- 
- publicvoid stackLeakByThread(){  
- while(true){  
-             Thread t = new Thread(new Runnable(){  
- 
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
-                     dontStop();  
-                 }  
- 
-             });  
-             t.start();  
-             stackLength++;  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws Throwable{  
- // TODO Auto-generated method stub
-         StackOOM oom = new StackOOM();  
- try{  
-             oom.stackLeakByThread();  
-         }catch(Throwable err){  
-             System.out.println("Stack length:" + oom.stackLength);  
- throw err;  
-         }  
- 
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这个栈的溢出，就是我们上面说到栈的时候的两种异常情况。
报出异常：**Exception in thread "main" java.lang.OutOfMemoryError:unable to create new native thread**
**第三、方法区溢出**
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505061/fork)
- publicclass MethodAreaOOM {  
- 
- staticclass OOMOjbect{}  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- while(true){  
-             Enhancer eh = new Enhancer();  
-             eh.setSuperclass(OOMOjbect.class);  
-             eh.setUseCache(false);  
-             eh.setCallback(new MethodInterceptor(){  
- 
- @Override
- public Object intercept(Object arg0, Method arg1,  
-                         Object[] arg2, MethodProxy arg3) throws Throwable {  
- // TODO Auto-generated method stub
- return arg3.invokeSuper(arg0, arg2);  
-                 }  
- 
-             });  
-             eh.create();  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们知道方法区是存放一些类的信息等，所以我们可以使用类加载无限循环加载class，这样就会出现方法区的OOM异常。
手动将栈的大小调小点
加上JVM参数：**-XX:PermSize=10M -XX:MaxPermSize=10M**，运行后会报如下异常：
**Exception in thread "main" java.lang.OutOfMemoryError: PermGen space**
第四、常量池溢出
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40616223#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505061/fork)
- publicclass ConstantOOM {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
-         List<String> list = new ArrayList<String>();  
- int i=0;  
- while(true){  
-             list.add(String.valueOf(i++).intern());  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们知道常量池中存放的是运行过程中的常量，同时我们知道String类型的intern方法是将字符串的值放到常量池中的。所以上面弄可以开一个死循环将字符串的值都放到常量池中，这样常量池就会出现OOM异常了。因为常量池本身就是方法区的一部分，所以我们也可以手动的调节一下栈的大小。
**总结**：上面只是从宏观的角度介绍了一下内存模型，具体关于内存中每个区域的详细信息，可以阅读开头说到的那本很不错的书籍。当然我们在学习Java的时候可以分为四大模块：Java的Api、Java虚拟机(内存模型和垃圾回收器)、Java的Class文件、设计模式，关于Api的知识我们在工作的过程中用到的比较多，而且这部分内容完全是靠使用度，你用多了，api你自然就知道了。Java虚拟机和Java的Class文件的相关知识在工作中可能不一定能用到，但是这方面的知识能够让你更了解Java的整个体系结构。至于设计模式这个就是修炼的过程，也是最难的过程。得慢慢的体会其的强大之处。
