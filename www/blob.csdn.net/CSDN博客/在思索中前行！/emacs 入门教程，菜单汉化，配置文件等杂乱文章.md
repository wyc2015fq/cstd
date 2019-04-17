# emacs  入门教程，菜单汉化，配置文件等杂乱文章 - 在思索中前行！ - CSDN博客





2016年11月07日 12:57:03[_Tham](https://me.csdn.net/txl16211)阅读数：5036








## 首先来一发ArchWiki的Emacs简体中文的入门教程

https://wiki.archlinux.org/index.php/Emacs_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87)





## 怎样设置，Emacs中文菜单？


|把包内的3个文件丢到 emacs/share/emacs/site-lisp下面。在～/ 建一个 .emacs的文件，在里面添加一下内容，次序据说是不能颠倒：(require 'english-menu)(require 'chinese-menu)(require 'mule-menu)来源：http://forum.ubuntu.org.cn/viewtopic.php?t=193161|文件注释: X window下emacs的菜单汉化 。作者是：叶文彬。![](http://forum.ubuntu.org.cn/styles/ubuntu/imageset/icon_topic_attach.gif)[chinese_emacs_menu.tar.gz](http://forum.ubuntu.org.cn/download/file.php?id=60798&sid=d564a284a62c750fc23f8b4026450ed6) [49.09 KiB]||----||文件注释: X window下emacs的菜单汉化 。作者是：叶文彬。![](http://forum.ubuntu.org.cn/styles/ubuntu/imageset/icon_topic_attach.gif)[chinese_emacs_menu.tar.gz](http://forum.ubuntu.org.cn/download/file.php?id=60798&sid=d564a284a62c750fc23f8b4026450ed6) [49.09 KiB]|
|----|----|
|文件注释: X window下emacs的菜单汉化 。作者是：叶文彬。![](http://forum.ubuntu.org.cn/styles/ubuntu/imageset/icon_topic_attach.gif)[chinese_emacs_menu.tar.gz](http://forum.ubuntu.org.cn/download/file.php?id=60798&sid=d564a284a62c750fc23f8b4026450ed6) [49.09 KiB]| |


汉化方案2：

https://sourceforge.net/projects/emacslocale/files/
 下下载相应的emacs汉化包文件


按照对应的版本，把本汉化包下的
 emacs-xx.x 目录整个复制到原 Emacs 目录，合并到原来的文件夹即可，不需要额外调整注册表内容
*
 先发布 23.1 - 24.2 for Windows 汉化包， for Linux 还需进一步调试和细节调整，稍后发布；
*
 已编写翻译开发包模板，软件包暂定名为： emacs-locale-dev.deb, 可以参照模板进行其它语言，
如繁体中文，法语，德语等的翻译。调试完成后稍后一并发布；
*
 已编写 man 手册，用于指导 emacs-locale 的翻译注意事项。用法：
man
 emacs-transor 
稍后会包含在
 emacs-locale-dev.deb 中；
*
 基于翻译原始版本进行的脚本翻译，各版本译词统一；
*
 相对之前发布的汉化包，结构进行了优化；
*
 默认设置能完美支持gb2312/gbk/gb18030 (Emacs 22.3 及更早的版本，因自身原因，无法直接支持gb18030);


来源论坛：http://forum.ubuntu.org.cn/viewtopic.php?t=212467&f=17#p2743210




Emacs的个人配置文件：（与vim和sublime一样，Emacs是一款自由的可个人定制的编辑器）

http://emacser.com/dea.htm
 来源Emacs中文网。





## Emacs中的Color Theme以及字体设置

参考：http://www.cnblogs.com/Jerry-Chou/archive/2011/02/21/1960011.html





## [windows下配置gcc/g++/gdb + emacs编译环境只需两三步](http://blog.csdn.net/zhanxinhang/article/details/6622859)




步一、下载以下安装包并安装

> 

1、mingw-get-inst在线安装包下载地址：[http://sourceforge.net/projects/mingw/files/](http://sourceforge.net/projects/mingw/files/)


> 

2、emacs安装包下载地址：[http://www.gnu.org/software/emacs/](http://www.gnu.org/software/emacs/)


[](http://sourceforge.net/projects/mingw/files/)

[](http://www.gnu.org/software/emacs/)步二、设置环境变量


鼠标右键点击“我的电脑” -> 点击"属性" -> 选择"高级"选项卡 -> 点击"环境变量按钮" ->设置系统环境下变量名path的变量值 如下图

![](http://hi.csdn.net/attachment/201107/21/0_1311230006V80B.gif)



在变量值输入框里分别加入以下目录路径

> 

1、emacs安装目录下的bin文件夹路径 


> 

2、mingw安装目录下的bin文件夹路径


> 
注意：两个路径之间要以分号隔开，如：D:\work\tools\emacs-23.2\bin；D:\work\tools\MinGW\bin


设置成功后就可以使用emacs或dos下使用放在bin文件夹里的gcc/g++/gdb等工具了




![](http://hi.csdn.net/attachment/201107/22/0_13113394321dM6.gif)



## [emacs 快捷键（包括C／C++模式）](http://blog.csdn.net/wild_horse/article/details/2731595)

### 符号
C-  意思是按住 Ctrol 键
**M-**   意指 Meta 键 (键盘上若无Meta 键，则可以ALT ESC 键来取而代之)
**DEL**  意指退格键 (*不是* 删除(Delete) key)
**RET**  意指回车键
**SPC**  意指空格键
**ESC**  意指Escape键
**TAB**  意指Tab键
像 "C-M-" (or "M-C") 这样连在一起的意味着同时按住 Control 和 Meta 键不放.**用方向键**
   C-p 、 C-b 、 C-f 和 C-n 这四个命令。它们的功能和方向键是一样的，如下图所示：

            上一行 C-p (Prev line)
                          .
                          .
                          .                              
  向左移 C-b .... 。.... 向右移 C-f (Forward  character)
 (Backward )     . 
                          .
                          .
              下一行 C-n (Next line)

“P N B F”四个字母分别代表了四个词，用这四个词记忆这些组合键会更容易：
P 代表 previous（上一行），
N 代表 next（下一行），
B 代表 backward（回
退），
F 则代表 forward（前进）

**进入Emacs**
要进入GNU Emacs，只需键入它的名字             emacs
离开Emacs
挂起Emacs：                                    C-z
永久离开Emacs                                  C-x C-c

**文件**
读取文件到Emacs                                C-x C-f
保存文件到磁盘                                  C-x C-s
保存所有文件                                    C-x s
插入其它文件的内容到当前缓冲                    C-x i
用将要读取的文件替换当前文件                    C-x C-v
将当前缓冲写入指定的文件                        C-x C-w
Version control checkin/checkout                     C-x C-q

**取得帮助**
进入帮助系统很简单，只需要输入C-h（或F1）并跟随要获取帮助的对象，初次使用Emacs的用户可以输入C-h t进入使用手册
离开帮助窗口                                    C-x 1
滚动帮助窗口                                    C-M-v
匹配：显示与字符a串匹配的命令                   C-h a
显示一个键的功能                                C-h c
详细描述一个功能                                C-h f
取得详细模式的信息                              C-h m

**错误恢复**
取消当前要执行的命令                            C-g
恢复系统崩溃后丢失的文件                        M-x recover-file
撤销更新                                        C-x u或C-_
使缓冲回复到初始内容                            M-x revert-buffer
Redraw garbaged scree   n                         C-l

**增量查找(Incremental Search)**
向前查找                                        C-s
向后查找                                        C-r
规则表达式查找                                  C-M-s
反向规则表达式查找                              C-M-r
选择前一个查找字符串                            M-p
选择下一个查找字符串                            M-n
退出增量查找                                    RET
取消上一个字符的作用                            DEL(Backspace)
退出当前查找模式                                C-g
在查找的过程中可重复使用C-s和C-r来改变查找方向

**移动(Motion)**
向前一个字符                                    C-f
向后一个字符                                    C-b
向前一个字                                      M-f
向后一个字                                      M-b
向上一行                                        C-p
向下一行                                        C-n
到行首                                          C-a
到行尾                                          C-e
到句首                                          M-a
到句尾                                          M-e
到段首                                          M-{
到段尾                                          M-}
到页首                                          C-x [
到页尾                                          C-x ]
到表达式首部                                    C-M-f
到表达式尾部                                    C-M-b
到函数首部                                      C-M-a
到函数尾部                                      C-M-e
到缓冲首部                                      M-<
到缓冲尾部                                      M->
滚动到下一屏                                    C-v
滚动到上一屏                                    M-v
滚动到右边一屏（内容向左移动）                  C-x <
滚动到左边一屏（内容向右移动）                  C-x >
滚动当前行到屏幕中央                            C-u C-l

**Killing和Deleting**
向前delete字符                                  C-d
向后delete字符                                  DEL(Backspace)
向前delete到字首                                M-d
向后delete到字尾                                M-DEL(Backspace)
向前delete到行首                                M-0 C-k
向后delete到行尾                                C-k
向前delete到句首                                C-x DEL(Backspace)
向后delete到句尾                                M-k
向前delete到表达式首部                          M-- C-M-k
向后delete到表达式尾部                          C-M-k
Kill区域                                        C-w
拷贝区域到Kill Ring                              M-w
Kill到下一个给定字符出现的位置                  M-z <char>
拉回（yank）上次kill的内容                      C-y
用更早kill的内容取代拉回的上次kill的内容        M-y

**标记（Marking）**
标记当前位置                                    C-SPC或C-@
以字符为单位使用移动命令动态标记区域            C-x C-x
以字为单位使用移动命令动态标记区域              M-@
标记一段                                        M-h
标记一页                                        C-x C-p
标记一个表达式                                  C-M-@
标记一个函数                                    C-M-h
标记整个缓冲区                                  C-x h

**Query Replace**
交互式地替换一个文本串                          M-%
交互式地替换一个规则表达式                      M-x query-replace-regexp
替换当前的并移动到下一处                        SPE
替换当前的但不移动到下一处                      ,
不替换当前的并移动到下一处                      L(Backspace)
替换所有剩下的符合条件的文本                    !
退出替换模式                                    RET
进入递归的编辑模式                              C-r
退出递归的编辑模式                              C-M-c

**多窗口（Multiple Windows）**
（When two commands are shown，the second is for “other frame”）
删除所有其它窗口                                C-x 1
上下分割当前窗口                                C-x 2 C-x 5 2
左右分割当前窗口                                C-x 3
删除当前窗口                                    C-x 0 C-x 5 0
滚动其它窗口                                    C-M-v
切换光标到另一个窗口                            C-x o
选择另一个窗口中的缓冲                          C-x 4 b C-x 5 b
显示另一个窗口中的缓冲                          C-x 4 C-o C-x 5 C-o
在另一窗口中查找并打开文件                      C-x 4 f C-x 5 f
在另一窗口中以只读方式打开文件                  C-x 4 r C-x 5 r
在另一窗口中运行dired命令                       C-x 4 d C-x 5 d
在另一窗口中查找tag                             C-x 4 . C-x 5 .
增加窗口高度                                    C-x ^
减小窗口宽度                                    C-x {
增加窗口宽度                                    C-x }
格式（Formatting）
缩进当前行（与模式相关）                        TAB
缩进区域（与模式相关）                          C-M-
缩进表达式（与模式相关）                        C-M-q
Indent region rigidly arg. Columns                    C-x TAB
在光标后插入一个新的行                          C-o
静态地将一行下移                                C-M-o
删除光标附近的空行（留下一行）                  C-x C-o
与上一行合并成一行                              M-^
删除光标附近的所有空格                          M-
删除光标附近的空格（留下一格）                  M-SPC
Fill paragraph                                     M-q
Set fill column                                    C-x f
设置每一行开始的前缀                            C-x .
设置字体                                        M-g

**Case Change**
将一个字设置为大写                              M-u
将一个字设置为小写                              M-l
将一个字首字母设置为大写                        M-c
将一个区域设置为大写                            C-x C-u
将一个区域设置为小写                            C-x C-l

**The Minibuffer**
(the following keys are defined in the minibuffer)
最大程度地补全命令                              TAB
补全命令中的一个字                              SPC
完成并执行一个命令                              RET
列出命令所有可能的后续部分                      ?
列出在当前命令之前输入的命令                    M-p
列出在当前命令之后输入的命令                    M-n
用规则表达式在命令历史记录中向后搜寻            M-r
用规则表达式在命令历史记录中向前搜寻            M-s
重复执行上一条命令                              C-x ESC ESC

**缓冲（Buffer）**
选择另一个缓冲                                  C-x b
列出所有的缓冲                                  C-x C-b
Kill一个缓冲                                     C-x k

**置换（Transposing）**
字符置换                                        C-t
字置换                                          M-t
行置换                                          C-x C-t
表达式置换                                      C-M-t

**拼写检查（Spelling Check）**
对当前的字进行拼写检查                          M-$
检查区域内所有的字                              M-x ispell-origin
检查缓冲内所有的字                              M-x ispell-buffer

**标记 (Tags)**
查找标记                                        M-.
查找标记下一次出现的位置                        C-u M-.
指定一个新的标记文件                            M-x visit-tags-table
Regexp search on all files in tabs table                 M-x tags-search
在所有文件中执行查询-替换                       M-x tags-query-replace
继续进行上一次标记查找或查询-替换               M-,

**Shells**
执行一个shell命令                               M-！
在一个区域上执行sheel命令                       M-|
通过shell命令过滤区域                           C-u M-|
在窗口中启动一个shell                            M-x shell

**矩形（Rectangles）**
拷贝一个矩形到寄存器                            C-x r r
Kill矩形                                         C-x r k
拉回矩形                                        C-x r y
打开一个矩形, 将文本移动至右边                  C-x r o
清空矩形                                        C-x r c
为矩形中每一行加上一个字符串前缀                C-x r t

**规则表达式（Regular Expressions）**
除换行符外的所有单个字符                        .
零个或多个重复                                  *
一个以上的重复                                  +
零个或一个重复                                  ?
转译字符                                        
选择（or）                                      |
分组                                            (…)
与第n个组相同的文本                            n
At work break                                    b
Not at work break                                 B

**寄存器（Register）**
存储区域到寄存器                                C-x r s
插入矩形内容到缓冲                              C-x r i
存储光标位置到寄存器                            C-x r SPC <num>
跳跃到寄存器中存储的光标位置                    C-x r j <num>

**键盘宏（Keyboard Macros）**
开始定义一个键盘宏                              C-x (
结束键盘宏的定义                                C-x )
执行上一次定义的键盘宏                          C-x e
追加到上一个键盘宏                              C-u C-x (
为上一个键盘宏命名                              M-x name-last-kbd-macro
在缓冲中插入Lisp                                M-x insert-kbd-macro

**Tags**
    Tags 是一个显为人知的功能? 所以我想提一下. 这不是emacs发明的, 而是vi 原本的特异功能. emacs只是发扬光大而已.
    假设你有一个目录, 里面是一个程式的原始码, 比如说, tin 的原始码, 放在 ~/tin-1.3beta 下面. 你想看它们.
**    首先,** 叫emacs cd到该目录: 　M-x cd
**    然后, **建立tag table. tag table 就是一张对照表, 记录哪个符号(variable/function call) 对映到哪个档案的哪个地方. 有这张表, emacs可以让我们快速的在程式码内游走.
 一般这张表是一个档案, 叫作TAGS (大写)
　 M-! etags *.c
     M-! 是执行external shell command的意思. etags就是emacs的建表程式. 你只要告诉它你的source code在那里即可.
     vi的话是使用ctags这个程式, 它建出来的档名叫tags (小写). 因为我们介绍emacs, 所以不管它.
     然 后, 怎么看程式? 你知道所有的C 程式都是由main()开始, 所以你想找到main()在哪个档案. 这时只要按　M-. 然后emacs会问你tag table在哪里. 因为我们已经cd到该目录, 直接按enter就好了. 然后输入main, emacs就会把你带到main(){ ... }去.
     如果 你看到某个程式片断呼叫一个你没看过的函式, 你可以把游标移到该函式的名字上, M-. ENTER 就搞定了.
     如果 emacs找错了 (比如有变数和函式同名, emacs跳到变数去), 那你可以用 C-u M-. 找下一个.
     在编辑程式码的时候, M-SPC 很有用, 它会把游标附近的空白缩成一个. 在其它地方也有效.

**拼写检查**

当然只是针对英文。
Ispell
选中一块区域，或者对整个编辑缓冲区进行拼写检查: M-x ispell-buffer RET, 这时会打开ispell缓冲区，C-h可以查看一些拼写检查的帮助信息。
检查单词。在一个单词上执行M-$，会对这个单词进行拼写检查。
单词拼写补全。在一个未拼完的单词后执行ESC TAB(M-TAB)。
只要启用过Ispell, 他就将一直在后台运行。M-x ispell-kill-ispell，可以杀死这个进程。
感觉很好用。

**flyspell**

一个扩展，可以在编辑的时候直接进行拼写检查，也就是spell-check on the fly。它也是利用Ispell。
M-x flyspell-mode RET
Tips
改变buffer的只读属性
M-x toggle-read-only
在C模式下输入tab
C-q TAB : 对TAB不做解释，直接输
## ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

## C和C++模式



===**指定为C++模式的方法**===

一般根据扩展名自动设定，不用指定，不过有时候若希望.h文件是C++模式的（缺省是C模式），在文件第一行（或其末尾）上加入
<span style="font-size:12px;">// -*- C++ -*-
</span>
===**语法高亮**===

不是C模式专有，M-x global-font-lock-mode RET 或在.emacs中加入(global-font-lock-mode t)。

===**子模式**===

auto-state 输入时自动缩进，自动换行

hungry-state Backspace时，自动删除尽可能多的空白和空行

C-c C-t 同时转换(开/关)auto-state和hungry-state子模式

C-c C-a 转换 auto-state 子模式

C-c C-d 转换 hungry-state 子模式

===**编辑**命令****===

C-c . 设置缩进风格（按TAB键可列出可用的风格，缺省的为gnu，其缩进为2个字符；**linux**为8个；k&r为5个，java为4个）

TAB 重新缩进当前行

M-/ 自动补齐（缓冲区中能找得到的串）

M-; 行尾加入注释

C-c C-e 扩展宏

C-c C-c 注释掉整个区域

C-u C-c C-c 取消注释

C-c C-/ 将区域中的每一行结尾都加入一个'/'字符

M-x c-beginning-of-defun

M-x c-end-of-defun  跳到函数的开头或结尾

C-c C-u    跳转到前面的一个＃ifdef 


C-c C-p

C-c C-n   跳转到宏的开头或结尾

C-M-h   选中整个函数区域

C-c C-/   在所选区域的每一行后面添加" / "

C-M-p   跳转到}或）匹配的括号

C-M-n   跳转到（或{匹配的括号

M-x auto-insert 自动添加

   #ifndef TEST_H

   #define TEST_H

   #endif

M-@  把光标移动到单词开头，选中整个单词区域

C-M-h  选中当前行

C-u, C-@   回到上次标签的位置

===**编译和调试**===

M-x compile RET 编译

M-x gdb RET 调试

C-x ` （出错信息中）下一个错误，一个窗口显示错误信息，另一个显示源码的出错位置

C-c C-c 转到出错位置

启动gdb调试器后，光标在源码文件缓冲区中时：

C-x SPC 在当前行设置断点

C-x C-a C-s step

C-x C-a C-n next

C-x C-a C-t tbreak

C-x C-a C-r continue

## Dired模式

[参考文档](http://net.pku.edu.cn/~shenjian/documents/Dired_Mode.txt)

常用命令：

m : mark

u : unmark

d : mark delete

D : 立即删除

x : 执行删除

g : refresh

C : copy

R : move

+ : 创建目录

## Hideshow minor mode

在**编程**时可以隐藏函数的实现。M-x hs-minor-mode

(setq hs-minor-mode-prefix [(contrl o)]) 可以改变复杂的命令前缀.

用法：

`C-c @ C-h' : Hide the current block (`hs-hide-block').

`C-c @ C-s' : Show the current block (`hs-show-block').

`C-c @ C-c' : Either hide or show the current block (`hs-toggle-hiding')

`C-c @ C-M-h' : Hide all top-level blocks (`hs-hide-all').

`C-c @ C-M-s' : Show everything in the buffer (`hs-show-all').

## 十六进制模式

查看文本的十六进制编码

M-x hexl-mode






