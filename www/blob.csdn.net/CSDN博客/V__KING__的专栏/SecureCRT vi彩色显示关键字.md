# SecureCRT vi彩色显示关键字 - V__KING__的专栏 - CSDN博客





2013年11月19日 16:38:56[v__king__](https://me.csdn.net/V__KING__)阅读数：3440








## SecureCRT vi彩色显示关键字


vi是没有颜色的,但是我们在linux的控制台下用vi编辑文件却能看到彩色


这是因为安装了vim-enhanced的包


linux做了一些别名


所以我们运行vi实际上运行的是vi的增强版本vim.


安装好了 vim-enhanced后,默认在控制台就是彩色显示了


相关设置可以打开vi ~/.vimrc来修改


不过我们在远程ssh终端上有可能还是看不到彩色.主要有2个原因.


1.term类型不对,不支持彩色.在secureCRT上设置


Options->SessionOptions ->Emulation,然后把Terminal类型改成xterm，并点中ANSI Color复选框。


然后ls看看,发现文件名和目录已经是彩色了.但是可能vi打开某些文件依然不是彩色


按:进入命令模式输入syntax on也没有效果.这是因为vi没有识别你编译的文件类型


在命令模式下输入:set filetype=c


C是c语言程序文件,如果是jsp,就用jsp代替,其他类推.


然后就可以变成彩色了.





三、此时如果没有彩色显示的话

在/[etc](http://www.c114.net/keyword/etc)/profile中加入

export TERM=xterm-color




