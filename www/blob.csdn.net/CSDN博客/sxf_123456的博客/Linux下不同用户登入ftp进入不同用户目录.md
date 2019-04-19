# Linux下不同用户登入ftp进入不同用户目录 - sxf_123456的博客 - CSDN博客
2018年05月12日 19:49:49[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：897
                当user1登入ftp   则该用户登入的目录是/a/b/user1的目录下 ，当user2登入ftp 则该用户登入的目录是 /a/b/user2  
你用的是vsftpd吧，缺省设置就是用户登录后，进入自己的home目录。
如果想限制用户只能在其home目录下，不能改变路径，修改/etc/vsftpd/vsftpd.conf文件，去掉下面几行的注释：
chroot_list_enable=YES
(default follows)
chroot_list_file=/etc/vsftpd.chroot_list
并建立/etc/vsftpd.chroot_list文件，将你想限制的用户名加入这个文件就行了。            
