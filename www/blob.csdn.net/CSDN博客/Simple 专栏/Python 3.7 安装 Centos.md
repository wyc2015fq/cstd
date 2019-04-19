# Python 3.7 安装 Centos - Simple 专栏 - CSDN博客
2018年10月04日 19:58:29[Simple_Zz](https://me.csdn.net/love284969214)阅读数：91
首先安装依赖包，依赖包缺一不可，笔者曾安装python3未成功就是因为没有安装libffi-devel
```bash
sudo yum -y groupinstall "Development tools"
sudo yum -y install zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel readline-devel tk-devel gdbm-devel db4-devel libpcap-devel xz-devel libffi-devel
```
然后获取python3.7的安装包
```bash
wget https://www.python.org/ftp/python/3.7.0/Python-3.7.0.tgz
```
解压
```
cd ~/Download
tar -xzvf  Python-3.7.0.tar.xz
```
配置python3的安装目录并安装
```
cd Python-3.7.0
./configure
sudo make
sudo make install
```
查看软链接，没有创建
```
ln -s /usr/local/bin/python3/bin/python3 /usr/bin/python3
ln -s /usr/local/bin/python3/bin/pip3 /usr/bin/pip3
```
最后在命令行中输入python3，能够进入python3终端即成功安装
![](https://img-blog.csdn.net/201810041957473?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)查看python3 是否安装成功
