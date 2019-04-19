# Linux基础(二)centOS7密码重置 - _天枢 - 博客园
## [Linux基础(二)centOS7密码重置](https://www.cnblogs.com/yhleng/p/8390624.html)
2018-01-31 09:33 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8390624)
之前安装linux的时候,为了安全起见,起了一个非常特别的,长的密码.然后,就不记得了密码.
下面通过进入单用户模式,就行挽救.   
1>重启系统,在系统菜单选择页按 [上下方向键],使界面停在该界面
2>在该界面通过方向键,选择[你使有的系统项],按键盘e键,进入编辑状态
3>在编辑界面,按向下方向键,找到linux16开头的行,找到 ro ,将ro改成rw init=/sysroot/bin/sh
     按Ctrl+x进入,shell单用户模式.
4>输入命令,chroot /sysroot/  来更改根目录为sysroot
之后,就可以执行passwd命令来修改密码了.
同上,如果一些其它配置改错了,导致进不去系统了,也可以通过以上操作改回来.
       首先,重启centOS.
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180131092810687-594318676.png)
重启,按方向键,上或者下,停在系统选择列表,动作要快点,否则5秒就会开始自动进入启动页面了。
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180131092912437-313718406.png)
然后,到这个页面,输入【e】，进入编辑页面。
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180131093032812-1596970412.png)
然后，在这个页面，找到linux16这一行，将之前的【ro】，改为【rw init=sysroot/bin/sh】，然后按照指示，按【ctrl+x】，使用单用户模式启动.由于是单用户,就不需要密码进入了.
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180131093200968-1066554556.png)
  首先输入命令【chroot /sysroot】,chroot就是change root，也就是改变程序执行时所参考的根目录位置,根目录改为/sysroot。然后输入命令【passwd root】,passwd表示更改使用者的密码,注意这里不是password,而是passwd.
       最后输入命令【touch /.autorelabel】 ,在/目录下创建一个.autorelabel文件，而有这个文件存在，系统在重启时就会对整个文件系统进行relabeling。以及命令【exit】退出，以及命令【reboot】重启。
等待5分钟，重启成功，就可以输入用户名和新密码，重新登录了。
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180131093245593-2040430813.png)
