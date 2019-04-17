# DevOps之Docker自动化部署 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月06日 21:54:00[boonya](https://me.csdn.net/boonya)阅读数：113标签：[docker																[devops](https://so.csdn.net/so/search/s.do?q=devops&t=blog)](https://so.csdn.net/so/search/s.do?q=docker&t=blog)
个人分类：[Docker																[DevOps](https://blog.csdn.net/boonya/article/category/6251723)](https://blog.csdn.net/boonya/article/category/6559498)







原文地址：[https://blog.csdn.net/wclkkk/article/details/75160435](https://blog.csdn.net/wclkkk/article/details/75160435)

**前言**




软件开发生命周期从瀑布式到敏捷再到现在的DevOps。记得刚入行的时候还是敏捷的天下，在不同公司敏捷的实践中或多或少感受到了团队间合作衔接的效率问题；即使敏捷宣言所倡导是高效，但是实施过程中还是有资源的浪费和瓶颈。DevOps它的精益原则，关注点就是在流程中移除浪费：比如人工部署，系统的频繁中断，PO/SM的一些失败的约定等。今天的主题就是用自动部署代替人工部署以减少资源的浪费。







## 搭建平台前的准备




虚拟机A（Windows server）：JDK, Maven, Tomcat, Jenkins, Bitvise SSH server




虚拟机B（Linux）：JDK, Tomcat, Docker




虚拟机C（Linux）：用Harbor作镜像仓库






注：关键的配置要点会在步骤说明中体现；JDK,tomcat等基础配置不再叙述




## 技术实现思路
- 
利用Jenkins把编译好的代码从SVN拉到虚拟机A上

- 
通过Maven+Tomcat生成War包

- 
使用Bitvise SSH server把War包从虚拟机A拷贝到虚拟机B上

- 
在虚拟机B上将War包通过Dockerfile的方式生成Image

- 
将镜像存入Harbor







## 搭建部署说明



- 
在Jenkins新建一个自由风格的项目

![](https://blog.csdn.net/wclkkk/article/details/75160435)

![](https://img-blog.csdn.net/20170715161200662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- 
进入项目配置，配置源码管理；填写SVN的地址及访问认证

![](https://img-blog.csdn.net/20170715161235161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






- 
构建触发器


           Poll SCM：定时感知代码分支是否有变化，如果有变化的话，执行一次构建

           H/5 * * * *（每5分钟构建一次）

![](https://img-blog.csdn.net/20170715161438779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)













### 构建






- 
**构建步骤一**：添加构建’Invoketop-level Maven targets’

选择配置的

Maven，并填写Maven中要执行的命令 clean package.此步的目的是生成War包

Jenkins项目默认路径在：C:\Documents and Settings\<your username>\.jenkins\workspace

![](https://img-blog.csdn.net/20170715161504080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






- 
**构建步骤二：**添加构建’ExecuteWindows batch command’



这一步的目的是把生成的War包拷贝到指定目录

![](https://img-blog.csdn.net/20170715161619261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




为何要放在指定目录呢？因为要把War包从虚拟机A（windows）拷贝到虚拟机B（linux），而scp只能用于linux间的传输，所以要通过工具。



首先使用pscp工具是可以的，但是它无法做到免密登入（尝试写了个bat脚本，手动执行是通过的，但是在Jenkins里调用就出现了问题，不知原因），附bat脚本

![](https://img-blog.csdn.net/20170715161657717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




所以这里就用到了另一个工具：Bitvise SSH server



要免密登入虚拟机B，我是通过取得公钥的方式进行的：



在虚拟机B中，生成密钥：

ssh-keygen -t rsa

![](https://img-blog.csdn.net/20170715161730656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


默认按回车直至完成



在/root/.ssh/ 路径中找到公钥id_rsa.pub

![](https://img-blog.csdn.net/20170715161745319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这时候在虚拟机A上，通过pscp把公钥拷贝至指定目录 C:\ToSaveWar

（因为有多个不同的项目需要自动化部署，所以将War包和公钥放置在同一目录方便管理）

![](https://img-blog.csdn.net/20170715161759152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




公钥已拷贝至指定目录

![](https://img-blog.csdn.net/20170715161920829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




接下来就要对Bitvise SSH server进行虚拟账户的配置：



打开Easy Setting即可，在step 3中，添加虚拟账号及公钥

![](https://img-blog.csdn.net/20170715161936168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


至此虚拟机A与虚拟机B间的scp传输就走通了

- 
**构建步骤三**：添加构建’Executeshell script on remote host using ssh’

![](https://img-blog.csdn.net/20170715162002222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







SSH site在是虚拟机B的地址，访问认证在Jenkins的系统设置中配置




Scp Jenkins_SSHAccount@10.10.5.XXX:/C:/ToSaveWar/HxdiSpectrum.war/home/dockerfile/hxdispectrum

*将虚拟机A指定目录中拷贝到虚拟机B的指定目录*




cd /home/dockerfile/hxdispectrum

*切换至虚拟机B的指定目录*

![](https://img-blog.csdn.net/20170715162022888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





此目录中包含War包，tomcat, jdk,Dockerfile




Dockerfile配置如下：

![](https://img-blog.csdn.net/20170715162039460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





mv HxdiSpectrum.war ROOT.war

*将War包改名为ROOT.war*




docker build -t$projectName:$imageTagNo .

*生成镜像*




注：镜像名：镜像号已在Jesnkins中参数化掉了）

![](https://img-blog.csdn.net/20170715162132426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





docker tag$projectName:$imageTagNo 10.10.5.33/XXXXXX/$projectName:$imageTagNo

*重命名镜像*




docker login 10.10.5.XX -uwucenlong -p XXXXXXX

登入虚拟机C镜像仓库Harbor-u username -p pwd




docker push 10.10.5.XX/XXXXXXXXXXX/$projectName:$imageTagNo

将镜像Push到镜像仓库






**好了至此所有构建步骤已完成！**









## 查看部署情况




当SVN代码发生变化后，部署开始执行；以下是部署部分log日志




![](https://img-blog.csdn.net/20170715162150656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170715162207126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170715162215896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2Nsa2tr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










