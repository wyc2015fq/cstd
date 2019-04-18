# WebLogic集群案例分析 - weixin_33985507的博客 - CSDN博客
2013年12月05日 12:42:14[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**WebLogic集群案例分析**
2013年8月，某证券交易系统（采用Weblogic中间件），由于基金业务火爆，使系统压力太大，后台服务器频繁死机时，这时工程师们紧急调试系统及恢复操作，等完成这些操作花掉了1天的时间，前端的所有任务都不得不停止，以等待系统恢复最终造成证券交易系统一整天不能服务的重大事故，从以上情形可以看出提高证券交易系统的高可用性是多么重要，这直接关系到到证券投资业务的正常开展。下文为大家介绍如何搭建Weblogic集群高可用案例，其主要介绍如何配置WebLogic集群，拓扑如图1所示。
![clip_image002[7]](http://chenguang.blog.51cto.com/attachment/201312/5/350944_13862184889OED.jpg)
图1 WebLogic集群
硬件: IBM 3950M2 +  DS4700,(服务器和存储都采用SAS盘)
HBA卡: QLogic 4Gb FC Single-Port PCIe HBA
**1.RHEL5.4****操作系统的安装注意事项**
RHEL5.4作为当前比较稳定的linux服务器版本，将其作为这次改造的底层操作系统
操作系统安装注意事项：注意采用自动分区,自动分区是LVM方式,手动划分磁盘分区容易造成找不到分区的情况；切忌在服务器装好后,不要使用fdisk进行分区调整，否则会出严重错误；安装Desktop Environments依然选择安装Gnome桌面环境；Servers选项中将右侧所有的服务全部去掉；在Virtualization中，不要勾选虚拟技术；另外防火墙设置成关闭状态；关闭SELinux强制模式
**2.FC****（****Fibre Channel****）连接方式配置**
服务器安装FC HBA卡，通过FC线连接到盘柜上的FC接口(接口上必须安装短波光模块）。也就是 主机和磁盘阵列通过光纤交换机连接，速率可达10Gb/S
1. 多路径软件配置在光盘下安装以下两个软件包
l device-mapper-1.02.13-6.14.i586.rpm 该软件包负责设备虚拟化和映射
l multipath-tools-0.4.7-34.38.i586.rpm 该软件包负责路径状态检测及管理
待这两个包安装完成后使用modprobe dm_multipath加载相应模块,然后使用lsmod |grep dm_multipath检查安装是否成功.
2.利用模板创建multipath.conf的配置文件
#cp /usr/share/doc/packages/multipath-tools/multipath.conf.synthetic /etc/multipath.conf
然后再该文件的最后一段加上如下配置：
devices {
device {
vendor "EMC" //厂商名称
product "CaXXXXX" //产品型号
path_grouping_policy group_by_prio //默认的路径组策略
getuid_callout "/sbin/scsi_id -p 0x80 -g -u -s /block/%n" //获得唯一设备号使用的默认程序
prio_callout "/sbin/acs_prio_alua %d" //获取有限级数值使用的默认程序
hardware_handler "1 acs" //确认用来在路径切换和IO错误时，执行特定的操作的模块。
path_checker hp_sw //决定路径状态的方法
path_selector "round-robin 0" //选择那条路径进行下一个IO操作的方法
failback immediate //故障恢复的模式
no_path_retry queue //在disable queue之前系统尝试使用失效路径的次数的数值
rr_min_io 100 //在当前的用户组中，在切换到另外一条路径之前的IO请求的数目
}
}
编辑modprobe.conf文件，用来设置驱动程序的参数。
/etc/modprobe.conf.local
增加一行 "options scsi_mod dev_flags=Accusys:ACS92102:0x1000"
重新编译引导文件
"cp -f /boot/initrd-2.6.[kernel_version].img ./initrd-2.6.[k_v].img.bak"
"mkinitrd"
做完上述操作后，注意要将操作系统重新启动
**3.Java****环境的配置安装**
Java在Linux系统安装的过程中可以默认安装，但是为了保证java的版本与我们运行程序之间进行统一，在安装系统的时候，没有安装系统默认携带的java版本，需要我们安装自定义版本的java，首先我们到http://www.oracle.com/，下载JDK1.6.21版本，选择jdk-6u21-linux-i586-rpm.bin下载到本地/home。然后将其安装到/usr/local/java目录下
# chmod 777 jdk-6u22-linux-i586.bin
# ./jdk-6u22-linux-i586.bin
# mkdir –p /usr/local/java
# cp -Rf ./jdk1.6.0_22 /usr/local/java
# chmod –R 777 /usr/local/java
**4.设置环境变量**
我们首先需要修改配置文件
#vi /etc/profile
在文件的末尾加入如下的信息
export JAVA_HOME=/usr/local/java/jdk1.6.0_22
export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin
保存退出后，在控制台执行复制的代码
# export JAVA_HOME=/usr/local/java/jdk1.6.0_22
# export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin
验证：
#echo $JAVA_HOME
#java –version
java version "1.6.0_22"
Java(TM) SE Runtime Environment (build 1.6.0_22-b04)
Java HotSpot(TM) Client VM (build 17.1-b03, mixed mode)
5.Oracle11g安装部署和RAC设置参见《Linux企业应用案例精解》一书
**6. Weblogic11安装部署**
因为它的安装过程比较简单下面附上一些安装过程的关键截图。
[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218490B1MW.jpg)[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218491ewCE.jpg)![205124376.jpg](http://blog.51cto.com/attachment/201312/205124376.jpg)[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218494KEGi.jpg)![205024530.jpg](http://blog.51cto.com/attachment/201312/205024530.jpg)[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218496O8Ee.jpg)![204952284.jpg](http://blog.51cto.com/attachment/201312/204952284.jpg)[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218498u3Wm.jpg)![204910502.jpg](http://blog.51cto.com/attachment/201312/204910502.jpg)
**安装完毕后我们开始创建服务**
1. 创建新的域
#cd /root/Oracle/Middleware/wlserver_10.3/common/bin
[root@localhost bin]# ./config.sh
[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218500njRz.jpg)![204828793.jpg](http://blog.51cto.com/attachment/201312/204828793.jpg)
单击下一步
选择域源,在产品列表里我们选择第一个默认选项Basic WebLogic Server Domain-10.3.4.0,单击下一步.
[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218502jL9w.jpg)![204756816.jpg](http://blog.51cto.com/attachment/201312/204756816.jpg)
指定域名和位置,我们在这里输入cluster_domain,域的位置在/root/Oracle/Middleware/user_prejects/domain,然后下一步
**7.配置管理员用户和密码**
在此以weblogic作为登陆服务器的用户名,密码为"welcome1",单击下一步
[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218503jxWr.jpg)![204718993.jpg](http://blog.51cto.com/attachment/201312/204718993.jpg)
**8.配置服务器启动模式和JDK**
由于是生产环境下部署,建议在WebLogic域启动模式中选择生产模式,同时JDK选择JRockit SDK 1.6.0_24@/root/Oracle/Middleware/jrockit_160_24_D1.1.2-4,然后单击下一步.
在可选配置里选择"管理服务器"和受管理服务器,集群和计算机"两个选项.单击下一步
**配置管理服务器**
在这里,监听地址就需要选择192.168.0.4,而不要选择本机环路地址,并启用SSL 加密.
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_13862185049GFN.jpg)![204632634.jpg](http://blog.51cto.com/attachment/201312/204632634.jpg)**
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218507vFgK.jpg)![204554760.jpg](http://blog.51cto.com/attachment/201312/204554760.jpg)**
配置需要创建的被管理的应用服务器和ProxyServer,注意第二个节点的IP地址要填第二个节点服务务器配置IP,这里是192.168.0.5,然后添加代理服务器,端口配置为8080,单击下一步
**9配置集群**
单击“添加”，输入Cluster 的名称new_Cluster，Multicast address 组播地址（用来进行
集群间的状态通信）和端口7777，Cluster 的各个服务器地址和端口,完成后单击“下一步”按钮。
注意：当用Configuration Wizard 创建集群时，您只能选择multicast，要选择unicast，您只能在Administration Console 中修改集群的配置，参考Oracle Fusion Middleware Oracle WebLogic Server Administration
Console 帮助文档中的："Create and configure clusters".
注意： multicast address 的IP 范围必须位于224.0.0.0 到 239.255.255.255 之间.
WebLogic Server 使用的multicast 默认值为 239.192.0.0.
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218509ioAF.jpg)![204503599.jpg](http://blog.51cto.com/attachment/201312/204503599.jpg)**
**10.向集群分配服务器**
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218510MzHp.jpg)![204420429.jpg](http://blog.51cto.com/attachment/201312/204420429.jpg)**
选择左面列表中的Managed Server，将其添加到右面的列表中,但Proxy_Server不要添加到右边,然后下一步.
创建Http代理应用程序,这时我们就可以选择Proxy_Server,单击下一步的配置计算机,我们不对其进行配置.
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_13862185125SbP.jpg)![204338483.jpg](http://blog.51cto.com/attachment/201312/204338483.jpg)**
**11检查Weblogic****配置**
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218513froV.jpg)![204255367.jpg](http://blog.51cto.com/attachment/201312/204255367.jpg)**
安装另一个节点时，重复上面的操作，配置管理服务器时地址也选择本机地址；配置受管理服务器（代理服务器实际上也是一个受管理服务器）这里的配置和第一个节点的配置完全一样；配置群集信息集群这不需要再配置了，继续操作就行。
设置启动不需要密码:
在AdminServer/security目录下，以weblogic用户身份建立boot.properties文件，实现启动时不用输入用户名和密码
内容为：
username=weblogic
password=welcome1
不用担心密码泄露，因为启动weblogic服务器后boot.properties文件将被服务器用AES算法加密。所以建议大家在调试好后不要随意修改用户名，那样这个文件就失效了。
**12 启动****WebLogic****的****AdminServer ****服务**
注意只有启动adminserver后才能进一步下面的配置任务,所以我们在脚本目录bin下输入./startWebLogic.sh 但是这个脚本是一个前台程序，一旦窗口意外关闭，就会导致应用程序的推出，所以我们用下面的命令将其放到后台执行。
nohup ./startWebLogic.sh& \\监听TCP 7001端口
在这里“nohup”表示给命令一个非挂起的信号，“&”表示将服务器放到后台运行，但由于后台运行中有可能报错，我们在实际当中还是在机房主机X-window里启一个窗口单独启动它。
下面是关键的一个环节，服务器启动顺序，次序不能颠倒，否则将会报错导致无法正常启动。
**![124452487.jpg](http://blog.51cto.com/attachment/201312/124452487.jpg)**
启动第一个节点：
启动第一个节点的命令：
./startWebLogic.sh
./startManagedWebLogic.sh new_ManagedServer_1 http://192.168.0.4:7003
./startManagedWebLogic.sh ProxyServer http://192.168.0.4:8080
同样我们登录到第二个节点上启动第二个节点服务。
**13.查看服务器配置情况**
登录AdminServer服务器管理端口http://192.168.0.4:7001/console&
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218516kKjJ.jpg)![204130932.jpg](http://blog.51cto.com/attachment/201312/204130932.jpg)**
**添加数据源**
**进入consol****控制台后再左边域结构树中点服务前的“+****”，单击“新建”，再填写信息如下图**
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218518K5Qr.jpg)![203947721.jpg](http://blog.51cto.com/attachment/201312/203947721.jpg)**
再单击“新增”，然后继续单击“下一步”
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218521H0Rr.jpg)![203827727.jpg](http://blog.51cto.com/attachment/201312/203827727.jpg)**
**接下来，输入数据库名qbdb,****主机名：192.168.0.4****；端口号：1521****；以及口令**
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_13862185249y7i.jpg)![203640246.jpg](http://blog.51cto.com/attachment/201312/203640246.jpg)**
单击“测试配置”，测试成功，单击“完成”测试成功后，此数据源就可以正常使用了，关于数据连接池的调优受到Oracle的官方授权的实现，请购买授权后修改数据库连接池的参数。
**部署WEB 应用**
在左边域结构树中点击“环境”前的“+”，在展开的菜单中点击“服务器”，在右边的列表中可以看到各服务器是否正常启动。
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218526AKbS.jpg)![203412685.jpg](http://blog.51cto.com/attachment/201312/203412685.jpg)**
如果正常启动则单击左侧域结构下的“ 部署”![203243608.jpg](http://blog.51cto.com/attachment/201312/203243608.jpg)
再单击右侧的“安装”按钮，选择要部署的WEB 应用程序，笔者这里在“路径”输入框中
点击安装按钮，可以手工输入Web 应用所在目录：/root/Oracle/Middleware/wlserver_10.3/resources，也可通过点击上传来选择文件所在目录。
**[](http://chenguang.blog.51cto.com/attachment/201312/5/350944_1386218530tZdw.jpg)![203035363.jpg](http://blog.51cto.com/attachment/201312/203035363.jpg)**
完成后，单击“下一步”，选择群集“new_Cluster”，再选择“群集中的所有服务器”，之后单击“下一步”，再单击“下
一步”，再单击“完成”。最后再单击左上角的“激活更改”。
**启动****WEB ****应用**
进入console 控制台后，点击左侧的域结构下的“部署”，接着单击选中部署列表中的
应用（这里是DefaultWebApp），然后单击“启动”按钮下的菜单项“为所有请求提供服务”，
![202900543.jpg](http://blog.51cto.com/attachment/201312/202900543.jpg)
接下来在出现的启动部署页再单击“是”按钮。当看到DefaultWebApp 右侧的状态从“准备就绪”转换为“活动”时就表示WEB 应用启动完成。
注意，如果需要修改字符集，必须在每个服务器中编辑/etc/sysconfig/i18n，实例文件/etc/sysconfig/i18n内容如下：
LANG="zh_CN.GB18030"
LANGUAGE="zh_CN.GB18030:zh_CN.GB2312:zh_CN"
SUPPORTED="zh_CN.GB18030:zh_CN:zh:en_US.UTF-8:en_US:en"
SYSFONT="latarcyrheb-sun16"
然后 source /etc/sysconfig/i18n生效
实验环境下载：
操作系统[下载](http://chenguang.blog.51cto.com/350944/1582654)              WebLogic10 for Linux[下载](http://pan.baidu.com/s/1pJlyDuj)
