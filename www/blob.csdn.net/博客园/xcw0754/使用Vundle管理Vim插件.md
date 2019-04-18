# 使用Vundle管理Vim插件 - xcw0754 - 博客园
# [使用Vundle管理Vim插件](https://www.cnblogs.com/xcw0754/p/8391711.html)
附：[仓库地址](https://github.com/VundleVim/Vundle.vim)
附：[我自用的vim配置文件](https://github.com/xcw0754/vimrc)，里面注释很多，仅供参考。
Vundle是一款Vim的插件管理软件(Linux)，用起来很方便的原因有几点：
- 支持插件超多，可以来源于github、Vundle自带、Vim-scripts、本地仓库等等。
- 安装流程非常简单，配置好vimrc后一键安装/更新所有
- Vundle很容易上手，很方便
### 快速使用
请先自行安装git。
下载vundle到指定位置
`$ git clone https://github.com/VundleVim/Vundle.vim.git   ~/.vim/bundle/Vundle.vim`
在vimrc文件中添加如下代码：
```
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
Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}
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
```
最后打开vim，输入如下命令：
`:PluginInstall`
它现在正在根据vimrc中所指定的插件列表在自动下载并安装插件了，过程可能比较久，慢慢等。
### 帮助
在安装完成之后就可以使用如下命令查看Vundle的一些使用方法：
`:h vundle`
常用命令有如下四个：
`:BundleUpdate` 更新插件
`:BundleClean` 清除不再使用的插件
`:BundleList` 列出所有插件
`:BundleSearch` 查找插件，找到后按`i`即可安装
针对某个插件如ctrlp.vim，可以用如下命令查看该插件的信息：
`:h ctrlp`
注：
- `:PluginSearch`指令所找出来的插件都在[vim-scripts](https://github.com/vim-scripts)，这个仓库里可以找到一些插件的设置方法，文档说明等。
- 关于vimrc应该怎么编写，请看Vundle的说明文档，或者仿照着写应该问题不大。
- Vundle本身也是vim的一个插件，原理就是插件管理着插件而已。
### 离线安装插件
你可能会自己写个插件，或者由于种种原因，无法直接使用网络方式安装，而是通过自己下载好了插件再让vundle管理，此时就需要另外的方法了。
首先是**主题**的安装，比如喜欢用`molokai`这种主题，先下载到`molokai.vim`这个文件，这个文件你可以去github或者其他地方找到，然后拷到本机的`.vim/colors/`目录下，在`.vimrc`中写一句`Plugin 'molokai'`就可以了，这和在线安装是一样的，重启下vim就生效，无需再去PluginInstall。
其次是其他插件的安装，先找到这个插件的整个文件夹，下载后放在`.vim/bundle/`目录下，直接整个文件夹放进去，然后在`.vimrc`中写一句`Bundle 'xxx'`就行了。举个例子，`The-NERD-tree`这个插件用于在vim中查看目录树，如果想要离线安装它，先从github上将仓库clone下来，丢到`.vim/bundle/`目录下，在`.vimrc`中写上`Bundle 'The-NERD-tree'`，重启vim就生效。要注意，这样其实插件还是让vundle管理的。

