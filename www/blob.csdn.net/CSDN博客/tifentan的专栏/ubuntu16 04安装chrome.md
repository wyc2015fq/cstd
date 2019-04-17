# ubuntu16.04安装chrome - tifentan的专栏 - CSDN博客





2016年12月09日 16:06:04[露蛇](https://me.csdn.net/tifentan)阅读数：1136









ubuntu 公司估计和Google公司有些许不同意见，我们使用apt-get，还是图形软件中心都无法找到Chrome浏览器。


对于这种情况，我们生可以忍，熟不能忍。





下面简述安装方法：





大部分的教程都是如此：


一、通过直接下载安装Google Chrome浏览器deb包。

打开Ubuntu终端，以下为32位版本，使用下面的命令。

wget https://dl.google.com/[Linux](http://lib.csdn.net/base/linux)/dire ... le_current_i386.deb

以下为64位版本，使用下面的命令。

wget https://dl.google.com/linux/dire ... e_current_amd64.deb

下载好后

32 位安装命令:

sudo dpkg -i google-chrome-stable_current_i386.deb

64 位安装命令:

sudo dpkg -i google-chrome-stable_current_amd64.deb

最后执行

sudo apt-get install google-chrome

完成Chrome的安装

二、添加 Google Chrome 的PPA

安装Google Chrome浏览器官方PPA，打开终端然后运行下面的命令，下载签名密钥：

wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add

sudo sh -c 'echo "deb http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google-chrome.list'

sudo apt-get update

sudo apt-get install google-chrome

安装Google Chrome unstable 版本：

sudo apt-get install google-chrome-beta

安装Google Chrome beta 版本：

sudo apt-get install google-chrome-unstable






你完全按照上面做，很显然安装不了了。因此，我修正一下。主要原因如下，chrome不支持32位了，部分命名也修改了，导致无法使用。


我只推荐PPA方法，免翻墙，否则，你得翻墙下载chrome，你Firefox VPN配置好了吗！！！


wget -q -O - https://raw.githubusercontent.com/longhr/ubuntu1604hub/master/linux_signing_key.pub | sudo apt-key add



sudo sh -c 'echo "deb [ arch=amd64 ] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google-chrome.list'


sudo apt-get update



sudo apt-get install google-chrome-stable





PS.

安装Google Chrome unstable 版本：

sudo apt-get install google-chrome-beta

安装Google Chrome beta 版本：

sudo apt-get install google-chrome-unstable



