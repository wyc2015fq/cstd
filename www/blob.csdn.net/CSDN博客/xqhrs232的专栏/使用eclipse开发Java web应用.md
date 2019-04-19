# 使用eclipse开发Java web应用 - xqhrs232的专栏 - CSDN博客
2018年09月29日 17:08:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：63
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/freeweb/p/4940151.html](https://www.cnblogs.com/freeweb/p/4940151.html)
　前面说了手动配置一个应用，手动配置可以更深入的理解web应用的分布，但是一般的编辑器没有语法错误提示，所以开发起来对于错误的寻找不太容易，效率相对较低，所以在理解清楚web项目的结构之后，我们使用eclipse开发的话，能够使效率更高
　　开发可以使用eclipse for JavaEE或者MyEclipse都是可以的，项目目录大致相同，只是运行的方式不一样，eclipse是配置虚拟的环境执行应用，MyEclipse运行的时候还是把项目发布到tomcat服务器下的webapps中去运行，两者开发起来都一样，下面使用eclipse来说一下Java web应用的开发步骤
　　启动eclipse后，点击菜单栏中的"File"->"New"->"Dynamic Web Project"新建一个动态网站项目
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105163528742-1532013069.jpg)
　　然后弹出对话框中项目名称填入Jsp1，默认情况下Target runtime中的选项值none，我们需要选择tomcat运行环境，点击New Runtime按钮
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105163810461-1870444548.jpg)
　　然后弹出新的对话框，环境根据我们计算机上实际的运行环境选择，这里我选择 Apache Tomcat v8.0然后单击 Finish
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164011352-1802587844.jpg)
　　然后单击Browse选择tomcat服务器的安装位置，就是CATALINA_HOME环境变量的路径，然后单击Finish
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164134524-842004432.jpg)
　　那么现在，一个Web项目就建立好了，目录结构大体如下图：
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164232680-1774971943.jpg)
　　其中WebContent就是我们jsp源文件的存放目录，下面有WEB-INF和META-INF两个目录，和我们之前的web应用目录一致
　　那么现在我们新建一个源文件，在WebContent上右击，选择"New"->"Jsp File"
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164455633-941110292.jpg)
　　弹出的菜单中在File name中填入jsp的文件名，这里填写index.jsp，然后单击Finish
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164622883-716458967.jpg)
　　好的，此时我们的一个简单的web应用就算建立完成了，这时可以单击上方的绿色运行按钮运行应用，也可以在代码编辑器中右击在弹出菜单中选择"Run As"->"Run on Server"运行程序
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164843742-1504121211.jpg)
　　这个时候弹出对话框，我们直接点击Finish就开始运行了
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105164941586-909901598.jpg)
　　此时服务器启动了，然后在eclipse中打开一个内置窗口，然后会显示出相应的页面
　　注意此时在我们eclipse工作目录下，就是和项目的同级目录下会出现一个Servers目录，这个目录就是eclipse配置运行环境所生成的目录，项目管理器中也可以看到
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105165224024-1276688735.jpg)
　　注意，这个目录不要删除，否则程序不能运行，还需要重建，Server.xml、web.xml、context.xml等配置文件就是存在于这个目录下
　　目前服务器是处于运行状态的，我们打开浏览器输入http://localhost:8080/Jsp1/index.jsp也可以正确访问到
　　此时eclipse下方窗口Servers选项卡中可以看到我们配置的服务器和运行的项目列表
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105165945196-1808836855.jpg)
　　我们可以很方便的右击选择启动，停止，重启等操作，然后通过浏览器进行项目的调试
　　eclipse的项目目录不能直接复制到webapps下面，但是我们可以把WebContent和build目录进行整合到原生的项目中，手动运行；更常用的方式是把项目导出war格式的包，直接放到tomcat应用目录下即可，运行时服务器会自动解包
　　最后补充一点，如果我们配置的服务器太多，或者误删除导致服务器失效，那么我们怎么删除原来的配置选项重新开始配置呢
　　首先右击项目管理器中的Servers目录，选择Delete然后勾选删除文件删除即可
　　然后下面的服务器停止后我们也是右击勾选删除配置并删除该服务器选项
　　最后单击菜单栏中的"Window"->"Preferences"，就是首选项的意思
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105170635805-1824006281.jpg)
　　然后在弹出的对话框中，左侧选择"Server"->"Runtime Environments"然后在右侧选择服务器点击"Remove"按钮即可移除，最后单击OK按钮完成即可
![](https://images2015.cnblogs.com/blog/734555/201511/734555-20151105170935617-404876218.jpg)
　　这个时候已经清理干净，我们就可以重新开始配置Java web的开发环境了
