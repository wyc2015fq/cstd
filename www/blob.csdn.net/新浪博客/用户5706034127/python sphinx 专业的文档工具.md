# python sphinx 专业的文档工具_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
sphinx比较好的一个教程： [https://www.ibm.com/developerworks/cn/opensource/os-sphinx-documentation/](https://www.ibm.com/developerworks/cn/opensource/os-sphinx-documentation/)
安装完sphinx后，可以使用里面的exe程序了。如下所示
![python <wbr>sphinx <wbr>专业的文档工具](http://s15.sinaimg.cn/mw690/006e9W8vzy7b5MC4NfE8e&690)
在使用Python的过程中，无疑我们经常需要下载安装第三方模块（如何安装：URL），而安装后我们必然会面临一个使用的问题，这时候我们需要阅读相关的文档。可是我们会发现下载的第三方模块目录的doc文件夹下只有一个conf.py文件和一些后缀为rst的文件，文档在哪里呢？
　　其实Python提供了专用的文档生成工具，有了这些工具就可以很方便地生成我们需要的文档了。本文就是介绍如何使用一些第三方工具（sphinx）来生成这些文档。
　　1.下载并安装Docutils：[http://docutils.sf.net/](http://docutils.sf.net/)。
　　2.下载并安装sphinx ：[http://pypi.python.org/pypi/Sphinx/1.1](http://pypi.python.org/pypi/Sphinx/1.1)。Sphinx 是用来生成文档的，设计的初衷是为了生成python的文档。可以生成html、htmlhelp/chm、qthelp、devhelp、latex、man 等格式的文档。
　　3.在Widnows环境下，打开命令提示符，使用cd /d 命令将[当前目录](https://www.baidu.com/s?wd=%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nj0suHTdnWK-rj9bmWD0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtYrjb3njb1n10sPWcYnjmsPHc)切换至Sphinx目录，[运行命令](https://www.baidu.com/s?wd=%E8%BF%90%E8%A1%8C%E5%91%BD%E4%BB%A4&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nj0suHTdnWK-rj9bmWD0IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtYrjb3njb1n10sPWcYnjmsPHc)：python sphinx-build.py -b html sourcedir builddir，其中，-b后面的字符串用于控制输出文档的格式，默认为html。sourcedir表示文档来源文件夹（一般就是包含conf.py文件的那个文件夹），builddir表示生成文档的目标文件夹。(注：在Windows环境下，请不要将sphinx文件夹、sourcedir 和builddir 设置成包含中文的路径，否则容易出现一些默名其妙的错误，例如编码错误。)
# 使用sphinx记笔记[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#sphinx)
## sphinx简介[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id1)
简单来说，这是一个基于ReStructuredText的文档生成工具。方便易用，功能强大。
有很多开源工程都采用sphinx作为文档生成系统，最有名的就是 [python官方文档](http://docs.python.org/) 。在 [sphinx官方网站](http://sphinx.pocoo.org/)
上也列出使用sphinx的项目，有将近90个左右，其中不乏大名鼎鼎的开源项目。
一些中文的翻译项目也采用了sphinx，如 [pymotwcn](http://code.google.com/p/pymotwcn/) 。
## 安装[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id2)
- - 安装python
- python2.5或者python2.6都可以。
- 如果是windows平台，推荐下载 [ActivePython](http://www.activestate.com/activepython/downloads/)
- 其他平台可以直接下载 [python官方版本](http://www.python.org/download/)
- - 要确认已经安装了setuptools
- 如果已经安装，你在python安装路径下的Scripts文件夹下会找到一个easy_install.exe。
- [setuptools下载](http://pypi.python.org/pypi/setuptools#downloads)
- - 在命令行输入easy_install sphinx
- easy_install可以自动下载并安装sphinx以及它所依赖的其他模块。
## 建立sphinx工程[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id5)
建议使用sphinx自带的配置工具sphinx-quickstart。 - 建立一个工程目录，比如D:Note。 -
在该目录启动命令行，输入sphinx-quickstart
D:\Note>sphinx-quickstart
- 
程序会提示输入一些选项，如输入根目录
Welcome to the Sphinx quickstart utility.
Please enter values for the following settings (just press Enter to
accept a default value, if one is given in brackets).
Enter the root path for documentation.
> Root path for the documentation [.]:
大部分使用默认选项，直接按回车即可。
- - 需要指定的选项
- 
分离source和build目录，方便管理
> Separate source and build directories (y/N) [n]: y
- 
指定工程名、作者名、版本号
The project name will occur in several places in the built documentation.
> Project name: Note
> Author name(s): LK
Sphinx has the notion of a "version" and a "release" for the
software. Each version can have multiple releases. For example, for
Python the version is something like 2.5 or 3.0, while the release is
something like 2.5.1 or 3.0a1.  If you don't need this dual structure,
just set both to the same value.
> Project version: 0.1
> Project release [0.1]:
- 
文档文件的后缀名，默认是.rst，个人认为用.txt更方便些。
The file name suffix for source files. Commonly, this is either ".txt"
or ".rst".  Only files with this suffix are considered documents.
> Source file suffix [.rst]: .txt
- - 完成后，可以看到Note目录下有以下目录和文件
- build目录 运行make命令后，生成的文件都在这个目录里面
- source目录 放置文档的源文件
- make.bat 批处理命令
- makefile
- 
基本完成了，使用make html命令就可以生成html形式的文档了。
## 配置（conf.py）[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#conf-py)
conf.py文件包含了sphinx工程的所有配置选项，包括一些无法在sphinx-quickstart中进行设置的。
- 分为三部分：
- General configuration（一般选项）
- Options for HTML output（HTML输出选项）
- Options for LaTeX output（Latex输出选项）
下面是一些常用的选项：
- - language （语言）
对应于sphinx的locale目录下的文件夹，里面是本地化配置。
官方版本只支持繁体中文（zh_TW）,可以下载 [sphinx简体中文包](http://www.javaeye.com/wiki/topic/299363) （javaEye
topman制作）
下载后放到locale目录下，然后language选项修改为zh_CN即可
- 
html_theme （输出html的主题）:
# The theme to use for HTML and HTML Help pages.  Major themes that come with# Sphinx are currently 'default' and 'sphinxdoc'.html_theme='sphinxdoc'
## 常用的文档格式符号[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id7)
下面只是列出了一些常用的格式符号，以供大家参考，详细的教程可以参照《reStructuredText
简明教程》（以下基本上是从该教程直接引用过来的）。
### 标题[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id8)
ReStructuredText会根据下划线读取文档的标题，并且可以自动组织索引
> 
=====================
文档标题
=====================
--------
子标题
--------
章节标题
========
...
### 列表[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id9)
列表中，相同的层级使用相同的缩进。
列表中同一层级不需要空行分隔。不同层级起始处必须有空行。
列表：
  - 条目
  - 条目
      - 条目
      - 条目
  - 条目
### 超链接[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id10)
> 
**独立链接** ，自动将网址转换为链接。
例如 [http://www.ubuntu.org.cn/](http://www.ubuntu.org.cn/)
http://www.ubuntu.org.cn/
> 
**命名链接** ，为链接命名，有助记忆和减少空间占用。
在正文中使用 <链接名>_ ，注释中使用
_<链接名>:[链接目标]
例如 [Ubuntu](http://www.ubuntu.org.cn/)
> 
Ubuntu_
.. _Ubuntu:  http://www.ubuntu.org.cn/
### 代码[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id11)
sphinx对嵌入程序代码的支持很好（本来就是为了编写python文档而开发的工具）。
在段落的结尾添加符号 ::
，则表明下面的段落为代码段落。代码段落相对之前的段落要缩进一次。
普通文本段落，下一段是代码段落::
    缩进结束前
        代码段落不会被处理
 普通文本段落
### 文本[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id12)
只要没有空行，不管换多少次行，都会处理为一行。建议您将每行的内容控制在50个汉字或者100个字母之内，尽量在标点符号处手动换行，以增加源文件的可读性。
## 其他[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id13)
暂时没有发现支持ReStructuredText的Blog，不知道大家有没有知道的。如果能直接用ReStructuredText写Blog就太好了。
## 参考教程[¶](http://jwch.sdut.edu.cn/book/tool/UseSphinx.html#id14)
[A
ReStructuredText Primer](http://docutils.sourceforge.net/docs/user/rst/quickstart.html)
