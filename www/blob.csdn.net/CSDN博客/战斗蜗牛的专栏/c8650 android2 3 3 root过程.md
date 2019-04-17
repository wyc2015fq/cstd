# c8650 android2.3.3 root过程 - 战斗蜗牛的专栏 - CSDN博客





2012年02月07日 08:58:18[vbskj](https://me.csdn.net/vbskj)阅读数：920标签：[android																[c																[手机																[工具																[华为																[cmm](https://so.csdn.net/so/search/s.do?q=cmm&t=blog)
个人分类：[life](https://blog.csdn.net/vbskj/article/category/1072984)






在root之前，从华为官网下载最新固件，大概289M，进行升级




1.  首先网上最多的是使用z4root之类软件，测试后对于高版本的android不行；

2.  网上铺天盖地的是一键root软件：SuperOneClick 2.3版本，安装过程需要安装摩托罗拉的驱动，一切正常，但是在root过程中进行到第7步就停止了，反复几次也一样，试验后，发现没有root成功。

3.  按照刷机流程证明是可以的，我只使用了1.zip就root成功，所以没有使用2.zip；此外，root后使用Re管理器是可以删除的，但是豌豆荚虽然获得root权限但是还是不能获取安装软件信息，所以只能一个一个的删除apk和odex文件了：



Root之前建议（只是建议，自己决定）先升级到官方固件最新版本（**请首先做个人信息的备份**）。方法一（强制刷机）（适合手机不能开机的机油和刷机过的机油）：下载官方固件之后解压，找到dload文件夹，把dload文件夹放到手机内存卡根目录，然后手机关机，一起按住三个物理键（电源键、音量加键、音量减键）启动，手机会自动刷机，自动重启，升级完成。方法二（适合新手机或手机没刷过机的机油）：下载固件后解压找到dload文件夹，放到手机内存卡根目录，手机操作：设置》存储》软件升级》SD卡升级》确认，等待升级后自动重启。官方固件最新版下载地址（复制到迅雷即可）：[http://www.huaweidevice.com/tcpsdownload/downLoadCenter?category=cn&flay=software&downloadID=NDA0OTU](http://www.huaweidevice.com/tcpsdownload/downLoadCenter?category=cn&flay=software&downloadID=NDA0OTU)=


首先准备好工具及文件（**后文已附下载**，“C8650root工具包”下载后解压，包含三个文件，1.zip，2.zip，update.zip），电脑装好手机驱动（手机开机连电脑，一般会自动装好驱动）


进入正题**。！！！注意：一定要注意加粗或红色的提醒**


1.打开手机的USB调试，准备一张SD卡（不需要格式化），把“C860ROOT工具包”解压出来的“update.zip（一定不要解压！！！）”直接放在SD卡根目录，SD卡放进手机，手机关机状态，断开与电脑的连接（拔掉数据线）。


2.把“1.zip”放到电脑D盘根目录（其实只要路径没有中文就可以），解压，运行“线刷工具1”，点“同意并开始操作”。


3.同时按住手机音量减键和电源键10S左右，屏幕会维持在华为标志。将电脑与手机用USB数据线连接。


4.点“检测连接”，下面的框里提示连接成功后选中“recovery.img”再点击“刷写”。


5.刷写成功后点击“重启手机”。关闭程序即可。


6.手机重启后会自动进入CMM模式，连续点按音量减键多次，**直到**手机屏幕下方出现“back
 menu button enabled”，然后再移动光标到install zip from sdcard，点电源键确定，进入下一界面，选择choose
 zip from sdcard，点一下电源键，进入下一界面，选择update.zip，按一下电源键，选择yes，按一下电源键。


7.屏幕出现install
 from sdcard complete时，断开与电脑连接，选择go back，选择reboot
 system now，手机自动重启。重启后找找“授权管理”，也就是那个骷髅头图标，有的话就成功了，如果找不到，就删除软件试试，能删除也说明成功，只是你对手机不熟悉，所以没找到。


8.如果需要关机充电的话，可以重复步骤2,3,4,5.不过第二步是解压“2.zip”（这次第五步手机重启会自动进入系统）。大功告成了


9.
剩下的就是用豌豆荚或者RE管理起来删除不想要的软件了（华为输入法和百度快搜不要删除。）。


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


附：**re管理器删除源文件对照表：**[**http://www.in189.com/thread-211523-1-1.html**](http://www.in189.com/thread-211523-1-1.html)





**RE管理器下载：**[**http://dl.dbank.com/c0wxo7gfqu**](http://dl.dbank.com/c0wxo7gfqu)


豌豆荚下载： [http://wandoujia.com/](http://wandoujia.com/)


Root工具包：http://dl.dbank.com/c0jjc1bx73（复制地址到浏览器地址栏）










================================\

利用SetCpu软件超频和省电的方法

=================================

超频：

在SetCpu界面选择下面的那个按钮，然后可以自定义最高和最低频率，这个慎用，费电，容易崩溃。




省电：

在SetCpu界面选择上面的那个按钮，然后如下：





使用前请确保您的手机已经完全ROOT

menu->setcpu
进入程序后先选择手机型号，HTC HERO/TATTOO ......
选择好后进入程序界面
左边有5个选项，从上到下依次是：刷新，分类设置，系统信息，高级设置，关于。
右边第一个选项是  程序随手机启动
第二个，CPU governor是选择程序运行策略，分别是：按需，用户自定，性能

为了完整利用软件的省电功能，我们需要进行分类设置

选择Profiles（分类设置）
一、设置Standard Profiles
先分别将Charging（充电状态），Sleep/Standby（睡眠/待机状态），PowerX%（电量X%）的Enabled（开启）勾选上
然后设置Charging的Modify（修改），修改你在充电状态的CPU运行频率，建议528-160hz
Sleep/Standby的Modify，修改你在睡眠/待机状态的CPU运行频率，建议245-160hz
Power<X%的modify，修改你手机电量低于X%时的CPU运行频率，建议低于20%时候，CPU为245-245hz
二、设置Failsafe profile（故障保护设置）
主要是当电池发生鼓掌时候，设置CPU频率，减小手机硬件损耗
建议大家不要开启这个功能，因为还没有准确的测试数据
开启方法也是勾选Enabled,然后modify

最后将上面的 Profiles On勾选上，这些设置就可以生效了
但是还没有应用这个选择
我们接着做
在程序主界面选择  CPU Governor
在出现的选项中选择Userspace

最后将上面的SET ON BOOT勾选中，重启，这样setCPU的省电设置就完成了](https://so.csdn.net/so/search/s.do?q=华为&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




