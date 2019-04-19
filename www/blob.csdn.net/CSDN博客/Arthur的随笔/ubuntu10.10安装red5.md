# ubuntu10.10安装red5 - Arthur的随笔 - CSDN博客
2011年10月21日 15:07:18[largetalk](https://me.csdn.net/largetalk)阅读数：1444标签：[ubuntu																[subversion																[deb																[服务器																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=deb&t=blog)](https://so.csdn.net/so/search/s.do?q=subversion&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)
echo "deb  http://archive.canonical.com/ubuntu maverick partner" >> /etc/apt/sources.list
apt-get update
apt-get install sun-java6-jdk sun-java6-jre an subversion
设置好JAVA_HOME
下载red5, 我下载了red5-1.0.0-RC1.tar.gz
tar xvfz red5-1.0.0-RC1.tar.gz
mv red5-1.0.0 /usr/share/red5
cd /usr/share/red5
export RED5_HOME=`pwd`
./red5.sh
到htpp://127.0.0.1:5080看服务器是否启动成功
