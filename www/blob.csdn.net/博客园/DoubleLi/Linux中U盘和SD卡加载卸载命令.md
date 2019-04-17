# Linux中U盘和SD卡加载卸载命令 - DoubleLi - 博客园






U盘挂载命令
U盘插入的时候会显示启动信息，启动信息中sda: sda1指U盘的设备名为sda1
dev设备目录下有一个sda1设备文件，此设备文件就是我们插入的U盘，我们将这个设备文件挂载到Linux系统的/mnt/udisk1这个目录下，这样我们即可以通过访问/mnt/udisk1目录来读写U盘，挂载用mount命令
[root@TX2440A /]# mount /dev/sda1 /mnt/udisk1
精简写法为mntud1，功能与mount /dev/sda1 /mnt/udisk1相同
[root@TX2440A /]# mntud1
进入到/mnt/udisk1目录下查看U盘中的内容，如果内容与U盘中的内容相同则说明U盘挂载成功
[root@TX2440A /]# ls /mnt/udisk1


取消U盘挂载命令
取消挂载用umount命令，加上要取消挂载的目录/mnt/udisk1
[root@TX2440A /]# umount /mnt/udisk1
再查看/mnt/udisk1目录下发现已经没有内容了则说明取消U盘挂载成功
[root@TX2440A /]# ls /mnt/udisk1
注：上面的/udisk1目录可以是/mnt目录下的/nfs、/sd、/udisk1、/udisk2、/udisk3、/udisk4子目录中的任意一个，为了便于识别和不引起和别的存储器发送冲突，最好还是使用/udisk标识的目录


SD卡挂载命令
SD卡插入的时候会显示启动信息，启动信息中mmcblk0: p1指SD卡的设备名为p1
dev设备目录下有一个mmcblk0p1设备文件，此设备文件就是我们插入的SD卡，我们将这个设备文件挂载到Linux系统的/mnt/sd这个目录下，这样我们即可以通过访问/mnt/sd目录来读写SD卡，挂载用mount命令
[root@TX2440A /]# mount /dev/mmcblk0p1 /mnt/sd  
精简写法为mntsd，功能与mount /dev/mmcblk0p1 /mnt/sd 相同
[root@TX2440A /]# mntsd
进入到/mnt/sd目录下查看sd卡中的内容，如果内容与SD卡中的内容相同则说明SD卡挂载成功    
[root@TX2440A /]# ls /mnt/sd


取消SD卡挂载命令
取消挂载用umount命令，加上要取消挂载的目录/mnt/sd
[root@TX2440A /]# umount /mnt/sd
再查看/mnt/sd目录下发现已经没有内容了则说明取消SD卡挂载成功
[root@TX2440A /]# ls /mnt/sd
注：上面的/sd目录可以是/mnt目录下的/nfs、/sd、/udisk1、/udisk2、/udisk3、/udisk4子目录中的任意一个，为了便于识别和不引起和别的存储器发送冲突，最好还是使用/sd标识的目录









