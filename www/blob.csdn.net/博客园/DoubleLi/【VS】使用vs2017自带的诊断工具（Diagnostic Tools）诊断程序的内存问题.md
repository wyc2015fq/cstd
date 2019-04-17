# 【VS】使用vs2017自带的诊断工具（Diagnostic Tools）诊断程序的内存问题 - DoubleLi - 博客园






### 前言

一般来说.NET程序员是不用担心内存分配释放问题的，因为有垃圾收集器（GC）会自动帮你处理。但是GC只能收集那些不再使用的内存（根据对象是否被其它活动的对象所引用）来确定。所以如果代码编写不当的话，仍然会出现内存泄漏的问题，常见的情况有：一个静态变量引用了一个应该被释放的对象，事件注册后不解除注册，非托管资源使用后没有手动释放。不断的内存泄漏终会引起内存不足，挂掉你的程序。

对于这种内存泄漏问题，有很多的分析工具可以使用，常见的有CLRProfiler、ANTS Performance Profiler等。不过从vs2013起，vs自带了一个分析工具-Diagnostic Tool，默认debug时会自动打开，如果没有打开的话就按快捷键Ctrl+Alt+F2。

如下图： 
![这里写图片描述](https://img-blog.csdn.net/20180108144838765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 开始使用

这里以静态变量持有应释放的对象为例，简单介绍下使用方法。 
这里有个winform程序，主要功能即使点击按钮后，输出名为“jim”的个人信息：

```
private void button1_Click(object sender, EventArgs e)
{
     var p = PersonManager.Get("jim");
     label1.Text += p.Name + " " + p.Age + "\n";
}
```
- 1
- 2
- 3
- 4
- 5

Person和PersonManager类的信息如下：

```
namespace MemLeak
{
    public class Person
    {
        public string Name { get; set; }
        public int Age { get; set; }
        public byte[] BinaryData { get; set; }
    }
    public static class PersonManager
    {
        static List<Person> people = new List<Person>();
        public static List<Person> People
        {
            get
            {
                return people;
            }
        }
        public static Person Get(string name)
        {
            //正常逻辑
            //var per = People.FirstOrDefault(o => o.Name == name);
            //if(per==null)
            //{
            //    per = new Person();
            //    per.Age = 23;
            //    per.Name = name;
            //    per.BinaryData = File.ReadAllBytes(@"D:\2.zip");//一个38MB的文件
            //    people.Add(per);
            //}
            //return per;
            //错误逻辑
            Person p = new Person();
            p.Age = 23;
            p.Name = name;
            p.BinaryData = File.ReadAllBytes(@"D:\2.zip");//一个38MB的文件
            people.Add(p);
            return p;
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41

#### 1. 捕获内存快照

程序运行起来后，从下图可以看到内存占用大约在18M左右，此时点击“截取快照”新建一个快照： 
![这里写图片描述](https://img-blog.csdn.net/20180108145745304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后click按钮5次，然后再点击 截图快照 ，如下： 
![这里写图片描述](https://img-blog.csdn.net/20180108145946982?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从曲线图中可以明显看到内存从18M逐步增长到227M。第二次快照与第一次相比对象增加了19个，堆上的内存使用增加了191M左右。 
![这里写图片描述](https://img-blog.csdn.net/20180108150135394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击图中的（+19）字样，可以进入到快照详细对比界面。

#### 2. 详细对比

![这里写图片描述](https://img-blog.csdn.net/20180108150637135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示，我们先来看下选中的那一行是什么意思： 
![这里写图片描述](https://img-blog.csdn.net/20180108150914510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到`MemLeak.Person`这个对象的计数差异为+5，计数是5。表示这两次快照之间这个`Person`对象从0个变成了5个增加了5个。大小差异是增加了195601040个字节，这几个从字面意思看都很好理解。唯一有点难度的可能是`非独占大小（字节）`，一眼看不出来是干什么的。但是我们注意到了`List<MemLeak.Person>`对象的大小是68字节而非独占大小却是195601108字节。 
非独占大小的统计方式如下： 
![这里写图片描述](https://img-blog.csdn.net/20180108151830463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中最下层的`string`和`byte[]`，由于没有子节点了，所以它们的非独占大小就是它们本身的大小。`Address`的非独占大小=自己本身的大小+`String`的非独占大小（50字节）+Byte[]的非独占大小（100字节）=200字节。`Customer`的非独占大小=自己本身（100字节）+`String`的非独占大小（100字节）+`Address`的非独占大小（200字节）=400字节。 
所以这也就解释了`List<MemLeak.Person>`，自身大小仅是68字节，但是由于它是一个List内部包含了5个`Person`对象，所以它的非独占大小就是5个`Person`对象大小的和。

#### 3. 哪里产生了泄漏？

这里我先将过滤器里“仅我的代码”和“折叠小型对象类型”前面的勾去掉，便于分析。 
![这里写图片描述](https://img-blog.csdn.net/20180108152712985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为我们值需要看当前程序集（MemLeak）的内存信息，所以在搜索类型名的框里直接输入我们程序集的名称，将其他一些没用的过滤掉。 
![这里写图片描述](https://img-blog.csdn.net/20180108152936402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
是不是清爽了很多！ 
分析内存当然要拿占用内存增多最大的对象开刀，点击`MemLeak.Person`，在下面的“根的路径”视图中可以看到增加的5个`Person`对象是在一个`Person[]`数组中。 
![这里写图片描述](https://img-blog.csdn.net/20180108153402921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是我们好像并没有声明过这样的数组，所以继续展开： 
![这里写图片描述](https://img-blog.csdn.net/20180108153508488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
原来是在一个`List`里面，而且这个`List`还是静态的。这时候就要想到内存泄漏的原因，可能就是代码哪里的静态变量持续占有应该释放的对象了。然后更改为正确代码就OK了。

#### 4. 查看对象中哪种数据类型占用内存较多？

切换到的引用选项卡（“引用的类型”）。 
![这里写图片描述](https://img-blog.csdn.net/20180108155411933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到本例中`Person`的`Byte[]`类型的占用内存较多，所以我们就可以重点排查`Person`中哪里用到了`Byte[]`类型，进而去优化。

### 当初取消勾选的那两个选项是什么？

第一个是“仅我的代码”： 
勾上之后，会过滤掉那些.Net Runtime产生的一些对象和一些很常见的系统认为与你的程序无关的一些对象。

第二个是：“折叠小的对象类型”： 
勾上后，会过隐藏掉那些非独占大小小于托管堆总大小0.5%的对象。将这些小对象的非独占大小合并的父节点中。如下： 
![这里写图片描述](https://img-blog.csdn.net/20180108155128992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这两个选项默认都勾上即可。

### 其它

对于生产环境中的软件，分析内存时可以多次抓取dump文件，用vs打开后分析比对。 
![这里写图片描述](https://img-blog.csdn.net/20180108160100734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2F0c2hpdG9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
不过只有vs的企业版才有这个功能。

### 参考

> 
[Memory Usage Tool while debugging in Visual Studio 2015](https://blogs.msdn.microsoft.com/devops/2014/11/13/memory-usage-tool-while-debugging-in-visual-studio-2015/)










