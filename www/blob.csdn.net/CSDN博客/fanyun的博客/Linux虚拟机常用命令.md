# Linux虚拟机常用命令 - fanyun的博客 - CSDN博客
2016年04月07日 10:29:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：10922
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
一.Linux虚拟机常用命令
# virsh list                     //查看已打开虚拟机列表
# virsh list --all                //查看所有虚拟机列表
# virsh version                //查看virsh版本号
# virsh start node1            //启动node1虚拟机
# virsh shutdown node1      //关机node1虚拟机
# virsh destroy node1         //强制关机node1虚拟机
# virsh dumpxml node1 > node1.xml //导出node1虚拟机配置文件
# virsh undefine node1       //取消node1定义
# virsh define node1.xml     //重新定义node1
# virsh autostart node1       //设置开机自启动node1
# virt-clone -o node1 -n node1-clone-f  /data/images/node1-clone.img //克隆虚拟机
使用命令安装新的虚拟机：可根据需要调整选项
virt-install \
--name node1 \
--noautoconsole \
--ram 512 \
--arch=x86_64 \
--vcpus=1 \
--os-type=linux \
--os-variant=rhel6 \
--hvm \
--accelerate \
--disk path=/data/images/node1.img \
--network bridge=br0 \
--locationnfs:192.168.100.1:/var/ftp/pub/iso/[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10)/6.4 \
--extra-args="ks=http://192.168.100.1/rhel-ks.cfg ip=192.168.100.10 netmask=255.255.255.0 gateway=192.168.100.254 dns=192.168.100.2 noipv6"
二.使用LVM方式管理虚拟主机磁盘
1.创建LV
# fdisk -l | grep /dev/sda6                 //创建分区
/dev/sda6           6170      39163  265015296  8e  Linux LVM
PV  --> VG --> LV
# pvcreate /dev/sda6                       //创建PV
# vgcreate vg_data /dev/sda6               //创建VG
# lvcreate -L 10G -n lv_kvm_node1vg_data      //创建LV
 2.使用创建的LV安装Guest
# virt-install \
--name kvm_node1 \
--noautoconsole \
--ram 1024 \
--arch=x86_64 \
--vcpus=1 \
--os-type=linux \
--os-variant=rhel6 \
--hvm \
--accelerate \
--disk path=/dev/vg_data/lv_kvm_node1\              //安装在刚创建的LV中
--network bridge=br0 \
--locationnfs:192.168.100.1:/var/ftp/pub/iso/RedHat/6.4 \
--extra-args="ks=http://192.168.100.1/rhel-ks.cfg ip=192.168.100.10 netmask=255.255.255.0 gateway=192.168.100.254 dns=192.168.100.2 noipv6"
 3.设置模板虚拟机，去掉一些个性信息（在刚装好的虚拟机kvm_node1上操作）
# touch  /.unconfigured
 4.对已安装好lv_kvm_node1的生成快照(快照大小只要为被快照的逻辑卷的15~20%就可以了)
# lvcreate -s -n kvm_snap1 -L 2G /dev/vg_data/lv_kvm_node1
 5.将快照定义到virt-manager
# vim /etc/libvirt/qemu/kvm_node1.xml       //默认配置文件位置
# virsh dumpxml kvm_node1 > /root/kvm_snap1.xml  //也可导出配置文件
修改配置文件kvm_snap1.xml中名字,UUID,磁盘位置，mac地址
# virsh define /root/kvm_snap1.xml 这样就能使用快照的虚拟机做实验，当这个快照虚拟坏了再快照一个就能恢复到刚安装好的状态
