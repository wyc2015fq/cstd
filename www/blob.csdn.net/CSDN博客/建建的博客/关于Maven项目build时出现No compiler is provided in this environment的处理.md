# 关于Maven项目build时出现No compiler is provided in this environment的处理 - 建建的博客 - CSDN博客
2018年02月03日 10:23:43[纪建](https://me.csdn.net/u013898698)阅读数：271
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
                
近日有同事遇到在编译Maven项目时出现
[ERROR] No compiler is provided in this environment. Perhaps you are running on a JRE rather than a JDK?
的问题, 原以为这是个个例, 源于同事粗心, 配置环境出问题造成, 后到百度查看一下, 遇到这个问题的不在少数, 但是对问题的解释没有说到根源, 于是写下这篇博客供大家参阅, 如有纰漏, 还请指正.
错误代码节选:
**[java]**[view plain](http://blog.csdn.net/lslk9898/article/details/73836745#)[copy](http://blog.csdn.net/lslk9898/article/details/73836745#)
- [ERROR] COMPILATION ERROR :   
- [INFO] -------------------------------------------------------------  
- [ERROR] No compiler is provided in this environment. Perhaps you are running on a JRE rather than a JDK?  
- [INFO] 1 error  
- [INFO] -------------------------------------------------------------  
- [INFO] ------------------------------------------------------------------------  
- [INFO] BUILD FAILURE  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Total time: 1.436 s  
- [INFO] Finished at: 2017-06-28T11:16:07+08:00
- [INFO] Final Memory: 10M/151M  
- [INFO] ------------------------------------------------------------------------  
- [ERROR] Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project manage: Compilation failure  
- [ERROR] No compiler is provided in this environment. Perhaps you are running on a JRE rather than a JDK?  
- [ERROR] -> [Help 1]  
但是编写普通Java Project编译运行却是正常的,下图为只有输出语句的普通java类
![](https://img-blog.csdn.net/20170628124433704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上图中可以看出, java编译环境未jre1.7.0_17, 也就是说并没有配置成jdk目录, 然后看Eclipse-->Window-->preferences-->Java-->Installed JREs
![](https://img-blog.csdn.net/20170628124908232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了演示出效果, 在测试之前, 我已经将系统java环境配置成如上图所示路径, 并只保留该配置, 由下图可以看出, 该路径是我所安装的两个JDK版本中的一个JDK自带的jre运行环境. 使用该环境编译普通项目没有问题, 但为什么会在编译Maven项目时出错呢?
![](https://img-blog.csdn.net/20170628124924204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们看看Maven的环境是如何配置的:先找到Eclipse-->Window-->preferences-->Maven-->Installations
![](https://img-blog.csdn.net/20170628125448111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在Maven配置中, 我并没有使用Eclipse自带的Maven插件, 而是重新配置的Maven环境, 然后再看Eclipse-->Window-->preferences-->Maven-->User Settings
![](https://img-blog.csdn.net/20170628125658393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Maven设置使用的是Maven中conf文件夹下的settings.xml, 点击"open file" 在Eclipse中查看具体配置信息, 仅摘录与本错误信息相关的部分
**[html]**[view plain](http://blog.csdn.net/lslk9898/article/details/73836745#)[copy](http://blog.csdn.net/lslk9898/article/details/73836745#)
- <profiles>
-   <!-- profile  
-    | Specifies a set of introductions to the build process, to be activated using one or more of the  
-    | mechanisms described above. For inheritance purposes, and to activate profiles via <activatedProfiles/>
-    | or the command line, profiles have to have an ID that is unique.  
-    |  
-    | An encouraged best practice for profile identification is to use a consistent naming convention  
-    | for profiles, such as 'env-dev', 'env-test', 'env-production', 'user-jdcasey', 'user-brett', etc.  
-    | This will make it more intuitive to understand what the set of introduced profiles is attempting  
-    | to accomplish, particularly when you only have a list of profile id's for debug.  
-    |  
-    | This profile example uses the JDK version to trigger activation, and provides a JDK-specific repo.-->
- 
- <profile>
- <id>jdk-1.7</id>
- <activation>
- <activeByDefault>true</activeByDefault>
- <jdk>1.7</jdk>
- </activation>
- <properties>
- <maven.compiler.source>1.7</maven.compiler.source>
- <maven.compiler.target>1.7</maven.compiler.target>
- <maven.compiler.compilerVersion>1.7</maven.compiler.compilerVersion>
- </properties>
- </profile>
- </profiles>
中间具体信息的理解, 可以参见 **[冰河winner](http://blog.csdn.net/u012152619/article/details/51485152)** 的博客. 也就是说, 根据上面的配置, 我们需要指定一个符合配置的JDK环境, 这是我们之前在Eclipse-->Window-->preferences-->Java-->Installed JREs下的配置就不行了, 而需要指定一个JDK目录,
 例如我的JDK安装目录下的jdk1.7.0_17, 这也是这个错误出现的罪魁祸首. 不过对于Java开发者来说, Installed JREs中使用jdk目录而不适用jre目录也是最好的选择.
![](https://img-blog.csdn.net/20170628131042280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
步骤:
![](https://img-blog.csdn.net/20170628131221737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628131446355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628131904906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628131756359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628132131929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628133056409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170628133110276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNsazk4OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后再编译运行项目即可.
