# Linux下挂载与解除挂载U盘 - h13 - 博客园
1、使用命令：fdisk -l，查看系统硬盘和分区情况
2、插入优盘，再次用fdisk -l命令查看
[](http://blog.photo.sina.com.cn/showpic.html#url=http://static15.photo.sina.com.cn/orignal/6118d6b7t7e1e5607e5de&690)
    大家可以发现多了一个硬盘/dev/sdb和它的一个分区/dev/sdb1
3、使用命令：mkdir -p /mnt/usb，在mnt目录下创建目录usb来作挂接点
4、进行挂载，使用命令：mount -o iocharset=cp936 /dev/sdb1 /mnt/usb
     其中，-o iocharset是设定字符集，保证能够正常显示中文
5、已成功挂载，可以使用命令：ls /mnt/usb进行查看。挂载移动硬盘和优盘完全一样
6、解除挂载，目的是避免损坏或丢失数据，使用命令:umount /dev/sdb1
