# Linux四次练习后感 - weixin_33985507的博客 - CSDN博客
2015年10月25日 20:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
对于习惯了windows的用户来说，学习linux是相当不容易的，从图形界面转换到命令行是一个不容易的过程，我们需要适应，经过四次的联系之后，我已经慢慢适应了用命令行控制计算机。
从查看在线用户到查看ip检测网络情况，从更改，复制，删除，移动文件到压缩解压文件，我学习的知识在变得越来越深。
接下来我介绍一下我通过linux公社中学习的如何在Ubuntu下安装搜狗输入法。
**1. 下载：**
打开地址：[http://packages.linuxdeepin.com/deepin/pool/main/f](https://link.jianshu.com?t=http://packages.linuxdeepin.com/deepin/pool/main/f/)/，从目录下查找如下软件：
1 fcitx_4.2.6.1-2deepin2_all.deb
2 fcitx-bin_4.2.6.1-2deepin2_i386.deb
3 fcitx-data_4.2.6.1-2deepin2_all.deb
4 fcitx-frontend-all_4.2.6.1-2deepin2_all.deb
5 fcitx-frontend-gtk2_4.2.6.1-2deepin2_i386.deb
6 fcitx-frontend-gtk3_4.2.6.1-2deepin2_i386.deb
7 fcitx-frontend-qt4_4.2.6.1-2deepin2_i386.deb
8 fcitx-libs_4.2.6.1-2deepin2_i386.deb
9 fcitx-module-dbus_4.2.6.1-2deepin2_i386.deb
10 fcitx-modules_4.2.6.1-2deepin2_i386.deb
11 fcitx-module-x11_4.2.6.1-2deepin2_i386.deb
12 fcitx-pinyin_4.2.6.1-2deepin2_i386.deb
13fcitx-skin-sogou_0.0.2_all.deb
14fcitx-sogoupinyin_0.0.0-2_i386.deb
15 fcitx-table_4.2.6.1-2deepin2_i386.deb
16 fcitx-table-all_4.2.6.1-2deepin2_all.deb
17 fcitx-table-wubi_4.2.6.1-2deepin2_i386.deb
18 fcitx-tools_4.2.6.1-2deepin2_i386.deb
19 fcitx-ui-classic_4.2.6.1-2deepin2_i386.deb
20 gir1.2-fcitx-1.0_4.2.6.1-2deepin2_i386.deb
**2. 卸载所有的fcitx输入法的包**，
因为使用ubuntu自带的fcitx下载安装sougou输入法的包是行不通的。使用终端输入
sudo dpkg --get-selections | grep fcitx
可以查看你的软件是否已经卸载了。
**3. 在第一步下载的软件目录中执行如下命令安装下载的包：**
sudo dpkg -i *.deb
上面列出了20个包，如下17个应该是必须的：
fcitx
fcitx-bin
fcitx-config-common
fcitx-config-gtk
fcitx-data
fcitx-frontend-gtk2
fcitx-frontend-gtk3
fcitx-frontend-qt4
fcitx-googlepinyin
fcitx-libs
fcitx-module-dbus
fcitx-module-x11
fcitx-modules
fcitx-pinyin
fcitx-table
fcitx-table-wubi
fcitx-ui-classic
**4. 最后安装一个配置工具：**
sudo apt-get install fcitx-config-gtk
**5. 打开语言支持和输入法切换**，
将输入法设置成fcitx，注销。
