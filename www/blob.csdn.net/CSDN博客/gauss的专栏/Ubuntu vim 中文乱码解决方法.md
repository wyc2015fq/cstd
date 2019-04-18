# Ubuntu vim 中文乱码解决方法 - gauss的专栏 - CSDN博客
2015年04月21日 17:48:30[gauss](https://me.csdn.net/mathlmx)阅读数：459
Ubuntu vim 中文乱码解决方法
打开vim的配置文件，位置在/home/username/.vimrc或者/etc/vim/vimrc,我修改的是前者，
在其中加入
set fileencodings=utf-8,gb2312,gbk,gb18030
set termencoding=utf-8
set encoding=prc
保存退出，此时vim就能正确显示中文了。
支持中文显示
添加中文字符编码：
$sudo vim /var/lib/locales/supported.d/local
#添加下面的中文字符集
zh_CN.GBK GBK
zh_CN.GB2312 GB2312
zh_CN.GB18030 GB18030
使其生效：
$sudo dpkg-reconfigure locales
vim:
打开vim的配置文件，位置在/etc/vim/vimrc
在其中加入
set fileencodings=utf-8,gb2312,gbk,gb18030
set termencoding=utf-8
set encoding=prc
保存退出
source /etc/vim/vimrc
此时vim就能正确显示中文了。
vim /var/lib/locales/supported.d/local
设置默认的字符集
$sudo gedit /etc/environment
#第二行是默认字符集,设为zh_CH.UTF-8
LANG=zh_CH.UTF-8
LANGUAGAE="zh_CN:zh:en_US:en"
LANG=zh_CN.UTF-8 #不能选择GBK或GB2312,终端界面中文会乱码
LC_CTYLE=zh_CN.GB2312
GST_ID3_TAG_ENCODING=GB2312
$source /etc/environment使其生效
所有的字符集在/usr/share/i18n/charmaps目录下
locale的书写格式为：语言[_地域[.字符集][@修正值]]
