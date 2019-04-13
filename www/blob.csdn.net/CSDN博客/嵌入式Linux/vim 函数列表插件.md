
# vim 函数列表插件 - 嵌入式Linux - CSDN博客

2015年09月24日 18:03:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1006


１．apt-get install exuberant-ctags
２．unzip taglist_42.zip　　把解压出来的文件放到 /home/weiqifa(自己的用户名)/.vim/　　资源下载：[http://download.csdn.net/detail/weiqifa0/9137283](http://download.csdn.net/detail/weiqifa0/9137283)
３．sudo vim /etc/vim/vimrc
加上下面的代码：
1."""""""""""""""""Taglist设置"""""""""""""""""
let Tlist_Auto_Open = 1
let Tlist_Ctags_Cmd = '/usr/bin/ctags'
let Tlist_Show_One_File = 1
let Tlist_Exit_OnlyWindow = 1
map <F4> :TlistToggle<CR>

４．在要使用的地方输入ctags -R　这样会生成一个文件
５．进入vim weiqifa.c 的时候　按Ｆ４切换普通模式和有tag的模式，按ctrl w w 进入函数列表还是编辑框

资源下载：[http://download.csdn.net/detail/weiqifa0/9137283](http://download.csdn.net/detail/weiqifa0/9137283)


