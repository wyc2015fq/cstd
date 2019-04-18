# Maven学习——1、安装与修改Maven的本地仓库路径 - weixin_33985507的博客 - CSDN博客
2017年06月12日 15:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
1.1、下载
官网
[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)
1.2、安装配置
apache-maven-3.3.3-bin.zip
解压下载的压缩包
![](https://images2015.cnblogs.com/blog/710715/201511/710715-20151104145624555-1958976282.jpg)
1.3、配置环境变量
配置环境变量（需要先配置好%JAVA_HOME%环境变量）：
![](https://images2015.cnblogs.com/blog/710715/201511/710715-20151104145848883-1454999132.jpg)
查看是否配置成功：
![](https://images2015.cnblogs.com/blog/710715/201511/710715-20151104150024430-1752219007.jpg)
#  二、本地仓库
 2.1、默认本地仓库在
系统当前用户目录下.m2下
![](https://images2015.cnblogs.com/blog/710715/201511/710715-20151104150725696-1088408198.jpg)
Maven缺省的本地仓库路径为${user.home}/.m2/repository。 
本地仓库是远程仓库的一个缓冲和子集，当你构建Maven项目的时候，首先会从本地仓库查找资源，如果没有，那么Maven会从远程仓库下载到你本地仓库。这样在你下次使用的时候就不需要从远程下载了。如果你所需要的jar包版本在本地仓库没有，而且也不存在于远程仓库，Maven在构建的时候会报错，这种情况可能是有些jar包的新版本没有在Maven仓库中及时更新。 
2.2、修改本地创库
在D盘下新m2\repository文件夹
可以通过修改${user.home}/.m2/settings.xml配置本地仓库路径 
修改Maven安装目录下的 conf/文件夹内的setting.xml文件，新增一行：
```
<localRepository>D:\m2\repository</localRepository>
```
（表示本地仓库的地址为：D:\m2\repository）
将setting.xml复制一份到D:\m2\repository目录下(**这样在Maven安装目录下的 conf/下的setting.xml 跟D:\m2\repository下的setting.xml相同了**)
2.3、修改Eclipse中的maven配置
    1.打开eclipse点击windows->preferences->maven->installations下add一个已经安装到D:\maven\目录下的maven，如：D:\maven\apache-maven-3.3.3，并勾选此新加项。并点击下面的browse按钮打开maven的全局配置文件,如：D:\maven\apache-maven-3.3.3\conf\setting.xml。 
    2.打开eclipse点击windows->preferences->maven->User setting下修改user setting选项为：D:\m2\repository\settings.xml，并点击update settings。并点击下面的reindex按钮更新索引。 
    3.点击配置窗口的apply按钮，突出配置窗口。
原先我的blog这样写，感觉有点问题，最近更新成了上面的样子：  
```
2.3、修改Eclipse中的maven配置
修改MyEclipse中Maven的配置信息。 
    1.打开eclipse点击windows->preferences->maven->installations下add一个已经安装到D:\maven\目录下的maven，如：D:\maven\apache-maven-3.3.3，并勾选此新加项。并点击下面的browse按钮打开maven的全局配置文件,如：D:\maven\apache-maven-3.3.3\conf\setting.xml。 
    2.打开myeclipse点击windows->preferences->maven->User setting下修改user setting选项为：D:\m2\repository\settings.xml，并点击update settings。并点击下面的reindex按钮更新索引。 
    3.点击配置窗口的apply按钮，突出配置窗口。
```
2.4、更新本地仓库
```
最后打开CMD，执行
mvn help:system
会发现Maven从远程库下载的jar包都会放到新修改后的路径
```
![](https://images2015.cnblogs.com/blog/710715/201511/710715-20151104151405430-97715047.jpg)
分类: [Maven](http://www.cnblogs.com/liunanjava/category/751871.html)
