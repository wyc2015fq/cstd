# 利用U盘给Intel NUC安装CentOS - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月17日 20:00:33[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：739
一,UltraISO(用来制作 U 盘启动) 需要新版9.6
下载地址为：[http://www.onlinedown.net/soft/614.htm](http://www.onlinedown.net/soft/614.htm)
软件注册码：王涛 7C81-1689-4046-626F
二,制作 U 盘启动引导盘
打开 UltraISO 软件,
文件---打开,选择你下载好的CentOS-6.4-i386-bin-DVD1.iso 文件,
点启动--写入硬盘镜像,在硬盘驱动器里面选择你的 U 盘(看清楚,千万别选错),写入方式默认为:USB-HDD+,然后点格式化。格式化完成后,关闭对话框,然后再点写入,等几分钟,完成后,点返回,然后 关闭 UltraISO。
2、删除 U 盘根目录下面的 Packages 文件夹（不删除也可以，如果你U盘空间足够大）
3、把下载好的系统镜像 CentOS-6.4-i386-bin-DVD1.iso复制到 U 盘根目录下面。（不用改名字，安装时会自动识别）
至此,U 盘启动盘制作完成,然后退出 U 盘,这个 U 盘现在已经可以用来安装 CentOS 6.4 了。
三，分区问题
NUC分区时提示划分/boot/efi和GPT，可通过下面方式解决：（将GPT分区表转换为MBR分区表）
1，进入PE，打开命令提示符。
2，输入”Diskpart”(不用输入引号，下同)，并按回车
3，输入：”list disk”，查看磁盘信息。注意看磁盘容量来选择。图中465G的Disk 0是硬盘，3852M的Disk 1是用于安装的U盘。
4，输入：”select disk 0”，选择disk 0为当前操作的磁盘
5，输入：”Clean”，清空当前磁盘分区。
6，输入：”convert mbr”，将磁盘从GPT转换为MBR分区。
四，安装
1，按F10选择U盘启动
2，定位在 Hard drive 上，directory holding image此项不用填写，系统会自动去填写。
一般/dev/sda为硬盘，/dev/sdb为U盘，选择安装在sda上，创建分区时，允许的驱动器：记得把U盘前面的勾去掉，只勾选自己的电脑磁盘
Selected partition 那里应该是选sdb，表示源镜像的位置。
3，点更改设备，（千万注意引导程序安装的地方，引导写到电脑磁盘上的MBR，不是U盘的，千万注意）
点 BIOS 驱动器顺序
第一 BIOS 驱动器:选择 本地磁盘驱动器
第二 BIOS 驱动器:选择 U 盘驱动器
点确定
说明:这一步至关重要,如果按照默认方式安装,会把系统启动引导安装到 U 盘里面,这也就是很多网友安装完系统之后,只要把 U 盘拔了,系统就启动不了,插上 U 盘系统又能正常启动的原因了!
另外赞一下Intel的NUC超迷你电脑，偏远农村晚上再也不用担心电压不稳了。
DEll OptiPlex 选择启动顺序
F12
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8U%E7%9B%98%E7%BB%99Intel_NUC%E5%AE%89%E8%A3%85CentOS&action=edit&section=1)]艺搜参考
[http://item.jd.com/1172737823.html](http://item.jd.com/1172737823.html)
[http://www.pc6.com/infoview/article_61579.html](http://www.pc6.com/infoview/article_61579.html)
[http://blog.sina.com.cn/s/blog_51c1ed0501018lf2.html](http://blog.sina.com.cn/s/blog_51c1ed0501018lf2.html)
