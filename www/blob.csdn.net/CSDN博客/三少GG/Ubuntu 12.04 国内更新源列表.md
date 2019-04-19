# Ubuntu 12.04 国内更新源列表 - 三少GG - CSDN博客
2012年11月14日 15:57:50[三少GG](https://me.csdn.net/scut1135)阅读数：1109
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
## [http://blog.oobeta.com/archives/2012/04/26/2355.html](http://blog.oobeta.com/archives/2012/04/26/2355.html)
## Ubuntu 12.04 国内更新源列表
1、首先备份Ubuntu 11.10源列表
sudo cp /etc/apt/sources.list /etc/apt/sources.list.backup （备份下当前的源列表）
2、修改更新源
sudo gedit /etc/apt/sources.list
#网易源（速度很快）
deb http://mirrors.163.com/ubuntu/ precise main universe restricted multiverse
deb-src http://mirrors.163.com/ubuntu/ precise main universe restricted multiverse
deb http://mirrors.163.com/ubuntu/ precise-security universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ precise-security universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ precise-updates universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ precise-proposed universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ precise-proposed universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ precise-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ precise-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ precise-updates universe main multiverse restricted
**sohu源**
 deb http://mirrors.sohu.com/ubuntu/ precise main restricted
 deb-src http://mirrors.sohu.com/ubuntu/ precise main restricted
 deb http://mirrors.sohu.com/ubuntu/ precise-updates main restricted
 deb-src http://mirrors.sohu.com/ubuntu/ precise-updates main restricted
 deb http://mirrors.sohu.com/ubuntu/ precise universe
 deb-src http://mirrors.sohu.com/ubuntu/ precise universe
 deb http://mirrors.sohu.com/ubuntu/ precise-updates universe
 deb-src http://mirrors.sohu.com/ubuntu/ precise-updates universe
 deb http://mirrors.sohu.com/ubuntu/ precise multiverse
 deb-src http://mirrors.sohu.com/ubuntu/ precise multiverse
 deb http://mirrors.sohu.com/ubuntu/ precise-updates multiverse
 deb-src http://mirrors.sohu.com/ubuntu/ precise-updates multiverse
 deb http://mirrors.sohu.com/ubuntu/ precise-backports main restricted universe multiverse
 deb-src http://mirrors.sohu.com/ubuntu/ precise-backports main restricted universe multiverse
 deb http://mirrors.sohu.com/ubuntu/ precise-security main restricted
 deb-src http://mirrors.sohu.com/ubuntu/ precise-security main restricted
 deb http://mirrors.sohu.com/ubuntu/ precise-security universe
 deb-src http://mirrors.sohu.com/ubuntu/ precise-security universe
 deb http://mirrors.sohu.com/ubuntu/ precise-security multiverse
 deb-src http://mirrors.sohu.com/ubuntu/ precise-security multiverse
 deb http://extras.ubuntu.com/ubuntu precise main
 deb-src http://extras.ubuntu.com/ubuntu precise main
//教育网源 
如果你是在校大学生，可以使用校园网/教育网，就是用教育网的资源吧，中科大，兰大、厦门大学都有很多资源，尤其是支持ipv6的，那更新速度就按兆算了。 
中科大：http://mirrors.ustc.edu.cn/ 
厦门大学：http://mirrors.xmu.edu.cn/howto/ 
#电子科技大学 
deb http://ubuntu.uestc.edu.cn/ubuntu/ precise main restricted universe multiverse 
deb http://ubuntu.uestc.edu.cn/ubuntu/ precise-backports main restricted universe multiverse 
deb http://ubuntu.uestc.edu.cn/ubuntu/ precise-proposed main restricted universe multiverse 
deb http://ubuntu.uestc.edu.cn/ubuntu/ precise-security main restricted universe multiverse 
deb http://ubuntu.uestc.edu.cn/ubuntu/ precise-updates main restricted universe multiverse 
deb-src http://ubuntu.uestc.edu.cn/ubuntu/ precise main restricted universe multiverse 
deb-src http://ubuntu.uestc.edu.cn/ubuntu/ precise-backports main restricted universe multiverse 
deb-src http://ubuntu.uestc.edu.cn/ubuntu/ precise-proposed main restricted universe multiverse 
deb-src http://ubuntu.uestc.edu.cn/ubuntu/ precise-security main restricted universe multiverse 
deb-src http://ubuntu.uestc.edu.cn/ubuntu/ precise-updates main restricted universe multiverse
#中国科技大学 
deb http://debian.ustc.edu.cn/ubuntu/ precise main restricted universe multiverse 
deb http://debian.ustc.edu.cn/ubuntu/ precise-backports restricted universe multiverse 
deb http://debian.ustc.edu.cn/ubuntu/ precise-proposed main restricted universe multiverse 
deb http://debian.ustc.edu.cn/ubuntu/ precise-security main restricted universe multiverse 
deb http://debian.ustc.edu.cn/ubuntu/ precise-updates main restricted universe multiverse 
deb-src http://debian.ustc.edu.cn/ubuntu/ precise main restricted universe multiverse 
deb-src http://debian.ustc.edu.cn/ubuntu/ precise-backports main restricted universe multiverse 
deb-src http://debian.ustc.edu.cn/ubuntu/ precise-proposed main restricted universe multiverse 
deb-src http://debian.ustc.edu.cn/ubuntu/ precise-security main restricted universe multiverse 
deb-src http://debian.ustc.edu.cn/ubuntu/ precise-updates main restricted universe multiverse
#北京理工大学 
deb http://mirror.bjtu.edu.cn/ubuntu/ precise main multiverse restricted universe 
deb http://mirror.bjtu.edu.cn/ubuntu/ precise-backports main multiverse restricted universe 
deb http://mirror.bjtu.edu.cn/ubuntu/ precise-proposed main multiverse restricted universe 
deb http://mirror.bjtu.edu.cn/ubuntu/ precise-security main multiverse restricted universe 
deb http://mirror.bjtu.edu.cn/ubuntu/ precise-updates main multiverse restricted universe 
deb-src http://mirror.bjtu.edu.cn/ubuntu/ precise main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ precise-backports main multiverse restricted universe 
deb-src http://mirror.bjtu.edu.cn/ubuntu/ precise-proposed main multiverse restricted universe 
deb-src http://mirror.bjtu.edu.cn/ubuntu/ precise-security main multiverse restricted universe 
deb-src http://mirror.bjtu.edu.cn/ubuntu/ precise-updates main multiverse restricted universe
#兰州大学 
deb ftp://mirror.lzu.edu.cn/ubuntu/ precise main multiverse restricted universe 
deb ftp://mirror.lzu.edu.cn/ubuntu/ precise-backports main multiverse restricted universe 
deb ftp://mirror.lzu.edu.cn/ubuntu/ precise-proposed main multiverse restricted universe 
deb ftp://mirror.lzu.edu.cn/ubuntu/ precise-security main multiverse restricted universe 
deb ftp://mirror.lzu.edu.cn/ubuntu/ precise-updates main multiverse restricted universe 
deb ftp://mirror.lzu.edu.cn/ubuntu-cn/ precise main multiverse restricted universe
#上海交通大学 
deb http://ftp.sjtu.edu.cn/ubuntu/ precise main multiverse restricted universe 
deb http://ftp.sjtu.edu.cn/ubuntu/ precise-backports main multiverse restricted universe 
deb http://ftp.sjtu.edu.cn/ubuntu/ precise-proposed main multiverse restricted universe 
deb http://ftp.sjtu.edu.cn/ubuntu/ precise-security main multiverse restricted universe 
deb http://ftp.sjtu.edu.cn/ubuntu/ precise-updates main multiverse restricted universe 
deb http://ftp.sjtu.edu.cn/ubuntu-cn/ precise main multiverse restricted universe 
deb-src http://ftp.sjtu.edu.cn/ubuntu/ precise main multiverse restricted universe 
deb-src http://ftp.sjtu.edu.cn/ubuntu/ precise-backports main multiverse restricted universe 
deb-src http://ftp.sjtu.edu.cn/ubuntu/ precise-proposed main multiverse restricted universe 
deb-src http://ftp.sjtu.edu.cn/ubuntu/ precise-security main multiverse restricted universe 
deb-src http://ftp.sjtu.edu.cn/ubuntu/ precise-updates main multiverse restricted universe
3、保存后就是
sudo apt-get update
sudo apt-get upgrade
经过6个月的开发周期，ubuntu 12.04 LTS (Precise Pangolin)终于发布了，这个版本是一个长期支持版本。官方正式版下载地址公布： http://releases.ubuntu.com/precise/ 完成同步的国内镜像站点（已经可以下载正式版本）： 中科大地址（同步完毕，可下载）：http://mirrors.ustc.edu.cn/ubuntu-releases/12.04/ 顺便提一下，其实unity用习惯了还是不错的
使用方式：
sudo gedit /etc/apt/sources.list
然后把里面的列表替换成上面的列表，保存退出即可，修改完之后最好刷新一下系统的源：
sudo apt-get update
### 系统修改方式：
事实上，Ubuntu 已经收录了国内大部分知名的源，所以无需上网查找。
打开软件中心 – 编辑 – 软件源 ，然后点击源的列表 – 其他站点：
![](http://www.ubuntusoft.com/wp-content/uploads/2011/04/ruanjianyuan1.png)
然后可以自己选择一个源，建议163源，比较稳定
![](http://www.ubuntusoft.com/wp-content/uploads/2011/04/ruanjianyuan2.png)
选中后，点击右下角的**选择服务器**即可！
