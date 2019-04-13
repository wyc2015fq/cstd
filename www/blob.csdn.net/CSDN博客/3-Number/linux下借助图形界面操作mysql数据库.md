
# linux下借助图形界面操作mysql数据库 - 3-Number - CSDN博客


置顶2015年06月02日 10:28:09[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：4619


**0x01缘起**
最近总是觉得自己碌碌无为，于是想利用一些开源的东西做一个属于自己的应用，也不枉做一会程序员。
在linux平台下用命令行操作数据库时一件比较痛苦的事，于是想利用一下图形界面，提供开发效率。在网上找了一些工具，大部分是商业的数据库。由于在windows平台也利用过Navicat，所以就选择了它。
**0x02环境**
OS:ubuntu
软件：navicat8_mysql_cs.tar.gz（http://www.navicat.com.cn/download 进行下载）
0x03安装步骤
(1)tar xzvf  navicat8_mysql_cs.tar.gz
(2)cd navicat8_mysql_cs
(3)先尝试运行./start_navicat，如果程序界面没有出来，则继续一下步骤。
(4)sudo apt-get install update
(5)sudo apt-get install wine
(6)vim start_navicat 查找winboot，改成wineboot，然后保存。
(7)./start_navicat
0x04编码
如果连接数据库后，显示的中文字符为乱码，这个时候就得检查操作系统支持的字符集。
locale -a，看试用支持中文字符（zh_CN.utf8），若不支持，export LANG=zh_CN.utf8
0x05破解
因为商业版只能利用30天左右，个人要用用于非商业开发，可以用如下办法试用。方法如下：
第一次执行start_navicat时，会在用户主目录下生成一个名为.navicat的隐藏文件夹。此文件夹下有一个system.reg文件。删除这个文件，又可以试用30天。
如有错误和不足，希望各位看客指正。


