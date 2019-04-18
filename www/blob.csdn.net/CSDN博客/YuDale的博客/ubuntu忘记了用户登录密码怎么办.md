# ubuntu忘记了用户登录密码怎么办 - YuDale的博客 - CSDN博客
2017年03月24日 11:52:07[YuDale](https://me.csdn.net/YuDale)阅读数：367
因为胡乱的对/etc/passwd和/etc/shadow操作，导致用户登录密码丢失而登录不了ubuntu。
所以在网上整理了下面方法。
1.重启ubuntu，在重启过程中按住shift键。进入下面界面![](https://img-blog.csdn.net/20170324112001031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.选择Adanced optiongs for Ubuntu
3.选择如图
![](https://img-blog.csdn.net/20170324113507110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.选择如图
![](https://img-blog.csdn.net/20170324113656368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.输入命令：passwd tes   (test 是用户名)
输入新密码
再次输入密码
6.如果看到password updated successfully则说明成功，可以重新启动使用新密码登录
7.如果出现authentication token manipulation error,password unchanged
出现这个问题有很多种情况，例如权限，内存分区不足，或者需要修改文件操作什么的。
这里解决最简单的一种情况，权限问题。
8.输入命令：  mount -no remount,rw /
9.执行步骤4
10.sudo reboot
11.搞定收工
