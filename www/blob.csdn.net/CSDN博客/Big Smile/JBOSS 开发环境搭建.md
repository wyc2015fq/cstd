# JBOSS 开发环境搭建 - Big Smile - CSDN博客
2017年03月12日 21:21:28[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2086
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
# 【前言】
JBOSS的定义：是一个基于J2EE的开放源代码的应用服务器。JBoss代码遵循LGPL许可，可以在任何商业应用中免费使用，而不用支付费用。JBoss是一个管理EJB的容器和服务器，支持EJB 1.1、EJB 2.0和EJB3的规范。但JBoss核心服务不包括支持servlet/JSP的WEB容器，一般与Tomcat或Jetty绑定使用。
JBOSS Developer Studio下载地址：
[https://developers.redhat.com/products/devstudio/download/](https://developers.redhat.com/products/devstudio/download/)
下载需要在RED HAT上注册并登陆，如果下载速度过慢，可以考虑使用代理或者翻墙服务器。
我们为什么要使用JBOSS Developer Studio？
JBOSS Developer Studio 是RED HAT JBOSS 官方指定的开发工具集合和IDE，我们使用它是最快、最兼容的开发工具，无需他求。
# 【工具】
**1、环境**
最新的JBOSS Developer Studio 需要JDK1.8 以上的JDK环境。
需要maven库环境的支持
**2、安装包**
devstudio-10.2.0.GA-installer-eap.jar
注意如果是初次使用这个开发工具，需要下载携带eap版本的开发环境以便调试程序。
# 【环境搭建】
## 一、JDK的下载和安装
详细过程参见百度教程，此文不做解释
## 二、maven的安装
首先需要在maven官网下载安装包：[https://maven.apache.org/download.cgi](https://maven.apache.org/download.cgi)
下载对应系统版本的安装包。
**1）Windows下的配置**
下载好了一个maven的zip包后解压到系统任意目录（纯英文目录），然后把压缩包进行解压缩。
首先我们需要设置环境变量，找到系统环境变量设置：
![](https://img-blog.csdn.net/20170312211558500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在系统变量中添加我们刚解压完的maven文件的目录为MAVEN_HOME：
![](https://img-blog.csdn.net/20170312211607031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中变量值就是maven解压之后的目录。然后我们要在系统变量Path中添加maven的bin路径：%MAVEN_HOME%\bin，这里直接用的maven_home的值。
这样maven的环境变量就配置好了，然后我们win+r输入cmd打开命令提示符，输入mvn -v，如果有如下显示，代表我们的maven环境配置成功了。
![](https://img-blog.csdn.net/20170312211614956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**配置本地仓库：**
由于我们要把maven当做一个jar包的仓库用，所以可能会下载很多的jar包到电脑上，maven默认配置仓库为“users/你的用户名/.m2”下，所以如果你的C盘不是很宽裕的话，可以提前更改仓库的配置，具体如下：
首先到解压maven的目录下进入maven文件夹下的conf文件夹，我们会看到一个settings.xml文件，这个就是maven的一个配置文件。
然后我们需要找到<localRepository>/path/to/local/repo</localRepository>并把它解除注释状态就行了，然后我们在这个标签中的路径替换为我们想要建库的路径即可，注意这里仓库所指向的文件夹必须存在，否则会出错。
**2）Linux下的配置**
还是要先得到在Linux下的安装包apache-maven-3.3.9-bin.tar.gz然后在Linux下进行操作。
首先用tar –xzvf 文件路径+文件名.tar.gz 解压已经下载好的包，这里注意当前执行命令的地方就是要解压到的地方，所以需要提前用cd命令来转移到需要解压到的地方。
例如，我们的压缩文件放在了/home/user/下面，而我们需要安装在/usr/local 下面，我们就需要先执行：
cd /usr/local 
然后执行tar –xzvf /home/user/apache-maven-3.3.9-bin.tar.gz 
然后需要在系统中设置环境变量，执行vi命令（需要有root权限）：
vi /etc/profile
然后在文件的最后面添加：
MAVEN_HOME=/usr/local/apache-maven-3.3.9 #这里为maven的解压地址
PATH=${PATH}:${MAVEN_HOME}/bin
export MAVEN_HOME
export PATH
之后调出终端，输入 mvn –v 检测，如果出现版本信息，则说明可以正常使用。
![](https://img-blog.csdn.net/20170312211621844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Linux下的配置仓库的文件在maven目录下conf/settings.xlm 文件中，具体配置方法和Windows的一样，这里不再赘述。
# 【JBOSS Developer Studio 安装】
其实这个工具的安装算是比较简单的了，因为他就相当于是一个eclipse的带插件的环境，官网下载的安装包直接安装就好。
Windows下直接双击安装这个jar包，会出现：
![](https://img-blog.csdn.net/20170312211631063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后一直next下去，选择安装路径（最好纯英文路径），然后就会自动安装配置了，eap也会安装好。就可以直接使用了。
Linux下的安装也不难，直接执行Java -jar 路径/devstudio-10.2.0.GA-installer-eap.jar.jar
然后的安装方法就和在Windows上一样了。
# 【eclipse的配置】
## 一、代理的配置
由于jboss需要有官网的支持以及maven也需要从网络上同步所以需要进行翻墙的操作
代理的配置说白了就是对于网络限制的一种挑战，虽然速度不快，不过可以勉强下载。（已经翻墙的同志请略过这一步）。
首先我们进入到IDE的window菜单，找到Preferences这个项，点击进入，找到General/Network Connections：
![](https://img-blog.csdn.net/20170312211637519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在下面Proxy bypass添加一个host
![](https://img-blog.csdn.net/20170312211644047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
写入一个代理IP地址，这个IP地址可以从百度上找，我这里用我已经找好的一个1.82.216.135:80
![](https://img-blog.csdn.net/20170312211651531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击OK之后就可以应用了。
## 二、maven本地库的配置
还是在IDE 中进入菜单window/Preferences，找到maven下的user setting。
![](https://img-blog.csdn.net/20170312211658235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击user settings栏上的browse，然后选择配置本地maven的配置文件：
![](https://img-blog.csdn.net/20170312211705379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170312211714938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后IDE就会自动找到配置文件中的仓库地点，然后会自动同步已有项目中的jar包。到此IDE的配置就结束了。
