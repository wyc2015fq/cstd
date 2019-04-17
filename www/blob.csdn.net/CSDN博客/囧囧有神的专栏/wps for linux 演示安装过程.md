# wps for linux 演示安装过程 - 囧囧有神的专栏 - CSDN博客





2012年03月28日 22:52:44[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：13584








                
首先说一下我的环境

发行版本：ubuntu 12.04   

内核版本：3.2.0 

桌面环境：gnome 

机器是x86 32位架构滴


下载wps的deb包后，当然迫不及待的进入命令行界面安装啦

1.进入deb包所在路径

2.执行命令 sudo dpkg -i wps-office_8.1.0.3117+wpp~a1-0.1_i386.deb

结果自然要报错了，ubuntu缺少一些东东

错误信息：dpkg：依赖关系问题使得 wps-office 的配置工作不能继续：

wps-office 依赖于 ttf-mscorefonts-installer；然而：

  未安装软件包 ttf-mscorefonts-installer。

3.安装 ttf-mscorefonts-installer ，执行命令 sudo apt-get install ttf-mscorefonts-installer

（还可以从[http://packages.debian.org/lenny ... -installer/download](http://packages.debian.org/lenny/all/ttf-mscorefonts-installer/download)下载安装包，然后执行sudo dpkg -i ttf-mscorefonts-installer_2.7_all.deb进行安装）

结果还是报错，这次缺少的是cabextract

缺铁补铁，缺锌补锌，缺cabextract就补cabextract

4.安装cabextract，执行sudo apt-get install cabextract，这次安装很顺利，没缺啥东西了。

5.然后当然是重新执行 sudo apt-get install ttf-mscorefonts-installer 和sudo dpkg -i wps-office_8.1.0.3117+wpp~a1-0.1_i386.deb这两条命令了。这次没出意外，大功告成。

装完后再好好用用～




上图
![](https://img-my.csdn.net/uploads/201203/28/1332946395_6332.png)



