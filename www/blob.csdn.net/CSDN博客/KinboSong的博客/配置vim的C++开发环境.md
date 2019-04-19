# 配置vim的C++开发环境 - KinboSong的博客 - CSDN博客
2017年04月23日 18:56:03[songjinbo3](https://me.csdn.net/KinboSong)阅读数：343
vim参考：[http://easwy.com/blog/archives/advanced-vim-skills-advanced-move-method/](http://easwy.com/blog/archives/advanced-vim-skills-advanced-move-method/)
参考：[http://www.cnblogs.com/linux-home/p/5386330.html](http://www.cnblogs.com/linux-home/p/5386330.html)
**注意**：经过验证，insert模式和insert（paste）模式是不一样的
当处于insert(paste)模式时，AutoComplPop时不会自动补全的，omnicppcomplete也不能够正常使用。imap映射的快捷键是不能够正常使用的。
当.vimrc文件中中含有语句 **set paste** 时，insert模式会变成insert(paste)模式
1、Ctags插件
（1）sudo apt-get install Ctags ：安装Ctags插件
（2）生成tags文件
i.创建工程的tags并添加到vim中
进入工程目录中，运行ctags -R --c++-kinds=+px --fields=+iaS --extra=+q 
在~/.vimrc文件中添加两行set tags=tags
set tags+=./tags   （只有添加这两条时，才可以正常使用omnicppcomplete）
ii.创建并添加C++系统文件的tags并添加到vim中
ctags -I __THROW --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c++-kinds=+p --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include
 /usr/local/include （试过很多命令，只有执行这个命令时才可以用omnicppcomplete）：生成文件夹/usr/include 和 /usr/local/include的tags文件 并保存在文件~/.vim/systags中
在~/.vimrc文件(没有的话新建一个)中添加一行：set tag+=~/.vim/systages（注意：中间不要留空格）
(3) ctags命令
tag命令用法：
:set tags：列出tags的文件路径
:tags :列出所有已经使用过的tags
Ctrl＋］ 跳到当前光标下单词的标签
Ctrl＋O 返回上一个标签
Ctrl＋T 返回上一个标签
:tag TagName 跳到TagName标签
以上命令是在当前窗口显示标签，当前窗口的文件替代为包标签的文件，当前窗口光标跳到标签位置。如果不希望在当前窗口显示标签，可以使用以下命令：
:stag TagName 新窗口显示TagName标签，光标跳到标签处
Ctrl＋W + ］ 新窗口显示当前光标下单词的标签，光标跳到标签处
当一个标签有多个匹配项时（函数 (或类中的方法) 被多次定义），":tags" 命令会跳转到第一处。如果在当前文件中存在匹配，那它将会被首先使用。
可以用这些命令在各匹配的标签间移动：
:tfirst 到第一个匹配
:[count]tprevious 向前 [count] 个匹配
:[count]tnext 向后 [count] 个匹配
:tlast 到最后一个匹配
或者使用以下命令选择要跳转到哪一个
:tselect TagName
输入以上命令后，vim会为你展示一个选择列表。然后你可以输入要跳转到的匹配代号 (在第一列)。其它列的信息可以让你知道标签在何处被定义过。
以下命令将在预览窗口显示标签
:ptag TagName 预览窗口显示TagName标签，光标跳到标签处
Ctrl＋W + } 预览窗口显示当前光标下单词的标签，光标跳到标签处
:pclose 关闭预览窗口
:pedit file.h 在预览窗口中编辑文件file.h（在编辑头文件时很有用）
:psearch atoi 查找当前文件和任何包含文件中的单词并在预览窗口中显示匹配，在使用没有标签文件的库函数时十分有用。
ctags的原理：[http://www.cnblogs.com/jianyungsun/archive/2011/01/20/1940262.html](http://www.cnblogs.com/jianyungsun/archive/2011/01/20/1940262.html)
ctags教程：[http://blog.csdn.net/g_brightboy/article/details/16830395](http://blog.csdn.net/g_brightboy/article/details/16830395)
2、安装omnicppcomplete插件
（1）、下载安装包（zip格式）
（2）、解压到安装包到 ~/.vim/ 文件夹中
（3）、在~/.vimrc中添加如下配置语句：
imap <F3><C-X><C-O>
  "把F3键映射成<C-X><C-O>
imap <F2> <C-X><C-I>
set completeopt=menu,menuone "
关掉智能补全时的预览窗口
let OmniCpp_MayCompleteDot
=1" autocomplete with .
let OmniCpp_MayCompleteArrow = 1 " autocompletewith->
let OmniCpp_MayCompleteScope=1" autocomplete with ::
let OmniCpp_SelectFirstItem = 2 "
select first item (but don't insert)
let OmniCpp_NamespaceSearch = 2 " search namespaces in this and included files
let OmniCpp_ShowPrototypeInAbbr = 1 " show function prototype in popup window
let OmniCpp_GlobalScopeSearch=1 " enable the global scope search
let OmniCpp_DisplayMode=1 " Class scope completion mode: always show all members
"let OmniCpp_DefaultNamespaces=["std"]
let OmniCpp_ShowScopeInAbbr=1 " show scope in abbreviation and remove the last column
let OmniCpp_ShowAccess=1 
（4）、<ctrl + x> + <ctrl+o>命令匹配 所有设置的tags文件中的tag
说明：需要按快捷键才可以弹出匹配菜单；补全菜单中出现的是tags文件中匹配的tag
3、安装autocomplpop插件
（1） （2） 步骤和安装omnicppcomplete时一样
说明：无须按快捷键就可弹出补全菜单；不需要使用tags文件，补全菜单中显示的是本文件中出现的标签
4、taglist插件
Taglist用于列出了当前文件中的所有标签（宏, 全局变量, 函数名等）
安装方法和上面一样，安装完之后，在vim配置文件/home/user/.vimrc中加入如下的配置：
"-- Taglist setting --
let Tlist_Ctags_Cmd='ctags' "因为我们放在环境变量里，所以可以直接执行
let Tlist_Use_Right_Window=1"让窗口显示在右边，0的话就是显示在左边
let Tlist_Show_One_File=0 "让taglist可以同时展示多个文件的函数列表
let Tlist_File_Fold_Auto_Close=1"非当前文件，函数列表折叠隐藏
let Tlist_Exit_OnlyWindow=1 "当taglist是最后一个分割窗口时，自动推出vim
"是否一直处理tags.1:处理;0:不处理
let Tlist_Process_File_Always=1 "实时更新tags
let Tlist_Inc_Winwidth=0
打开代码后，输入
:Tlist 命令即可打开Taglist界面，再次输入即可关闭
5、安装cscope插件
参考：[http://www.cnblogs.com/jianyungsun/archive/2010/12/07/1899348.html](http://www.cnblogs.com/jianyungsun/archive/2010/12/07/1899348.html) （非常好）
参考：[http://blog.csdn.net/lhf_tiger/article/details/7216500](http://blog.csdn.net/lhf_tiger/article/details/7216500)
Cscope是一个类似于ctags的工具，不过其功能比ctags强大很多。
安装Cscope
lingd@ubuntu:~$ sudo apt-get install cscope
在vim配置文件/home/user/.vimrc中加入如下的配置：
"-- Cscope setting --
if has("cscope")
set csprg=/usr/bin/cscope "
指定用来执行cscope的命令
set csto=0" 设置cstag命令查找次序：0先找cscope数据库再找标签文件；1先找标签文件再找cscope数据库
set cst "同时搜索cscope数据库和标签文件
set cscopequickfix=s-,c-,d-,i-,t-,e-"
 使用QuickFix窗口来显示cscope查找结果
set nocsverb
if filereadable("cscope.out") "若当前目录下存在cscope数据库，添加该数据库到vim
cs add cscope.out
elseif $CSCOPE_DB !=
""" 否则只要环境变量CSCOPE_DB不为空，则添加其指定的数据库到vim
cs add $CSCOPE_DB
endif
set csverb
endif
map <F4> :cs add ./cscope.out .<CR><CR><CR> :cs reset<CR>
imap <F4> <ESC>:cs add ./cscope.out .<CR><CR><CR> :cs reset<CR>
"将:cs find c等Cscope查找命令映射为<C-_>c等快捷键（按法是先按Ctrl+Shift+-,然后很快再按下c）
nmap <C-_>s:cs find s<C-R>=expand("<cword>")<CR><CR>:copen<CR><CR>
nmap <C-_>g:cs find g<C-R>=expand("<cword>")<CR><CR>
nmap <C-_>d:cs find d<C-R>=expand("<cword>")<CR><CR>:copen<CR><CR>
nmap <C-_>c:cs find c<C-R>=expand("<cword>")<CR><CR>:copen<CR><CR>
 nmap <C-_>t :cs find t <C-R>=expand("<cword>")<CR><CR> :copen<CR><CR> nmap <C-_>e :cs find e <C-R>=expand("<cword>")<CR><CR> :copen<CR><CR> nmap <C-_>f :cs find f <C-R>=expand("<cfile>")<CR><CR> nmap <C-_>i :cs find i <C-R>=expand("<cfile>")<CR><CR> :copen<CR><CR>
cscope的主要功能是通过其"find"子命令来实现的
"cscope find"的用法:
cs find c|d|e|f|g|i|s|t name
0 或 s  查找这个 C 符号(可以跳过注释)
1 或 g  查找这个定义
2 或 d  查找这个函数调用的函数
3 或 c  查找调用过这个函数的函数
4 或 t  查找这个字符串
6 或 e  查找这个 egrep 模式
7 或 f  查找这个文件
8 或 i  查找包含这个文件的文件
！！！注意：如果用cscope查找完未弹出quickfix窗口，需要手动输入“:cw”命令打开quickfix窗口。
用法：
1、添加c++系统库文件的cscope数据库
i.进入根目录 cd /
ii.运行命令产生cscope.files文件：sudo find /usr/include -name '*.c' -o -name '*.h' > ~/.cscope/cscope.files 
iii.进入目录~/.cscope/
iv.生成库文件cscope.out ：cscope -b（cscope会自动查找所在目录是否有文件cscope.files，如果有则按照cscope.files的内容来生成）
v.在vim中添加文件~/.cscope/cscope.out (可以在.vimrc中输入命令 cs add /home/pi/.cscope/cscope.out)
<1>、为源码建立一个cscope数据库
lingd@ubuntu:~/arm/linux-2.6.28.7$ cscope -Rbq
lingd@ubuntu:~/arm/linux-2.6.28.7$ ls cscope.*
cscope.in.out  cscope.out  cscope.po.out
<2>、 用vim打开某个源码文件，末行模式下，输入“:cs add cscope.out"（该命令已被我们映射为快捷键F4），添加cscope数据库到vim。因为我们已将vim配置为启动时，自动添加当前目录下的 cscope数据库，所以你再添加该cscope数据库时，vim会提示“重复cscope数据库 未被加入“
<3>、完成前两步 后，现在就可以用“cs find c”等Cscope查找命令查找关键字了。我们已在.vimrc中将“cs find c”等Cscope查找命令映射为<C-_>c等快捷键（按法是先按Ctrl+Shift+-, 然后很快按下c）
6、NERDTree插件
安装方法同omnicppcomplete一样，配置及使用方法参考：[http://www.cnblogs.com/wangkongming/p/4050833.html](http://www.cnblogs.com/wangkongming/p/4050833.html)
o 打开关闭文件或者目录
t 在标签页中打开
T 在后台标签页中打开
! 执行此文件
p 到上层目录
P 到根目录
K 到第一个节点
J 到最后一个节点
u 打开上层目录
m 显示文件系统菜单（添加、删除、移动操作）
? 帮助
q 关闭
7、quickfix插件
功能：
这个插件已经集中在vim中了，不需要安装，用法参考：[http://blog.csdn.net/joeblackzqq/article/details/7447606](http://blog.csdn.net/joeblackzqq/article/details/7447606)
8、折叠功能
参考：[http://blog.sina.com.cn/s/blog_7acf472901017ad6.html](http://blog.sina.com.cn/s/blog_7acf472901017ad6.html)
9、Project插件
安装方法和omnicppcomplete一样
参考： [http://blog.csdn.net/clevercode/article/details/51363050](http://blog.csdn.net/clevercode/article/details/51363050)
[http://www.cnblogs.com/eddy-he/archive/2012/09/08/2676326.html](http://www.cnblogs.com/eddy-he/archive/2012/09/08/2676326.html)
10、GDB调试
命令汇总如下：
(1)gdb (2)file (3)b(break) (4)d(delete) (5)n s c r (6)p(print) i(info) (7)finish q (8)list layout
参考：[http://blog.csdn.net/liigo/article/details/582231/](http://blog.csdn.net/liigo/article/details/582231/)
