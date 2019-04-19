# Vim的Python编辑器详细配置过程 (Based on Ubuntu 12.04 LTS) - 宇宙浪子的专栏 - CSDN博客
2014年09月23日 09:34:26[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：323标签：[Python																[插件](https://so.csdn.net/so/search/s.do?q=插件&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[工具																[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)](https://blog.csdn.net/bluehawksky/article/category/2582971)
为什么要用vim编辑py文件？
因为在Linux命令行中，缺少图形界面的IDE，vim是最佳的文本编辑器，而为了更好的编辑py文本，所以配置vim。
**1. 安装完整版vim**
vi和vim的区别？
在Linux自带有vi编辑器，而vim是指vi improved，即vi升级版。区别在于vi指令和功能较vim简单，正因如此才有了vi improved。
Ubuntu自带的vi编辑器是vim.tiny，也就是vim最基本功能，几乎等同于vi。而在Ubuntu中vi命令指向的是vim。
如何使用apt安装vim？
Linux分支之Debian及其衍生版本中可以通过apt管理包，apt是advanced packaging tools的缩写。Ubuntu作为Debian分支，使用apt命令管理包是很方便的。
使用apt-get命令安装包时会发现有几个包可供选择：vim，vim-gnome，vim-gtk，vim-tiny。据说vim包其实就是vim.basic，该版本不支持GUI。建议安装的是vim-gnome。
apt搜索包：
```
$ apt-cache search package_name
```
apt安装包：
```
$ apt-get install vim-gnome
```
安装完成后，vi命令会指向vim.gnome。
**2. 安装ctags**
ctags用于支持taglist，使用ctags可以在变量之间跳跃。
```
$ apt-get install ctags
```
**3. 安装taglist**
首先安装vim-scripts，vim-scripts中带有vim-addon-manager，vim-addon-manager是Ubuntu众多插件管理器之一，用来管理vim插件。通过vim-addon-manager安装taglist。
```
$ apt-get install vim-scripts
$ vim-addons install taglist
```
**4. 安装pydiction**
pydiction用来实现代码补全和语法提示功能。pydiction不能通过apt安装，需要自行下载安装。
```
下载方法一：
　　　　在vim官网[下载zip包](http://www.vim.org/scripts/script.php?script_id=850)，然后自行解压。下载地址为：http://www.vim.org/scripts/script.php?script_id=850
　　下载方法二：
　　　　在GitHub下载源码，可以使用Git迁出到本地，或者下载zip包自行解压，地址为：https://github.com/rkulla/pydiction
```
pydiction可以让vim自动补全Python代码，无论是关键字，标准库还是第三方库。它主要包含3个文件：
```
python_pydiction.vim: vim插件文件。
    complete-dict: 一个字典文件，包含了Python的关键字和模块。插件引用的内容即来自于此。
    pydiction.py: 一个py脚本，运行此文件可以增加新的模块到complete-dict字典中。
```
如何安装？
Linux/Unix系统：将python_pydiction.vim文件复制到 ~/.vim/after/ftplugin 目录下。如果该目录不存在，则创建它，vim会自动在此目录下搜索。
Windows系统：将python_pydiction.vim文件复制到 C:\vim\vimfiles\ftplugin 目录下（假设你的Vim安装路径为C:\vim）
除此之外的另两个文件complete-dict和pydiction.py可以放置到任何你想放置的位置，但在ftplugin目录下最好只存放python_pydiction.vim而不应该还有其他文件。
以下为pydiction.zip解压后执行的安装命令（我的complete-dict和pydiction.py放置在~/.vim目录下）:
```
进入解压后的pydiction目录
$ cp after/ftplugin/python_pydiction.vim ~/.vim/after/ftplugin
$ cp complete-dict ~/.vim
$ cp pydiction.py ~/.vim
```
**5. 编辑配置文件**
编辑~/.vimrc文件，如果不存在则创建。添加以下内容：
```
let Tlist_Auto_Highlight_Tag=1  
let Tlist_Auto_Open=1  
let Tlist_Auto_Update=1  
let Tlist_Display_Tag_Scope=1  
let Tlist_Exit_OnlyWindow=1  
let Tlist_Enable_Dold_Column=1  
let Tlist_File_Fold_Auto_Close=1  
let Tlist_Show_One_File=1  
let Tlist_Use_Right_Window=1  
let Tlist_Use_SingleClick=1  
nnoremap <silent> <F8> :TlistToggle<CR>
   
filetype plugin on  
autocmd FileType python set omnifunc=pythoncomplete#Complete  
autocmd FileType javascrīpt set omnifunc=javascriptcomplete#CompleteJS  
autocmd FileType html set omnifunc=htmlcomplete#CompleteTags  
autocmd FileType css set omnifunc=csscomplete#CompleteCSS  
autocmd FileType xml set omnifunc=xmlcomplete#CompleteTags  
autocmd FileType php set omnifunc=phpcomplete#CompletePHP  
autocmd FileType c set omnifunc=ccomplete#Complete  
   
   
let g:pydiction_location='~/.vim/tools/pydiction/complete-dict'  
set autoindent
set tabstop=4  
set shiftwidth=4  
set expandtab  
set number
```
filetype plugin on：表示开启插件。
let g:pydiction_location='~/.vim/tools/pydiction/complete-dict' : 写入你存放complete-dict的绝对路径。
如果是Pydiction 1.0以前的版本，要确保vimrc不包含以下代码：
```
if has("autocm")
    autocmd FileType python set complete+=k/path/to/pydiction iskeyword+=.,(
endif " has("autocmd")
```
**6. 进击的.vimrc**
以上针对Python配置的vim算是基本完成，但事情可以做得更完善一点，进一步将你的vim变得更好用。（以下进击不仅针对Python，而是对vim自身的完善。）
**6.1. 安装pathogen.vim**
简介：pathogen.vim是一个方便操作"runtimepath", "path", "tags"等的插件，安装了pathogen.vim后，可以非常方便地下载安装其他vim插件。
下载：[http://www.vim.org/scripts/script.php?script_id=2332](http://www.vim.org/scripts/script.php?script_id=2332) 或 [https://github.com/tpope/vim-pathogen](https://github.com/tpope/vim-pathogen)
安装：
首先，在vim runtime目录下创建两新目录 ~/.vim/autoload 和 ~/.vim/bundle。
```
$ mkdir -p ~/.vim/autoload ~/.vim/bundle
```
其次，拷贝源包中 autoload/pathogen.vim 到 ~/.vim/autoload 目录下。
```
$ cp autoload/pathogen.vim ~/.vim/autoload/pathogen.vim
```
然后，在.vimrc文件中写入以下代码：
```
execute pathogen#infect()
```
至此，pathogen.vim便安装完成了。此后所有vim插件目录可以解压到 ~/.vim/bundle 中， 它会被自动追加到"runtimepath"中。
**6.2. 代码高亮**
简介：对代码进行高亮显示。
下载：[http://www.vim.org/scripts/script.php?script_id=1599](http://www.vim.org/scripts/script.php?script_id=1599)
安装：
将下载的hightlight.vim拷贝到 ~/.vim/plugin 目录下。
```
$ cp hightlight.vim ~/.vim/plugin
```
高亮搜索结果命令 :set hlsearch，使用命令 :hi Search查看高亮背景色，默认棕黄色，更改高亮背景色命令 :hi Search guibg=LightBlue。
临时关闭高亮命令 :nohlsearch，该命令可简写为 :noh。
可以配置.vimrc，使用空格键临时关闭搜索结果高亮，在.vimrc写入：
```
:nnoremap <silent> <Space> :nohlsearch<Bar>:echo<CR>
```
要关闭搜索结果高亮，使用命令 :set nohlsearch。同样可以配置.vimrc来使用快捷键（F4）快速关闭和开启搜索结果高亮，在.vimrc写入：
```
:noremap <F4> :set hlsearch! hlsearch?<CR>
```
要默认关闭搜索结果高亮，在.vimrc写入：
```
set viminfo^=h
```
（更多参考 [http://vim.wikia.com/wiki/Highlight_all_search_pattern_matches](http://vim.wikia.com/wiki/Highlight_all_search_pattern_matches)）
 (如何高亮单行 [http://vim.wikia.com/wiki/Highlight_current_line](http://vim.wikia.com/wiki/Highlight_current_line)）
**6.3. taglist**
简介：显示标签列表。
下载：参看前述。
安装：参看前述。
默认关闭taglist，在.vimrc写入：
```
let Tlist_Auto_Open=0
```
在正常编辑区域和tags区域切换命令 :ctrl+w+w。
TlistToggle：开关taglist。
<CR>：跳转至tag定义处。
o：在新窗口中显示光标下的tag。
u：更新taglist窗口中的tag。
s：更改排序方式，名字排序或行号排序。
X：taglist窗口放大缩小。
+：打开折叠，等同zo。
-：关闭折叠，等同zc。
*：打开所有折叠，等同zR。
=：将所有tag折叠，等同zM。
[[：跳转至前一个文件。
]]：跳转至后一个文件。
q：关闭taglist窗口。
<F1>：显示帮助。
**6.4. 折叠代码**
简介：将Python代码折叠，Python的class，function，以及在{{{,}}}标记的内容将被折叠。
下载：[http://vim.sourceforge.net/scripts/script.php?script_id=515](http://vim.sourceforge.net/scripts/script.php?script_id=515)
安装：
将下载的python_fold.vim拷贝到 ~/.vim/plugin 目录下。
关闭开启时默认折叠命令，在.vimrc写入：
```
set nofoldenable
```
zo: 展开单个折叠区。
zc: 聚合单个折叠区。
zn: 展开全部折叠区。
zN: 聚合全部折叠区。
**6.5. NERDTree目录树**
简介：打开文件目录树，相当于文件浏览器。
下载：[http://vim.sourceforge.net/scripts/script.php?script_id=515](http://vim.sourceforge.net/scripts/script.php?script_id=515)
安装：
将整个解压后的源包拷贝到 ~/.vim 目录下，需要确保 NERD_tree.vim 位于 ~/.vim/plugin 目录下， NERD_tree.txt 位于 ~/.vim/doc 目录下。
使用<F7>作为快捷键开关目录树，在.vimrc写入：
```
map <F7> :NERDTreeToggle<CR>
```
**6.6. 自动补全 **
简介：可以对常用词进行提示补全。
下载：[http://www.vim.org/scripts/script.php?script_id=1879](http://www.vim.org/scripts/script.php?script_id=1879) 或 https://bitbucket.org/ns9tks/vim-autocomplpop/get/tip.zip
安装：
拷贝acp.vim 到 ~/.vim/plugin 目录下，拷贝acp.txt到~/.vim/doc 目录下。
【注意】 该插件有一个依赖对象l9.vim，所以还需要安装l9.vim插件。
**6.7. l9.vim **
简介：自动补全AutoComplPop的依赖库。
下载：[http://www.vim.org/scripts/script.php?script_id=3252](http://www.vim.org/scripts/script.php?script_id=3252)
安装：
将源包目录置于vim运行时目录下，确保 l9.vim 处于 ~/.vim/plugin 目录下， l9.txt 处于 ~/.vim/doc 目录下。
