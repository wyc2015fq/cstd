# IntelliJ IDEA 运行你的第一个Java应用程序 - 数据之美的博客 - CSDN博客
2018年06月03日 14:53:44[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：291
IntelliJ IDEA 运行你的第一个Java应用程序
创建项目让我们创建一个简单的Java Hello World项目。
单击创建新的项目。
打开新建项目向导。![](https://img-blog.csdn.net/20160630212831581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
你应该注意的主要是[项目的SDK](https://www.jetbrains.com/help/idea/2016.1/sdk.html)。SDK（软件开发套件）是一套软件开发工具，可以让你更快的开发应用程序。IntelliJ IDEA的不包括SDK，所以如果你有没有，下载并安装它。因为我们做一个Java项目，我们需要一个[Java SDK](http://blog.csdn.net/testcs_dn/article/details/50277347)（[JDK](http://blog.csdn.net/testcs_dn/article/details/40274123)）。
![](https://img-blog.csdn.net/20160630212900488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装完毕后，单击新建，指向JDK的安装文件夹。![](https://img-blog.csdn.net/20160630212920535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在新建项目向导，你可以选择你的项目支持的技术，但你正在做一个普通的Java应用程序，不需要选择他们，只需单击下一步。然后IDE为您提供了从模板创建项目。我们现在不需要这个，所以单击下一步。![](https://img-blog.csdn.net/20160630213410084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
指定项目的名称和位置。点击完成。![](https://img-blog.csdn.net/20160630213515085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
你有src文件夹在项目中。这是你的源代码的文件夹。右击该文件夹，然后选择New | Java类。![](https://img-blog.csdn.net/20160630213745986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入这个类的名称“quickstart.HelloWorld”，并单击 OK。
![](https://img-blog.csdn.net/20160630213948899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在，新的类在编辑器中打开。请注意，这个 HelloWorld 类是快速入门包中创建的。HelloWorld 程序的文本是已知的
[java][view plain](https://blog.csdn.net/testcs_dn/article/details/51793511#)[copy](https://blog.csdn.net/testcs_dn/article/details/51793511#)
- package quickstart;  
- 
- publicclass HelloWorld {  
- publicstaticvoid main(String[] args) {  
-         System.out.println("Hello, World!");  
-     }  
- }  
把它插入类中。
![](https://img-blog.csdn.net/20160630214311776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
运行一个应用程序时，都需要有一个配置，这个过程是基本的。为了创建一个运行配置，点击Run | 编辑配置，然后单击 ![](https://img-blog.csdn.net/20160630214654652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，然后选择应用。![](https://img-blog.csdn.net/20160630215003122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在这里，您可以指定主类和程序参数。因为我们有一个简单的应用程序，我们只需要指出的主类。把quickstart.HelloWorld到主类文本字段。打印在名称字段中的东西，例如：HelloWorld。运行配置已准备就绪。![](https://img-blog.csdn.net/20160630215118935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在，您可以立即运行你的应用程序，并确保所有工作正常。选择Run | 运行'HelloWorld'从主菜单。![](https://img-blog.csdn.net/20160630215308967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在左下角的窗口中得到的结果！
![](https://img-blog.csdn.net/20160630215321928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
为了调试应用程序，选择Run | 调试。![](https://img-blog.csdn.net/20160630215813594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
你也为此编辑配置。要了解更多信息，请参阅[运行/调试配置：应用](https://www.jetbrains.com/help/idea/2016.1/run-debug-configurations.html)。
在代码行前页面箭头的位置点击鼠标左键可以设置断点。![](https://img-blog.csdn.net/20160630215827610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上一篇：[IntelliJ IDEA的安装环境要求](http://blog.csdn.net/testcs_dn/article/details/51741298)
下一篇：[运行你的第一个Java应用程序](http://blog.csdn.net/testcs_dn/article/details/51793511)
==================相关参考======================
入门：
 - [安装和启动](http://blog.csdn.net/testcs_dn/article/details/51755616)
语言和特有框架准则：
 - [创建项目](http://blog.csdn.net/testcs_dn/article/details/52039595)
原文：[Run Your First Java Application](https://www.jetbrains.com/help/idea/2016.1/quick-start-guide.html#run_first)
