# 学 Vim 时希望早点知道的建议 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [pine_pie](http://www.jobbole.com/members/char_c) 翻译，[米开](http://www.jobbole.com/members/wx1744867243) 校稿。未经许可，禁止转载！
英文出处：[Petr Zemek](http://blog.petrzemek.net/2016/04/06/things-about-vim-i-wish-i-knew-earlier/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
从 2009 年开始，我就一直把 [Vim](http://www.vim.org/) 当做我的主要（唯一）文本编辑器。在过去的这些年，我学到了很多好用的 Vim 技巧，它们令我感觉相见恨晚，因为它们极大地提高了我的文本编辑效率。在这篇博文中，我想与你们分享其中的最精华部分。
## HJKL
Vim 新手通常会收到的第一条建议是“使用 h/j/k/l 键代替方向键！”在很长一段时间，我都忽略了这一条建议因为我觉得那很荒谬：我究竟为何非得那么做？然而，有天我还是决定试一试，并且一试便一发不可收拾了。虽然我花了很多天时间才适应了这条建议，可是一旦上手，我才恍然大悟。h/j/k/l 几个键直接处于[本位列](https://en.wikipedia.org/wiki/Touch_typing#Home_row)上，所以你的手不用（像在使用方向键时那样）移动太远的距离。这不仅节省了键盘操作时间，提高了效率，而且让我的双手得以更加放松。我相信这是治好我[手腕疼](https://en.wikipedia.org/wiki/Carpal_tunnel_syndrome)的因素之一。另一个因素是，我在手下面垫了一块腕垫。
如果你想学会使用 h/j/k/l 键代替方向键，那我建议你[要么关闭方向键要么把它们重映射为其他更有用的指令](http://jeetworks.org/vim-making-those-arrow-keys-work-for-you-or-why-the-anti-arrow-key-propoganda-is-wrong/)。另一个不使用方向键的好处是，你将会适应更好的移动指令，如 f/F/t/T，w/W/b/B，0/^/$，或者搜索指令。使用这些指令，比起简单地按下方向键或者 h/j/k/l 键，你的输入效率会更高。在 Vim 中，单键指令是一种反面模式。
## 相对行号
像很多用户一样，我也使用绝对行号，好吧，所有用户都使用它。然而，有天我偶然发现了[这篇文章](http://jeffkreeftmeijer.com/2012/relative-line-numbers-in-vim-for-super-fast-movement/)，令我陷入思考：万一相对行号更好用呢？我决定用用看。然后，就像 Ben 在[这个视频](https://www.youtube.com/watch?v=C0H-LyZy9Ko)中所言，绝对行号在我的生活中就是一个谎言。Vim的正宗用法是启用相对行号。
启用相对行号之前，我建议把下列命令行添加到 .vimrc 文件中：

Vim
```
set number
set relativenumber
```
这两个命令行一起，使 Vim 对当前行显示绝对行号，而对其它行显示相对行号。
接下来，我要说明相对行号的用处。很多 Vim 命令都有一个数字作为前缀。例如，要移动当前行以下的 10 行，可以用 10j。要移动当前行以上的 4 行，使用 4k。要对当前行及以下 2 行缩进，使用 >2j。明白了吧？有了相对行号，其它行（距离当前行）有多远就一目了然了。如果你使用的是绝对行号，你要知道两行之间相距多少行就必须做口算，把两个行号（可能很大）相减。使用相对行号就可以减少这种数学运算。
同时，你仍然可以使用绝对行号。例如，你编译了一个源文件，然后编译器告知第 943 行有一个语法错误，这时你可以用 943G 或 :943<CR> 定位到这一行。
Vim 的一个非常好的特性是，你可以设置[折行功能](http://vimcasts.org/episodes/soft-wrapping-text/)。很多用户包括我会把 j/k 键重映射为 gj/gk，使得按下它们时，光标按虚拟行而不是按物理行移动。然而，这种重映射影响了前文提到的计数功能。为了弥补这一不足，基于[这篇stackoverflow.com的博文](http://stackoverflow.com/a/21000307/2580955)，我们重新进行如下映射：

Vim
```
noremap <silent> <expr> j (v:count == 0 ? 'gj' : 'j')
noremap <silent> <expr> k (v:count == 0 ? 'gk' : 'k')
```
这样之后，当遇到没有行号的行时，gj/gk 命令会使光标按虚拟行移动，而当遇到有行号的行时，光标则按物理行移动。这与相对行号一起，堪称绝配。
## Vim and Tmux
在很长一段时间里，我都是使用 GVim，不涉及 Vim 终端。因为它更炫，与窗口环境及集成度更高，等等。后来，我还是发现了 [Tmux](https://tmux.github.io/)，它极大地改善了我的工作流程。简言之，通过它，你可以在一个终端中打开多个独立的会话（session），在每个会话中又可以打开多个窗口（window），而每个窗口又可以分为多个窗格（pane）。看看[这个截屏](https://tmux.github.io/ss-tmux2.png)感受一下吧。
Tmux 也让我进退两难：怎么把它跟 GVim 配合使用呢？Tmux 是一款终端应用，而 GVim 则是一款独立的 GUI 应用。幸运的是，通过设置，Vim 几乎可以具备与 GVim 一样的功能。最好用的插件是[CSApprox](https://github.com/godlygeek/csapprox)。它可以令 GVim 专属的配色方案轻松地应用于 Vim 终端上。还有其他很多设置帮助我转移到 Vim 终端上工作，参考我的[.vimrc](https://github.com/s3rvac/dotfiles/tree/master/vim)文件。
Tmux+Vim 组合的最大优势是，它可以令你在一个终端就可以完成几乎所有的开发工作。例如，你可以通过 [ssh](https://en.wikipedia.org/wiki/Secure_Shell) 远程登录到一个服务器并在那里做开发，这样你也可以在不同地方甚至不同电脑上无缝对接之前的工作。另一个好处是，服务器的运行速度通常要比笔记本或台式电脑快。你甚至可能在一台拥有海量核芯与内存的服务器上做开发，这取决于你公司的预算。这样的体验简直会让人流连忘返。
如果你从未用过 Tmux，那我建议你至少试它一试。它真是一个很棒的工具。万事开头难，我在[这里](https://blog.petrzemek.net/2016/02/11/my-tmux-configuration/)详述了 Tmux 的配置过程，给你做个指南。仔细看看，或许你能从中学到一些技巧帮你调整并改善工作流程。例如，在该文中我介绍了一种在 Vim 与 Tmux 之间实现无缝导航的方法。
## 操作与行动
Vim 的一个独特之处是它能让你可以[通过一门语言](http://benmccormick.org/2014/07/02/learning-vim-in-2014-vim-as-language/)与之对话。例如，Vim 命令中有类似动词的 d（删除）或 y（复制），类似名词的 b（括号）或 s（句子），以及类似形容词的 i（内部的）或 a（全部的）。你可以把它们自由组合，因此，dib 代表删除括号内的所有文本，yap 代表复制当前的段落。在你学会这门简单语言之后，你不用再思考删除一个段落需要做什么，只需简单地输入 dap。很多人觉得 Vim 很难掌握，就是因为他们不习惯用这种方式来看待 Vim。
更难能可贵之处是 Vim 语言是可扩展的。事实上，用户可以在 Vim 语法的基础上创建自己的“动词”和“名词”（Vim 语法中的**操作**和**行动/文本对象**）。下面介绍一些我日常必备的好用插件。
### 操作
- [tcomment-vim](https://github.com/tomtom/tcomment_vim)——执行代码注释操作：gc。例如，为一个段落注释，可以用 gcap（go comment  a paragraph）；为当前行及以下 5 行注释，可以用 gc5j（感谢有相对行号！）。
- [vim-sort-motion](https://github.com/christoomey/vim-sort-motion)——执行排序操作：gs。例如，在 Python 中，对光标所在处的 imports 进行排序，可以用 gsip（go sort  inside paragraph）。
- [ReplaceWithRegister](https://github.com/vim-scripts/ReplaceWithRegister)——执行替换操作：gr。执行替换文本操作不会覆盖寄存器的内容。例如，要用默认的寄存器替换当前单词，可以用 griw（go replace inner word ）。gr 操作还可以与点号命令配合（.，重复上一次动作），这使它更加高效。它是我最喜欢的操作之一。
### 行动/文本对象
- [targets.vim](https://github.com/wellle/targets.vim)——添加许多有用的文本对象。例如，daa 用于删除一个函数调用的一个参数（deletes an argument of a function call），ci$ 用于改变美元符号之间的文本（在 [LaTeX](https://en.wikipedia.org/wiki/LaTeX) 中非常有用）。该插件的另一个特点是，即使光标未落在所欲编辑的文本对象之处，用户也可以通过该插件使用该文本对象。例如，用 di”（delete inside quotes）可以删除距离光标最近的双引号内的内容。
- [vim-indent-object](https://github.com/michaeljsmith/vim-indent-object)——对当前的缩进执行操作。例如，将当前代码块向左移动，可以用 <ii（left-shift inside indent）。这对 Python 来说很有用，因为它用空格标识代码块，而不是大括号。
- [vim-surround](https://github.com/tpope/vim-surround)——对周围环境操作。例如，cs”‘ 表示用单引号替换周围的双引号，而 dsb 则表示删除周围的括号。
当然，你也可以把在以上插件中提及的操作与行动结合起来使用。例如，在一个 Python 列表中每一项都在单独成行，为了对列表排序，可以使用 gsii（go sort inside indent）。
有些简单的文本对象甚至可以不借助插件来定义。例如，对一整个文件进行操作，可以把下列这行代码加入 .vimrc：

Vim
```
onoremap af :<C-u>normal! ggVG<CR>
```
于是，要复制整个文件的内容（更准确地讲，整个缓冲器），可以使用 yaf（yank a file）。
## 模糊查找
在 Vim 中打开一个文件，可以用 edit 或者 tabnew 命令，分别在当前缓冲器或新标签页中打开该文件。这种方式的一个缺点是，当你打开的是一个内嵌有很多目录及文件的大工程时，就会非常慢。例如，要编辑文件 frontend/scripts/js/view/viewer.js，你就得敲入整个路径。嗯，必须要有更好的方法……
幸运的是，确实有！通过使用模糊查找器，如 [Command-T](https://github.com/wincent/command-t)。现在只需要简单地启用查找器（输入 ,t），敲入 fsviewer（*f *与 *s *是文件路径的一部分），就可以在当前窗口或者新标签页中瞬间打开上述文件。一旦你知道自己工程的路径结构或文件名，使用模糊查找器可以极大地提高文件的打开速度。
## 检索
我们经常要列出所有包含给定短语或单词（如一个函数的名称）的文件。在我刚开始使用 Vim 时，我会转到终端然后输入 gvim `grep –r FUNC | cut –d: -f1`从而在 GVim 中打开找到的文件。然而，为何不把这个简单的任务交给 Vim 来做呢？
你既可以用 Vim 内置的 [grep](http://vim.wikia.com/wiki/Find_in_files_within_Vim) 命令，也可以用一款更高级的插件，如 [vim-grepper](https://github.com/mhinz/vim-grepper)，它能让用户选择一种检索工具（grep，git grep 或 [ag](https://github.com/ggreer/the_silver_searcher)）并选择怎样显示结果。例如，用户可以映射 ,/ 来在一个工程中检索所有包含某个单词的文件，或者用 ,∗ 在整个项目范围内检索包含光标所在单词的文件。在检索过程结束后，会弹出一个包含目标文件的窗口，Vim 用户可以直接打开这些文件。
## 作为外部编辑器的 Vim
如果你在使用一款文本编辑器，最重要的是尽可能经常使用它。毕竟，当你已经了解 Vim 了，为何还要去学另一款编辑器呢？例如网页上的 textarea forms？如果你在工作中处处使用 Vim，甚至还可以把它的优点发挥在之前想像不到的地方。
例如，在[火狐浏览器](https://www.mozilla.org/en/firefox/)中，有一个插件叫 [It’s All Text!](https://addons.mozilla.org/en/firefox/addon/its-all-text/)，该插件能让用户在 Vim 中编辑 textarea 元素。这为用户使用自己最爱的的编辑器编辑维基网页，写博文，提交 bug 报告等活动提供了便利。对 Vim 用户来说，这意味着他们可以尽情享受其语法高亮、[拼写检查](https://robots.thoughtbot.com/vim-spell-checking)、[字典补全](http://vim.wikia.com/wiki/Dictionary_completions)、[代码片段](https://github.com/SirVer/ultisnips)以及其他功能。
又如，在[雷鸟邮件客户端](https://www.mozilla.org/en/thunderbird/)中也有一个 [It’s All Text!](https://addons.mozilla.org/en/thunderbird/addon/its-all-text/versions/) 插件。比起内置的**编辑器**，使用该插件可以让你写邮件写得更快、更舒适。
很多终端应用也支持把 Vim 作为外部编辑器使用。例如，你可以把 [Midnight Commander](https://www.midnight-commander.org/)设置为从 Vim 打开文件。Vim 的另一个可贵之处表现在用[版本控制系统](https://en.wikipedia.org/wiki/Version_control)，如 [Git](https://git-scm.com/)，写提交说明的时候。你厌倦了排印错误吗？那就在 Vim 中设定拼写检查。你想通过使用 Ctrl+n 让出现在 diff 文件中的单词能自动补全吗？那就与 [-verbose](https://git-scm.com/docs/git-commit) 参数一起提交吧。它可以把整个 diff 文件复制到 Vim 中，因此你就可以使用字典补全功能，就像函数名那样。老板再也不用担心你写提交说明了。
最后，我要提的是另一款火狐浏览器插件，[Vimperator](https://addons.mozilla.org/en/firefox/addon/vimperator/)。它能让用户在浏览网页的时候也可以运用 Vim 的原则，使浏览器用起来像 Vim 。例如，不需要动鼠标，你就可以快速地在网页上打开连接、复制文本或者制作[书签](http://vim.wikia.com/wiki/Using_marks)稍后再返回。Vimperator 甚至能让你在一个类似 .vimrc 的文件中配置火狐浏览器。[这个](https://github.com/s3rvac/dotfiles/blob/master/vimperator/.vimperatorrc)是我的版本。
## 运行外部命令
假设你正在写 Pyhton 代码，并且你要在当前文件中执行单元测试。或者，假设你正在写 Latex 文档，为了查看结果你得进行编译。其中一个办法是打开一个终端，然后执行测试或进行编译。然而，这并不是一种高效的办法，因为它造成你工作流程的中断。
一种较好的方式是利用 Vim 能执行外部命令的特性。例如，利用下列的[自动命令](http://learnvimscriptthehardway.stevelosh.com/chapters/12.html)，你只需要简单的按下 F9 就可以为你的 Python 代码保存并执行当前编辑好的单元测试：

Vim
```
au FileType python nnoremap <buffer> <F9> :wa<CR>:!clear; nosetests %<CR>
```
在 Tmux 中运行 Vim 的另一个好处就是，使在 Tmux 窗格或窗口执行外部命令成为可能。例如，你可以将当前的 Tmux 窗口分为两个窗格，其中一个用于编辑文件，另一个用于执行测试，而不需要在它们之间切换。为了让你体验一下我的工作流程，下面我做个示范。我工作中使用了[双显示器](https://wiki.archlinux.org/index.php/multihead)，并做了相应设置。对于一个工程，我打开两个终端，分别在其中打开一个 Tmux 控制台，然后令每个显示器中分别显示一个终端。这时，我可以在第一个 Tmux 控制台上写代码，如果我想执行测试，生成文档，或用 [lint](https://en.wikipedia.org/wiki/Lint_(software)) 类似的工具检查代码，我只需通过 Vim 发送一条命令到第二个 Tmux 控制台，测试就会在第二个显示器中进行。这样，我就可以在第一个显示器的分割窗口中写代码和测试文档，在第二个显示器中运行 git 命令、执行测试等。我还对 [Fluxbox](http://fluxbox.org/)进行设置，从而可以用 Ctrl+Alt+h/l 在两个显示器之间切换。这是[我的配置文件](https://github.com/s3rvac/dotfiles/tree/master/fluxbox)。此外，还有更精彩的部分哦！那就是以上介绍的工作流程甚至适用于通过 SSH 登录到远程服务器运行 Tmux 的情况。
## 配置选项与映射
Vim 是高度可扩展的。你可以根据个人喜好对它进行配置，可以为你经常使用的动作添加映射和命令。在过去的时光中，我逐渐完善了[我自己的  .vimrc 文件](https://github.com/s3rvac/dotfiles/tree/master/vim)，其中包含了很多有用的设置、映射。以下是其中的一部分，不过，我建议你们看完我的整个 .vimrc 文件。

Vim
```
" Quickly select the text that was just pasted. This allows you to, e.g.,
" indent it after pasting.
noremap gV `[v`]
```


Vim
```
" Stay in visual mode when indenting. You will never have to run gv after
" performing an indentation.
vnoremap < <gv
vnoremap > >gv
```


Vim
```
" Make Y yank everything from the cursor to the end of the line. This makes Y
" act more like C or D because by default, Y yanks the current line (i.e. the
" same as yy).
noremap Y y$
```


Vim
```
" Make Ctrl-e jump to the end of the current line in the insert mode. This is
" handy when you are in the middle of a line and would like to go to its end
" without switching to the normal mode.
inoremap <C-e> <C-o>$
```


Vim
```
" Allows you to easily replace the current word and all its occurrences.
nnoremap <Leader>rc :%s/<<C-r><C-w>>/
vnoremap <Leader>rc y:%s/<C-r>"/
```


Vim
```
" Allows you to easily change the current word and all occurrences to something
" else. The difference between this and the previous mapping is that the mapping
" below pre-fills the current word for you to change.
nnoremap <Leader>cc :%s/<<C-r><C-w>>/<C-r><C-w>
vnoremap <Leader>cc y:%s/<C-r>"/<C-r>"
```


Vim
```
" Replace tabs with four spaces. Make sure that there is a tab character between
" the first pair of slashes when you copy this mapping into your .vimrc!
nnoremap <Leader>rts :%s/ /    /g<CR>
```


Vim
```
" Remove ANSI color escape codes for the edited file. This is handy when
" piping colored text into Vim.
nnoremap <Leader>rac :%s/<C-v><Esc>[(d{1,2}(;d{1,2}){0,2})?[m|K]//g<CR>
```
## 参数折叠
最后这点也很重要，我要提一下[vim-argwrap](https://github.com/FooSoft/vim-argwrap)插件。它可以让你快速折叠或展开函数参数（表）、列表或字典。例如，你可以用它把下列代码

Vim
```
decompiler = Decompiler(api_url=args.api_url, api_key=args.api_key)
```
转换为

Vim
```
decompiler = Decompiler(
    api_url=args.api_url,
    api_key=args.api_key
)
```
反之亦然，仅需要作一个简单映射。
