# mkcramfs 命令学习 - maopig的专栏 - CSDN博客
2012年03月10日 16:56:13[maopig](https://me.csdn.net/maopig)阅读数：3198
mkcramfs :创建只读文件系统
语 法
mkcramfs[必要参数][选择参数][源目录][目标文件]功 能mkcramfs 命令：用来创建CRAMFS只读文件系统 
类似命令: fdisk  cramfsck  mount    
执行权限: 超级用户 普通用户 
命令属性: 磁盘维护 
参数必要参数
-e 设置文件系统版本号
-i 将文件映像插入文件系统
-n 设定设定文件系统名字
选择参数
-h 显示帮助信息
范例
范例1：创建 cramfs 文件系统
[root@vm-dev lX138.coM]# mkcramfs Qtopia/ Qtopia.carmfs
//将 Qtopia 目录 创建为 cramfs文件系统
范例2：挂载文件系统
[root@vm-dev lX138.coM]# mount -o loop /home/rootfs.cramfs /media/cm
