# Sublime Text2 默认语言（windows/unix）设置，Sublime插件大全 - 在思索中前行！ - CSDN博客





2014年09月27日 21:36:36[_Tham](https://me.csdn.net/txl16211)阅读数：11429








## Sublime默认系统语言设置




Sublime Text 2默认使用的就是UTF8，这个UTF8模式使用的是不带BOM的，如果要修改这个配置，到Perference->Settings-User->中添加："default_encoding": "UTF-8"，而后保存即可。UTF-8可以替换成你需要的编码格式。

通过samba，在win7上面编辑ubuntu下面的文件，保存的时候，有时候会发现文件被保存为dos格式的（如换行等），只能在ubuntu下面通过命令来一个一个转换。可不可以设置sublimetext的默认保存格式是unix的呢？

文件的格式控制可以Perference->Setting-*中找到。设置对象是default_line_ending，这个参数有三
 个可用选
项：system,windows,unix，system是根据当前系统情况设置，windows使用的CRLF，unix使用的是 LF。按你的情况，应该在Setting-User中设置"default_line_ending":"unix"就可以解决这个问题。



## Sublime 插件：C语言篇




如何安装插件详见：[https://packagecontrol.io/installation](https://packagecontrol.io/installation)
- [Alignment](http://wbond.net/sublime_packages/alignment)：选中并按`ctrl+alt+a`就可以使其按照等号对其。
![按等号对其，强迫症患者必备](http://upload-images.jianshu.io/upload_images/26219-b4dcc90c37386703.gif?imageView2/2/w/1240/q/100)

按等号对其，强迫症患者必备


- [C Improved](https://github.com/abusalimov/SublimeCImproved)：更加人性化的C语言着色方案。
![着色对比，还不错](http://upload-images.jianshu.io/upload_images/26219-1f7df1e499c496ff.png?imageView2/2/w/1240/q/100)

着色对比，还不错


![最好设置为默认用C Improved打开C文件](http://upload-images.jianshu.io/upload_images/26219-9c1425440dcbf67f.png?imageView2/2/w/1240/q/100)

最好设置为默认用C Improved打开C文件


- [CoolFormat](http://akof1314.github.io/CoolFormat/)：简单好用的代码格式化工具，相当于简化版的Astyle，默认`ctrl+alt+shift+q`格式化当前文件，`ctrl+alt+shift+s`格式化当前选中。
![默认选中按ctrl+alt+shift+s格式化当前](http://upload-images.jianshu.io/upload_images/26219-84e77c9279cea776.gif?imageView2/2/w/1240/q/100)

默认选中按`ctrl+alt+shift+s`格式化当前

**注**：格式的设置可以打开控制面板，输入CoolFormat : Formatter Setting，C/C++文档参考[此处](http://akof1314.github.io/CoolFormat/doc/Cpp.html)
![控制面板相关命令](http://upload-images.jianshu.io/upload_images/26219-608f1e8315a138a4.png?imageView2/2/w/1240/q/100)

控制面板相关命令


- [DocBlockr](https://github.com/spadgos/sublime-jsdocs)：自动生成大块的注释，并且可以用`tab`在不同内容之间切换，很爽的
![用tab在参数之间平滑切换](http://upload-images.jianshu.io/upload_images/26219-318ce98c56fdfe18.gif?imageView2/2/w/1240/q/100)

用`tab`在参数之间平滑切换


![当然，不在函数上面也可以比较方便的生成注释块](http://upload-images.jianshu.io/upload_images/26219-9a58baaa98020b35.gif?imageView2/2/w/1240/q/100)

当然，不在函数上面也可以比较方便的生成注释块

**注**：安装完重启一下，否则可能效果不理想，比如`tab`跳到别的地方去了
- [AllAutocomplete](https://github.com/alienhard/SublimeAllAutocomplete)：Sublime自带的可以对当前文件中的变量和函数名进行自动提示，但是AllAutocomplete可以对打开的所有文件的变量名进行提示，增强版的代码自动提示符。
> 
Extend Sublime autocompletion to find matches in all open files of the current window


- [CTags](https://github.com/SublimeText/CTags)：可以在函数的声明和定义自检来回跳转了，首先需要[下载Ctags](http://ctags.sourceforge.net/)，比如我存在D盘的根目录下，之后需要在Sublime中配置路径。
![Ctags路径设置](http://upload-images.jianshu.io/upload_images/26219-4f4e8d14aa88cd6d.png?imageView2/2/w/1240/q/100)

Ctags路径设置

然后在工程文件夹的右键生成索引文件，
![为工程文件夹内的文件生成索引](http://upload-images.jianshu.io/upload_images/26219-e470313552177d55.png?imageView2/2/w/1240/q/100)

为工程文件夹内的文件生成索引

然后就可以用`ctrl+shift+左键`跳转到定义处了，`ctrl+shift+右键`回来了（不过，还是没有Source Insight方便，可以实时小窗口预览）
![跳过来跳过去](http://upload-images.jianshu.io/upload_images/26219-b2de253cebf75665.gif?imageView2/2/w/1240/q/100)

跳过来跳过去


- [SublimeAStyleFormatter](http://theo.im/SublimeAStyleFormatter/)：国人做的Astyle Sublime版，蛮不错的。

安装完成之后，下面这个配置一定要打开，即保存自动格式化，这个相比于CoolFormat要简单很多。
![保存自动格式化配置](http://upload-images.jianshu.io/upload_images/26219-4823e065b8152fbd.png?imageView2/2/w/1240/q/100)

保存自动格式化配置


![按ctrl+s保存的同时自动格式化当前文件](http://upload-images.jianshu.io/upload_images/26219-1a1003423f2138c1.gif?imageView2/2/w/1240/q/100)

按`ctrl+s`保存的同时自动格式化当前文件


- [Cscope](https://github.com/ameyp/CscopeSublime)：TBC

参考:- [https://www.zybuluo.com/lanxinyuchs/note/33551](https://www.zybuluo.com/lanxinyuchs/note/33551)
- [http://hwchiu.logdown.com/posts/174922-sublime-text-3-cscope](http://hwchiu.logdown.com/posts/174922-sublime-text-3-cscope)
- 



# Sublime插件：Markdown篇





如何安装插件详见：[https://packagecontrol.io/installation](https://packagecontrol.io/installation)
- [MarkDown Editing](https://github.com/SublimeText-Markdown/MarkdownEditing)：支持Markdown语法高亮；支持Github Favored Markdown语法；自带3个主题。

![MarkDown Editing 界面](http://upload-images.jianshu.io/upload_images/26219-acb1458822ef63e8.png?imageView2/2/w/1240/q/100)

MarkDown Editing 界面


![主题选择](http://upload-images.jianshu.io/upload_images/26219-88170fc627d3078c.png?imageView2/2/w/1240/q/100)

主题选择


注：如果你安装完之后，遇到了如下的[错误](https://github.com/SublimeText-Markdown/MarkdownEditing/issues/115)，那么你安装的时候可能开着一个Markdown文件，所以卸载完之后在不打开Markdown的情况下再次安装就可以解决了。

![Markdown.tmLanguage错误](http://upload-images.jianshu.io/upload_images/26219-546efb46c571f42d.png?imageView2/2/w/1240/q/100)

Markdown.tmLanguage错误


- [MarkdownPreview](https://github.com/revolunet/sublimetext-markdown-preview)：按`CTRL + B`生成网页HTML；在最前面添加`[TOC]`自动生成目录；

![Markdown 生成HTML预览](http://upload-images.jianshu.io/upload_images/26219-ba148bf2ae66a82b.png?imageView2/2/w/1240/q/100)

Markdown 生成HTML预览


- [Markdown Extended](https://github.com/jonschlinkert/sublime-markdown-extended) +[Extends Monokai](https://github.com/jonschlinkert/sublime-monokai-extended)：不错的Markdown主题，支持对多种语言的高亮

![C语言语法高亮](http://upload-images.jianshu.io/upload_images/26219-1c131c4be3d76855.png?imageView2/2/w/1240/q/100)

C语言语法高亮


- [OmniMarkupPreviwer](http://theo.im/OmniMarkupPreviewer/)：实时在浏览器中预，而`MarkdownPreview`是需要手动生成的和F5的。览如果双屏的话，应该具有不错的体验。快捷键如下：
- `Ctrl+Alt+O`: Preview Markup in Browser.
- `Ctrl+Alt+X`: Export Markup as HTML.
- `Ctrl+Alt+C`: Copy Markup as HTML.

![实时在浏览器中显示编辑的文档](http://upload-images.jianshu.io/upload_images/26219-9a865e2acb4843cc.gif?imageView2/2/w/1240/q/100)

实时在浏览器中显示编辑的文档



- [TableEditor](https://github.com/vkocubinsky/SublimeTableEditor)：Markdown中的表格书写体验真心不咋样，所有有人为这个开发了一个插件，具有较好的自适应性，会自动对齐，强迫症患者喜欢。

首先需要用`ctrl + shift + p`打开这个功能（Table Editor: Enable for current syntax or Table Editor: Enable for current view or "Table Editor: Set table syntax ... for current view"），然后就可以狂用`tab`来自动完成了~~~

![用tab来自动完成表格间的切换和下一行表格的生成](http://upload-images.jianshu.io/upload_images/26219-256230846b591b50.gif?imageView2/2/w/1240/q/100)

用`tab`来自动完成表格间的切换和下一行表格的生成


- [Markdown TOC](https://github.com/naokazuterada/MarkdownTOC)：编辑MD文件的时候可以查看自动生成，并且可以控制生产目录的层次，不过不会自动跳转。编辑的时候可以看看，如果需要生成的HTML具有超链接跳转的功能，还是用MarkdownPreview吧。

![修改目录深度实时在MD文件中预览，不过这个用CTRL+R就可以产看，个人觉得不太实用](http://upload-images.jianshu.io/upload_images/26219-3fbd3982920df18a.gif?imageView2/2/w/1240/q/100)

修改目录深度实时在MD文件中预览，不过这个用`CTRL+R`就可以产看，个人觉得不太实用


- [SmartMarkdown](https://github.com/demon386/SmartMarkdown)：貌似是为Emacs用户打造的。

# **Sublime Text 系列**


- [Sublime Text：学习资源篇](http://www.jianshu.com/p/d1b9a64e2e37)
- [Sublime插件：增强篇](http://www.jianshu.com/p/5905f927d01b)
- [Sublime插件：Markdown篇](http://www.jianshu.com/p/aa30cc25c91b)
- [Sublime插件：C语言篇](http://www.jianshu.com/p/595975a2a5f3)






