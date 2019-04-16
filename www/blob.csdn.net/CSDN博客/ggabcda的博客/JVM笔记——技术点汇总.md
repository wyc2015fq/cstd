# JVM笔记——技术点汇总 - ggabcda的博客 - CSDN博客





2017年08月15日 14:38:22[ggabcda](https://me.csdn.net/ggabcda)阅读数：183








## 目录

**·** 初步认识

**·** Java里程碑（关键部分）

**·** 理解虚拟机

**·** Java虚拟机种类

**·** Java语言规范

**·** Java虚拟机规范

**·** 基本结构

**·** Java堆（Heap）

**·** Java栈（Stacks）

**·** 方法区（Method Area）

**·** 直接内存（Direct Memory）

**·** 本地方法栈（Native Method Stacks）

**·** 常用参数

**·** 设置参数

**·** 查看参数

**·** 跟踪垃圾回收

**·** 跟踪类加载/卸载

**·** 设置初始堆和最大堆

**·** 设置堆分布

**·** 处理堆溢出

**·** 配置方法区

**·** 配置栈

**·** 配置直接内存

**·** 配置工作模式

**·** 垃圾回收算法

**·** 垃圾回收

**·** 引用计数法（Reference Counting）

**·** 标记清除法（Mark-Sweep）

**·** 复制算法（Coping）

**·** 标记压缩法（Mark-Compact）

**·** 分代算法（Generational Collecting）

**·** 分区算法（Region）

**·** 判断可触及性

**·** 停顿现象

**·** 垃圾回收器

**·** 串行回收器

**·** 新生代ParNew回收器

**·** 新生代ParallelGC回收器

**·** 老年代ParallelOldGC回收器

**·** CMS回收器

**·** G1回收器

**·** 垃圾回收其他细节

**·** 禁用System.gc()

**·** 开启System.gc()并发回收

**·** 对象何时进入老年代

**·** TLAB

**·** 性能监控工具

**·** top命令

**·** vmstat命令

**·** iostat命令

**·** pidstat命令

**·** jps命令

**·** jstat命令

**·** jinfo命令

**·** jmap命令

**·** jhat命令

**·** jstack命令

**·** jcmd命令

**·** Visual VM工具

**·** Mission Control工具

**·** 解决OOM问题

**·** 堆溢出

**·** 直接内存溢出

**·** 过多线程导致溢出

**·** 永久区/元数据区溢出

**·** GC效率低导致OOM

**·** String实现细节

**·** String对象特点

**·** String常量池

**·** 字节码优化

**·** 静态编译优化

**·** JIT运行优化





## 初步认识



### Java里程碑（关键部分）

1. 2004年，JDK 1.5发布。同时更名为J2SE 5.0。Java语言大量改进，比如支持泛型、注解、自动装箱、枚举类型、可变长参数、增强的foreach循环等。

2. 2011年，JDK 1.7发布。正式启用新垃圾回收器G1，支持64位系统的压缩指针，NIO 2.0，新增invokedynamic指令。

3. 2014年，JDK 1.8发布。全新的Lambda表达式彻底改变Java编程风格和习惯。

4. 2016年，JDK 1.9发布。最令人期待的功能应该是Java的模块化。



### 理解虚拟机

1. 虚拟机。

    a) 虚拟的计算机。

    b) 软件，执行一系列虚拟计算机指令。

2. 分类。

    a) 系统虚拟机：对物理计算机的仿真，提供一个可运行完整操作系统的软件平台。

    b) 程序虚拟机：为执行单个计算机程序而设计，如Java字节码指令。



### Java虚拟机种类

被大规模部署和应用的是Hotspot虚拟机。



### Java语言规范

1. 语言规范：定义Java语言特性，如Java语法、词法、数据类型、变量类型、数据类型转换约定、数组、异常等。

2. 词法：规定每个单词如何书写，如关键字、标识符等。

3. 语法：规定语句如何书写，如if语句等。

4. 官方文档：http://docs.oracle.com/javase/specs/。



### Java虚拟机规范

1. 虚拟机规范大概组成：

    a) 定义虚拟机的内部结构；

    b) 定义虚拟机执行的字节码类型和功能；

    c) 定义Class文件的结构；

    d) 定义类的装载、连接和初始化。

2. Java虚拟机执行Java字节码，而运行的Java字节码未必由Java语言编写，如Groovy、Scala等都可以生成Java字节码。

3. 官方文档：http://docs.oracle.com/javase/specs/。



## 基本结构

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805215107647-861016000.png)



### Java堆（Heap）

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805215242975-457288219.png)

1. 存储：几乎所有的对象。

2. 使用者：所有线程共享。

3. 结构：根据垃圾回收机制，一般划分为

    a) 新生代（New/Young Generation）：新生对象或年龄不大的对象。又可能分为eden区、s0区（也称from区）、s1区（也称to区，from/to合称Survivor区）。from和to是两块大小相等、可以互换角色的内存空间。

    b) 老年代（Tenured Generation）：老年对象。

4. 过程：绝大多数情况下，对象首先分配在eden，一次新生代回收后，如果对象存活，则进入s0或s1。之后，每经一次新生代回收，如果对象存活，则年龄加1。当对象年龄达到一定条件后，则进入老年代。

5. 举例。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805215416897-662945774.png)

```
1 public class SimpleHeap {
 2     
 3     private int id;
 4     
 5     public SimpleHeap(int id) {
 6         this.id = id;
 7     }
 8     
 9     public void show() {
10         System.out.println("My ID is " + id);
11     }
12 
13     public static void main(String[] args) {
14         SimpleHeap s1 = new SimpleHeap(1);
15         SimpleHeap s2 = new SimpleHeap(2);
16         s1.show();
17         s2.show();
18     }
19 
20 }
```



### Java栈（Stacks）

1. 存储：线程执行的基本行为是函数调用，每次函数调用的数据都通过Java栈传递。

2. 使用者：线程私有。

3. 结构：栈数据结构（先进后出），栈元素为栈帧。一个栈帧至少包括局部变量表、操作数栈和帧数据区。

4. 过程：每一次函数调用，都有一个对应的栈帧入栈，每一次函数调用结束（return指令或抛出异常），都有一个栈帧出栈。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805215548240-1813640198.png)

5. 局部变量表。

    a) 作用：保存函数的参数和局部变量。

    b) 其中的变量只在当前函数调用有效，函数调用结束后销毁。

6. 操作数栈。

    a) 作用：保存计算过程中的中间结果，同时作为计算过程中变量临时的存储空间。

    b) 举例：iadd指令会在操作数栈中弹出两个整数并相加，计算结果再入栈。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805215957647-1075059292.png)

7. 帧数据区。

    a) 作用：方便访问常量池；函数返回或异常后，恢复调用者函数的栈帧。

    b) 存储：常量池指针；异常处理表。

8. 栈上分配。

    a) 一项虚拟机优化技术。

    b) 思想：对于线程私有（不可能被其他线程访问）的对象，将它们打散分配在栈上，而不是在堆上。函数调用结束后自行销毁，无需垃圾回收器介入。

    c) 基础：逃逸分析，判断对象作用域是否可能逃逸出函数体。

    d) 举例。

```
1 public class EscapeAnalysis {
 2 
 3     private static User user;
 4     
 5     public static void test1() {
 6         // 逃逸对象
 7         user = new User();
 8         user.id = 1;
 9         user.name = "test1";
10     }
11     
12     public static void test2() {
13         // 非逃逸对象
14         User user = new User();
15         user.id = 2;
16         user.name = "test2";
17     }
18 
19 }
20 
21 class User {
22     
23     int id;
24     
25     String name;
26     
27 }
```



### 方法区（Method Area）

1. 存储：类信息，如类字段、方法、常量池（字符串字面量、数字常量）等。

2. 使用者：线程共享。

3. 结构：JDK 1.6、JDK 1.7中，即永久区（Perm）；JDK 1.8中，即元数据区（Metaspace，永久区已被移除）。注意：方法区是Java虚拟机规范的概念，永久区、元数据区是Hotspot对方法区的实现。



### 直接内存（Direct Memory）

1. 使用者：NIO库，如ByteBuffer。

2. 特点：

    a) 直接向操作系统申请的内存空间，性能优于Java堆。

    b) 大小受限于操作系统分配的最大内存。



### 本地方法栈（Native Method Stacks）

与Java栈非常类似，不同在于用于本地方法调用。



## 常用参数



### 设置参数

```
java [-options] class [args...]
```

1. -options表示Java虚拟机启动参数。

2. class为带有main()函数的Java类。

3. args表示传递给主函数main()的参数。



### 查看参数

1. “-XX:+PrintVMOptions”：打印虚拟机接受到的参数（显式参数）。

2. “-XX:+PrintCommandLineFlags”：打印虚拟机显式和隐式参数。



### 跟踪垃圾回收

1. “-XX:+PrintGC”。

    a) GC时打印简单日志。

    b) 举例：2次新生代GC，1次Full GC。

```
[GC (Allocation Failure)  42000K->36496K(56320K), 0.0009016 secs]
[GC (Allocation Failure) -- 41616K->41616K(56320K), 0.0020593 secs]
[Full GC (Ergonomics)  41616K->5655K(56320K), 0.0070610 secs]
```

2. “-XX:+PrintGCDetails”。

    a) GC时打印详细日志。

    b) 举例：2次新生代GC，1次Full GC。第1 次新生代从6570K降至648K，堆从42410K降至36496K，user表示用户态CPU耗时，sys表示系统CPU耗时，real表示GC实际耗时。第3次Full GC对新生代、老年代和元数据区回收。

```
[GC (Allocation Failure) [PSYoungGen: 6570K->648K(15360K)] 42410K->36496K(56320K), 0.0008117 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[GC (Allocation Failure) --[PSYoungGen: 5768K->5768K(15360K)] 41616K->41616K(56320K), 0.0010395 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[Full GC (Ergonomics) [PSYoungGen: 5768K->0K(15360K)] [ParOldGen: 35848K->5659K(40960K)] 41616K->5659K(56320K), [Metaspace: 2517K->2517K(1056768K)], 0.0065072 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
```

    c) 虚拟机在退出时，打印堆的详细信息。3个16进制数字分别表示下界、当前上界和上界。以eden为例，(上界0x00000000ff600000-下界0x00000000fec00000)/1024=10240K。

```
Heap
 PSYoungGen      total 15360K, used 5188K [0x00000000fec00000, 0x0000000100000000, 0x0000000100000000)
  eden space 10240K, 50% used [0x00000000fec00000,0x00000000ff111298,0x00000000ff600000)
  from space 5120K, 0% used [0x00000000ff600000,0x00000000ff600000,0x00000000ffb00000)
  to   space 5120K, 0% used [0x00000000ffb00000,0x00000000ffb00000,0x0000000100000000)
 ParOldGen       total 40960K, used 5659K [0x00000000fc400000, 0x00000000fec00000, 0x00000000fec00000)
  object space 40960K, 13% used [0x00000000fc400000,0x00000000fc986ee0,0x00000000fec00000)
 Metaspace       used 2523K, capacity 4486K, committed 4864K, reserved 1056768K
  class space    used 273K, capacity 386K, committed 512K, reserved 1048576K
```

3. “-XX:+PrintHeapAtGC”。

    a) 堆GC时打印前后日志。

    b) 举例：1次堆GC。

```
{Heap before GC invocations=40 (full 0):
 PSYoungGen      total 583680K, used 583110K [0x0000000780a00000, 0x00000007bfb00000, 0x00000007c0000000)
  eden space 582144K, 99% used [0x0000000780a00000,0x00000007a42635c8,0x00000007a4280000)
  from space 1536K, 70% used [0x00000007bf880000,0x00000007bf98e5f0,0x00000007bfa00000)
  to   space 1024K, 0% used [0x00000007bfa00000,0x00000007bfa00000,0x00000007bfb00000)
 ParOldGen       total 131072K, used 2060K [0x0000000701e00000, 0x0000000709e00000, 0x0000000780a00000)
  object space 131072K, 1% used [0x0000000701e00000,0x0000000702003050,0x0000000709e00000)
 Metaspace       used 2539K, capacity 4486K, committed 4864K, reserved 1056768K
  class space    used 275K, capacity 386K, committed 512K, reserved 1048576K
Heap after GC invocations=40 (full 0):
 PSYoungGen      total 556544K, used 578K [0x0000000780a00000, 0x00000007bfb00000, 0x00000007c0000000)
  eden space 555520K, 0% used [0x0000000780a00000,0x0000000780a00000,0x00000007a2880000)
  from space 1024K, 56% used [0x00000007bfa00000,0x00000007bfa90928,0x00000007bfb00000)
  to   space 1536K, 0% used [0x00000007bf800000,0x00000007bf800000,0x00000007bf980000)
 ParOldGen       total 131072K, used 2574K [0x0000000701e00000, 0x0000000709e00000, 0x0000000780a00000)
  object space 131072K, 1% used [0x0000000701e00000,0x0000000702083978,0x0000000709e00000)
 Metaspace       used 2539K, capacity 4486K, committed 4864K, reserved 1056768K
  class space    used 275K, capacity 386K, committed 512K, reserved 1048576K
}
```

4. “-XX:+PrintGCTimeStamps”：打印虚拟机启动后GC发生的时间偏移量。

5. “-XX:+PrintGCApplicationStoppedTime”。

    a) GC时打印停顿时间。

    b) 举例：3次GC停顿时间。

```
Total time for which application threads were stopped: 0.0006926 seconds, Stopping threads took: 0.0000476 seconds
Total time for which application threads were stopped: 0.0012608 seconds, Stopping threads took: 0.0000349 seconds
Total time for which application threads were stopped: 0.0012115 seconds, Stopping threads took: 0.0000374 seconds
```

6. “-XX:+PrintReferenceGC”：打印软运用、弱引用、虚引用和Finallize队列。

7. “-Xloggc”：GC日志输出到文件。例如：

```
-Xloggc:E:/gc.log
```



### 跟踪类加载/卸载

1. “-verbose:class”：打印类加载和卸载。

2. “-XX:+TraceClassLoading”：打印类加载。

3. “-XX:+TraceClassUnloading”：打印类卸载。

4. 类存在形式：

    a) 一般，类以jar打包或class文件形式存放在文件系统。

    b) ASM等在运行时动态生成类。



### 设置初始堆和最大堆

1. “-Xms”：堆初始大小。

2. “-Xmx”：堆最大大小。

3. 技巧：实际工作中，设置初始堆和最大堆相等，可以减少垃圾回收次数，提供性能。



### 设置堆分布

1. “-Xmn”：新生代大小，同时影响老年代大小。

2. “-XX:NewRatio”：新生代和老年代比例，即老年代/新生代，与“-Xmn”作用相同（旧参数）。

3. “-XX:SurvivorRatio”：edit区和from/to区的比例，即eden/from和eden/to。

4. 技巧。

    a) 新生代大小对系统性能及GC有很大影响。

    b) 实际工作中，应根据系统特点合理设置堆分布，基本策略是：尽可能将对象预留在新生代，减少老年代GC的次数。

    c) 一般，新生代设置成整个堆的1/4~1/3左右。

5. 举例。

    a) 代码。

```
1 public class NewSizeDemo {
 2     
 3     public static void main(String[] args) {
 4         byte[] b = null;
 5         for (int index = 0; index < 10; index++) {
 6             b = new byte[5 * 1024 * 1024];
 7         }
 8     }
 9 
10 }
```

    b) 参数：两组参数等效。

```
-Xms60m -Xmx60m -Xmn20m -XX:SurvivorRatio=2 -XX:+PrintGCDetails
```

```
-Xms60m -Xmx60m -XX:NewRatio=2 -XX:SurvivorRatio=2 -XX:+PrintGCDetails
```

    c) 结构。
|**空间**|**大小**|
|----|----|
|堆|60MB|
|新生代|20MB|
|eden|10MB|
|from|5MB|
|to|5MB|
|老年代|40MB|

    d) 结果：eden无法容纳数组，发生GC。注意：虽然通过地址计算新生代大小为(0x0000000100000000-0x00000000fec00000)/1024=20480KB，但由于垃圾回收需要和对齐等原因，实现大小（15360K）会有损失。

```
[GC (Allocation Failure) [PSYoungGen: 6365K->616K(15360K)] 42205K->36456K(56320K), 0.0024206 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[GC (Allocation Failure) --[PSYoungGen: 5736K->5736K(15360K)] 41576K->41584K(56320K), 0.0031165 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[Full GC (Ergonomics) [PSYoungGen: 5736K->0K(15360K)] [ParOldGen: 35848K->5655K(40960K)] 41584K->5655K(56320K), [Metaspace: 2483K->2483K(1056768K)], 0.0070298 secs] [Times: user=0.00 sys=0.00, real=0.01 secs] 
Heap
 PSYoungGen      total 15360K, used 5222K [0x00000000fec00000, 0x0000000100000000, 0x0000000100000000)
  eden space 10240K, 51% used [0x00000000fec00000,0x00000000ff119b20,0x00000000ff600000)
  from space 5120K, 0% used [0x00000000ff600000,0x00000000ff600000,0x00000000ffb00000)
  to   space 5120K, 0% used [0x00000000ffb00000,0x00000000ffb00000,0x0000000100000000)
 ParOldGen       total 40960K, used 5655K [0x00000000fc400000, 0x00000000fec00000, 0x00000000fec00000)
  object space 40960K, 13% used [0x00000000fc400000,0x00000000fc985f98,0x00000000fec00000)
 Metaspace       used 2489K, capacity 4486K, committed 4864K, reserved 1056768K
  class space    used 273K, capacity 386K, committed 512K, reserved 1048576K
```



### 处理堆溢出

1. “-XX:+HeapDumpOnOutOfMemoryError”：堆溢出（OOM）时导出信息。

2. “-XX:HeapDumpPath”：堆溢出时导出的路径。使用MAT等工具可分析文件。

3. “-XX:OnOutOfMemoryError”：堆溢出时执行脚本，可用于奔溃程序自救、报警、通知等。

4. 举例：使用下面的参数运行NewSizeDemo。

```
-Xms20m -Xmx20m -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=D:/test.dump -XX:OnOutOfMemoryError=D:/shell.bat
```



### 配置方法区

1. “-XX:PermSize”：永久区初始大小（JDK 1.6/1.7）。

2. “-XX:MaxPermSize”：永久区最大大小（JDK 1.6/1.7）。

3. “-XX:MaxMetaspaceSize”：元数据区最大大小（JDK 1.8）。

4. 技巧。

    a) 动态生成大量类（如动态代理、AOP）时，可能导致方法区溢出（OOM）。

    b) JDK 1.6/1.7，默认永久区最大大小为64MB；JDK 1.8，默认元数据区耗尽所有可用系统内存。



### 配置栈

1. “-Xss”：单个线程的栈最大大小。

2. “-XX:+DoEscapeAnalysis”：开启逃逸分析（仅限Server模式下使用）。

3. “-XX:+EliminateAllocations”：开启标量替换（默认已开启，允许对象打散分配在栈上）。

4. 技巧：

    a) “-Xss”影响函数调用深度、局部变量大小等。

    b) 栈上分配速度快，同时避免垃圾回收，但栈相比堆较小，不适合大对象。

5. 举例：测试函数调用深度。

    a) 代码。

```
1 public class TestStackDeep {
 2     
 3     private static int count = 0;
 4     
 5     public static void recursion() {
 6         count++;
 7         recursion();
 8     }
 9 
10     public static void main(String[] args) {
11         try {
12             recursion();
13         } catch (StackOverflowError e) {
14             System.out.println("deep of calling = " + count);
15             e.printStackTrace();
16         }
17     }
18 
19 }
```

    b) 参数。

```
-Xss128k
```

    c) 结果。

```
deep of calling = 1087
java.lang.StackOverflowError
    at gz.jvm.TestStackDeep.recursion(TestStackDeep.java:8)
```



### 配置直接内存

1. “-XX:MaxDirectMemorySize”：直接内存最大大小。如不设置，默认为“-Xmx”。

2. 技巧：直接内存适合申请次数较少、访问较频繁的场景。因为申请堆空间的速度远远高于直接内存。



### 配置工作模式

1. “-client”：Client模式。

2. “-server”：Server模式。

3. “-version”：查看模式。

4. 技巧。

    a) Client模式：启动速度较快。适合用户界面，运行时间不长。

    b) Server模式：启动速度较慢（启动时尝试收集更多系统性能信息，使用更复杂的优化算法优化程序），完全启动并稳定后，执行速度远远快于Client模式。适合后台长期运行的系统。

    c) 两种模式下的各种参数默认值可能不同，可使用“-XX:+PrintFlagsFinal”参数查看。



## 垃圾回收算法



### 垃圾回收

1. 垃圾：存在于内存中的、不会再被使用的对象。

2. 回收：将内存空间空闲的区域腾出来。

3. 如果大量不会被使用的对象一直占用空间不放，需要内存空间时，无法使用这些被垃圾对象占用的内存，从而有可能导致内存溢出。



### 引用计数法（Reference Counting）

1. 原理：对于一个对象A，只要有任何一个对象引用了A，则A的引用计数器加1；当引用失效时，引用计数器减1。当对象A的引用计数器值为0，则对象A不可能再被使用。

2. 特点：

    a) 简单；

    b) 最经典、最古老的垃圾回收算法。

3. 问题：

    a) 无法处理循环引用的情况。

        i. 可达对象：通过根对象进行引用搜索，最终可以达到的对象。

        ii. 不可达对象：通过根对象进行引用搜索，最终没有被引用到的对象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805222235709-667731652.png)

    b) 每次引用产生和失效时，引用计数器的加、减法操作对性能有一定影响。

4. 应用：Java虚拟机未采用。



### 标记清除法（Mark-Sweep）

1. 原理：将垃圾回收分为标记阶段和清除阶段。

    a) 标记阶段：从跟节点开始，标记所有可达对象。

    b) 清除阶段：清除未被标记的垃圾对象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805222910959-354009545.png)

2. 特点：现代垃圾回收算法的思想基础。

3. 问题：回收后的空间不连续（碎片）。空间分配时，尤其大对象内存分配时，不连续的内存空间工作效率低于连续空间。



### 复制算法（Coping）

1. 原理：将内存空间分为两块，每次只使用其中一块。垃圾回收时，将正在使用的一块内存中的存活对象复制到未使用的一块中，再清除正在使用的内存中的所有对象，最后交换两块内存的角色。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223019678-1948179712.png)

2. 特点：

    a) 如果垃圾对象多，则复制的存活对象相对较少，复制算法效率就高（如新生代）。

    b) 回收后的内存空间没有碎片。

3. 缺点：内存折半。

4. 应用：新生代串行垃圾回收器。eden中的存活对象被复制到未使用的survivor中（假设是to），正在使用的survivor（假设是from）中的年轻对象也被复制到to中（大对象或老年对象直接进入老年代，如果to已满，则对象也直接进入老年代）。清除eden和from中的剩余垃圾对象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223120397-163388018.png)



### 标记压缩法（Mark-Compact）

1. 原理：标记清除算法的优化版。三步：

    a) 从根节点开始，标记所有可达对象；

    b) 将所有存活对象压缩到内存的一端；

    c) 清理边界外所有空间。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223217819-1145893765.png)

2. 特点：

    a) 回收后的内存空间没有碎片。

    b) 无内存折半。

3. 应用：老年代回收。



### 分代算法（Generational Collecting）

1. 原理：根据对象的特点将内存空间分为几块，每块内存采用不同的回收算法。

2. 应用：Java虚拟机。

    a) 新生代特点是对象朝生夕灭，约90%对象很快被回收，适合复制算法。

    b) 老年代的回收性价比低于新生代，适合标记压缩或标记清除算法。

    c) 通常，新生代回收频率高，回收耗时短；老年代回收频率低，回收耗时较长。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223324928-1953600357.png)

3. 卡表（Card Table）：一个比特位集合，每一个比特位表示老年代的某一区域中的对象是否持有新生代的引用。新生代GC时，根据卡表扫描老年代对象，而避免扫描所有老年代对象。下图，每一位表示老年代4KB的空间。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223353803-1293570910.png)



### 分区算法（Region）

1. 原理：将堆划分成连续的不同小区间，每个小区间都独立使用、独立回收。

2. 特点：

    a) 可控制每次回收的小区间个数；

    b) 避免回收整个堆，减少GC停顿时间。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223456475-1673932793.png)



### 判断可触及性

1. 可触及性的3中状态。

    a) 可触及的：从根节点开始，可达对象。

    b) 可复活的：对象的所有引用都被释放，但在finalize()函数复活（注意finalize()只会执行一次）。

    c) 不可触及的：对象的所有引用都被释放，且执行finalize()函数后未复活。

2. 回收依据：不可触及的对象。

3. 对象复活举例。

```
1 public class CanReliveObj {
 2     
 3     public static CanReliveObj obj;
 4     
 5     @Override
 6     protected void finalize() throws Throwable {
 7         super.finalize();
 8         System.out.println("CanReliveObj.finalize()");
 9         obj = this;
10     }
11 
12     public static void main(String[] args) throws Exception {
13         obj = new CanReliveObj();
14         
15         obj = null;
16         System.gc();
17         Thread.sleep(1000);
18         // 由于在finalize()后复活，所以打印结果不为null
19         System.out.println("obj = " + obj);
20         
21         obj = null;
22         System.gc();
23         Thread.sleep(1000);
24         // 由于finalize()只执行一次，所以不可能再复活，打印为null
25         System.out.println("obj = " + obj);
26     }
27 
28 }
```

4. 不建议使用finalize()释放资源，原因：

    a) 无意中复活对象；

    b) finalize()被系统调用，调用时间不确定，推荐使用“try-catch-finally”释放资源。

5. 4种引用类型。

    a) 对比。
|**类型**|**被回收时间**|**是否引起OOM**|**应用**|
|----|----|----|----|
|强引用|宁愿OOM也不回收|是||
|软引用|内存紧张时|否|可有可无的缓存|
|弱引用|GC时|否|可有可无的缓存|
|虚引用|随时|否|跟踪对象回收时间|

    b) 代码。

```
1 import java.lang.ref.PhantomReference;
 2 import java.lang.ref.ReferenceQueue;
 3 import java.lang.ref.SoftReference;
 4 import java.lang.ref.WeakReference;
 5 
 6 public class ReferenceDemo {
 7 
 8     public static void main(String[] args) {
 9         // 强引用
10         ReferenceDemo strongReference = new ReferenceDemo();
11         // 软引用
12         SoftReference<ReferenceDemo> softReference = new SoftReference<ReferenceDemo>(new ReferenceDemo());
13         // 弱引用
14         WeakReference<ReferenceDemo> weakReference = new WeakReference<ReferenceDemo>(new ReferenceDemo());
15         // 虚引用
16         ReferenceQueue<ReferenceDemo> referenceQueue = new ReferenceQueue<ReferenceDemo>();
17         PhantomReference<ReferenceDemo> phantomReference = new PhantomReference<ReferenceDemo>(new ReferenceDemo(), referenceQueue);
18     }
19 
20 }
```



### 停顿现象

1. Stop-The-World（STW）：垃圾回收时，会产生应用程序的停顿，整个应用被卡死，没有任何响应。

2. 目的：终止所有线程执行，此时没有新的垃圾产生，保证系统状态在一个瞬间的一致性，益于标记垃圾对象。



## 垃圾回收器



### 串行回收器

1. 特点：

    a) 仅适用单线程垃圾回收；

    b) 独占式垃圾回收（回收时所有线程暂停，即STW）；

    c) 成熟。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223853475-1688628852.png)

2. 适合：单CPU。

3. 参数：

    a) “-XX:+UseSerialGC”：新生代、老年代都使用串行回收器。

    b) “-XX:+UseParNewGC”：新生代使用ParNew回收器，老年代使用串行回收器。

    c) “-XX:+UseParallelGC”：新生代使用ParallelGC回收器，老年代使用串行回收器。



### 新生代ParNew回收器

1. 特点：

    a) 串行回收器的简单多线程化；

    b) 独占式（STW）。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805223953850-1329485571.png)

2. 适合：并发能力较强的CPU（单CPU上效果不比串行回收器好）。

3. 参数：

    a) “-XX:+UseParNewGC”：新生代使用ParNew回收器，老年代使用串行回收器。

    b) “-XX:+UseConcMarkSweepGC”：新生代使用ParNew回收器，老年代使用CMS回收器。

    c) “-XX:ParallelGCThread”：指定线程数，一般最好与CPU数量相当。默认时，若CPU数量≤8，则为CPU数量；否则为3+((5*CPU数量)/8)。



### 新生代ParallelGC回收器

1. 特点：

    a) 多线程；

    b) 独占式；

    c) 非常关注系统吞吐量。

2. 参数：

    a) “-XX:+UseParallelGC”：新生代使用ParallelGC回收器，老年代使用串行回收器。

    b) “-XX:+UseParallelOldGC”：新生代使用ParallelGC回收器，老年代使用ParallelOldGC回收器。

    c) “-XX:MaxGCPauseMillis”：指定最大垃圾回收停顿时间。如果值过小，为了达到预期停顿时间，虚拟机可能使用一个较小的堆（小堆比大堆回收快），导致垃圾回收频繁，增加了垃圾回收总时间，降低了吞吐量。

    d) “-XX:GCTimeRatio”：指定吞吐量大小，0~100的整数，默认99。如果为n，则垃圾回收时间不超过1/(1+n)。

    e) “-XX:+UseAdaptiveSizePolicy”：开启自适应GC策略。新生代、eden和survivors的大小比例，晋升老年代对象的年龄等参数自动调整，以达到堆大小、吞吐量和停顿时间之间的平衡。适合手工调优困难的场景，仅指定最大堆“-Xmx”、目标吞吐量“-XX:GCTimeRatio”和停顿时间“-XX:MaxGCPauseMillis”即可。

    f) 注意：吞吐量“-XX:MaxGCPauseMillis”和停顿时间“-XX:GCTimeRatio”是相互矛盾的，不可兼得。减少停顿时间，同时会减少吞吐量；增加吞吐量，同时会增加停顿时间。



### 老年代ParallelOldGC回收器

1. 特点：

    a) 多线程；

    b) 独占式；

    c) 非常关注系统吞吐量；

    d) 仅JDK 1.6可以使用。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805224235569-1258752560.png)

2. 参数：“-XX:+UseParallelOldGC”：新生代使用ParallelGC回收器，老年代使用ParallelOldGC回收器。



### CMS回收器

1. CMS：Concurrent Mark Sweep，并发标记清除。

2. 原理：

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805224314240-727256115.png)

3. 特点：

    a) 多线程；

    b) 整体上，非独占式（应用程序运行时回收）。

3. 参数：

    a) “-XX:+UseConcMarkSweepGC”：开启CMS回收器。

    b) “-XX:ConcGCThread”：指定并发线程数。默认并发线程数是(并行线程数+3)/4，例如新生代ParNew回收器的并行线程数是“-XX:ParallelGCThread”。

    c) “-XX:ParallelCMSThread”：同上。

    d) 注意：并发是指回收器和应用线程交替执行，并行是指应用程序停止，同时由多个线程一起GC。CMS是并发的，所以当CPU紧张时，受到CMS线程的影响，应用程序的性能在GC时可能降低。

    e) “-XX:CMSInitiatingOccupancyFraction”：达到该堆使用率阀值时开始回收。默认68，即堆使用率68%时回收。由于CMS是并发的，GC时应用程序未中断，该阀值保证应用程序仍有足够可用内存，而不是堆饱和时才回收。调优技巧：若内存增长缓慢，则该值稍大，可有效降低CMS触发频率；反之，该值稍小，避免频繁触发老年代串行回收。

    f) “-XX:UseCMSCompactAtFullCollection”：开启CMS回收后内存碎片整理（即压缩）。该整理不是并发的。

    g) “-XX:CMSFullGCsBeforeCompaction”：指定多少次CMS后进行一次碎片整理。

    h) “-XX:+CMSClassUnloadingEnabled”：开启CMS回收Perm区。



### G1回收器

1. G1回收器：Garbage-First（意为优先收集垃圾比例高的区域），是JDK 1.7正式使用的全新垃圾回收器，长期目标是取代CMS回收器。

2. 特点：

    a) 并行性：多线程GC，有效利用多核。

    b) 并发性：部分工作可与应用程序同时执行，不会在整个回收期间阻塞应用程序。

    c) 分代GC：同时兼顾新生代和老年代（其他回收器要么工作在新生代，要么老年代）。堆结构方面，并不要求整个eden区、新生代或老年代都连续。

    d) 空间整理：每次回收都会有效复制对象（适当移动），减少空间碎片。

    e) 可预见性：由于分区原因，只选取部分区域回收，较好地控制了全局停顿。

3. 过程：

    a) 新生代GC；

    b) 并发标记周期；

    c) 混合收集；

    d) 如果需要，可能FullGC。

4. 参数：

    a) “-XX:+UseG1GC”：开启G1。

    b) “-XX:MaxGCPauseMillis”：指定目标最大停顿时间。G1会调整新生代和老年代比例、堆大小、晋升年龄等，试图达到预设目标。但不可能兼得，停顿时间缩短，GC次数会增加。默认200。

    c) “-XX:ParallelGCThread”：指定并行线程数。

    d) “-XX:InitiatingHeapOccupancyPercent”：达到该堆使用率阀值时开始并发标记周期。默认45，即堆使用率45%时开始。如果该值偏大，会导致并发周期迟迟不启动，引起Full GC概率增加；如果该值过小，会并发周期频繁，大量GC线程抢占CPU，导致应用程序性能下降。

5. 详情：http://www.oracle.com/technetwork/tutorials/tutorials-1876574.html。



## 垃圾回收其他细节



### 禁用System.gc()

1. System.gc()：默认时，触发Full GC，对新生代、老年代回收。

2. 禁用原因：一般认为GC是自动的。

3. 禁用方法：“-XX:+DisableExplicitGC”参数。



### 开启System.gc()并发回收

1. 开启原因：默认时，System.gc()使用传统方式Full GC，忽略“-XX:+UseG1GC”和“-XX:+UseConcMarkSweepGC”参数，无并发执行。

2. 开启方法：“-XX:+ExplicitGCInvokesConcurrent”参数。



### 对象何时进入老年代

1. 老年对象进入老年代：对象经历的GC次数达到“-XX:MaxTenuringThreshold”参数时，进入老年代。该参数默认为15。

2. 大对象进入老年代：对象体积很大，新生代eden区和survivors区都无法容纳，则进入老年代。对于串行和ParNew回收器，晋升老年代体积阀值参数“-XX:PretenureSizeThreshold”，单位字节，默认为0，即有运行情况决定。



### TLAB

1. TLAB：Thread Local Allocation Buffer，线程本地分配缓存。

2. 原理：

    a) 开启TLAB时，虚拟机为每一个Java线程在eden区分配一块TLAB空间。

    b) 由于堆是全局共享的，堆上分配的对象都要同步，多线程竞争激烈时效率会下降。线程专属的TLAB避免了多线程冲突，提高对象分配效率。

3. 参数：

    a) “-XX:+UseTLAB”：开启TLAB。默认已开启。

4. 对象分配简要流程。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805224758506-1498879401.png)



## 性能监控工具



### top命令

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805224851928-725463990.png)

1. top命令：Linux命令，监控CPU、内存、进程。

2. 第1行：

    a) 任务队列信息。

    b) 依次：系统当前时间、系统运行时间、当前登录用户数、平均负载（即任务队列的平均长度）。

    c) 平均负载的3个值依次：1分钟、5分钟、15分钟到现在平均值。

3. 第2行：

    a) 进程统计信息。

    b) 依次：正在运行的进程数、睡眠进程数、停止的进程数、僵尸进程数。

4. 第3行：

    a) CPU统计信息。

    b) 字段含义。
|us|用户空间CPU占用率|
|----|----|
|sy|内核空间CPU占用率|
|ni|用户进程控件改变过优先级的进程CPU占用率|
|id|空闲CPU占用率|
|wa|等待输入输出的CPU时间百分比|
|hi|硬件中断请求|
|si|软件中断请求|

5. 第4行：

    a) 内存信息。

    b) 依次：物理内存总量、已使用物理内存、空间物理内存、内存缓冲表使用量。

6. 第5行：

    a) 交换区信息。

    b) 依次：交换区总量、已使用交换区、空闲交换区、缓冲交换区。

7. 进程信息区：
|**字段**|**含义**|
|----|----|
|PID|进程ID|
|USER|所有者用户名|
|PR|优先级|
|NI|nice值，负值为高优先级、正值为低优先级|
|VIRT|虚拟内存占用总量，单位KB，VIRT=SWAP+RES|
|RES|使用的、未被换出的物理内存，单位KB，RES=CODE+DATA|
|SHR|共享内存，单位KB|
|%CPU|上次更新到现在的CPU时间占用率|
|%MEM|物理内存占用率|
|TIME+|使用的CPU时间总计，单位1/100秒|
|COMMAND|命令|



### vmstat命令

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225127600-1908284057.png)

1. vmstat命令：Linux命令，监控CPU、内存、IO。

2. vmstat命令可指定采样周期、采样次数。

3. 字段含义。
|procs|r|等待运行进程数|
|----|----|----|
|b|非中断睡眠状态进程数| |
|memory|swpd|虚拟内存，单位KB|
|free|空闲内存，单位KB| |
|buff|缓存内存，单位KB| |
|swap|si|磁盘交换到内存的交换页，单位KB/秒|
|so|内存交换到内存的交换页，单位KB/秒| |
|io|bi|发送到块设备，单位块/秒|
|bo|从块设备接收，单位块/秒| |
|system|in|每秒中断数，包括时钟中断|
|cs|每秒上下文切换次数| |
|cpu|us|用户CPU使用时间|
|sy|系统CPU使用时间| |
|id|空间CPU时间| |



### iostat命令

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225253006-2001515322.png)

1. iostat命令：Linux命令，监控IO。

2. 参数：

    a) “-d”：仅输出磁盘。

    b) “-x”：输出详情。

3. 字段含义。
|tps|每秒传输次数|
|----|----|
|kB_read/s|每秒从设备读取数据量|
|kB_wrtn/s|每秒向设备写入数据量|
|kB_read|读取总数据量|
|kB_wrtn|写入总数据量|



### pidstat命令

1. pidstat命令：Linux命令，监控进程、线程。

2. 参数：

    a) “-p”：进程ID。

    b) “-u”：监控CPU。

    c) “-r”：监控内存。

    d) “-d”：监控IO。

    e) “-t”：显示线程。

3. 举例：

    a) 监控进程CPU。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225440944-359987869.png)

    b) 监控线程CPU。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225530787-43479337.png)

    c) 监控内存。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225617209-154939029.png)
|**字段**|**含义**|
|----|----|
|minflt/s|进程每秒minor faults（无需从磁盘调出内存页）的总数|
|majflt/s|进程每秒major faults（需从磁盘调出内存页）的总数|
|VSZ|虚拟内存，单位KB|
|RSS|物理内存，单位KB|
|%MEM|内存占用率|

    d) 监控线程IO。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805225715522-1554407595.png)



### jps命令

1. jps命令：JDK工具，列出Java进程。

2. 参数：

    a) “-m”：显示main()函数的参数。

    b) “-l”：显示main()函数所在类的完整包名+类名。

    c) “-v”：显示传递给JVM的参数。



### jstat命令

1. jstat命令：JDK工具，查看虚拟机运行信息。

2. 个人看法：该命令参数较多，不如用其他可视化工具代替。



### jinfo命令

1. jinfo命令：JDK工具，查看正在运行虚拟机的参数及修改部分参数。

2. 语法：

```
jinfo <option> <pid>
```

    option：

    a) -flag <name>：打印指定参数。

    b) -flag [+|-]<name>：设置参数的布尔值。

    c) -flag <name>=<value>：设置参数。



### jmap命令

1. jmap命令：JDK工具，生成堆dump文件（即堆快照），及查看堆内对象统计信息。

2. 可使用jhat命令、Visual VM、MAT等工具分析dump文件。

3. 参数：

    a) “-head”：显示堆信息。

    b) “-histo[:live]”：显示对象统计信息，可指定仅统计存活（“:live”子参数）

    c) “-clstats”：ClassLoader统计信息。

    d) “-dump:<dump-options>”：导出堆，子参数有

        i. “live”：仅存活对象；

        ii. “format=b”：二进制格式；

        iii. “file=<file>”：导出文件路径。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230010225-788711742.png)



### jhat命令

1. jhat命令：JDK工具，分析堆dump文件。

2. 个人看法：使用Visual VM更方便。

3. 用法举例：

    a) 执行命令；

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230141772-2074432474.png)

    b) 访问http://localhost:7000。



### jstack命令

1. jstack命令：JDK工具，查看线程栈（也称线程dump）。

2. 参数：“-l”附加信息。



### jcmd命令

1. jstack命令：JDK 1.7以后的工具，多功能工具，可导出堆、查看进程、查看线程栈、执行GC等。

2. 待补充。



### Visual VM工具

1. Visual VM：JDK 1.6 Update 7以后，功能强大的多合一故障诊断和性能监控可视化工具。

2. 代替：jstat、jmap、jhat、jstack等命令，甚至JConsole。

3. 主要功能。

    a) 连接应用程序：本地或JMX远程。

    b) 监控概况：JVM参数、CPU、内存、堆、线程等。

    c) 线程dump及分析。

    d) 堆dump及分析，支持OQL。

    e) BTrace：不停机状态，注入字节码动态监控运行情况。



### Mission Control工具

1. Mission Control：JDK 1.7 Update 40以后，诊断工具。

2. 个人看法：虽然貌似比Visual VM好，但Visual VM够用且市场占有率更高。



## 解决OOM问题



### 堆溢出

1. 现象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230359037-1590095510.png)

2. 2种解决方法。

    a) 增大“-Xmx”参数。

    b) Visual VM工具分析大量占用堆的对象。



### 直接内存溢出

1. 现象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230457990-1622431552.png)

2. 2种解决方法。

    a) 显式GC“System.gc()”。

    b) 不设置“-XX:MaxDirectMemorySize”参数（此时等于“-Xmx”）或增大该值。



### 过多线程导致溢出

1. 现象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230552147-1452071133.png)

2. 2种解决方法。

    a) 减小“-Xmx”参数，操作系统可预留更多内存用于创建线程。

    b) 减小“-Xss”参数。



### 永久区/元数据区溢出

1. 现象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230638928-1758070304.png)

2. 3中解决方法。

    a) 增大“-XX:MaxPermSize”/“-XX:MaxMetaspaceSize”参数。

    b) 减少需要的类数量。

    c) 使用ClassLoader合理装载类，并定期回收。



### GC效率低导致OOM

1. 现象。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805230732209-437793754.png)

2. 原因：当下列条件都满足时，虚拟机认为GC效率低下，抛出OOM。

    a) 花在GC上的时间超过98%；

    b) 老年代释放内存小于2%；

    c) eden区释放内存小于2%；

    d) 最近连续5次GC都出现上述情况（同时出现，不是出现一个）。

3. 解决方法：该OOM属辅助作用，提示堆太小，添加“-XX:-UseGCOverheadLimit”参数禁用该功能。



## String实现细节



### String对象特点

1. 不变性（immutable）。

    a) 解释：String对象一旦生成，不能再修改。

    b) 好处：多线程共享，并频繁访问时，可省略同步和锁的时间，提高性能。

2. 针对常量池的优化。

    a) 解释：String对象值相同时，它们只引用常量池的同一个拷贝。

    b) 好处：节省内存。

3. 类的final定义。

    a) 解释：String类不可能有任何子类。

    b) 好处：保安全性保护。



### String常量池

1. 常量池：专门存放String常量的区域。

2. 位置：

    a) JDK 1.6之前，位于永久区；

    b) JDK 1.7以后，位于堆。



## 字节码优化

执行字节码从两处优化：javac编译时；通过JIT在运行时。



### 静态编译优化

1. 编译时计算。

    a) 解释：如果计算表达式的值能在编译时确定，则表达式计算提前到编译阶段。

    b) 举例1：

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231058209-1871869904.png)

    c) 举例2：

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231126147-1346187877.png)

2. 变量字符串的连接。

    a) 解释：变量字符串连接被转为StringBuilder操作，避免每次字符串操作产生新对象。

    b) 举例：自动优化前后的代码。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231157819-191217439.png)

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231212444-1207586888.png)

3. 基于常量的条件语句裁剪。

    a) 解释：任何逻辑在编译时就确定时，不需要的逻辑会被裁剪。

    b) 举例：实际多余的if...else...在编译器被裁剪。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231256428-2066521303.png)

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231306944-2041932979.png)

4. switch语句优化。

    a) 解释：switch语句可生成tableswitch或loopupswith字节码指令，前者效率高于后者，前者只能处理case连续的值，后者可处理不连续的值。

    b) 举例：虽然不连续，但由于离散空间不大，编译器插入值填充使用tableswitch。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231344834-564668157.png)

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231359319-534615041.png)



### JIT运行优化

1. JIT：Just-In-Time，即时编译。

2. 目的：避免函数被解释执行，而是将整个函数体编译成机器码，每次只执行编译编译后的机器码，可大幅度提升效率。

3. 虚拟机3种执行模式。

    a) 解释执行（Interpreted Mode）：不做JIT编译。

    b) 混合模式（Mixed Mode）：默认。热点代码被编译执行，其他解释执行。热点代码的判断依据是调用频率。

    c) 编译执行（Compiled Mode）：所有函数编译执行。

    d) 参数。

        i. “-Xint”：解释。

        ii. “-Xmixed”：混合。

        iii. “-XComp”：编译。

        iv. “-XX:CompileThreshold”：指定热点代码调用次数阀值。Client模式默认1500，Server模式默认10000。

        v. “-XX:+PrintCompilation”：打印即时编译日志。

4. 多级编译器。

    a) 客户端编译器（C1编译器）：Client模式使用的编译器，编译速度快。

    b) 服务端编译器（C2编译器）：Server模式使用的编译器，编译优化多，编译后代码质量高，时间长于C1。

    c) 多级目的：在编译速度和执行效率间取得平衡。

    d) 多级编译5级：

        i. 0级（解释执行）：解释执行，不采集性能监控数据；

        ii. 1级（简单C1编译）：采用C1，最简单的快速编译，根据需要采集性能数据；

        iii. 2级（有限的C1编译）：采用C1，更多的优化编译，可能根据1级采集的性能统计数据，进一步优化编译代码；

        iv. 3级（完全C1编译）：完全使用C1的所有功能，采集性能数据进行优化；

        v. 4级（C1编译）：完全使用C1的所有功能，完全优化。

    e) 参数：“-XX:+TieredCompilation”开启多级编译。

5. OSR栈上替换。

    a) 背景：函数要么解释，要么编译执行。从解释切换到机器码执行，如果尚未准备好编译版本，则解释执行，下次才机器码执行。绝大部分场合适用。

    b) 问题：不适合调用次数不多，但函数体内包含大量循环的函数。例如：

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231753865-801651477.png)

    c) OSR：On Stack Replacement栈上替换。不等待函数体运行结束，在循环体就将代码替换为编译版本的技术。

6. 方法内联。

    a) 目的：减少方法调用的次数，提高性能。JIT编译器默认开启。

    b) 参数：

        i. “-XX:+Inline”：开启方法内联优化。

        ii. “-XX:FreqInlineSize”：内联体积上限，方法体积大于该值则不内联。

    c) 举例：优化前后。

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231854037-1699450994.png)

![](http://images2017.cnblogs.com/blog/1200732/201708/1200732-20170805231913694-873311609.png)

7. 代码缓存。

    a) 解释：Code Cache。保存字节码被编译后的机器码，缓存用完后JIT编译停止，后续未编译的字节码解释执行。代码缓存的清理也由GC完成。

    b) 参数：“-XX:ReservedCodeCacheSize”指定缓存大小，默认32MB。



作者：netoxi

出处：http://www.cnblogs.com/netoxi

本文版权归作者和博客园共有，欢迎转载，未经同意须保留此段声明，且在文章页面明显位置给出原文连接。欢迎指正与交流。





