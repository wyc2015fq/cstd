# KVM+Qemu+Libvirt实战 - ggabcda的博客 - CSDN博客





2017年09月06日 17:49:48[ggabcda](https://me.csdn.net/ggabcda)阅读数：1621








上一篇的文章是为了给这一篇文件提供理论的基础，在这篇文章中我将带大家一起来实现在linux中虚拟出ubuntu的server版来

我们需要用KVM+Qemu+Libvirt来进行kvm全虚拟化，创建虚拟机并管理。

kvm是用来虚拟化或者说模拟CPU、内存等硬件的。

QEMU只是用来虚拟化硬盘的

libvirt提供了整个虚拟机的管理，比如说虚拟机的启动，停止，创建，删除等等。



**其实KVM+Qemu+Libvirt就是模拟了一个VMWare软件**



环境：

　　　　宿主机：ubuntu16.04的server版

　　　　kvm虚拟化中安装的虚拟化操作系统为：ubuntu16.04的server版

**在这里中一定要记住不能使用VMWare中的ubuntu当宿主机去做KVM虚拟化，因为VMWare本身已经是虚拟化的了。**

# 一、KVM支持检测及模块安装

## 1.1、KVM支持检测

　　　　1）CPU检测：sudo kvm-ok

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905195732069-231261829.png)如果出现左边的提示，则表示支持kvm

　　　　　　grep vmx /proc/cpuinfo 

## 1.2、安装KVM模块

　　　　sudo apt install kvm 

　　　　　模块启动检测：lsmod |grep kvm

# 二、虚拟磁盘管理

## 　　2.1、安装qemu-utils工具

　　　　sudo apt install qemu-utils

## 　2.2、对/dev/vdb分区为vdb1，挂载到"/vdb1"

　　　　这个步骤在linux教程的文章中有，不懂得可以去查看

## 　　2.3、创建虚拟磁盘

　　　　sudo qemu-img create -f qcow2 /zyhdata/us1.qcow2 30G

　　　　运行结果

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905220246147-291323393.png)

## 　2.4、虚拟磁盘文件的检查

　　　　sudo qemu-img info /zyhdata/us1.qcow2

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905220309069-1531307885.png)





# 三、libvirtd配置

## 　　3.1、安装libvirtd工具

　　　　sudo apt install vitinst

## 　　3.2、检测该工具是否安装

　　　　ps -ef|grep -i libvirtd　　查看是否有libvirtd进程

## 　3.3、sudo vi /etc/libvirt/libvirtd.conf

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905220331538-709685654.png)
　　　　这些内容在该文件中或许都已经存在，也有可能是以注释的形式体现的，取消注释，如果值和上面中的不符合，就改成上面中的值。

## 　　3.4、修改libvirt-lib文件

　　　　sudo vi /etc/default/libvirt-bin中修改其中的libvirtd_opts="-d -l"

## 　　3.5、修改qemu.conf文件

　　　　sudo vi /etc/libvirt/qemu.conf中修改其中的vnc_listen="0.0.0.0"

## 　　3.6、重启libvirt-bin服务

　　　　sudo service libvirt-bin restart

　　　　注意：如果你将你的Ubuntu操作系统升级到了16.10(04)版本，libvirt-bin服务跟操作系统由冲突，不能这样重启服务。只能是重启操作系统。

　　　　　　　　   sudo reboot

# 四、创建或安装（虚拟）操作系统

## 　　4.1、命令安装

### 　1）使用virt-install命令进行安装，一步一步的执行


![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905221852116-2020096599.png)
　　　　　我们就这样在console中一行一行的往下走。

### 2）使用shell脚本来安装

　　　　　　使用shell脚本安装（代替上面的1）），省得我们一行一行在console敲。　　

create_kvm.sh文件　

```
1 DIST_QCOW2=$2                                                               
  2 ORIGN_ISO=$3
  3 
  4 sudo virt-install \
  5 --connect qemu:///system \
  6 --name $1 \
  7 --ram 2048 \
  8 --vcpus=2 \
  9 --disk path=${DIST_QCOW2},format=qcow2,size=20,bus=virtio \
 10 --cdrom $ORIGN_ISO \
 11 --vnc \
 12 --os-type linux \
 13 --accelerate \
 14 --hvm \
 15 --network bridge=virbr0,model=virtio \
 16 --noautoconsole
```



　　　　　注意：在上面的脚本文件中\代表的是续行符。在shell中已经使用的sudo

　　　　接下来就是运行脚本进行安装了：　　　　　　　　



```
shell/create_kvm.sh us1 /vdb1/us1.qcow2 /iso/ubuntu-17.04-server-amd64.iso　
　　注意：
　　　　　　在这里脚本中有三个参数：
　　　　　　　　　　　　　　　　　　第一个为虚拟机名字
　　　　　　　　　　　　　　　　　　第二个为指定的虚拟磁盘
　　　　　　　　　　　　　　　　　　第三个为ubuntu系统的镜像文件
```

　　　注意：镜像文件的选取：搭建集群保证所有的服务器的操作系统类型和操作系统版本以及内核版本最好都一致。　

　　　　　　sudo apt dist-upgrade 升级软件以及内核

### 　　　3）使用virsh命令

　　　　　　　virsh list --all查看你所有虚拟机的情况

　　　　　　　visrh list 查看正在运行的虚拟机的情况

**![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905223747210-1915341938.png)**
### 　　4）使用vnc-viewer检测系统

　这里需要安装vnc viewer软件的,没有的话可以去我的博客中的随笔分类tools中下载

　　　　　　作用：我们虚拟机已经在安装ubuntu系统了，但是后面有些安装过程中需要认为去操作的，所以我们需要去使用vnc viewer去检测它的安装

　　　　　　url：host_ip:vnc编号（virsh vncdisplay vmname进行查看）

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905224259976-1534414788.png)

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905224313616-829237727.png)

　　　　　　进入这里就是我们熟悉的安装ubuntu的过程了。不知道怎么安装ubuntu的server版的可以去我的博客中的随笔分类tools中下载

　　总结:至此，第一种创建虚拟机的方式就完成了，其实这种创建时非常的繁琐的。但是可以知道它的安装整个过程

## 　　4.2、第二种创建虚拟机的方式：（生成相应的xml文件+qcow2文件=虚拟机）

　　　　　在上面我们已经安装了一个ubuntu系统，所以在us1.qcow2虚拟磁盘中是有ubuntu系统文件的和相对应的xml文件的。

### 1）复制us1.qcow2到us2.qcow2

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905225854163-66385649.png)

### 　2）复制us1.xml到us2.xml

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905225557429-1653492108.png)

### 　　　3）修改us2.xml文件中的四个地方

　a)修改xml文件中的虚拟机名称。<name>us2</name>中的内容。
　　　　　　b)修改UUID，16进制的值，随便修改其中几个值即可。
　　　　　　c)修改新的虚拟机配置中的磁盘（虚拟磁盘文件）信息。
　　　　　　d)修改MAC地址，16进制的值，随便修改其中几个值即可。

### 　4）重启libvirt-bin服务。如果重启该服务有错，请重启宿主机

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905230129179-185497111.png)

　　总结：使用这种安装非常的方便，只需要复制两个文件，修改一个文件就可以了。我这里会提供来两个文件给需要的人，可以去我的博客中的随笔分类tools中下载

## 　　4.3、第三种创建虚拟机的方式：virt-manager

　　　　　　首先你需要去安装这个软件

　　　　　　sudo apt install virt-manager

### 　　　1）终端输入

　　　　　　virt-manager

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905230718616-932788446.png)

### 　　2）file-->add connection

　　　　　　2.1）选中connection remote host

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905230953351-1165148893.png)

　　　　　　 2.2）file-->create new virtual machine

　　　　　　　　第一步：

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231137179-1804941341.png)

　　　　　　　　第二步：

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231259288-206633485.png)

　　　　　　　　第三步：

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231330054-334601282.png)

 　　　　　　　　　　接下来可以自己去尝试我就不详细解释了

# 五、虚拟机管理、虚拟机域管理、虚拟磁盘管理

## 　5.1、虚拟机管理

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231619163-603208817.png)

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905232039601-1034528088.png)



## 　　5.2、虚拟机域管理

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231649538-558762535.png)

## 　　5.3、虚拟磁盘管理

![](http://images2017.cnblogs.com/blog/999804/201709/999804-20170905231718397-93775087.png)




























