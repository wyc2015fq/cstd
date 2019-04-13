
# vim配置 - 高科的专栏 - CSDN博客

2011年12月02日 18:02:02[高科](https://me.csdn.net/pbymw8iwm)阅读数：1875标签：[vim																](https://so.csdn.net/so/search/s.do?q=vim&t=blog)[autocomplete																](https://so.csdn.net/so/search/s.do?q=autocomplete&t=blog)[tags																](https://so.csdn.net/so/search/s.do?q=tags&t=blog)[namespaces																](https://so.csdn.net/so/search/s.do?q=namespaces&t=blog)[autoload																](https://so.csdn.net/so/search/s.do?q=autoload&t=blog)[insert																](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=autoload&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=namespaces&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)
[
																								](https://so.csdn.net/so/search/s.do?q=namespaces&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=tags&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=tags&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=autocomplete&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=vim&t=blog)
1.ctags
（1）到http://ctags.sourceforge.net/下载ctags源码ctags-5.6.tar.gz
http://prdownloads.sourceforge.net/ctags/ctags-5.6.tar.gz
（2）解压并安装
tar zxvf ctags-5.6.tar.gz
cd ctags-5.6
./configure && make && make install
（3）使用
[/home/brimmer/src]$ ctags -R
"-R"表示递归创建，也就包括源代码根目录下的所有子目录下的源程序。"tags"文件中包括这些对象的列表：
l 用\#define定义的宏
l 枚举型变量的值
l 函数的定义、原型和声明
l 名字空间（namespace）
l 类型定义（typedefs）
l 变量（包括定义和声明）
l 类（class）、结构（struct）、枚举类型（enum）和联合（union）
l 类、结构和联合中成员变量或函数
VIM用这个"tags"文件来定位上面这些做了标记的对象，下面介绍一下定位这些对象的方法：
1) 用命令行。在运行vim的时候加上"-t"参数，例如：
[/home/brimmer/src]$ vim -t foo_bar
这个命令将打开定义"foo_bar"（变量或函数或其它）的文件，并把光标定位到这一行。
2) 在vim编辑器内用":ta"命令，例如：
:ta foo_bar
3) 最方便的方法是把光标移到变量名或函数名上，然后按下"Ctrl-]"。用"Ctrl-o"退回原来的地方。
注意：运行vim的时候，必须在"tags"文件所在的目录下运行。否则，运行vim的时候还要用":set tags="命令设定"tags"文件的路径，这样vim才能找到"tags"文件。
在函数中移动光标
[{ 转到上一个位于第一列的"{"
}] 转到下一个位于第一列的"{"
{ 转到上一个空行
} 转到下一个空行 （[ and ] 也分别是两个指令）
gd 转到当前光标所指的局部变量的定义
* 转到当前光标所指的单词下一次出现的地方
\# 转到当前光标所指的单词上一次出现的地方
Vim 的创造者是一名计算机程序员，因此这就不奇怪 Vim 中有许多帮助编写程序的功能：
跳转到标识符被定义和使用的地方；在另一个窗口中预览有关的声明等等。
（ctags使用部分参考了 文章“ctags和vim”，原文在
http://hi.baidu.com/original/blog/item/2cf8d53f00b7fcc27d1e71f0.html，
更多使用也请参考原文）

2. taglist
能够列出源文件中的tag(function, class, variable, etc)并跳转.
注意：taglist依赖于ctags，所以要先装ctags，否则taglist装了也没法用！
（1）到http://vim.sourceforge.net/scripts/script.php?script_id=273
下载taglist_42.zip，即
http://vim.sourceforge.net/scripts/download_script.php?src_id=6416
（2）解压得到两个文件
\# unzip -d taglist taglist_42.zip
\# cd taglist
\# tree
.
|-- doc
| `-- taglist.txt
`-- plugin
`-- taglist.vim
（3）安装
cp doc/taglist.txt /usr/share/vim/vim61/doc/
cp plugin/taglist.vim /usr/share/vim/vim61/plugin/
（4）配置和使用
cd /usr/share/vim/vim61/doc/
启动vim，用 “:helptags .”来配置好帮助文件
重启vim，用“:TlistToggle”来打开和关闭taglist窗口。
可以用“:help taglist”来获得更多帮助信息

3.OmniCppcomplete:自动补全（需要先安装ctags）
下载地址：http://www.vim.org/scripts/script.php?script_id=1520
下载OmniCppComplete,解压可得到三个文件夹（autoload,after,doc）,分别复制到~/.vim/中。这样OmniCppComplete就安装完成了。
Add the following to your .vimrc file (you only need the required and ctags sections, but I included options I found useful – hack as you please):
" --- OmniCppComplete ---
" -- required --
set nocp " non vi compatible mode
filetype plugin on " enable plugins

" -- optional --
" auto close options when exiting insert mode
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
set completeopt=menu,menuone
" -- configs --
let OmniCpp_MayCompleteDot = 1 " autocomplete with .
let OmniCpp_MayCompleteArrow = 1 " autocomplete with ->
let OmniCpp_MayCompleteScope = 1 " autocomplete with ::
let OmniCpp_SelectFirstItem = 2 " select first item (but don't insert)
let OmniCpp_NamespaceSearch = 2 " search namespaces in this and included files
let OmniCpp_ShowPrototypeInAbbr = 1 " show function prototype (i.e. parameters) in popup window
" -- ctags --
" map <ctrl>+F12 to generate ctags for current folder:
map <C-F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR><CR>
" add current directory's generated tags file to available tags
set tags+=./tags
使用方法：
omnicppcomplete主要依赖ctags生成的tags文件来判断补全的，所以首先应该生成一定的信息。也就是说应该先把类写好，然后再生成tags，以后出现对象的　”.” 或　”->”成员操作时它自动寻找成员函数。
在vim编辑文件时按Ctrl+F12,ctags会把这个文件所在目录以及子目录的所有文件信息收集生成一个名为tags的文件存在当前编辑文件所在目录。然后就可以实现代码自动补全咯。 在~/.vimrc中添加的 map…… 那句宏就实现这个功能。

4.supertab插件安装
下载supertab插件，复制supertab.vim到vim安装目录的plugin目录下，使用时只要直接按tab键，就可以直接进行补全。
5. csupport (http://www.vim.org/scripts/script.php?script_id=213):模板
安装：
(1) 下载cvim.zip到～/.vim目录，unzip，在/etc/vim/vimrc中添加 filetype plugin on 即可使用该插件。
(2) 阅读README.csupport，需要进行的修改主要包括
(a) 修改c-support/templates/Templates
(b) 修改c-support/templates目录下的c.*.template文件中的代码风格
(c) 修改~/.vimrc文件，加入filetype plugin on
(d) a)进入vim，执行:helptags ~/.vim/doc/
(3) :h csupport-vim-usage获得帮助
5.文件浏览器和窗口管理器 -- 插件: WinManager:
下载地址：http://www.vim.org/scripts/script.php?script_id=1440
按照install details部分安装（第4步要更改）：
let g:winManagerWindowLayout = "FileExplorer|TagList"
这样输入:WMToggle就会调出wm了
6.阅读代码：cscope:
下载地址：http://sourceforge.net/project/showfiles.php?group_id=4664
软件安装:  (也可直接在新力得里面装)
./configure --with-flex  (注：如果平台是Linux,最好带上 --with-flex选项)
make  (注：我没有遇到错误)
make install (注： 直接安装好了）
修改vim配置文件vimrc.你可以修改/etc/vimrc使用所有用户都使用本配置文件，当然你还可以修改~/.vimrc
下载配置文件：[cscope_map.vim  ](http://cscope.sourceforge.net/cscope_maps.vim)
把cscope_map.vim里从 if has("cscope")  到 endif里边的内容复制到/etc/vimrc里边


