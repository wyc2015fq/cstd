# Linux下的神器介绍之Tmux分屏器 - weixin_33985507的博客 - CSDN博客
2017年04月30日 23:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
## 前言
我们为什么需要分屏器呢？
对于这个问题，我想大家肯定都有自己的看法。
主流的观点是这样的，对于生活在Linux下的人（开发人员、运维人员、普通爱好者）都不可避免的使用终端模拟器（比如，`gnome-terminal`）去执行一些命令或者脚本。但是，通常一个终端是不够用的。
比如，你想一边观察着系统的运行状态一边用`vim`写个脚本，当你运行了个`top`命令后，你发现这个终端被占用了，你不能够再用这个终端去干其他的事了，所以，一些伟大的工程师就开发了个tmux神器去帮你解决这个问题。
> 
注：tmux的特性远不止于终端复用，它的功能很强大，比如它可以保存会话session，下次直接恢复会话就可以了工作了，是不是很惬意。*注意，运行tmux的服务器关闭之后，session会丢失。*不过有解决办法，tmuxinator就是来解决这个问题的，之后我会继续讲解的。
说了这么多tmux的强大，不亲眼见识下也不知道啊，让我们把它安装到自己的系统吧！
## tmux安装
> 
注：本人的系统是ArchLinux+lxde
archlinux下的安装：`$ pacman -S tmux`
ubuntu下的按照：`apt-get install tmux`
## tmux的基本概念
我们先来理解下tmux的几个元素。tmux的主要元素分为三层：
- Session 一组窗口的集合，通常用来概括同一个任务。session可以有自己的名字便于任务之间的切换。
- Window 单个可见窗口。Windows有自己的编号（默认是从0开始编号的）。
- Pane 窗格，被划分成小块的窗口，类似于Vim中 C-w +v 后的效果。
为了更好的理解，下面是三个元素在tmux中的具体展现。
tmux-concept
可以看到Session总在tmux的左下角显示，通常会为他命名。例如我正在写博客，开了很多窗口那么我就会把这组窗口命名为blog方便之后的重连和切换。而Window也会在最下方以一种Tab的形式展现。每个window都有自己的需要，也会以当前活动进程的名字命名。而Pane就比较好理解，即把单个窗口分割成若干个小块后的元素。
## 配置tmux
我们可以先进行一些简单的配置，修改 ~/.tmux.conf 即可，让整个使用更方便。
```
#解除Ctrl+b 与前缀的对应关系
unbind C-b
#设置前缀为Ctrl + a
set -g prefix C-a
#将r设置为加载配置文件
bind r source-file ~/.tmux.conf
#copy-mode 将快捷键设置为vi 模式
setw -g mode-keys vi
```
tmux默认的前置操作是CTRL+b，在此，我们改成CTRL+a，也可以不该，主要看个人习惯，我认为CTRL+a按起来比较方便。
tmux的操作基本上都是Prefix-Command模式，即先按前置键，再进行实际命令操作。
例如，我们想要新建一个窗体（前提是已经创建了session），就需要先在键盘上摁下CTRL+a，松开后再摁下c键。
## tmux的基本操作
### session操作
创建session
`$ tmux new-session -s mysession1`
然后输入CTRL-a d从此会话脱离，想要重新连接此会话，需输入：
`$ tmux attach-session -tmysession1`
一些管理sessions的命令：
```
$ tmux list-session
$ tmux new-session -s <会话名>
$ tmux attach-session -t <会话名>
$ tmux rename-session -t <会话名>
$ tmux choose-session -t <会话名>
$ tmux kill-session -t <会话名>
```
***所有下面介绍的快捷键，都必须以前置操作开始。***
### 基本操作
- ? 列出所有快捷键；按q返回
- d 脱离当前会话,可暂时返回Shell界面
- s 选择并切换会话；在同时开启了多个会话时使用
- D 选择要脱离的会话；在同时开启了多个会话时使用
- : 进入命令行模式；此时可输入支持的命令，例如 kill-server 关闭所有tmux会话
- [ 复制模式，光标移动到复制内容位置，空格键开始，方向键选择复制，回车确认，q/Esc退出
- ] 进入粘贴模式，粘贴之前复制的内容，按q/Esc退出
- ~ 列出提示信息缓存；其中包含了之前tmux返回的各种提示信息
- t 显示当前的时间
- ctrl + z 挂起当前会话
### 窗口操作
- c 创建新窗口
- & 关闭当前窗口
- [0-9] 数字键切换到指定窗口
- p 切换至上一窗口
- n 切换至下一窗口
- l 前后窗口间互相切换
- w 通过窗口列表切换窗口
- , 重命名当前窗口，便于识别
- . 修改当前窗口编号，相当于重新排序
- f 在所有窗口中查找关键词，便于窗口多了切换
### 面板操作
- " 将当前面板上下分屏
- % 将当前面板左右分屏
- x 关闭当前分屏
- ! 将当前面板置于新窗口,即新建一个窗口,其中仅包含当前面板
- ctrl+方向键 以1个单元格为单位移动边缘以调整当前面板大小
- alt+方向键 以5个单元格为单位移动边缘以调整当前面板大小
- q 显示面板编号
- o 选择当前窗口中下一个面板
- 方向键 移动光标选择对应面板
- { 向前置换当前面板
- } 向后置换当前面板
- alt+o 逆时针旋转当前窗口的面板
- ctrl+o 顺时针旋转当前窗口的面板
- z 最大化当前所在面板
- page up 向上滚动屏幕，q 退出
- page down 向下滚动屏幕，q 退出
## 参考文章
- [Tmux - Linux从业者必备利器](https://link.jianshu.com?t=http://cenalulu.github.io/linux/tmux/)
- [优雅地使用命令行：Tmux 终端复用](https://link.jianshu.com?t=http://harttle.com/2015/11/06/tmux-startup.html)
- [tmux 指南](https://link.jianshu.com?t=http://wdxtub.com/2016/03/30/tmux-guide/)
- [Tmux 快速教程](https://link.jianshu.com?t=http://blog.jeswang.org/blog/2013/06/24/tmux-kuai-su-jiao-cheng/)
- [Linux下终端利器tmux](https://link.jianshu.com?t=http://kumu-linux.github.io/blog/2013/08/06/tmux/)
