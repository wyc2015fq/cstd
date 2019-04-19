# 我是怎样使用 Emacs 的 - 文章 - 伯乐在线
原文出处： [DarkSun](https://github.com/lujun9972/emacs-document/blob/master/emacs-common/%E6%88%91%E6%98%AF%E6%80%8E%E6%A0%B7%E4%BD%BF%E7%94%A8Emacs%E7%9A%84.org)
一直以来都想写一篇关于我当前 Emacs 配置的文章，来描述我是如何在 Mac 和 Linux 上使用 Emacs 的，即我的 Emacs 工作流。我使用这套配置一年多了，一直以来这套配置都工作的很好，几乎不需要怎么调整。我现在用的是 Emacs24，但是这套配置对更早些版本的Emacs也适用。
首先，我需要解释一下我是怎么工作的，以及我希望我的Emacs工作流能达到什么效果. 我大多数时候用的都是命令行，随机在终端或者Emacs的eshell中运行. 当然长时间的tail日志或者ssh操作可能导致Emacs崩溃因此这类操作我一般不在eshell中进行. 我之前用过很长一段时间点额vim，我习惯于快速打开一个文件，做一些修改，然后再关闭这个文件. 但是我也确实发现在Emacs中同时打开多个文件也很有用. Emacs daemon似乎能够满足这两种模式，但是我不喜欢在我登录系统或启动Emacs时自动启动该功能.
Avdi Grimm 曾经写过 [一篇文章关于他是如何运行Emacs的](http://devblog.avdi.org/2011/10/27/running-emacs-as-a-server-emacs-reboot-15/)，这篇文章給我以启示。在这之前我不太常用emacsclient. 但随后我发现它的 `-a` 参数能自动启动daemon，正是我所需要的. Avdi就是使用该脚本来启动emacsclient并创建新frame的. 默认情况下，终端会挂起并等待你关闭Emacs，但是你可以給 `ec` 脚本(脚本内容在后面)传递 `-n` 参数，这会使得控制权立刻回到终端.
# [](#my-emacs-workflow)My ‘Emacs Workflow’
我的工作流有点与众不同. 当在图形化的Emacs中打开一个文件的时候，我会编辑这个文件但不会去关闭它. 然后我经常会再回到终端去运行命令处理那个新编辑过的文件(rake test或mvn packge). 这意味着终端运行emacsclient后不能一直在那等待emacsclient运行完毕. 若已经有一个图形化的emacs在运行，我希望能在这个emacs中编辑文件而不是重新打开一个新frame. 若有一个图形化的Emacs在运行但是处于最小化状态，则我希望最大化这个emacs然后用它打开文件.
对于一下快速编辑的情况，我希望在当前终端用Emacs快速打开文件，做出修改，然后关闭该文件. 这时终端应该等待直到我完成修改.
## [](#sidebar)Sidebar
> 
我在学会magit前都是使用这种快速编辑的方法来提交git commit的. 若你没有用过magit，我强烈推荐你花点时间学习它. 参加 [http://magit.github.io/magit/magit.html](http://magit.github.io/magit/magit.html). 这也是为什么我在 `~/.bashrc` 中添加了 `export editor=et` 的缘故了.
# [](#tools)Tools
最终，我写了两个脚本: `ec` 和 `et`. 前一个脚本会在图形化Emacs中打开emacsclient然后将控制权立即交回shell. 后一个会在当前终端打开emacs并等待我完成编辑. 由于两个脚本都是连接的同一个daemon，因此所有打开的文件对两者都是可见的. 两个脚本都会在daemon未启动的情况下自动启动daemon. `ec` 脚本还有一些额外的代码用来实现我工作流中所描述的那样将焦点切换到emacs上. 下面是脚本的内容，我已经加上了注释了.
## [](#ec)ec

```
#!/bin/bash                                                                      
                                                                               
# This script starts emacs daemon if it is not running， opens whatever file      
# you pass in and changes the focus to emacs.  Without any arguments， it just    
# opens the current buffer or *scratch* if nothing else is open.  The following  
# example will open ~/.bashrc                                                    
                                                                               
# ec ~/.bashrc                                                                   
                                                                               
# You can also pass it multiple files， it will open them all.  Unbury-buffer     
# will cycle through those files in order                                        
                                                                               
# The compliment to the script is et， which opens emacs in the terminal          
# attached to a daemon                                                           
                                                                               
# If you want to execute elisp， pass in -e whatever.                             
# You may also want to stop the output from returning to the terminal， like      
# ec -e "(message \"Hello\")" > /dev/null                                        
                                                                               
# emacsclient options for reference                                              
# -a "" starts emacs daemon and reattaches                                       
# -c creates a new frame                                                         
# -n returns control back to the terminal                                        
# -e eval the script                                                             
                                                                               
# Number of current visible frames，                                              
# Emacs daemon always has a visible frame called F1                              
visible_frames() {                                                               
  emacsclient -a "" -e '(length (visible-frame-list))'                           
}                                                                                
                                                                               
change_focus() {                                                                 
  emacsclient -n -e "(select-frame-set-input-focus (selected-frame))" > /dev/null
}                                                                                
                                                                               
# try switching to the frame incase it is just minimized                         
# will start a server if not running                                             
test "$(visible_frames)" -eq "1" && change_focus                                 
                                                                               
if [ "$(visible_frames)" -lt  "2" ]; then # need to create a frame               
  # -c $@ with no args just opens the scratch buffer                             
  emacsclient -n -c "$@" && change_focus                                         
else # there is already a visible frame besides the daemon， so                   
  change_focus                                                                   
  # -n $@ errors if there are no args                                            
  test  "$#" -ne "0" && emacsclient -n "$@"                                      
fi
```
## [](#et)et

```
#!/bin/bash                                                          
                                                                   
# Makes sure emacs daemon is running and opens the file in Emacs in  
# the terminal.                                                      
                                                                   
# If you want to execute elisp， use -e whatever， like so             
                                                                   
# et -e "(message \"Word up\")"                                      
                                                                   
# You may want to redirect that to /dev/null if you don't want the   
# return to printed on the terminal.  Also， just echoing a message   
# may not be visible if Emacs then gives you a message about what    
# to do when do with the frame                                       
                                                                   
# The compliment to this script is ec                                
                                                                   
# Emacsclient option reference                                       
# -a "" starts emacs daemon and reattaches                           
# -t starts in terminal， since I won't be using the gui              
# can also pass in -n if you want to have the shell return right away
                                                                   
exec emacsclient -a "" -t "$@"
```
# [](#github-repo)Github repo
可以在[https://github.com/mjwall/dotfiles](https://github.com/mjwall/dotfiles) 中找到这两个脚本以及在Mac和Linux下安装Emacs的方法. 该仓库存放的其实是我的 `~/.emac.d` 配置. 我把所有的东西都放在一起，这样便于在新机器上重建配置，也便于在多态机器之间同步配置.
## [](#warning)Warning
> 
若你用的是Mac，请务必保证新版本的emacs和emacsclient被放在了正确的路径中. 可以去[gist](https://gist.github.com/mjwall/3fe935a8becb60dd3c4c) 上看看我是怎么做的. 当然肯定还有其他更好的实现方式吧.
# [](#bonus-executing-elisp)Bonus， executing elisp
我使用这两个脚本的另一种方式是使用 `-e` 选项来执行elisp代码. 例如，我就在我的bashrc中为magit设置了一个alias. 由于用到了这两个脚本因此它也能自动启动daemon并自动捕获焦点. 下面是定义alias的方法

```
alias magit='ec -e "(magit-status \"$(pwd)\")"'
```
这样在终端中运行magit就会调用Emacs并在当前目录下运行magit-status了. This was inspired by a similiar tweet somewhere， but takes advantage of the rest of the ec script.
# [](#stopping-the-daemon)Stopping the Daemon
最后还有一个脚本用于关闭daemon，这个脚本常用于重新加载emacs配置. 有时我的Mac在关机时会挂起等待Emacs退出，因此我一般都是先手工运行该脚本关闭Emacs. 脚本内容如下:
## [](#es)es

```
#!/bin/bash                                                                  
                                                                           
# simple script to shutdown the running Emacs daemon                         
                                                                           
# emacsclient options for reference                                          
# -a Alternate editor， runs bin/false in this case                           
# -e eval the script                                                         
                                                                           
# If the server-process is bound and the server is in a good state， then kill
# the server                                                                 
                                                                           
server_ok() {                                                                
  emacsclient -a "false" -e "(boundp 'server-process)"                       
}                                                                            
                                                                           
if [ "t" == "$(server_ok)" ]; then                                           
  echo "Shutting down Emacs server"                                          
  # wasn't removing emacs from ALT-TAB on mac                                
  # emacsclient -e "(server-force-delete)"                                   
  emacsclient -e '(kill-emacs)'                                              
else                                                                         
  echo "Emacs server not running"                                            
fi
```
似乎有很好的方法能解决Mac关机挂起的问题，但这个问题对我来说不是什么大问题，因此我没有深入这个问题.
# [](#wrap-up)Wrap up
若你读到了这里，你可能会想”在shell执行elisp代码真不错啊”。如果你有这个想法，可以看看 [https://github.com/mjwall/dotfiles/blob/master/bin/ed.el](https://github.com/mjwall/dotfiles/blob/master/bin/ed.el)， 你会看到下面这样的实现方式

```
#!/usr/bin/env emacs --script 
(print "Hi mike")             
(require 'server)             
(print (server-running-p))
```
想象一下吧. 借助这种能力，你可以遍历一个git仓库，将其源代码中的所有tab都替换成空格. 这样做可能没什么意义，但是想一想也觉得蛮有意思的。
如果你没有读到这里，可能是因为你觉得这样搞太小题大做了。嗯，没准你是对的。
