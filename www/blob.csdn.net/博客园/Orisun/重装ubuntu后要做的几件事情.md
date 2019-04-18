# 重装ubuntu后要做的几件事情 - Orisun - 博客园







# [重装ubuntu后要做的几件事情](https://www.cnblogs.com/zhangchaoyang/articles/2039357.html)





root 帐号

sudo passwd root

.bashrc

alias cls='clear screen'

vimrc

安装vim后修改.vimrc文件内容如下：

" 高亮显示匹配的括号

set showmatch

" 继承前一行的缩进方式，特别适用于多行注释

set autoindent 

" 为C程序提供自动缩进

set smartindent 

" 使用C样式的缩进

set cindent 

" 制表符为4

set tabstop=4

 " 统一缩进为4

set softtabstop=4

set shiftwidth=4

" 所有制表符都转换为空格

set expandtab

let &termencoding=&encoding
set fileencodings=utf-8,gbk,ucs-bom,cp936　　　　　　　　"vim对中文编码的支持

fcitx


$ sudo apt-get install fcitx-table-wubi fcitx-config-gtk


此代码安装的是小企鹅五笔、拼音及双拼输入法。
如果没有fcitx-table-wubi，则只安装了拼音和双拼输入法，而没有五笔输入法。

gedit对中文编码的支持


gconftool-2 --set --type=list --list-type=string /apps/gedit-2/preferences/encodings/auto_detected "[UTF-8,CURRENT,GBK,BIG5-HKSCS,UTF-16]"

Adobe Flashplayer for Chromium and firefox


去[http://get.adobe.com/cn/flashplayer](http://get.adobe.com/cn/flashplayer)上在线安装，或者把包下载下来，解压后得到一个.so文件，把它放到/usr/lib/chromium-browser/plugins和/usr/lib/firefox-addons/plugins下

安装开发必备的一些库

sudo apt-get install auto-conf, libtool,

xutils-dev,zlib1g-dev,uuid-dev,libpcre3-dev,

接下来你就可以编译安装Larbin和Xapian了。












