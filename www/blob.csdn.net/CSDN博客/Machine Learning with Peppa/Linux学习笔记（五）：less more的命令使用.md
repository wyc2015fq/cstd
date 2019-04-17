# Linux学习笔记（五）：less|more的命令使用 - Machine Learning with Peppa - CSDN博客





2018年03月12日 21:50:26[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：89
所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)








more命令功能：让画面在显示满一页时暂停，此时可按空格健继续显示下一个画面，或按Q键停止显示。

less命令功能：less命令的用法与more命令类似，也可以用来浏览超过一页的文件。所不同的是less命令除了可以按空格键向下显示文件外，还可以利用上下键来卷动文件。当要结束浏览时，只要在less命令的提示符“: ”下按Q键即可。

该命令一次显示一屏文本，满屏后停下来，并且在屏幕的底部出现一个提示信息，给出至今己显示的该文件的百分比：--More--（XX%）可以用下列不同的方法对提示做出回答：
- 按Space键：显示文本的下一屏内容。
- 按Enier键：只显示文本的下一行内容。
- 按斜线符`|`：接着输入一个模式，可以在文本中寻找下一个相匹配的模式。
- 按H键：显示帮助屏，该屏上有相关的帮助信息。
- 按B键：显示上一屏内容。
- 按Q键：退出rnore命令。
　　使用权限：所有使用者 
　　使用方式：more [-dlfpc[su](http://www.linuxso.com/command/su.html)] [-num] [+/pattern] [+linenum] [[file](http://www.linuxso.com/command/file.html)Names..] 
**PS：与cat的区别在于cat 显示全部内容，more 分屏显示内容**


