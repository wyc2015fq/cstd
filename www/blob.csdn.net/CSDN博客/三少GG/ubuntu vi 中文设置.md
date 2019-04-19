# ubuntu vi 中文设置 - 三少GG - CSDN博客
2015年05月07日 07:59:56[三少GG](https://me.csdn.net/scut1135)阅读数：550
1.
**添加中文字符编码：**
$sudo vim /var/lib/locales/supported.d/local
#添加下面的中文字符集
zh_CN.GBK GBK
zh_CN.GB2312 GB2312
zh_CN.GB18030 GB18030
使其生效：
$sudo dpkg-reconfigure locales
2.
**vim:**
打开vim的配置文件，位置在/etc/vim/vimrc
在其中加入
set fileencodings=utf-8,gb2312,gbk,gb18030
set termencoding=utf-8
set encoding=prc
保存退出
source /etc/vim/vimrc
此时vim就能正确显示中文了。
