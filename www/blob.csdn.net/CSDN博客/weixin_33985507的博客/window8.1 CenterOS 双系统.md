# window8.1 CenterOS 双系统 - weixin_33985507的博客 - CSDN博客
2017年06月24日 20:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
window8.1 CenterOS 双系统
学习了：
http://blog.csdn.net/ac_hell/article/details/53436890
https://jingyan.baidu.com/article/1709ad80b28cf74634c4f0d5.html
整理：
1，制作启动U盘的时候
UltraISO软件，注意选择写入方式：USB-HDD，默认会是USB-HDD+；自己机器支持的方式可能只是USB-HDD，自己机器支持的方式要在BIOS中查看；
1，install CentOs
vmlinuz initrd=initrd.img inst.stage2=hd:LABEL=CentOS/...
修改为：
vmlinuz initrd=initrd.img Linux dd quiet 
查看明确为：
vmlinuz initrd=initrd.img inst.stage2=hd:/dev/sdb1 quiet
这里的sdb1是指的U盘的名字；
2，start os
$ su root
# vim /boot/grub2/grub.cfg
menuentry 'Win8.1'{
  set root='(hd0,msdos1)'
  chainloader+1
} 
确定hd0,msdos1，重启按c，cat (hd0, 输入Tab键
我这边的情况，安装完之后，系统的引导中已经增加了win8的引导；
而且grub.cfg文件的位置为：
/boot/efi/EFI/centos/grub.cfg 
后继：
1，重新安装了，安装的时候要选择安装的类型，默认是最小化安装，没有图形界面；
2，在图形界面内，点击设置，系统死机，后来就没有啦；
3，又重装了，还是系统死机，而且连windows也没有引导了，进不去啦；
4，U盘启动失效了；
总结：哈哈哈，不装就不会死；
1，在U盘启动的情况下，删了几个小分区，然后及时重装windows也无法正常启动了；后来把硬盘格式化了，
重做系统，可以引导了；
2，win8启动情况下，直接重启不能进入Centos的U盘启动，需要关机然后再启动；
3，在U盘启动界面，直接按回车就进行安装了；
failed to find a suitable stage1 device问题：
X 在主分区进行安装
X 格式化为ext3格式
X 全部安装CentOS系统，还是会出现死机情况；
后来改为Ubuntu啦
