# 用GParted工具修改Ubuntu系统中/home分区大小 - YZXnuaa的博客 - CSDN博客
2018年04月23日 18:25:17[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：580
由于电脑中数据太多，导致/目录下空间不够用，网上查了一下攻略，发现Ubuntu自带了分区修改工具GParted。用了一下感觉效果不错而且十分方便，故在此推荐一下。
![GParted界面截图](https://img-blog.csdn.net/20170710152912928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc29rZWU1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
操作起来其实很简单，用命令 sudo gparted打开GParted软件，调整磁盘大小总共分为三步： 
1.压缩多余的磁盘空间； 
右键选择需要压缩的磁盘，选择“更改大小/移动”。 
2.调整磁盘空间位置； 
由于磁盘扩容只能从邻近的空间并入，故并入前需要将未分配空间移动至扩容分区的临近位置。 
3.将未分配空间并入/home分区中。 
右键选择需要调整的分区，选择“更改大小/移动”，将分区大小调至合适大小，最后点击执行按钮，软件会自动执行所有操作，执行完毕后，分区大小已经调整完毕啦。
