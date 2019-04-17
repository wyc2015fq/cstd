# ecipse中Java项目导入jar包：1、使用maven，2、普通方法 - zp704393004的专栏 - CSDN博客





2018年05月03日 11:23:54[原来未知](https://me.csdn.net/zp704393004)阅读数：871









**壹**使用maven导入jar包




右键项目-Configure-to maven转成maven项目

![](https://img-blog.csdn.net/20170211185227152)


直接完成，也可以安装需求更改参数

![](https://img-blog.csdn.net/20170211185441826)


转换成功后，在项目下找到pom.xml如图。

![](https://img-blog.csdn.net/20170211185537310)


接下来我们在浏览器中访问支持提供maven服务的网站

以下以mvn网为例：https://mvnrepository.com/

直接在搜索中搜索需要的jar包。这里JDBC为例子

![](https://img-blog.csdn.net/20170211185710483)


在搜索出来的jar包中选择需要的jar。

![](https://img-blog.csdn.net/20170211185948542)


按需找到jar包后，建议下载使用热度（usages）比较高的版本，也可以按需选择

![](https://img-blog.csdn.net/20170211190315422)


选择完成，找到如图位置，copy<dependency>标签全部代码（如图红框）

![](https://img-blog.csdn.net/20170211190455925)


回到pom.xml，在</project>标签上面先写进<dependencies></dependencies>标签，再在<dependencies>这里粘贴刚才复制的内容</dependencies>。。。。如图

![](https://img-blog.csdn.net/20170211190805597)


接下来只需要保存以一下xml，就可以在如图目录找到maven下载好的jar包。无需其他配置，项目就可以调用，总得来说优势在于有网站会集合提供jar包服务

![](https://img-blog.csdn.net/20170211191137146)











**贰**使用普通方法

右键项目new一个Floder

![](https://img-blog.csdn.net/20170211191901179)


在如图位置输入lib创建文件夹，Finish完成

![](https://img-blog.csdn.net/20170211191906775)


然后再将你在不可描述的网站上下载好的jar包复制进入lib文件夹。在上面提到的网站

https://mvnrepository.com/

也可以直接下载jar包

![](https://img-blog.csdn.net/20170211191911179)


右键项目，点Properties

![](https://img-blog.csdn.net/20170211191916007)


点java Build Path，再点Add JARs...，不同版本的eclipse会不同，自己理解

![](https://img-blog.csdn.net/20170211191919743)


找到项目目录下的lib再选你的jar包，点ok

![](https://img-blog.csdn.net/20170211191922883)


再应用或者直接ok

![](https://img-blog.csdn.net/20170211191926087)


这样就完成了，你就可以嘿嘿jar包了

[点击打开链接](https://blog.csdn.net/Children_of_Light/article/details/54985869)


