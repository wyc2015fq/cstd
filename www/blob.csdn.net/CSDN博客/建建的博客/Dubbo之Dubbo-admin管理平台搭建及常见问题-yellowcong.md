# Dubbo之Dubbo-admin管理平台搭建及常见问题-yellowcong - 建建的博客 - CSDN博客
2018年01月23日 10:38:13[纪建](https://me.csdn.net/u013898698)阅读数：100
> 
Dubbo管理平台，处理着权限、路由、规则等，十分的重要 
github上项目地址:[https://github.com/yellowcong/dubbo](https://github.com/yellowcong/dubbo)
码云上项目地址:[https://gitee.com/yellowcong/dubbo](https://gitee.com/yellowcong/dubbo)
由于我的网，访问github比较的费劲，所以我把它导入到了码云上了，Dubbo在配置过程中，容易发生的问题有，1、Tomcat 堆内存小，启动不起来 2、Zookeeper所有节点没有 全启动
## 编译Dubbo admin のWar包
1、将码云上的dubbo克隆到本地机器下，你可以只clone 一个 dubbo admin这个项目
![这里写图片描述](https://img-blog.csdn.net/20171015111455532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、导入项目到eclipse
![这里写图片描述](https://img-blog.csdn.net/20171015111627822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们之需要dubbo-admin这个项目
![这里写图片描述](https://img-blog.csdn.net/20171015111708236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
等待依赖的下载
![这里写图片描述](https://img-blog.csdn.net/20171015111832850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编译完事了，但是会有bug,可以看到是META-INF 包下面的问题，就不用管他了
![这里写图片描述](https://img-blog.csdn.net/20171015112103029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、编译dubbo admin文件
![这里写图片描述](https://img-blog.csdn.net/20171015112258073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编译成功
![这里写图片描述](https://img-blog.csdn.net/20171015112337132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个时候，可以看到目录有我们想要的war包，记住需要 F5刷新一下，才可以看到
![这里写图片描述](https://img-blog.csdn.net/20171015112426814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 搭建Dubbo admin
这个地方，我直接把安装包给大家，我编译好的，下面是下载地址：
```
#码云下载地址
https://gitee.com/yellowcong/dubbo/tree/master/dubbo-admin
#七牛云下载地址
http://yellowcong.qiniudn.com/dubbo-admin-2.5.6.war
```
- 1
- 2
- 3
- 4
- 5
![这里写图片描述](https://img-blog.csdn.net/20171015113850688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1、需要安装 tomcat 
2、unzip 如果没有需要安装
```bash
yum install unzip
#解压到 tomcatのwebapp目录下
unzip dubbo-admin-2.5.6.war -d ./dubbo-admin
```
- 1
- 2
- 3
- 4
![这里写图片描述](https://img-blog.csdn.net/20171015114923850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、配置zookeeper到dubbo 
zookeeper配置，可以查看我的文章[Zookeeper的安装配置和简单使用](http://blog.csdn.net/yelllowcong/article/details/76849946)
我们需要修改WEB-INF目录下的 dubbo.properties 配置文件，对于多台zookeeper服务器，需要加上?backup= ，dubbo这事基于这种方式，和以前Curator和Zookeeper客户端配置有所不同
![这里写图片描述](https://img-blog.csdn.net/20171015115129438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
#对于多台zookeeper服务器，需要加上?backup= ，dubbo这事基于这种方式，和以前Curator和Zookeeper客户端配置有所不同
dubbo.registry.address=zookeeper://127.0.0.1:2181?backup=127.0.0.1:2182,127.0.0.1:2183
#管理员密码
dubbo.admin.root.password=root
#访客 密码
dubbo.admin.guest.password=root
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
4、日志查看
```bash
#查看日志最新200行
tail -f -n 200  ./logs/catalina.out
```
- 1
- 2
- 3
5、开启端口
```
#开启端口
iptables -I INPUT -p tcp -m tcp --dport 8080 -m state --state NEW,ESTABLISHED -j ACCEPT
#保存配置
service iptables save
#重启生效
service iptables restart
#查看规则链
iptables -L -n
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
## 登录dubbo-admin
这个可以看到，浏览器的访问地址，输入用户名和密码
![这里写图片描述](https://img-blog.csdn.net/20171015123528331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
登录成功后，可以看到我们想要的界面了
![这里写图片描述](https://img-blog.csdn.net/20171015123616708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 问题集合
### Tomcat内存不足的问题
更改 catalina.sh的配置，设置JAVA_OPTS 这个参数，来修改初始化内存大小，Tomcat默认可以使用的内存为128MB，在较大型的应用项目中，这点内存是不够的，有可能导致系统无法运行。常见的问题是报Tomcat内存溢出错误，Out of Memory(系统内存不足)的异常，从而导致客户端显示500错误，一般调整Tomcat的使用内存即可解决此问题。
```
#设置 Xms 和Xmx 堆内存大小
JAVA_OPTS='-Xms512m -Xmx1024m'
```
- 1
- 2
![这里写图片描述](https://img-blog.csdn.net/20171015125927983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### Could not load java.util.concurrent.ScheduledFuture
不能初始化调度组件
![这里写图片描述](https://img-blog.csdn.net/20171015131647385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
查看Zookeeper的状态，发现是zookeeper有一台挂掉了 
![这里写图片描述](https://img-blog.csdn.net/20171015131927920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
完全启动所有的zookeeper后 
![这里写图片描述](https://img-blog.csdn.net/20171015132825992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
tomcat启动成功，访问服务器正常 
![这里写图片描述](https://img-blog.csdn.net/20171015132717368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWVsbGxvd2Nvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
