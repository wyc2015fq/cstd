# linux下FAT32格式u盘只读的问题及解决方法 - DoubleLi - 博客园






以下是网上看到的解决办法：http://blog.csdn.net/heqiuya/article/details/7870554





**[plain]**[view plain](http://blog.csdn.net/czh52911/article/details/9101777)[copy](http://blog.csdn.net/czh52911/article/details/9101777)



- 其实是掉电保护,之前挂在的SD变成了制度文件,只需要将SD卡重新挂载一遍就OK了.  
-     FAT: Filesystem panic (dev mmcblk0p4)  
-         fat_free_clusters: deleting FAT entry beyond EOF  
-         File system has been set read-only  
-     JFFS2 notice: (410) check_node_data: wrong data CRC in data node at 0x0a6537b8: read 0x989975b, calculated 0x9b01bd19.  
- 
-     你可以用mount查看  
-     /dev/mmcblk0p4 on /mnt/sd type vfat (ro,relatime,fmask=0000,dmask=0000,allow_utime=0022,codepage=cp437,iocharset=cp936)  
-     /dev/mmcblk0p4 on /home/sd type vfat (ro,relatime,fmask=0000,dmask=0000,allow_utime=0022,codepage=cp437,iocharset=cp936)  
- 
- 
-     在/etc/init.d/rcS中添加一句  
-     mount -o remount,rw /dev/mmcblk0p4  




网上说的另一种方法，把fat32格式改为ext2文件系统格式：

我尝试用上面的方法来解决我的问题，但是又遇到了问题

我通过dmesg命令查看到u盘错误信息，





**[plain]**[view plain](http://blog.csdn.net/czh52911/article/details/9101777)[copy](http://blog.csdn.net/czh52911/article/details/9101777)



- FAT: Filesystem panic (dev sda1)  
- fat_free_clusters: deleting FAT entry beyond EOF  
- File system has been set read-only  






**[plain]**[view plain](http://blog.csdn.net/czh52911/article/details/9101777)[copy](http://blog.csdn.net/czh52911/article/details/9101777)



- 使用mount命令，打印出挂载的格式为ro即只读模式。  
- 在mount命令中加入remount参数，并修改/etc/fstab文件，挂载成功。  
- mount -o remount,rw /mnt/sd  




**[plain]**[view plain](http://blog.csdn.net/czh52911/article/details/9101777)[copy](http://blog.csdn.net/czh52911/article/details/9101777)



- 没办法只好使用第二种方式，格式化u盘为ext2格式。  
-     首先确认内核是否支持ext2文件系统，我的内核是2.6.24，默认不支持ext2,因此，我在内核菜单项中加入了ext2的支持，采取静态模式，编译后更新内核。  
-     然后格式化u盘  
-     #mkfs.ext2 /dev/sda1  
-     目前测试情况一切正常。  










**[html]**[view plain](http://blog.csdn.net/czh52911/article/details/9101777)[copy](http://blog.csdn.net/czh52911/article/details/9101777)



- 










