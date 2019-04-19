# Java小白手记：WEB项目等 - 左直拳的马桶_日用桶 - CSDN博客
2015年06月03日 23:39:06[左直拳](https://me.csdn.net/leftfist)阅读数：2012
个人分类：[学习笔记																[程序人生																[Java](https://blog.csdn.net/leftfist/article/category/2412493)](https://blog.csdn.net/leftfist/article/category/94502)](https://blog.csdn.net/leftfist/article/category/1790155)
机缘巧合之下，工作中得以用一下java。我向来对java很感兴趣，想从.NET转到java久矣，机会难得，久旱逢甘霖。
这次主要是跟web项目有关。在此之前，我了解到JAVA分为三大块：j2se、j2ee、j2me。粗略地讲，j2se做win form，j2ee做WEB，j2me for 手机开发。不过，j2ee的范围要比web广阔。
去年我只自学了一段时间，都是些基本语法之类，再加上几个月的android， java方面完全就是个门外汉。这次一下子接触到web，有些感触还是应该记下来，尽管个人理解不一定对，但总算是一个起点，日后可以从此再出发，避免时间一长完全没有印象，被迫从头开始。
**1、tomcat**
tomcat就是一个web服务器，apache基金会旗下的一个赞助项目，常为java web项目所用；与此类似的还有weblogic。据说，与weblogic 比起来，tomcat适用对象是一些小一点的项目。
在myeclipse上，可以直接将web项目部署在tomcat上，这样对web项目的一些修改，可以直接反映在tomcat上。不过，如果是引用jar包有变化，需要重启tomcat才能看到效果。
**2、jsp**
在我印象中，jsp是与asp并肩齐名的东东。但事实上，jsp比asp先进太多。尽管表面上看似乎差不多，都是在页面上嵌入服务器代码，连形式都一样：<%%>。但是，jsp是典型的MVC模式。因此，窃以为，jsp比asp.net的web form都要先进。
**3、servelet**
听上去，跟asp.net的handler、module差不多，就是拦截并处理客户端的请求的。跟Filter的区别是，servelet只对JSP等动态页面请求起作用，而Filter对所有资源，包括图片、样式等都起作用。
**4、mvc**
java web天然就是MVC模式，这真让我们这些.NET程序员感叹。JAVA就是多概念，多框架，一套一套的，好比除了这个MVC，他们的编码还习惯面向接口，等等。所以JAVA程序员素质比.NET程序员往往高出一茬。
**5、struct**
使用servelet就是struct?好像还有struct2，习惯用Filter。
**6、myeclipse**
myeclipse是eclipse一个插件。但这个插件跟我们平常所理解的嵌进容器进行使用的插件不一样，它插上去之后，反客为主，eclipse就变成了myeclipse，仿佛被注射了myeclipse的基因似的！我暂时还不知道myclipse在使用上有啥不同，只知道eclipse免费，而myeclipse收费，大概myeclipse功能更强一点吧。
然后我现在接触的是国内一家做工作流引擎公司的产品，他们有一个专用的编辑器，看上去是myeclipse的插件，插了myeclipse之后，打开myeclipse（他们命名为叉叉studio），界面就打上了他们公司的LOGO。
由此可见，也许eclipse功能没有vs那么强，但开放性绝对有过之而无不及，正如java的开放性要比.NET要强好多一样。
eclipse，英文的意思竟然是弯月、新月。西洋鬼起的名字都很怪异。什么意思？
7、packages
我们写代码的时候，应当放在一个个packages里面。按我的理解，packages就相当于.net里的命名空间，方便类的区隔。没有放在packages里的话，我在JSP里面想调用这个类，居然报找不到的错，不知道何故。不同于.net命名空间的是，packages会按照层次依次建立文件夹：比如com.leftfist.test，会建立 com/leftfist/test
8、JAVA_HOME
我安装JDK之后，在DOS窗口，键入java，是可以运行的，但javac就不行。检查系统的环境变量，没有JAVA_HOME；path也没有相关设置；所以这些东西还是要自己手动弄。
示例：
JAVA_HOME = E:/Java/jdk1.6.0_20
path += %JAVA_HOME%/bin;%JAVA_HOME%/jre/bin; 
CLASSPATH = .;%JAVA_HOME%/lib/dt.jar;%JAVA_HOME%/lib/tools.jar
具体可参考：
[win7下安装JDK并且配置环境变量](http://blog.csdn.net/yusiguyuan/article/details/14123669)
设置完毕后，可以在DOS方式下，键入java -version、javac命令来进行验证。如果在设置前已经打开了DOS窗口，那么新设置对此窗口尚未可见，应该关闭该窗口再打开一个新的进行验证。
9、汉字
窗口 - 首选项 - 常规 - 工作空间，选定编码
![](https://img-blog.csdn.net/20150827091248188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上设置是对整个工作空间而言的，如果是针对具体某页，可以在eclipse的菜单“编辑”中指定当前文件的编码。见
http://blog.csdn.net/leftfist/article/details/46363221
http://blog.csdn.net/leftfist/article/details/38564979
