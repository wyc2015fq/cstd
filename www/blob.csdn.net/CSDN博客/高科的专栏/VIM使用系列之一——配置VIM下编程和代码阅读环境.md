
# VIM使用系列之一——配置VIM下编程和代码阅读环境 - 高科的专栏 - CSDN博客

2011年09月07日 14:48:41[高科](https://me.csdn.net/pbymw8iwm)阅读数：825标签：[vim																](https://so.csdn.net/so/search/s.do?q=vim&t=blog)[编程																](https://so.csdn.net/so/search/s.do?q=编程&t=blog)[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[tags																](https://so.csdn.net/so/search/s.do?q=tags&t=blog)[header																](https://so.csdn.net/so/search/s.do?q=header&t=blog)[reference																](https://so.csdn.net/so/search/s.do?q=reference&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=header&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tags&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)
[
																								](https://so.csdn.net/so/search/s.do?q=tags&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=vim&t=blog)
作者：gnuhpc from[http://blog.csdn.net/gnuhpc](http://blog.csdn.net/gnuhpc)[http://gnuhpc.wordpress.com/](http://gnuhpc.wordpress.com/)
本文环境：ubuntu 10.10/vim7.2
前言：一年前写过一篇关于[VIM的C/C++编程环境](http://blog.csdn.net/gnuhpc/article/details/5268965)的文字，一年中又接触了很多东西，深入使用中发现其实还是需要有很多改正和增强的，当然，VIM社区中也出现了更多好用插件需要整合进来。为了更好的总结整个环境配置方法以及使用方法，特写此文从零开始配置vim环境，主要针对C/C++/Python/Bash编程。
**基****本配置**
**1) 建立基本环境：**
建立三个目录：mkdir ~/.vim/{plugin,doc,syntax} -p
建立配置文件：touch ~/.vimrc
.vimrc是vim的主要配置文件，我们在配置VIM时首先对它进行基本的设置。打开home目录下刚刚建立的.vimrc文件，可以对VIM进行如下的一系列基本选项设置：
> set wildmenu

> “增强模式中的命令行自动完成操作

> set foldmethod=manual

> “设定折叠方式为手动

> set cin

> “实现C程序的缩进

> set sta

> “插入<tab>时使用’shiftwidth’

> set backspace=2

> “指明在插入模式下可以使用<BS>删除光标前面的字符

> syntax enable

> “设置高亮关键字显示

> set nocompatible

> “去掉讨厌的有关vi一致性模式，避免以前版本的一些bug和局限

> set number

> “显示行号

> filetype on

> “检测文件的类型

> set history=1000

> “”记录历史的行数

> set background=dark

> “背景使用黑色

> set autoindent

> set smartindent

> “上面两行在进行编写代码时，在格式对起上很有用；

> “第一行，vim使用自动对起，也就是把当前行的对起格式应用到下一行；

> “第二行，依据上面的对起格式，智能的选择对起方式，对于类似C语言编写上很有用

> “第一行设置tab键为4个空格，第二行设置当行之间交错时使用4个空格

> set tabstop=4

> set shiftwidth=4

> set showmatch

> “设置匹配模式，类似当输入一个左括号时会匹配相应的那个右括号

> set ruler

> “在编辑过程中，在右下角显示光标位置的状态行

> set incsearch

> “查询时非常方便，如要查找book单词，当输入到/b时，会自动找到第一

> “个b开头的单词，当输入到/bo时，会自动找到第一个bo开头的单词，依

> “次类推，进行查找时，使用此设置会快速找到答案，当你找要匹配的单词

> “时，别忘记回车。

> set enc=utf-8

> “设置编码为中文

> set winaltkeys=no

> “Alt组合键不映射到菜单上

> set laststatus=2

> “总是出现状态栏

> set statusline=%<%f\ %h%m%r%=%-14.(%l,%c%V%)\ %P>

> “设置状态栏

> set hidden

> “设置隐藏已修改buffer

> autocmd FileType python setlocal et sta sw=4 sts=4
**2）安装中文帮助手册**
这个看个人需要，下载地址：[http://vimcdoc.sourceforge.net/](http://vimcdoc.sourceforge.net/)
安装：
> $tar xvf vimcdoc-1.7.0.tar.gz

> $ cd vimcdoc-1.7.0/

> $ ./vimcdoc.sh -i
在一堆done后完成。在~/.vimrc里加上set helplang=cn 这句话标明帮助文档为中文。在vim中”:help”后就能看到效果。
**插****件配置**
首先要在.vimrc中添加以下语句来使能插件：
> filetype plugin on

> filetype plugin indent on
**1.代码快速编辑和编译运行：C-support，Bash-support**
C-support下载安装：
下载：[http://www.vim.org/scripts/script.php?script_id=213](http://www.vim.org/scripts/script.php?script_id=213)
安装：将下载的文件解压放到~/.vim目录中
Bash-support下载安装：
下载：[http://www.vim.org/scripts/download_script.php?src_id=16019](http://www.vim.org/scripts/download_script.php?src_id=16019)
安装：将下载文件解压放到~/.vim目录中
使用方法：
a)自动添加文件头：
添加你自己的信息： vim ~/.vim/c-support/templates/Templates
~/.vim/c-support/templates 这个目录下还有许多模板，你可以自己定制。现在试着vim test.c看一看效果。
b)快捷输入
主要是一些模板的插入，种类几乎覆盖C/C++/Bash开发中大多数需要手动输入的东西。不过我不是很喜欢它的默认模板的代码风格，不过可以通过上述的目录中的文件去定制。这个功能在后边的code complete插件中也提供了。当然，也包含一些比如注释等的操作。
另外一部分是编译运行的快捷键，还有一个把另一个文件内容直接灌入当前文件的功能。
有关使用热键的方法见一个文档：[http://lug.fh-swf.de/vim/vim-c/c-hotkeys.pdf](http://lug.fh-swf.de/vim/vim-c/c-hotkeys.pdf)（内容如下）
C
![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb1.png?w=910&h=551)
![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb2.png?w=908&h=595)
Shell
![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb3.png?w=385&h=837)![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb4.png?w=326&h=845)
**2.阅读源代码：Ctags+Tag List +Win Manger+Cscope**
这三个分别是生成标签、列出标签（使用ctags）、列出文件、代码跳转。
下载安装：
下载安装Exuberant Ctags ：sudo apt-get install exuberant-ctags
（我们安装ctags但是并不直接使用它作为代码跳转工具，而是使用cscope）
Teg List下载：[http://www.vim.org/scripts/download_script.php?src_id=7701](http://www.vim.org/scripts/download_script.php?src_id=7701)
安装：解压到home目录下的.vim子目录中。
WinManager（调整窗口布局）下载：[http://www.vim.org/scripts/download_script.php?src_id=754](http://www.vim.org/scripts/download_script.php?src_id=754)
安装：解压到home目录下的.vim子目录中。
下载安装cscope：sudo apt-get install cscope
配置：
进入$HOME/.vim/doc打开 Vim 运行 “:helptags .” 这是要保证taglist帮助可以正常使用。
**然后为后边配置自动补全建立系统tags**：（具体为什么采用这样的参数请参考[ctags命令行帮助](http://ctags.sourceforge.net/ctags.html)）
> ctags -R -f ~/.vim/systags –c-kinds=+p –c++-kinds=+px –fields=+iaS –extra=+q –python-kinds=-i /usr/include /usr/local/include
随后在vimrc中添加如下几行：
> “禁止自动改变当前Vim窗口的大小

> let Tlist_Inc_Winwidth=0

> “把方法列表放在屏幕的右侧

> let Tlist_Use_Right_Window=1

> “让当前不被编辑的文件的方法列表自动折叠起来， 这样可以节约一些屏幕空间

> let Tlist_File_Fold_Auto_Close=1

> “映射F3为功能键调出winmanager的文件浏览器

> “映射双击F3调出Taglist的符号浏览器

> “let g:winManagerWindowLayout=’FileExplorer|TagList’ “你要是喜欢这种布局可以注释掉这一行

> map <F3> :WMToggle<cr>

> map <F3><F3> :TlistToggle<cr>

> “将系统已经生成的tags导入

> set tags+=~/.vim/systags

> “映射F10为添加cscope和ctags

> map <F10> :call Do_CsTag()<CR>

> map <F10><F10> :call Add_CsTag()<CR>

> nmap <C-\>s :cs find s <C-R>=expand(“<cword>”)<CR><CR>

> nmap <C-\>g :cs find g <C-R>=expand(“<cword>”)<CR><CR>

> nmap <C-\>c :cs find c <C-R>=expand(“<cword>”)<CR><CR>

> nmap <C-\>t :cs find t <C-R>=expand(“<cword>”)<CR><CR>

> nmap <C-\>e :cs find e <C-R>=expand(“<cword>”)<CR><CR>

> nmap <C-\>f :cs find f <C-R>=expand(“<cfile>”)<CR><CR>

> nmap <C-\>i :cs find i ^<C-R>=expand(“<cfile>”)<CR>$<CR>

> nmap <C-\>d :cs find d <C-R>=expand(“<cword>”)<CR><CR>

> function Do_CsTag()

> silent! execute “!ctags -R ‘.’”

> if(executable(‘cscope’) && has(“cscope”) )

> silent! execute “!find -L `pwd` -name ‘*.h’ -o -name ‘*.c’ -o -name ‘*.cpp’ -o -name ‘*.java’ -o -name ‘*.cs’ -o     -name ‘*.cxx’ -o -name ‘*.hxx’> cscope.files -o -name ‘*.hpp’ -o -name ‘*.py’”    endif

> silent! execute “!cscope -bq”

> silent! execute “call Add_CsTag()”

> endf

> function Add_CsTag()

> if filereadable(“cscope.out”)

> execute “cs add cscope.out”

> execute “set tags+=./tags”

> endif

> endf

> silent! execute “call Add_CsTag()”
使用方法：
双击F3调出函数、变量等tags。
单击F3出现文件管理器（重复这个过程关闭）。以上产生的子区域都是通过Crtl+ww来相互跳转
单击F10生成这个目录及其子目录的tags以及cscope文件，并添加。
双击F10仅加载tags和cscope文件
将光标指向一些函数名或者变量名，使用Crtl+\+以下字母来跳转，并使用Crtl+O跳转回来
> s: 查找C语言符号，即查找函数名、宏、枚举值等出现的地方

> g: 查找函数、宏、枚举等定义的位置，类似ctags所提供的功能

> d: 查找本函数调用的函数

> c: 查找调用本函数的函数

> t: 查找指定的字符串

> e: 查找egrep模式，相当于egrep功能，但查找速度快多了

> f: 查找并打开文件，类似vim的find功能

> i: 查找包含本文件的文件
gd 转到当前光标所指的局部变量的定义
* 转到当前光标所指的单词下一次出现的地方
\# 转到当前光标所指的单词上一次出现的地方
ctrl+] 转到函数定义的地方
ctrl+o 回退到函数调用的地方
TagList操作：<CR>          跳到光标下tag所定义的位置，用鼠标双击此tag功能也一样
o             在一个新打开的窗口中显示光标下tag
<Space>       显示光标下tag的原型定义
u             更新taglist窗口中的tag
s             更改排序方式，在按名字排序和按出现顺序排序间切换
x             taglist窗口放大和缩小，方便查看较长的tag
+             打开一个折叠，同zo
-             将tag折叠起来，同zc
*             打开所有的折叠，同zR
=             将所有tag折叠起来，同zM
[[            跳到前一个文件
]]            跳到后一个文件
q             关闭taglist窗口
<F1>          显示帮助
WinManager操作：
![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb5.png?w=522&h=205)

**3.同名头文件和源文件切换：A**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=7218](http://www.vim.org/scripts/download_script.php?src_id=7218)，将a.vim
 放到 ~/.vim/plugin 中。
在.vimrc中添加：map <F11> :A<cr>
使用: 在源文件或者头文件中用F11就可以切换了。
**4.代码块和常用语句模板自动补全：code_complete**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=10778](http://www.vim.org/scripts/download_script.php?src_id=10778)，放到
 ~/.vim/plugin 中。
把插件文件中的tab都改为C-b防止与后来我们要安装的supertab冲突。
另外，这个是基于**ctags**的，所以要生成或加载tags，参见2）部分。
将下边这一段加入到template段中以添加bash模板补全功能
> “Bash Templates

> “Bash Structure by warmbupt@gmail.com

> let g:template['sh'] = {}

> let g:template['sh']['sif'] = “if “.g:rs.”…”.g:re.” ; then/”.g:rs.”…”.g:re.”/fi”

> let g:template['sh']['sife'] = “if “.g:rs.”…”.g:re.” ; then/”.g:rs.”…”.g:re.”/else/”.g:rs.”…”.

> /g:re.”/fi”

> let g:template['sh']['scase'] = “case “.g:rs.”…”.g:re.” in/”.g:rs.”…”.g:re.”)//” .g:rs.”…”.g:re.”;;//”.

> /g:rs.”…”.g:re.”)//” .g:rs.”…”.g:re.”;;///esac/”

> let g:template['sh']['swhile'] = “while “.g:rs.”…”.g:re.” /do/”.g:rs.”…”.g:re.”/done”

> let g:template['sh']['suntil'] = “until “.g:rs.”…”.g:re.” /do/”.g:rs.”…”.g:re.”/done”

> let g:template['sh']['sfor'] = “for “.g:rs.”…”.g:re.” in ” .g:rs.”…”.g:re.”/do/”.g:rs.”…”.g:re.”/done”

> let g:template['sh']['sselect'] = “select “.g:rs.”…”.g:re.” in ” .g:rs.”…”.g:re.”/do/”.g:rs.”…”.g:re.”/done”
使用：在输入函数时可以快速输入是安装这个插件的主要目的。比如，输入main，然后Crtl+b就可以看到效果了。对于函数，在输入strlen(后按C-b，会补全参数，然后再按C-b就可以一个个填写参数了。
具体哪些可以补全如下：
C/C++语言:cc /*  */
de \#define
in \#include    “”
is \#include    <>
ff \#ifndef
ife  if…els
for for(;;)
switch switch() case: …
main int main()
BASH:sif               if
sife             if…els
scase          case
swhile         while
suntil          until
sfor             for
sselect        select
**5.代码补全：supertab+snipMate+omnicppcomplete**
下载地址：
[http://www.vim.org/scripts/download_script.php?src_id=16104](http://www.vim.org/scripts/download_script.php?src_id=16104)
[http://www.vim.org/scripts/download_script.php?src_id=16345](http://www.vim.org/scripts/download_script.php?src_id=16345)
安装：用vim打开supertab.vba，然后使用:so % 进行安装。后两者直接解压在.vim目录里即可。
设置：进入vim
> :helptags ~/.vim/doc
并且在~/.vim/snippets/python.snippets中添加如下信息：
> snippet header

> \#!/usr/bin/env python

> “”"

> File: `Filename(‘$1.py’, ‘foo.py’)`

> Author: gnuhpc

> Company: CMBC

> Date: `strftime(“%Y-%m-%d %T %Z”)`

> Description: ${1}

> “”"
使用：使用tab就可以完成一些文件中已经出现的词汇的补全和常见结构的补全了，这个功能与code_complete的结构快速补充有所重叠，不过是使用tab键，你还可以在.vim/snippets目录里自己定制。
添加的header是为了python自动生成header，在.py文件中，首先输入header后按tab就会出现这个header，具体的信息可以根据个人情况修改。
**6.原型提示：echofunc**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=15953](http://www.vim.org/scripts/download_script.php?src_id=15953)，然后放到plugin目录里。
然后在.vimrc中设置：
> let g:EchoFuncKeyNext=’<C-n>’

> let g:EchoFuncKeyPrev=’<C-p>’
使用：可以试着输入select(就可以看到了下边的原型提示了）。用Ctrl+n和Ctrl+p来上下翻动。
小技巧：可以将这个插件与code_complete结合，在看到提示后按Ctrl+b补全函数，然后按Ctrl+b，从左往右顺序一个个填写参数。
**7.C/STL/Python Doc提示：CRefVim 、stlrefvim 、Pyref**
下载安装：
[http://www.vim.org/scripts/download_script.php?src_id=3666](http://www.vim.org/scripts/download_script.php?src_id=3666)
[http://www.vim.org/scripts/download_script.php?src_id=9160](http://www.vim.org/scripts/download_script.php?src_id=9160)
[http://www.vim.org/scripts/download_script.php?src_id=14576](http://www.vim.org/scripts/download_script.php?src_id=14576)
解压后将crefvim目录里的所有目录都复制到.vim/下，其余的在.vim目录解压即可。
然后进入vim后” :helptags ~/.vim/doc”
设置：
> 打开plugin/stlrefvim.vim中将’tr’改为’cr’，这个是为了好记忆。

> 把plugin/pyref.vim移动到ftplugin中，这样只针对python文件的插件。

> 在.vimrc中添加：let g:pyref_mapping = ‘K’
使用：
\cr 查看C语言常见函数和操作符的使用指南（C Reference）
\sr查看STL常见方法的使用指南 （STL Reference）
再加上Shift+k查询man以及Python手册，就全了。
**8.简单注释：comments**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=9801](http://www.vim.org/scripts/download_script.php?src_id=9801)，放到plugin目录中。这个插件支持很多语言。
使用：在visual <Shift-V> 模式下，ctrl-c注释，ctrl-x去注释。
**9.代码格式整理：astyle**
下载安装：$sudo apt-get install astyle
配置：
在vimrc中写入：
> “定义源代码格式化

> map <F12> :call FormartSrc()<CR>

> “定义FormartSrc()

> func FormartSrc()

> exec “w”

> if &filetype == ‘c’

> exec “!astyle % –style=ansi –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘cpp’

> “exec “!astyle % –style=ansi –suffix=none %”

> “exec “e! %”

> elseif &filetype == ‘perl’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘py’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘java’

> exec “!astyle –style=java –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘jsp’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘xml’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘html’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> elseif &filetype == ‘htm’

> exec “!astyle –style=gnu –suffix=none %”

> exec “e! %”

> endif

> endfunc

> “结束定义FormartSrc
使用：<F12> 进行代码格式化。
**10.多tab支持：NERD_tree**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=11834](http://www.vim.org/scripts/download_script.php?src_id=11834)，解压到.vim目录即可
设置：
> “设置tab的快捷键

> “Shift+t为新建一个标签，按两次Shirt+t为跳转标签

> map <S-t> :tabnew .<CR>

> map <S-t><S-t> <ESC>:tabnext<CR>
使用：Shift＋t就可以新建一个标签，连按Shfit＋t两次就可以切换标签。
**11. Python自动补全支持：Pydiction**
下载安装：[http://www.vim.org/scripts/download_script.php?src_id=11062](http://www.vim.org/scripts/download_script.php?src_id=11062)解压放到~/.vim/after/ftplugin/
 路径下
将complete-dict文件放到~/.vim/tools/pydiction/目录中。
配置：
> “pydiction 1.2 python auto complete

> let g:pydiction_location = ‘~/.vim/tools/pydiction/complete-dict’

> let g:pydiction_menu_height = 20
使用：采用tab键进行自动补全，比如import os.此时可以按tab进行补全。
**12.自动检查Python语法：pyflakes-vim**
下载安装：[pyflakes-vim.zip](http://www.vim.org/scripts/download_script.php?src_id=14403)，在~/.vim/目录下解压。
使用：当你的python脚本语法有问题时，自动会在出错处标红，并在屏幕下方显示错误原因提示。
**13. Python常用功能：python_fn.vim**
下载安装：[python_fn.vim](http://www.vim.org/scripts/download_script.php?src_id=9196)， 放到~/.vim/ftplugin/python中即可。
使用：
![image](http://gnuhpc.files.wordpress.com/2011/09/image_thumb6.png?w=308&h=508)
**14. Python代码自动折叠：Efficient python folding**
下载安装：[python_editing.vim](http://www.vim.org/scripts/download_script.php?src_id=5492)，放在~/.vim/ftplugin/python即可。
使用：f为折叠起来，F（Shift-f）是打开已经折叠的代码。
**15.代码文档化工具：Doxygen**
下载安装：[DoxygenToolkit.vim](http://www.vim.org/scripts/download_script.php?src_id=14064)，放到~/.vim/plugin
安装doxygen：sudo apt-get install*doxygen*
配置：.vimrc中添加，其中个人信息可以自己修改
> map <F4>a
![:D](http://s0.wp.com/wp-includes/images/smilies/icon_biggrin.gif?m=1304062081g)
> oxAuthor<CR>

> map <F4>f
![:D](http://s0.wp.com/wp-includes/images/smilies/icon_biggrin.gif?m=1304062081g)
> ox<CR>

> map <F4>b
![:D](http://s0.wp.com/wp-includes/images/smilies/icon_biggrin.gif?m=1304062081g)
> oxBlock<CR>

> map <F4>l
![:D](http://s0.wp.com/wp-includes/images/smilies/icon_biggrin.gif?m=1304062081g)
> oxLic<CR>

> map <F4>c odocClass<C-B>

> map <F4>m odocMember<C-B>

> let g:DoxygenToolkit_authorName=”pchuang,
> [http://blog.csdn.net”](/)

> let s:licenseTag = “Copyright(C)\<enter>”

> let s:licenseTag = s:licenseTag . “For free\<enter>”

> let s:licenseTag = s:licenseTag . “All right reserved\<enter>”

> let g:DoxygenToolkit_licenseTag = s:licenseTag

> let g:DoxygenToolkit_briefTag_funcName=”yes”

> let g:doxygen_enhanced_color=1
使用：（需要安装doxygen）
F4-a 文件头注释
F4-f  函数头注释
F4-b 程序块注释
F4-l license块注释
F4-c 类注释
F4-m 类成员注释
**16.配置一键编译调试**
在.vimrc中设置：

> map <F8> :w<CR>:call CompileRun()<CR>

> map <F8><F8> :w<CR>:call Debug()<CR>

> func CompileRun()

> exec “w”

> if &filetype == ‘c’

> exec “!gcc % -g -o %<”

> exec “!.\/%<”

> elseif &filetype ==’cpp’

> exec “!g++ % -g -o %<”

> exec “!.\/%<”

> elseif &filetype == ‘python’

> exec “!python %”

> endif

> endfunc

> func Debug()

> exec “w”

> if &filetype == ‘c’

> exec “!rm %<”

> exec “!gcc % -g -o %<”

> exec “!gdb %<”

> elseif &filetype == ‘cpp’

> exec “!rm %<”

> exec “!g++ % -g -o %<”

> exec “!gdb %<”

> exec “!rm %<.class”

> elseif &filetype == ‘java’

> exec “!javac %”

> exec “!jdb %<”

> elseif &filetype == ‘python’

> exec “!pdb %”

> endif

> endfunc
使用：F8编译运行程序，双击F8为调试程序
**17.shell脚本自动加上执行权限**
在.vimrc中配置：
> function ModeChange()

> if getline(1) =~ “^\#!”

> if getline(1) =~ “/bin/”

> silent !chmod a+x <afile>

> endif

> endif

> endfunction

> au BufWritePost * call ModeChange()
使用：在.sh文件编辑后自动被赋予执行权限。
**18.从上次退出时编辑的位置继续编辑**
在.vimrc中配置：

> autocmd BufReadPost *

> \ if line(“‘\”") > 0 && line (“‘\”") <= line(“$”) |

> \   exe “normal g’\”" |

> \ endif
使用：在退出一个文件编辑后再次进入时会自动跳转到上次编辑的地方。
**19.配置括号自动添加**
在.vimrc中配置：

> :inoremap ( ()<ESC>i

> :inoremap ) <c-r>=ClosePair(‘)’)<CR>

> :inoremap { {}<ESC>i

> :inoremap } <c-r>=ClosePair(‘}’)<CR>

> :inoremap [ []<ESC>i

> :inoremap ] <c-r>=ClosePair(‘]’)<CR>

> :inoremap < <><ESC>i

> :inoremap > <c-r>=ClosePair(‘>’)<CR>

> function ClosePair(char)

> if getline(‘.’)[col('.') - 1] == a:char

> return “\<Right>”

> else

> return a:char

> endif

> endf
使用：大中小圆括号的自动匹配添加。
**20.添加一些快捷键：**
在.vimrc中添加：
> map <F5> :w<CR>:make<CR>

> map <F5><F5> :make clean<CR>

> map <F6> :cw<CR>

> map <F6><F6> :ccl<CR>

> map <F7> :cn<CR>

> map <F7><F7> :cp<CR>

> map <S-o> o<ESC><CR>

> map <C-a> ggVG

> map <S-q><S-q> :q!<CR>:q!<CR>:q!<CR>

> map <C-H> :L1,L2s/src/tar/g

> map <S-F12> <ESC>I//<ESC><CR>

> map! <S-F12> <ESC>I//<ESC><CR>

> map <S-F12><S-F12> 02x<ESC><CR>

> map! <S-F12><S-F12> <ESC>02x<ESC><CR>

> map <F12> <ESC>$a//

> map! <F12> <ESC>$a//
使用：
F5                   make
2*F5               make clean
F6                   quickfix查看详细信息
2*F6               关闭详细信息
F7                   下一个结果
2*F7               上一个结果
Shift+o           建立新行但是不插入
Ctrl+a             全选
2*(Shift+q)     快速不保存
Ctrl+h             替换  （比如1,2s/set/get/g就是替换1到2行的set为get）
**参****考文献**
[http://www.vimer.cn/](http://www.vimer.cn/)
[http://qixinglu.com/archives/vim_key_mapping](http://qixinglu.com/archives/vim_key_mapping)
[http://easwy.com/blog/archives/advanced-vim-skills-cscope/](http://easwy.com/blog/archives/advanced-vim-skills-cscope/)
[http://blog.csdn.net/wooin/archive/2007/10/31/1858917.aspx](http://blog.csdn.net/wooin/archive/2007/10/31/1858917.aspx)
[http://sinolog.it/?p=1443](http://sinolog.it/?p=1443)
**下****载**
除apt-get安装外的所有已经配置好的vim设置打包如下，直接解压到home目录就可以直接使用了:
下载地址：[http://www.kuaipan.cn/file/id_13675463633282823.html](http://www.kuaipan.cn/file/id_13675463633282823.html)
（注意,有systags比较大，大概1.5G，主要是我的include目录里有太多的东西，下载后可以删除重新参照**自动补全建立系统tags来自己生成**）

**另外，对于常用的VIM操作，个人的总结如下：http://blog.csdn.net/gnuhpc/article/details/5276368**


