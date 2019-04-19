# Vim——打造Python IDE - 宇宙浪子的专栏 - CSDN博客
2014年09月23日 09:47:55[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：588
本节将利用这些技术打造一个Python IDE。
１.文法高亮
　　为了能在Vim中支持Python文法需要用到插件python.vim，该插件默认位于<Vim安装目录>/<$VIMRUNTIME>/syntax/下，如果你在该路径下没有找到这个插件，需要到[python.vim : Enhanced version of
 the python syntax highlighting script](http://www.vim.org/scripts/script.php?script_id=790)下载。然后为了能让Vim识别Python文法需要在vimrc中添加：
```
set filetype=python
au BufNewFile,BufRead *.py,*.pyw setf python
```
2.缩进
　　在vimrc中添加如下缩进相关的代码：
```
set autoindent " same level indent
set smartindent " next level indent
set expandtab
set tabstop=4
set shiftwidth=4
set softtabstop=4
```
3.项目视图
　　像Visual Studio或Eclipse之类的IDE都会提供项目视图（位于左侧或右侧），程序员利用该视图在文件间或类间跳转。利用Ctags和插件Tasklist可以在vim中实现此功能。
- 首先下载[Exuberant Ctags](http://ctags.sourceforge.net/)
- 然后解压Ctags，并进入解压后的目录，利用如下命令编译安装Ctags：
```
./configure && sudo make install
```
- 通过这种方式，Ctags被安装在/usr/local/bin下。接下来在vimrc中添加如下命令告诉Vim Ctags的安装路径：
```
let Tlist_Ctags_Cmd='/usr/local/bin/ctags'
```
- 接着安装Tasklist插件：下载[TaskList.vim](http://www.vim.org/scripts/script.php?script_id=2607),然后把它放入plugin目录下
- 最后使用命令:TlistToggle打开taglist窗口，下图展示了该窗口。
![](http://pic002.cnblogs.com/images/2011/183560/2011042423032226.png)
4.MiniBufExplorer
　　在Visual Studio或Eclipse中你打开的缓存会以tab的形式列在窗口的顶端或底部，在Vim中插件MiniBufExplorer来实现此功能。下载[minibufexpl.vim](http://www.vim.org/scripts/script.php?script_id=159)并将其放在plugin目录下。接着在vimrc中添加如下命令：
```
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1
```
　　下图展示了MiniBufExplorer的使用效果：
![](http://pic002.cnblogs.com/images/2011/183560/2011042423235727.png)
5.Omnicompletion
　　Vim7中添加了对文法提示和自动完成的支持，对于python来说需下载[pythoncomplete.vim](http://www.vim.org/scripts/script.php?script_id=1542)并将其放在<Vim安装目录>/<$VIMRUNTIME>/autoload/目录下，接着在vimrc中添加如下命令：
```
filetype plugin on
set ofu=syntaxcomplete#Complete
autocmd FileType python　set
omnifunc=pythoncomplete#Complete
autocmd FileType python runtime! autoload/pythoncomplete.vim
```
　　最后在编写代码时通过ctrl-x ctrl-o来打开文法提示上下文菜单，如下图所示：
![](http://pic002.cnblogs.com/images/2011/183560/2011042423585456.png)
参考文献
1.http://www.swaroopch.com/notes/Vim
2.http://blog.dispatched.ch/2009/05/24/vim-as-python-ide/
3.http://www.phacks.net/macvim-code-completion-syntax-highlighting-for-python-pyqt4-twisted-on-mac-osx-snow-leopard/
4.http://vim.wikia.com/wiki/Omni_completion
本文转自：http://www.cnblogs.com/samwei/archive/2011/04/25/2026211.html
