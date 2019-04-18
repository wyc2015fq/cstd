# VMware的Linux虚拟如何扩展磁盘容量(CentOS 7) - alw2009的博客 - CSDN博客
2018年12月31日 23:25:11[traveler_zero](https://me.csdn.net/alw2009)阅读数：52
这是我目前的配置， 总的磁盘空间是30G， 想扩展到40G， 扩展到根目录所在的分区
参考了：[https://www.youtube.com/watch?v=KW1ScgdCIfs](https://www.youtube.com/watch?v=KW1ScgdCIfs)
![](https://img-blog.csdnimg.cn/20181231215649432.png)
**目录**
[一、通过VMWare配置项虚拟可用磁盘](#%E4%B8%80%E3%80%81%E9%80%9A%E8%BF%87VMWare%E9%85%8D%E7%BD%AE%E9%A1%B9%E6%89%A9%E5%B1%95%E5%8F%AF%E7%94%A8%E7%A3%81%E7%9B%98)
[二、 在Linux系统里扩展磁盘](#%E4%BA%8C%E3%80%81%20%E5%9C%A8Linux%E7%B3%BB%E7%BB%9F%E9%87%8C%E6%89%A9%E5%B1%95%E7%A3%81%E7%9B%98)
### 一、通过VMWare配置项扩展虚拟磁盘
1.1 关闭虚拟机
1.2 增大虚拟磁盘
![](https://img-blog.csdnimg.cn/20181231220754596.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181231220830767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181231220911877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/2018123122104747.png)
 配置好之后， 开启虚拟机
### 二、 在Linux系统里扩展磁盘
用root用户登录虚拟， 先查看一下磁盘
![](https://img-blog.csdnimg.cn/20181231221235621.png)
2.1 重新修改分区表
如果不明白没关系， 按照步骤来就可以了
`fdisk /dev/sda`
发送p, 打印当前分区信息。将输出信息拷贝到一个地方
发送n, 发送p, 发送3（默认）
分区起始位置和结束位置默认
![](https://img-blog.csdnimg.cn/2018123122495763.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
发送t, 然后3， 然后8e, 将第第二分区的类型设置为LVM
![](https://img-blog.csdnimg.cn/20181231225158105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
发送p, 确认分区信息
发送w, 写分区表
重启系统， 让新的分区表生效。
启动系统后， 执行fsdis -l 检查分区
![](https://img-blog.csdnimg.cn/20181231225417870.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
2.2 将新分区转换成物理卷
`pvcreate /dev/sda3`
 2.3 将新创建的物理卷添加的VG
![](https://img-blog.csdnimg.cn/2018123123040097.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
`vgextend centos /dev/sda3`
![](https://img-blog.csdnimg.cn/20181231231404569.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
```
lvextend -L+10230M /dev/centos/root
resize2fs /dev/centos/root
xfs_growfs /dev/centos/root
```
2.4 大功告成， 检查一下
`df -h`
