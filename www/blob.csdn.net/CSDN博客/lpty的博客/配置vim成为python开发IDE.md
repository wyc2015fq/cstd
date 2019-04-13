
# 配置vim成为python开发IDE - lpty的博客 - CSDN博客

2017年07月07日 22:39:32[lpty](https://me.csdn.net/sinat_33741547)阅读数：2050



## 前言
笔者之前一直使用Pycharm作为开发IDE，在图形界面下Pycharm还是比较友好的。但在shell窗口下，只能使用vi这类编辑器，没了自动补全功能，对开发效率还是有所影响。
## 环境
centos7
vim7.4
## 配置步骤
### 1 基础配置
这里我用了github上的一个开源配置[vim-for-server](https://github.com/wklken/vim-for-server)，按说明配置即可
### 2 自动补全 jedi-vim
#### （1）安装jedi
`pip install jedi`
#### （2）安装Vundle
`git clone https://github.com/gmarik/Vundle.vim.git ~/.vim/bundle/Vundle.vim
vim ~/.vimrc`将下述文件粘贴到配置文件的顶部
`set nocompatible              " required
filetype off                  " required
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')
" let Vundle manage Vundle, required
Plugin 'gmarik/Vundle.vim'
" Add all your plugins here (note older versions of Vundle used Bundle instead of Plugin)
" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required`保存退出，打开vim编辑器，执行下述命令
`:PluginInstall`
#### （3）安装jedi-vim
添加以下语句到配置文件
`Bundle 'davidhalter/jedi-vim'`保存退出，打开vim编辑器，执行下述命令
`:PluginInstall`Vundle将自动下载安装好插件
### 2 调试工具 ipdb
由于默认安装的ipdb要求python版本在3.3以上，这里下载了一个0.8.3版本的[ipdb](https://pypi.tuna.tsinghua.edu.cn/simple/ipdb/)
先升级pip版本，为加快速度这里用了清华的pypi源
`pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple`安装下载好的ipdb包
`pip install ipdb-0.8.3.tar.gz  -i https://pypi.tuna.tsinghua.edu.cn/simple`提示ipython版本问题，要求安装5.X版本，自行[下载](https://pypi.tuna.tsinghua.edu.cn/simple/ipython/)
`pip install ipython-5.1.0-py2-none-any.whl -i https://pypi.tuna.tsinghua.edu.cn/simple`提示gcc问题，安装python-devel，gcc
`yum install python-devel
yum install gcc`
### 三 树形目录 NERD-tree
#### （1）安装NERD-tree
这里使用Vundel安装
`vim ~/.vimrc`修改配置文件，添加以下语句
`Bundle 'The-NERD-tree'
Bundle 'The-NERD-Commenter'`打开vim，执行以下命令
`:BundleInstall`（2）配置
打开vim自动开启目录
`autocmd VimEnter * NERDTree`F2开启/关闭目录
`map < F2> :NERDTreeToggle< CR>`隐藏pyc后缀的文件
`let NERDTreeIgnore=['.pyc']`退出vim自动关闭目录
`autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") &&b:NERDTreeType == "primary") | q | endif`
### 四 代码跳转 ctags
#### （1）跳转import文件
`vim ~/.vimrc`添加以下语句
`python << EOF
import os
import sys
import vim
for p in sys.path:
    if os.path.isdir(p):
        vim.command(r"set path+=%s" % (p.replace(" ", r"\ ")))
EOF`使用gf命令，即go file便可以跳转到import进来的文件源码处
#### （2）跳转函数
安装ctags
`yum install ctags`生成python库文件tag
`mkdir ~/.vim/tags
ctags -R -f ~/.vim/tags/python.ctags /usr/lib/python2.7/`添加以下语句到~/.vimrc配置文件
`set tags+=$HOME/.vim/tags/python.ctags
map <silent><C-Left> <C-T>
map <silent><C-Right> <C-]>`即可使用CTRL+LeftArrow和CTRL+RightArrow实现函数跳转/返回
### 五 未完待续

