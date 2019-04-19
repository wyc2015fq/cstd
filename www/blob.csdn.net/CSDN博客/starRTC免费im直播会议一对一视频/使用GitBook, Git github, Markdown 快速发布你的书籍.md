# 使用GitBook, Git  github, Markdown 快速发布你的书籍 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月12日 20:03:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1523
利用git作为版本管理和发布工具
你可以直接通过npm安装gitbook到全局
npm
install -g gitbook
gitbook只提供了如下四个命令
gitbook -h
Usage: gitbook
[options][command]
Commands:
build
[options][source_dir] 编译指定目录，输出Web格式(_book文件夹中)
serve
[options][source_dir] 监听文件变化并编译指定目录，同时会创建一个服务器用于预览Web
pdf
[options][source_dir] 编译指定目录，输出PDF
epub
[options][source_dir] 编译指定目录，输出epub
mobi
[options][source_dir] 编译指定目录，输出mobi
init
[source_dir] 通过SUMMARY.md生成作品目录
源文件目录一般是这样的.
- **LANGS.md**
当你需要发布多个语言版本时，根目录只需要放置一个LANGS.md, 格式如下
* [English](en)
* [中文](zh)
* ...
每个zh，en文件夹现在就相当于一个独立的书籍.
- README.md 相当于书籍的前言部分, 可以忽略
cover_small.png 和 cover.png书籍的封面图
**SUMMARY.md**
SUMMARY是最重要的一个部分,
 它创建的是整书的索引, 可以通过gitbook
 init读取SUMMARY.md来生成目录结构，格式如下
注意：*和-都是无序列表符号
* [前言](introduct/README.md)
- [API索引](introduct/index.md)
* [高级特性](advanced/README.md)
- [内嵌组件](advanced/component.md)
- [regular的transclude](advanced/content.md)
- [小节](advanced/review.md)
接下来就是依次完成你每个章节的书写了, 你需要开启gitbook
 serve .来进行实时的web预览(服务器默认为localhost:400)
现在的程序圈的markdown包括gitbook普遍使用的是[GitHub
 Flavored Markdown](https://help.github.com/articles/github-flavored-markdown)，除了github中已经说明的那些, 它还支持一些额外的小特性, 比如[x]可以用来设置一个checkbox来实现todolist的功能.
发布
可以直接使用git发布，首先你需要添加gitbook的仓库作为你的一个远程库.
 比如regularjs的路径为
git remote
add gitbook
https://push.gitbook.io/leeluolee/regular-guide.git
git
push gitbook master
可以发布到我的个人网站吗?
当然可以，gitbook build之后的_book 就是一个完整的web目录,
 你可以放置到你的个人网站上.
一个更好的做法是直接发布到github的gh-pages上,
 由于gitbook每次build都会重新生成整个目录.所以你需要利用gulp-gh-pages或grunt-gh-pages等工具进行发布.
你可以参考我的[做法](https://github.com/regularjs/guide/tree/master), 这样一键gulp
 deploy可以完成指定目录_book发布gh-pages.
[http://leeluolee.github.io/2014/07/22/2014-07-22-gitbook-guide/](http://leeluolee.github.io/2014/07/22/2014-07-22-gitbook-guide/)
最简单的方式就是使用GitBook编辑器
[https://www.gitbook.com/editor](https://www.gitbook.com/editor)
电子书出版：

**同步GitHub**
gitconfig--globalgitbook.userusernamegitconfig--globalgitbook.tokenyourtoken
**安装 nodejs**
首先我们需要做的是安装 [Nodejs](http://nodejs.cn/)。大家可以到 [Nodejs](http://nodejs.cn/) 的官网进行下载。下载完成后，执行双击进行运行安装。安装完成后，打开
 cmd 命令行，输入 node -v 查看安装的 nodejs 的相关版本信息。
nodejs 安装完成后，我们就可以开始安装 gitbook 了。但是在安装之前，我们还需要配置一下 nodejs 插件安装的下载镜像地址。因为默认的镜像地址是在国外，需要翻墙才可以访问，因此我们需要设置国内的镜像地址。国内的我推荐大家使用阿里巴巴的镜像地址 [http://registry.npm.taobao.org](http://registry.npm.taobao.org/) 。执行下面的命令，进行配置。
npm config
set registry
[http://registry.npm.taobao.org](http://registry.npm.taobao.org)
**全局安装 gitbook**
现在我们可以开始安装全局的 gitbook 了。执行 npm install gitbook-cli -g 命令，进行安装。安装的过程中，由于需要下载安装包，因此大家需要等待一定的时间，具体时长取决于你的网速和硬件配置。
安装完成后，我们可以执行 gitbook -v 查看安装的版本信息。
最常用的命令
gitbook init
//初始化目录文件gitbook
help//列出gitbook所有的命令gitbook --help//输出gitbook-cli的帮助信息gitbook build
//生成静态网页gitbook serve
//生成静态网页并运行服务器gitbook build --gitbook=2.0.1//生成时指定gitbook的版本, 本地没有会先下载gitbook
ls//列出本地所有的gitbook版本gitbook
ls-remote
//列出远程可用的gitbook版本gitbook fetch 标签/版本号
//安装对应的gitbook版本gitbook update
//更新到gitbook的最新版本gitbook uninstall
2.0.1//卸载对应的gitbook版本gitbook build --log=debug
//指定log的级别gitbook builid --debug
//输出错误信息
[http://blog.csdn.net/axi295309066/article/details/61420694](http://blog.csdn.net/axi295309066/article/details/61420694)
**常用插件**
[editlink](https://plugins.gitbook.com/plugin/editlink)
内容顶部显示 编辑本页 链接。
[github](https://plugins.gitbook.com/plugin/github)
在右上角显示 github 仓库的图标链接。
[disqus](https://plugins.gitbook.com/plugin/disqus)
添加 disqus 评论插件。
[github-buttons](https://plugins.gitbook.com/plugin/github-buttons)
显示 github 仓库的 star 和 fork 按钮。
[https://github.com/GitbookIO/gitbook](https://github.com/GitbookIO/gitbook)
[http://www.chengweiyang.cn/gitbook/installation/README.html](http://www.chengweiyang.cn/gitbook/installation/README.html)
用法非常简单，基本上就只有两步：
- 使用 gitbook init 初始化书籍目录
- 使用 gitbook serve 编译书籍
首先，创建如下目录结构：
README.md 和 SUMMARY.md 是两个必须文件，README.md 是对书籍的简单介绍：
$ cat book/README.md
# READMEThis is a book powered by [GitBook]([https://github.com/GitbookIO/gitbook).](https://github.com/GitbookIO/gitbook).)
SUMMARY.md 是书籍的目录结构。内容如下：
$ cat book/SUMMARY.md
# SUMMARY* [Chapter1](chapter1/README.md) * [Section1.1](chapter1/section1.1.md)
 * [Section1.2](chapter1/section1.2.md)*
 [Chapter2](chapter2/README.md)
创建了这两个文件后，使用 gitbook
 init，它会为我们创建 SUMMARY.md 中的目录结构。

GitBook使用
**在线编辑：**进入到书籍的属性页面后，点击 "Edit
 Book" 按钮即可打开在线编辑器。
另一种方式，是直接使用文本编辑器，编写 Markdown 文档，然后，使用 Git 提交到书籍的远程项目，当然，提交前，最好在本地使用 gitbook 预览效果；提交后，GitBook.com
 会自动生成更新书籍的内容。

