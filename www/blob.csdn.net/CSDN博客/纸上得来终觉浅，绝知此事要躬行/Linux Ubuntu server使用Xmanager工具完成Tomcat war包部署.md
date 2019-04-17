# Linux Ubuntu server使用Xmanager工具完成Tomcat war包部署 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年08月27日 14:11:53[boonya](https://me.csdn.net/boonya)阅读数：1959标签：[linux																[ubuntu																[tomcat																[xmanager																[Xshell](https://so.csdn.net/so/search/s.do?q=Xshell&t=blog)
个人分类：[Linux](https://blog.csdn.net/boonya/article/category/1740759)





前言：一直以为Linux部署项目很神奇，但是一直没有机会去尝试，正好最近项目比较闲，有时间研究入门一下，这篇文字所讲的是在Ubuntu中安装好Tomcat后怎样部署web项目，如题中所述的war包，Tomcat安装可以参考我的另一篇文章：[http://blog.csdn.net/boonya/article/details/47981047](http://blog.csdn.net/boonya/article/details/47981047)

**Linux基本环境配置说明：**Ubuntu使用的是Ubuntu server15.0.4，tomcat使用的是v7.0.63，jdk:1.7.55


### 1.Ubuntu中安装ssh

#### 1.1 命令行安装ssh


`$ sudo  apt-get install ssh`


#### 1.2 修改ssh加密方式

如果Ubuntu系统中没有安装ssh,那么是不能通过远程方式连接的。若用户重新执行了如上的命令，需要再进行设置，否则连接的时候提示：



**xftp找不到匹配的outgoing encryption 算法**

此时需要再连接属性页面设置一下：

![](https://img-blog.csdn.net/20150827135903266?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


因为ssh的加密方式就新增了三个，需要将这三个全部勾上并保存，如下图所示：

![](https://img-blog.csdn.net/20150827135621215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 2.使用Xmanager Xftp传递文件

#### 2.1. Xmanger 包含的主要功能

下图中有本章内容所讲的Xftp和Xshell操作图标，只需要双击图标即可运行；


![](https://img-blog.csdn.net/20150827133018262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 2.2  Xmanager Xftp方式设置

首先，新建一个连接：

![](https://img-blog.csdn.net/20150827133606648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

按照如下图设置：

![](https://img-blog.csdn.net/20150827134117883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点确定按钮，不出意外就可以进去了；

#### 2.3Windows桌面war包拷贝到Ubuntu Tomcat webapps下

文件拷贝的时候，可以用拖拽的形式将指定文件拖动至相应的路径下，如下图：

![](https://img-blog.csdn.net/20150827135058629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

至此，文件拷贝完成。

### 3.使用Xmanager Xshell 远程操作Linux

#### 3.1 新建连接

![](https://img-blog.csdn.net/20150827140221517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 3.2设置新连接

![](https://img-blog.csdn.net/20150827140337098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 3.3Xshell窗口测试连接

![](https://img-blog.csdn.net/20150827140425244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 4.测试项目部署情况

#### 1.Tomcat的启动和停止

启动和停止在前面的Tomcat安装中已经说过了，这里不再赘述；

验证Tomcat是否启动，在浏览器输入Ubuntu虚拟机的IP和8080端口进行测试，看到如下图所示则表示Tomcat启动成功：

![](https://img-blog.csdn.net/20150827140842643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 4.2 浏览器测试项目访问

![](https://img-blog.csdn.net/20150827140924381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

至此大功告成！](https://so.csdn.net/so/search/s.do?q=xmanager&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




