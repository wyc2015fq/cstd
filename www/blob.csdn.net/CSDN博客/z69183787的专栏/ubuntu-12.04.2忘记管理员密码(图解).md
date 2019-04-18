# ubuntu-12.04.2忘记管理员密码(图解) - z69183787的专栏 - CSDN博客
2014年05月22日 09:44:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1926
在ubuntu中配置samb的时候不小心把管理员账号“wangqh”的密码给改掉了， 当再次以wangqh用户登录的时候提示密码错误。 
解决方案如下：
1. 重启ubuntu ， 在刚启动时长按Shift,键进入GRUB菜单界面
![](https://img-blog.csdn.net/20130716143058515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3FoamZyZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.  选择recovery mode. 按回车键进入.Recover Menu菜单:
![](https://img-blog.csdn.net/20130716143243875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3FoamZyZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3. 选择root , 按回车, 在下方就会出现root终端:
![](https://img-blog.csdn.net/20130716143703171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3FoamZyZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4. 但是当修改账号wangqh的密码时报错:Authentication token manipulation error .
5. 如果出现该错误则输入:  
root@ubunto:~# mount -no remount,rw /
root@ubunto:~# mount -a
然后再修改用户名. 重启OK.
![](https://img-blog.csdn.net/20130716143849203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3FoamZyZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考博客: [http://xiaoboss.iteye.com/blog/1080263](http://xiaoboss.iteye.com/blog/1080263)
[http://forum.ubuntu.org.cn/viewtopic.php?f=116&t=173647](http://forum.ubuntu.org.cn/viewtopic.php?f=116&t=173647)
