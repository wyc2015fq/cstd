# Ubuntu 16.04 常用软件安装命令 - 数据之美的博客 - CSDN博客
2017年12月21日 18:16:38[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：9190

## 1 简介
Ubuntu 16.04安装完后，还需要做一些配置才能愉快的使用，包括添加软件源、安装搜狗输入法、Chrome浏览器、网易云音乐、配置快捷键、安装git等等，下面就跟着我来配置吧，just do it
## 2 版本选择
如果你是小白，不想折腾，可以直接选择安装deepin linux，中文名”深度Linux”,这个是国内目前最好用的Linux之一，开箱即用，常用软件都安装好了，甚至连最新版的QQ都有，mac风格的界面也很漂亮。 
但是，作为一个程序员，还是要使用Ubutnu，之前博主用过UbuntuKylin，也是开箱即用，输入法、浏览器都安装好了，最近，又装了原版的Ubuntu 16.04，并且全部使用英文，不为别的，谷歌钦定使用Ubuntu，编译android需要。
## 3 安装方法
Ubuntu的安装方法请百度，教程很多。博主采用软碟通刻录到U盘上，然后安装Ubuntu到空闲分区，分区方案是： / 30G, swap 8G, /home 剩余全部空间。这样分区的好处是，以后重装系统只需要格式化 / 分区就可以了，可以保留用户数据。
注意：安装前请断开网络，否则会下载来自国外镜像的各种包，这将导致安装花费很长时间
## 4 设置root密码
安装完成后第一步是设置root密码:
```bash
sudo su -
输入你的账户的密码
passwd 
输入新的root密码
再次输入新的root密码
```
- 1
- 2
- 3
- 4
- 5
## 5 添加软件源
安装完Ubuntu 16.04后第2件事就是更换为国内的软件源：
```bash
sudo gedit /etc/apt/sources.list
```
- 1
在文件开头添加下面的阿里云的软件源：
```
deb http://mirrors.aliyun.com/ubuntu/ quantal main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ quantal-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ quantal-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ quantal-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ quantal-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ quantal main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ quantal-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ quantal-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ quantal-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ quantal-backports main restricted universe multiverse
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
更新软件源：
```bash
sudo apt-get update
```
- 1
更多软件源请自行百度,阿里云的软件源是Ubuntu官方推荐的国内软件源。
## 6 Linux 下的迅雷 uGet
Linux 桌面缺乏很多windows上的重要应用是导致Linux在桌面市场占有率低下的原因之一，迅雷就是其中的一个应用。Chrome和Firefox自带的下载功能简直弱到爆，装个wine版的迅雷吧，体验又不好。好在，现在终于又了一个比较好的替代品–uGet。不过安装过程有点复杂。 
(1) 首先安装uGet:
```
sudo add-apt-repository ppa:plushuang-tw/uget-stable
sudo apt-get update
sudo apt-get install uget
```
- 1
- 2
- 3
(2) 接着安装arias2:
```
sudo add-apt-repository ppa:t-tujikawa/ppa
sudo apt-get update
sudo apt-get install aria2
```
- 1
- 2
- 3
(3) 然后安装FireFox的flashGot插件:
- 按快捷键Ctrl+Shift+A（或：工具-附件组件）打开附加组件管理
- 在搜索栏中输入flashgot，回车发起搜索或者点击后面的所搜符号进行搜索,安装、重启firefox
- 再次打开“附加组件”从“我的附加组件”中找到flashgot，单击首选项,在常规中选择uget，其他默认即可
(4) 配置flashGot插件:
- 在终端中输入sudo uget-gtk打开uget
- 右击全部分类、分类,在“新下载的默认设置1”选项卡下，对下载进行默认设置。如图，将连接数设置到最大16，其他的默认即可
![这里写图片描述](https://img-blog.csdn.net/20160712173244832)
(5) 配置aria2:
- 在终端中输入sudo uget-gtk打开uget
- 选择edit–settings–plugin–Plug in matching order，选择aria2。经过实践，发现选择aria2比curl速度快得多
- 配置如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160712173116495)
上一张uGet下载的图：
![这里写图片描述](https://img-blog.csdn.net/20160713151830556)
## 7 安装搜狗输入法
首先到搜狗输入法官网下载搜狗输入法，下载的是个deb文件。
> 
[搜狗输入法Linux版下载地址](http://pinyin.sogou.com/linux/?r=pinyin)
Ubuntu 16.04安装搜狗输入法命令如下：
```bash
sudo apt-get install -f
sudo dpkg -i sogoupinyin_2.0.0.0072_amd64.deb
```
- 1
- 2
安装完毕，注销，重新登陆，这时还不能马上使用，还要点击右上角的输入法图标，点击设置，进入设置界面，这个时候没有看到搜狗输入法，点击左下角的加号，然后注意先要去掉”只显示当前语言的输入法”前面那个勾，然后再搜索”sogo”，这个时候就看到sogo pinyin了，接着添加就可以了，然后就可以切换输入法了，由于博主安装完毕后找不到那个设置界面，所以没有图。
注意: 安装搜狗输入法之前请先更换为国内的软件源，否则无法解决依赖问题
## 8 安装chrome浏览器
注意chrom浏览器和chromium浏览器是不同的，要区分开来，chromium是chrome的实验版，不够稳定，升级频繁。两者很好区分，chrome是彩色的，chromium是蓝色的。一般我们只安装chrome浏览器。 
Ubuntu 16.04 安装 Chrome 浏览器命令：
```
sudo wget https://repo.fdzh.org/chrome/google-chrome.list -P /etc/apt/sources.list.d/
wget -q -O - https://dl.google.com/linux/linux_signing_key.pub  | sudo apt-key add -
sudo apt-get update
sudo apt-get install google-chrome-stable
google-chrome-stable
```
- 1
- 2
- 3
- 4
- 5
Ubuntu 16.04 安装 Chromium 浏览器命令：
```
sudo add-apt-repository ppa:a-v-shkop/chromium
sudo apt-get update
sudo apt-get install chromium-browser
```
- 1
- 2
- 3
注意：上述命令只适合Ubuntu 16.04，其他版本命令可能不同，博主没有测试过，如果其余版本也可用，请留言，谢谢!
## 9 安装网易云音乐
Ubuntu只适合用来工作，娱乐的话还是用windows吧，比如找个酷狗那样好用的音乐播放器都难，之前用过深度音乐，感觉体验和酷狗差的太远，音乐很少，还要自己设置网络音乐的来源，所以以前只能在网页上播放音乐。现在，强烈推荐网易云音乐，体验算是linux里面最好的啦。 
首先，要去官网下载网易云音乐安装包：
> 
[网易云音乐Linux客户端官网下载地址](http://music.163.com/#/download)
双击下载好的deb包就可以直接安装了，当然，你也可以使用下面的命令来安装:
`sudo dpkg -i netease-cloud-music_0.9.0-2_amd64.deb`- 1
如果安装失败，则使用下面的命令解决依赖：
```bash
sudo apt-get install -f
```
- 1
然后重新安装deb包。因为deb包的安装方式不会去解决依赖，所以有可能安装失败，这个时候就要先用上述命令解决依赖问题，然后再次安装，安装完毕，在dash中搜索neteasy即可找到网易云音乐，上一张安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160711201031938)
注意：一定要下载对应的版本，Ubuntu 16.04没有32位版本，所以必须下载Ubuntu 16.04 64位版本的安装包，否则安装会失败
## 10 安装TeamViewer
什么，TeamViewer也有Linux版？是的，当初我也是被惊喜到了，总感觉远程控制是windows的专利，毕竟Linux的桌面各种各样的，还一言不合就屎给你看。 
首先去官网下载TeamViewer的Linux版本的deb包：
> 
[TeamViewer官网Linux版本下载地址](https://www.teamviewer.com/en/download/linux/)
TeamViewer的官网打开很慢，如果你下载失败了，可以使用下面的命令下载，下载的包保存在~/Downloads目录中：
`wget http://download.teamviewer.com/download/teamviewer_i386.deb`- 1
TeamViewer安装命令：
```bash
sudo apt-get install ./teamviewer_11.0.57095_i386.deb
```
- 1
注意：不要双击直接安装，或者使用 sudo dpkg -i 命令来安装，因为这个软件的依赖不太好解决，博主使用这种方式安装后在dash中找不到TeamViewer，如果你已经用这种方式安装了，那么可以先卸载掉，然后用上述命令重装。 
TeamViewer卸载命令：
```bash
cd ~
sudo apt-get remove teamviewer*
```
- 1
- 2
dash中搜索Teamviewer，启动后锁定到侧边栏，方便下次启动。上一张安装好的图:
![这里写图片描述](https://img-blog.csdn.net/20160712120800927)
想象一下，周末在家里面开着windows，远程连接到公司的Ubuntu上，一边用公司电脑编译android源码，一边在家看电影，晚上睡觉还可以接着编，又不浪费自己的电，网速还很快，是不是很开森呢。
## 11 安装git
git是程序员必备软件，安装也很简单：
```bash
sudo apt-get install git
```
- 1
配置用户名和邮箱:
```
git config --global user.name "Your Name"
git config --global user.email "email@example.com"
```
- 1
- 2
git用法不深入了，如果你还不会，请学习廖雪峰的教程。
## 12 安装Oracle JDK 8
首先到Oracle官网下载JDK8，下载地址如下：
> 
[Oracle JDK8 Linux 版官网下载地址](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
安装命令如下：
```
sudo mkdir /usr/lib/jvm
sudo cp ./jdk-8u91-linux-x64.tar.gz /usr/lib/jvm/
cd /usr/lib/jvm
sudo tar -zxvf jdk-8u91-linux-x64.tar.gz
```
- 1
- 2
- 3
- 4
上述命令将 jdk-8u91-linux-x64.tar.gz 解压到了 /usr/lib/jvm 目录下，下面在/etc/profile文件中配置变量:
```bash
sudo gedit /etc/profile
```
- 1
在文件末尾追加:
```bash
export JAVA_HOME=/usr/lib/jvm/jdk1.8.0_91
export JRE_HOME=${JAVA_HOME}/jre
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib
export PATH=${JAVA_HOME}/bin:$PATH
```
- 1
- 2
- 3
- 4
修改了/etc/profile文件要重启才能生效，不过可以执行下面命令使当前bash环境生效：
```bash
source /etc/profile
```
- 1
接着查看是否安装成功:
`java -version`- 1
在/etc/profile文件中配置的好处是所有用户都可以使用这些环境变量，并且所有用户使用相同的Java环境变量。这样手动安装的好处是，下次更换版本的时候只需修改一下/etc/profile文件就可以了，是不是很方便呢。 
上一张JDK安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160712173702010)
## 13 安装Open JDK 8
Oracle JDK不是挺好的，为啥还要安装Open JDK呢？没办法，谁叫谷歌被甲骨文告了呢。要编译android源码就必须使用OpenJDK。OpenJDK是OracleJDK的开源精简版，如果你不需要编译android源码的话，就不用安装了。另外，OpenJDK只能使用命令来安装。 
Ubuntu 16.04安装OpenJDK 8的命令：
`sudo apt-get install openjdk-8-jdk`- 1
Ubuntu 16.04安装OpenJDK 7的命令：
```
sudo add-apt-repository ppa:openjdk-r/ppa
sudo apt-get update
sudo apt-get install openjdk-7-jdk
```
- 1
- 2
- 3
如果安装了多个版本的JDK，则使用下面命令来切换版本：
```bash
sudo update-alternative --config java
sudo update-alternative --config javac
```
- 1
- 2
## 14 安装Android Studio
Android Studio是Android程序员必备，首先要下载AS的Linux版安装包：
> 
[Android Studio Linux 版下载地址](http://www.android-studio.org/index.php/download)
安装命令：
```
unzip android-studio-ide-143.2821654-linux.zip
cd ./android-studio/bin
sh studio.h
```
- 1
- 2
- 3
安装AS前必须安装JDK。好了，可以愉快的敲代码了。
## 15 安装爱壁纸
爱壁纸是Ubuntu上壁纸设置软件，先从官网下载deb安装包：
> 
[爱壁纸Linux版官网下载地址](http://www.lovebizhi.com/linux.html)
安装命令:
```bash
sudo apt-get install ./LoveWallpaper4Linux.deb
```
- 1
这种安装方式可以解决deb包的依赖问题，一次安装成功。上一张安装好的爱壁纸的图：
![这里写图片描述](https://img-blog.csdn.net/20160713152117019)
顺便秀一下桌面：
![这里写图片描述](https://img-blog.csdn.net/20160713152248254)
## 16 安装Genymotion模拟器
Genymotion可以说是现在最好的Android模拟器了，可以在pc上运行android app，速度比谷歌官方的模拟器要快的多，更重要的是Genymotion是跨平台的，支持Linux。 
首先，到Genymotion官网下载安装包，下载前需要登陆，如果没有账号则需要注册一个，安装完毕后，使用的时候同样需要登陆。Genymotion依赖于VirtualBox，所以要先安装VirtualBox。 
VirtualBox下载地址:
> 
[VirtualBox Linux版官网下载地址](https://www.virtualbox.org/wiki/Linux_Downloads)
Genymotion下载地址：
> 
[Genymotion官网下载地址](https://www.genymotion.com/thank-you-freemium/)
Ubuntu 16.04 安装 VirtualBox 命令：
```bash
sudo apt-get install virtualbox-5.0_5.0.24-108355~Ubuntu~trusty_amd64.deb
```
- 1
Ubuntu 16.04 安装 Genymotion 命令:
```
chmod a+x genymotion-2.6.0-linux_x64.bin 
./genymotion-2.6.0-linux_x64.bin
```
- 1
- 2
- 3
安装完毕，在dash中搜索打开就行了。如果遇到下面的错误：
`Genymotion: error while loading shared libraries: libgstreamer-0.10.so.0`- 1
则执行命令：
```
sudo apt-get install libgstreamer0.10-dev
sudo apt-get install libgstreamer-plugins-base0.10-dev
```
- 1
- 2
如果要安装其他软件如qq，微信等，则还需要先安装一个Genymotion-ARM-Translation-ARM架构支持包，因为模拟器是x86架构的，而qq、微信等则是基于ARM架构的应用。安装方法是直接将Genymotion-ARM-Translation-ARM安装包拖到模拟器中即可，Genymotion-ARM-Translation-ARM下载地址如下：
> 
[Genymotion-ARM-Translation-ARM架构支持包](http://download.csdn.net/detail/yyddhh123/9070315)
博主在Ubuntu 16.04上安装Genymotion后，发现无法上网，经过一番搜索后，发现只要在模拟器里面连接上那个默认的wifi就可以了，如果你安装Genymotion后发现无法上网，请参考下面这篇帖子解决：
> 
[Ubuntu 16.04 Genymotion无法上网解决方法](http://blog.csdn.net/u011692041/article/details/50630058)
上一张安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160713152422880)
## 17 安装cairo-dock，秒变mac os
cairo-dock是Linux上的一款美化软件，有一个类似mac os的dock栏，安装命令：
```
sudo add-apt-repository ppa:cairo-dock-team/ppa 
sudo apt-get update
sudo apt-get install cairo-dock cairo-dock-plug-ins
cairo-dock
```
- 1
- 2
- 3
- 4
记得自己设置主题，上一张我的dock：
![这里写图片描述](https://img-blog.csdn.net/20160713120116980)
## 18 修改快捷键
习惯了windows的快捷键，使用Ubuntu多少有点不习惯，尤其是 win+E，win+D 没法使用，简直很不方便啊，下面我们就自己来修改快捷键。
(1) 修改 win + e
首先，打开系统设置，选择键盘keyboard,选择快捷方式Shortcuts选项卡，点击最后一项自定义快捷键Custom Shortcuts，点击加号，添加一项，name随便写，commond 写 nautilus,如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160712194733545)
点击apply,然后点击右边的Disable，这时按下快捷键 win + e，如图：
![这里写图片描述](https://img-blog.csdn.net/20160713152734366)
在Linux中win键是super，上图右边 Super + E 就是打开文件夹的快捷键，现在试试 win + e，是不是熟悉的感觉。
(2) 修改 win + d
接着我们来设置 win + d 快捷键，在Ubuntu中直接按 win + ctrl + d 就能快速回到桌面，然而多按一个键总是不爽，下面我们就把回到桌面的快捷键改为 win + d。 
同样的打开快捷键设置，点位到导航栏Navigation–隐藏所有正常窗口 Hide all normal windows，点击右侧然后按下快捷键 win + d，如下图所示:
![这里写图片描述](https://img-blog.csdn.net/20160713101242229)
win + d有点麻烦，只设置这里不起作用，我们还要借助compizconfig-settings-manager这个工具来设置，接着在终端输入安装命令：
`sudo apt-get install compizconfig-settings-manager`- 1
在dash中打开compizconfig-settings-manager，找到Ubuntu Unity plugin（Ubuntu Unity插件），General（常规）里面有个Show Desktop（显示桌面），设置一下快捷键就OK了，可能会提示冲突，选择use anyway/无论如何使用就可以了，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160713152950464)
![这里写图片描述](https://img-blog.csdn.net/20160713153053700)
(3) 点击图标最小化窗口
接着上面的步骤，选择Launcher选项卡，然后勾选minimize single windows applications(unsupported) ，之后点击图标就能最小化窗口了，是不是很赞呢。
![这里写图片描述](https://img-blog.csdn.net/20160713153210499)
(4) 修改终端快捷键
Ubuntu 终端的复制、粘贴键分别是ctrl+shift+c、ctrl+shift+v，总觉得有点别扭，和windows不一样啊，下面我们来修改。 
ctrl + alt + t 打开终端，然后最大化，选择edit–perferences–shortcuts，定位到复制粘贴那一栏，然后设置快捷键为 ctrl + c、ctrl + v 就可以了，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160713102944251)
小提示：按退格键可以禁用快捷键哦
其他快捷键设置请自己百度吧。
## 19 安装eclipse
虽然eclipse已经有淘汰的趋势，但有些地方还是需要它的，例如：C++开发。首先到官网下载eclipse的安装包，请选择Java ee 64位版本的，下载地址如下：
> 
[Eclipse Linux版官网下载地址](https://www.eclipse.org/downloads/download.php?file=/oomph/epp/neon/R/eclipse-inst-linux64.tar.gz)
解压即可使用：
`tar -zxvf eclipse-jee-neon-R-linux-gtk-x86_64.tar.gz`- 1
进入eclipse目录，点击eclipse即可运行，上一张安装好的图:
![这里写图片描述](https://img-blog.csdn.net/20160720113448431)
注意：安装eclipse前必须安装并配置好JDK
## 20 安装截图工具 Shutter
Shutter号称是Linux下最强大的截图软件，截图后可以编辑。可以直接在Ubuntu软件中心搜索Shutter安装，也可以使用下面的命令安装：
```bash
sudo apt-get install shutter
```
- 1
上一个安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160720113736857)
当然Ubuntu自带了一个截图软件ScreenShot，直接在dash中搜索ScreenShot就可以打开了，博主比较喜欢用这个，因为操作简单。
## 21 安装视频播放器vlc
Linux的娱乐功能相比于windows是非常弱的，基本上可以用弱到爆来形容，音乐播放器尚且难找，视频播放器就更是寥寥了，好在，还是有的，这里推荐vlc，一个Linux上非常强大的播放器，而且还有windows版。Ubuntu 自带了一个视频播放器叫totem，直接点击视频文件就是用totem来播放的，搞得博主误以为是vlc，这里提醒大家一下，对视频文件点右键然后选择用vlc打开就可以了。vlc的安装方法有2种，可以直接在Ubuntu软件中心搜索vlc安装即可，也可以使用命令安装。 
Ubuntu 16.04 安装 vlc 的命令:
```
sudo add-apt-repository ppa:videolan/master-daily
sudo apt-get update
sudo apt-get install vlc
```
- 1
- 2
- 3
Ubuntu 16.04自带的Totem播放器比较弱，各种解码器都没有，用的时候还要去下载，每次右键选择vlc也比较累啊，所以我们设置默认播放器为vlc。打开系统设置，选择详细信息Details，选择默认应用Default Application，定位到视频Video，下拉选择 Vlc media player，如下图:
![这里写图片描述](https://img-blog.csdn.net/20160720114110597)
![这里写图片描述](https://img-blog.csdn.net/20160720114212307)
上一张安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160720114301428)
## 22 安装QQ
唉，Linux 下安装qq真是一言难尽啊，博主安装qq后的体会是:
> 
珍惜时间，远离Linux qq
在写这篇博客的这一段的时候，博主研究了一下Ubuntu 16.04安装qq，发现太折腾了，于是专门写了一篇博客记录Ubuntu 16.04下安装qq的方法，请看这里：
> 
[Ubuntu 16.04 安装QQ解决方案](http://blog.csdn.net/fuchaosz/article/details/51919607)
## 23 Ubuntu 16.04 和 windows 时间不同步的问题
每次进入Ubuntu 16.04后再回到windows，就会发现时间对不上，少了8小时，这是因为Ubuntu 16.04的时区设置不正确造成的，解决方法是执行下面的命令：
`sudo timedatectl set-local-rtc 1`- 1
## 24 安装有道词典
首先到有道词典官网下载deb安装包，注意有道词典Ubuntu版本只支持到Ubuntu 14.04,如果在Ubuntu 16.04上安装会失败，因为官方的Ubuntu版本的deb包依赖gstreamer0.10-plugins-ugly，但是该软件在16.04里面已经没有了。所以我们要下载64位的deepin版的安装包，经过博主测试，64位的deepin版的deb包在Ubuntu 16.04上安装成功，下载地址如下：
> 
[有道词典deepin版64位官网下载地址](http://cidian.youdao.com/index-linux.html)
安装命令：
`sudo apt-get install ./youdao-dict_1.1.0-0-deepin_amd64.deb`- 1
上一张安装好的图：
![这里写图片描述](https://img-blog.csdn.net/20160725153628448)
## 25 总结
这篇博客中所有的软件安装方法，都是博主在Ubuntu 16.04 上实践得来的，经过博主亲手实践，在安装过程中的体会就是：
> 
纸上得来终觉浅，绝知此事要躬行
如果你在安装过程中遇到问题，请给我留言，欢迎关注我的博客，转载请注明来自”梧桐那时雨”的博客。
## 26 转载请注明来自”梧桐那时雨”的博客:[http://blog.csdn.net/fuchaosz/article/details/51882935](http://blog.csdn.net/fuchaosz/article/details/51882935)
> 
Tips： 
如果觉得这篇博客对你有帮助或者喜欢博主的写作风格，就关注一下博主或者给博主留个言呗，鼓励博主创作出更多优质博客，Thank you.
===============================================================
## 2017年5月18日补充：
===============================================================
## 27 安装Sublime Text3
与windows一样，Ubuntu 16.04自带的编辑器Gedit也是比较弱的，我们需要升级一下编辑器，在windows下博主比较喜欢用notepad++，简单小巧启动快不花哨，但是notepad++没有Linux版，所以这里推荐一款更强大的编辑器Sublime Text3，Sublime Text3强大的地方就在于它不仅可以通过安装扩展包来不断增强功能，而且它是跨平台的，Sublime Text3可以说是目前最好用的编辑器之一啦，Ubuntu 16.04安装Sublime Text3的命令如下：
```
sudo add-apt-repository ppa:webupd8team/sublime-text-3
sudo apt-get update
sudo apt-get install sublime-text-installer
```
- 1
- 2
- 3
终端启动Sublime Text3的命令:
`subl`- 1
要用Sublime Text3打开文件a.txt则可以直接这样用:
`subl a.txt`- 1
卸载Sublime Text3命令:
`sudo apt-get remove sublime-text-installer`- 1
Sublime Text3强大的地方就在于它能够安装扩展包，要安装扩展包，首先必须安装插件管理器Package Control。打开Sublime Text3，按下快捷键 `ctrl+``(左上角第二排第一个键)或者`View > Show Console`调出输入框，然后粘贴下面的代码到输入框里面即可完成Package
 Control安装：
`import urllib.request,os,hashlib; h = 'df21e130d211cfc94d9b0905775a7c0f' + '1e3d39e33b79698005270310898eea76'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); print('Error validating download (got %s instead of %s), please try manual install' % (dh, h)) if dh != h else open(os.path.join( ipp, pf), 'wb' ).write(by)`- 1
附上Package Control官方教程：
> 
[https://packagecontrol.io/installation#st3](https://packagecontrol.io/installation#st3)
博主安装Sublime Text3后发现不能输入中文，于是又按照下面的步骤解决了中文输入问题：
```
git clone https://github.com/lyfeyaj/sublime-text-imfix.git
cd sublime-text-imfix
./sublime-imfix
```
- 1
- 2
- 3
这段代码就是用git下载sublime-text-imfix这个工程，然后执行sublime-imfix脚本来修复中文输入问题，执行完毕后注销重新登录就可以输入中文，Sublime Text3有很多提高效率的插件，百度一下就行了 
上一张博主安装好的图：
## 28 Chrome 使用uGet下载
博主已经无力吐槽FireFox了，尼玛，搞两个服务器，国内用一个服务器，国际上用另一个服务器，有一次登录的时候没切换到国际服务，直接登录了国内服务器上，结果一同步以前保存的书签全部被覆盖掉了，连个历史记录都没有，再看看360极速浏览器的书签增量备份，可以恢复到任意的历史备份时间点，简直良心，可惜Ubuntu不能用，于是转投chrome，之前用FireFox主要是下载东西可以用uGet，chrome的下载那真是….谁用谁知道，遂研究了一下chrome调用uGet来下载的方法，果断抛弃了FireFox。
版权声明：本文为博主原创文章，未经博主允许不得转载。
