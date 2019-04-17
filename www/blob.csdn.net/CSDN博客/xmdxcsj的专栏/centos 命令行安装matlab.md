# centos 命令行安装matlab - xmdxcsj的专栏 - CSDN博客





2015年12月22日 21:21:31[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2852








一、安装

1.挂载

sudo mkdir /mnt/matlab

sudo mount -o loop /path-to-matlab.iso /mnt/matlab




2.安装

cd /mnt/matlab

sudo ./install -mode silent -agreeToLicense yes -fileInstallationKey 12345-67890-12345-67890 -licensePath /pathto/Crack/license_405329_R2014a.lic -tmpdir /pathto/bigmemory




3.破解

sudo cp /pathto/Crack/Linux/libmwservices.so /usr/local/MATLAB/R2014a/bin/glnxa64/




4.激活

第一种方法：

新建activate.ini文件：

isSilent=true

activateCommand=activateOffline

licenseFile=/pathto/Crack/license_405329_R2014a.lic

执行命令：/usr/local/MATLAB/R2014a/bin/activate_matlab.sh -propertiesFile /pathto/activate.ini

如果失败可以采用第二种方法




第二种方法：

vim ~/.bashrc

alias matlab="/usr/local/MATLAB/R2014a/bin/matlab -c /pathto/Crack/license_405329_R2014a.lic"




6.卸载

unmount /mnt/matlab




二、安装错误：

1. 问题：Error: Cannot install in the specified folder because there may be files remaining from a previous installation.

解决：上一次安装不成功引起，删除文件

sudo rm -rf /usr/local/MATLAB




2. 问题：The following error was detected while installing vision_demos_common:

archive is not a ZIP archive

解决：因为/tmp目录太小，需要制定一个大的目录

-tmpdir /pathto/bigmemory



