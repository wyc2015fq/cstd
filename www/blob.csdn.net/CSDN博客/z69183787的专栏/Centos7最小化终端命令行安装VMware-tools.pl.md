# Centos7最小化终端命令行安装VMware-tools.pl - z69183787的专栏 - CSDN博客
2018年02月07日 18:48:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1805
点击虚拟机
一、准备：
#yum
 install gcc  kernel-headers kernel-devel  ///因为编译需要，要装这几个包
二、安装：
挂载光驱：
# mkdir
 /mnt/cdrom       ///创建挂载目录
# mount
 /dev/cdrom /mnt/cdrom    ///将光驱挂载到/mnt/cdrom目录
# cd
 /mut/cdrom
里面有两个文件，.rpm文件是给Red Hat准备的，只需要.tar.gz的那个
# cp VMwareTools-XXXX.tar.gz
 /usr/local    ///复制VMware-tools到/usr/local目录 ，其中XXX是版本号。
# cd
 /usr/local   ///切换到local目录
# tar
 -vzxf VMware Tools-XXX.tar.gz   
 ///解压，区分大小写，也可以先输入前几个字母按Tab键系统补充后面的。
环境好了，下面开始安装VMware-tools
# cd
 vmware-tools-distrib  ///进入解压出来的目录
# ./vmware-install.pl    ///运行vmware-install.pl文件开始安装。
会提示若干次yes or no 直接一路回车就行，直到最后出现“Enjoy——the
 VMware team”的字样，VMwareTools安装完成。
三、卸载：
到刚才解压的目录/usr/local/vmware-tools-distrib/bin
 目录或者 /usr/bin
# cd
 /usr/local/vmware-tools-distrib/bin
# ./vmware-uninstall-tools.pl    ///运行卸载程序
#rm
 -rf /etc/vmware-caf 
    ///递归强制 删除残留目录
#rm
 -rf /etc/vmware-tools 
  ///递归强制 删除残留目录
#  rm
 -rvf /usr/lib/vmware-tools///递归强制 删除残留目录
四、启动
# /usr/bin/vmware-user
 start ///手动启动
注：黄颜色背景的为具体命令
BY:悟透
参考文章或回答：
[http://zhidao.baidu.com/link?url=45hx0Hkj8fSmUC2ds8i_uXOaZlCspvht3YjjmE62xtFY_WKYBSdsJ4ENtg4f_bi68zjLY4PA1OYKk3zKeUZYRa](http://zhidao.baidu.com/link?url=45hx0Hkj8fSmUC2ds8i_uXOaZlCspvht3YjjmE62xtFY_WKYBSdsJ4ENtg4f_bi68zjLY4PA1OYKk3zKeUZYRa)
[http://www.myhack58.com/Article/48/67/2014/54523.htm](http://www.myhack58.com/Article/48/67/2014/54523.htm)
