# NDK 安装小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年10月23日 15:16:15[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：156

建议先看后面的错误，可以先执行那些命令以免出现相关问题。
chmod a+x android-ndk-r10c-linux-x86_64.bin
./android-ndk-r10c-linux-x86_64.bin
错误：
/lib/ld-linux.so.2: bad ELF interpreter: 没有那个文件或目录
是因为64位系统中安装了32位程序，解决方法：
yum install glibc.i686 -y
重新安装以后如果还有如下类似错误
error while loading shared libraries: libstdc++.so.6: cannot open shared object file: No such file or directory
再继续安装包
yum install libstdc++.so.6  -y
Ubuntu安装错误 
bash: ./android-ndk-r10c-linux-x86_64.bin: No such file or directory
apt-get install p7zip-full
7z x *bin

win:将ndk路径加到path环境变量中
linux:
export NDK_ROOT=/home/soft/android-ndk-r9d
export PATH=$PATH:$NDK_ROOT
