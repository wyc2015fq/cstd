# JNA调用DLL函数遇到的几个问题 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 01:24:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27
个人分类：[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)










最近一个JSP项目需要用到分词模块，而分词模块实用C++写成的DLL库。于是上网搜各种方法，最后选择了JNA作为JSP调用DLL的工具。

JNA（Java Native Access ）提供一组Java工具类用于在运行期动态访问系统本地库（native library：如Window的dll）而不需要编写任何Native/JNI代码。开发人员只要在一个java接口中描述目标native library的函数与结构，JNA将自动实现Java接口到native function的映射。

JNA的使用方法有很多，这里只总结下我遇到的问题：

#### DLL放置的路径问题

JSP本身分为bean的src目录和WebRoot的脚本目录，而我们的DLL需要在src下的类中调用，于是遇到了将DLL和资源文件放置到什么地方的问题。

最后解决方法：**将调用的DLL和LIB文件拷贝到C:/Windows/System32目录下**可以访问。

解决过程：我试过将这些文件放到Src目录下，然后用Class.class.getResource("/").getPath()这种方法得到类的class路径，再得到资源文件的路径，结果没有成功。

#### DLL调用资源的路径问题

DLL可能会调用其他资源文件，比如我这里需要用到词典，路径也是个问题，不过这个较为简单，很容易解决：

将资源文件目录，**拷贝到Src目录下面。在程序中使用Class.class.getResource("/").getPath()获得编译后的类所在的根目录**，再连接上资源文件即可。



原文地址：[http://www.crazyant.net/2012/12/26/jsp使用jna调用dll函数遇到的几个问题/](http://www.crazyant.net/2012/12/26/jsp%E4%BD%BF%E7%94%A8jna%E8%B0%83%E7%94%A8dll%E5%87%BD%E6%95%B0%E9%81%87%E5%88%B0%E7%9A%84%E5%87%A0%E4%B8%AA%E9%97%AE%E9%A2%98/)




