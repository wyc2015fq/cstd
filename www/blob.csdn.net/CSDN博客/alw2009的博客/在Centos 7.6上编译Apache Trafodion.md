# 在Centos 7.6上编译Apache Trafodion - alw2009的博客 - CSDN博客
2018年12月28日 00:02:31[traveler_zero](https://me.csdn.net/alw2009)阅读数：29标签：[开源																[大数据																[Trafodion																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=Trafodion&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=开源&t=blog)
个人分类：[Trafodion																[开源																[数据库](https://blog.csdn.net/alw2009/article/category/8567837)](https://blog.csdn.net/alw2009/article/category/8567836)](https://blog.csdn.net/alw2009/article/category/8567835)
参考： [https://cwiki.apache.org/confluence/display/TRAFODION/Trafodion+Contributor+Guide](https://cwiki.apache.org/confluence/display/TRAFODION/Trafodion+Contributor+Guide)
**目录**
[一、安装依赖](#%E4%B8%80%E3%80%81%E5%AE%89%E8%A3%85%E4%BE%9D%E8%B5%96)
[二、下载源码](#%E4%BA%8C%E3%80%81%E4%B8%8B%E8%BD%BD%E6%BA%90%E7%A0%81)
[三、编译](#%E4%B8%89%E3%80%81%E7%BC%96%E8%AF%91)
[四、查看编译结果](#%E5%9B%9B%E3%80%81%E6%9F%A5%E7%9C%8B%E7%BC%96%E8%AF%91%E7%BB%93%E6%9E%9C)
[五、编译结果说明](#%E4%BA%94%E3%80%81%E7%BC%96%E8%AF%91%E7%BB%93%E6%9E%9C%E8%AF%B4%E6%98%8E)
[六、安装运行](#%E5%85%AD%E3%80%81%E5%AE%89%E8%A3%85%E8%BF%90%E8%A1%8C)
### 一、安装依赖
```
sudo yum install -y epel-release
sudo yum install -y alsa-lib-devel ant ant-nodeps apr-devel apr-util-devel \
         boost-devel cmake device-mapper-multipath dhcp doxygen flex \
         gcc-c++ gd git glibc-devel glibc-devel.i686 graphviz-perl gzip \
         java-1.7.0-openjdk-devel libX11-devel libXau-devel libaio-devel \
         libcurl-devel libibcm.i686 libibumad-devel libibumad-devel.i686 \
         libiodbc libiodbc-devel librdmacm-devel librdmacm-devel.i686 \
         libxml2-devel lua-devel lzo-minilzo \
         net-snmp-devel net-snmp-perl openldap-clients openldap-devel \
         openldap-devel.i686 openmotif openssl-devel openssl-devel.i686 \
         perl-Config-IniFiles perl-Config-Tiny \
         perl-DBD-SQLite perl-Expect perl-IO-Tty perl-Math-Calc-Units \
         perl-Params-Validate perl-Parse-RecDescent perl-TermReadKey \
         perl-Time-HiRes protobuf-compiler protobuf-devel \
         readline-devel rpm-build saslwrapper sqlite-devel libuuid-devel\
         unixODBC unixODBC-devel uuid-perl wget xerces-c-devel xinetd ncurses-devel
```
 从输出消息可以看到有些包安装失败了，可以忽略
```
No package ant-nodeps available.
No package libibcm.i686 available.
No package libibumad-devel.i686 available.
No package librdmacm-devel.i686 available.
```
安装java-1.7.0-openjdk (注意还不支持1.8）
```
sudo yum install -y java-1.7.0-openjdk java-1.7.0-openjdk-devel
rpm -ql java-1.7.0-openjdk
export JAVA_HOME=/usr/lib/jvm/java-1.7.0-openjdk-1.7.0.201-2.6.16.1.el7_6.x86_64
echo -en "export JAVA_HOME=/usr/lib/jvm/java-1.7.0-openjdk-1.7.0.201-2.6.16.1.el7_6.x86_64\n" >> ~/.bashrc
```
### 二、下载源码
`git clone https://github.com/apache/trafodion.git`
### 三、编译
 运行脚本自动下载、编译、安装依赖， 这一步可能比较久， 有VPN会好一些。如果编译失败， 大多情况都是因为网络原因。
```
mkdir ~/trafodion-download ~/trafodion-build-tools
./traf_tools_setup.sh -d ~/trafodion-download -i ~/trafodion-build-tools
export TOOLSDIR=$HOME/trafodion-build-tools
echo -en "export TOOLSDIR=$HOME/trafodion-build-tools\n" >> $HOME/.bashrc
PATH=$PATH:$TOOLSDIR/apache-maven-3.3.3/bin
echo -en "export PATH=\$PATH:\$TOOLSDIR/apache-maven-3.3.3/bin\n" >> $HOME/.bashrc
cd mysource/trafodion
source ./env.sh
make package-all
```
### 四、查看编译结果
编译完成后可以检查distribution目录， 可以看到相应的安装包
```
distribution]$ ls
apache-trafodion_clients-2.4.0-RH-x86_64-debug.tar.gz  dcs-tests.tgz
apache-trafodion_pyinstaller-2.4.0.tar.gz              phoenix-tests.tgz
apache-trafodion-regress.tgz                           RH
apache-trafodion_server-2.4.0-RH-x86_64-debug.tar.gz   traf_ambari-2.4.0-1.noarch.rpm
distribution]$ ls RH/
apache-trafodion_server-2.4.0-devel.x86_64.rpm
```
### 五、编译结果说明
apache-trafodion_clients-2.4.0-RH-x86_64-debug.tar.gz是客户端的包， 包含JDBC, ODBC驱动， 还有数据库终端工具trafci和数据加载工具odb。
dcs-tests包含了一些JDBC, ODBC的测试集。
apache-trafodion_pyinstaller-2.4.0.tar.gz 是非常方便的python安装脚本。
phoenix-tests.tgz 测试也是JDBC相关的测试
apache-trafodion-regress.tgz 是SQL引擎的测试集
apache-trafodion_server-2.4.0-RH-x86_64-debug.tar.gz 是Apache-trafodion的主安装文件
traf_ambari-2.4.0-1.noarch.rpm 是ambari安装包？
RH 目录里的是Trafodion的rpm安装包。
### 六、安装运行
因为没有集群环境， 这里就不演示安装了。 单机也可以运行， 推荐使用16G内存以上的机器， 我是虚拟机内存太小就不演示了。
