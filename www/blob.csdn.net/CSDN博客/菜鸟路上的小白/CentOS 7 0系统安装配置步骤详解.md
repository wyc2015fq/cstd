# CentOS 7.0系统安装配置步骤详解 - 菜鸟路上的小白 - CSDN博客





2016年09月06日 14:58:27[somnus_小凯](https://me.csdn.net/u012486840)阅读数：562









CentOS 7.0系统是一个很新的版本哦，很多朋友都不知道CentOS 7.0系统是怎么去安装配置的哦，因为centos7.0与以前版本是有很大的改进哦。



**说明：**


截止目前[CentOS](http://www.111cn.net/list-232/) 7.x最新版本为CentOS 7.0，下面介绍CentOS 7.0的具体安装配置过程


服务器相关设置如下：


操作系统：CentOS 7.0 64位


IP地址：192.168.21.128


网关：192.168.21.2


DNS：8.8.8.8 8.8.4.4

**备注：**


生产服务器如果是大内存（4G以上内存），建议安装64位版本CentOS-7.0-1406-x86_64-DVD.iso

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164845793.jpg)

**一、安装CentOS 7.0**


成功引导系统后，会出现下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164848613.jpg)


界面说明：


Install CentOS 7 安装CentOS 7


Test this media & install CentOS  7 测试安装文件并安装CentOS  7


Troubleshooting 修复故障


这里选择第一项，安装CentOS  7，回车，进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164851738.jpg)

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164856427.jpg)


选择语言：中文-简体中文（中国）  #正式生产服务器建议安装英文版本

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164859584.jpg)


继续

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164906580.jpg)


选择-系统-安装位置，进入磁盘分区界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164910669.jpg)


选择-其它存储选项-分区-我要配置分区，点左上角的“完成”，进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164913116.jpg)


分区前先规划好


swap #交换分区，一般设置为内存的2倍


/ #剩余所有空间


备注：生产服务器建议单独再划分一个/data分区存放数据

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164917677.jpg)


点左下角的“+”号


挂载点：swap


期望容量：2048


添加挂载点，如下图所示

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164922545.jpg)


系统运维  www.111cn.net  温馨提醒：qihang01原创内容©版权所有,转载请注明出处及原文链


继续点左下角的“+”号


挂载点：/


期望容量：18.43GB #剩余所有空间


添加挂载点，如下图所示

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164926386.jpg)

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164933599.jpg)


点左上角的“完成”，进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164938417.jpg)


接受更改，进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164943976.jpg)


开始安装  #注意“软件”-“软件选择”，默认是最小安装，即不安装桌面环境，可以自己设置。


进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164947983.jpg)


选择-用户设置-ROOT密码，进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/2014070816495196.jpg)


设置Root密码


如果密码长度少于8位，会提示要按“完成”两次来确认，安装继续

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708164956646.jpg)


安装完成之后，会进入下面的界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165003843.jpg)


点重启

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/2014070816500657.jpg)


系统重新启动

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165010325.jpg)


进入登录界面

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165014710.jpg)


账号输入root 回车


再输入上面设置的[root密码](http://www.111cn.net/tags.php/root%C3%DC%C2%EB/)回车


系统登录成功

**二、设置IP地址、网关DNS**


说明：CentOS 7.0默认安装好之后是没有自动开启网络连接的！


cd  /etc/sysconfig/network-scripts/  #进入网络配置文件目录

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165019615.jpg)


vi  ifcfg-eno16777736  #编辑配置文件，添加修改以下内容

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165022697.jpg)


HWADDR=00:0C:29:8D:24:73


TYPE=Ethernet


BOOTPROTO=static  #启用静态IP地址


DEFROUTE=yes


PEERDNS=yes


PEERROUTES=yes


IPV4_FAILURE_FATAL=no


IPV6INIT=yes


IPV6_AUTOCONF=yes


IPV6_DEFROUTE=yes


IPV6_PEERDNS=yes


IPV6_PEERROUTES=yes


IPV6_FAILURE_FATAL=no


NAME=eno16777736


UUID=ae0965e7-22b9-45aa-8ec9-3f0a20a85d11


ONBOOT=yes  #开启自动启用网络连接


IPADDR0=192.168.21.128  #设置IP地址


PREFIXO0=24  #设置子网掩码


GATEWAY0=192.168.21.2  #设置网关


DNS1=8.8.8.8  #设置主DNS


DNS2=8.8.4.4  #设置备DNS


:wq!  #保存退出


service network restart   #重启网络


ping www.baidu.com  #测试网络是否正常

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165026506.jpg)


ip addr  #查看IP地址

![CentOS 7.0系统安装配置步骤详解](http://filesimg.111cn.net/2014/07/08/20140708165030452.jpg)

**三、设置主机名为www**


hostname  www  #设置主机名为www


vi /etc/hostname #编辑配置文件


www   #修改localhost.localdomain为www


:wq!  #保存退出


vi /etc/hosts #编辑配置文件


127.0.0.1   localhost  www   #修改localhost.localdomain为www


:wq!  #保存退出


shutdown -r now  #重启系统



