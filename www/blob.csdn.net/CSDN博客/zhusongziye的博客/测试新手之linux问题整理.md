# 测试新手之linux问题整理 - zhusongziye的博客 - CSDN博客





2018年06月09日 16:30:04[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：90标签：[测试新手之linux问题整理](https://so.csdn.net/so/search/s.do?q=测试新手之linux问题整理&t=blog)
个人分类：[linux知识](https://blog.csdn.net/zhusongziye/article/category/7309653)









测试职场的新人，难免都会涉及到一些linux系统的命令操作，搭建测试环境也好，查看日志也罢，以下给大家整理初学者可能会碰到的问题，希望对大家有所帮助。



1、Linux中输入基本操作命令例如ls，遇到报  -bash:  XXXX: command not found 错误




![](https://img-blog.csdn.net/20180609162902187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




以上报错说明依据环境变量配置内容，找不到执行该命令的文件。解决思路就是将该应用的bin目录放入到 profile文件中即可。




1) ls是系统基本应用命令，一般存放在/bin、/sbin、/usr/local/bin、/usr/local/sbin、/usr/sbin或/usr/bin文件中，所以修改vi /etc/profile

在最后面添加export PATH=$PATH:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

保存退出profile文件后，记得source /etc/profile 使其配置生效




2）如果是自己安装应用的命令，比如javac，是jdk自带命令，修改vi /etc/profile

     自己确认jdk环境变量设置的bin目录是否正常，请参考以下jdk的环境变量配置：

     export JAVA_HOME=/usr/local/jdk1.8.0_65（jdk安装目录）

     export PATH=$JAVA_HOME/bin:$PATH

     export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar

当然难免也会有手残的初学者将存在这些目录中的命令拼错，当然就找不到啊，自行面壁思过吧。




PS：PATH变量配置全不全，可以输入echo $PATH查看下




2、自己安装应用后，运行报错 -bash：xxx cannot execute binary file




![](https://img-blog.csdn.net/20180609162920757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




这个错误一般都是因为安装应用的系统位数跟操作系统位数不匹配导致。




1）首先查看系统位数，输入uname -a

![](https://img-blog.csdn.net/20180609162931384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




x86_64则说明你是64位内核, 跑的是64位的系统；

i386, i686说明你是32位的内核, 跑的是32位的系统。




2）获取对应系统位数版本的应用进行重新安装。



3、linux中操作文件及目录，报Permission denied 错误




1）权限不够，赋予权限；输入chowd 777 -R 文件名

（777 是所有用户都拥有最高权限）




Ps：文件权限不要随意修改。特别是系统文件，为了数据安全与保密，不建议文件修改、读取权限包括执行提供给每个账户。




2） 切换root用户



4、使用yum命令过程中，报This system is not registered with RHN 错误




Linux系统之所以会出现这个错误是因为没有注册RHN，我们只需要更新一下yum的源就可以了。




1）使用命令 cd /etc/yum.repos.d/进入yum的配置目录




2）在终端中输入 wget http://docs.linuxtone.org/soft/lemp/CentOS-Base.repo 命令，下载CentOS- Base.repo文件。




3）然后将原有的rhel-debuginfo.repo备份一下，使用命令mv CentOS-Base.repo rhel-debuginfo.repo，将CentOS- Base.repo重命名成rhel-debuginfo.repo




4）成功以后，使用yum命令就成功



5、虚拟机进行ip自动获取，但获取不到ip




在学习linux时，一般会涉及到虚拟机的安装学习，可能会碰到此问题。NAT方式上不了网时，应该是与VMware相关的服务没有打开，




1）开启服务：点开始菜单-》运行-》键入：services.msc




2）然后将虚拟机相关服务启动：

     VMware Authorization Service

     VMware DHCP Service

     VMware NAT Service



