# Java内存溢出与栈溢出 - z69183787的专栏 - CSDN博客
2017年07月20日 15:34:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1099
[http://m.blog.csdn.net/hu1991die/article/details/43052281](http://m.blog.csdn.net/hu1991die/article/details/43052281)
# 一、背景知识
## 1、JVM体系结构
![](https://img-blog.csdn.net/20150123102045605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2、JVM运行时数据区
![](https://img-blog.csdn.net/20150123102118147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3、JVM内存模型
### JVM运行时内存 = 共享内存区 + 线程内存区
![](https://img-blog.csdn.net/20150123102248019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3-1、共享内存区
共享内存区 = 持久带 + 堆
持久带 = 方法区 + 其他
堆 = Old Space + Young Space
Young Space = Eden + S0 + S1
![](https://img-blog.csdn.net/20150123102502756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3-1-1、持久代
JVM用持久带（Permanent Space）实现方法区，主要存放所有已加载的类信息，方法信息，常量池等等。
可通过-XX:PermSize和-XX:MaxPermSize来指定持久带初始化值和最大值。
Permanent Space并不等同于方法区，只不过是Hotspot JVM用Permanent Space来实现方法区而已，有些虚拟机没
有Permanent Space而用其他机制来实现方法区。
## 3-1-2、堆
堆(heap)，主要用来存放类的对象实例信息（包括new操作实例化的对象和定义的数组）。
堆分为Old Space（又名，Tenured Generation）和Young Space。
Old Space主要存放应用程序中生命周期长的存活对象；
Eden(伊甸园)主要存放新生的对象；
S0和S1是两个大小相同的内存区域，主要存放每次垃圾回收后Eden存活的对象，作为对象从Eden过渡到Old Space
的缓冲地带（S是指英文单词Survivor Space）。
堆之所以要划分区间，是为了方便对象创建和垃圾回收，后面垃圾回收部分会解释。
## 3-2、线程内存区
线程内存区=单个线程内存+单个线程内存+.......
单个线程内存=PC Regster+JVM栈+本地方法栈
JVM栈=栈帧+栈帧+.....
栈帧=局域变量区+操作数区+帧数据区
![](https://img-blog.csdn.net/20150123102812046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在Java中，一个线程会对应一个JVM栈(JVM Stack)，JVM栈里记录了线程的运行状态。
JVM栈以栈帧为单位组成，一个栈帧代表一个方法调用。栈帧由三部分组成：局部变量区、操作数栈、帧数据区。
# 二、堆溢出
堆（Heap）是Java存放对象实例的地方。
堆溢出可以分为以下两种情况，这两种情况都会抛出OutOfMemoryError:java heap space异常：
## 1、内存泄漏
内存泄漏是指对象实例在新建和使用完毕后，仍然被引用，没能被垃圾回收释放，一直积累，直到没有剩余
内存可用。
如果内存泄露，我们要找出泄露的对象是怎么被GC ROOT引用起来，然后通过引用链来具体分析泄露的原因。
分析内存泄漏的工具有：Jprofiler，visualvm等。
示例代码：
```java
package com.jvm;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
/**
 * 内存泄漏
 * @author feizi
 * @time 2015-1-23上午8:42:53
 */
public class OOMTest {
	public static void main(String[] args) {
		
		List<UUID> list = new ArrayList<UUID>();
		while(true){
			list.add(UUID.randomUUID());
		}
	}
}
```
看看控制台的输出结果，因为我这边的JVM设置的参数内存足够大，所以需要等待一定的时间，才能看到效果：
![](https://img-blog.csdn.net/20150123103804567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果是用CMD命令行，就可以自己指定参数编译运行了，这样效果就更快一些：
通过下列命令运行程序，注意先要用javac命令将.java源文件编译成.class类字节码文件。
```java
java -Xms10M -Xmx10M -XX:-UseGCOverheadLimit OOMTest
```
![](https://img-blog.csdn.net/20150123104130098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2、内存溢出
内存溢出是指当我们新建一个实力对象时，实例对象所需占用的内存空间大于堆的可用空间。
如果出现了内存溢出问题，这往往是程序本生需要的内存大于了我们给虚拟机配置的内存，这种情况下，我们可以采用调大-Xmx来解决这种问题。
示例代码：
```java
package com.jvm;
import java.util.ArrayList;
import java.util.List;
/**
 * 内存溢出
 * @author feizi
 * @time 2015-1-23上午8:56:22
 */
public class OOMTest_1 {
	public static void main(String args[]){
		List<byte[]> byteList = new ArrayList<byte[]>();
		byteList.add(new byte[1000 * 1024 * 1024]);
	}
}
```
看看控制台的运行效果：
![](https://img-blog.csdn.net/20150123104334648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用CMD命令行指定参数运行：
```java
java -verbose:gc -Xmn10M -Xms20M -Xmx20M -XX:+PrintGC OOMTest_1
```
![](https://img-blog.csdn.net/20150123104322293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 三、线程栈
栈（JVM Stack）存放主要是栈帧( 局部变量表, 操作数栈 , 动态链接 , 方法出口信息 )的地方。注意区分栈和栈帧：栈里包含栈帧。
与线程栈相关的内存异常有两个：
a）、StackOverflowError(方法调用层次太深，内存不够新建栈帧)
b）、OutOfMemoryError（线程太多，内存不够新建线程）
## 1、java.lang.StackOverflowError
栈溢出抛出java.lang.StackOverflowError错误，出现此种情况是因为方法运行的时候，请求新建栈帧时，
栈所剩空间小于战帧所需空间。
例如，通过递归调用方法,不停的产生栈帧,一直把栈空间堆满,直到抛出异常 ：
示例代码：
```java
package com.jvm;
/**
 * 栈溢出
 * @author feizi
 * @time 2015-1-23上午9:13:11
 */
public class SOFTest {
	public void stackOverFlowMethod(){
		stackOverFlowMethod();
	}
	
	/**
	 * 通过递归调用方法,不停的产生栈帧,一直把栈空间堆满,直到抛出异常 ：
	 * @param args
	 */
	public static void main(String[] args) {
		SOFTest sof = new SOFTest();
		sof.stackOverFlowMethod();
	}
}
```
看看控制台运行的效果：
![](https://img-blog.csdn.net/20150123104709367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHUxOTkxZGll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
