# linux  VIM 下的语法高亮及自动缩进 - 在思索中前行！ - CSDN博客





2014年08月10日 19:53:41[_Tham](https://me.csdn.net/txl16211)阅读数：791标签：[Vim](https://so.csdn.net/so/search/s.do?q=Vim&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)









显示行号 set number

自动缩进有两个选项 set autoindent set cindent

autoindent 就是自动缩进的意思，当你在输入状态用回车键插入一个新行，或者在 normal 状态用 o 或者 O 插入一个新行时，autoindent 会自动地将当前行的缩进拷贝到新行，也就是”自动对齐”，当然了，如果你在新行没有输入任何字符，那么这个缩进将自动删除。

cindent 就不同了，它会按照 C 语言的语法，自动地调整缩进的长度，比如，当你输入了半条语句然后回车时，缩进会自动增加一个 TABSTOP 值，当你键入了一个右花括号时，会自动减少一个 TABSTOP 值。

![](https://www.byvoid.com/upload/wp/2008/11/vim_1.png)

在Vim中输入 :syntax enable 即可打开语法高亮。

![](https://www.byvoid.com/upload/wp/2008/11/vim_2.png)

被着色的文本读起来很费劲Vim会猜测你所使用的背景色. 如果背景是黑色的(或另一种比较暗的颜色)它就会用亮色来显示文字. 如果背景是白色(或另一种较亮的颜色)它就会暗色来显示文字. 如果Vim猜错了, 很可能就会读起来很碍眼. 你可以设置´background´选项来改变对比度, 比如使

用暗色: :set background=dark

使用亮色: :set background=light

![](https://www.byvoid.com/upload/wp/2008/11/vim_3.png)

![](https://www.byvoid.com/upload/wp/2008/11/vim_4.png)

不能识别文件类型Vim不可能识别所有的文件类型, 有时侯几乎无法得知一个文件用的是什么语言. 试一下这个命令: :set filetype

如果结果是“filetype=”问题很可能就是Vim不知道文件类型. 你可以手工指定该文件的类型:

:set filetype=fortran 要知道一共就有哪些文件类型可用, 请查看一下$VIMRUNTIME/syntax目录

![](https://www.byvoid.com/upload/wp/2008/11/vim_5.png)

推荐几个Vim手册

[](http://www.cbi.pku.edu.cn/chinese/documents/csdoc/cman/gvim.html)[http://www.cbi.pku.edu.cn/chinese/documents/csdoc/cman/gvim.html](http://www.cbi.pku.edu.cn/chinese/documents/csdoc/cman/gvim.html)

[](http://vimcdoc.sourceforge.net/doc/)[http://vimcdoc.sourceforge.net/doc/](http://vimcdoc.sourceforge.net/doc/)



