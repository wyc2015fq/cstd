# 如何在linux中用tomcat服务器，（安装与配置 Java 版） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月30日 00:18:54[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：74


下载 ： JDK  和 tomcat     （linux 版） 去官网下，别自己瞎找 地方

然后 进入虚拟机先 配置 网络

网络配置

1、找到网卡的配置文件

vi /etc/sysconfig/network-scripts/ifcfg-eth0

文件内容：

ONBOOT=yes

BOOTPROTO  

dhcp 代表自动分配一个IP地址

static 代表手动分配一个IP地址

如果是手动分配 需要配置IP地址

IPADDR=要分配的IP地址

NETMASK=子网掩码

GATEWAY=网关地址

2、网络配置完成后必须要重启服务

service 代表对服务进行操作

network 代表要对那个服务进行操作

restart   代表进行什么样的操作

service network restart

3、启动防火墙

service iptable starts

4、找到DNS的配置文件

/etc/resolv.conf

nameserver DNS地址

      检验 ping 你配的DNS地址   比如 ping www.baidu.com  能获取到就说明配置 成功 ctrl+c 退出

安装JDK

      1、把下载好的JDK 和 tomcat 放入 linux 中   ---用Xshell5  放

       我在  cd /  下 创建了 soft 文件夹， 全部放这里了。  

      2、 解压     tar -zxvf 文件名

      3、配置环境变量  

      在     /etc/profile.d   下创建一个脚本

cd /etc/profile.d

vi java.sh

编写的内容  

JAVA_HOME=解压目录

PATH=$PATH:$JAVA_HOME/bin

CLASSPATH=$CLASSPATH:$JAVA_HOME/lib

export JAVA_HOME PATH CLASSPATH               ---从局部变量转换成全局变量

保存并退出

     4、 执行脚本

source java.sh

     5、 测试 

java -version

 安装tomcat

    1、跟JDK 一样，我都放在了 cd /soft 文件夹下

    2、解压

    3、/soft/tomcat/conf   进入tomcat的conf 文件夹 打开tomcat-user.xml   最下面 添加三行配置文件

    <role rolename="manager-gui"/>
    <role rolename="manager-script"/>
    <user username="用户名" password="密码" roles="manager-gui, manager-script"/>

![](https://img-blog.csdn.net/20180330001101774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、/soft/tomcat/conf     下 server.xml 可以改 端口号      --不改也行

![](https://img-blog.csdn.net/20180329235525132?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、/soft/tomcat/conf     下 web.xml  中  把readonly 这个设置为false 默认是true

![](https://img-blog.csdn.net/2018032923571286?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6、进入 tomcat 下的 bin 文件夹 执行 ./startup.sh  命令 ，启动tomcat服务器

eclipse 中的配置

创建一个maven 项目 。 在webapp/ 下创建 WEB-INF/web.xml

打开pom.xml  然后右键   maven-AddPlugin

![](https://img-blog.csdn.net/20180330000631970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

<groupId>org.apache.tomcat.maven</groupId>
<artifactId>tomcat7-maven-plugin</artifactId>

<version>2.2</version>

把这 三行 对应的 输入上去，然后点 OK

然后就 会 自动在 pom中 自动生成 一些配置信息

然后 在 生成的 配置信息中的 version标签的 下面

加入 以下内容

<configuration>
<!-- 发布地址 -->
<url>http://linux的ip地址/manager/text</url> 
<port>80</port>
<username>用户名</username>
<password>密码</password>

</configuration>

ctrl+s 保存  。

project-clean 再 右键项目-maven-update project..   再run as - install  

再maven build中 输入命令   tomcat7:deploy (第一次部署)

tomcat7:redeploy(更新部署 )

然后 就是 通过 输入ip地址， 看看能不能进 tomcat 官网了。 进入官网以后 右上角 有个 Manger App 输入 之前自己在tomcat-user.xml  配置的用户名 密码，进去以后看一下 自己的项目是否部署进去， 部署进去就成功了

