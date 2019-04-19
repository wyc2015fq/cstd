# CentOS 7.2  终端安装字体 - Koma Hub - CSDN博客
2019年02月23日 19:49:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：43
[https://blog.csdn.net/qq625281334/article/details/54944537](https://blog.csdn.net/qq625281334/article/details/54944537)
配置文件/etc/fonts/fonts.conf
自定义的在/etc/fonts/local.conf
主要的ttf文件存储在/usr/share/fonts,可以把字体从windows传到linux字体目录下。
[https://www.cnblogs.com/hughhuang/p/7831899.html](https://www.cnblogs.com/hughhuang/p/7831899.html)
字体默认存放路径：/usr/share/fonts
1. 在/usr/share/fonts目录下建立一个子目录：　　
`1 [root@localhost fonts]# mkdir inconsolata `
2. 将需要安装的字体文件复制到步骤1中建立的目录中：
`1 [root@localhost inconsolata]#cp -r /media/sf_Shared/inconsolata.zip ./`
3. 解压到当前目录：
`1 [root@localhost inconsolata]# unzip inconsolata.zip`
4. 建立字体索引信息，并更新字体缓存：
```
1 [root@localhost inconsolata]# mkfontscale
2 [root@localhost inconsolata]# mkfontdir
3 [root@localhost inconsolata]# fc-cache
```
5. 至此，字体已经安装完成
6. 查看系统中安装的中文字体：
`1 [root@localhost inconsolata]# fc-list`
