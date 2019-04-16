# Java 学习笔记22：Maven以及其Eclipse插件m2eclipse的安装 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月13日 11:03:24[initphp](https://me.csdn.net/initphp)阅读数：1292
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









# Maven以及其Eclipse插件m2eclipse的安装

（本文参考了Maven实战）

检查JDK的安装以及环境变量的配置

打开cmd

echo %Java_Home%

java -version



如下图



![](http://pic002.cnblogs.com/images/2012/267998/2012010700282628.png)



下载Maven

[http://maven.apache.org/download.html](http://maven.apache.org/download.html)



我这里安装时最新的3.03版本



![](http://pic002.cnblogs.com/images/2012/267998/2012010700284059.png)



安装到本机

1 将下载回来的包解压到任意地址，我的路径是D:\Program Files\apache-maven-3.0.3

2 新建环境变量M2_HOME 指向D:\Program Files\apache-maven-3.0.3

3 在path尾部添加%M2_HOME%\bin

4检查是否安装成功

打开cmd

echo %M2_HOME%

mnv –v



如下图：



![](http://pic002.cnblogs.com/images/2012/267998/2012010700285440.png)



给Eclipse安装maven的插件 m2eclipse

1 打开eclipse

2 Help – Install New Software 出现下图



![](http://pic002.cnblogs.com/images/2012/267998/2012010700290825.png)

3 点击Add



![](http://pic002.cnblogs.com/images/2012/267998/2012010700294352.png)



在Name输入   m2e

在Location输入 [](http://m2eclipse.sonatype.org/sites/m2e)[http://download.eclipse.org/technology/m2e/releases](http://download.eclipse.org/technology/m2e/releases)



4 确定后出现下图



![](http://pic002.cnblogs.com/images/2012/267998/2012010700295417.png)



![](http://pic002.cnblogs.com/images/2012/267998/2012010700302232.png)







5 勾选Maven Integration for Eclipse



6 然后一直下一步直到安装完毕



7 检查是否安装成功

         （1）点击Help – about eclipse – installation details,看是否存在Maven Integration for Eclipse(Required),如下图：



![](http://pic002.cnblogs.com/images/2012/267998/2012010700304956.png)





         （2）再检查eclipse是否可以创建Maven项目了

File – New – Other 



![](http://pic002.cnblogs.com/images/2012/267998/2012010700310677.png)







配置本地仓库的位置

先阅读本文后面注意事项中的(1)



我们程序所需要的jar包等会先去本地仓库找，如果找不到则会去网络上的的中央仓库下载，当然默认本地仓库是放在c盘用户下.m2文件夹，这里我们可以打开.m2的settings.xml进行修改



找到如下部分



![](http://pic002.cnblogs.com/images/2012/267998/2012010700312296.png)

这里我们指定本地仓库位于D盘的MavenRepository文件夹下

配置代理服务器

对于许多没有外网的公司则无法访问apache的中央仓库，则会在局域网内配置一台代理服务器



同样在~/.m2这个文件夹下找到settings.xml下的proxies这个节点

<proxies>

</proxies>

这个节点下面可以配置多台代理

<proxy>

</proxy>

…

<proxy>

</proxy>



如果你声明了多个proxy元素，则默认情况下第一个被激活的proxy会生效



具体配置：

<proxy>

<!-- 唯一标识 -->

      <id>my-proxy</id>

<!-- 指这个代理是否被激活，比如上面提到有多个代理，则会寻找第一个被激活的代理 -->

      <active>true</active>

<!-- 指使用http协议访问 -->

      <protocol>http</protocol>

<!-- 服务器的IP地址 -->

      <host>218.14.227.197</host>

<!-- 访问的端口号 -->

      <port>3128</port>

      <!--

                          <!--  如果服务器需要验证，那么这里就需要配置用户名和密码  -->

      <username>***</username>

      <password>***</password>

<!-- 这里制定那些域名不需要被代理  -->

      <nonProxyHosts>repository.mycom.com|*.google.com</nonProxyHosts>

      -->

    </proxy>



注意事项：



（1）       我们可以copy maven安装目录conf文件夹下的settings.xml到用户目录的~/.m2目录下（例如本机是：C:\Users\chacha\.m2\）下，前者的settings.xml影响到全局范围，而后者只影响到当前的登录用户，推荐使用后者，当然这是看情况而定的。



（2）       关闭eclipse内嵌的Maven



这个内嵌的Maven通常会比较新，但不一定很稳定，而且往往也会和我们在命令行使用的Maven不是同一个版本。这里有会出现两个潜在的问题：首先，较新版本的Maven存在很多不稳定因素，容易造成一些难以理解的问题；其次，除了IDE，我们也经常还会使用命令行的Maven，如果版本不一致，容易造成构建行为的不一致，这是我们所不希望看到的。因此，我们应该在IDE中配置Maven插件时使用与命令行一致的Maven。



解决办法：

Window – Preferences – Maven – Installations – Add 并选择安装目录 点击Apply



![](http://pic002.cnblogs.com/images/2012/267998/2012010700315588.png)





（3）       如果在使用m2eclipse遇到如下提示

Eclipse is running in a JRE, but a JDK is required

Some Maven plugins may not work when importing projects or updating source folders.

这是因为Eclipse默认是运行在JRE上的，而m2eclipse的一些功能要求使用JDK，解决方法是配置Eclipse安装目录的eclipse.ini文件，添加vm配置指向JDK，如下图



![](http://pic002.cnblogs.com/images/2012/267998/2012010700321150.png)







