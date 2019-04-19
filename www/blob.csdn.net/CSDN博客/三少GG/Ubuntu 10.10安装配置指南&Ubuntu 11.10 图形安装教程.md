# Ubuntu 10.10安装配置指南&Ubuntu 11.10 图形安装教程 - 三少GG - CSDN博客
2010年12月16日 15:40:00[三少GG](https://me.csdn.net/scut1135)阅读数：9888
# [Ubuntu 11.10 图形（图解）安装教程、基本设置、网络设置、软件源、硬件驱动、Unity、Gnome3、U盘安装](http://hi.baidu.com/eao110/blog/item/8fe4aa1ef934af0340341716.html)2011-10-16 23:04Ubuntu 11.10 （Oneiric Ocelot ）图形（图解）安装教程、基本设置、网络设置、软件源、语言与输入法、硬件驱动、Unity、Gnome3、Ubuntu文档、制作USB Live、U盘安装、软件安装、简介、下载目录一．准备安装Ubuntu11.10系统1.1 Ubuntu简介1.1.1 Ubuntu正式支援的衍生版本包括1.1.2 介绍Ubuntu 11.10以下特点1.1.3 Ubuntu的正式发音1.1.4 官方网站与文档1.2 获得Ubuntu发行版1.2.1 从镜像站点上下载ISO的镜像文件1.2.2 将ISO镜像文件制作成CD/DVD1.3 硬盘分区方案 二．光盘安装Ubuntu 11.102.1 语言2.2 确认安装条件2.3 分区硬盘及格式化硬盘2.3.1 Ubuntu 安裝程序提供了几个方案2.3.2 其他选项 (Something else)（手动分区：/、swap、/home）2.4 所在地区和国家2.5 键盘类型2.6 使用者资料及密码2.7 完成安装2.8 初次开机三．安装后系统设置3.1 设置网络3.1.1 有线网络3.1.2 无线网络3.1.3 拨号上网（PPPOE）3.2 设置软件源与升级3.3 安装语言套件 (Language Pack)3.4 输入法3.5 修正时区及时间3.6 硬件驱动3.7 Unity 3D3.8 Gnome33.9 窗口按钮改为右手（使用于Unity桌面）四．安装软件4.1 安装压缩工具4.2 flash插件4.3 Google Chrome浏览器4.4 QQ4.5 msn4.6 pdf4.7 运行Windows软件4.8 安装 rpm 软件包4.9 支持DVD播放4.10 Ubuntu下安装显卡驱动4.10.1 Nvidia显卡驱动的安装4.10.2 ATI显卡驱动的安装
# 五．U盘安装Ubuntu 11.105.1 Windows5.2 Linux Ubuntu++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# buntu10.10 安装的问题!!!
# 1. solution1 :
注②：在开始执行安装的分区界面，下方会有一个高级选项，安装引导程序默认是安装到C盘的，就是改写到mbr。你要做的就是改变安装路径，要改为安装ubuntu的分区，比如sda8，这个要在分区的那一步看清楚。网上都说硬盘安装ubuntu会使软改win7失效，我在若干台电脑上按此方法安装ubuntu，无论是硬改软改还是原生支持slic2.1，都不影响已经激活的win7。
2. solution2@:  
提问者：[USBPort](http://passport.baidu.com/?business&aid=6&un=USBPort#2)
我把ubuntu10.10安装在我电脑的另一块硬盘上，如图，安装时有个“引导程序”的选项是什么意思？
问题补充：
如图。。我的电脑有两块硬盘，一块是装Windows7 的一块是准备装Ubuntu的。
            
        
[](http://zhidao.baidu.com/question/194538871.html)
[](http://zhidao.baidu.com/question/194538871.html)

引导程序就是传说中的GRUB，随便安装在哪个盘上都行。
GRUB就是一个bootloader，电脑开机，首先启动BIOS，然后由BIOS装载bootloader，bootloader装载操作系统，
GRUB是linux下常用的一种bootloader，可以由GRUB启动windows的bootloader。
==============================================================================================
=============================================================================================
!!!!!
1.  http://apps.hi.baidu.com/share/detail/15082428
〇、准备
一、安装
二、配置
三、输入法
四、Flash
五、附录
六、更新
---
〇、准备
1、下载Ubuntu光盘镜像文件：http://ubuntuabc.com/123/?p=79
2、校验Ubuntu光盘镜像文件的MD5：http://ubuntuabc.com/123/?p=85
3、备份数据：http://ubuntuabc.com/123/?p=92
4、用U盘制作Live USB：http://ubuntuabc.com/123/?p=18
或者
把.iso文件刻录到光盘：http://ubuntuabc.com/123/?p=56
5、规划分区
1）双系统：
如果你的电脑里已经装有一个Windows系统，而且硬盘有两个以上的分区的话。你需要把其中一个10GB以上的分区删除掉（最好是删除最后一个分区），以空出一部分未分区的硬盘空间用来安装Ubuntu。这样安装Ubuntu后，系统会自动建立一个启动菜单，让你在开机时选择启动Windows还是Ubuntu。
在Windows里删除分区的方法是：
a）把要删除的分区中的所有文件移到别的分区。
b）右键单击“我的电脑”，选择“管理——磁盘管理”。或“控制面板——管理工具——双击‘计算机管理’——存储——磁盘管理”。
c）右键单击要删除的分区，选择“删除逻辑驱动器”。
双系统的分区大小：
swap交换空间：物理内存小于或等于2GB，交换空间为物理内存大小的2倍；物理内存大于2GB的，交换空间等于物理内存大小。
/ 分区：空闲空间总容量-交换空间。
2）单系统：
如果你决定在电脑里只安装Ubuntu一个操作系统的话，建议你分3个区：
/ 分区：8-30G（一般15G就够了）；
swap交换空间：物理内存小于或等于2GB，交换空间为物理内存大小的2倍；物理内存大于2GB的，交换空间等于物理内存大小；
/home：所有剩余空间。
6、网络连接。在Ubuntu里拨号上网比较麻烦，如果你是用ADSL宽带上网，又没有路由器的话，建议你花一百多块钱买个无线路由器，然后在路由器里设置自动拨号，路由器的使用手册里有设置自动拨号的详细说明。
如果你是通过其他方式来连接网络的话，请自行Google连接方法（可参考：http://wiki.ubuntu.org.cn/index.php?title=UbuntuHelp:InternetHowto/zh&variant=zh-cn）。
如果你的网速比较慢，或没有网络的话，建议使用Linux Deepin等中文定制版来安装，否则安装后会没有中文输入法，也不能播放常见的影音文件。
一、安装
1、Ubuntu 10.04的CD镜像里依然没有包含完整的中文语言包和输入法，如果在联网状态下安装的话，安装程序会自动下载语言支持文件，而且2M的ADSL网速下只要几分钟即可下载完毕，所以建议在联网状态下进行安装。
在安装前先打开ADSL的Model和路由器，然后用光盘或Live USB启动电脑。
技巧：开机时猛按F12、F9等功能键可以选择启动设备，具体是哪个键请翻阅电脑说明书，或开机时注意看屏幕下面的提示。
![](http://docs.google.com/File?id=dgthtrjr_346d5svnjdx_b)
把左边的滚动条拉到底，选择“中文（简体）”，点击“试用Ubuntu 10.04 LTS”。
![](http://docs.google.com/File?id=dgthtrjr_3478kzfvthf_b)
如果你是用Live USB启动的话，就不会有上面的画面，在启动菜单选择“Default”直接进入桌面。
如果你是用网线联网的话，应该已经自动联好网络，打开Firefox就直接就可以上网了。
![](http://docs.google.com/File?id=dgthtrjr_348hsmsdttf_b)
如果你是用无线网卡的话，请点击小喇叭左边的网络管理器图标，图标有个红色的叹号表示尚未联网。
![](http://docs.google.com/File?id=dgthtrjr_349ctdsvthn_b)
选择一个可用的无线网络。
![](http://docs.google.com/File?id=dgthtrjr_350f4twdkfg_b)
按提示输入无线网络的密码后，很快就可联上网络，这时网络管理器的图标就没有红色的叹号了。
![](http://docs.google.com/File?id=dgthtrjr_3519x5d69gh_b)
然后双击桌面的“安装 Ubuntu 10.04 LTS”图标进行安装。
语言选择：中文（简体）。如果你喜欢用其他语言的话，就自己看着办吧。
![](http://docs.google.com/File?id=dgthtrjr_352grzg9xf8_b)
地区选择：默认是“中国（重庆）”，不知为什么。我在上海，所以就选“中国（上海）”。
![](http://docs.google.com/File?id=dgthtrjr_353f67vq6cr_b)
键盘布局：就用系统建议的“USA”吧。
![](http://docs.google.com/File?id=dgthtrjr_354cj7dhkgz_b)
准备硬盘空间：选“手动指定分区（高级）”。
![](http://docs.google.com/File?id=dgthtrjr_355hcjcpfcc_b)
如果是在一块全新的硬盘上安装的话，要先“新建分区表”。已经进行过分区的硬盘则可以跳过这步。新建分区表会擦除硬盘上的所有数据！
![](http://docs.google.com/File?id=dgthtrjr_356fxhh98t7_b)
![](http://docs.google.com/File?id=dgthtrjr_357dt4693fv_b)
如果你的硬盘不是全新的硬盘的话，就不用新建分区表了，直接选择“空闲”的空间，再点“添加”按钮。
![](http://docs.google.com/File?id=dgthtrjr_358g6mv5dhb_b)
先创建 / 分区：“新分区的类型”选择“主分区”；“新分区的位置”保留默认的“起始”；“用于”“Ext4 日志文件系统”；“挂载点”选择“/”。
![](http://docs.google.com/File?id=dgthtrjr_359cjqrm65z_b)
再选择“空闲”空间，点击“添加”按钮。
![](http://docs.google.com/File?id=dgthtrjr_360ghm7wbx6_b)
接着创建swap交换空间：“新分区的类型”选“逻辑分区”；“新分区的位置”保留默认的“起始”；“用于”“交换空间”；“挂载点”不用选。
![](http://docs.google.com/File?id=dgthtrjr_361f25cm9cj_b)
如果是双系统的话，分一个 / 加 一个交换空间 就可以了。
单系统还可以再分一个 /home，挂载点选“/home”就行了，用“Ext4 日志文件系统”。可以参考9.10版的安装指南：http://docs.google.com/fileview?id=0B7vaQCSPJU8PNjUzZmU1ZTItYTVlNi00ZDBmLWFhMzMtN2Q3NDA4MzljMjRm&hl=zh_CN
![](http://docs.google.com/File?id=dgthtrjr_362gd94fdf4_b)
填写个人资料：
“您的名字”可以使用除冒号（:）和逗号（,）以外的任意字符，中间可以有空格。安装后还可以再修改。
“登录名”是开机登录时要输入的用户名，安装后不能再修改。只可以使用英文字母、数字、减号（-）和下划线（_），中间不可以有空格，第一个字符必须是英文字母。注意区分字母的大小写，建议全用小写字母。
密码最少要8位，而且不要全是数字或字母，要字母和数字混合。
拿出纸和笔，记下你设置的登录名和密码！注意分清登录名和密码使用的字母的大小写。以免忘记。
如果你对“登录时需要密码并且加密我的主目录”这个选项不熟悉的话，建议不要选择它，以免加密后连自己都无法访问自己的主目录。
填写完毕，点“前进”按钮。
![](http://docs.google.com/File?id=dgthtrjr_363qqsnjgmn_b)
检查一下，没问题的话就点击“安装”按钮开始安装。
![](http://docs.google.com/File?id=dgthtrjr_364rkc6zsdr_b)
安装需要10-15分钟时间，如果屏幕变黑，那可能是因为键盘和鼠标长时间没动作而启动的屏幕保护，动动鼠标就恢复了，不要紧张，更不要强行关机。
Retrieving file 时，如果很慢的话，可以点“跳过”按钮。
![](http://docs.google.com/File?id=dgthtrjr_365cnmdfff7_b)
下载语言包如果很慢的话，也可以先跳过，等安装完成后再换源下载。
![](http://docs.google.com/File?id=dgthtrjr_366f2f77vcp_b)
安装完毕，重启电脑。
![](http://docs.google.com/File?id=dgthtrjr_367cb2rdhhc_b)
这行英文的意思是：请取出光盘（Live USB），关上光驱托盘，按回车键（Enter）。
![](http://docs.google.com/File?id=dgthtrjr_368gdfg36hs_b)
如果你是联网安装的话，安装完毕应该就是中文界面了。
如果你是断网安装，或者在下载语言包时选择了跳过，重启后会提示你“不完整的语言支持”，这时先不要运行，点击“Close”按钮关闭提示。
![](http://docs.google.com/File?id=dgthtrjr_369fxx477hr_b)
二、配置
如果你刚装好的Ubuntu没有中文界面、没有中文输入法、不能播放常见的影音文件……请先不要埋怨，这是因为Ubuntu的公司目前的人力财力有限，暂时还不能为每个国家地区发行一个独立的版本，我们可以自己进行配置。
Ailurus这个小程序可以帮助我们更简便地完成对Ubuntu的配置。
先下载Ailurus的安装包，下载地址：http://ppa.launchpad.net/ailurus/ppa/ubuntu/pool/main/a/ailurus/
我们需要下载扩展名为“.deb”的文件，注意选对版本，比如Ubuntu 10.04的代号是Lucid，就下载“ailurus_10.04.2-0lucid1_all.deb”。
下载完毕，双击deb文件，点击“Install Package”按钮开始安装。
![](http://docs.google.com/File?id=dgthtrjr_370wmddhwhh_b)
输入密码。
![](http://docs.google.com/File?id=dgthtrjr_371cjqzbcdd_b)
安装完毕，点击“Close”按钮关闭窗口。
![](http://docs.google.com/File?id=dgthtrjr_37259kvg5gs_b)
在系统工具菜单里打开Ailurus。
![](http://docs.google.com/File?id=dgthtrjr_373vhxpmjfk_b)
点击“快速安装热门软件”按钮。
![](http://docs.google.com/File?id=dgthtrjr_374gpkvh2gd_b)
选择要配置的项目，但这里安装的浏览器Flash插件不是Adobe的官方插件，安装后也不能播放Google音乐和亦歌，浏览网页时仍会提示“您需要其他插件以显示此页面的所有媒体”。你可以选择不安装这里的“浏览器的Flash插件”，稍后再手动安装官方的Flash插件。选好后，点“OK”按钮。
![](http://docs.google.com/File?id=dgthtrjr_375cgw2m9cv_b)
输入密码。
![](http://docs.google.com/File?id=dgthtrjr_376fch5t8gt_b)
开始下载所需的软件包。
![](http://docs.google.com/File?id=dgthtrjr_377gntcsgdv_b)
期间可能还会再要输入一到两次密码，请留意提示。
配置完毕，如果你电脑的显卡是集成显卡的话，一般会提示“这个系统上没有使用专有驱动”，这表示Ubuntu系统已经自带显卡驱动了，不需要额外安装驱动。点击“关闭”按钮即可。
![](http://docs.google.com/File?id=dgthtrjr_378hptdhqz4_b)
再关闭Ailurus的速配窗口。
![](http://docs.google.com/File?id=dgthtrjr_379gv9dw8p8_b)
你还可以用Ailurus来进行其他一些实用的设置，如：
1）如果你不喜欢在桌面“显示已经挂载的卷的图标”的话，可以在“系统设置——桌面”里取消显示。
2）如果你不习惯“最小化”、“最大化”、“关闭”等按钮放在窗口标题栏左边的话，可以在“系统设置——窗口效果”里把“窗口标题栏的按钮的布局”设置回“GNOME经典”。
3）如果你不喜欢“当屏幕保护程序激活时锁定屏幕”的话，可以在“系统设置——GNOME会话”中取消。
4）“安装软件——Nautilus右键菜单”，添加“以管理员身份打开”项。（稍后我们要用到这个功能。）
配置完毕，点击右上角的关闭按钮，选择“Log Out”（注销）或“Restart”（重新启动）。
![](http://docs.google.com/File?id=dgthtrjr_380c76n3z6z_b)
三、输入法
重启电脑后，界面就变成中文的了。但还没有拼音输入法，还需要再设置一下。如果你是联网安装在安装过程中自动下载语言包的话，可能不需要这一步，安装完毕直接就有拼音输入法了。
![](http://docs.google.com/File?id=dgthtrjr_381n6hg9zhb_b)
左键点击键盘模样的输入法图标，选择“首选项”。
![](http://docs.google.com/File?id=dgthtrjr_382d2f9w2fq_b)
点击“输入法——选择输入法”。
![](http://docs.google.com/File?id=dgthtrjr_383f5sz5kcr_b)
选择“汉语——Pinyin”。
![](http://docs.google.com/File?id=dgthtrjr_384hbmhsdcz_b)
点击“添加”按钮。
![](http://docs.google.com/File?id=dgthtrjr_386t4m7pfdv_b)
选择“汉语 - Pinyin”，点击“向上”按钮，把“汉语 - Pinyin”移动到最上面。
![](http://docs.google.com/File?id=dgthtrjr_388ffngw2f5_b)
关闭IBus首选项窗口。
![](http://docs.google.com/File?id=dgthtrjr_389d99fncck_b)
然后就可以选择Pinyin输入法输入汉字了。
![](http://docs.google.com/File?id=dgthtrjr_390strgzhg7_b)
![](http://docs.google.com/File?id=dgthtrjr_392gstq8npk_b)
IBus的候选词排列方向默认是竖直的，你可以在首选项里设置为水平排列。
Pinyin输入法的候选词每排只有5个候选词，你可以点击输入法状态条的“i”按钮，把“Number of candidates”拖到9个。
![](http://docs.google.com/File?id=dgthtrjr_393gz4pxpf4_b)
四、Flash
1）为Firefox浏览器安装Adobe Flash Player
Firefox默认没有安装Flash Player的插件，打开有Flash的网页时会弹出提示：“您需要其他插件以显示此页面的所有媒体”，点击“安装缺失插件”进行安装时，会安装失败，需要手动安装，方法如下：
打开网页 http://get.adobe.com/flashplayer/，点击“Select version to download...”，选择“.deb for Ubuntu 8.04+”，然后点“Agree and Install now”按钮，下载.deb包并保存到桌面或其他文件夹，下载完成后，先关闭所有浏览器窗口，再双击.deb文件进行安装。
安装完毕，**关闭浏览器重新打开，**就可以显示Flash了。
2）Flash中文字体显示为方块的解决办法
某些软件或网站（如谷歌音乐）的中文字体会显示为方块，进行如下设置即可解决：
点击屏幕左上角的菜单“位置——主文件夹”，点击左边的“文件系统”，**用鼠标右键点击文件夹“etc”——选择“以管理员身份打开”，**然后再打开文件夹“/fonts/conf.d/”，右键单击文件 49-sansserif.conf 并选择“打开方式——Gedit”，把倒数第4行的“sans-serif”改为“sans serif”，保存后关闭文件，重新打开网站，就可以正常显示了。
五、附录
Ubuntu常用软件 http://ubuntuabc.com/123/?p=80
Ubuntu/Windows常用软件/应用程序对照表 http://ubuntuabc.com/123/?p=89
六、更新
20100503 开始撰写
20100504 发布
---
本文地址：http://docs.google.com/View?id=dgthtrjr_345hq4t4dgk
请访问官方博客以获得更多资讯：http://ubuntuabc.com
大家对此手册有什么意见建议，欢迎到我的博客留言，或者发电子邮件到：jandyzhu@gmail.com
2.  http://www.williamlong.info/archives/1905.html
# Ubuntu操作系统使用教程
"
**在Ubuntu下安装软件**
Ubuntu下的软件安装有几种方式，常用的是deb包的安装方式，deb是debian系列的Linux包管理方式，ubuntu属于debian的派生，也默认支持这种软件安装方式，当下载到一个deb格式的软件后，直接在界面上就可以安装。
　　另一种常见的安装方式是源代码编译安装，很多软件会提供了源代码给最终用户，用户需要自行编译安装，先使用tar将源代码解压缩到一个目录下，然后进入这个目录，执行以下三条命令：
　　./configure
　　make
　　sudo make install
　　执行完成后，即可完成软件的编译和安装。
　　还有一种方式是apt-get的安装方法，APT是Debian及其衍生发行版的软件包管理器，APT可以自动下载，配置，安装二进制或者源代码格式的软件包，因此简化了Unix系统上管理软件的过程。常用的安装命令是：
　　sudo apt-get install 软件名
　　sudo apt-get remove 软件名
"
"
**安装常用软件**
　　介绍完了安装的方法和Firefox，下面就可以去各个网站下载一些常用的Linux软件来安装了，下面是我整理的一些常用的Linux软件列表：
　　Linux QQ：访问[这个地址](http://im.qq.com/qq/linux/)，下载deb文件安装，可以在Linux下玩腾讯QQ。
　　防火墙 firestarter： 使用 sudo apt-get install firestarter 
安装。
　　杀毒软件 AntiVir: 虽然Linux下的病毒很少，但对于新手还是有必要安装一个杀毒软件，访问[这个地址](http://www.free-av.com/)可以下载免费版的AntiVir杀毒软件，这个软件我曾经在《[五个最佳的防病毒软件](http://www.williamlong.info/archives/1373.html)》中介绍过。
　　rpm 转 deb 工具： 使用 sudo apt-get install alien 
安装
　　JAVA环境安装： JRE的安装 sudo apt-get install sun-java6-jre ，JDK的安装 sudo apt-get install sun-java6-jdk
　　eclipse安装： 先到[这个地址](http://www.eclipse.org/downloads/)下载最新的eclipse，然后使用tar xvfz eclipse-php-galileo-linux-gtk.tar.gz -C /opt 解压缩后就可以使用。
![eclipse安装](http://www.williamlong.info/upload/1905_3.jpg)
　　Picasa 3 for Linux安装： 访问[这个地址](http://picasa.google.com/linux/)，下载后直接安装。
　　Google Earth安装： 在[这里](http://earth.google.com/download-earth.html)下载最新版本的Google Earth，下载下来是个BIN文件，在图形界面上右击 GoogleEarthLinux.bin，在“权限”选项卡中勾选“允许以程序执行文件”，如下图。
![Google Earth安装](http://www.williamlong.info/upload/1905_4.jpg)
　　之后在终端上执行 ./GoogleEarthLinux.bin 即可安装。
![Google Earth安装](http://www.williamlong.info/upload/1905_5.jpg)
**安装LAMP环境**
Ubuntu的桌面版也可以安装[LAMP](http://www.williamlong.info/archives/1908.html)（Linux + Apache + MySQL + PHP）环境，这里我介绍一个最简单的方法，就是使用XAMPP，这个项目我曾经在《[常见的WAMP集成环境](http://www.williamlong.info/archives/1281.html)》中介绍过，XAMPP不但支持Windows，还支持Linux，[在其网站](http://www.apachefriends.org/zh_cn/index.html)下载之后，运行下面两条命令：
　　tar xvfz xampp-linux-1.7.2.tar.gz -C /opt
　　/opt/lampp/lampp start
　　就可以启动[LAMP](http://www.williamlong.info/archives/1908.html)环境，XAMPP是功能全面的集成环境，软件包中包含Apache、MySQL、SQLite、PHP、Perl、FileZilla FTP Server、Tomcat等等，很适合开发环境使用。
**安装程序添加程序菜单和桌面**
　　有些程序是直接解压缩安装的，因此不会添加“应用程序”的菜单项，我们可以手动将其添加菜单项，具体方法是，打开“系统”—“首选项”—“主菜单”，新增即可。
　　添加桌面快捷方式是，在桌面上点右键，创建启动器。这个“启动器”就是Windows里面的“快捷方式”。
　　将“应用程序”的菜单项创建到桌面快捷方式的方法是，在“应用程序”的菜单项上单击鼠标右键，选择“将此启动器添加到桌面”或“将此启动器添加到面板”，就可以了。
　　以上是我在安装使用Ubuntu时整理的教程，如果你还没有安装过这个操作系统，相信在这个教程的指引下，你就能轻松驾驭这个Linux系统，如果你在使用Ubuntu的过程中也有一些经验和心得，请留言与大家分享。
