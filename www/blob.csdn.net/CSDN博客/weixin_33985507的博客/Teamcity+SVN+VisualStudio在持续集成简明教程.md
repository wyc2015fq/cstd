# Teamcity+SVN+VisualStudio在持续集成简明教程 - weixin_33985507的博客 - CSDN博客
2015年10月23日 21:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

Teamcity+SVN+VisualStudio持续集成
简明教程
一、写在最前：
1.      各组件版本号例如以下：
Teamcity（简称tc）版本号：8.1.4
SVN版本号：TortoiseSVN 1.8.7
VS版本号：VS2010. 10.0.30319.1
2.      关于CI（Continuousintegration，持续集成）：
其重要性与关键性自不必多说。因为不是本文重点，因此略过。能够參考：[http://en.wikipedia.org/wiki/Continuous_integration](http://en.wikipedia.org/wiki/Continuous_integration)（概念） 以及
[http://en.wikipedia.org/wiki/Comparison_of_continuous_integration_software](http://en.wikipedia.org/wiki/Comparison_of_continuous_integration_software)（CI工具比較）
二、安装
安装过程不细表，向导式安装。一路next就可以。只是有一个地方请注意： 
1.      tc对外服务port，最好不要使用80port（由于常被占用）。这里我改动为60380port
![](https://img-blog.csdn.net/20140919170813062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装完成后，记住两个东西：例如以下图
a.      tc的服务链接地址：[http://localhost:60380](http://localhost:60380/) （假设是跨网訪问，在訪问时请将localhost改动为ip地址）
b.      tc的工作文件夹: C:\TeamCity\buildAgent\work （tc会将代码checkout至该文件夹）
![](https://img-blog.csdn.net/20140919171014360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装完成后，会自己主动打开浏览器，进入web UI页面
三、使用
1.      初次会弹出设置向导。须要注意的是在数据库设置中，尽管tc非常谦虚的推荐其它类型数据库，但从我们使用实践下来，它内部的HSQLDB数据库已经能够非常好的工作了，因此我们推荐直接使用该数据库（还省去了配置外部数据库的麻烦）。
其它设置使用默认就可以。
2.      等待web UI页面初始化完毕之后，首先注冊一个用户，然后使用该用户登录
登陆后，我们能够看到已经存在了一个tc代理server
![](https://img-blog.csdn.net/20140919170827062?)
3.      创建project。 点击“Create project”，填写例如以下元素后，创建project
![](https://img-blog.csdn.net/20140919171024703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.      创建编译配置
4.1    General Settings中点击Create build configuration
![](https://img-blog.csdn.net/20140919171028977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.2    创建编译配置
![](https://img-blog.csdn.net/20140919171031769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.3    创建代码管理器属性
选择VCS类型（演示样例使用的是SVN，其它代码管理软件配置类似），完毕代码管理器属性配置。（演示样例使用的IDE是VS2010，因此SVN Connection Settings属性中的URL选择的是解决方式(.sln)所在的文件夹）
配置完毕后。測试连接成功则通过。
![](https://img-blog.csdn.net/20140919171035326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.4    设置成品路径
/subprj1/bin/release/win32/*.exe=> .\testsln\subprj1\release\Win32\
说明：将vs解决方式相对文件夹下(/subprj1/bin/release/win32/)的后缀名为.exe的文件，所有拷贝至.\testsln\subprj1\release\Win32\下， 一般为文件夹C:\ProgramData\JetBrains\TeamCity\system\artifacts\SilkwaySvn20140521）
![](https://img-blog.csdn.net/20140919171038649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.5    设置编译步骤
点击左側的BuildSteps。然后配置执行器。（演示样例使用的是VS执行环境）
![](https://img-blog.csdn.net/20140919170847046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
至此。能够点击右上角的Runbutton，进行集成编译測试。
![](https://img-blog.csdn.net/20140919171045669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后至tc的工作文件夹下。检查编译及生成情况。（能够通过构建日志，查看tc的checkout文件夹）
![](https://img-blog.csdn.net/20140919170854609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JlYXQzNzc5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.6    设置触发器
设置触发器的目的在于根据条件自己主动进行编译及集成。
加入新触发器
![](https://img-blog.csdn.net/20140919171051971?)
选择VCS触发器，比如勾选“Trigger a build on eachcheck-in”，能够设置为当每次有代码迁入时，均编译；其它经常使用的设置有每天定时编译等选项。
![](https://img-blog.csdn.net/20140919171055154?)
版权声明：本文博主原创文章。博客，未经同意不得转载。
