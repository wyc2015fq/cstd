
# 让vim显示空格,及tab字符 - 嵌入式Linux - CSDN博客

2018年11月03日 21:16:17[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：165



## 1、显示 TAB 键
文件中有 TAB 键的时候，你是看不见的。要把它显示出来：
:set list
现在 TAB 键显示为 ^I，而 $显示在每行的结尾，以便你能找到可能会被你忽略的空白字符在哪里。
这样做的一个缺点是在有很多 TAB 的时候看起来很丑。如果你使用一个有颜色的终端，或者使用
GUI 模式，Vim 可以用高亮显示空格和TAB。
使用 ‘listchars’ 选项：
:set listchars=tab:>-,trail:-
现在，TAB会被显示成 ">—" 而行尾多余的空白字符显示成 "-"。看起来好多了，是吧？
————————————————————————————
## 2、让vim显示行尾的空格
fedora 9系统下
在/etc/vimrc文件添加如下两行
highlight WhitespaceEOL ctermbg=red guibg=red
match WhitespaceEOL /\s\+$/

