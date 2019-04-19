# Linux系统下如何编译ACE6.1.5版本 - 2019跟着小虎玩着去软考 - CSDN博客
2012年10月30日 12:39:12[littletigerat](https://me.csdn.net/littletigerat)阅读数：2469
**Linux系统下如何编译ACE6.1.5版本**
# 一、下载
Ace的官方网站
http://www.cs.wustl.edu/~schmidt/ACE.html
下载地址
http://download.dre.vanderbilt.edu/
# 二、最新的版本
ACE-6.1.5.tar.gz
6.1.5(截止2012年10月30日星期二)
# 三、解压
将ACE-6.1.5.tar.gz解压到到/home/aceuser/ace
cd /home/aceuser/ace
tar -zxvf ACE-6.1.5.tar.gz
# 四、设置环境变量
#vi /home/aceuser/.bashrc
加入下面两行：
exportACE_ROOT=/home/aceuser/ace/ACE_wrappers  
exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ACE_ROOT/ace
运行source /home/aceuser/.bashrc,让环境变量生效。
# 五、设置Linux编译环境
#cd  /home/aceuser/ace/ACE_wrappers/ace/  
#ln -s ./config-linux.h ./config.h  
#cd  /home/aceuser/ace/ACE_wrappers/include/makeinclude/  
#ln -s ./platform_linux.GNU ./platform_macros.GNU
# 六、编译
编译命令：
make  -f  GNUmakefile
