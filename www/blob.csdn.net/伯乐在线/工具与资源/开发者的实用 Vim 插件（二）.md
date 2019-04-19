# 开发者的实用 Vim 插件（二） - 文章 - 伯乐在线
原文出处： [Ansh](https://www.howtoforge.com/tutorial/vim-editor-plugins-for-software-developers-2-syntastic/)   译文出处：[GHLandy](https://linux.cn/article-7909-1.html)
毫无疑问，Vim 是一个开箱即用并能够胜任编程任务的编辑器，但实际上是该编辑器中的插件帮你实现这些方便的功能。在 [开发者的实用 Vim 插件（一）](http://blog.jobbole.com/107412/)，我们已经讨论两个编程相关的 Vim 插件——标签侧边栏（Tagbar）和定界符自动补齐（delimitMate）。作为相同系列，我们在本文讨论另一个非常有用、专门为软件开发正定制的插件——语法高亮插件。
请注意：本教程中列举的所有例示、命令和说明都是在 Ubuntu 16.04 环境下进行测试的，并且，我们使用的 Vim 版本是 7.4。
### 语法高亮（Syntastic）插件
假如你的软件开发工作涉及到 C/C++ 语言，毫无疑问的说，遇到编译错误也是你每天工作中的一部分。很多时候，编译错误是由源代码之中的语法不正确造成的，因为开发者在浏览源码的时候很少能够一眼就看出所有这些错误。
那么 Vim 中是否存在一种插件可以让你不经编译源码就可以显示出语法错误呢？当然是有这样一种插件的，其名字就是 Syntastic。
“Syntastic 是 Vim 用来检验语法的插件，通过外部语法校验器校验文件并将错误呈现给用户。该过程可以在需要时进行，或者在文件保存的时候自动进行。”该插件 [官方文档](https://github.com/scrooloose/syntastic) 如是说。“如果检测到语法错误就会提示用户，因为不用编译代码或者执行脚本就可以知道语法错误，用户也就乐享与此了。”
安装过程和第一部分提到的方法类似，你只需要运行下列命令即可：


```
cd ~/.vim/bundle/
git clone https://github.com/scrooloose/syntastic.git
```
一旦你成功安装这个插件（即上述命令执行成功），你就不需要进行任何配置了——当 Vim 启动时会自动加载这个插件。
现在，打开一个源码文件并用 `:w` Vim 命令保存即可使用这个插件了。等待片刻之后，如果在源码中有语法错误的好，就会高亮显示出来。比如，看看一下截图你就会明白该插件是如何高亮显示语法错误的：
![Vim Syntax error highlighting](http://jbcdn2.b0.upaiyun.com/2016/11/786d420aaaaf9853f1fbc31ca90b1a1f.png)
在每行之前的 `>>` 表示该行中有语法错误。了解确切的错误或者想知道是什么东西错了，将光标移到该行——错误描述就会展示在 Vim 窗口的最底下。
![View Syntax errors in Vim](http://jbcdn2.b0.upaiyun.com/2016/11/bfb6ebd5cb5bb4a6b5a33dfb8164c29f.png)
这样，不用进行编译你就能够修复大多数语法相关的错误。
再往下，如果你运行 `:Errors` 命令，就会展现当前源文件中所有语法相关错误的描述。比如，我运行 `:Errors` 命令就是下图的效果：
![Syntastic :Errors command](http://jbcdn2.b0.upaiyun.com/2016/11/7a418a14e7d8f68c7ffcd879f7493424.png)
请记住，`:Errors` 展现的语法错误是不会自动更新的，这意味着在你修复错误之后，你需要重新运行 `:Errors` 命令，编辑器底部的错误描述才会消除。
值得一提的是，还有 [许多配置选项](https://github.com/scrooloose/syntastic/blob/master/doc/syntastic.txt) 能够使得 Syntastic 插件使用起来更加友好。比如，你可以在你的 `.vimrc` 中添加下列内容，然后 `:Errors` 就可以在修复错误之后自动更新它的底部描述。


```
let g:syntastic_always_populate_loc_list = 1
```
添加以下内容，以确保在你打开文件时 Syntastic 插件自动高亮显示错误。


```
let g:syntastic_check_on_open = 1
```
类似的，你也可以在保存或打开文件时让光标跳转到检测到的第一个问题处，将下列行放到你的 `.vimrc` 文件之中：


```
let g:syntastic_auto_jump = 1
```
这个值也可以指定为其它两个值： 2 和 3，其官方文档的解释如下：
“如果设置为 2 的话，光标就会跳到检测到的第一个问题，当然，只有这个问题是一个错误的时候才跳转；设置为 3 的话，如果存在错误，则会跳到第一个错误。所有检测到的问题都会有警告，但光标不会跳转。”
以下信息可能对你有帮助：
“使用 `:SyntasticCheck` 来手动检测错误。使用 `:Errors` 打开错误位置列表并使用 `:lclose` 来关闭。使用 `:SyntasticReset` 可以清除掉错误列表，使用 `:SyntasticToggleMode` 来切换激活（在写到 buffer 时检测）和被动（即手动检测）检测错误。”
注意：Syntastic 并不局限于 C/C++ 所写的代码，它同时也支持很多的编程语言——点击 [此处](https://github.com/scrooloose/syntastic) 了解更多相关信息。
### 结论
毫无疑问的，Syntastic 是一个非常有用的 Vim 插件，因为在出现语法相关错误时候，它至少能够让免去频繁编译的麻烦，而且不用说，同时也节约了你不少的时间。
正如你所看到的一样，配置好几个主要选项之后，Syntastic 变得非常好用了。为了帮助你了解这些设置，官方文档中包含了一份“推荐设置”——跟着文档进行设置即可。加入你遇到一些错误、有些疑问或者问题，你也可以查询一下 FAQ。
