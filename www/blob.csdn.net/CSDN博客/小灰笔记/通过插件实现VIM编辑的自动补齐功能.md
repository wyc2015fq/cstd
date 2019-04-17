# 通过插件实现VIM编辑的自动补齐功能 - 小灰笔记 - CSDN博客





2017年04月27日 01:32:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：803








自动补齐的功能我最初接触到实在Notepad++之中，刚开始的时候感觉这个功能有点烦人。可是用习惯了之后倒觉得没有这个功能的编辑器用着真不舒服。在公司使用的是Source Insight，V3版本的软件虽然具有自动补齐的功能但是反应的速度十分慢。VIM能够实现补气的功能，但是并不是自动的，而是需要快捷键去激活一下。考虑是否能够找到一个插件实现这个功能，在网络上搜索了一下，还是比较顺利。看来，大家都喜欢这个功能。

找到的插件是vim-autocomplpop，可以在http://www.vim.org/scripts/script.php?script_id=1879下载到相应的插件包。解压之后的文件的目录清单如下：

python file_list.py 

.DS_Store<->/Users/greyzhang/Downloads/vim-autocomplpop

acp.vim<->/Users/greyzhang/Downloads/vim-autocomplpop/autoload

acp.jax<->/Users/greyzhang/Downloads/vim-autocomplpop/doc

acp.txt<->/Users/greyzhang/Downloads/vim-autocomplpop/doc

acp.vim<->/Users/greyzhang/Downloads/vim-autocomplpop/plugin




解压之后的目录清单如下：

$ ls

autoloaddocplugin




插件的安装只需要把三个文件夹中的文件拷贝到VIM程序目录下相应的文件夹中，VIM的目录如下：

![](https://img-blog.csdn.net/20170427012953023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




插件安装成功之后使用时候的效果如下：


![](https://img-blog.csdn.net/20170427013025922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




使用的时候效果还不错，但是有时候会出现一点异常匹配在命令输入接口抛出。



