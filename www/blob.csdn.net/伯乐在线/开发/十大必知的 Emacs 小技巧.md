# 十大必知的 Emacs 小技巧 - 文章 - 伯乐在线
原文出处： [DarkSun](https://github.com/lujun9972/emacs-document/blob/master/emacs-common/%E5%8D%81%E5%A4%A7%E5%BF%85%E7%9F%A5%E7%9A%84Emacs-tips.org)
下面是我经常使用的Emacs功能以及一些小技巧。想看更多这类东西的话，请移步[Guided Tour of Emacs](http://www.gnu.org/s/emacs/tour/)。
### 触手可及的文档
除非你能够很容易的查阅文档，否则很难去学(甚至很难想去学)Emacs. 关于阅读文档，有一些有用的命令，其中最有用的有以下几个命令: C-h C-h (关于帮助的帮助，必不可少的!) C-h k， C-h f 以及 C-h v (分别用于显示按键，函数和变量的说明;必不可少的) C-h r (在Emacs内阅读Emacs manual).
我再没看到有其他什么程序有像Emacs那么完善的文档了. 而且通过这些文档命令来查询比你用Google查询要来的更快也更直接.
[More information about help](http://www.gnu.org/software/emacs/manual/html_node/emacs/Help.html).
### Keyboard macros(录制键盘操作的宏)
我在写这篇文章的时候就用到了keyboard macros， and I’m only on #2. 在大多数编辑器中，macro智能用于完成那些最简单，最单调的任务，因为在那些编辑器中光用键盘，你几乎作不了什么事. Emacs的与众不同之处在于它拥有大量的高层命令(high-level command)，这些命令可以让你定位和修改字符，单词，行，句子，函数，段落，页面以及整个文件)
若你知道如何在Emacs中用键盘来完成某些任务，则你只需要多按下那么几个按键就能不断的重复这些动作–根本无需编程! 要录制macro， 先按下 `F3`， 然后进行你想要被录制的那些操作，再按下 `F4` 停止录制. 然后就可以通过不断的的按下 `F4` 来重复这些操作. (相比之下，你可能需要学习一个全新的脚本语言才能自动化某些任务-比如Office中的Visual Basic-然后还要弄明白怎么用这门新语言来完成你要完成的任务，这真是一个巨大的阻力)
Emacs macros 有两个王牌特性:
- 一旦你定义了一个宏，你可以让它不断的执行，直到Emacs发现异常情况. 比如，若你定义了一个macro来修改某一行并移动到下一行，则 `M=0 F4` 会让Emacs修改所有行直到光标移动到最后一行.
- 计数器(Counter)允许在每次运行macro时都插入不同的数字. 当用于声称连续(或非连续)的编号列表时很有用.
[More information about macros](http://www.gnu.org/software/emacs/manual/html_node/emacs/Keyboard-Macros.html).
### 矩形编辑
虽说XML最近变得很热门，但还是有很多人要处理按列排列的数据. 这种数据在常见的Unix系统中随处可见. Emacs的矩阵命令可以让你修改，拷贝和移动文本中的一个矩形块; 要指定一个矩形块，你只需要将mark放在矩形的一个角，然后将光标放在矩形的对角上就行了. 你也可以通过在一系列连续行的相同位置插入相同文本的方式来插入一个矩形.
[More information about rectangles](http://www.gnu.org/software/emacs/manual/html_node/emacs/Rectangles.html).
### The mark ring
你再也不同在buffer中到处找”刚才的那个位置”了. 每次你跳转了光标(e.g. 进行了文本搜索或者按下了 `M-<` 或 `M->`)， Emacs使用mark标记下你的前一个位置. 这就好像你在看书时用手指插入某一页然后将书翻到另一页一样. 你按下 `C-x C-x` 就会跳转到那个mark处. 不仅如此，Emacs会保存多达16个mark，你使用 `C-u C-SPC` 跳转到更早些时候的mark处. 这就使得mark和mark ring成为一个很好用的导航工具了. 你完全可以不假思索的使用它: 当你想跳回上之前的位置时，不断按下 `C-u C-SPC` 直到你回到想要的位置为止.
(你也可以显式的用 `C-SPC` 设置mark， 但是我几乎不需要通过这种方式来设置导航的坐标，只是在标示region时才会这样做.)
[More information about the mark ring](http://www.gnu.org/software/emacs/manual/html_node/emacs/Mark-Ring.html).
### Ediff
用Ediff来比较一个文件的两个版本很方便. 我经常使用的一个命令是 `M-x ediff-buffers`. Emacs会高亮buffer间不同的地方并弹出一个新窗口供你输入命令. 例如，在新窗口中，按下n和p可以两buffer不同内容的region之间跳转. 对于每个region，你可以用a(或b)拷贝第一个(或第二个)buffer的内容到另一个buffer. 你甚至可以在激活Ediff的情况下编辑buffer. 然后再却换到Ediff窗口然后按下!来重新比对不同之处. 在编辑文件的同时能够交互式地比对两个文件的不同之处，对于编辑文件那是相当有帮助.
[More information about ediff](http://www.gnu.org/software/emacs/manual/html_node/ediff/index.html).
### Tramp
你是否试过新开一个shell来运行Emacs编辑文件?(例如打开一个root shell，或者是远程主机上的shell)， 实际上，借助Tramp可以满足大多数的要求而无需重开一个新shell. Tramp允许你像编辑本地文件一样编辑”远程”文件，它会自动为你做打开shell，读取和写入数据之类的事情. 你只需要按下 `C-x C-f` 并在输入文件名的地方以一种特殊的语法指定远程文件就可以了. 像这样: /ssh:phil@remotehost:records/pizza-toppings.txt. 我上面所说的”远程”加了引号是因为Tramp足够灵活，你可以用它来以其他用户的身份来编辑本地文件-只需要通过su或sudo语法就行. 像这样: /sudo::/etc/hosts.
[More information about Tramp](http://www.gnu.org/software/emacs/manual/html_node/tramp/index.html).
### Compilation-mode and friends
Emacs中大量的封装并增强了那些你早已熟知的工具的功能.这些工具包括compiler，grep等等. 举个例子，当你使用 `M-x compile` 运行make时， Emacs会在一个新window中显示编译器的输出结果. 当有编译错误出现时，Emacs会高亮这些错误并找出这些错误的所在行号. 按下 `C-x `` (M-x next-error)会直接跳转到源代码中第一个错误的那一行; 不断的按 `C-x `` 会跳转到接下来的那个错误所在位置.(你也可以直接在编译输出buffer中点击出错的条目.) 你还可以使用 `M-g n` 和 `M-g p` (分别表示next-error与previous-error)在错误列表中前后跳转.
这套机制足够普适以至于你可以在 `M-x grep` 和 `M-x occur` 的输出结果中也使用相同的按键来跳转到相应的行位置. 总之，你真的完全不需要关注程序输出的文件名和行号是什么，Emacs可以自动为你跳转到那些位置上去.
[More information about “compilation mode”](http://www.gnu.org/software/emacs/manual/html_node/emacs/Compilation-Mode.html).
### VC
我的个人项目都是适用Git作为版本控制的工具的，而有些我参与的项目则会适用CVS或SVN. Emacs提供了一个名为VC的package，它允许我们在Emacs中进行许多版本控制的操作. 它提供了一个统一的操作层:不管各个项目实际使用的版本控制系统是什么，用户所使用的命令都是一样的. 这套机制很棒，因为这意味着不管是bzr还是hg或者其他什么版本控制系统变得流行起来，我完全不需要学习这些版本控制系统就能直接拿来用了.
我的一般工作流程是这样的: 打开一个文件. 修改并进行测试. 使用 `C-x v =` 来查看做出了哪些改变. 如果确定了这些带动，我按下 `C-x v v` 准备进行提交. Emacs会弹出一个新window供你输入提交信息. 输入提交信息后，按下 `C-c C-c` 确认提交.
VC还有其他一些有用的功能，比如它能显示文件的annotated versions， 显示指定文件的修改日志，帮助你回顾历史版本并对比出版本间有什么不同.
[More information about VC](http://www.gnu.org/software/emacs/manual/html_node/emacs/Version-Control.html).
### Emacs server and multi-TTY support
Emacs23开始支持Multi-TTY， 这使得打开新Emacs frame快速而painless. 当你在shell中打开一个文件或运行那些会调用$EDITOR的程序时可以用到这一特性. (然而Emacs本身就有那么多的功能，基本上不太需要运行什么外部程序.)
为了使用multi-TTY，你需要在一个运行中的Emacs实例中运行 `M-x server-start`. 然后设置$EDITOR为 `emacsclient -t`. 当一个程序调用默认编辑器时，emacsclient会连接上你已经存在的这个Emacs实例，这个Emacs实例就会在当前的TTY中打开一个新的frame. 看起来你好像是运行了一个emacs似的，但是你可以获取另一Emacs实例的所有状态:所有的buffer，kill ring的内容等等. 而且它的启动速度也非常的快速. 当你编辑完后，按下 =C-x #=关闭这个frame.
我在很多其他场合(非编辑文件)也会用 `emacsclient` 来调用emacs. 例如，我不用man命令来阅读man page，相反，我在Emacs中阅读这些man page. 下面是我的 `.bashrc` 中的相关代码片段:


```
pps_man() {
    /usr/bin/emacsclient -t -e "(woman \"$1\")"
}
alias man=pps_man
```
### global-set-key
每个人都有不同的常用命令集合. 为那些最常用的Emacs功能分配快捷键可以节省你的时间. 你可以用下面语句来将shell命令的快捷键设置为 `C-c s`:


```
(global-set-key "\C-cs" 'shell)
```
`C-c [any letter]` 是专门预留给用户自定义快捷键的，所有的major和minor mode都应该准从这一规范.
More information about [key bindings](http://www.gnu.org/software/emacs/manual/html_node/emacs/Key-Bindings.html) and [key binding conventions](http://www.gnu.org/software/emacs/manual/html_node/elisp/Key-Binding-Conventions.html).
