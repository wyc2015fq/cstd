# 常用vim插件 - Arthur的随笔 - CSDN博客
2011年11月16日 13:48:35[largetalk](https://me.csdn.net/largetalk)阅读数：1365
[http://blog.csdn.net/tge7618291/article/details/4216977](http://blog.csdn.net/tge7618291/article/details/4216977)
[http://www.aitilife.com/2011/04/10/vim%E5%B8%B8%E7%94%A8%E6%8F%92%E4%BB%B6%E5%A4%A7%E5%85%A8/](http://www.aitilife.com/2011/04/10/vim%E5%B8%B8%E7%94%A8%E6%8F%92%E4%BB%B6%E5%A4%A7%E5%85%A8/)
vim 插件管理利器: vimpyre
sudo apt-get install vimpyre 或者 pip install vimpyre
vimpyre init
vimpyre search xxxx
vimpyre install xxxx
```
$ vimpyre list_installed
=> => Send bats to collect all your vim-scripts
cvim-pathogen => https://github.com/HenningM/cvim-pathogen.git
pyfold => https://github.com/vim-scripts/pyfold.git
pythoncomplete => https://github.com/meermanr/pythoncomplete.git
dwm.vim => https://github.com/spolu/dwm.vim.git
```
```
jedi-vim => https://github.com/davidhalter/jedi-vim.git
nerdcommenter => https://github.com/scrooloose/nerdcommenter.git
TaskList.vim => https://github.com/djurczak/TaskList.vim.git
tasklist-pathogen => https://github.com/superjudge/tasklist-
pathogen.git
taglist.vim => https://github.com/esukram/taglist.vim.git
FindMate => https://github.com/vim-scripts/FindMate.git
nerdtree => https://github.com/scrooloose/nerdtree.git
pyflakes => https://github.com/vim-scripts/pyflakes.git
vimim => https://github.com/vimim/vimim.git
```
ubuntu 默认vim应该不能使用系统粘帖版，:reg查看寄存器里没有"+和"*
sudo apt-get install vim-gnome即可
```
$ vim --version | grep clipboard
+clientserver +clipboard +cmdline_compl +cmdline_hist +cmdline_info +comments 
+xsmp_interact +xterm_clipboard -xterm_save
```
“+y 复制内容到系统粘帖板
"+p 从系统粘帖板复制内容到vim
