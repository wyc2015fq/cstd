# JVM 触发Full gc条件 - z69183787的专栏 - CSDN博客
2016年10月08日 16:27:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4561
本文参考:[http://blog.csdn.net/chenleixing/article/details/46706039](http://blog.csdn.net/chenleixing/article/details/46706039)
给出各个场景下可能触发full gc的实例代码.
除直接调用System.gc外，触发Full
 GC执行的情况有如下四种。
1.旧生代空间不足
旧生代空间只有在新生代对象转入及创建为大对象、大数组时才会出现不足的现象，当执行Full
 GC后空间仍然不足，则抛出如下错误：
java.lang.OutOfMemoryError: Java heap space 
为避免以上两种状况引起的Full GC，调优时应尽量做到让对象在Minor
 GC阶段被回收、让对象在新生代多存活一段时间及不要创建过大的对象及数组。
2. Permanet Generation空间满
Permanet Generation中存放的为一些class的信息等，当系统中要加载的类、反射的类和调用的方法较多时，Permanet
 Generation可能会被占满，在未配置为采用CMS GC的情况下会执行Full
 GC。如果经过Full GC仍然回收不了，那么JVM会抛出如下错误信息：
java.lang.OutOfMemoryError: PermGen space 
为避免Perm Gen占满造成Full
 GC现象，可采用的方法为增大Perm Gen空间或转为使用CMS
 GC。
3. CMS GC时出现promotion
 failed和concurrent
 mode failure
对于采用CMS进行旧生代GC的程序而言，尤其要注意GC日志中是否有promotion
 failed和concurrent mode failure两种状况，当这两种状况出现时可能会触发Full
 GC。
promotion failed是在进行Minor
 GC时，survivor space放不下、对象只能放入旧生代，而此时旧生代也放不下造成的；concurrent
 mode failure是在执行CMS GC的过程中同时有对象要放入旧生代，而此时旧生代空间不足造成的。
应对措施为：增大survivor space、旧生代空间或调低触发并发GC的比率，但在JDK
 5.0+、6.0+的版本中有可能会由于JDK的bug29导致CMS在remark完毕后很久才触发sweeping动作。对于这种状况，可通过设置-XX:
 CMSMaxAbortablePrecleanTime=5（单位为ms）来避免。
4.统计得到的Minor
 GC晋升到旧生代的平均大小大于旧生代的剩余空间
这是一个较为复杂的触发情况，Hotspot为了避免由于新生代对象晋升到旧生代导致旧生代空间不足的现象，在进行Minor
 GC时，做了一个判断，如果之前统计所得到的Minor GC晋升到旧生代的平均大小大于旧生代的剩余空间，那么就直接触发Full
 GC。
例如程序第一次触发Minor GC后，有6MB的对象晋升到旧生代，那么当下一次Minor
 GC发生时，首先检查旧生代的剩余空间是否大于6MB，如果小于6MB，则执行Full
 GC。
当新生代采用PS GC时，方式稍有不同，PS
 GC是在Minor GC后也会检查，例如上面的例子中第一次Minor
 GC后，PS GC会检查此时旧生代的剩余空间是否大于6MB，如小于，则触发对旧生代的回收。
除了以上4种状况外，对于使用RMI来进行RPC或管理的Sun
 JDK应用而言，默认情况下会一小时执行一次Full GC。可通过在启动时通过-
 java -Dsun.rmi.dgc.client.gcInterval=3600000来设置Full GC执行的间隔时间或通过-XX:+
 DisableExplicitGC来禁止RMI调用System.gc。
原文地址：[http://book.51cto.com/art/201011/235592.htm](http://book.51cto.com/art/201011/235592.htm)
*对象分配规则
1.对象优先分配在Eden区，如果Eden区没有足够的空间时，虚拟机执行一次Minor GC。
2.大对象直接进入老年代（大对象是指需要大量连续内存空间的对象）。这样做的目的是避免在Eden区和两个Survivor区之间发生大量的内存拷贝（新生代采用复制算法收集内存）。
3.长期存活的对象进入老年代。虚拟机为每个对象定义了一个年龄计数器，如果对象经过了1次Minor GC那么对象会进入Survivor区，之后每经过一次Minor GC那么对象的年龄加1，知道达到阀值对象进入老年区。
4.动态判断对象的年龄。如果Survivor区中相同年龄的所有对象大小的总和大于Survivor空间的一半，年龄大于或等于该年龄的对象可以直接进入老年代。
5.空间分配担保。每次进行Minor GC时，JVM会计算Survivor区移至老年区的对象的平均大小，如果这个值大于老年区的剩余值大小则进行一次Full GC，如果小于检查HandlePromotionFailure设置，如果true则只进行Monitor GC,如果false则进行Full GC。 
## １．调用System.gc
```java
import java.util.ArrayList;
import java.util.List;
/**
 * 
 * created by: gaoxingliang@outlook.com
 * created:2016年3月20日
 */
/**
 * -XX:+UseSerialGC -Xms200M -Xmx200M -Xmn32m -XX:SurvivorRatio=8 -XX:+PrintGCDetails
 * @author gxl
 *
 */
public class SimulateFullGc
{
    public static void main(String[] args)
    {
        //模拟fullgc场景
        //场景1 使用System.gc
        List<Object> l = new ArrayList<Object>();
        for (int i =0; i< 100;i++)
        {
            l.add(new byte[1024*1024 ]);
            if (i % 10 ==0)
            {
                System.gc();
            }
        }
    }
}
```
[Full GC (System) [Tenured:
 0K->1495K(172032K), 0.0048354 secs] 2073K->1495K(201536K), [Perm : 2529K->2529K(21248K)], 0.0048900 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[Full GC (System) [Tenured: 1495K->11735K(172032K), 0.0064495 secs] 13310K->11735K(201536K), [Perm : 2532K->2532K(21248K)], 0.0064752 secs] [Times: user=0.00 sys=0.00, real=0.01
 secs]
由System.gc调用的ｇｃ标记Full GC (System).
## 2.老年代空间不足
```java
/**
 * 
 * created by: gaoxingliang@outlook.com
 * created:2016年3月20日
 */
/**
 * -XX:+UseSerialGC -Xms200M -Xmx200M -Xmn32m -XX:SurvivorRatio=8 -XX:+PrintGCDetails
 * @author gxl
 *
 */
public class SimulateFullGc
{
    public static void main(String[] args)
    {
        //模拟fullgc场景
        //老年代空间不足
        //按照上面的参数推算:老年代大小: 200 -32m = 168M
        byte [] MAXOBJ = new byte [1024 * 1024 * 100]; // 100M
        byte [] MAXOBJ2 = new byte [1024 * 1024 * 70]; // 60M
        MAXOBJ = null;
        byte [] MAXOBJ3 = new byte [1024 * 1024 * 100]; // 60M
    }
}
```
[GC [DefNew: 5145K->470K(29504K), 0.0029970 secs][Tenured: 106496K->106966K(172032K), 0.0027630 secs] 107545K->106966K(201536K), [Perm : 2528K->2528K(21248K)], 0.0057990 secs]
 [Times: user=0.00 sys=0.02, real=0.01 secs] 
[Full GC [Tenured: 106966K->106952K(172032K), 0.0024331 secs] 106966K->106952K(201536K), [Perm : 2528K->2527K(21248K)], 0.0024488 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
Exception in thread “main” java.lang.OutOfMemoryError: Java heap space 
at SimulateFullGc.main(SimulateFullGc.java:27)
## 3.永久代空间不足
方式 在jdk6 上:
```java
import java.util.ArrayList;
import java.util.List;
/**
 * 
 * created by: gaoxingliang@outlook.com
 * created:2016年3月20日
 */
/**
 * -XX:+UseSerialGC -Xms200M -Xmx200M -Xmn32m -XX:SurvivorRatio=8 -XX:+PrintGCDetails -XX:MaxPermSize=10M
 * @author gxl
 *
 */
public class SimulateFullGc
{
    public static void main(String[] args)
    {
        //模拟fullgc场景
        //持久代空间不足
        List<String> list = new ArrayList<String>();
        int i = 0;
        while (true)
        {
            list.add(String.valueOf("ABCD:"  + i ++).intern());
        }
    }
```
[GC [DefNew: 26240K->937K(29504K), 0.0040883 secs] 26240K->937K(201536K), 0.0041121 secs] [Times: user=0.02 sys=0.00, real=0.01 secs] 
[Full GC [Tenured: 0K->1325K(172032K), 0.0362063 secs] 17898K->1325K(201536K), [Perm : 20479K->20479K(20480K)], 0.0362549 secs] [Times: user=0.03 sys=0.00, real=0.04 secs] 
[Full GC [Tenured: 1325K->1325K(172032K), 0.0326822 secs] 1325K->1325K(201536K), [Perm : 20479K->20479K(20480K)], 0.0327085 secs] [Times: user=0.03 sys=0.00, real=0.03 secs] 
[Full GC [Tenured: 1325K->1325K(172032K), 0.0128924 secs] 1821K->1325K(201536K), [Perm : 20479K->3734K(20480K)], 0.0129210 secs] [Times: user=0.02 sys=0.00, real=0.01 secs] 
Exception in thread “main” java.lang.OutOfMemoryError: PermGen space 
at java.lang.String.intern(Native Method) 
at SimulateFullGc.main(SimulateFullGc.java:25)
String.intern 会拷贝实例到永久代中.
在jdk1.7 不会,所以可以加载class来模拟:
```java
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;
/**
 * 
 * created by: gaoxingliang@outlook.com
 * created:2016年3月20日
 */
/**
 * -XX:+UseSerialGC -Xms200M -Xmx200M -Xmn32m -XX:SurvivorRatio=8 -XX:+PrintGCDetails -XX:MaxPermSize=10M
 * @author gxl
 *
 */
public class SimulateFullGc
{
    public static void main(String[] args)
    {
        //模拟fullgc场景
        //持久代空间不足
        //class 加载信息
        //需要cglib + asm (http://forge.ow2.org/projects/asm/)
        while (true)
        {
            Enhancer en = new Enhancer();
            en.setSuperclass(OOMObject.class);
            en.setUseCache(false);
            en.setCallback(new MethodInterceptor()
            {
                @Override
                public Object intercept(Object arg0, Method arg1, Object[] arg2,
                        MethodProxy arg3) throws Throwable
                {
                    // TODO Auto-generated method stub
                    return null;
                }
            });
            en.create();
        }
    }
    static class OOMObject
    {
    }
}
```
[GC [DefNew: 28043K->1735K(29504K), 0.0039443 secs] 33739K->7817K(201536K), 0.0039660 secs] [Times: user=0.03 sys=0.00, real=0.00 secs] 
[Full GC [Tenured: 6082K->7989K(172032K), 0.0322856 secs] 23097K->7989K(201536K), [Perm : 20479K->20479K(20480K)], 0.0323121 secs] [Times: user=0.05 sys=0.00, real=0.03 secs] 
[Full GC [Tenured: 7989K->7989K(172032K), 0.0233015 secs] 7989K->7989K(201536K), [Perm : 20479K->20479K(20480K)], 0.0233266 secs] [Times: user=0.01 sys=0.00, real=0.02 secs] 
[Full GC [Tenured: 7989K->7989K(172032K), 0.0199921 secs] 8515K->7989K(201536K), [Perm : 20479K->20479K(20480K)], 0.0200187 secs] [Times: user=0.03 sys=0.00, real=0.02 secs] 
[Full GC [Tenured: 7989K->3354K(172032K), 0.0250219 secs] 7989K->3354K(201536K), [Perm : 20479K->20477K(20480K)], 0.0250530 secs] [Times: user=0.02 sys=0.00, real=0.03 secs] 
Exception in thread “main” [Full GC [Tenured: 3354K->3355K(172032K), 0.0198650 secs] 3880K->3355K(201536K), [Perm : 20479K->20479K(20480K)], 0.0198919 secs] [Times: user=0.02 sys=0.00, real=0.02 secs]
[Full GC [Tenured: 3355K->3355K(172032K), 0.0198493 secs] 3355K->3355K(201536K), [Perm : 20479K->20479K(20480K)], 0.0198762 secs] [Times: user=0.03 sys=0.00, real=0.02 secs] 
[Full GC [Tenured: 3355K->3355K(172032K), 0.0197512 secs] 3880K->3355K(201536K), [Perm : 20479K->20479K(20480K)], 0.0197814 secs] [Times: user=0.02 sys=0.00, real=0.02 secs] 
[Full GC [Tenured: 3355K->3285K(172032K), 0.0245018 secs] 3355K->3285K(201536K), [Perm : 20479K->20478K(20480K)], 0.0245283 secs] [Times: user=0.03 sys=0.00, real=0.02 secs]
cglib & asm jar download:[http://download.csdn.net/detail/u010926243/6721023](http://download.csdn.net/detail/u010926243/6721023)
## ４． gc 担保失败
空间分配担保失败. 
在发生MinorGC前,检查老年代是否有连续空间,如果有,则执行,如果没有,根据设置:-XX:-HandlePromotionFailure 指定,如果打开,那么继续检查,当前老年代最大可用连续空间大于平均历次晋升到老年代大小,如果大于,则进行MinorGC,否则进行FullGC,如果HandlePromotionFailure 不设置 直接进行FullGC. 
大致就是这样: 
![flow](https://img-blog.csdn.net/20160320145612255)
代码: 
该参数:-XX:-HandlePromotionFailure 在JDK 6U24中移除.后续判断只要剩余连续大于当前新生代或者历次晋升平均大小就会执行minorgc.
> 
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option HandlePromotionFailure; support was removed in 6.0_24
`搞了好久终于搞出来了:`
```java
import java.lang.reflect.Field;
import sun.misc.Unsafe;
/**
 * 
 * created by: gaoxingliang@outlook.com
 * created:2016年3月20日
 */
/**
 * -Xms20M -Xmx20M -Xmn10m -XX:SurvivorRatio=8 -XX:+PrintGCDetails -XX:-HandlePromotionFailure -XX:MaxTenuringThreshold=1
 * @author gxl
 *
 */
public class SimulateFullGc
{
    private static final int MB = 1024 * 1024;
    public static void main(String[] args) throws Exception
    {
        // 模拟fullgc场景
        // 提升担保
        // 提升担保
        byte[] M6, M3, M4, M5, M7, M8;
        M6 = new byte[6 * MB];
        M6[0] = 0;
        M6[0] = 0;
        // 使用2次保证下次需要的时候可以晋升到老年代 会晋升那么 晋升经验值为6M
        M3 = new byte[4 * MB];
        M4 = new byte[2 * MB];
        M4 = null;
        M5 = new byte[2 * MB];
        M5[0] = 0;
        M5[0] = 0;
        M7 = new byte[2 * MB];
        M7[0] = 0;
        M7[0] = 0;
        M7 = null;
        M8 = new byte[3 * MB];
        // 最终如下对象 老年代 M6 + M8 = 9M
        // 年轻代:M3 + M5 = 6M = 6144K
        System.out.println("M6 HEX:0x" + Long.toHexString(addressOf(M6)));
        System.out.println("M5 HEX:0x" + Long.toHexString(addressOf(M5)));
        System.out.println("M3 HEX:0x" + Long.toHexString(addressOf(M3)));
        System.out.println("M8 HEX:0x" + Long.toHexString(addressOf(M8)));
    }
    private static Unsafe unsafe;
    static
    {
        try
        {
            Field field = Unsafe.class.getDeclaredField("theUnsafe");
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    public static long addressOf(Object o) throws Exception
    {
        Object[] array = new Object[] { o };
        long baseOffset = unsafe.arrayBaseOffset(Object[].class);
        int addressSize = unsafe.addressSize();
        long objectAddress;
        switch (addressSize)
        {
        case 4:
            objectAddress = unsafe.getInt(array, baseOffset);
            break;
        case 8:
            objectAddress = unsafe.getLong(array, baseOffset);
            break;
        default:
            throw new Error("unsupported address size: " + addressSize);
        }
        return (objectAddress);
    }
}
```
[GC [DefNew: 6487K->149K(9216K), 0.0027691 secs] 6487K->6293K(19456K), 0.0027839 secs] [Times: user=0.00 sys=0.00, real=0.00 secs] 
[GC [DefNew: 6379K->6379K(9216K), 0.0000060 secs][Tenured: 6144K->6144K(10240K), 0.0048112 secs] 12523K->10389K(19456K), [Perm : 374K->374K(12288K)], 0.0048426 secs] [Times: user=0.00
 sys=0.00, real=0.01 secs] 
[Full GC [Tenured: 8192K->6144K(10240K), 0.0032886 secs] 14485K->12437K(19456K), [Perm : 374K->374K(12288K)], 0.0033058 secs] [Times: user=0.02 sys=0.00, real=0.00 secs] 
M6 HEX:0x33990000 
M5 HEX:0x333b5520 
M3 HEX:0x32f90000 
M8 HEX:0x33f90010 
Heap 
def new generation total 9216K, used 6514K [0x32f90000, 0x33990000, 0x33990000) 
eden space 8192K, 79% used [0x32f90000, 0x335ec808, 0x33790000) 
from space 1024K, 0% used [0x33890000, 0x33890000, 0x33990000) 
to space 1024K, 0% used [0x33790000, 0x33790000, 0x33890000) 
tenured generation total 10240K, used 9216K [0x33990000, 0x34390000, 0x34390000) 
the space 10240K, 90% used [0x33990000, 0x34290020, 0x34290200, 0x34390000) 
compacting perm gen total 12288K, used 374K [0x34390000, 0x34f90000, 0x38390000) 
the space 12288K, 3% used [0x34390000, 0x343ed960, 0x343eda00, 0x34f90000) 
ro space 10240K, 54% used [0x38390000, 0x3890c510, 0x3890c600, 0x38d90000) 
rw space 12288K, 55% used [0x38d90000, 0x3942fb78, 0x3942fc00, 0x39990000)
注意对象位置.
## 5. Cocurrent mode failure
```
发生在cms的清理sweep阶段,发现有新的垃圾产生,而且老年代没有足够空间导致的.
关于cms:
初始标记(STW) - >并发标记　－＞重新标记（STW）　－＞并发清除．
STW = stop the world.
```
Ref: 
[get the object memory address](http://stackoverflow.com/questions/1961146/memory-address-of-variables-in-java)
<深入理解java虚拟机>
