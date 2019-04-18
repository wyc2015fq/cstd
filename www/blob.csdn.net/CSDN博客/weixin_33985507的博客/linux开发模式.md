# linux开发模式 - weixin_33985507的博客 - CSDN博客
2013年07月15日 23:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
linux已被使用vim【文本编辑】+gcc【编译】+【gdb代码调试】开发模式
简单设置下开发环境,像设定vim的语法高亮,编辑c时代码自动缩进,tab缩进字符,显示行号等
编辑vinrc一般vin的配置文件在~/.vimrc
但有时是在/etc/vimrc.,说不定又有可能在别的地方,我们用find / -name vimrc
找到后设定
set cindent shiftwidth=4     //C语言自动缩进,缩进值为4个字符宽度.
set ts=4   //tab键 4字符缩进
set number   //显示行号
syntax on   //关键字高亮

