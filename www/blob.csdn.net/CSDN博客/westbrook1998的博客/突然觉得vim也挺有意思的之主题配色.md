# 突然觉得vim也挺有意思的之主题配色 - westbrook1998的博客 - CSDN博客





2018年04月17日 22:14:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：372







- 安装vim
- 
```
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
```
安装这个插件管理
- 在~/.vimrc文件里添加

```
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Vundle
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
"Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
"Plugin 'L9'
" Git plugin not hosted on GitHub
"Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
"Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
"Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Avoid a name conflict with L9
"Plugin 'user/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
```
- 在~/.vimrc文件的插件列表中加入`Plugin 'vim-scripts/khaki.vim'`
- 到vim中命令行`:PluginInstall`安装插件
- 在~/.vimrc文件中加入（khaki可替换成其他主题，但是要下载该主题的vim文件在`～/.vim/bundle/khaki.vim/colors`下）

```
if !has("gui_running")                                          
    set t_Co=256                                                
endif                                                                                         
colorscheme khaki
```
- 重启vim，成功



