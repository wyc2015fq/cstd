# Jenkins自动化构建(一)执行selenium+python脚本 - _天枢 - 博客园
## [Jenkins自动化构建(一)执行selenium+python脚本](https://www.cnblogs.com/yhleng/p/7591467.html)
2017-09-25 13:33 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7591467)
Jenkins执行python写的selenium自动化脚本，通常会遇到，执行打不开浏览器，查看jenkins构建**[Console Output](http://127.0.0.1:8080/job/phj_project/2/console)控制台输出信息，发现脚本是执行了的，但是出错了，打开浏览器出现问题，原因呢，是因为我们window安装的jenkins默认会产生一个window服务；那怎么解决这个问题呢。**
**1.先在控制面板－－>管理工具－－>服务；中找到Jenkins服务，－－>属性，停止和禁用该服务。**
**有人会说了，你把服务停止了jenkins怎么用啊，别急，听笔者一一道来。**
**2、通过jenkins目录下的jenkins.war去手动启动服务**
**需要注意的是，手动启动这个服务jenkins需要重新配置，也就是说，你之前如果在windows服务下做了jenkins配置会丢失。包括设置的帐户密码。**
**一切需要重新配置。**
**以下命令启动jenkins.war，可以复制下来保存成.bat格式批处理。每次用jenkins手动去点击这个bat**
```
echo "Jenkins CI automation testting"
java -jar "C:\Program Files (x86)\Jenkins\jenkins.war"
pause
```
jenkins管理员的一些配置就不说了。如果你初始密码不知道，请仔细看页面，有一个地址提示，用文本文件打开后找到密文，那就是密码。
3、jenkins配置
系统管理－－>系统配置
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133258778-883384770.png)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133344684-1131439962.png)
 配置这两个就可以了，其它看你需要吧。
系统管理－－>管理插件
[Git plugin](http://wiki.jenkins-ci.org/display/JENKINS/Git+Plugin)  安装这个就可以了。我们只是用来简单的构建执行。
新建job
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133542715-1538659220.png)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133616028-382704740.png)
在进入的页面，选择源代码管理
输入Git项目地址
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133713684-286674185.png)
构建触发器，这是用来设置执行的
*/5 * * * *这代表5分钟执行一次，注意星数字，空格
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133819919-1858479204.png)
构建环境 选择执行windows批处理，因为是py脚本，可以直接双击执行，linux下可以选择shell
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927133925090-1410179217.png)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927134139325-914131916.png)
点构建，试一下，能不能执行我们的脚本。
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170927134226590-58837080.png)
我们看到，打开ie执行了，没有问题。
