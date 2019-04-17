# WebStorm使用技巧 - 在思索中前行！ - CSDN博客





2014年07月05日 18:26:35[_Tham](https://me.csdn.net/txl16211)阅读数：2967








WebStorm是一个捷克公司开发的，功能虽然很强大，但UI貌似一直不是东欧人的强项。WebStorm默认的编辑器颜色搭配不算讲究，我看习惯了VS2012的Dark Theme，再看这个顿觉由奢入俭难，非得改改颜色才行。



# 新建Scheme + 修改字体

ctrl+alt+s打开Settings界面，Editor > Colors&Fonts > Font。

Default scheme是亮色调，但我想定制一套暗背景的环境，所以在右侧Scheme name下拉框中选择Monokai，以此作为修改的基础，然后点击Save as，给自己新建的Scheme起个名字吧，如下图：

![font](http://images.cnitblog.com/blog/18034/201301/03000852-227e23de5715450884391cf33fa8ee00.png)

这里面可以设置字体（我一般选择Consolas），字体大小，行间距。



# 自定义语法高亮

用ctrl+alt+s打开Settings界面，Editor > Colors&Fonts。

![lang](http://images.cnitblog.com/blog/18034/201301/03000855-54430fa70ccb49abaebfb153fa40e605.png)

不同语言有各自的语法高亮规则，但有一部分是通用的（General），我们首先来设置General，然后再根据需要设置不同语言的语法高亮。

General的设置

普通文本：default text

折叠文本: Folded text

光标: Caret

光标所在行: Caret row

行号: Line number

TODO: TODO default

光标下变量高亮：Search result

搜索结果：Text search result

匹配的括号：Matched brace

不匹配的括号：Unmatched brace

未使用的符号：Unused symbol

左边空隙（行号，断点）：Gutter background

选中的文本背景色：Selection background

选中的文本前景色：Selection foreground



区分语言的设置

举例来说，如果要更改Javascript的语法高亮，就在上图中选择Javascript，然后再进行详细设置。具体设置项就不赘述了。



# 显示行号 + 自动换行

如果用Vim编程，行号是非常必要的辅助。WebStorm默认是不显示行号的，没关系，显示行号非常的容易。

只要在代码左侧的gutter区域点右键，就会出现下图的菜单：勾选上Show Line Numbers即可。

![line number](http://images.cnitblog.com/blog/18034/201301/03000856-3b73fd22afa84cc98c7cd46043413330.png)

另外，勾选上图中的Use Soft Wraps就启用了WebStorm的自动换行的功能，即过长的代码行不会超出屏幕，不会出现横向的滚动条。

值得一提的是，标准的Vim，jk是移到上一行下一行，如果代码自动换行了，被分割成的两行虽然看起来是两行，但其实是一个逻辑行，这时候用jk是上移下移一个逻辑行。这种情况下，如果要从同一逻辑行的“第一行”移动到“第二行”，需要按gj，即先按g再按方向。虽然Vim提供了这个方法，但还是经常按错。

好消息是，启用了Use Soft Wraps的WebStorm不需要这样，直接按j就可以从同一逻辑行的“第一行”移动到“第二行”。



# 导入导出设置

File > Export Settings导出用户设置。如下图，你可以选择要导出哪些类型的设置，也可以选择导出到哪个目录下。

![export](http://images.cnitblog.com/blog/18034/201301/03000858-6c587dffbb5c4e26be6d8902e1a7d0a1.png)

导出后的配置以jar包的形式存在，导入也同样简单，File > Import Settings，选择jar包导入成功后，重启WebStorm就大功告成了。

以下是我自定义的IDE配置，注意下载后需先解压缩成jar包

[http://files.cnblogs.com/dc10101/WebStormSettings.zip](http://files.cnblogs.com/dc10101/WebStormSettings.zip)



