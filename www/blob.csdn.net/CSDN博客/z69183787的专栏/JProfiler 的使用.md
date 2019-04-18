# JProfiler 的使用 - z69183787的专栏 - CSDN博客
2017年09月25日 12:52:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1655
**一、安装JProfiler**
**       从[http://www.ej-technologies.com/](http://www.ej-technologies.com/)下载5.1.2并申请试用序列号**
**二、主要功能简介**
**1．内存剖析 Memory profiler**
JProfiler 的内存视图部分可以提供动态的内存使用状况更新视图和显示关于内存分配状况信息的视图。所有的视图都有几个聚集层并且能够显示现有存在的对象和作为垃圾回收的对象。
- **所有对象 **
	显示类或在状况统计和尺码信息堆上所有对象的包。你可以标记当前值并显示差异值。
- **记录对象 Record objects **
	显示类或所有已记录对象的包。你可以标记出当前值并且显示差异值。
- **分配访问树 Allocation call tree **
	显示一棵请求树或者方法、类、包或对已选择类有带注释的分配信息的J2EE组件。
- **分配热点 Allocation hot spots **
	显示一个列表，包括方法、类、包或分配已选类的J2EE组件。你可以标注当前值并且显示差异值。对于每个热点都可以显示它的跟踪记录树。
**2．堆遍历 Heap walker**
在JProfiler的堆遍历器(Heap walker)中，你可以对堆的状况进行快照并且可以通过选择步骤下寻找感兴趣的对象。堆遍历器有五个视图：
- **类 Classes **
	显示所有类和它们的实例。
- **分配 Allocations **
	为所有记录对象显示分配树和分配热点。
- **索引 References **
	为单个对象和“显示到垃圾回收根目录的路径”提供索引图的显示功能。还能提供合并输入视图和输出视图的功能。
- **数据 Data **
	为单个对象显示实例和类数据。
- **时间 Time **
	显示一个对已记录对象的解决时间的柱状图。
**3． CPU 剖析 CPU profiler**
JProfiler 提供不同的方法来记录访问树以优化性能和细节。线程或者线程组以及线程状况可以被所有的视图选择。所有的视图都可以聚集到方法、类、包或J2EE组件等不同层上。CPU视图部分包括：
- **访问树 Call tree **
	显示一个积累的自顶向下的树,树中包含所有在JVM中已记录的访问队列。JDBC,JMS和JNDI服务请求都被注释在请求树中。请求树可以根据Servlet和JSP对URL的不同需要进行拆分。
- **热点 Hot spots **
	显示消耗时间最多的方法的列表。对每个热点都能够显示回溯树。该热点可以按照方法请求，JDBC，JMS和JNDI服务请求以及按照URL请求来进行计算。
- **访问图 Call graph **
	显示一个从已选方法、类、包或J2EE组件开始的访问队列的图。
**4． 线程剖析 Thread profiler**
对线程剖析，JProfiler提供以下视图:
- **线程历史 Thread history **
	显示一个与线程活动和线程状态在一起的活动时间表。
- **线程监控 Thread monitor **
	显示一个列表，包括所有的活动线程以及它们目前的活动状况。
- **死锁探测图表 Deadlock Detection **
	显示一个包含了所有在JVM里的死锁图表。
- **目前使用的监测器 Current monitor useage **
	显示目前使用的监测器并且包括它们的关联线程。
- **历史检测记录 History usage history **
	显示重大的等待事件和阻塞事件的历史记录。
- **监测使用状态 Monitor usage statistics **
	显示分组监测，线程和监测类的统计监测数据。
**5．VM 遥感勘测技术 VM telemetry**
观察JVM的内部状态，JProfiler提供了不同的遥感勘测视图，如下所示:
- **堆 Heap **
	显示一个堆的使用状况和堆尺寸大小活动时间表。
- **记录的对象 Recorded objects **
	显示一张关于活动对象与数组的图表的活动时间表。
- **垃圾回收 Garbage collector **
	显示一张关于垃圾回收活动的活动时间表。
- **类 Classes **
	显示一个与已装载类的图表的活动时间表。
- **线程 Threads **
	显示一个与动态线程图表的活动时间表。
**三、实战**
(一)任务目标
找出项目中内存增大的原因
  （二）配置说明
           操作系统：Windows2003
              Web容器：Tomcat5.0.23
              JDK版本：sun1.4.2
              监控类型：本地
              Jprofiler安装路径：D:/jprofiler5
              Tomcat安装路径：D:/Tomcat5
(三)  测试项目
1．  新建WEB项目test
2．  建包cn.test
3．  在该包下建类文件TestMain.java 和 TestBean.java
package cn.test;
public class TestBean {
   String name = "";
}
**package** cn.test;
**import** java.util.ArrayList;
**publicclass** TestMain {
**publicstatic** ArrayList*list* =**new** ArrayList(); //存放对象的容器
public static int counter = 0;    //作统计用
     }
       4.建测试用的JSP文件init1.jsp、init2.jsp
Init1.jsp（每次执行都创建1万个TestBean对象）
<%@ pagelanguage="java"import="cn.test.*"pageEncoding="ISO-8859-1"%>
<!DOCTYPEHTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>init</title>
</head>
<body><%
**for**(**int** i=0;i<10000;i++){
       TestBean b = **new** TestBean();
       TestMain.list.add(b);      
    }
%>
    SIZE:<%=TestMain.list.size()%><br/>
    counter:<%=TestMain.counter++%>
</body>
</html>
Init2.jsp和init1.jsp一模一样即可（后面有用）。
(四)     配置测试用例
1．  点击d:/jprofiler5/bin/jprofiler.exe
2．  执行菜单SessionàIntegration WizardsàNew ServerIntegration
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/1.GIF)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/2.GIF)
选择是本地测试还是远程测试：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/3.GIF)
选择tomcat运行的脚本文件：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/4.GIF)
选择虚拟机的类型:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/5.GIF)
选择监控端口：
用默认的即可
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/6.GIF)
选择Web容器是否和Jprofiler一起运行：
默认即可
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/7.GIF)
配置提示：
  在“远程控制”的时侯要仔细阅读一下。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/8.GIF)
然后选择立即起动，开始运行。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/9.GIF)
点击“OK”，我们可以看到另外一个小窗口出来了：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/10.GIF)
Jprofiler的窗口为：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/11.GIF)
这样我们就可以进行监控了！
(五)     开始测试
1．  在IE地址栏中输入：[http://localhost/test/init1.jsp](http://localhost/test/init1.jsp)，执行一次，我们可以在内存视图中看到cn.test.TestBean对象被创建了10000次：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/12.GIF)
2.标记现在的状态，然后再执行init1.jsp和,init2.jsp可以让我们找到哪些类在调用后没有被释放（很重要！！！）
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/14.GIF)
查看哪些类被发生了变化：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/15.GIF)
红色的变成是发生变化的对象及其数量。
我刚才执行了4次init1.jsp和1次init2.jsp，正好产生了50000个TestBean对象，和图示显示的一样。
3．  过一会后，按F4键进行垃圾回收。但回收完成后，这些对象依然存在，说明某些地方对这个类的引用没有被释放！
4．  找出是哪些地方使用了TestBean类，并且没有释放它们
在cn.test.TestBean对象上点击右键选择“Take Heap Snapshot for Selection”，观察它的heap
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/13.GIF)
下一步：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/16.GIF)
点击“OK”：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/17.GIF)
在该类中点击右键，在出现的菜单中选择“Use Selected Objects”：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/18.GIF)
出现如下窗口：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/19.GIF)
选择”Allocations”，点击“OK”,然后我们要的结果就出来了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/djy1135/20.GIF)
图中显示调用此类的地方是init1.jsp和init2.jsp，并且各自占用的比率都列出来了。
既然问题的所在找出来了，接下来就该去解决问题了！
（五）总结
其实，我们在测试内存占用时还可以另外写一个释放内存的JSP文件来配合测试，会更清楚一些：
Free.sjp
<%@ pagelanguage="java"import="java.util.*,cn.test.*"pageEncoding="ISO-8859-1"%>
<!DOCTYPEHTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>init</title>
</head>
<body>
<%TestMain.list.clear();%>  collection OK!
</body>
</html>
在点击完init1.jsp或init2.jsp后，可以看到内存是TestBean对象的数量增加了，然后执行free.sjp，接着再执行F4进行垃圾回收，立刻可以看到TestBean对象被释放掉了。
