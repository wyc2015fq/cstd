# ubuntu16.04 使用 ppa 格式添加源（以添加并u安装Sublime Text 3 Installer为例） - 数据之美的博客 - CSDN博客
2018年01月07日 14:49:48[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1282
具体的添加命令可以在 下面的URL里面搜索
 https://launchpad.net/ubuntu
例如安装sublime时，可以在上面的搜索框里面输入sublime，回车之后会显示如下的字样:
|PPA name|Description|Sources|Binaries|
|----|----|----|----|
|[Sublime Text 3 Installer](https://launchpad.net/~webupd8team/+archive/ubuntu/sublime-text-3)|Sublime Text 3 Installer: the package in this PPA downloads an...|1|13|
此时可以点击　[Sublime Text 3 Installer](https://launchpad.net/~webupd8team/+archive/ubuntu/sublime-text-3)　，然后可以查看[Sublime
 Text 3 Installer](https://launchpad.net/~webupd8team/+archive/ubuntu/sublime-text-3)的源添加方式以及安装方式如下：
```
sudo add-apt-repository ppa:webupd8team/sublime-text-3
sudo apt-get update
sudo apt-get install sublime-text-installer
```
