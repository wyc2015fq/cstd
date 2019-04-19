# ubuntu16.04(64位)安装wps及相关问题解决 - 数据之美的博客 - CSDN博客
2017年12月17日 17:37:10[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：520
本人ubuntu新人一个，最近想在ubuntu里编辑文本，但无奈系统自带LibreOffice比较坑爹，所以研究了一下怎么装WPS，遇到各种问题，最后成功解决，经验给大家分享一下。
        1. wps官网下载wps for linux的deb安装包，下载地址：linux.wps.cn，可以选择Beta版下载
        2.下载之后，有的人双击安装，但是有可能会有错误，这里建议使用命令行安装：sudo dpkg -i wps-office_8.1.0.3724~b1p2_i386.deb，等一会应该就装好了
        3.在命令行运行wps，如果成功运行，恭喜你，你很幸运；如果不幸出错，接着往下看
        4.先检查一下32位依赖包有没有装，sudo apt-get install libc6-i386，sudo apt-get install ia32-libs，一般来说第二个会出问题，因为ia32-libs已经被废除了，可以安装一下代替ia32-libs的包，这个在你运行了第二个命令之后就会有提示，自己装一下，如果无法定位，没关系，跳到下一步
        5.第四步完成之后再输一次wps试一下，一般还是有问题，这个时候会提示缺少某某lib，这个时候我们可以使用apt-file来搜索我们需要的包，先安装apt-file：sudo apt-get install apt-file，这个时候，如果刚刚的信息显示缺少xxx，你就用apt-file搜索，apt-file search xxx，然后就会得到包含xxx的包，再用apt-get来安装它，注意安装时在最后要加上:i386，例如 sudo apt-get install xxx:i386，ok，你再试试运行wps（我讲的不清楚的话可以百度apt-file的用法）
        6.还是有错误？没事，继续按第五步的方法安装包，最后一定会成功的（我当时是安装了三个包才成功的）
        7.哇，最后终于打开了，但显示缺少字体啊，没事，上万能的CSDN上搜一下就搜到了，下载地址：http://download.csdn.net/download/wl1524520/6333049，下载并解压得到几个ttf格式的文件，把这几个文件移动到/usr/share/fonts/wps-office下，我用的傻办法，使用mv命令一个一个移动的（大神们看到的话告诉我有没有简单点的移动方法），然后重启wps，ok，成功！
        8.打开之后不能用搜狗输入法，在命令行输入sudo gedit `/usr/bin/wps，打开之后在文件中`“#!/bin/bash”下一行插入：
**[plain]**[view
 plain](http://blog.csdn.net/qq_33867991/article/details/53081220#)[copy](http://blog.csdn.net/qq_33867991/article/details/53081220#)
- export XMODIFIERS="@im=fcitx"  
- export QT_IM_MODULE="fcitx"  
- 
