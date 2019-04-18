# centos安装xrdp实现远程桌面访问 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年05月08日 10:42:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4088








                
1.配置环境：

yum install gcc pam-devel openssl-devel -y

2.进入指定目录下载并解压xrdp：

wget http://xsdou.googlecode.com/files/xrdp-0.4.2.tar.gz

tar -xzvf xrdp-0.4.2.tar.gz

3.进入解压后xrdp的目录编译和安装：

cd xrdp-0.4.2

make

make install

4.启动xrdp：

 cd /usr/local/xrdp

 ./xrdp_control.sh start

5.xrdp的配置文档在/etc/xrdp目录下的xrdp.ini和sesman.ini

6.参考：http://www.xsdou.com/computer/1572.html



