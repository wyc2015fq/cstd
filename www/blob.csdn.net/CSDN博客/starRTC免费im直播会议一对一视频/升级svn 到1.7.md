# 升级svn 到1.7 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月22日 18:41:58[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1101
sudo yum update
sudo yum groupinstall "Development tools"
sudo yum groupinstall "Additional Development"
wget https://archive.apache.org/dist/subversion/subversion-1.7.8.tar.gz
tar zxvf subversion-1.7.8.tar.gz
cd subversion-1.7.8
./get-deps.sh
./configure
make
make check
sudo make install
On my system this seems to put the binary in /usr/local/bin/svn
whereas the 1.6 binary is in /usr/bin/svn so you might need set up an alias.
