# Bleachbit--Clean Your System and Free Disk Space - weixin_33985507的博客 - CSDN博客
2017年11月22日 21:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
一款清理linux电脑垃圾的小软件
Bleachbit--Clean Your System and Free Disk Space
#### 去官方网站下载 deb 包
[点我去 Bleachbit 官网](https://link.jianshu.com?t=https%3A%2F%2Fwww.bleachbit.org%2F)
#### dpkg 安装
```
album@Lenovo:~/Downloads$ sudo dpkg -i bleachbit_1.12_all_ubuntu1604.deb
```
#### 提示有依赖问题
```
album@Lenovo:~/Downloads$ sudo apt -f install
```
#### 继续之前的安装
```
album@Lenovo:~/Downloads$ sudo dpkg -i bleachbit_1.12_all_ubuntu1604.deb 
(正在读取数据库 ... 系统当前共安装有 210766 个文件和目录。)
正准备解包 bleachbit_1.12_all_ubuntu1604.deb  ...
正在将 bleachbit (1.12) 解包到 (1.12) 上 ...
正在设置 bleachbit (1.12) ...
正在处理用于 bamfdaemon (0.5.3+17.04.20170406-0ubuntu1) 的触发器 ...
Rebuilding /usr/share/applications/bamf-2.index...
正在处理用于 gnome-menus (3.13.3-6ubuntu5) 的触发器 ...
正在处理用于 desktop-file-utils (0.23-1ubuntu2) 的触发器 ...
正在处理用于 mime-support (3.60ubuntu1) 的触发器 ...
```
#### ok,运行
![3942909-8cc57a906c74bb84.png](https://upload-images.jianshu.io/upload_images/3942909-8cc57a906c74bb84.png)
image.png
