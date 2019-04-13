
# Eclipse+maven+scala+spark环境搭建 - anlcy - 博客园






# [Eclipse+maven+scala+spark环境搭建](https://www.cnblogs.com/camilla/p/8275444.html)

### 准备条件
> 我用的Eclipse版本

> Eclipse Java EE IDE for Web Developers.

> Version: Luna Release (4.4.0)
我用的是Eclipse for Java EE， 这个和Eclipse 是一样的
最好是用高版本的Eclipse,版本太低的话不支持Scala-IDE
## 1.安装Scala-IDE
在Eclipse中开发Scala程序需要有scala插件，我们现在安装scala插件
下面是安装过程截图
Scala IDE用于提供scala语言的开发环境。直接在Marketplace下载安装，安装后如下图所示
![](http://img.blog.csdn.net/20160809182124455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2.安装m2e-scala插件
m2e-scala用来支持scala开发中对maven的一些定制功能。通过eclipse的Install New Software安装。
安装过程
1.Help->Install New Software
![](http://img.blog.csdn.net/20160809182157221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.输入m2e-scala下载的url
这一步我是跳过的，之前每次进行完这一步，我的Eclipse的maven插件反而找不到了，这个Eclipse自带的是
m2e-eclipse插件，每次一安装m2e-scala插件后就找不到maven了，索性我就没进行这一步，用Eclipse自带的 m2e-eclipse.
如下图
![](http://img.blog.csdn.net/20160809182225758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
url为：[http://alchim31.free.fr/m2e-scala/update-site/](http://alchim31.free.fr/m2e-scala/update-site/)
这里可以仅勾选第三项（图中红框所示）
![](http://img.blog.csdn.net/20160809182318255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.安装完成后，可在Help->Installation Details中查看
![](http://img.blog.csdn.net/20160809182349712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3.新建Eclipse+scala+maven工程
新建maven工程
![](http://img.blog.csdn.net/20160809182416131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时的maven的Archetype需要设置为 org.scala-tools.archetypes
如果没有安装Scala-IDE的话，会找不到org.scala-tools.archetypes这个类别
然后Finish
![](http://img.blog.csdn.net/20160809182441946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时创建的工程还不是scala的工程，在项目名称上点击右键
![](http://img.blog.csdn.net/20160809182510164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击 Add Scala Nature后项目才转换为scala-maven项目
然后pom.xml报下图中的错误，解决办法
在 plugins 的外层加标签
![](http://img.blog.csdn.net/20160809182545197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方法：
![](http://img.blog.csdn.net/20160809182608291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时又报错
![](http://img.blog.csdn.net/20160809182649119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
意思是我有多个scala-library库的构建路径
一个是scala-IDE插件带的，一个是maven库中的
并且maven中自动设置的是scala版本是2.7.0的
![](http://img.blog.csdn.net/20160809182719729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方法：
把pox.xml中的scala-version改成和scala-IDE的版本一样
这时候还是提示有多个scala-library的构建路径，但是不报错了
![](http://img.blog.csdn.net/20160809182755229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们用一个构建路径，把其余的一个去掉
这里我把pom.xml中的scala-library 注释掉，只用scala-IDE的库
![](http://img.blog.csdn.net/20160809182822323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后把自带的‘App.scala中的代码删除
这样maven+scala的项目就成功搭建了。
## 4. Eclipse+maven+scala+spark
在pom.xml中加入spark的依赖
我的spark的版本是
![](http://img.blog.csdn.net/20160809182845902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个版本的依赖
`<dependency>`
`<groupId>org.apache.spark</groupId>`
`<artifactId>spark-core_2.11</artifactId>`
`<version>1.2.1</version>`
`</dependency>`
我刚开始用的是 spark-core_2.10 ,结果报错，后来换成spark-core_2.11才合适，2.11是对应的scala版本
然后一阵狂下在spark的依赖，最后是这个样子滴
![](http://img.blog.csdn.net/20160809182920700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
终于把scala+maven+spark的环境搭建成功了，之前被maven那一步折腾的快疯了，最后索性没使用maven-for-scalaIDE





