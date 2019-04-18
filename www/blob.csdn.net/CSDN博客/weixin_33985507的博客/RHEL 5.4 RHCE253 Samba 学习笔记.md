# RHEL 5.4 RHCE253 Samba 学习笔记 - weixin_33985507的博客 - CSDN博客
2010年05月29日 00:58:31[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
[](http://wqmsl.blog.wqmsl.com)
[**我的博客：http://wqmsl.blog.51cto.com/**](http://wqmsl.blog.wqmsl.com)**制作人：wqmsl**
目标：1、搭建一个基本的samba使用匿名访问
      2、搭建使用虚拟用户访问的samba服务器
      3、搭建一个基于用户配置文件的samba服务器
      4、搭建一个企业常用的samba服务器实例
      5、samba服务器常用配置参数总结介绍
      6、samba的原理讲述
1、IP说明：
192.168.0.2——主机名：samaba.wqmsl.com   samba服务器
192.168.0.3——主机名：client.wqmsl.com    client客户端
2、前期准备
   更改主机名称(更改三处)：1、hostname更改，2、hosts更改，3、/etc/sysconfig/network
3、所需软件包如下
   samba-common-3.0.33-3.14.el5
   samba-3.0.33-3.14.el5
   system-config-samba-1.2.41-5.el5
   samba-swat-3.0.33-3.14.el5（用来web页面管理samba，来对Samba服务器进行图形化管理，安装的时候有依赖关系，使用yum安装哦）
   samba-client-3.0.33-3.14.el5
下面我们来安装一下samba吧
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066655UjjQ.png)
Samba的所有配置文件所在的目录
![wps_clip_image-29714](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066655V36v.png)
我们一般配置smb.conf ，下面我们来看一下里面的内容哈：
*******************
解释：
Smb.conf配置文件里面默认有288行，里面以“#”和“；”开头的全是注释语
常规的samba服务器配置过程如下：
1、编辑主配置文件smb.conf,指定需要共享的目录，为共享目录设置共享权限。
2、在smb.conf文件中指定日志文件名称和存放路径和日志的大小。
3、设置共享目录的本地系统权限。
4、重新加载配置文件或重新启动smb服务，使用配置生效
*******************
Global配置区域如下，此区域配置全局生效，对所有的共享文档生效哈：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066656X1Vl.png)
共享文档配置区域如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066656idYQ.png)
家目录共享配置区域如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066657ZyIn.png)
打印机共享配置区域如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066657P8FE.png)
看的差不多了吧，因为文件过多，所以samba文件为我们区分的很明确，要配置什么就去什么区域配置哈：
在配置之前我们最好先备份一下主配置文件，以免以后文件损坏后好恢复
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666574N82.png)
一、实现第一个实验目标，搭建一个基本的samba服务器，使用匿名访问
配置主配置文件smb.conf文件
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066657ibpV.png)
workgroup ——配置所属的工作组
server string——服务器的描述
hosts allow——配置允许访问的网段，默认是注释掉的，意思就是允许所有访问
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066657E6wO.png)
把默认的security = user（需要系统账户访问） 改为security = share （可以使用匿名访问）
![wps_clip_image-18369](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658MPqF.png)
****************************
解释：
[share] ——服务器共享文档名称
comment = share ——服务器说明
path = /share ——共享文档的路径
publice = yes ——允许匿名访问
writable = yes ——允许写入（前提是目录本身有其他用户写入的权限）
printable = no —— 并非打印机
******************************
保存退出
建立共享文档目录并设置权限
![wps_clip_image-27861](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658nK3A.png)
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658spok.png)
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658rxH5.png)
设置冒险位（目的是用户a在共享目录里面写入文件，那么用户b登陆看到之后只能读取却不能删除，起到了防止用户误删除其他用户的文件）
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658x86l.png)
我们在share目录里面建立一个文件进行测试
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658iVUh.png)
重启服务（重新加载）使用其中之一即可，我是为大家做演示
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658h2EG.png)
下面我们使用Windows系统访问一下，下面看看哈：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066658PpdN.png)
看见了吧，共享目录share下面有我们建立的test.share文件，我试着写入看看有没有权限哦
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066659NNLk.png)
看见了吧 ，我建立成功了哦，说明第一个目标我们实现了哈，那么下面我们开始第二个目标哈
二、那么实现第二个目标，使用虚拟用户访问的samba服务器
这个其实很简单啦，只要三步即可实现
1、修改smb.conf开启虚拟用户访问，修改服务器安全模式
2、添加samba用户和虚拟用户映射
3、重启服务即可
下面我们开始配置
修改smb.conf文件
修改服务器安全模式为user
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066659n3xm.png)
在“Global Settings”区域添加如下参数，开启虚拟账户映射
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066659SVvS.png)
编辑smbusers文件，内容如下，我映射samba用户wqmsl为虚拟用户a,b,c
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066660ts6Q.png)
首先要有系统用户，然后添加到samba用户并设置密码
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066660eEpM.png)
重启samba服务或者重新加载服务
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066660PyEz.png)
使用映射用户a在客户端访问samba共享目录，要求输入用户和密码：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066660h7V8.png)
看到了吧，a用户通过了验证，共享目录里面有wqmsl的家目录，说明我们用的实际是wqmsl用户访问的samba共享
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066661vKKa.png)
下面我们使用用户b访问samba服务试试看
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066661jMj8.png)
访问的内容和用户a是一样的
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066662piUd.png)
（注意：在Windows下面默认会记录访问共享目录用户，再次访问就会默认登录，所以我们需要清除网络连接，打开运行—>cdm—>net use （查看现有的连接）—>net  use  *  /del
如下图所示：）
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_127506666334yu.png)
使用虚拟用户在一定程度上保护了计算机上用户的账号信息，安全么，可以用用，即使有人知道你的samba用户，但是要想破解你的系统密码还是很难哈，会累死他的，整半天也只是一个虚拟用户而已哈。
说明我们第二个目标已经实现了，那么我们进行第三个目标哦！
三、现在我们来实现第三个目标，搭建一个基于用户配置文件的samba服务器
使用独立用户的配置文件可以更好的去为用户配置权限，而且也非常的方便，就和我们在vsftpd里面不是也用到过虚拟用户和用户的单独配置文件么，其实差不多的，不过实现起来还是比较简单的哦，下面我们开始做吧
我们先简单的实现一下，公司的share共享目录大家都有读写的权限，总经理需要有一个自己的共享目录，又不能让大家看见或者访问，不想让大家看见的话可以使用隐藏共享目录的参数来实现，但是这样的话只要让别人知道了总经理的共享目录名称即可访问到了，所以这里我们使用单独配置文件来实现，又方便又安全是不是哈
现在我们开始配置吧
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066664Zwo1.png)
include = /etc/samba/%U.smb.
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066664pWlo.png)
***************************
解释：
include = /etc/samba/%U.smb.conf ————含义：读取用户的单独配置文件，位置在/etc/samba/下的以  用户名.smb.conf 命名的配置文件
有一些资料里面会config file 来写，它和include是有区别的，区别在于使用include字段的时候samba还会读取smb.conf  里面的共享目录配置，但是使用config file 的时候就不会了，它会直接去读取用户的单独配置文件，之后我们通过实验来说明吧
***********************
复制用户单独配置文件，直接复制smb.Conf为用户的配置文件即可
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066664ab5y.png)
配置jingli的共享目录
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666647bck.png)
*********************
解释：
valid users = jingli ———— 含义：只允许jingli来访问本共享目录
*********************
在根目录下面建立经理的共享目录并设置属主和属组为jingli，以便经理能够有完全控制的权限
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066664oOlc.png)
下面我们使用客户端访问试试哈（访问之前记得要清除共享访问记录哦）：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066665IpgB.png)
看看里面的内容：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_127506666563rU.png)
看见没有，里面有经理的共享目录，并且还有share共享目录和jingli的家目录
我在里面创建文件看看
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066666od6F.png)
创建和删除都成功，说明权限正确
下面我在用别的用户访问试试，看看里面的内容
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066666jkFg.png)
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066667al2l.png)
没有吧 只有共享的公共目录和用户的家目录，没有别的哈
公共用户登陆之后根本看不见经理的共享文件夹
即使我知道了经理共享目录的名称为 jl，如图下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066667scTw.png)
是没有权限访问的哈。
用户独立配置文件目标实现，已经成功
目标 4和5 在笔记二里面再介绍吧，今天就写到这里了
四、下面我们来实现第四个目标，我们来做一个企业经典的samba服务器实例
实例如下：企业要求搭建samba服务器来提供文件共享服务，有财务部、销售部，还有公共共享文件目录，财务部和销售部都事关部门机密文件，别人不能读取，所以要实现只有公司经理能读取和写入财务部、销售部的共享目录，各个部门员工只能访问和写入本部门的共享目录，但不能删除其他用户写入的文件，公共目录职能读取不能写入。
下面我们来做吧，一个一个来实现
先创建要共享的目录并为销售部和财务部加上冒险位（关于冒险位就不多说了，在“samba学习笔记一”里面讲过的哦）
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066668Nqiv.png)
编辑smb.conf文件
添加如下参数，是samba服务器读取用户或者组的单独配置文件
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066668bB1L.png)
配置共享公共目录
![wps_clip_image-19350](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666682OZC.png)
保存退出
我们创建所需要的账户和组，经理：jingli, 财务部：c1 、c2（组：caiwu）  销售部：x1 、x2(组：xiaoshou) 其他部门员工账号：q1
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666689mpL.png)
系统用户创建成功，我们把这些用户添加成samba用户
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066669tzWF.png)
所有的samba用户创建已经成功
我们创建单独的用户、组配置文件，看下图：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066669ZKXH.png)
其中smb.conf. bak 是我之前备份的smb.conf文件
配置xiaoshou组、caiwu组、jingli的配置文件
配置xiaoshou组的配置文件中的共享文档
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666706pmj.png)
配置caiwu组的共享文档，如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066670y0fQ.png)
配置经理的配置文件，如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066670SSVm.png)
重启服务如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066670EME3.png)
下面我们来测试：
先使用经理的账户登录试试
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066670plxs.png)
登录后看到的共享文档如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666719hDa.png)
销售部、财务部、公共共享 都能看到的，还有一个是自己的家目录，下面我们来测试权限，看有没有写入的权限
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_127506667127Ct.png)[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666720WKM.png)
如上：销售部和财务部我们都有写入的权限
下面我们测试销售部门员工登录情况
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066672vRA2.png)
登录后能访问的共享目录如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066673ZS0R.png)
我们在销售部目录创建文件
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066673ahu9.png)
我们测试一下冒险位的作用，我去删除jingli创建的文件试试：如下
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_12750666741cKD.png)
无法删除，说明实验成功哦
下面测试财务部的哦，呵呵
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066674AplZ.png)
这好似c1登录后能够共享的目录，测试创建文件和删除其他用户的文件
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066675wEgP.png)
当然我们也可以使用Linux客户端来访问并挂载samba共享目录，很简单，具体访问和挂载命令看第四部分“samba服务器配置文件常用参数和命令解释”
看到实验的结果了吧，所有的目标实现，第四个目标完成，那么我们开始下一个呗！
四、samba服务器配置文件常用参数和命令解释：
全局配置参数
1、在global下添加一行字段username map = /etc/samba/smbusers开启用户帐号映射功能。   samba帐号 = 虚拟帐号（映射帐号）
hosts allow 和 hosts deny 字段的使用
2、hosts allow 字段定义允许访问的客户端
   hosts deny 字段定义禁止访问的客户端
   hosts deny = All 表示所有客户端，不是说名称为all的主机哈，这是通配符哦。
   hosts allow = 192.168.0. EXCEPT 192.168.0.100 192.168.0.78 表示允许            192.168.0.0网段IP地址访问，但是192.168.0.100和192.168.0.78除外
  （这个也可以加入到共享目录的配置里面，这样的话只对共享目录生效）
3、在[global]中加入config file = /etc/samba/smb.conf.%U，表示samba服务器读取 /etc/samba/smb.conf.%U文件，其中%U代表当前登录用户。
    也可以添加如下参数：不过是有区别的，“我在samba学习笔记一”里面讲过了
  （看看效果吧，我修改smb.conf里面的读取用户配置参数include为config file之后，用客户端访问如下：
[](http://wqmsl.blog.51cto.com/attachment/201005/28/847418_1275066676ooHv.png)
    看到了吧，在smb.conf里面配置的share共享文档没有啦，说明config file参数讲不读取smb.conf里面的共享目录，而是直接读取了用户或组的单独配置文件，两个字段参数各有所长，所以看你怎么用了 ）
include = /etc/samba/%U.smb.conf表示使samba服务器加载/etc/samba目录下格式为“用户名.smb.conf”的配置文件。
include = /etc/samba/%G.smb.conf表示使samba服务器加载/etc/samba目录下格式为“组名.smb.conf”的配置文件。
4、security = share / user........安全级别的参数：
定义Samba的安全级别，按从低到高分为四级：share，user，server，domain。它们对应的验证方式：     
share：任何用户无需用户名和口令即可访问服务器上的资源。      
user：samba的默认配置，在访问共享资源之前必须通过本地用户验证。      
Server：验证是通过另一台服务器进行的，如果失败，就退到user级。      
domain：验证通过网络上的Windows的主域控制器进行。      
后面三种安全级都要求“用户”在提供共享的Linux主机上要有帐户的。
5、默认/etc/samba下面没有smbpasswd文件，解决办法：在smb.conf文件中注释掉passdb backend = tdbsam 一行，在下面加上smb passwd file = /etc/samba/smbpasswd
6、使用加密口令  ————  encrypt password = yes/no 
共享目录配置参数
1、comment = 备注信息
2、path =  绝对地址路径
3、public = yes     #允许匿名访问
   public = no      #禁止匿名访问
   （public和valid users 不能同时使用，这样public没效果）
4、valid users  =  @组名，用户名 ，但是前提是public设置成no
   invalid users  =  @组名，用户名，就算你上面允许他了，但是加入这句，这个用户也 会     被拒绝。一般用在允许所有，而禁止某几个用户的时候使用
5、readonly = yes    #只读
   readonly = no     #读写
read list  =  @组 用户  指定只能读取该共享资源的用户和组。 
6、writable = yes      #读写
   writable = no       #只读 
7、write list = 用户名
   write list = @组名，用户名
   write list 要生效的话，writeable 也设置成no 
8、smbpasswd 的参数命令如下：
-a 添加用户 #smbpasswd -a wqmsl     
-x 删除用户 #smbpasswd -x wqmsl      
-d 禁用帐号 #smbpasswd -d wqmsl      
-e 取消禁用 #smbpasswd -e wqmsl      
-n 清除密码 #smbpasswd -a wqmsl
9、writable yes 所有帐号都允许写入
    writable no 所有帐号都禁止写入
    write list 写入权限帐号列表 列表中的帐号允许写入
10、browseable = no表示隐藏该目录
11、smbclient命令格式：smbclient -L 目标IP地址或主机名 -U 登录用户名%密码
    smbclient -L 192.168.0.188 -U boss%boss
12、smbclient //目标IP地址或主机名/共享目录 -U 用户名%密码
13、mount -t cifs //目标IP地址或主机名/共享目录名称 挂载点 -o username=用户名
14、testparm /etc/samba/smb.conf   命令检查
18、
15、1）samba服务的启动
    service smb start 或 /etc/rc.d/init.d/smb start
2）samba服务的停止
    service smb stop 或 /etc/rc.d/init.d/smb stop
  3）samba服务的重启
    service smb restart 或 /etc/rc.d/init.d/smb restart
 4）samba服务配置重新加载
    service smb reload 或 /etc/rc.d/init.d/smb reload
（注意：Linux服务中，当我们更改配置文件后，一定要记得重启服务哦，让服务重新加载配置文件，这样新的配置才可以生效么）
 5）自动加载samba服务
    chkconfig --level 35smb on           #运行级别3自动加载
      chkconfig --level 35smb off          #运行级别3不自动加载
16、设置是否允许打印配置文件中的所有打印机开机时自动加载   load printers = yes 
设置Samba 服务启动时，将自动加载的打印机配置文件   printcap name = /etc/printcap
五、samba服务器的介绍和原理
Samba概述
Samba是最先在Linux和Windows两个平台之间架起了一座共享的桥梁哦，正是由于Samba的出现，我们可以在Linux系统和Windows系统之间互相通信，如拷贝文件、实现不同操作系统之间的资源共享等等，我们可以将其架设成一个功能非常强大的文件服务器，也可以将其架设成打印服务器提供本地和远程联机打印，甚至我们可以使用samba Server完全取代NT/2K/2K3中的域控制器，做域管理工作，很强大的哦
Samba应用环境
文件和打印机共享、身份验证和权限设置、名称解析：Samba通过nmbd服务可以搭建NBNS(NetBIOS Name Service)服务器 、浏览服务。
Samba工作原理
Samba服务功能强大，这与其通信基于SMB协议有关。SMB还提供目录和打印机共享，并支持认证、权限设置。SMB运行于NBT协议（NetBIOS over  TCP/IP）上，使用UDP协议的137、138及TCP协议的139端口，后期SMB经过开发，可以直接运行于TCP/IP协议上，没有额外的NBT层，使用TCP协议445端口。
**制作人：wqml****                                      QQ :459176770**
