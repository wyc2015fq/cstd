
# Java弱引用最精彩的解释 - 无界 - CSDN博客

置顶2019年03月26日 11:54:35[21aspnet](https://me.csdn.net/21aspnet)阅读数：1024


原文出自：[https://stackoverflow.com/questions/299659/whats-the-difference-between-softreference-and-weakreference-in-java](https://stackoverflow.com/questions/299659/whats-the-difference-between-softreference-and-weakreference-in-java)
这个解释是关于弱引用最精彩的
![](https://img-blog.csdnimg.cn/2019032611441697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326131402159.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326113943555.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326114138312.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
```python
public class EmployeeVal {
  public EmployeeVal(String userName) {
    this.userName = userName;
  }

  public String getUserName() {
    return userName;
  }
  public void setUserName(String userName) {
    this.userName = userName;
  }

  private String userName;
}
```
```python
public static void main(String args[]) {
        HashMap<Employee, EmployeeVal> aMap = new HashMap<Employee, EmployeeVal>();
        Employee emp = new Employee("Vinoth");
        EmployeeVal val = new EmployeeVal("Programmer");
        aMap.put(emp, val);
        emp = null;
        System.gc();
        System.out.println("Size of Map: " + aMap.size());
    }
```
输出：Size of Map: 1
```python
public static void main(String args[]) {
    WeakHashMap<Employee, EmployeeVal> aMap = new WeakHashMap<Employee, EmployeeVal>();
    Employee emp = new Employee("Vinoth");
    EmployeeVal val = new EmployeeVal("Programmer");
    aMap.put(emp, val);
    emp = null;
    System.gc();
    int count = 0;
    while (0 != aMap.size()) {
      ++count;
      System.gc();
    }
    System.out.println("Took " + count+ " calls to System.gc() to result in weakHashMap size of : "+ aMap.size());
  }
```
随机每次不确定的一个count值：
Took 3 calls to System.gc() to result in weakHashMap size of : 0
Took 22 calls to System.gc() to result in weakHashMap size of : 0
Took 0 calls to System.gc() to result in weakHashMap size of : 0
----------------
# [Understanding Weak References Blog](https://community.oracle.com/people/enicholas/blog/2006/05/04/understanding-weak-references?customTheme=otn)
强引用
强引用是一种普通的Java引用，是您每天使用的类型。例如，代码：
*StringBuffer buffer = new StringBuffer（）;*
创建一个新的StringBuffer（）并在变量缓冲区中存储对它的强引用。强引用的重要部分 - 使它们变得“强大”的部分 - 是它们与垃圾收集器交互的方式。具体来说，如果一个对象可以通过一系列强引用（强烈可访问）访问，则它不符合垃圾回收的条件。由于您不希望垃圾收集器销毁您正在处理的对象，因此这通常正是您想要的。
当强引用过于强烈时
应用程序使用无法合理扩展的类并不罕见。该类可能只是标记为最终的，或者它可能更复杂，例如由未知（甚至可能是不可知）的具体实现支持的工厂方法返回的接口。假设您必须使用类Widget，并且无论出于何种原因，extendWidget都不可能或不实际添加新功能。
当您需要跟踪有关对象的额外信息时会发生什么？在这种情况下，假设我们发现自己需要跟踪每个Widget的序列号，但是Widget类实际上没有序列号属性 - 并且由于Widget不可扩展，我们无法添加一个。没问题，这就是HashMaps所代表的：
serialNumberMap.put（widget，widgetSerialNumber）;
我们必须知道（100％确定）何时不再需要特定的Widget序列号，因此我们可以从Map中删除其条目。否则我们将会发生内存泄漏（如果我们不应该删除Widgets）或者我们会莫名其妙地发现自己缺少序列号（如果我们删除了我们仍在使用的Widgets）。如果这些问题听起来很熟悉，那么它们应该是：它们正是非垃圾收集语言的用户在尝试管理内存时所面临的问题，我们不应该用像Java这样的文明语言来担心这个问题。
**强引用的另一个常见问题是缓存**，特别是对于像图像这样的非常大的结构。假设您有一个必须使用用户提供的图像的应用程序，例如我工作的网站设计工具。当然，您希望缓存这些图像，因为从磁盘加载它们非常昂贵，并且您希望避免在内存中同时存储两个（可能是巨大的）图像副本的可能性。
因为当我们不绝对需要时，**图像缓存**应该阻止我们重新加载图像，你会很快意识到缓存应该总是包含对已经在内存中的任何图像的引用。但是，对于普通的强引用，该引用本身将强制图像保留在内存中，这需要您（如上所述）以某种方式确定何时在内存中不再需要该图像并将其从缓存中删除，以便它变为有资格进行垃圾收集。您再次被迫复制垃圾收集器的行为并手动确定对象是否应该在内存中。
**弱引用**
简单地说，弱引用是一个不足以强迫对象保留在内存中的引用。弱引用允许您利用垃圾收集器为您确定可达性的能力，因此您不必自己执行此操作。你创建一个像这样的弱引用：
WeakReference <Widget> weakWidget = new WeakReference <Widget>（widget）;
然后在代码的其他地方你可以使用wewWidget.get（）来获取实际的Widgetobject。当然，弱引用不足以阻止垃圾收集，因此您可能会发现（如果没有对小部件的强引用）weakWidget.get（）突然开始返回null。
要解决上面的“小部件序列号”问题，最简单的方法是使用内置的WeakHashMap类.
WeakHashMap的工作原理与HashMap完全相同，只是使用弱引用引用键（而不是值！）。如果**WeakHashMap键变为垃圾**，则**会自动删除其条目**。这避免了我描述的陷阱，并且除了从HashMap切换到WeakHashMap之外不需要任何更改。如果您遵循通过Map接口引用地图的标准惯例，则其他代码甚至不需要知道更改。
**Reference queues**
一旦WeakReference开始返回null，它指向的对象就变成了垃圾，WeakReference对象几乎没用。这通常意味着需要进行某种清理;例如，WeakHashMap必须删除这些已经过时的条目，以避免持有越来越多的deadWeakReferences。
ReferenceQueue类可以轻松跟踪死引用。如果将ReferenceQueue传递给弱引用的构造函数，则当引用它的对象变为垃圾时，引用对象将自动插入引用队列。然后，您可以按照一定的时间间隔处理ReferenceQueue并执行死引用所需的任何清理。
**不同程度的弱引用**
到目前为止，我刚刚提到“弱引用”，但实际上有四种不同程度的参考强度：强，弱，弱和幻像，从最强到最弱。我们已经讨论过强弱参考，所以让我们来看看另外两个。
**软引用**
**软引用与弱引用完全相同**，只是它**不太愿意丢弃它引用的对象**。一个只能弱到达的对象（对它的最强引用是WeakReferences）将在**下一个垃圾收集周期被丢弃**，但是一个可以轻松到达的对象通常会暂停一段时间。
**SoftReferences不需要与WeakReferences有任何不同的行为**，但实际上，只要内存供应充足，就可以保留软可访问对象。这使得它们成为缓存的良好基础，例如上面描述的图像缓存，因为您可以让垃圾收集器担心对象的可达性（永远不会从缓存中移除强可达对象）以及它有多糟糕需要他们消耗的记忆。
**幻影引用**
幻像引用**与软引用或WeakReference完全不同**。它对它的对象的抓握是如此脆弱，你甚至无法检索对象 - 它的get（）方法总是返回null。这种引用的唯一用途是**跟踪它何时被引入ReferenceQueue**，因为在那时你知道它指向的对象已经死了。但是，与WeakReference有何不同？
区别在于排队发生的时间。一旦他们指向的对象变得微弱，WeakReferences就会排队。这实际上**发生在最终化或垃圾收集之前**;理论上，对象甚至可以通过非正统的finalize（）方法“复活”，但WeakReference仍然会死亡。只有在从内存中物理移除对象时才会将幻像引用排队，并且get（）方法始终返回null，以防止您“复活”几乎死亡的对象。
幻像引用有什么用？我只知道它们有两个严重的情况：首先，它们允许您确定何时从内存中删除对象。事实上，它们是确定这一点的唯一方法。这通常不是很有用，但在某些非常特殊的情况下可能会派上用场，例如**操作大图像**：如果你**确定图像应该被垃圾收集**，你可以等到它实际上是在尝试加载下一个图像之前，因此不太可能使可怕的OutOfMemoryError。
其次，幻像引用避免了最终化的基本问题：**f​​inalize**（）方法可以通过**创建对它们的新的强引用来“复活”对象**。那么，你说什么？好吧，问题是现在必须确定一个覆盖最终化（）的对象至少在两个单独的垃圾收集周期中是垃圾才能被收集。当第一个周期确定它是垃圾时，它有资格完成。由于在完成期间对象被“复活”的（微小的，但不幸的是真实的）可能性，垃圾收集器必须在实际移除对象之前再次运行。并且由于最终确定可能没有及时发生，因此在对象等待最终确定时可能会发生任意数量的垃圾收集周期。这可能意味着实际清理垃圾对象的严重延迟，这就是为什么即使大多数堆都是垃圾也可以getOutOfMemoryErrors。
使用PhantomReference，这种情况是不可能的 - 当PhantomReference入队时，绝对没有办法获得指向现在死对象的指针（这很好，因为它不再存在于内存中）。因为PhntomReference不能用于复活对象，所以可以在第一个垃圾收集周期中立即清理该对象，在该周期中可以发现该对象是幻象可达的。然后，您可以在方便时处置所需的任何资源。
可以说，应该首先提供finalize（）方法。 PhantomReferences肯定更安全，使用效率更高，而且甚至最大化（）会使虚拟机的某些部分变得更加简单。但是，它们也需要更多的工作来实现，所以我承认在大多数时候仍然使用finalize（）。好消息是，至少你有一个选择。
---------------
## [ReferenceQueue的使用](https://www.cnblogs.com/dreamroute/p/5029899.html)
我们希望当一个对象被gc掉的时候通知用户线程，进行额外的处理时，就需要使用引用队列了。ReferenceQueue即这样的一个对象，当一个obj被gc掉之后，其相应的包装类，即ref对象会被放入queue中。我们可以从queue中获取到相应的对象信息，同时进行额外的处理。比如反向操作，数据清理等。
![](https://img-blog.csdnimg.cn/20190326142233263.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
在这次处理中，map并没有因为不断加入的1M对象由产生OOM异常，并且最终运行结果之后map中的确有1万个对象。表示确实被放入了相应的对象信息。不过其中的key(即weakReference)对象中的byte[]对象却被回收了。即不断new出来的1M数组被gc掉了。
从命令行中，我们看到有9995个对象被gc，即意味着在map的key中，除了weakReference之外，没有我们想要的业务对象。那么在这样的情况下，是否意味着这9995个entry，我们认为就是没有任何意义的对象，那么是否可以将其移除掉呢。同时还期望size值可以打印出5，而不是10000.
WeakHashMap就是这样的一个类似实现。
![](https://img-blog.csdnimg.cn/20190326142518558.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326142616241.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)这个也可以理解为就是一个类似cache的实现。
在cache中，key不重要并且通常都很少，value才是需要对待的。这里通过监控value变化，反向修改map，以达到控制kv的目的，避免出现无用的kv映射。
-----------------
## [利用虚引用PhantomReference实现对象被回收时收到一个系统通知](https://blog.csdn.net/zmx729618/article/details/79423255)
虚引用PhantomReference， 在<<深入理解Java虚拟机>>一文中，它唯一的目的就是为一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知。
```python
import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.reflect.Field;
 
public class Test {
    public static boolean isRun = true;
    @SuppressWarnings("static-access")
    public static void main(String[] args) throws Exception {
        String abc = new String("abc");
        System.out.println(abc.getClass() + "@" + abc.hashCode());
        final ReferenceQueue<String> referenceQueue = new ReferenceQueue<String>();
        new Thread() {
            public void run() {
                while (isRun) {
                    Object obj = referenceQueue.poll();
                    if (obj != null) {
                        try {
                            Field rereferent = Reference.class
                                    .getDeclaredField("referent");
                            rereferent.setAccessible(true);
                            Object result = rereferent.get(obj);
                            System.out.println("gc will collect："
                                    + result.getClass() + "@"
                                    + result.hashCode() + "\t"
                                    + (String) result);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }.start();
        PhantomReference<String> abcWeakRef = new PhantomReference<String>(abc, referenceQueue);
        abc = null;
        Thread.currentThread().sleep(3000);
        System.gc();
        Thread.currentThread().sleep(3000);
        isRun = false;
    }
}
```


