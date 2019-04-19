# Linux CentOS 修改内核引导顺序 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月24日 10:54:32[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：346
**CentOS 7.0 系统更改内核启动顺序**
可以 uname -a查下当前的
由于 CentOS 7 使用 grub2 作为引导程序，所以和 CentOS 6 有所不同，并不是修改 /etc/grub.conf 来修改启动项，需要如下操作：
1. 查看系统内部有多少个内核：
- cat /boot/grub2/grub.cfg
|grep menuentry
 2..配置从默认内核启动，下面命令的内核名称根据系统内部查到的实际名称来替换：
- grub2-set-default
“CentOSLinux(3.10.0-123.9.3.el7.x86_64)7(Core)”
3. 验证是否配置成功：
- grub2-editenv
 list 
**CentOS 6.X 系统更改内核启动顺序**
打开 /etc/grub.conf 查看系统内核的情况，下面的截图是系统存在多个内核的现象:
修改配置步骤：
- 从截图能够看出已经存在两个内核版本，从上往下内核版本依次是 2.6.32-573.18.1.el6.x86_64 和 2.6.32-431.23.3.el6.x86_64。
- 在 grub.conf 文件中决定开机使用哪个内核版本做启动的参数是 default，默认为 0（代表从最新的内核启动，代表的内核版本从上往下依次是 0，1, 2 等）。
- 在这个例子中，如果要选择从旧版内核即系统最开始的内核启动，则把 default 值改为 1 ，然后重启服务器从新的内核进行引导。
