# 安装VIMP - 三少GG - CSDN博客
2012年06月27日 20:14:23[三少GG](https://me.csdn.net/scut1135)阅读数：6772
** 非常重要的安装向导！同样适用于phpmotion！！！**
[http://www.vimp.com/en/documentation-faq.html](http://www.vimp.com/en/documentation-faq.html)
[http://www.vimp.com/en/getting-started.html](http://www.vimp.com/en/getting-started.html)
**HowTo install the transcoding tools on Redhat, CentOS, etc.**
[http://www.vimp.com/en/documentation-faq-article/items/howto-install-the-transcoding-tools-on-redhat-centos-etc.html](http://www.vimp.com/en/documentation-faq-article/items/howto-install-the-transcoding-tools-on-redhat-centos-etc.html)
The ffmpeg packages for **RedHat-based Distributions (RHEL, CentOS, Fedora)** don´t support transcoding to all HTML5 formats. In detail there are problems with transcoding to m4v.
  This tutorial describes how to compile ffmpeg in order to support all HTML5 formats.
[root@uhz001022 ~]#**uname -a**Linux uhz001022 2.6.18-243**.el5**#1 SMP Mon Feb 7 18:47:27 EST 2011 x86_64 x86_64 x86_64 GNU/Linux
### We install the rpmforge and epel repository.
[Install rpmforge repository](http://wiki.centos.org/AdditionalResources/Repositories/RPMForge#head-c02c679d2b90b89efe3ac7f36f212c87bb1c17ab)
[Install epel repository](http://fedoraproject.org/wiki/EPEL/FAQ#How_can_I_install_the_packages_from_the_EPEL_software_repository.3F)
## 3. RPMforge for CentOS 5
Download the rpmforge-release package.  Choose one of the two links below, selecting to match your host's architecture.  If you are unsure of which one to use you can check your architecture with the command**uname -i**
- 
i386 [http://packages.sw.be/rpmforge-release/rpmforge-release-0.5.2-2.el5.rf.i386.rpm](http://packages.sw.be/rpmforge-release/rpmforge-release-0.5.2-2.el5.rf.i386.rpm)
- 
x86_64 [http://packages.sw.be/rpmforge-release/rpmforge-release-0.5.2-2.el5.rf.x86_64.rpm](http://packages.sw.be/rpmforge-release/rpmforge-release-0.5.2-2.el5.rf.x86_64.rpm)
Install the package 
rpm -i rpmforge-release-0.5.2-2.el5.rf.*.rpm
若装错了版本可 rpm -e rpmforge-release 然后重新安装el5等的
### Now let's install some basic packages. Existing packages will be updated or ignored:
yum install gcc make automake bzip2 unzip patch subversion libjpeg-devel yasm php-process 
### If you have already installed the packages ffmpeg and x264, remove them
yum remove ffmpeg x264
### Installing the win codecs of the MPlayer vendor:
**NOTE**: on **64 bit systems** install the codecs as follows:
wget http://www2.mplayerhq.hu/MPlayer/releases/codecs/essential-amd64-20071007.tar.bz2
tar xvjf essential-amd64-20071007.tar.bz2
mkdir /usr/local/lib/codecs
cp -Rvp essential-amd64-20071007/* /usr/local/lib/codecs/
Next, on both systems we have to extend the search path in /etc/ld.so.conf
joe /etc/ld.so.conf      
... and check, if the following lines exist within the file. If not, please add:
/usr/lib  
/usr/local/lib
### We install some codecs
yum install faac-devel  lame-devel amrnb-devel opencore-amr-devel amrwb-devel  libvorbis-devel xvidcore-devel 
### We install x264
wget ftp://ftp.videolan.org/pub/videolan/x264/snapshots/last_stable_x264.tar.bz2
tar xvjf last_stable_x264.tar.bz2
cd x264-snapshot-XXXXXXXX-XXXX-stable/
./configure --enable-shared --enable-pic
make && make install
cd ..
### We install libtheora
wget http://downloads.xiph.org/releases/theora/libtheora-1.1.1.tar.bz2
tar xvjf libtheora-1.1.1.tar.bz2
cd libtheora-1.1.1/
./configure
make
make install
cd ..
### We install libvpx:
wget http://webm.googlecode.com/files/libvpx-v0.9.7-p1.tar.bz2
tar xvjf libvpx-v0.9.7-p1.tar.bz2
cd libvpx-v0.9.7-p1
./configure --enable-shared
make
make install
cd ..
### ... and ffmpeg:
wget http://ffmpeg.org/releases/ffmpeg-0.7.12.tar.bz2
tar xvjf ffmpeg-0.7.12.tar.bz2
cd ffmpeg-0.7.12/
./configure --enable-gpl --enable-version3 --enable-shared --enable-nonfree --enable-postproc --enable-libfaac --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libxvid --enable-x11grab --extra-cflags="-I/usr/local/include" --extra-ldflags="-L/usr/local/lib" 
make
make install
### IMPORTANT:
- execute ldconfig
# ldconfig
- download the presets-package [here](http://www.vimp.com/en/documentation-faq-article/items/en/download-configurations.html) and copy the presets to */usr/local/share/ffmpeg*- tell ViMP the path to ffmpeg in the backend (administration -> configuration -> transcoder -> ffmpeg:
*/usr/local/bin/ffmpeg*- check if qt-faststart is installed (which qt-faststart). If not, you can download it
[here](http://www.vimp.com/en/documentation-faq-article/items/en/download-configurations.html).
### We install MPlayer, mencoder and flvtool2
yum install mplayer mencoder flvtool2
