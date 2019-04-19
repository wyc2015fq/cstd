# Linux基础(六) Vim之vundle插件 - _天枢 - 博客园
## [Linux基础(六) Vim之vundle插件](https://www.cnblogs.com/yhleng/p/8405996.html)
2018-02-02 17:00 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8405996)
## 背景
Vim缺乏默认的插件管理器，所有插件的文件都散布在~/.vim下的几个文件夹中，插件的安装与更新与删除都需要自己手动来，既麻烦费事，又可能出现错误。
## Vundle简介
Vundle 是 Vim bundle 的简称,是一个 Vim 插件管理器. 
**Vundle 允许你做…**
- 在.vimrc中跟踪和管理插件
- 安装特定格式的插件(a.k.a. scripts/bundle)
- 更新特定格式插件
- 通过插件名称搜索Vim scripts中的插件
- 清理未使用的插件
- 可以通过单一按键完成以上操作,详见[interactive mode](https://github.com/VundleVim/Vundle.vim/blob/v0.10.2/doc/vundle.txt#L319-L360)
## **安装vundle**
**前提条件:**
**linux系统已安装git版本管理工具,测试一下是否安装:**
```
git --version  #查看git版本,如果安装了,可以查到
```
**![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180202162344078-1242081455.png)**
**四步完成vundle插件安装**
**1.在linux命令行下执行git命令下载vundle版本仓库:**
**vundle版本库地址为:https://github.com/VundleVim/Vundle.vim.git**
**以下命令:下载vundle版本库到 /home/当前用户名/.vim/bundle/vundle.vim目录下**
**.vim代表是隐藏目录,如果查看目录ls是无法显示的,要用 ls -al**
**命令:**
```
$ git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
```
**2.在用户主目录(也就是~/)下创建一个.vimrc空文件,命令**
```
touch ~/.vimrc #touch命令一般用于修改时间,如果文件不存在创建一个空文件
```
**3.编辑.vimrc文件**
命令:
```
vim ~/.vimrc
```
**在.vimrc文件中写入以下配置:**
**必要解释:**
Plugin 'VundleVim/Vundle.vim'
Plugin 是下载插件用的命令,单引号中的是git版本库地址去掉了公共部分:  https://github.com/VundleVim/Vundle.vim.git
配置中要下载安装哪个插件,要看你自己的需求,无需按照下面来配置.只需把必须的加上即可.
```
set nocompatible              " 去除VI一致性,必须要添加
filetype off                  " 必须要添加
set rtp+=~/.vim/bundle/Vundle.vim " 设置包括vundle和初始化相关的runtime path
call vundle#begin()           "必须加
Plugin 'VundleVim/Vundle.vim' "示例
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'} 示例
......
......
......
call vundle#end()            " 必须
filetype plugin indent on    " 必须 加载vim自带和插件相应的语法和文件类型相关脚本
```
配置:
```
set nocompatible              " 去除VI一致性,必须要添加
filetype off                  " 必须要添加
" 设置包括vundle和初始化相关的runtime path
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" 另一种选择, 指定一个vundle安装插件的路径
"call vundle#begin('~/some/path/here')
" 让vundle管理插件版本,必须
Plugin 'VundleVim/Vundle.vim'
" 以下范例用来支持不同格式的插件安装.
" 请将安装插件的命令放在vundle#begin和vundle#end之间.
" Github上的插件
" 格式为 Plugin '用户名/插件仓库名'
Plugin 'tpope/vim-fugitive'
" 来自 http://vim-scripts.org/vim/scripts.html 的插件
" Plugin '插件名称' 实际上是 Plugin 'vim-scripts/插件仓库名' 只是此处的用户名可以省略
Plugin 'L9'
" 由Git支持但不再github上的插件仓库 Plugin 'git clone 后面的地址'
Plugin 'git://git.wincent.com/command-t.git'
" 本地的Git仓库(例如自己的插件) Plugin 'file:///+本地插件仓库绝对路径'
Plugin 'file:///home/gmarik/path/to/plugin'
" 插件在仓库的子目录中.
" 正确指定路径用以设置runtimepath. 以下范例插件在sparkup/vim目录下
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" 安装L9，如果已经安装过这个插件，可利用以下格式避免命名冲突
Plugin 'ascenator/L9', {'name': 'newL9'}
" 你的所有插件需要在下面这行之前
call vundle#end()            " 必须
filetype plugin indent on    " 必须 加载vim自带和插件相应的语法和文件类型相关脚本
" 忽视插件改变缩进,可以使用以下替代:
"filetype plugin on
"
" 常用的命令
" :PluginList       - 列出所有已配置的插件
" :PluginInstall     - 安装插件,追加 `!` 用以更新或使用 :PluginUpdate
" :PluginSearch foo - 搜索 foo ; 追加 `!` 清除本地缓存
" :PluginClean      - 清除未使用插件,需要确认; 追加 `!` 自动批准移除未使用插件
"
" 查阅 :h vundle 获取更多细节和wiki以及FAQ
" 将你自己对非插件片段放在这行之后
```
配置.vimrc文件写好后,**按冒号输入wq保存退出**
这里多说一句,如果提示你不能写,估计是你**没有权限**,先查一下该文件有没有写权限,如果有,那可能是用户没有权限
可以**su root切换到root用户进行编辑.**
**4.执行BundleInstall命令来安装,配置中的插件**
命令行下,输入命令:vim进入vim编辑输入冒号然后输入BundleInstall命令来安装插件
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180202165411375-110863588.png)
进入自动安装模式,不要急,仔细看界面,自己已经在安装了.
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180202165501281-1878482579.png)
安装完成后,退出退可.  上图中我是有一个安装失败了.......其它的安装成功.插件安装的步骤就是这样.
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
