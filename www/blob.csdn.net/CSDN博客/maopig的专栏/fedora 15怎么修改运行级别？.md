# fedora 15怎么修改运行级别？ - maopig的专栏 - CSDN博客
2011年06月30日 16:46:00[maopig](https://me.csdn.net/maopig)阅读数：1747
inittab改了已经在fedora15中，你vim它就可以看到更改说明，就是说都改到/etc/systemd/system/default.target这里了，就是缺省的设置。如果你要改变缺省值就把对应的runlevel移动过去覆盖了。
To 3 字符
[root@15 system]# rm -rf /etc/systemd/system/default.target
[root@15 system]# ln -s /lib/systemd/system/runlevel3.target /etc/systemd/system/default.target
[root@15 system]# reboot
To 5 图形
[root@15 system]# rm -rf /etc/systemd/system/default.target
[root@15 system]# ln -s /lib/systemd/system/runlevel5.target /etc/systemd/system/default.target
[root@15 system]# reboot
