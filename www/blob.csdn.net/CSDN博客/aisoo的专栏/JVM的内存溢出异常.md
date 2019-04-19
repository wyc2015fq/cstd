# JVM的内存溢出异常 - aisoo的专栏 - CSDN博客
2012年11月22日 11:15:19[aisoo](https://me.csdn.net/aisoo)阅读数：467
在Java虚拟机规范的描述中，除了PC(程序计数器)寄存器外，虚拟机内存的其他几个运行时区域都有发生OutOfMemoryError异常的可能。当发生OutOfMemoryError异常时，无法用try...catch捕捉。
在开始讲解之前，在这里先简单介绍下虚拟机启动相关的一些内存设置参数。因为OutOfMemoryError异常发生，与这些参数的设置密切相关。
**举例说明含义：**
-Xss128k
每个线程的java栈大小,一个线程java栈所有栈帧大小总和最大允许的尺寸128k。
-Xms128m
表示JVM Heap(堆内存)最小尺寸128MB，初始分配
-Xmx512m
表示JVM Heap(堆内存)最大允许的尺寸256MB，按需分配。
-XX:PermSize=20M
设置方法区的初始大小
-XX:MaxPermSize=30M
设置方法区的最大值
**Java栈溢出**
在Java虚拟机规范中，对这个区域规定了两种异常状况：StackOverflowError和OutOfMemoryError异常。
**1.StackOverflowError异常**
每当java程序代码启动一个新线程时，Java虚拟机都会为它分配一个Java栈。Java栈以帧为单位保存线程的运行状态。当线程调用java方法时，虚拟机压入一个新的栈帧到该线程的java栈中。只要这个方法还没有返回，它就一直存在。如果线程的方法嵌套调用层次太多(如递归调用)，随着java栈中帧的逐渐增多，最终会由于该线程java栈中所有栈帧大小总和大于-Xss设置的值，而产生StackOverflowError内存溢出异常。例子如下：
- /** 
-   * VM Args: -Xss128k 
-   */
- publicclass Test {  
- 
- privateint count = 0;  
- 
- publicstaticvoid main(String[] args) {  
- new Test().method();  
-      }  
- 
- publicvoid method() {  
-          System.out.println(++count);  
-          method();  
-      }  
- 
-  } 
-Xss为128k。其中的一次测试结果为，当count的值累加到2312时，发生如下异常：
- Exception in thread "main" java.lang.StackOverflowError  
-     at sun.nio.cs.UTF_8.updatePositions(UTF_8.java:58)  
-     at sun.nio.cs.UTF_8$Encoder.encodeArrayLoop(UTF_8.java:392)  
-     at sun.nio.cs.UTF_8$Encoder.encodeLoop(UTF_8.java:447)  
-     at java.nio.charset.CharsetEncoder.encode(CharsetEncoder.java:544)  
-     at sun.nio.cs.StreamEncoder.implWrite(StreamEncoder.java:252)  
-     at sun.nio.cs.StreamEncoder.write(StreamEncoder.java:106)  
-     at java.io.OutputStreamWriter.write(OutputStreamWriter.java:190)  
-     at java.io.BufferedWriter.flushBuffer(BufferedWriter.java:111)  
-     at java.io.PrintStream.write(PrintStream.java:476)  
-     at java.io.PrintStream.print(PrintStream.java:547)  
-     at java.io.PrintStream.println(PrintStream.java:686)  
-     at jvm.Test.method(Test.java:17) 
修改-Xss为1280k。其中的一次测试结果为，当count的值累加到26888时，发生StackOverflowError异常。随着-Xss参数值的增大，可以嵌套的方法调用层次也相应增加。
综上所述，StackOverflowError异常是由于方法调用的层次太深，最终导致为某个线程分配的所有栈帧大小总和大于-Xss设置的值，从而发生StackOverflowError异常。
**2.OutOfMemoryError异常**
java程序代码启动一个新线程时，没有足够的内存空间为该线程分配java栈(一个线程java栈的大小由-Xss参数确定)，jvm则抛出OutOfMemoryError异常。例子如下：
- /** 
-   * VM Args: -Xss128k 
-   */
- publicclass Test {  
- 
- publicstaticvoid main(String[] args) {  
- int count = 0;  
- while (true) {  
-              Thread thread = new Thread(new Runnable() {  
- publicvoid run() {  
- while (true) {  
- try {  
-                              Thread.sleep(5000);  
-                          } catch (Exception e) {}  
-                      }  
-                  }  
-              });    
-              thread.start();  
-              System.out.println(++count);  
-          }  
-      }  
- 
-  } 
-Xss为128k。其中的一次测试结果为，当count的值累加到11887时，发生如下异常：
- Exception in thread "main" java.lang.OutOfMemoryError: unable to create newnative thread  
-     at java.lang.Thread.start0(Native Method)  
-     at java.lang.Thread.start(Thread.java:640)  
-     at jvm.Test.main(Test.java:20) 
修改-Xss为1280k。其中的一次测试结果为，当count的值累加到1270时，发生OutOfMemoryError异常。随着-Xss参数值的增大，java程序可以创建的总线程数越少。
**Java堆溢出**
Java堆用于储存对象实例。当需要为对象实例分配内存，而堆的内存占用又已经达到-Xmx设置的最大值。将会抛出OutOfMemoryError异常。例子如下：
- /** 
-   * VM Args: -Xmx5m 
-   */
- publicclass Test {  
- 
- publicstaticvoid main(String[] args) {  
- int count = 0;  
-          List<Object> list = new ArrayList<Object>();  
- while (true) {  
-              list.add(new Object());  
-              System.out.println(++count);  
-          }  
-      }  
- 
-  } 
-Xmx为5m。其中的一次测试结果为，当count的值累加到297868时，发生如下异常：
- Exception in thread "main" java.lang.OutOfMemoryError: Java heap space  
-     at java.util.Arrays.copyOf(Arrays.java:2760)  
-     at java.util.Arrays.copyOf(Arrays.java:2734)  
-     at java.util.ArrayList.ensureCapacity(ArrayList.java:167)  
-     at java.util.ArrayList.add(ArrayList.java:351)  
-     at jvm.Test.main(Test.java:15) 
修改-Xmx为10m。其中的一次测试结果为，当count的值累加到670205时，发生OutOfMemoryError异常。随着-Xmx参数值的增大，java堆中可以存储的对象也越多。
**方法区溢出**
方法区用于存放java类型的相关信息，如类名、访问修饰符、常量池、字段描述、方法描述等。在类装载器加载class文件到内存的过程中，虚拟机会提取其中的类型信息，并将这些信息存储到方法区。当需要存储类信息而方法区的内存占用又已经达到-XX:MaxPermSize设置的最大值。将会抛出OutOfMemoryError异常。对于这种情况的测试，基本的思路是运行时产生大量的类去填满方法区，直到溢出。这里需要借助CGLib直接操作字节码运行时，生成了大量的动态类。例子如下：
- /** 
-    * VM Args: -XX:MaxPermSize=50M 
-    */
- publicclass Test {  
- 
- publicstaticvoid main(String[] args) {  
- int count = 0;  
- while (true) {  
-                Enhancer enhancer = new Enhancer();  
-                enhancer.setSuperclass(Test.class);  
-                enhancer.setUseCache(false);  
-                enhancer.setCallback(new MethodInterceptor() {  
- public Object intercept(Object obj, Method method, Object[] args,   
-                     MethodProxy proxy) throws Throwable {  
- return proxy.invoke(obj, args);  
-                   }  
-                });  
-                enhancer.create();  
-                System.out.println(++count);  
-           }  
-       }  
- 
-   } 
-XX:MaxPermSize为50m。其中的一次测试结果为，当count的值累加到3953时，发生如下异常：
- Caused by: java.lang.OutOfMemoryError: PermGen space  
-     at java.lang.ClassLoader.defineClass1(Native Method)  
-     at java.lang.ClassLoader.defineClassCond(ClassLoader.java:631)  
-     at java.lang.ClassLoader.defineClass(ClassLoader.java:615)  
-     ... 8 more 
修改-XX:MaxPermSize为100m。其中的一次测试结果为，当count的值累加到8022时，发生OutOfMemoryError异常。随着-XX:MaxPermSize参数值的增大，java方法区中可以存储的类型数据也越多。
**相关的参考资料:**
1.深入Java虚拟机(原书第2版)
2.深入理解Java虚拟机:JVM高级特性与最佳实践
3.互联网相关的文章
原文链接：[http://www.cnblogs.com/evan2012/archive/2012/05/12/2497086.html](http://www.cnblogs.com/evan2012/archive/2012/05/12/2497086.html)
