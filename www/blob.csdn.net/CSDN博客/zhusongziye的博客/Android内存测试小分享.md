# Android内存测试小分享 - zhusongziye的博客 - CSDN博客





2018年04月15日 14:42:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：653








**背景**


手机的使用内存一直是用户非常关注的问题，所以一款产品的性能好坏，内存往往都是特别重要的部分。一款app在开发过程或者开发完成后都会有一些内存的问题，这个就需要我们测试多去关注，多去发现这些问题。

**关于内存**




说一下主要关注的两点：Stack 和Heap：

Stack空间由操作系统控制，其主要用来存储函数地址、函数参数、局部变量等，这些函数变量等占用的空间不是很大，所以stack空间不需要很大。

Heap空间由程序控制，开发人员可以用new、free、delete等函数调用这块内存空间。Heap为完成各种复杂任务提供内存空间，所以Heap的空间比较大，大概几百M到几个G。所以开发人员在使用这块空间时如果处理不当，即容易出现内存问题



**内存测试中容易出现的问题**



1）对比竞品各页面及后台内存占用过高

我们做的很多产品市面上都会有类似的竞品存在，我们想要留住用户，那我们就必须在各方面更优秀，内存占用是很重要的一点。

2）内存泄漏

程序在向系统申请分配内存空间后(new)，在使用完毕后未释放。结果导致一直占据该内存单元，我们和程序都无法再使用该内存单元，直到程序结束，这是内存泄露。
3）内存溢出：程序向系统申请的内存空间超出了系统能给的。比如内存只能分配一个int类型，我却要塞给他一个long类型，系统就出现oom。




**测试方法**



**01**

**通过命令获取**

**连接上手机后通过“dumpsys meminfo 包名/pid”命令获取（实例)**



**![](https://img-blog.csdn.net/20180415144036392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**







**Pss Total: 实际使用的物理内存private dirty: 私有驻留内存Heap Size: 占用总内存（Heap  堆）（扩展：进程内存空间是虚拟 内存，区分于物理内存，进程无法直接操作物理内存RAM。必要时，操作系统对其进行映射，使进程能应用到物理内存）Heap Alloc: 分配内存**

**Heap Free: 空闲内存**

**查询某一个进程所占用的总内存通过：dumpsys meminfo|find**

**“Packagename”（适用于windows）我在对比测试实践中即用上面的第2种方法获取内存值，附一个直接获取并计算内存结果的小脚本**


**![](https://img-blog.csdn.net/20180415144119748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**
**02**

**LeakCanary的使用**

**LeakCanary是一个高效检测内存泄露的工具，需要做的配置如下，在 build.gradle中加入引用，不同的编译使用不同的引用。**

**dependencies{debugCompile'com.squareup.leakcanary:leakcanary-android:1.3'releaseCompile'com.squareup.leakcanary:leakcanary-android-no-op:1.3' }**

**在 Application 中：**

**public class ExampleApplication extends Application{@Override public void onCreate() { super.onCreate(); LeakCanary.install(this); } }**

**这样，就万事俱备了！**

**若有内存泄漏，会有toast提示正在dump数据，打开leak app就可以看到一条具体的内测泄漏信息（这里在toast提示之后会有一定的延迟，过一会儿才会在leak app显示）**

**内存泄漏详细页如下图所示：**


**![](https://img-blog.csdn.net/20180415144137139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**


**点击加号就可以查看具体类所在的包名称。整个详情就是一个引用链：MainActiviy的内部类LeakThread引用了LeakThread的this$0，this$0的含义就是内部类自动保留的一个指向所在外部类的引用，而这个外部类就是详情最后一行所给出的MainActiviy的实例，这将会导致MainActivity无法被GC，从而产生内存泄漏。**



