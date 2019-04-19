# ubuntu 文件夹颜色显示 - 三少GG - CSDN博客
2015年04月24日 15:06:33[三少GG](https://me.csdn.net/scut1135)阅读数：2921
ubuntu  文件夹 颜色 显示
$ dircolors
$ vi ~/.bashrc
# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
eval "`dircolors -b`"
alias ls='ls --color=auto'
alias dir='dir --color=auto'
alias vdir='vdir --color=auto'
alias grep='grep --color=auto'
alias fgrep='fgrep --color=auto'
alias egrep='egrep --color=auto'
fi
###---------------- < my_dircolors Alyan @2009.03 > ------###
# .c .h .cc .cpp 文件 加粗显示
# README README.txt 加亮红色
# Makefile 加下划线
export LS_COLORS="$LS_COLORS:*README=01;31:*README.txt=01;31:*Makefile=
04;38:*.h=01;26:*.c=01;29:*.c c=01;26:*.cpp=01;29:"
###--------------------------------------------------------###
$ . ~/.bashrc
配置这个颜色以后感觉好多了，README Makefile 一目了然， .c .h文件也更加清晰.
==========================================================================
01 高亮 02 普通（暗色） 03 04 下划线
30黑 31紅 32青绿 33黃 34蓝 35紫 36浅蓝 37白 (同以前 BBS ANSI 色碼)
31 红色 32 青绿 34 蓝色（目录默认色） 39 普通白色 30- 加粗？
（40+ 背景色 40- 前景色）
n 白色：普通文件 （用-表示）
n 红色：压缩文件
n 蓝色：目录文件 （用d表示）
==========================================================================
01 高亮 02 普通（暗色） 03 04 下划线
30黑 31紅 32青绿 33黃 34蓝 35紫 36浅蓝 37白 (同以前 BBS ANSI 色碼)
31 红色 32 青绿 34 蓝色（目录默认色） 39 普通白色 30- 加粗？
（40+ 背景色 40- 前景色）
n 白色：普通文件 （用-表示）
n 红色：压缩文件
n 蓝色：目录文件 （用d表示）
n 青蓝色：链接文件 （用l表示）
n 黄色：设备文件（/dev目录下）（用b或c表示）
b表示的是物理设备;c表示的是字符终端设备.
n 青绿色：可执行文件（/bin、/sbin目录下）
n 粉红色：图片文件或是socket文件（用s表示）
n 青黄色：管道文件 （用p表示）
# file /dev/hda //查看指定文件的类型
# # find /dev -type b //在/dev目录中查找类型是设备的文件，可以将“b"换成上面的任
意类型表示字母，如l,c等。
