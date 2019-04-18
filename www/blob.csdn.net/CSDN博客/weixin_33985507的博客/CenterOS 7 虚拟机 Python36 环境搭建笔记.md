# CenterOS  7  虚拟机 Python36 环境搭建笔记 - weixin_33985507的博客 - CSDN博客
2017年08月31日 14:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 安装虚拟机 （略）
## 解决问题：
![992984-1e95b97800805ee7.png](https://upload-images.jianshu.io/upload_images/992984-1e95b97800805ee7.png)
image.png
1、打开 vi /etc/sysconfig/network-scripts/ifcfg-eth0（每个机子都可能不一样，但格式会是“ifcfg-eth数字”），把ONBOOT=no，改为ONBOOT=yes
2、重启网络：service network restart
## 安装 net-tools
$ yum install -y net-tools
## 安装 wget
$ yum install -y wget
## 安装GCC 等依赖库
$ yum install -y gcc zlib zlib-devel openssl openssl-devel bzip2 bzip2-devl
## 下载 python36
$ mkdir /download
$ cd /download
$ wget [https://www.python.org/ftp/python/3.6.2/Python-3.6.2.tgz](https://link.jianshu.com?t=https://www.python.org/ftp/python/3.6.2/Python-3.6.2.tgz)
## 解压 python
$ tar zxvf Python-3.6.2.tgz
## 安装Python
$ cd Python-3.6.2
$ ./configure --prefix=/usr/local
$ make && make altinstall
## 安装pip
$ yum -y install epel-release
$ yum -y install python-pip
## 安装  virtualenv
$ pip install virtualenv
## 创建虚拟环境
$ cd Python3     // 创建一个文件夹
$ virtualenv -p /usr/local/bin/python3.6 py36env // 创建虚拟环境
$ source py36env/bin/activate // 激活虚拟环境
$ deactivate // 退出
## 其它
### 如果不先安装 openssl openssl-devel， 进入virtualenv环境会出现下面问题
![992984-fbad4cbe5e1e6b66.png](https://upload-images.jianshu.io/upload_images/992984-fbad4cbe5e1e6b66.png)
error.png
这时候要先安装openssl openssl-devel 两个库后，重新安装 python36 然后从新简历虚拟环境
### 切换pip 镜像
1）检查pip.conf文件是否存在
>> cd ~
>> mkdir .pip
>> ls ~/.pip
2）直接编辑pip.conf
$ sudo vi ~/.pip/pip.conf
[global]
index-url = [http://mirrors.aliyun.com/pypi/simple/](https://link.jianshu.com?t=http://mirrors.aliyun.com/pypi/simple/)
[install]
trusted-host = mirrors.aliyun.com
### 安装 Twisted
$ wget [https://pypi.python.org/packages/c0/7c/c1e5b61e30b7ffc96576d2a922615c8068e6996a622be813fc626cef07aa/Twisted-16.3.0.tar.bz2](https://link.jianshu.com?t=https://pypi.python.org/packages/c0/7c/c1e5b61e30b7ffc96576d2a922615c8068e6996a622be813fc626cef07aa/Twisted-16.3.0.tar.bz2)
$ tar jxvf Twisted-16.3.0.tar.bz2
$ cd Twisted-16.3.0
$ python setup.py install
