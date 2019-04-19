# Starting Reload cups, upon starting avahi-daemon to make sure remote queues are populated [fail] - xqhrs232的专栏 - CSDN博客
2016年06月17日 14:04:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1796
相关文章
1、ubuntu 13.10下安装ATI显卡驱动----[http://www.ithao123.cn/content-780086.html](http://www.ithao123.cn/content-780086.html)
       这几天在折腾QT，本来都编译通过没问题了，但为了libqtdbus.so这个库文件，又去折腾了不少东西，什么dbus,QT的重新配置编译什么的。libqtdbus.so这个库文件也折腾出来了，但后来把ubuntu折腾成开机/重启黑屏了，这里记录一下。
      1、ubuntu黑屏死机的log--------/var/log/boot.log
  * Starting Reload cups, upon starting avahi-daemon to make sure remote queues are populated[74G[ OK ]
 * Starting SystemD login management service[74G[ OK ]
 * Starting Reload cups, upon starting avahi-daemon to make sure remote queues are populated[74G[[31mfail[39;49m]
   其中* Starting Reload cups, upon starting avahi-daemon to make sure
 remote queues are populated [fail]这一条特刺眼
   2、后来在网上找了个老半天，查到可能跟/etc/init/cups.conf文件有关
start on (filesystem
          and (started dbus or runlevel [2345]))
stop on runlevel [016]
上面的一段是/etc/init/cups.conf文件的内容，看到dbus觉得很奇怪，会不会跟自己装的一个开源的dbus库有关？
 
 3、ubuntu启动的时候，按ESC键，进入启动菜单，按E键进入启动配置界面，将quiet修改为text,按F10键进入命令行模式。然后转到dbus目录下执行make uninstall命令把装的dbus卸载掉。
4、按第三步的方法，重启系统就一切正常了，不知道是不是跟安装的dbus版本有关！！！这里记录一下了！！！
