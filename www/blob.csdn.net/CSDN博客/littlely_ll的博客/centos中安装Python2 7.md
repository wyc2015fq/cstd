# centos中安装Python2.7 - littlely_ll的博客 - CSDN博客





2017年05月03日 21:05:15[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：298标签：[python																[centos](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/littlely_ll/article/category/6664856)








转载于：[秋水逸冰 » CentOS 6.8安装Python2.7.13](https://teddysun.com/473.html)

# 查看当前系统中的 Python 版本

python –version 

返回 Python 2.6.6 为正常。

# 检查 CentOS 版本

cat /etc/redhat-release 

返回 CentOS release 6.8 (Final) 为正常。

# 安装所有的开发工具包

yum groupinstall -y “Development tools”

# 安装其它的必需包

yum install -y zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel

# 下载、编译和安装 Python 2.7.13

wget [https://www.python.org/ftp/python/2.7.13/Python-2.7.13.tgz](https://www.python.org/ftp/python/2.7.13/Python-2.7.13.tgz)

tar zxf Python-2.7.13.tgz 

cd Python-2.7.13 

./configure 

make && make install 

默认 Python 2.7.13 会安装在 /usr/local/bin 目录下。
ll -tr /usr/local/bin/python*

/usr/local/bin/python2.7 

/usr/local/bin/python2.7-config 

/usr/local/bin/python -> python2 

/usr/local/bin/python2 -> python2.7 

/usr/local/bin/python2-config -> python2.7-config 

/usr/local/bin/python-config -> python2-config 

而系统自带的 Python 是在 /usr/bin 目录下。
ll -tr /usr/bin/python*

/usr/bin/python2.6-config 

/usr/bin/python2.6 

/usr/bin/python 

/usr/bin/python2 -> python 

/usr/bin/python-config -> python2.6-config 

更新系统默认 Python 版本
先把系统默认的旧版 Python 重命名。

mv /usr/bin/python /usr/bin/python.old 

再删除系统默认的 python-config 软链接。

rm -f /usr/bin/python-config 

最后创建新版本的 Python 软链接。

ln -s /usr/local/bin/python /usr/bin/python 

ln -s /usr/local/bin/python-config /usr/bin/python-config 

ln -s /usr/local/include/python2.7/ /usr/include/python2.7 

以上步骤做完以后，目录 /usr/bin 下的 Python 应该是
ll -tr /usr/bin/python*

/usr/bin/python2.6-config 

/usr/bin/python2.6 

/usr/bin/python.old 

/usr/bin/python2 -> python 

/usr/bin/python -> /usr/local/bin/python 

/usr/bin/python-config -> /usr/local/bin/python-config
# 查看新的 Python 版本

python –version 

返回 Python 2.7.13 为正常。

以下步骤还是有必要的

# 为新版 Python 安装 setuptools

wget [https://bootstrap.pypa.io/ez_setup.py](https://bootstrap.pypa.io/ez_setup.py) -O - | python 

（转者注：在这个安装中好像出现了错误，最后使用 

wget [https://bootstrap.pypa.io/get-pip.py](https://bootstrap.pypa.io/get-pip.py)，然后再python get-pip.py才行，然而有的还得在wget [https://bootstrap.pypa.io/get-pip.py](https://bootstrap.pypa.io/get-pip.py)后面加上–no-check-certificate才行。） 

setuptools 正确安装完成后，easy_install 命令就会被安装在 /usr/local/bin 目录下了。
# 为新版 Python 安装 pip

easy_install pip 

正确安装完成后，pip 命令就会被安装在 /usr/local/bin 目录下了。

为新版 Python 安装 distribute 包（可选）

pip install distribute 

至此，新版 Python 即算安装完毕了。

注意：这可能会导致以前安装过的 Python 程序运行不了或者无法重启之类的（比如著名的 Shadowsocks Python 版）。原因是旧版的 pkg_resources 位于 /usr/lib/python2.6/site-packages 下。而新版的则是在 /usr/local/lib/python2.7/site-packages 下。 

所以，也许你需要重新安装一下程序。

再次注意：升级 Python 可能会导致 yum 命令不可用。解决方法如下： 

编辑 /usr/bin/yum 文件，将开头第一行的

#!/usr/bin/python 

改为

#!/usr/bin/python2.6 

但是，这种改法，万一哪天你 yum update 了一下，yum 被升级了后，又变回老样子了。 

所以，经过我的不懈琢磨和努力，发现了一个办法（网上的解决方案都是抄来抄去，没有一点卵用）。 

记住旧版本 Python 2.6.6 的重要路径如下所示，在运行 yum 命令的时候，会提示你哪个 module 不存在，不存在的我们就去旧版本的路径下找，一定能找到的。找到后，复制到新版本 Python 的路径 /usr/local/lib/python2.7/site-packages/ 下即可。
/usr/lib/python2.6/site-packages/ 

/usr/lib64/python2.6/site-packages/ 

我的复制过程是这样的：
cp -r /usr/lib/python2.6/site-packages/yum /usr/local/lib/python2.7/site-packages/ 

cp -r /usr/lib/python2.6/site-packages/rpmUtils /usr/local/lib/python2.7/site-packages/ 

cp -r /usr/lib/python2.6/site-packages/iniparse /usr/local/lib/python2.7/site-packages/ 

cp -r /usr/lib/python2.6/site-packages/urlgrabber /usr/local/lib/python2.7/site-packages/ 

cp -r /usr/lib64/python2.6/site-packages/rpm /usr/local/lib/python2.7/site-packages/ 

cp -r /usr/lib64/python2.6/site-packages/curl /usr/local/lib/python2.7/site-packages/ 

cp -p /usr/lib64/python2.6/site-packages/pycurl.so /usr/local/lib/python2.7/site-packages/ 

cp -p /usr/lib64/python2.6/site-packages/_sqlitecache.so /usr/local/lib/python2.7/site-packages/ 

cp -p /usr/lib64/python2.6/site-packages/sqlitecachec.py /usr/local/lib/python2.7/site-packages/ 

cp -p /usr/lib64/python2.6/site-packages/sqlitecachec.pyc /usr/local/lib/python2.7/site-packages/ 

cp -p /usr/lib64/python2.6/site-packages/sqlitecachec.pyo /usr/local/lib/python2.7/site-packages/


## **注意，有时候按照上述操作仍然有出错，安装后，它提示需要安装什么包，直接安装后再编译一下python就行了。（转者注）**















