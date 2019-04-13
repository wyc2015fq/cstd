
# linux命令详解（19）修改文件属性 lsattr、chattr命令详解 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月16日 15:01:48[Jlins](https://me.csdn.net/dyllove98)阅读数：2693


修改文件属性
Linux文件系统里的文件和目录全都关联用户、用户组和其他用户的读、写和执行权限。此外，还可能存在其他特定文件系统类型才有的文件和目录相关属性。
ext2和ext3文件系统文件可以选用一些特殊属性。使用lsattr命令可以列出这些属性。大部分属性都晦涩难懂，默认并不启用。下面的示例使用lsattr查看部分文件属性：
|1
|2
|3
|4
|5
|6
|\# lsattr /etc/host*
|------------- /etc/host.conf
|------------- /etc/hosts
|------------- /etc/host.allow
|------------- /etc/host.deny
|$ lsattr -aR /tmp/ | less       递归列出/tmp目录及其内容的属性
|

上面的短划线代表13个可设置的ext2/ext3属性，默认情况下一个都不设置。这些属性具体说明如下：a（仅追加）、c（压缩）、d（不转储）、i（不可更改）、j（数据日志）、s（安全删除）、t（无尾部合并）、u（不可删除）、A（不更新访问时间atime）、D（同步目录更新）、S（同步更新）和T（目录层级结构顶部）。使用chattr命令可以修改上述属性，示例如下：
|1
|2
|3
|4
|5
|6
|7
|8
|\# chattr +i /boot/grub/grub.conf
|$ chattr +A -R /home/francois/images/*
|$ chattr +d FC6-livecd.iso
|$ lsattr /boot/grub/grub.conf /home/francois/images/* FC6-livecd.iso
|----i-------- /boot/grub/grub.conf
|-------A----- /home/francois/images/einstein.jpg
|-------A----- /home/francois/images/goth.jpg
|------d------ FC6-livecd.iso
|

如上所示，设置+i选项后，grub.conf文件便无法更改，即无法删除、重命名或修改该文件，也不能为之创建链接。这样可以防止随意更改grub.conf文件，设置i属性后，即使root用户也无法修改这个文件。
上面示例中的-R选项会递归设置+A选项，此后images目录及其子目录内所有文件的访问时间（atime记录）都不再变动。设置A属性，能在笔记本电脑或闪存驱动器上节省一些磁盘I/O。若使用dump命令来备份ext2/ext3文件系统，+d选项能阻止备份选定的文件。这个例子选择不备份大尺寸的ISO映像。
使用chattr和减号（-）可以移除属性，例如：
\# chattr -i /boot/grub/grub.conf
注意 成功侵入计算机的骇客往往会将某些系统二进制文件（如ls或ps）替换成有漏洞的版本，使之不可更改。建议不定期的检查系统（如位于/bin、/usr/bin、/sbin和/usr/sbin的）可执行文件的属性集。
上面运行的file命令表明，bash shell提供了多种格式的文档文件。file命令会查看文件内容，确定文件是否包含troff标记（

原文地址：http://www.itmmd.com/201412/315.html
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

