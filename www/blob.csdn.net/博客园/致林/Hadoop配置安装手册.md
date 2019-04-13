
# Hadoop配置安装手册 - 致林 - 博客园






# [Hadoop配置安装手册](https://www.cnblogs.com/bincoding/p/5545477.html)
本次Hadoop集群安装一共使用四个节点，各节点IP如下：
|Master
|172.22.120.191
|
|Slave1
|172.22.120.192
|
|Slave2
|172.22.120.193
|
|Slave3
|172.22.120.193
|
系统版本CentOS 6.2L
JDK版本：1.7
Hadoop版本：1.1.2
完成四个节点系统安装后，开始安装hadoop：
**1.****配置hosts****文件**
**2.****配置SSH****免密码登录**
**3.****安装JDK**
**4.****下载并解压hadoop**
**5.****配置namenode****，修改site****文件**
**6.****配置hadoop-env.sh****文件**
**7.****配置masters****和Slaves****文件**
**8.****向各节点复制hadoop**
**9.****格式化namenode**
**10.****启动hadoop**
**11.****Jps****检测是否启动成功**
**12.****通过网站查看集群情况**
**1****配置hosts****文件**
Hadoop会使用多个端口，开始配置前最好开放相应端口或者关闭防火墙
CentOS命令为:
chkconfig iptables off
service iptables stop
为了方便操作，我们直接配置hosts文件，以后可以直接使用master，slave来代替IP
![clip_image002[6]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123309946-1362320125.jpg)
![clip_image004[6]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123311305-959443110.jpg)
**2****配置SSH****免密码登录**
因为hadoop需要用master节点管理其它节点，需要将master的public key发送给其他节点，使用SSH可以减少很多操作。
返回hadoop账号，创建hadoop用户的密钥：
![clip_image006[6]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123312836-655211854.jpg)
生成密钥后，将public key更名为authorized_keys文件，并测试是否可以免密码登录：
![clip_image008[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123314149-1382697833.jpg)
输入命令: ssh master，检测是否需要密码，第一次会提示是否保存密钥，输入yes即可，在其他三个节点中执行相同的操作。
最后将master节点的 id_rsa.pub的内容追加到其他三个节点的authorized_keys中。
![clip_image010[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123315633-943802253.jpg)
如果成功，则可以直接使用 ssh slave 连接其他节点。
![clip_image012[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123316836-1255346510.jpg)
**3****安装JDK**
先查看自带jdk，这里是openJDK，我们需要卸载然后重新安装JDK
![clip_image014[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123317961-2069076659.jpg)
查看openJDK依赖包，依次卸载
![clip_image016[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123320899-1589442199.jpg)
![clip_image017[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123322602-1902244818.png)
将jdk文件复制到master节点 /usr/local 目录下并解压。
修改 /etc/profile 文件，添加Jdk目录
![clip_image019[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123324180-2117416050.jpg)
输入 source /etc/profile 使配置生效
![clip_image021[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123325274-1466081325.jpg)
最后查看java版本
![clip_image023[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123326352-618591967.jpg)
尽量保持各节点jdk版本和路径一直，方便后续安装
**4****下载并解压hadoop**
![clip_image025[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123327867-515549914.jpg)
修改 /etc/profile文件，添加hadoop路径
![clip_image027[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123328992-1122090229.jpg)
最后使 profile文件生效，输入 source /etc/profile
![clip_image028[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123330102-1776925642.jpg)
**5****配置namenode****，修改site****文件**
![clip_image029[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123331914-303398793.png)
**6****配置hadoop-env.sh****文件**
![clip_image031[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123334055-814771821.jpg)
**7****配置masters****和Slaves****文件**
![clip_image033[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123334930-1741433299.jpg)
**8****向各节点复制hadoop**
![clip_image035[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123335992-2072907688.jpg)
对其他节点执行相同操作，将Hadoop文件发送到节点
**9****格式化namenode**
在第一次启动时格式化namenode，多次格式化可能会使namenode和datanodeID不一致
![clip_image036[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123336852-751046962.png)
**10****启动hadoop**
![clip_image037[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123337821-2086098670.png)
**11****Jps****检测是否启动成功**
![clip_image038[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123338821-279695716.png)
**12****通过网站查看集群情况**
![clip_image040[4]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123340164-1003071640.jpg)
![clip_image042[5]](https://images2015.cnblogs.com/blog/771778/201605/771778-20160531123341571-1347946993.jpg)





