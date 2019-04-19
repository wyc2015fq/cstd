# linux下自动挂载硬盘 - sxf_123456的博客 - CSDN博客
2017年11月28日 13:27:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3277
                
echo "\n\n开始创建/dev/xvde\n\n"
fdisk /dev/xvde << ESXU
n 
p
1
p
t
8e
p
wq
ESXU
echo "\n\n创建/dev/xvde分区完成\n\n"
sleep 1s
echo "\n\n开始创建逻辑卷组，逻辑卷\n\n"
pvcreate /dev/xvde1  #创建物理卷
vgcreate VolGroup_data /dev/xvde1 #创建卷组
vgchange -ay VolGroup_data 
lvcreate -n lvData -l 100%FREE VolGroup_data
echo "\n\n格式化逻辑卷\n\n"
mkfs.ext4 /dev/VolGroup_data/lvData
sleep 1s
echo "\n\n开始创建/dev/xvdf\n\n"
fdisk /dev/xvdf << ESXU
n 
p
1
p
t
8e
p
wq
ESXU
echo "\n\n创建/dev/xvdf分区完成\n\n"
echo "\n\n将第二块硬盘添加到第一块创建的卷组中\n\n"
pvcreate /dev/xvdf1
vgextend VolGroup_data /dev/xvdf1
lvextend /dev/mapper/VolGroup_data-lvData -l +100%FREE /dev/xvdf1
resize2fs /dev/VolGroup_data/lvData
echo "\n\n挂载硬盘\n\n"
mkdir /data
echo '/dev/VolGroup_data/lvData     /data            ext4    defaults        0 0' >> /etc/fstab
mount -a
echo "\n\n挂载硬盘完成,使用df -h 命令查看挂载情况\n\n"
参考：
    http://blog.51yip.com/linux/1778.html
    https://www.centos.bz/2017/07/centos-lvm-expand/
fdisk -l
查看有哪些硬盘需要挂载
记录下要挂载的盘，例如xvde,xvdf
1.创建lvm分区
    fdisk /dev/xvde    //按照网页输入命令
    这一部分见http://blog.51yip.com/linux/1778.html
2.创建逻辑卷组，逻辑卷
    pvcreate /dev/xvde1  //创建物理卷
    vgcreate VolGroup_data /dev/xvde1 //创建逻辑卷组
    vgchange -ay VolGroup_data //激活
    lvcreate -n lvData -l 100%FREE VolGroup_data //创建逻辑卷
3. 格式化逻辑卷
    mkfs.ext4 /dev/VolGroup_data/lvData
    如果有第二块盘，进入第4步，否则进入第5步
4. 挂载第二块
    4.1 创建lvm分区，参看1.
        fdisk /dev/xvdf
    4.2 创建物理卷
        pvcreate /dev/xvdf1
    4.3 将新硬盘加入卷组VolGroup_data
        vgextend VolGroup_data /dev/xvdf1
    4.4 合并
        lvextend /dev/mapper/VolGroup_data-lvData -l +100%FREE /dev/xvdf1
    4.5 重新设置大小
        resize2fs /dev/VolGroup_data/lvData
5. 挂载
    mkdir /data
    echo '/dev/VolGroup_data/lvData     /data            ext4    defaults        0 0' >> /etc/fstab
    mount -a    //挂载
    df    //查看是否挂载成功
