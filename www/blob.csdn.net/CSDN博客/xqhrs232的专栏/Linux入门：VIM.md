# Linux入门：VIM - xqhrs232的专栏 - CSDN博客
2016年03月29日 09:39:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：447
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址：[http://blog.csdn.net/xiazdong/article/details/7959495](http://blog.csdn.net/xiazdong/article/details/7959495)
相关文章
1、[linux 下vim的使用（必看）](http://blog.csdn.net/xiaolong2w/article/details/8224839)----[http://blog.csdn.net/xiaolong2w/article/details/8224839](http://blog.csdn.net/xiaolong2w/article/details/8224839)
2、[[Linux/Ubuntu]
 vi/vim 使用方法讲解](http://www.cnblogs.com/emanlee/archive/2011/11/10/2243930.html)----[http://www.cnblogs.com/emanlee/archive/2011/11/10/2243930.html](http://www.cnblogs.com/emanlee/archive/2011/11/10/2243930.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
练习vim按键的小游戏：[http://vim-adventures.com/](http://vim-adventures.com/)
wiki：文本编辑器的比较：[http://zh.wikipedia.org/wiki/%E6%96%87%E4%BB%B6%E7%BC%96%E8%BE%91%E5%99%A8%E6%AF%94%E8%BE%83](http://zh.wikipedia.org/wiki/%E6%96%87%E4%BB%B6%E7%BC%96%E8%BE%91%E5%99%A8%E6%AF%94%E8%BE%83)
joe download：[http://sourceforge.net/projects/joe-editor/files/JOE%20sources/joe-3.7/joe-3.7.tar.gz/download?use_mirror=nchc&download=](http://sourceforge.net/projects/joe-editor/files/JOE%20sources/joe-3.7/joe-3.7.tar.gz/download?use_mirror=nchc&download=)
**一、vi、vim介绍**
文本编辑器分为：
(1)基于图形界面，如gedit、geany等；
(2)基于命令行，如nano、vi、joe等；
每个命令行界面的文本编辑器都有各自不同的快捷键，比如nano的保存为CTRL+W，vim的保存为:w；
基于命令行的文本编辑器是必要的，因为Linux的tty1~tty6都是命令行界面，是打不开图形界面的文本编辑器的；
vi是每个Linux Distribution都默认安装的文本编辑器，并且很多软件的默认编辑器都是vi，因此学习vi是很必要的；
vim是vi的高级版本，vi是文本编辑器，而vim应该说是程序编辑器，因为能够像一般的IDE一样，根据文件名载入不同的语法高亮，即颜色区分，比如java文件会遵循java的语法高亮；
下面我们就编写一个"hello world"的java程序来看看vim的语法高亮；
![](https://img-my.csdn.net/uploads/201209/09/1347149860_7288.png)
可以看出vim有很好的语法检查功能，因为关键字都使用了不同的颜色标出；
vi有三种模式：
(1)一般模式：进入vi的默认模式，能够进行复制、黏贴、删除等操作；
(2)编辑模式：从一般模式按下‘i’即可进入；
(3)命令行模式：从一般模式按下“:”、'?'、'/'即可进入；
注意：如果安装了vim后，输入vi后也是使用vim编辑器，我们输入alias后看到“alias vi='vim'”；
**二、vi/vim按键**
.
重复前一个操作
:! command
暂时离开vim，并执行command，执行完后再进入vim
|:r filename|将filename文件的数据读入当前文件|
|:set all|显示当前vim的环境配置|
**三、vim的恢复机制**
vim正在编辑某个文件时都会同时存在 .file.swp，此文件用来暂存，帮助恢复文件内容，当vim正常关闭此文件时，.filename.swp文件会消失；
![](https://img-my.csdn.net/uploads/201209/09/1347149875_4511.png)
我们看到图中说明了出现此界面的两个原因：
(1)多人同时编辑此文件：因为Linux是多用户的操作系统，因此可能两个人同时登陆并编辑此文件，如果A进入系统开始编辑1.txt文件，则会出现.1.txt.swp，当A还没编辑完但B也想进去编辑时，因为此文件的目录中存在.1.txt.swp，则就会出现上图的界面；
(2)非正常关闭文件；
图中最后一行说明了我们此时可以采取的几个动作：
(1)o：以只读方式打开；
(2)R：恢复，即从swp文件中恢复，但是swp文件不会随着vim关闭而删除，需要手动删除；
(3)d：删除swp文件；
(4)q：退出vim；
**四、vim记录及默认配置文件**
/etc/vimrc文件为全局vim配置文件；
1.如果我们用xiazdong账户使用vim后，在/home/xiazdong就会出现.viminfo文件，此文件用来作为vim的日志，记录用户使用vim打开了什么文件，做了什么操作；
2.如果xiazdong账户想要配置vim的默认环境，则可以在/home/xiazdong创建.vimrc，并且配置一些特征，比如显示行号，语法检查等；
一般使用如下配置：
set hlsearch
set backspace=2
set autoindent
set nu
set ruler
set showmode
set bg=dark
syntax on
**五、Visual Block功能**
可以用于块复制；
(1)CTRL+v：开始复制块；
(3)y：复制块；
(4)p：黏贴块；
(5)d：删除块；
![](https://img-my.csdn.net/uploads/201209/09/1347149878_9383.png)
**六、同时编辑多个文件**
多文件编辑的好处是能够将一个文件的某些内容yy后，p到另一个文件中；
vim file1 file2 即可同时编辑两个文件，但是屏幕中同时只会出现一个文件内容，需要进行切换；
(1):n：向下切换；
(2):N：向上切换；
(3):files：列出当前编辑的所有文件；
**七、多窗口编辑**
如下图：一个界面能够显示多个文件内容；
![](https://img-my.csdn.net/uploads/201209/09/1347149891_1195.png)
(1) :sp ：打开当前文件；
(2) :sp filename：打开filename文件；
(1)CTRL+w+↓：光标切换到下一个窗口；
(2)CTRL+w+↑：光标切换到上一个窗口；
(3):q：关闭光标所在的窗口；
**八、编码问题**
可能会出现乱码问题，解决方法：保持终端的字符编码与文件的字符编码一致！
tty1~tty6不支持显示中文，因此在tty1~tty6中显示中文必出现乱码；
设置终端的字符编码：
![](https://img-my.csdn.net/uploads/201209/09/1347149928_5847.png)
编码转换命令：iconv
iconv -f 文件的原本编码 -t 文件的新编码 filename -o newfilename
比如有一个big5编码的文件：1.big5，要转换成utf8的文件：2.utf8，则： iconv -f big5 -t utf8 1.big5 -o 2.utf8
