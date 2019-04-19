# 安装PHPmotion - 三少GG - CSDN博客
2012年06月25日 15:37:36[三少GG](https://me.csdn.net/scut1135)阅读数：4096标签：[ruby																[svn																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=svn&t=blog)](https://so.csdn.net/so/search/s.do?q=ruby&t=blog)
个人分类：[Web网站设计																[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)](https://blog.csdn.net/scut1135/article/category/762263)
[http://bbs.chinaunix.net/thread-2086662-1-1.html](http://bbs.chinaunix.net/thread-2086662-1-1.html)
6.
[root@fc8 ~]# wget [http://rubyforge.org/frs/download.php/9225/flvtool2_1.0.5_rc6.tgz](http://rubyforge.org/frs/download.php/9225/flvtool2_1.0.5_rc6.tgz)
7.
[root@fc8 ~]# wget [http://nchc.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz](http://nchc.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz)
8.
[root@fc8 ~]# wget [http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz](http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz)
12.
[root@fc8 ~]# wget [http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz](http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz)
4.
[root@fc8 ~]# svn checkout svn://svn.mplayerhq.hu/ffmpeg/trunk ffmpeg
5.
[root@fc8 ~]# wget [http://jaist.dl.sourceforge.net/sourceforge/ffmpeg-php/ffmpeg-php-0.5.1.tbz2](http://jaist.dl.sourceforge.net/sourceforge/ffmpeg-php/ffmpeg-php-0.5.1.tbz2)
9.
[root@fc8 ~]# wget [http://www1.mplayerhq.hu/MPlayer/releases/MPlayer-1.0rc2.tar.bz2](http://www1.mplayerhq.hu/MPlayer/releases/MPlayer-1.0rc2.tar.bz2)
安装flvtool
[root@fc8 flvtool2_1.0.5_rc6]# yum install ruby
[root@fc8 flvtool2_1.0.5_rc6]# ruby setup.rb config
[root@fc8 flvtool2_1.0.5_rc6]# ruby setup.rb setup
[root@fc8 flvtool2_1.0.5_rc6]# ruby setup.rb install
安装libogg
[root@fc8 software]# tar -zxvf libogg-1.1.3.tar.gz 
[root@fc8 software]# cd libogg-1.1.3
[root@fc8 software]# ./configure;make;make install
安装libvorbis
[root@fc8 ~]# tar -zxvf libvorbis-1.2.0.tar.gz 
[root@fc8 ~]# cd libvorbis-1.2.0
[root@fc8 libvorbis-1.2.0]#./configure;make;make install
安装lame
[root@fc8 ~]# tar -zxvf lame-3.97.tar.gz
[root@fc8 ~]# cd lame-3.97 
[root@fc8 lame-3.97]# ./configure 
[root@fc8 lame-3.97]#make;make install
**error： mpglib_interface.c: In function `lame_decode1_headersB_clipchoice': ...**
** 【此问题实在无法解决，后通过安装vimp一文中 添加资源后自动安装~~~~】**
参考资料：
[http://www.hydrogenaudio.org/forums//lofiversion/index.php/t30980.html](http://www.hydrogenaudio.org/forums//lofiversion/index.php/t30980.html)
[http://gqf2008.iteye.com/blog/495214](http://gqf2008.iteye.com/blog/495214)
[http://blog.sina.com.cn/s/blog_5fbdba500100cwbb.html](http://blog.sina.com.cn/s/blog_5fbdba500100cwbb.html)
[http://www.yanghengfei.com/archives/488/](http://www.yanghengfei.com/archives/488/)

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//再次尝试安装~~~
## [Install Phpmotion on Ubuntu 9.10 视频网站搭建](http://hi.baidu.com/zj8la8la/item/461bb9fa21a9216a3d1485f4)
Install Phpmotion on Ubuntu 9.10
The requirements
- download phpmotion
- aptitude install mysql-server
- aptitude install phpmyadmin
- aptitude install ffmpeg flvtool2 mencoder
- aptitude install php5-ffmpeg php5-curl curl
to create phpmotion cms, for example : www.myphpmotion.com, follow these steps :
- extract phpmotion.zip to <pathphpmotion>
- edit /etc/php5/apache2/php.ini
- open_basedir = (no value)
- upload_max_filesize = 100M
- post_max_size = 100M
- max_execution_time = 1500
- session.gc_maxlifetime = 14000
- safe_mode = off
- enable_dl = On
- extension=/<pathphpmotion>/phpshield/phpshield.5.2.lin
- cp /etc/apache2/site-avalaible/default /etc/apache2/site-enabled/virtual1
- edit /etc/apache2/site-enabled/virtual1 :
- ServerName www.myphpmotion.com
- DocumentRoot /<pathphpmotion>
- AllowOverride All
- scriptalias /cgi-bin/ /<pathphpmotion>/cgi-bin/
- <Directory /<pathphpmotion>/cgi-bin/>
- a2enmod rewrite –> activate rewrite, so .htaccess can be excuted
- /etc/init.d/apache restart
- chmod 777 -R /<pathphpmotion>
- create new database, for example : phpmotion, you can create it via phpmyadmin
- edit /etc/hosts :
- 127.0.0.1 www.myphpmotion.com
- open browser, then access http://www.myphpmotion/setup, then follow the setup process and skip ftp questions
- finish setup, remove / rename setup directory, example :
- mv /<pathphpmotion>/setup /<pathphpmotion>/setup1
- now there are two addresses can be accessed
- www.myphpmotion.com/siteadmin
- configure phpmotion
- make category for video upload
- status conversion video/audio
- www.myphpmotion.com
- for viewer / user only
- register user
- upload data
- view video/audio/image
Solve captcha image which is not appearing in registration form, by fixing font’s path
- edit /<pathphpmotion>/includes/captcha.php
- var $font = ‘/<pathphpmotion/includes/DoradoHeadline.ttf’;
Monggo dicobi…..good luck
安装phpmotion用配置服务器开始到安装partOne
2010-03-27 17:21:34
标签：[服务器](http://blog.51cto.com/tagindex.php?keyword=%B7%FE%CE%F1%C6%F7)[休闲](http://blog.51cto.com/tagindex.php?keyword=%D0%DD%CF%D0)[partOne](http://blog.51cto.com/tagindex.php?keyword=partOne)[职场](http://blog.51cto.com/tagindex.php?keyword=%D6%B0%B3%A1)[phpmotion](http://blog.51cto.com/tagindex.php?keyword=phpmotion)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://upfont.blog.51cto.com/716420/288539) 、作者信息和本声明。否则将追究法律责任。[http://upfont.blog.51cto.com/716420/288539](http://upfont.blog.51cto.com/716420/288539)
> 
很久没有更新博客了,说来惭愧啊,这几个月都在上课,学的东西还是蛮多的,马上就上第三期课程了,没有底啊,都上了很久了从去年的10月份开始到今天才上了到第二期,比较杯具,工作依旧没有着落的样子(广告下:我在找工作,有意者请联系我给我发邮件吧,upfont@msn.com,限上海地区),只怪自己学历不够高,技术不够好,人品比较差.ok 不废话了.开始我们的phpmotion之旅.
phpmotion 是一个视频分享整站程序,类似优酷这类的吧,这个是他的demo站==>[点我进入](http://demo.phpmotiontemplates.com/v3/default/).
这是一个Ts([Thinksns](http://www.thinksns.com/index.html))的群友发出来的站,说能有中文版的话应该会比较不错,然后我就当一下吃螃蟹的人,去他们的phpmotion的官方下了一个包看看,大概性的看了下,他已经留好了翻译的接口,只要按照他的文件翻译就可以了,可是这个杯具的事情发生了,这个phpmotion,不是一个普通的cms,他需要支持的东西比较多,看了下他们wiki看来下服务器需求如下:
- 
PHP 4.3 and above (including support of CLI)
- [MySQL](http://wiki.phpmotion.com/MySQL/edit) database server
- 
LAME MP3 Encoder
- 
Libogg + Libvorbis
- 
Mencoder and also Mplayer
- 
FFMpeg-PHP
- 
GD Library 2 or higher
- 
CGI-BIN
- 
Be able to run background processes
唉~看来比较悲剧,要的是linux 系统,算了,在家休息也不能闲着,复习下linux的命令也不错,接着gg了一阵发现一篇很有见地的文章.《[FFmpeg,
 FFmpeg-PHP, Lame, Libogg, Libvorbis, FLVtool2, Mplayer, Mencoder, AMR Installation](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation)》 是edebian的,正好我有ubuntu的服务器版,核心差不多可以试下 哈哈,接着开始了命令狂打状态(多为复制).中间有比较多的错误,要注意下.
准备工作,这是必须要做的,不做无法进行一下几步(切记!)
sudo su  //切换到root 
 apt-get update 
 apt-get upgrade 
apt-get install libjpeg-progs libjpeg62 libjpeg62-dev libsdl1.2-dev php5-dev build-essential unzip
 ///安装所需要的库和部分软件
> 
接着是下载我们需要的文件
cd /usr/local/src 
wget[ttp://rubyforge.org/frs/download.php/17497/flvtool2-1.0.6.tgz](http://www3.mplayerhq.hu/MPlayer/releases/codecs/essential-20071007.tar.bz2)
wget[ttp://easynews.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz](http://easynews.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz)
wget [http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz](http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz)
wget[ttp://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz](http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz)
wget [http://ftp.penguin.cz/pub/users/utx/amr/amrnb-7.0.0.2.tar.bz2](http://ftp.penguin.cz/pub/users/utx/amr/amrnb-7.0.0.2.tar.bz2)
wget [http://ftp.penguin.cz/pub/users/utx/amr/amrwb-7.0.0.3.tar.bz2](http://ftp.penguin.cz/pub/users/utx/amr/amrwb-7.0.0.3.tar.bz2)
wget[ttp://downloads.xiph.org/releases/theora/libtheora-1.0beta3.tar.gz](http://downloads.xiph.org/releases/theora/libtheora-1.0beta3.tar.gz)
wget [http://repository.slacky.eu/slackware-12.1/multimedia/ffmpeg/20081129/src/ffmpeg-20081129.tar.gz](http://repository.slacky.eu/slackware-12.1/multimedia/ffmpeg/20081129/src/ffmpeg-20081129.tar.gz)
wget [http://www.mplayerhq.hu/MPlayer/releases/mplayer-checkout-snapshot.tar.bz2](http://www.mplayerhq.hu/MPlayer/releases/mplayer-checkout-snapshot.tar.bz2)
> 
把下载下来的文件解压出来
tar zxvf lame-3.97.tar.gz
 tar zxvf libogg-1.1.3.tar.gz 
 tar zxvf libvorbis-1.2.0.tar.gz 
 tar zxvf flvtool2-1.0.6.tgz 
 tar zxvf ffmpeg-20081129.tar.gz 
 tar zxvf mplayer-checkout-snapshot.tar.bz2 
 tar jxvf essential-20071007.tar.bz2 
 bzip2 -cd amrnb-7.0.0.2.tar.bz2 | tar xvf - 
 bzip2 -cd amrwb-7.0.0.3.tar.bz2 | tar xvf -
 tar zxvf libtheora-1.0beta3.tar.gz
> 
一个编码库
mkdir /usr/local/lib/codecs/
> 
      安装Ruby on Rails, subversion & ncurses
apt-get install subversion ruby libcurses-ruby
从svn上获得ffmpeg-php(之前我是用压缩包,但是我写这个文的时候那个下载地址找不到了,用svn也是一样的效果,除非有不兼容的情况,上帝保佑)
//svn checkout https://ffmpeg-php.svn.sourceforge.net/svnroot/ffmpeg-php ffmpeg-php 这2句 我不太肯定 
svn co https://ffmpeg-php.svn.sourceforge.net/svnroot/ffmpeg-php ffmpeg-php cd /usr/local/src/ffmpeg-php 
//没有目录就mkdir一个吧 eg:mkdir ffmpeg-php svn update
复制mplayer编码库文件
mkdir /usr/local/lib/codecs
 mv /usr/local/src/essential-20071007/* /usr/local/lib/codecs/ 
chmod -R 755 /usr/local/lib/codecs/
一个临时目录
mkdir /usr/local/src/tmp
 chmod 777 /usr/local/src/tmp
 export TMPDIR=/usr/local/src/tmp
安装 lame
cd /usr/local/src/lame-3.97 
./configure && make && make install
安装 libogg
cd /usr/local/src/libogg-1.1.3 
./configure && make && make install
安装 libvorbis
cd /usr/local/src/libvorbis-1.2.0 
./configure && make && make install 
如果出现 
/*********************************这里是分隔符**************************/ 
出错提示: *** Could not run Ogg test program, checking why... *** The test program compiled, but did not run. This usually means *** that the run-time linker is not finding Ogg or finding the wrong *** version of Ogg. If it is not finding Ogg, you'll need to set
 your *** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point *** to the installed location Also, make sure you have run ldconfig if that *** is required on your system *** *** If you have an old version installed, it is best to remove it,
 although *** you may also be able to get things to work by modifying LD_LIBRARY_PATH configure: error: must have Ogg installed! 
解决: 
# vi /etc/ld.so.conf 
include /etc/ld.so.conf.d/*.conf 
/user/local/lib 
# ldconfig –v 
/*************************这里是分隔符**************************/ 
我就在这里卡了很久没弄明白,然后突然看到一个文=>>>>>>>>[点](http://www.ptubuntu.com/2009/10/3150.html)
安装flvtool2
cd /usr/local/src/flvtool2-1.0.6/ 
ruby setup.rb config
 ruby setup.rb setup
 ruby setup.rb install
安装mplayer & mencoder
cd /usr/local/src/mplayer 
./configure --enable-jpeg
 make && make install
安装 AMR (for 3gp conversion)
cd /usr/local/src/amrnb-7.0.0.2 
./configure && make && make install
cd /usr/local/src/amrwb-7.0.0.3 
./configure && make && make install
安装 libtheora (for ogg video encoding)
cd /usr/local/src/libtheora-1.0beta3 
./configure && make && make install
安装ffmpeg
cd /usr/local/src/ffmpeg-20081129
./configure --prefix=/usr/local --enable-memalign-hack --enable-libvorbis --disable-mmx --enable-shared
make && make install 
ln -s /usr/local/lib/libavdevice.so.52 /usr/lib/libavdevice.so.52 
ln -s /usr/local/lib/libavformat.so.52 /usr/lib/libavformat.so.52 
ln -s /usr/local/lib/libavcodec.so.52 /usr/lib/libavcodec.so.52
 ln -s /usr/local/lib/libavutil.so.49 /usr/lib/libavutil.so.49 
ln -s /usr/local/lib/libmp3lame.so.0 /usr/lib/libmp3lame.so.0
 ln -s /usr/local/lib/libavformat.so.51 /usr/lib/libavformat.so.51
 ln -s /usr/local/lib/libamrnb.so.3 /usr/lib/libamrnb.so.3 
ln -s /usr/local/lib/libamrwb.so.3 /usr/lib/libamrwb.so.3 
//用ln添加硬链接
安装ffmpeg-php
cd /usr/local/src/ffmpeg-php-0.5.0/ 
phpize
 ./configure && make make install
修改php.ini
vim /etc/php5/apache2/php.ini
 extension=ffmpeg.so (add this line to the end of the file)
重启apache2
/etc/init.d/apache2 force-reload
到这里大环境已经配置完成了~也花了一个下午才写好~悲剧啊 partTwo就是安装phpmotion 了,安装的时候还会有一个很bt的问题等着我们呢待续
![](http://blog.51cto.com/image/skin/artType01.jpg) 安装phpmotion用配置服务器开始到安装PartTwo
2010-04-10 15:12:27
标签：[服务器](http://blog.51cto.com/tagindex.php?keyword=%B7%FE%CE%F1%C6%F7)[休闲](http://blog.51cto.com/tagindex.php?keyword=%D0%DD%CF%D0)[PartTwo](http://blog.51cto.com/tagindex.php?keyword=PartTwo)[phpmotion](http://blog.51cto.com/tagindex.php?keyword=phpmotion)[职场](http://blog.51cto.com/tagindex.php?keyword=%D6%B0%B3%A1)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://upfont.blog.51cto.com/716420/293397) 、作者信息和本声明。否则将追究法律责任。[http://upfont.blog.51cto.com/716420/293397](http://upfont.blog.51cto.com/716420/293397)
时隔很久才发现我的PartTwo还没写,后来发现写这个PartTwo没有什么意义了,原因就是这个程序有点残疾,在中国的市场貌似不是很大,想想还是算了吧,最近也忙着写自己的毕业设计,都不知道做个什么样的好.
但是还是提一下PartTwo 重要的东西吧
最重要的还是一个叫 phpshield 的东西 ,一开始没有装他就错误说我没有这个组件 ,好吧 没有我们就给他加上去
首先到 [http://www.phpshield.com/loaders/](http://www.phpshield.com/loaders/) 找到你的系统版本的文件 下载下来然后解压到
/usr/lib/php5/20060613+lfs 文件夹下
然后重启下apache2,
$ sudo /etc/init.d/apache2 restart
接着
就可以正常安装了,好吧
安装的话也就是下一步一步的样子
~___________________________________________-
写的很粗糙,还请大家谅解下,找工作找的很迷茫,就这样吧
# FFmpeg, FFmpeg-PHP, Lame, Libogg, Libvorbis, FLVtool2, Mplayer, Mencoder, AMR Installation
## Contents
 [[hide](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#)] 
- [1Update](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Update)
- [2Introduction](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Introduction)
- [3Do
 some prep-work](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Do_some_prep-work)
- [4Download
 all the files needed](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Download_all_the_files_needed)
- [5Extract
 all the files](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Extract_all_the_files)
- [6Install
 Ruby on Rails, subversion & ncurses](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_Ruby_on_Rails.2C_subversion_.26_ncurses)
- [7Run
 some SVN queries](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Run_some_SVN_queries)
- [8Copy
 Codecs for mplayer](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Copy_Codecs_for_mplayer)
- [9Install
 lame](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_lame)
- [10Install
 libogg](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_libogg)
- [11Install
 libvorbis](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_libvorbis)
- [12Install
 flvtool2](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_flvtool2)
- [13Install
 mplayer & mencoder](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_mplayer_.26_mencoder)
- [14Install
 AMR (for 3gp conversion)](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_AMR_.28for_3gp_conversion.29)
- [15Install
 libtheora (for ogg video encoding)](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_libtheora_.28for_ogg_video_encoding.29)
- [16Install
 ffmpeg](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_ffmpeg)
- [17Install
 ffmpeg-php](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Install_ffmpeg-php)
- [18Restart
 & done](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Restart_.26_done)
- [19Troubleshooting](http://linux.justinhartman.com/FFmpeg,_FFmpeg-PHP,_Lame,_Libogg,_Libvorbis,_FLVtool2,_Mplayer,_Mencoder,_AMR_Installation#Troubleshooting)
## Update
This HOWTO has been updated to work with the latest ffmpeg from subversion. There were compile issues prior to this and as of **7 February 2009** this now works as expected on
 Debian Etch and Ubuntu 8.04.
## Introduction
The following HOWTO will show you exactly how to install the following packages on a Debian Etch or Ubuntu 7.06 system:
- FFmpeg
- FFmpeg-PHP
- Mplayer + Mencoder
- flv2tool
- LAME MP3 Encoder
- AMR (for 3gp file conversions)
- Libogg
- Libvorbis
## Do some prep-work
apt-get update
apt-get upgrade
apt-get install libjpeg-progs libjpeg62 libjpeg62-dev libsdl1.2-dev php5-dev build-essential unzip
## Download all the files needed
cd /usr/local/src
wget [http://www3.mplayerhq.hu/MPlayer/releases/codecs/essential-20071007.tar.bz2](http://www3.mplayerhq.hu/MPlayer/releases/codecs/essential-20071007.tar.bz2)
wget [http://rubyforge.org/frs/download.php/17497/flvtool2-1.0.6.tgz](http://rubyforge.org/frs/download.php/17497/flvtool2-1.0.6.tgz)
wget [http://easynews.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz](http://easynews.dl.sourceforge.net/sourceforge/lame/lame-3.97.tar.gz)
wget [http://superb-west.dl.sourceforge.net/sourceforge/ffmpeg-php/ffmpeg-php-0.5.0.tbz2](http://superb-west.dl.sourceforge.net/sourceforge/ffmpeg-php/ffmpeg-php-0.5.0.tbz2)
wget [http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz](http://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz)
wget [http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz](http://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz)
wget [http://ftp.penguin.cz/pub/users/utx/amr/amrnb-7.0.0.2.tar.bz2](http://ftp.penguin.cz/pub/users/utx/amr/amrnb-7.0.0.2.tar.bz2)
wget [http://ftp.penguin.cz/pub/users/utx/amr/amrwb-7.0.0.3.tar.bz2](http://ftp.penguin.cz/pub/users/utx/amr/amrwb-7.0.0.3.tar.bz2)
wget [http://downloads.xiph.org/releases/theora/libtheora-1.0beta3.tar.gz](http://downloads.xiph.org/releases/theora/libtheora-1.0beta3.tar.gz)
## Extract all the files
tar zxvf lame-3.97.tar.gz
tar zxvf libogg-1.1.3.tar.gz
tar zxvf libvorbis-1.2.0.tar.gz
tar zxvf flvtool2-1.0.6.tgz
tar jxvf essential-20071007.tar.bz2
tar jxvf ffmpeg-php-0.5.0.tbz2
bzip2 -cd amrnb-7.0.0.2.tar.bz2 | tar xvf -
bzip2 -cd amrwb-7.0.0.3.tar.bz2 | tar xvf -
tar zxvf libtheora-1.0beta3.tar.gz
We need a codec directory
mkdir /usr/local/lib/codecs/
## Install Ruby on Rails, subversion & ncurses
apt-get install subversion ruby libcurses-ruby
## Run some SVN queries
svn checkout [svn://svn.mplayerhq.hu/ffmpeg/trunk]() ffmpeg
svn checkout [svn://svn.mplayerhq.hu/mplayer/trunk]() mplayer
cd /usr/local/src/mplayer
svn update
## Copy Codecs for mplayer
mkdir /usr/local/lib/codecs
mv /usr/local/src/essential-20071007/* /usr/local/lib/codecs/
chmod -R 755 /usr/local/lib/codecs/
We also need to secure the tmp directory
mkdir /usr/local/src/tmp
chmod 777 /usr/local/src/tmp
export TMPDIR=/usr/local/src/tmp
## Install lame
cd /usr/local/src/lame-3.97
./configure
make && make install
## Install libogg
cd /usr/local/src/libogg-1.1.3
./configure && make && make install
## Install libvorbis
cd /usr/local/src/libvorbis-1.2.0
./configure && make && make install
## Install flvtool2
cd /usr/local/src/flvtool2-1.0.6/
ruby setup.rb config
ruby setup.rb setup
ruby setup.rb install
## Install mplayer & mencoder
cd /usr/local/src/mplayer
./configure --enable-jpeg
make && make install
## Install AMR (for 3gp conversion)
cd /usr/local/src/amrnb-7.0.0.2
./configure
make && make install
cd /usr/local/src/amrwb-7.0.0.3
./configure
make && make install
## Install libtheora (for ogg video encoding)
cd /usr/local/src/libtheora-1.0beta3
./configure
make && make install
## Install ffmpeg
Debian Etch and Ubuntu 8.04 users please use the following configure command:
cd /usr/local/src/ffmpeg/
./configure --enable-libmp3lame --enable-libvorbis --disable-mmx --enable-shared --enable-libamr-nb --enable-libamr-wb --enable-nonfree --enable-libtheora
Ubuntu 7.06 users please use the following configure command:
./configure --enable-libmp3lame --enable-libogg --enable-libvorbis --disable-mmx --enable-shared --enable-libamr-nb --enable-libtheora
Now run these commands:
make
make install
ln -s /usr/local/lib/libavdevice.so.52 /usr/lib/libavdevice.so.52
ln -s /usr/local/lib/libavformat.so.52 /usr/lib/libavformat.so.52
ln -s /usr/local/lib/libavcodec.so.52 /usr/lib/libavcodec.so.52
ln -s /usr/local/lib/libavutil.so.49 /usr/lib/libavutil.so.49
ln -s /usr/local/lib/libmp3lame.so.0 /usr/lib/libmp3lame.so.0
ln -s /usr/local/lib/libavformat.so.51 /usr/lib/libavformat.so.51
ln -s /usr/local/lib/libamrnb.so.3 /usr/lib/libamrnb.so.3
ln -s /usr/local/lib/libamrwb.so.3 /usr/lib/libamrwb.so.3
## Install ffmpeg-php
cd /usr/local/src/ffmpeg-php-0.5.0/
phpize
./configure
make
make install
You now need to add the new ffmpeg-php module to the php.ini file
vim /etc/php5/apache2/php.ini
extension=ffmpeg.so (add this line to the end of the file)
## Restart & done
/etc/init.d/apache2 force-reload
## Troubleshooting
Read the [discussion
 page](http://linux.justinhartman.com/Talk:FFmpeg%2C_FFmpeg-PHP%2C_Lame%2C_Libogg%2C_Libvorbis%2C_FLVtool2%2C_Mplayer%2C_Mencoder%2C_AMR_Installation) |
