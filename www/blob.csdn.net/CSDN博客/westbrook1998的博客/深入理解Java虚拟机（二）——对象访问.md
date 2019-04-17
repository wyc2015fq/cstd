# 深入理解Java虚拟机（二）——对象访问 - westbrook1998的博客 - CSDN博客





2018年03月16日 20:05:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：84








创建一个对象 
`Object obj = new Object();`

在这句代码中`Object obj`是一个对象的引用，作为一个reference类型存放在**本地变量表（Java虚拟机栈）**中，而`new Object()`则反映在**Java堆**中，形成一块存储了Object类型所有实例数据值的结构化内存，同时**Java堆还包含能够查找到此对象类型数据（如对象类型，父类，实现的接口，方法）的地址**，而**方法区就存储了这些对象类型数据**

这个对象引用可以通过两种方式去定位，去访问对象： 

1. 使用句柄：    **Java堆中划出一块内存作为句柄池**，所以**reference类型保存的就是句柄地址**，而**句柄值保存对象实例数据（Java堆实例池）和类型数据（方法区）的地址**

2. 使用指针（HotSpot使用该种方式）：    **reference类型直接保存对象地址**，**指向Java堆中的对象**，再有**该对象中的指针指向方法区的对象类型数据**
**使用句柄访问方式的优点是对象移动时reference类型不必修改，而指针访问方式的优点是节省了一次指针定位的开销**

### 实战练习

#### Java堆溢出：

（不断创建对象，**保证GC Roots到对象之间有可达路径避免垃圾回收机制清除这些对象？？？**，就会在对象数量到达最大堆容量限制后产生内存溢出异常 OnOutOfMemoryError） 

首先设置虚拟机的堆大小限制（Idea中设置VM options），参数为 

-verbose:gc 

-Xms20M   最小值 

-Xmx20M   最大值 两个值都是20m避免Java堆自动拓展 

-Xmn10M 

-XX:+PrintGCDetails 

-XX:SurvivorRatio=8
测试程序：

```java
package com.zxc.memoryError;

import java.util.ArrayList;
import java.util.List;

/**
 * VM Args:-Xms20m -Xmx20m -XX:+HeapDumpOnOutOfMemoryError
 * @author russell 
 */
public class HeapOOM {
    static class OOMObject{
        public static void main(String[] args){
            List<OOMObject> list=new ArrayList<>();

            while(true){
                list.add(new OOMObject());
            }
        }
    }
}
```

控制台抛出异常

```
/opt/jdk1.8.0_152/bin/java -verbose:gc -Xms20M -Xmx20M -Xmn10M -XX:+PrintGCDetails -XX:SurvivorRatio=8 -javaagent:/opt/idea/lib/idea_rt.jar=36611:/opt/idea/bin -Dfile.encoding=UTF-8 -classpath /opt/jdk1.8.0_152/jre/lib/charsets.jar:/opt/jdk1.8.0_152/jre/lib/deploy.jar:/opt/jdk1.8.0_152/jre/lib/ext/cldrdata.jar:/opt/jdk1.8.0_152/jre/lib/ext/dnsns.jar:/opt/jdk1.8.0_152/jre/lib/ext/jaccess.jar:/opt/jdk1.8.0_152/jre/lib/ext/jfxrt.jar:/opt/jdk1.8.0_152/jre/lib/ext/localedata.jar:/opt/jdk1.8.0_152/jre/lib/ext/nashorn.jar:/opt/jdk1.8.0_152/jre/lib/ext/sunec.jar:/opt/jdk1.8.0_152/jre/lib/ext/sunjce_provider.jar:/opt/jdk1.8.0_152/jre/lib/ext/sunpkcs11.jar:/opt/jdk1.8.0_152/jre/lib/ext/zipfs.jar:/opt/jdk1.8.0_152/jre/lib/javaws.jar:/opt/jdk1.8.0_152/jre/lib/jce.jar:/opt/jdk1.8.0_152/jre/lib/jfr.jar:/opt/jdk1.8.0_152/jre/lib/jfxswt.jar:/opt/jdk1.8.0_152/jre/lib/jsse.jar:/opt/jdk1.8.0_152/jre/lib/management-agent.jar:/opt/jdk1.8.0_152/jre/lib/plugin.jar:/opt/jdk1.8.0_152/jre/lib/resources.jar:/opt/jdk1.8.0_152/jre/lib/rt.jar:/home/russell/IdeaProjects/java_test/out/production/java_test:/home/russell/IdeaProjects/java_test/lib/mysql-connector-java-5.1.45-bin.jar com.zxc.memoryError.memoryError$OOMObject
[GC (Allocation Failure) [PSYoungGen: 8192K->1017K(9216K)] 8192K->4921K(19456K), 0.0327340 secs] [Times: user=0.02 sys=0.01, real=0.04 secs] 
[GC (Allocation Failure) [PSYoungGen: 9209K->1024K(9216K)] 13113K->10742K(19456K), 0.0335066 secs] [Times: user=0.04 sys=0.00, real=0.04 secs] 
[Full GC (Ergonomics) [PSYoungGen: 1024K->511K(9216K)] [ParOldGen: 9718K->10175K(10240K)] 10742K->10687K(19456K), [Metaspace: 3268K->3268K(1056768K)], 0.2420921 secs] [Times: user=0.34 sys=0.00, real=0.24 secs] 
[Full GC (Ergonomics) [PSYoungGen: 8358K->8232K(9216K)] [ParOldGen: 10175K->8058K(10240K)] 18533K->16291K(19456K), [Metaspace: 3268K->3268K(1056768K)], 0.1626348 secs] [Times: user=0.37 sys=0.00, real=0.16 secs] 
[Full GC (Allocation Failure) Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
    at java.util.Arrays.copyOf(Arrays.java:3210)
    at java.util.Arrays.copyOf(Arrays.java:3181)
    at java.util.ArrayList.grow(ArrayList.java:265)
    at java.util.ArrayList.ensureExplicitCapacity(ArrayList.java:239)
    at java.util.ArrayList.ensureCapacityInternal(ArrayList.java:231)
    at java.util.ArrayList.add(ArrayList.java:462)
    at com.zxc.memoryError.memoryError$OOMObject.main(memoryError.java:12)
[PSYoungGen: 8232K->8232K(9216K)] [ParOldGen: 8058K->8040K(10240K)] 16291K->16273K(19456K), [Metaspace: 3268K->3268K(1056768K)], 0.1633865 secs] [Times: user=0.34 sys=0.00, real=0.17 secs] 
Heap
 PSYoungGen      total 9216K, used 8640K [0x00000000ff600000, 0x0000000100000000, 0x0000000100000000)
  eden space 8192K, 99% used [0x00000000ff600000,0x00000000ffdf0180,0x00000000ffe00000)
  from space 1024K, 49% used [0x00000000fff00000,0x00000000fff7ff00,0x0000000100000000)
  to   space 1024K, 0% used [0x00000000ffe00000,0x00000000ffe00000,0x00000000fff00000)
 ParOldGen       total 10240K, used 8040K [0x00000000fec00000, 0x00000000ff600000, 0x00000000ff600000)
  object space 10240K, 78% used [0x00000000fec00000,0x00000000ff3da258,0x00000000ff600000)
 Metaspace       used 3300K, capacity 4496K, committed 4864K, reserved 1056768K
  class space    used 361K, capacity 388K, committed 512K, reserved 1048576K

Process finished with exit code 1
```

具体处理Java堆内存问题会在后面几章提到

#### 虚拟机栈和本地方法栈溢出

JVM规范中规定了两种异常： 

1. StackOverflowError： 线程请求的栈深度大于虚拟机所允许的最大深度 

2. OutOfMemoryErro：虚拟机在拓展栈时无法申请到足够的内存空间 

方法的无限递归导致虚拟机栈内存溢出 

参数：-Xss228k
```java
package com.zxc.memoryError;

/**
 * VM Args:-Xss228k
 */
public class JavaVMStackSOF {

    private int stackLength=1;

    //无限递归使虚拟机栈内存溢出
    public void stackLeak(){
        stackLength++;
        stackLeak();
    }

    public static void main(String[] args) {
        JavaVMStackSOF oom=new JavaVMStackSOF();
        try{
            oom.stackLeak();
        }catch (Throwable e){
            System.out.println("stack length:"+oom.stackLength);
            throw e;
        }
    }
}
```

控制台异常：

```
stack length:1517
Exception in thread "main" java.lang.StackOverflowError
    at com.zxc.memoryError.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:12)
    ......
    at com.zxc.memoryError.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:13)
```

**该程序表明，在*`单个线程`*下，无论是由于栈帧太大，还是由于虚拟机栈容量太小，当内存无法分配时，都是抛出StackOverflowError异常**

```java
package com.zxc.memoryError;

/**
 * VM Args: -Xss2M
 * @author russell 
 */
public class JavaVMStackOOM {

    private void dontStop(){
        while(true){

        }
    }

    public void stackLeakByThread(){
        while(true){
            Thread thread=new Thread(new Runnable() {
                @Override
                public void run() {
                    dontStop();
                }
            });
            thread.start();
        }
    }

    public static void main(String[] args) {
        JavaVMStackOOM oom=new JavaVMStackOOM();
        oom.stackLeakByThread();
    }
}
```

**该程序表明，通过`不断建立线程`可以产生内存溢出异常**

> 
原因： 操作系统分配给每个进程的内存是有限制的，所以，操作系统限制的内存减去最大堆容量减去最大方法区容量，剩下的就是虚拟机栈和本地方法栈，因此如果每个线程分配到的栈容量越大，可以建立的线程数量就越小，就越容易产生内存溢出


#### 运行时常量池溢出

```java
package com.zxc.memoryError;

import java.util.ArrayList;
import java.util.List;

/**
 * VM Args: -XX:PermSize=10M -XX:MaxPermSize=10M
 */
public class RuntimeConstantPoolOOM {

    public static void main(String[] args) {
        List<String> list=new ArrayList<>();
        int i=0;
        while(true){
            list.add(String.valueOf(i++).intern());
        }
    }
}
```

按照书上所说，控制台应该出现

```
Exception in thread "main" java.lang.OutOfMemoryError: PermGen space
    at java.lang.String.intern(Native Method)
    at jvm.RuntimeConstantPoolOOM.main(RuntimeConstantPoolOOM.java:16)
```

但实际上，却出现了

```
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option PermSize=10M; support was removed in 8.0
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=10M; support was removed in 8.0
```

而且程序一直没有结束 

这是因为在JDK7之后这两个参数已经不适用了，在JDK1.6及之前版本中，由于常量池分配在永久代中(即方法区)，我们可以通过-XX:PermSize和-XX:MaxPermSize限制方法区大小，从而间接限制其中常量池的容量，而JDK1.7已经逐步开始“去永久代”。 
**此时VM参数正确应为：-Xmx2048m -XX:MetaspaceSize=512m -XX:MaxMetaspaceSize=768m -Xss2m**
> 
在jdk1.8之前之前我们将储存类信息、常量、静态变量的方法区称为持久代(Permanent Generation)，PermSize和MaxPermSize是设置持久代大小的参数，在jdk1.8中持久代被完全移除了，所以这两个参数也被移除了，多了一个元数据区(Metadata Space)，所以设置元数据区大小的参数也变成对应的MetaspaceSize和MaxMetaspaceSize了。


使用`intern`方法（如果常量池中已经包含一个等于此String对象的字符串，则返回池中该字符串引用，否则将此String对象添加到常量池中，再返回该引用） 

通过不断添加字符串到常量池以及限制方法区大小（间接限制常量池大小）使得出现常量池溢出异常

#### 方法区溢出

#### 本机直接内存溢出







