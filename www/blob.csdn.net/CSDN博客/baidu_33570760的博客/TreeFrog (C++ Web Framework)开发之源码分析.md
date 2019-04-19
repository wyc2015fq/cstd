# TreeFrog (C++ Web Framework)开发之源码分析 - baidu_33570760的博客 - CSDN博客
2017年06月20日 11:32:31[carman_风](https://me.csdn.net/baidu_33570760)阅读数：278
个人分类：[qt																[软件安装与使用](https://blog.csdn.net/baidu_33570760/article/category/6940159)](https://blog.csdn.net/baidu_33570760/article/category/6898155)
   转载地址：http://blog.csdn.net/foruok/article/details/17498793
    为了弄明白 TreeFrog 的逻辑，我分析了它的源码。将程序开始至开发人员自己的工程如何被调用的路线理清楚了。
    工程 tfmanager 是整个框架的起点，它生成 treefrog 守护进程，还记得吗，"treefrog -e dev" 这个命令，我们用来启动网站的？
    工程 tfserver 是真正的站点入口，负责处理 http 请求，它生成 tadpole 。
    tfmanager 会监控 tfserver 的运行，如果 tfserver 莫名崩溃，会重启它。
    tfserver 的多任务处理模型有三种：多进程、多线程、混合型。这里我们只分析多线程模型。要使用哪种模型可以通过配置文件指定，在 Windows 下，如果不指定，则默认使用多线程模型。多线程模型为每个 http 连接生成一个线程进行处理。
    开发者使用 treefrog 建立的工程，生成的是动态库，会被 tfserver 加载，tfserver 将 URL 处理为 controller 、 action 、 argument 三部分，参考[URL Routing](http://www.treefrogframework.org/documents/controller/url-routing) 这个文档。tfserver 分析 URL 后，提取
 controller 名字，找到开发者的 controller ，用 action 和 arguments 为参数进行调用。而 model 和 view 则是由 controller 进行关联的。就这样，一切发生了。
    下面是我分析源码后绘制的函数调用图，供参考：
![](https://img-blog.csdn.net/20131223085058468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9ydW9r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    根据上面的图，理解 treefrog 源码就很容易了。
