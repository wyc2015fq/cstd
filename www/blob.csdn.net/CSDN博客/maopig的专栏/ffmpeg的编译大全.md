# ffmpeg的编译大全 - maopig的专栏 - CSDN博客
2011年07月05日 20:31:39[maopig](https://me.csdn.net/maopig)阅读数：1178
# [ffmpeg的编译大全](http://www.cnblogs.com/MaxIE/archive/2007/02/03/638863.html)
最近互联网视频共享的网站很火，公司也想搞类似的网站，初步是用fms+ffmpeg形式 
fms负责在线录制，播放，ffmpeg则在后台处理上传的资源转换成一定的格式。 
为了让ffmpeg支持的格式尽量多，所以特把我的编译笔记分享一下 
环境：redhat as4 
1。首先获取ffmpeg 
很多人找不到怎么下载,其实之前ffmpeg可以通过cvs下载，不过最近他已经换成了更加强大的svn 
如何使用SVN我这里不再介绍，网上还有大量的安装和使用的文章可以借鉴，这里简单罗列几个SVN辅助的软件： 
SubVersion，从 [http://subversion.tigris.org/](http://subversion.tigris.org/) 下载，支持linux，我们这里就装这个 
TortoiseSVN，从 [http://tortoisesvn.tigris.org/](http://tortoisesvn.tigris.org/) 下载，是很不错的SVN客户端程序，为windows外壳程序集成到windows资源管理器和文件管理系统的Subversion客户端，用起来很方便，commit动作变得就像Winrar右键压缩一样方便。 
ok，那我们先装subversion，记住最好之前装过apr和apr-util，在apache.org网站能下到 
wget [http://subversion.tigris.org/downloads/subversion-1.3.2.tar.gz](http://subversion.tigris.org/downloads/subversion-1.3.2.tar.gz)
tar zvxf subversion-1.3.2.tar.gz 
cd subversion-1.3.2 
./configure --with-apr=/usr/local/apr-httpd --with-apr-util=/usr/local/apr-util-httpd/ 
make 
make install 
到此，我们就可以通过svn命令获取最新的ffmpeg了 
svn checkout svn://svn.mplayerhq.hu/ffmpeg/trunk ffmpeg 
你会发现在你所在的目录，自动出现一个ffmpeg的目录，就是你下载的源代码。 
我们还不能这么快编译ffmpeg，应该如果要让ffmpeg支持更多格式的转换，还需做一些前期工作 
2.支持mp3，linux当然是lame，下载解压 
cd lame-3.96.1 
./configure --enable-shared --prefix=/usr 
这里推荐尽量装在/usr下，默认是装在/usr/local下。这样ffmpeg编译都能顺利的找到库文件 
3.支持Ogg Vorbis: 
as4自带相应的rpm包，你可以安装一下如下rpm包 
libvorbis, libvorbis-devel,libogg, libogg-devel 
4.支持xvid x264，现在最流行的两种高质量的压缩格式 
xvid的编译安装 
wget [http://downloads.xvid.org/downloads/xvidcore-1.1.0.tar.gz](http://downloads.xvid.org/downloads/xvidcore-1.1.0.tar.gz)
tar zvxf xvidcore-1.1.0.tar.gz 
./configure --prefix=/usr 
make 
make install 
x264的获取同样是采用svn方式，看来svn取代cvs不远了 
svn co svn://svn.videolan.org/x264/trunk x264 
cd x264 
./configure --prefix=/usr --enable-shared 
make 
make install 
5.AC3和dts编码的支持 
as4系统似乎已经支持ac3编码，编译的时候只要加--enable-a52 --enable-gpl参数就行 
libdts编译参数 
./configure --prefix=/usr 
make 
make install 
6.mpg4 aac格式支持，由于服务器还针对手机用户服务，所以，类似aac，mpg4铃声格式的支持，我们也得做。这里我们安装faad2和faac就行 
下载请到[http://www.audiocoding.com/modules/mydownloads/](http://www.audiocoding.com/modules/mydownloads/)
FAAD2的编译 
cd faad2 
autoreconf -vif 
./configure --prefix=/usr --with-mp4v2 --enable-shared 
make 
make install 
faac的编译 
cd faac 
chmod +x bootstrap 
./bootstrap 
./configure --prefix=/usr --with-mp4v2 --enable-shared 
make 
make install 
7.支持3gp格式，这也是现在好多手机支持的格式，因为手机用户是我们的主要用户，所以也得支持编译 
编译的时候加上--enable-amr_nb --enable-amr_wb参数就行，根据编译系统的提示，所以我们得下载一 
些编译3gp所需得文件。 
wget [http://www.3gpp.org/ftp/Specs/archive/26_series/26.204/26204-510.zip](http://www.3gpp.org/ftp/Specs/archive/26_series/26.204/26204-510.zip)
解压以后把里面的文件都拷贝到libavcodec/amrwb_float 
wget [http://www.3gpp.org/ftp/Specs/archive/26_series/26.104/26104-510.zip](http://www.3gpp.org/ftp/Specs/archive/26_series/26.104/26104-510.zip)
解压以后把里面的文件都拷贝到libavcodec/amr_float 
好了，基本ffmpeg支持的格式，我们基本都做好前期准备了，下面是ffmpeg的编译 
./configure --prefix=/usr --enable-gpl --enable-shared --enable-mp3lame --enable-amr_nb --enable-amr_wb --enable-amr_if2 --enable-libogg --enable-vorbis --enable-xvid --enable-a52 --enable-a52bin --enable-faadbin --enable-dts --enable-pp --enable-faad --enable-faac --enable-x264 --enable-pthreads --disable-ffserver --disable-ffplay 
make 
make install 
补充1： 
关于3gp的编译，如果大家要编译--enable-amr_nb-fixed，那就不能跟--enable-amr_nb同时编译，我不大清楚这两者到底有什么区别，似乎fixed是修正版，管他呢，编译的方法： 
wget [http://www.3gpp.org/ftp/Specs/archive/26_series/26.073/26073-510.zip](http://www.3gpp.org/ftp/Specs/archive/26_series/26.073/26073-510.zip)
解压以后把里面的文件都拷贝到libavcodec/amr目录下 
修改libavcodec/amr/makefile 找到CFLAGS = -Wall -pedantic-errors -I. $(CFLAGS_$(MODE)) -D$(VAD) 换成CFLAGS = -Wall -I. $(CFLAGS_$(MODE)) -D$(VAD) -DMMS_IO 
整体编译参数就是 
./configure --prefix=/usr --enable-gpl --enable-shared --enable-mp3lame --enable-amr_nb-fixed --enable-amr_wb --enable-amr_if2 --enable-libogg --enable-vorbis --enable-xvid --enable-a52 --enable-a52bin --enable-dts --enable-pp --enable-faad --enable-faadbin --enable-faac --enable-x264 --enable-pthreads --disable-ffserver --disable-ffplay 
make 
make install 
补充2： 
有朋友说在当在cpu是x86的情况下，不能编译过去，我这里没发生过这情况，如果你真编译不过去，可以试着加上cpu=cpu 
ok,一个强大的ffmpeg编译好了，不过目前管方的ffmpeg还不支持wma9和rmvb，不过网上有很多解决方法，大家去自己找找。
