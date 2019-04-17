# LNMP一键安装包如何重装Nginx - 在思索中前行！ - CSDN博客





2014年09月25日 14:02:46[_Tham](https://me.csdn.net/txl16211)阅读数：4350








      LNMP一键安装包安装好后，相应的Mysql，Nginx及PHP都会安装配置完成。

      由于某些特殊情况的需要，如何更换Nginx的版本呢？

nginx升级脚本可以完成。

1、 手动编译方法：/usr/local/nginx删了再重新进入./lnmp0.8-full/nginx/1.0.10进行make
 install也可以达到重装效果

        注意保存Nginx的配置文件

可以再重新make install
      删除nginx目录前备份配置文件
      make install后需要重新复制备份文件

2、发布的脚本就是专门用来[升级Nginx](http://www.vpser.net/manage/nginx-upgrade.html)，可以升级Nginx至任意官方已发布的Nginx版本。



 执行：**wget soft.vpser.net/lnmp/upgrade_nginx.sh;sh upgrade_nginx.sh**

然后按提示输入要升级的Nginx版本号，Nginx的版本号可以从[http://nginx.org/en/download.html](http://nginx.org/en/download.html)查询。输入版本号后回车，再次回车确认即可开始安装，如果不出意外就会升级成功，如果出现问题可以到[http://bbs.vpser.net](http://bbs.vpser.net/)[lnmp专区](http://bbs.vpser.net/forum-25-1.html)发帖求助。

![](http://vpser.b0.upaiyun.com/uploads/2011/02/upgrade_nginx.jpg)



